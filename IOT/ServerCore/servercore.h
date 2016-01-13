#ifndef SERVERCORE_H
#define SERVERCORE_H

#include "servercore_global.h"
#include <QObject>
#include <QVariantMap>

class ServerBroadCast;
class DeviceManager;
class QTcpSocket;
class QTcpServer;

class SERVERCORE_EXPORT ServerCore : public QObject
{
	Q_OBJECT 

public:
	static ServerCore *GetInstance();

private:
	static ServerCore *thePointer;


public:
	~ServerCore();
	
	void startServer();
	void stopServer();

	void queryDeviceDisplayInfo(QString deviceUID);
	void sendCommandToDevice(QString deviceUID, QVariantMap paramMap);

signals:
	void deviceAdded(QString uid);
	void displayInfoRecieved(QString uid, QVariantMap retMap);
	void commandReturned(QString uid, QVariantMap retMap);
	void dataWrittenToDevice(QString remoteIP, QString msg);

private:
	ServerCore();
	void init();
	void connectSignals();

	ServerBroadCast* BroadCastModule();
	DeviceManager* DeviceManagerModule();

	// handle new device
	void processNewDevice( QVariantMap& dataMap );
	void parseSupportCmds(QString uid, QByteArray byteArray);

	// handle device command
	void processDeviceCommandSocket();
	QVariantMap prepareDataForDevice(const QJsonObject& cmdObj);

	QTcpSocket* createSocket(QString deviceUID);

private:
	QTcpServer *deviceCmdServer;
	bool serverStart;

};

#endif // SERVERCORE_H
