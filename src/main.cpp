#include <OsmAndCore/Logging.h>
#include <OsmAndCore/CoreResourcesEmbeddedBundle.h>
#include <OsmAndCore.h>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtQml>
//#include <QQuickWindow>
#include <QtQuick/QQuickView>

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
  qmlRegisterUncreatableType<Resources>("OsmAndQt", 1, 0, "Resources", "");
  qmlRegisterUncreatableType<ResourceModel>("OsmAndQt", 1, 0, "ResourceModel", "");

  Resources resources;
  resources.downloadIfNecessary(QList<QString>()
              << "world_basemap.map.obf"
              << "austria_europe.map.obf"
              );

  QQuickView view;
  QQmlContext *context = view.rootContext();
  context->setContextProperty("contextResources", &resources);
  view.setResizeMode(QQuickView::SizeRootObjectToView);
  view.setSource(QUrl("qrc:/src/Main.qml"));
  view.show();


  return app.exec();
}
