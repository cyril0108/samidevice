#pragma once
#include "servercore_global.h"
#include "DeviceCommand.h"
#include <QString>
#include <QMap>
#include <QDataStream>


class SERVERCORE_EXPORT DeviceInfo
{
public:
	DeviceInfo(QString deviceUID = "", QString deviceName = "", QString deviceIP = "", QString deviceType = "");
	~DeviceInfo();

	void addSupportCommand(QString command, DeviceCommand cmdDetail);
	void removeSupportCommand(QString command);

	QString getUID() const { return deviceUID; }
	QString getName() const { return deviceName; }
	QString getIP() const { return deviceIP; }
	QString getType() const { return deviceType; }

	bool isSupportedCommand(QString command) const { return supportCommands.contains(command); }
	DeviceCommand getDeviceCommandDetail(QString command) const;
	QList<QString> getSupportCommands() const { return supportCommands.keys(); }

	friend QDataStream& operator<<(QDataStream& out, const DeviceInfo& info);
	friend QDataStream& operator>>(QDataStream& in, DeviceInfo& info);

private:
	QString deviceUID;
	QString deviceName;
	QString deviceIP;
	QString deviceType;

	QMap<QString, DeviceCommand> supportCommands;
};
