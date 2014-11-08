#include "mapcanvas.h"

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

#include "utils.h"

using namespace OsmAnd;
using namespace std;

const int referenceTileSize = 256;

MapCanvas::MapCanvas() : res(nullptr),
    m_mapStyle("default.render.xml"),
    m_mapStylePreset("type_general")
{
    renderer = createMapRenderer(MapRendererClass::AtlasMapRenderer_OpenGL2plus);

    auto conf = static_pointer_cast<AtlasMapRendererConfiguration>(renderer->getConfiguration());
    conf->referenceTileSizeOnScreenInPixels = referenceTileSize;
    conf->heixelsPerTileSide = 32;
    renderer->setConfiguration(conf);

    MapRendererSetupOptions setupOptions;
    setupOptions.gpuWorkerThreadEnabled = false;
    setupOptions.frameUpdateRequestCallback = [=] (auto) {
        if (this->window()) {
            this->window()->update();
        }
    };
    renderer->setup(setupOptions);

    renderer->setTarget(Utilities::convertLatLonTo31(LatLon(48, 16)));
    renderer->setZoom(16);
    renderer->setElevationAngle(90);
    renderer->setAzimuth(0);

    animator.reset(new MapAnimator);
    animator->setMapRenderer(renderer);

    connect(this, SIGNAL(windowChanged(QQuickWindow*)), this, SLOT(handleWindowChanged(QQuickWindow*)));
    connect(this, SIGNAL(mapStyleChanged(QString)), this, SLOT(setupLayers()));
    connect(this, SIGNAL(mapStylePresetChanged(QString)), this, SLOT(setupLayers()));

    renderer->stateChangeObservable.attach(this, [=] (auto, auto thisChange, auto allChanges) {
        switch (thisChange) {
        case MapRendererStateChange::Zoom: emit this->zoomChanged(this->zoom()); break;
        case MapRendererStateChange::Target: emit this->target31Changed(this->target31()); break;
        }
    });
}

MapCanvas::~MapCanvas() {
    setResources(nullptr);
}

void MapCanvas::setResources(Resources *resources) {
    if (res) {
        res->resourcesManager->localResourcesChangeObservable.detach(this);
    }

    res = resources;

    if (res) {
        setupLayers();

        res->resourcesManager->localResourcesChangeObservable.attach(this,
            [this] (auto, auto, auto, auto) { renderer->reloadEverything(); });
    }
}

float MapCanvas::zoom() const {
    return renderer->getState().requestedZoom;
}

QPoint MapCanvas::target31() const {
    return toQPoint(renderer->getState().target31);
}

QPoint MapCanvas::target31AtScreenPoint(const QPoint &screenPoint) {
    PointI targetAtScreenPt;
    renderer->getLocationFromScreenPoint(toPointI(screenPoint), targetAtScreenPt);
    return toQPoint(targetAtScreenPt);
}

void MapCanvas::setMapStyle(const QString &arg) {
    if (m_mapStyle != arg) {
        m_mapStyle = arg;
        emit mapStyleChanged(arg);
    }
}

void MapCanvas::setMapStylePreset(const QString &arg) {
    if (m_mapStylePreset != arg) {
        m_mapStylePreset = arg;
        emit mapStylePresetChanged(arg);
    }
}

void MapCanvas::setZoom(float zoom) {
    renderer->setZoom(zoom);

    if (window()) {
        window()->update();
    }
}

void MapCanvas::setTarget31(QPoint target) {
    renderer->setTarget(toPointI(target));

    if (window()) {
        window()->update();
    }
}

void MapCanvas::setTarget31AtScreenPoint(QPoint target, QPoint screenPoint) {
    // FIXME: can this even work with elevation != 90 ?
    setTarget31(target31() + (target - target31AtScreenPoint(screenPoint)));
}

void MapCanvas::sync() {
    if (!renderer->isRenderingInitialized()) {
        renderer->initializeRendering();

        connect(window(), SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
    }

    PointI windowSize = toPointI(window()->size());
    renderer->setWindowSize(windowSize);
    renderer->setViewport(AreaI(PointI(), windowSize));
}

void MapCanvas::cleanup() {
    renderer->releaseRendering();
    disconnect(window(), SIGNAL(beforeRendering()), this, SLOT(paint()));
}

void MapCanvas::handleWindowChanged(QQuickWindow *win) {
    if (win) {
        connect(win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
        connect(win, SIGNAL(sceneGraphInvalidated()), this, SLOT(cleanup()), Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }}

void MapCanvas::paint() {
    renderer->update();
    if (renderer->prepareFrame()) {
        renderer->renderFrame();
    }
}

void MapCanvas::setupLayers() {
    if (!resources()) return;

    auto mapSourceResource = resources()->resourcesManager->getResource(mapStyle());
    auto& unresolvedMapStyle =
            static_pointer_cast<const ResourcesManager::MapStyleMetadata>(mapSourceResource->metadata)
            ->mapStyle;
    auto resolvedMapStyle = resources()->resourcesManager->
            mapStylesCollection->getResolvedStyleByName(unresolvedMapStyle->name);

    binaryMapDataProvider.reset(new BinaryMapDataProvider(resources()->resourcesManager->obfsCollection));
    float displayDensityFactor = 1;
    mapPresentationEnvironment.reset(new MapPresentationEnvironment(resolvedMapStyle, displayDensityFactor));
    primitiviser.reset(new Primitiviser(mapPresentationEnvironment));
    binaryMapPrimitivesProvider.reset(new BinaryMapPrimitivesProvider(
                                          binaryMapDataProvider, primitiviser, referenceTileSize));

    auto preset = resources()->resourcesManager->
            mapStylesPresetsCollection->getPreset(unresolvedMapStyle->name, mapStylePreset());
    if (preset) {
        mapPresentationEnvironment->setSettings(preset->attributes);
    }

    binaryMapStaticSymbolsProvider.reset(
                new BinaryMapStaticSymbolsProvider(
                    binaryMapPrimitivesProvider, referenceTileSize));
    renderer->addSymbolsProvider(binaryMapStaticSymbolsProvider);

    rasterMapProvider.reset(new BinaryMapRasterLayerProvider_GPU(binaryMapPrimitivesProvider));
    renderer->setMapLayerProvider(0, rasterMapProvider);
}
