#pragma once

#include <OsmAndCore.h>
#include <QQuickItem>
#include <memory>
#include "resources.h"

namespace OsmAnd {
    class IMapRenderer;
    class MapAnimator;
    class IMapLayerProvider;
    class BinaryMapDataProvider;
    class MapPresentationEnvironment;
    class Primitiviser;
    class BinaryMapPrimitivesProvider;
    class BinaryMapStaticSymbolsProvider;
}

class Map: public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Resources* resources READ resources WRITE setResources)

public:
    Map();
    virtual ~Map();

    void setResources(Resources *resources);
    Resources *resources() { return res; }

public slots:
    void updateMapSource();

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void paint();

    void sync();
    void cleanup();

private:
    const int referenceTileSize = 256;

    std::shared_ptr<OsmAnd::IMapRenderer> renderer;
    std::shared_ptr<OsmAnd::MapAnimator> animator;

    // Current provider of raster map
    std::shared_ptr<OsmAnd::IMapLayerProvider> rasterMapProvider;

    // Offline-specific providers & resources
    std::shared_ptr<OsmAnd::BinaryMapDataProvider> binaryMapDataProvider;
    std::shared_ptr<OsmAnd::MapPresentationEnvironment> mapPresentationEnvironment;
    std::shared_ptr<OsmAnd::Primitiviser> primitiviser;
    std::shared_ptr<OsmAnd::BinaryMapPrimitivesProvider> binaryMapPrimitivesProvider;
    std::shared_ptr<OsmAnd::BinaryMapStaticSymbolsProvider> binaryMapStaticSymbolsProvider;

    Resources *res;
};
