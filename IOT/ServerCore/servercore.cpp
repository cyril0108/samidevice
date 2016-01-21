#include "servercore.h"
#include "Constant.h"
#include "common.h"
#include "ServerBroadCast.h"
#include "ServerTCPCommand.h"
#include "DeviceManager.h"
#include "DeviceInfo.h"
#include "DeviceCommand.h"
#include "JsonGenerator.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

ServerCore* ServerCore::thePointer = nullptr;

ServerCore* ServerCore::GetInstance()
{
	if (thePointer == nullptr)
	{
		thePointer = new ServerCore;
	}
	return thePointer;
}

ServerCore::ServerCore()
{
	init();
}

ServerCore::~ServerCore()
{
	
}

void ServerCore::init()
{
	connectSignals();
	serverStart = false;
	// load file  DeviceManager handle itself

	
}

void ServerCore::startServer()
{
	serverStart = true;
	BroadCastModule()->StartBroadCastingMessage();
	DeviceManagerModule()->LoadDeviceInfos();
}

void ServerCore::stopServer()
{
	serverStart = false;
	BroadCastModule()->StopBroadCastingMessage();
	DeviceManagerModule()->SaveDeviceInfos();
}

void ServerCore::connectSignals()
{
	QObject::connect(BroadCastModule(), &ServerBroadCast::ClientDiscovered, [=]( QVariantMap dataMap )
	{
		processNewDevice(dataMap);
	});

	deviceCmdServer = new QTcpServer(this);

	QObject::connect(deviceCmdServer, &QTcpServer::newConnection, [=]
	{
		processDeviceCommandSocket();
	});

	if (!deviceCmdServer->listen(QHostAddress::Any, PORT_SERVER_BROADCAST)) {
		deviceCmdServer->close();
	}
}

void ServerCore::processNewDevice(QVariantMap& dataMap)
{
	bool isSelfBroadcast = dataMap.contains(JSON_KEY_COMMAND) && dataMap[JSON_KEY_COMMAND] == CMD_QUERY
		&& (!dataMap.contains(JSON_KEY_UID) || !dataMap.contains(JSON_KEY_DISPLAYNAME)
			|| !dataMap.contains(JSON_KEY_IP));

	if (isSelfBroadcast)
	{
		return;
	}

	QString deviceUID = dataMap[JSON_KEY_UID].toString();
	QString deviceName = dataMap[JSON_KEY_DISPLAYNAME].toString();
	QString deviceIP = dataMap[JSON_KEY_IP].toString();
	QString deviceType = dataMap[JSON_KEY_TYPE].toString();

	DeviceInfo devInfo(deviceUID, deviceName, deviceIP, deviceType);
	DeviceManagerModule()->AddDevice(deviceUID, devInfo);

	QVariantMap paramMap;
	paramMap.insert(JSON_KEY_SEQ, 1234);

	QTcpSocket *socket = new QTcpSocket(this);

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		parseSupportCmds(deviceUID, byteArray);
		Q_EMIT deviceAdded(deviceUID);
		socket->disconnectFromHost();
	});

	QObject::connect(socket, static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
		[=](QAbstractSocket::SocketError socketError)
	{
		qDebug() << "Socket Error" << socketError;
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		socket->deleteLater();
	});

	socket->connectToHost(deviceIP, PORT_SERVER_BROADCAST, QIODevice::ReadWrite);

	socket->waitForConnected();

	QString jsonCmd = JsonGenerator::GenerateJsonCommand(TCP_COMMAND_TYPE::QUERY_DEVICE_SUPPORT_CMDS, paramMap);

	socket->write(jsonCmd.toLatin1());
	socket->flush();

	socket->waitForBytesWritten();
	socket->waitForReadyRead();

	
}

void ServerCore::parseSupportCmds(QString uid, QByteArray byteArray)
{
	QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
	QJsonObject jsonObj = jsonDoc.object();

	if (jsonObj[JSON_KEY_STATUS].toInt() == STATUS_SUCCESSFUL)
	{
		QJsonArray cmdArray = jsonObj[JSON_KEY_SUPPORTCMDS].toArray();

		for (int i = 0; i < cmdArray.size(); ++i)
		{
			QJsonObject cmdObj = cmdArray.at(i).toObject();

			QString devCmd = cmdObj[JSON_KEY_COMMAND].toString();
			QString devCmdDisplayName = cmdObj[JSON_KEY_CMD_DISPLAYNAME].toString();
			QString paramType = cmdObj[JSON_KEY_PARAM_TYPE].toString();

			PARAM_TYPE param_type = PARAM_TYPE::NONE;

			QString paramMin;
			QString paramMax;

			if (paramType == "integer")
			{
				param_type = PARAM_TYPE::INTEGER;
				paramMin = cmdObj[JSON_KEY_PARAM_MIN].toString();
				paramMax = cmdObj[JSON_KEY_PARAM_MAX].toString();
			}
			else if (paramType == "float")
			{
				param_type = PARAM_TYPE::FLOAT;
				paramMin = cmdObj[JSON_KEY_PARAM_MIN].toString();
				paramMax = cmdObj[JSON_KEY_PARAM_MAX].toString();
			}
			else if (paramType == "text")
			{
				param_type = PARAM_TYPE::TEXT;
			}

			DeviceCommand commandDetail(devCmd, devCmdDisplayName, param_type, paramMin, paramMax);
			DeviceManagerModule()->AddDeviceCommand(uid, commandDetail);
		}
	}
}

QTcpSocket* ServerCore::createSocket(QString deviceUID)
{
	DeviceInfo info = DeviceManagerModule()->GetDeviceInfo(deviceUID);
	QTcpSocket *socket = new QTcpSocket(this);
	socket->connectToHost(info.getIP(), PORT_SERVER_BROADCAST, QIODevice::ReadWrite);
	return socket;
}

DeviceInfo ServerCore::queryDeviceInfo(QString deviceUID)
{
    return DeviceManagerModule()->GetDeviceInfo( deviceUID);
}

void ServerCore::queryDeviceDisplayInfo(QString deviceUID)
{
	QTcpSocket *socket = createSocket(deviceUID);

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
		QJsonObject jsonObj = jsonDoc.object();

		if (jsonObj[JSON_KEY_STATUS].toInt() == STATUS_SUCCESSFUL)
		{
			QVariantMap retMap = jsonObj.toVariantMap();
			Q_EMIT displayInfoRecieved(deviceUID, retMap);
		}

		socket->disconnectFromHost();
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		socket->deleteLater();
	});

	QVariantMap paramMap;
	paramMap.insert(JSON_KEY_SEQ, 1234);

	QString jsonCmd = JsonGenerator::GenerateJsonCommand(TCP_COMMAND_TYPE::QUERY_DEVICE_DISPLAY_INFO, paramMap);

	socket->write(jsonCmd.toLatin1());
}

void ServerCore::sendCommandToDevice(QString deviceUID, QVariantMap paramMap)
{
	QTcpSocket *socket = createSocket(deviceUID);

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
		QJsonObject jsonObj = jsonDoc.object();

		if (jsonObj[JSON_KEY_STATUS].toInt() == STATUS_SUCCESSFUL)
		{
			QVariantMap retMap = jsonObj.toVariantMap();
			Q_EMIT commandReturned(deviceUID, retMap);
		}

		socket->disconnectFromHost();
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		socket->deleteLater();
	});

	QString jsonCmd = JsonGenerator::GenerateJsonCommand(TCP_COMMAND_TYPE::SEND_COMMAND_TO_DEVICE, paramMap);

	socket->write(jsonCmd.toLatin1());

	socket->waitForBytesWritten();
	socket->waitForReadyRead();
}

void ServerCore::processDeviceCommandSocket()
{
	if (!serverStart)
	{
		return;
	}

	QTcpSocket *socket = deviceCmdServer->nextPendingConnection();

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
		QJsonObject jsonObj = jsonDoc.object();

		QVariantMap retParamMap = prepareDataForDevice(jsonObj);
		TCP_REPLY_TYPE retType = (TCP_REPLY_TYPE)retParamMap[JSON_KEY_RETURN_TYPE].toInt();

		QString retString = JsonGenerator::GenerateJsonReply(retType, retParamMap);
		QString peerAddress = socket->peerAddress().toString();
		socket->write(retString.toLatin1());

		bool isSuccess = socket->waitForBytesWritten();

		Q_EMIT dataWrittenToDevice(peerAddress, retString);

		socket->disconnectFromHost();
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		socket->deleteLater();
	});
}

QVariantMap ServerCore::prepareDataForDevice(const QJsonObject& cmdObj)
{
	QString command = cmdObj[JSON_KEY_COMMAND].toString();
	QVariantMap retMap;
	bool isFail = false;

	if (command == SERVER_CMD_LIST_DEVICES)
	{
		QJsonArray devArray;

		for (QString uid : DeviceManagerModule()->GetDeviceUIDs())
		{
			QJsonObject devObj;
			devObj.insert(JSON_KEY_UID, uid);
			QString dispName = DeviceManagerModule()->GetDeviceInfo(uid).getName();
			QString ip = DeviceManagerModule()->GetDeviceInfo(uid).getIP();
			devObj.insert(JSON_KEY_DISPLAYNAME, dispName);
			devObj.insert(JSON_KEY_IP, ip);

			devArray.append(devObj);
		}

		retMap.insert(JSON_KEY_RETURN_TYPE, (int)TCP_REPLY_TYPE::QUERY_DEVICES);
		retMap.insert(JSON_KEY_DEVICES, devArray);
	}
	else if (command == SERVER_CMD_LIST_COMMANDS)
	{
		QJsonArray devArray;

		QString uid = cmdObj[JSON_KEY_UID].toString();

		if (!DeviceManagerModule()->HasDevice(uid))
		{
			isFail = true;
		}

		for (QString devCommand : DeviceManagerModule()->GetDeviceInfo(uid).getSupportCommands())
		{
			QJsonObject devObj;
			devObj.insert(JSON_KEY_DEVICECMD, devCommand);
			QString dispName = DeviceManagerModule()->GetDeviceInfo(uid).getDeviceCommandDetail(devCommand).getCommandDisplayName();
			int paramType = (int)DeviceManagerModule()->GetDeviceInfo(uid).getDeviceCommandDetail(devCommand).getParamType();
			QString paramMin = DeviceManagerModule()->GetDeviceInfo(uid).getDeviceCommandDetail(devCommand).getParamMin();
			QString paramMax = DeviceManagerModule()->GetDeviceInfo(uid).getDeviceCommandDetail(devCommand).getParamMax();
			devObj.insert(JSON_KEY_CMD_DISPLAYNAME, dispName);
			devObj.insert(JSON_KEY_PARAM_TYPE, paramType);
			devObj.insert(JSON_KEY_PARAM_MIN, paramMin);
			devObj.insert(JSON_KEY_PARAM_MAX, paramMax);

			devArray.append(devObj);
		}

		retMap.insert(JSON_KEY_RETURN_TYPE, (int)TCP_REPLY_TYPE::QUERY_COMMANDS);
		retMap.insert(JSON_KEY_SUPPORTCMDS, devArray);
	}
	else if (command == SERVER_CMD_SEND_CMD_TO_DEVICE)
	{
		QString uid = cmdObj[JSON_KEY_UID].toString();
		QString devCmd = cmdObj[JSON_KEY_DEVICECMD].toString();
		retMap.insert(JSON_KEY_RETURN_TYPE, (int)TCP_REPLY_TYPE::FORWARD_COMMAND_TO_DEVICE);

		if (!DeviceManagerModule()->HasDevice(uid))
		{
			isFail = true;
		}

		if (!isFail && !DeviceManagerModule()->GetDeviceInfo(uid).isSupportedCommand(devCmd))
		{
			isFail = true;
		}

		QString cmdParam = cmdObj[JSON_KEY_PARAM].toString();
		if (!isFail && !DeviceManagerModule()->GetDeviceInfo(uid).getDeviceCommandDetail(devCmd).isValidParam(cmdParam))
		{
			isFail = true;
		}

		if (!isFail)
		{
			QVariantMap cmdMap;
			cmdMap.insert(JSON_KEY_COMMAND, devCmd);
			cmdMap.insert(JSON_KEY_PARAM, cmdParam);
			cmdMap.insert(JSON_KEY_SEQ, 1234);

			sendCommandToDevice(uid, cmdMap);
		}
	}
	else
	{
		isFail = true;
	}

	retMap.insert(JSON_KEY_STATUS, ((isFail) ? STATUS_FAIL : STATUS_SUCCESSFUL));

	return retMap;
}

ServerBroadCast* ServerCore::BroadCastModule()
{
	return ServerBroadCast::GetInstance();
}

DeviceManager* ServerCore::DeviceManagerModule()
{
	return DeviceManager::GetInstance();
}
