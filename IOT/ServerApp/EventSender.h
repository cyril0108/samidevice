#pragma once

#include <QObject>

class EventSender : public QObject
{
    Q_OBJECT

public:
    EventSender(QObject* pParent = 0);
    virtual ~EventSender();

public:
    void Connect(EventSender* pToSender);
    void Disconnect(EventSender* pToSender);

    // 如果有新增修改signals的東西, 記得也要去修改Connect()及Disconnect()
Q_SIGNALS:
    // editing signal
    void commandReturned(QString kText);
};
