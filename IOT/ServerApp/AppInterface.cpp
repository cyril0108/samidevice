#include <QQmlContext>
#include <QtQml>

#include "QmlServerApp.h"
#include "AppInterface.h"
#include "DeviceHandle.h"
#include "DevicesInfoList.h"

#include "serverapp.h"
#include "ServerCore/servercore.h"
#include "ServerCore/Constant.h"
#include "ServerCore/DeviceInfo.h"
#include "ServerCore/DeviceCommand.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <assert.h>

#define CORE_MAX 1
//#define TEST_CODE

template< typename T > inline
void SafeDelete(T*& pPointer) { delete pPointer; pPointer = nullptr; }

CAppInterface::CAppInterface( QObject* pParent )
: QObject( pParent )
, m_pEngine( nullptr )
{    
}

CAppInterface::~CAppInterface()
{
    SafeDelete(m_pServerApp);
}

bool CAppInterface::Initialize(QQmlEngine* pEngine)
{
    m_pEngine = pEngine;
    m_DevicesInfoListModel = new DevicesInfoList( this );
    if (!m_DevicesInfoListModel){ return false; }
    m_DeviceCmdListModel = new DeviceCmdList(m_DevicesInfoListModel);
    if (!m_DeviceCmdListModel){ return false; }
    m_pServerApp = new CQmlServerApp;
    if (!m_pServerApp){ return false; }

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

    m_pEngine->rootContext()->setContextProperty(QStringLiteral("DevicesInfoList"), m_DevicesInfoListModel);
    m_pEngine->rootContext()->setContextProperty(QStringLiteral("DeviceCmdList"), m_DeviceCmdListModel);
    m_pEngine->rootContext()->setContextProperty(QStringLiteral("ServerApp"), m_pServerApp);
    CreateServerApp();

    //qmlRegisterType< DeviceHandle >("com.azul.iot", 1, 0, "DeviceHandle");
    //qmlRegisterType< DevicesInfoList >("com.azul.iot", 1, 0, "DevicesInfoList");
    //qmlRegisterType< DeviceCmdList >("com.azul.iot", 1, 0, "DeviceCmdList");

    return true;
}

void CAppInterface::CreateServerApp()
{
    //connect ServerCore Signal
    QObject::connect(ServerCore::GetInstance(), &ServerCore::deviceAdded, [=](QString uid)
    {
        if (m_DevicesInfoListModel->isExist(uid))
        {
            DeviceHandle* kDevicesIfo = new DeviceHandle;
            DeviceInfo kInfo = ServerCore::GetInstance()->queryDeviceInfo(uid);
            kDevicesIfo->setUID(uid);
            kDevicesIfo->setIP(kInfo.getIP());
            kDevicesIfo->setName(kInfo.getName());
            kDevicesIfo->setCmdKeys(kInfo.getSupportCommands());

            QList<QString> kDisplayName;
            for (auto i : kDevicesIfo->getCmdKeys())
            {
                DeviceCommand kCmd = kInfo.getDeviceCommandDetail(i);
                kDisplayName.append(kCmd.getCommandDisplayName());
            }
            kDevicesIfo->setCmdDisplayName(kInfo.getSupportCommands());

            m_DevicesInfoListModel->addDevice(*kDevicesIfo);
            m_DeviceCmdListModel->addDevice(*kDevicesIfo);
        }
    });
    
    /*
    QObject::connect(ServerCore::GetInstance(), &ServerCore::commandReturned, [=]
    {
    });
    QObject::connect(ServerCore::GetInstance(), &ServerCore::dataWrittenToDevice, [=]
    {
    });
    QObject::connect(ui.GetStatusButton, &QPushButton::clicked, [=]
    {
        QVariantMap paramMap;
        paramMap.insert("command", "queryDisplayInfo");
        ServerCore::GetInstance()->sendCommandToDevice("az0000", paramMap);
    });

    QObject::connect(ServerCore::GetInstance(), &ServerCore::commandReturned, [=](QString uid, QVariantMap retMap)
    {
        ui.plainTextEdit->appendPlainText(uid + " returns: ");

        for (QString key : retMap.keys())
        {
            ui.plainTextEdit->appendPlainText(key + ": " + retMap[key].toString());
        }
    });
    */
    ServerCore::GetInstance()->startServer();
}
