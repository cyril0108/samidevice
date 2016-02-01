#include <QUdpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantMap>
#include <QByteArray>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QTcpServer>
#include <QTcpSocket>

#include <QQmlContext>
#include <QtQml>

//#include "QmlServerApp.h"
#include "ClientApp.h"
#include "FakeDeviceHandle.h"
#include "FakeDevicesInfoList.h"
#include "DeviceEventSender.h"

#include <QDebug>
#include <assert.h>
#define CORE_MAX 1
//#define TEST_CODE

template< typename T > inline
void SafeDelete(T*& pPointer) { delete pPointer; pPointer = nullptr; }
/*
CQmlPair::CQmlPair(QObject* parent) 
: QObject(parent)
{
}
CQmlPair::CQmlPair(const CQmlPair& kHandle) 
: QObject(nullptr)
{
}
CQmlPair::~CQmlPair()
{
}
*/
CClientApp::CClientApp(QObject* pParent)
: QObject( pParent )
, m_pEngine( nullptr )
, m_pSender(nullptr)
{
}

CClientApp::~CClientApp()
{
}

bool CClientApp::Initialize(QQmlEngine* pEngine)
{
    m_pEngine = pEngine;
    m_pSender = new DeviceEventSender(this);
    if (!m_pSender){ return false; }
    m_DevicesInfoListModel = new FakeDevicesInfoList(this);
    if (!m_DevicesInfoListModel){ return false; }
    m_DeviceCmdListModel = new FakeDeviceCmdList(m_DevicesInfoListModel);
    if (!m_DeviceCmdListModel){ return false; }
 //   m_qmlData = new CQMap();
 //   if (!m_qmlData){ return false; }

#ifdef TEST_CODE
    DeviceHandle ka = new DeviceHandle();
    ka.setUID("az10000");
    ka.setName("TV");
    ka.setIP("192.168.0.2");
    QList<QString> aa;
    aa.append("44444");
    aa.append("555555");
    aa.append("556666665555");
    ka.setCmdDisplayName(aa);
    ka.setCmdKeys(aa);

    m_DevicesInfoListModel->addDevice(ka);
    m_DeviceCmdListModel->addDevice(ka);
    ka.setUID("az10001");
    ka.setName("TV");
    ka.setIP("192.168.0.5");
    aa.clear();
    aa.append("444aaaa44");
    aa.append("55aa5555");
    aa.append("5566fff66665555");
    ka.setCmdDisplayName(aa);
    ka.setCmdKeys(aa);
    m_DevicesInfoListModel->addDevice(ka);
    m_DeviceCmdListModel->addDevice(ka);

    ka.setUID("az10002");
    ka.setName("TV");
    ka.setIP("192.168.0.3");
    aa.clear();
    aa.append("444gg44");
    aa.append("555hh555");
    aa.append("556yy666665555");
    ka.setCmdDisplayName(aa);
    ka.setCmdKeys(aa);
    m_DevicesInfoListModel->addDevice(ka);
    m_DeviceCmdListModel->addDevice(ka);

#endif

    m_pEngine->rootContext()->setContextProperty(QStringLiteral("FakeDevicesInfoList"), m_DevicesInfoListModel);
    m_pEngine->rootContext()->setContextProperty(QStringLiteral("FakeDeviceCmdList"), m_DeviceCmdListModel);
    m_pEngine->rootContext()->setContextProperty(QStringLiteral("FakeDeviceApp"), this);
    m_pEngine->rootContext()->setContextProperty(QStringLiteral("FakeDeviceEventSender"), m_pSender);
    //qmlRegisterType< CQmlPair >("com.azul.iot", 1, 0, "CQmlPair");
    InitialConnect();
    return true;
}

void CClientApp::InitialConnect()
{
    broadCastSendSocket = new QUdpSocket(this);
    broadCastSendSocket->open(QIODevice::WriteOnly);
    broadCastRecieveSocket = new QUdpSocket(this);
    broadCastRecieveSocket->open(QIODevice::ReadOnly);
    temperature = 30;
    connected = false;
    deviceCmdServer = new QTcpServer(this);

    QObject::connect(deviceCmdServer, &QTcpServer::newConnection, [=]
    {
        processDeviceCommandSocket();
    });

    if (!deviceCmdServer->listen(QHostAddress::Any, 3479)) {
        deviceCmdServer->close();
    }

    broadCastRecieveSocket->bind(3479, QUdpSocket::ShareAddress);
    QObject::connect(broadCastRecieveSocket, &QUdpSocket::readyRead, [=]
    {
        ReadIncomeData();
    });

    QObject::connect(this, &CClientApp::BroadcastCaught, [=]
    {
        ReplyUDPServerInfo();
    });
}

void CClientApp::ReadIncomeData()
{
    if (connected)
    {
        return;
    }

    while (broadCastRecieveSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        QHostAddress remoteAddress;
        datagram.resize(broadCastRecieveSocket->pendingDatagramSize());
        broadCastRecieveSocket->readDatagram(datagram.data(), datagram.size(), &remoteAddress);
        serverIP = remoteAddress.toString();
        if (serverIP.startsWith("::ffff:"))
        {
            serverIP = serverIP.right(serverIP.size() - 7);
        }
        QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram);
        QVariantMap jsonMap = jsonDoc.object().toVariantMap();

        QString kText("ServerIP: " + serverIP + "\n");
        kText += "BroadCast: " + QString::fromLatin1(datagram) + "\n";

        if (jsonMap["command"] == "query")
        {
            kText += "Emit BroadCastCaught..\n";
            Q_EMIT BroadcastCaught();
        }
        Q_EMIT m_pSender->commandReturned(kText, true);

        //Q_EMIT ClientDiscovered(jsonMap);
        //ui.plainTextEdit->appendPlainText( "Read: " + datagram );
    }
}

void CClientApp::ReplyUDPServerInfo()
{
    QString currentIP = "";
    for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
        {
            currentIP = address.toString();
        }
    }
    currentIP = "192.168.0.120";

    if (currentIP.isEmpty())
    {
        return;
    }

    QJsonObject jsonObj;
    jsonObj.insert("uid", "fakemonitor");
    jsonObj.insert("DisplayName", "Monitor Device");
    jsonObj.insert("ip", currentIP);
    jsonObj.insert("type", "xxXxx");

    QJsonDocument jsonDoc(jsonObj);
    QByteArray bytedata = jsonDoc.toJson();

    //broadCastSendSocket->write(bytedata);
    QHostAddress address;
    address.setAddress(serverIP);
    quint64 byteWritten = broadCastSendSocket->writeDatagram(bytedata.data(), bytedata.size(), address, 3479);

    bool bWritten = byteWritten > 0;//broadCastSendSocket->waitForBytesWritten();
    QString result = (bWritten ? "true" : "false");

    QString kText("CurrentIP: " + currentIP + "\n");
    kText += "WritingServer: " + QString::fromLatin1(bytedata) + "\n";
    kText += "UDP Written: " + QString::number(byteWritten) + "\n";
    Q_EMIT m_pSender->commandReturned(kText, true);
}

void CClientApp::processDeviceCommandSocket()
{
    QTcpSocket *socket = deviceCmdServer->nextPendingConnection();

    QString kText("Recieve TCP Socket\n");
    Q_EMIT m_pSender->commandReturned(kText, false);

    QObject::connect(socket, &QTcpSocket::readyRead, [=]
    {
        QByteArray byteArray = socket->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
        QJsonObject jsonObj = jsonDoc.object();

        QString command = jsonObj["command"].toString();

        QString retJsonString;

        QJsonObject retJsonObj;

        QString kText("Recieve Cmd: " + command + "\n");

        if (command == "supportCmds")
        {
            retJsonObj.insert("status", 200);
            QJsonArray cmdArray;
            /*
            QJsonObject cmdObj1;
            cmdObj1.insert("command", "adjustTemperature");
            cmdObj1.insert("command_displayName", "Set Temperature");
            cmdObj1.insert("param_type", "integer");
            cmdObj1.insert("param_max", "100");
            cmdObj1.insert("param_min", "20");

            QJsonObject cmdObj2;
            cmdObj2.insert("command", "query");
            cmdObj2.insert("command_displayName", "Query Status");
            cmdObj2.insert("param_type", "none");

            cmdArray.append(cmdObj1);
            cmdArray.append(cmdObj2);
            */
            retJsonObj.insert("SupportCmds", cmdArray);

            connected = true;
        }
        else if (command == "queryDisplayInfo")
        {
            retJsonObj.insert("status", 200);
            QJsonArray keyArray;
            keyArray.append("Power");
            //keyArray.append("WindStrength");
            //keyArray.append("Humidity");

            retJsonObj.insert("DisplayInfo", keyArray);

            //retJsonObj.insert("Temperature", QString::number(temperature));
            //retJsonObj.insert("WindStrength", "Medium");
            //retJsonObj.insert("Humidity", "60");
            retJsonObj.insert("Power", "On");
        }
        /*else if (command == "adjustTemperature")
        {
        temperature = jsonObj["param"].toString().toInt();
        retJsonObj.insert("status", 200);
        }*/
        else
        {
            kText += command + " " + jsonObj["param"].toVariant().toString() + "\n";
            retJsonObj.insert("status", 200);
        }

        QJsonDocument retDoc(retJsonObj);

        kText += "WritingServer: " + QString::fromLatin1(retDoc.toJson()) + "\n";

        socket->write(retDoc.toJson());

        bool isSuccess = socket->waitForBytesWritten();
        QString result = (isSuccess ? "true" : "false");
        kText += "Written: " + result + "\n";

        socket->disconnectFromHost();
        Q_EMIT m_pSender->commandReturned(kText, false);
    });

    QObject::connect(socket, &QTcpSocket::disconnected, [=]
    {
        socket->deleteLater();
    });
}

void CClientApp::sendCommandToServer(QVariantMap cmdMap)
{
    QTcpSocket *socket = new QTcpSocket(this);
    //serverIP = "192.168.0.238";

    QString command = cmdMap["command"].toString();
    QJsonObject jsonObj;

    if (command == "listDevices")
    {
        jsonObj.insert("command", command);
    }
    else if (command == "listCommands")
    {
        QString deviceUID = cmdMap["uid"].toString();
        jsonObj.insert("command", command);
        jsonObj.insert("uid", deviceUID);
    }
    else if (command == "sendCommandToDevice")
    {
        QString deviceUID = cmdMap["uid"].toString();
        QString deviceCmd = cmdMap["deviceCmd"].toString();
        QString deviceParam = cmdMap["param"].toString();
        jsonObj.insert("command", command);
        jsonObj.insert("uid", deviceUID);
        jsonObj.insert("deviceCmd", deviceCmd);
        jsonObj.insert("param", deviceParam);
    }

    QJsonDocument doc(jsonObj);
    Q_EMIT m_pSender->commandReturned("SendCmdToServer: " + QString::fromLatin1(doc.toJson()) + "\n", false);

    QObject::connect(socket, &QTcpSocket::readyRead, [=]
    {
        QByteArray byteArray = socket->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
        QJsonObject jsonObj = jsonDoc.object();

        QString kText("Cmd Return Result: " + QString::number(jsonObj["status"].toInt()) + "\n");
        if (jsonObj["status"].toInt() == 200)
        {
            QVariantMap retMap = jsonObj.toVariantMap();
            qDebug() << jsonObj["devices"].toArray().size();
            parseDataToQML(retMap);
            kText += QString::fromLatin1(byteArray) + "\n";
        }
        Q_EMIT m_pSender->commandReturned(kText, false);
        socket->disconnectFromHost();
    });

    QObject::connect(socket, &QTcpSocket::disconnected, [=]
    {
        socket->deleteLater();
    });

    socket->connectToHost(serverIP, 3479, QIODevice::ReadWrite);

    socket->write(doc.toJson());
    bool bSentCmd = socket->waitForBytesWritten();
    QString result = (bSentCmd ? "true" : "false");
    Q_EMIT m_pSender->commandReturned("Cmd Sent: " + result, false);
}

void CClientApp::parseDataToQML(const QVariantMap& retMap)
{
    QJsonArray devices = QJsonArray::fromVariantList(retMap["devices"].toList());
    QJsonArray commands = QJsonArray::fromVariantList(retMap["SupportCmds"].toList());
    if (devices.size()>0)
    {
        deviceAdded(devices);
    }
    else if (commands.size()>0)
    {
        commandAdded(commands);
    }
    else
    {
        //??
    }
}

void CClientApp::deviceAdded(const QJsonArray& devices)
{
    for (auto i : devices)
    {
        QVariantMap deviceMap = i.toObject().toVariantMap();
        QString deviceUID = deviceMap["uid"].toString();
        QString deviceName = deviceMap["DisplayName"].toString();
        QString deviceIP = deviceMap["ip"].toString();

        if (!deviceUID.isEmpty() && m_DevicesInfoListModel->isExist(deviceUID))
        {
            DeviceHandle* kDevicesInfo = new DeviceHandle;
            kDevicesInfo->setUID(deviceUID);
            kDevicesInfo->setIP(deviceIP);
            kDevicesInfo->setName(deviceName);
            m_DevicesInfoListModel->addDevice(*kDevicesInfo);
        }
    }
}

void CClientApp::commandAdded(const QJsonArray& commands)
{
    QList<QString> kCmdName;
    QList<QString> kCmdDisplayName;
    DeviceHandle* kDevicesInfo = new DeviceHandle;
    for (auto i : commands)
    {
        QVariantMap cmdMap = i.toObject().toVariantMap();
        if (cmdMap.count()>0)
        {
            kCmdName.append(cmdMap["deviceCmd"].toString());
            kCmdDisplayName.append(cmdMap["command_displayName"].toString());
            kDevicesInfo->setCmdKeys(kCmdName);
            kDevicesInfo->setCmdDisplayName(kCmdDisplayName);
        }
    }
    m_DeviceCmdListModel->addDevice(*kDevicesInfo);
}

bool CClientApp::sendCommandToDevice(const QString& kKey1, const QString& kValue1,
    const QString& kKey2, const QString& kValue2,
    const QString& kKey3, const QString& kValue3,
    const QString& kKey4, const QString& kValue4)
{
    QVariantMap paramMap;
    paramMap.insert(kKey1, kValue1);
    paramMap.insert(kKey2, kValue2);
    paramMap.insert(kKey3, kValue3);
    paramMap.insert(kKey4, kValue4);
    sendCommandToServer(paramMap);
    return true;
}