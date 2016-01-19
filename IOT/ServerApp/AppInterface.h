#pragma once

#include <QObject>
#include <QQmlEngine>

class QQmlEngine;
class CQmlServerApp;
class DeviceHandle;
class DevicesInfoList;
class DeviceCmdList;
class EventSender;

class CAppInterface : public QObject
{
    Q_OBJECT;

public:
    CAppInterface( QObject* pParent = 0 );
    virtual ~CAppInterface();

    bool Initialize( QQmlEngine* pEngine );
    void CreateServerApp();

private:
    QQmlEngine*      m_pEngine;
    CQmlServerApp*   m_pServerApp;
    DeviceHandle*    m_kDevicesIfo;
    DevicesInfoList* m_DevicesInfoListModel;
    DeviceCmdList*   m_DeviceCmdListModel;
    EventSender*    m_pSender;
};