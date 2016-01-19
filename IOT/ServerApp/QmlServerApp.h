#pragma once

#include <QObject>
#include <QVariant>
#include "DeviceHandle.h"

class CQmlServerApp : public QObject
{
    Q_OBJECT

public:
    CQmlServerApp(QObject* pParent = 0);
    virtual ~CQmlServerApp();

public Q_SLOTS:
    void deviceAdded(QString uid);
    void displayInfoRecieved(QString uid, QVariantMap retMap);
    bool sendCommandToDevice(const QString& deviceUID, const QString& cmdKey, const QString& value);
    void queryDeviceInfo(QString deviceUID, QString& iP, QString& Name);
};
