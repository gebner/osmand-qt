#pragma once

#include <OsmAndCore/ResourcesManager.h>
#include <QObject>
#include <memory>

class Resources: public QObject {
    Q_OBJECT

public:
    Resources(QObject *parent = 0);
    virtual ~Resources();

    std::shared_ptr<OsmAnd::ResourcesManager> resourcesManager;

    void makeSureWorldMapIsInstalled();

signals:


public slots:
};
