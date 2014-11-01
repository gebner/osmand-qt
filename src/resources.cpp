#include "resources.h"

#include <QStandardPaths>

using namespace OsmAnd;

Resources::Resources(QObject *parent) : QObject(parent) {
    resourcesManager.reset(new ResourcesManager(
                               QStandardPaths::writableLocation(QStandardPaths::DataLocation)));
}

Resources::~Resources() {}

void Resources::makeSureWorldMapIsInstalled() {
    QString worldMapResourceId = "world_basemap.map.obf";
    if (!resourcesManager->isResourceInstalled(worldMapResourceId)) {
      resourcesManager->updateRepository();
      resourcesManager->installFromRepository(worldMapResourceId);
    }
}
