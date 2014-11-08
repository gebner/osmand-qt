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

class MapCanvas: public QQuickItem {
    Q_OBJECT
    Q_PROPERTY(Resources* resources READ resources WRITE setResources)
    Q_PROPERTY(QString mapStyle READ mapStyle WRITE setMapStyle NOTIFY mapStyleChanged)
    Q_PROPERTY(QString mapStylePreset READ mapStylePreset WRITE setMapStylePreset NOTIFY mapStylePresetChanged)
    Q_PROPERTY(float zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(QPoint target31 READ target31 WRITE setTarget31 NOTIFY target31Changed)

public:
    MapCanvas();
    virtual ~MapCanvas();

    void setResources(Resources *resources);
    Resources *resources() { return res; }

    QString mapStyle() const { return m_mapStyle; }
    QString mapStylePreset() const { return m_mapStylePreset; }

    float zoom() const;
    QPoint target31() const;
    Q_INVOKABLE QPoint target31AtScreenPoint(const QPoint& screenPoint);

public slots:
    void setMapStyle(const QString& mapStyleResourceId);
    void setMapStylePreset(const QString& presetResourceId);
    void setZoom(float zoom);
    void setTarget31(QPoint target);
    void setTarget31AtScreenPoint(QPoint target, QPoint screenPoint);

signals:
    void mapStyleChanged(QString arg);
    void mapStylePresetChanged(QString arg);
    void zoomChanged(float arg);
    void target31Changed(QPoint arg);

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void paint();

    void sync();
    void cleanup();

    void setupLayers();

private:
    Resources *res;
    QString m_mapStyle, m_mapStylePreset;

    // Rendering part
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
};
