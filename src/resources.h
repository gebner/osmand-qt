#pragma once

#include <OsmAndCore/ResourcesManager.h>
#include <QObject>
#include <memory>

class ResourceModel;

class Resources: public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> resourcesInRepository READ resourcesInRepository NOTIFY repositoryUpdated)

public:
    Resources(QObject *parent = 0);
    virtual ~Resources();

    std::shared_ptr<OsmAnd::ResourcesManager> resourcesManager;

    QList<QObject *> resourcesInRepository();

    void downloadIfNecessary(const QList<QString>& resourceIds);

signals:
    void repositoryUpdated();
    void localResourceChanged(QString);

public slots:
};

class ResourceModel: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY resourceChanged)
    Q_PROPERTY(uint64_t size READ size NOTIFY resourceChanged)
    Q_PROPERTY(QDateTime timestamp READ timestamp NOTIFY resourceChanged)

public:
    ResourceModel(Resources *res, const std::shared_ptr<const OsmAnd::ResourcesManager::ResourceInRepository>& resourceInRepo);
    virtual ~ResourceModel();

    QString id() const { return resourceInRepo->id; }
    uint64_t size() const { return resourceInRepo->size; }
    QDateTime timestamp() const {
        QDateTime dateTime;
        dateTime.setMSecsSinceEpoch(resourceInRepo->timestamp);
        return dateTime;
    }

signals:
    void resourceChanged();

private:
    Resources *res;
    std::shared_ptr<const OsmAnd::ResourcesManager::ResourceInRepository> resourceInRepo;
};
