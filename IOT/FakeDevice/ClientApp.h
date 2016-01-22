#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QVariantMap>

class QQmlEngine;

class FakeDeviceHandle;
class FakeDevicesInfoList;
class FakeDeviceCmdList;
class DeviceEventSender;

class QUdpSocket;
class QTcpServer;

/*
class CQmlPair : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString mapKey MEMBER mapKey)
    Q_PROPERTY(QString mapValue MEMBER mapValue)
public:
    CQmlPair(QObject* parent = 0);
    CQmlPair(const CQmlPair& kHandle);
    virtual ~CQmlPair();
private:
    QString mapKey;
    QString mapValue;
};
Q_DECLARE_METATYPE(CQmlPair);
*/

class CClientApp : public QObject
{
    Q_OBJECT
public:
    CClientApp(QObject* pParent = 0);
    virtual ~CClientApp();

    bool Initialize(QQmlEngine* pEngine);
    void InitialConnect();

signals:
    void BroadcastCaught();

public Q_SLOTS:
    void parseDataToQML(const QVariantMap& retMap);
    void deviceAdded(const QJsonArray& retMap);
    void commandAdded(const QJsonArray& retMap);
    bool sendCommandToDevice(const QString& kKey1, const QString& kValue1,
                             const QString& kKey2 = "", const QString& kValue2 = "",
                             const QString& kKey3 = "", const QString& kValue3 = "",
                             const QString& kKey4 = "", const QString& kValue4 = "");

public Q_SLOTS:
    void ReadIncomeData();
    void ReplyUDPServerInfo();
    void processDeviceCommandSocket();
    void sendCommandToServer(QVariantMap cmdMap);

private:
    QQmlEngine*      m_pEngine;
    //CQmlServerApp*   m_pServerApp;
    FakeDeviceHandle*    m_kDevicesIfo;
    FakeDevicesInfoList* m_DevicesInfoListModel;
    FakeDeviceCmdList*   m_DeviceCmdListModel;
    DeviceEventSender*    m_pSender;

    QUdpSocket *broadCastSendSocket;
    QUdpSocket *broadCastRecieveSocket;
    QTcpServer *deviceCmdServer;
    QString serverIP;
    int temperature;
    bool connected;
};

