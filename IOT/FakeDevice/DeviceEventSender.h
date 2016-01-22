#pragma once

#include <QObject>

class DeviceEventSender : public QObject
{
    Q_OBJECT

public:
    DeviceEventSender(QObject* pParent = 0);
    virtual ~DeviceEventSender();

public:
    void Connect(DeviceEventSender* pToSender);
    void Disconnect(DeviceEventSender* pToSender);

    // 如果有新增修改signals的東西, 記得也要去修改Connect()及Disconnect()
Q_SIGNALS:

    // editing signal
    void commandReturned(QString kText, bool bClear);
};
