#include "QmlServerApp.h"
#include "serverapp.h"
#include "ServerCore/servercore.h"
#include "ServerCore/ServerBroadCast.h"
#include "ServerCore/ServerTCPCommand.h"
#include "ServerCore/Constant.h"
#include "ServerCore/DeviceManager.h"
#include "ServerCore/DeviceInfo.h"
#include "ServerCore/DeviceCommand.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

CQmlServerApp::CQmlServerApp(QObject* pParent)
: QObject(pParent)
{
}

CQmlServerApp::~CQmlServerApp()
{
}

void CQmlServerApp::deviceAdded(QString uid)
{
    ServerCore::GetInstance()->deviceAdded(uid);
}

void CQmlServerApp::displayInfoRecieved(QString uid, QVariantMap retMap)
{
    ServerCore::GetInstance()->displayInfoRecieved(uid, retMap);
}

void CQmlServerApp::queryDeviceInfo(QString deviceUID, QString& iP, QString& Name)
{
    DeviceInfo kInfo = ServerCore::GetInstance()->queryDeviceInfo(deviceUID);
    iP = kInfo.getIP();
    Name = kInfo.getName();
}

bool CQmlServerApp::sendCommandToDevice(const QString& deviceUID, const QString& cmdKey, const QString& value)
{
    QVariantMap paramMap;
    paramMap.insert("command", cmdKey);
    paramMap.insert("param", value);
    ServerCore::GetInstance()->sendCommandToDevice(deviceUID, paramMap);
    return true;
}