#include <QQmlContext>
#include "DeviceHandle.h"

DeviceHandle::DeviceHandle(QObject* pParent)
: QObject(pParent)
{
}

DeviceHandle::DeviceHandle(const DeviceHandle& kHandle)
: QObject( nullptr )
, deviceUID(kHandle.deviceUID)
, deviceName(kHandle.deviceName)
, deviceIP(kHandle.deviceIP)
, deviceType(kHandle.deviceType)
, deviceCmdKeys(kHandle.deviceCmdKeys)
, deviceCmdDisplayNames(kHandle.deviceCmdDisplayNames)
{
}

DeviceHandle::DeviceHandle(const QString& kUID, const QString& kName, const QString& kIP, const QString& kType, const QList<QString>& kCmd, const QList<QString>& kCmdName)
: QObject( nullptr )
{
    deviceUID =     kUID;
    deviceName =    kName;
    deviceIP =      kIP;
    deviceType =    kType;
    deviceCmdKeys = kCmd;
    deviceCmdDisplayNames = kCmdName;
}

DeviceHandle::~DeviceHandle()
{
}

DeviceHandle& DeviceHandle::operator=(const DeviceHandle& kHandle)
{
    deviceUID =     kHandle.deviceUID;
    deviceName =    kHandle.deviceName;
    deviceIP =      kHandle.deviceIP;
    deviceType =    kHandle.deviceType;
    deviceCmdKeys = kHandle.deviceCmdKeys;
    deviceCmdDisplayNames = kHandle.deviceCmdDisplayNames;
    return *this;
}