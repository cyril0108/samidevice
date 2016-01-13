#include "DeviceManager.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QDebug>

DeviceManager* DeviceManager::thePointer = nullptr;

DeviceManager* DeviceManager::GetInstance()
{
	if (thePointer == nullptr)
	{
		thePointer = new DeviceManager;
	}
	return thePointer;
}

DeviceManager::DeviceManager()
{
	
}


DeviceManager::~DeviceManager()
{

}

void DeviceManager::AddDevice(QString uid, DeviceInfo deviceInfo)
{
	knownDevices.insert(uid, deviceInfo);
}

void DeviceManager::RemoveDevice(QString uid)
{
	if (knownDevices.contains(uid))
	{
		knownDevices.remove(uid);
	}
}

void DeviceManager::AddDeviceCommand(QString uid, DeviceCommand devCmd)
{
	if (knownDevices.contains(uid))
	{
		knownDevices[uid].addSupportCommand(devCmd.getCommandName(), devCmd);
	}
}

DeviceInfo DeviceManager::GetDeviceInfo(QString uid) const
{
	if (knownDevices.contains(uid))
	{
		return knownDevices[uid];
	}
	return DeviceInfo();
}

void DeviceManager::SaveDeviceInfos()
{
	QDir dir("./");

	dir.mkpath("./data");

	QString filename = "./data/devices.db";
	QFile myFile(filename);
	if (!myFile.open(QIODevice::WriteOnly))
	{
		qDebug() << "Could not write to file:" << filename << "Error string:" << myFile.errorString();
		return;
	}

	QDataStream out(&myFile);
	out.setVersion(QDataStream::Qt_5_5);
	out << knownDevices;
	myFile.close();
}

void DeviceManager::LoadDeviceInfos()
{
	QString filename = "./data/devices.db";
	QFile myFile(filename);

	if (!myFile.exists())
	{
		return;
	}

	QDataStream in(&myFile);
	in.setVersion(QDataStream::Qt_5_5);

	if (!myFile.open(QIODevice::ReadOnly))
	{
		qDebug() << "Could not read the file:" << filename << "Error string:" << myFile.errorString();
	}

	in >> knownDevices;

	myFile.close();
}