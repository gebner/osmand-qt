#include "map.h"

#include <QQuickWindow>
#include <OsmAndCore/Map/IMapRenderer.h>
#include <OsmAndCore/Map/MapAnimator.h>
#include <OsmAndCore/Map/UnresolvedMapStyle.h>
#include <OsmAndCore/Map/MapStylesCollection.h>
#include <OsmAndCore/Map/MapStylesPresetsCollection.h>
#include <OsmAndCore/Map/MapStylePreset.h>
#include <OsmAndCore/Map/BinaryMapRasterLayerProvider_GPU.h>
#include <OsmAndCore/Map/BinaryMapRasterLayerProvider_Software.h>
#include <OsmAndCore/Map/MapPresentationEnvironment.h>
#include <OsmAndCore/Map/BinaryMapStaticSymbolsProvider.h>
#include <OsmAndCore/Map/AtlasMapRendererConfiguration.h>
#include <OsmAndCore/ObfsCollection.h>
#include <OsmAndCore/Utilities.h>

using namespace OsmAnd;
using namespace std;

Map::Map() : res(nullptr) {
    setFlag(ItemHasContents); // enable update()

    renderer = createMapRenderer(MapRendererClass::AtlasMapRenderer_OpenGL2plus);

    auto conf = static_pointer_cast<AtlasMapRendererConfiguration>(renderer->getConfiguration());
    conf->referenceTileSizeOnScreenInPixels = referenceTileSize;
    renderer->setConfiguration(conf);

    MapRendererSetupOptions setupOptions;
    setupOptions.gpuWorkerThreadEnabled = false;
    setupOptions.frameUpdateRequestCallback = [=] (auto) { this->update(); };
    renderer->setup(setupOptions);

    animator.reset(new MapAnimator);
    animator->setMapRenderer(renderer);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
}

Map::~Map() {
    setResources(nullptr);
}

void Map::setResources(Resources *resources) {
    if (res) {
        res->resourcesManager->localResourcesChangeObservable.detach(this);
    }

    res = resources;

    if (res) {
        res->resourcesManager->localResourcesChangeObservable.attach(this,
            [this] (auto, auto, auto, auto) { renderer->reloadEverything(); });

        updateMapSource();
    }
}

void Map::sync() {
    if (!renderer->isRenderingInitialized()) {
        renderer->initializeRendering();
    }

    PointI windowSize(window()->size().width(), window()->size().height());
    renderer->setWindowSize(windowSize);
    renderer->setViewport(AreaI(PointI(), windowSize));

    connect(window(), SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
}

void Map::cleanup() {
    renderer->releaseRendering();
}

void Map::handleWindowChanged(QQuickWindow *win) {
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void Map::paint() {
    renderer->update();
    renderer->prepareFrame();
    renderer->renderFrame();
}

void Map::updateMapSource() {
    QString resourceId = "default.render.xml", variant = "type_general";

    auto mapSourceResource = resources()->resourcesManager->getResource(resourceId);
    auto& unresolvedMapStyle =
            static_pointer_cast<const ResourcesManager::MapStyleMetadata>(mapSourceResource->metadata)
            ->mapStyle;
    auto resolvedMapStyle = resources()->resourcesManager->
            mapStylesCollection->getResolvedStyleByName(unresolvedMapStyle->name);

    binaryMapDataProvider.reset(new BinaryMapDataProvider(resources()->resourcesManager->obfsCollection));
    mapPresentationEnvironment.reset(new MapPresentationEnvironment(resolvedMapStyle));
    primitiviser.reset(new Primitiviser(mapPresentationEnvironment));
    binaryMapPrimitivesProvider.reset(new BinaryMapPrimitivesProvider(
                                          binaryMapDataProvider, primitiviser, referenceTileSize));

    mapPresentationEnvironment->setSettings(resources()->resourcesManager->
        mapStylesPresetsCollection->getPreset(unresolvedMapStyle->name, variant)->attributes);

    binaryMapStaticSymbolsProvider.reset(
                new BinaryMapStaticSymbolsProvider(
                    binaryMapPrimitivesProvider, referenceTileSize));
    renderer->addSymbolsProvider(binaryMapStaticSymbolsProvider);

    rasterMapProvider.reset(new BinaryMapRasterLayerProvider_GPU(binaryMapPrimitivesProvider));
    renderer->setMapLayerProvider(0, rasterMapProvider);

    renderer->setTarget(Utilities::convertLatLonTo31(LatLon(52.22, 4.42)));
    renderer->setZoom(14);
    renderer->setElevationAngle(90);
    renderer->setAzimuth(0);
}
