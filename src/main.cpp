#include <OsmAndCore/Logging.h>
#include <OsmAndCore/CoreResourcesEmbeddedBundle.h>
#include <OsmAndCore.h>

#include <QDebug>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "mapcanvas.h"
#include "resources.h"

using namespace OsmAnd;
using namespace std;

extern "C" void __CoreResourcesEmbeddedBundle__FakeReferences();

int main(int argc, char *argv[]) {
  Q_INIT_RESOURCE(resources);

  QGuiApplication app(argc, argv);
  app.setApplicationName("osmand-qt");
  app.setApplicationVersion("0.1");

  __CoreResourcesEmbeddedBundle__FakeReferences();
  shared_ptr<const CoreResourcesEmbeddedBundle> bundle =
    CoreResourcesEmbeddedBundle::loadFromCurrentExecutable();
  InitializeCore(bundle);

  qmlRegisterType<MapCanvas>("OsmAndQt", 1, 0, "MapCanvas");
  qmlRegisterType<Resources>("OsmAndQt", 1, 0, "Resources");

  Resources resources;
  resources.downloadIfNecessary(QList<QString>()
              << "world_basemap.map.obf"
              << "austria_europe.map.obf"
              );

  QQuickView view;
  view.rootContext()->setContextProperty("contextResources", &resources);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:/src/Map.qml"));
  view.show();

  return app.exec();
}
