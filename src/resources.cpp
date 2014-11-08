#include "resources.h"

#include <QStandardPaths>

using namespace OsmAnd;

Resources::Resources(QObject *parent) : QObject(parent) {
    resourcesManager.reset(new ResourcesManager(
                               QStandardPaths::writableLocation(QStandardPaths::DataLocation)));
}

Resources::~Resources() {}

void Resources::downloadIfNecessary(const QList<QString>& resourceIds) {
    if (!resourcesManager->isRepositoryAvailable()) {
        resourcesManager->updateRepository();
    }

    for (auto& resourceId : resourceIds) {
        if (!resourcesManager->isResourceInstalled(resourceId)) {
            resourcesManager->installFromRepository(resourceId);
        }
    }
}
