#include "DeviceInfo.h"


DeviceInfo::DeviceInfo(QString deviceUID, QString deviceName, QString deviceIP, QString deviceType)
: deviceUID(deviceUID), deviceName(deviceName), deviceIP(deviceIP), deviceType(deviceType)
{
}

DeviceInfo::~DeviceInfo()
{
}

void DeviceInfo::addSupportCommand(QString command, DeviceCommand cmdDetail)
{
	supportCommands.insert(command, cmdDetail);
}

void DeviceInfo::removeSupportCommand(QString command)
{
	if (supportCommands.contains(command))
	{
		supportCommands.remove(command);
	}
}

DeviceCommand DeviceInfo::getDeviceCommandDetail(QString command) const
{
	if (isSupportedCommand(command))
	{
		return supportCommands[command];
	}
	return DeviceCommand();
}

QDataStream& operator<<(QDataStream& out, const DeviceInfo& info)
{
	out << info.deviceUID << info.deviceName << info.deviceIP << info.deviceType
		<< info.supportCommands;
	return out;
}

QDataStream& operator>>(QDataStream& in, DeviceInfo& info)
{
	in >> info.deviceUID >> info.deviceName >> info.deviceIP >> info.deviceType
		>> info.supportCommands;
	return in;
}
