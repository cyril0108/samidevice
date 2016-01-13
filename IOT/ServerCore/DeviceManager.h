#pragma once
#include "servercore_global.h"
#include <QObject>
#include <QMap>
#include "DeviceInfo.h"
#include "DeviceCommand.h"

class SERVERCORE_EXPORT DeviceManager :
	public QObject
{
	Q_OBJECT

public:
	static DeviceManager *GetInstance();

private:
	static DeviceManager *thePointer;

public:
	void AddDevice(QString uid, DeviceInfo deviceInfo);
	void RemoveDevice(QString uid);
	bool HasDevice(QString uid) const { return knownDevices.contains(uid); }
	void AddDeviceCommand(QString uid, DeviceCommand devCmd);
	DeviceInfo GetDeviceInfo(QString uid) const;
	QList<QString> GetDeviceUIDs() const { return knownDevices.keys(); }

	void SaveDeviceInfos();
	void LoadDeviceInfos();

public:
	DeviceManager();
	~DeviceManager();

private:
	QMap <QString, DeviceInfo> knownDevices;
};

