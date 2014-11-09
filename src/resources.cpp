#include "resources.h"

#include <QStandardPaths>

using namespace std;
using namespace OsmAnd;

Resources::Resources(QObject *parent) : QObject(parent) {
    resourcesManager.reset(new ResourcesManager(
                               QStandardPaths::writableLocation(QStandardPaths::DataLocation)));

    resourcesManager->localResourcesChangeObservable
            .attach(this, [=] (auto, auto added, auto removed, auto updated) {
       for (QString id: added) emit this->localResourceChanged(id);
       for (QString id: removed) emit this->localResourceChanged(id);
       for (QString id: updated) emit this->localResourceChanged(id);
    });
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

QList<QObject *> Resources::resourcesInRepository() {
    QList<QObject *> models;
    for (auto resInRepo: resourcesManager->getResourcesInRepository().values()) {
        models << new ResourceModel(this, resInRepo);
    }
    return models;
}

ResourceModel::ResourceModel(Resources *res, const shared_ptr<const ResourcesManager::ResourceInRepository>& resInRepo)
    : QObject(res), res(res), resourceInRepo(resInRepo)
{
    connect(res, &Resources::localResourceChanged, this, [=] (QString changedId) {
        if (id() == changedId) {
            // TODO: necessary?
            resourceInRepo = res->resourcesManager->getResourceInRepository(changedId);
            emit resourceChanged();
        }
    });
    connect(res, &Resources::repositoryUpdated, this, &ResourceModel::resourceChanged);
}

ResourceModel::~ResourceModel() {}
