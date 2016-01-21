#include "fakedevice.h"
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

FakeDevice::FakeDevice(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
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

	QObject::connect(this, &FakeDevice::BroadcastCaught, [=]
	{
		ReplyUDPServerInfo();
	});

	

	QObject::connect(ui.listDeviceBtn, &QPushButton::clicked, [=]
	{
		QVariantMap cmdMap;
		cmdMap.insert("command", "listDevices");
		sendCommandToServer(cmdMap);
	});

	QObject::connect(ui.listCommandBtn, &QPushButton::clicked, [=]
	{
		QVariantMap cmdMap;
		cmdMap.insert("command", "listCommands");
		cmdMap.insert("uid", "arduinozero");
		sendCommandToServer(cmdMap);
	});

	QObject::connect(ui.sendCmdBtn, &QPushButton::clicked, [=]
	{
		QVariantMap cmdMap;
		cmdMap.insert("command", "sendCommandToDevice");
		cmdMap.insert("uid", "arduinozero");
		cmdMap.insert("deviceCmd", "adjustTemperature");
		cmdMap.insert("param", "45");
		sendCommandToServer(cmdMap);
	});
}

FakeDevice::~FakeDevice()
{

}


void FakeDevice::ReadIncomeData()
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
		ui.plainTextEdit->appendPlainText("ServerIP: " + serverIP);
		ui.plainTextEdit->appendPlainText("BroadCast: " + QString::fromLatin1(datagram));
		if (jsonMap["command"] == "query")
		{
			ui.plainTextEdit->appendPlainText("Emit BroadCastCaught..");
			Q_EMIT BroadcastCaught();
		}

		//Q_EMIT ClientDiscovered(jsonMap);
		//ui.plainTextEdit->appendPlainText( "Read: " + datagram );
	}
}

void FakeDevice::ReplyUDPServerInfo()
{
	QString currentIP = "";
	for (const QHostAddress &address : QNetworkInterface::allAddresses()) {
		if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
		{
			currentIP = address.toString();
		}
	}
	//currentIP = "192.168.1.59";
	ui.plainTextEdit->appendPlainText("CurrentIP: " + currentIP);

	if (currentIP.isEmpty())
	{
		return;
	}

	QJsonObject jsonObj;
	jsonObj.insert( "uid", "fakemonitor");
	jsonObj.insert("DisplayName", "Monitor Device");
	jsonObj.insert("ip", currentIP);
	jsonObj.insert("type", "xxXxx");

	QJsonDocument jsonDoc(jsonObj);
	QByteArray bytedata = jsonDoc.toJson();

	ui.plainTextEdit->appendPlainText("WritingServer: " + QString::fromLatin1(bytedata));

	//broadCastSendSocket->write(bytedata);
	QHostAddress address;
	address.setAddress(serverIP);
	quint64 byteWritten = broadCastSendSocket->writeDatagram(bytedata.data(), bytedata.size(), address, 3479);

	bool bWritten = byteWritten > 0;//broadCastSendSocket->waitForBytesWritten();
	QString result = (bWritten ? "true" : "false");
	ui.plainTextEdit->appendPlainText("UDP Written: " + byteWritten);
}

void FakeDevice::processDeviceCommandSocket()
{
	QTcpSocket *socket = deviceCmdServer->nextPendingConnection();

	ui.plainTextEdit->appendPlainText("Recieve TCP Socket");

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
		QJsonObject jsonObj = jsonDoc.object();

		QString command = jsonObj["command"].toString();

		QString retJsonString;

		QJsonObject retJsonObj;

		ui.plainTextEdit->appendPlainText("Recieve Cmd: " + command);

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
			ui.plainTextEdit->appendPlainText( command + " " + jsonObj["param"].toVariant().toString());
			retJsonObj.insert("status", 200);
		}

		QJsonDocument retDoc(retJsonObj);

		ui.plainTextEdit->appendPlainText("WritingServer: " + retDoc.toJson());

		socket->write(retDoc.toJson());

		bool isSuccess = socket->waitForBytesWritten();
		QString result = (isSuccess ? "true" : "false");
		ui.plainTextEdit->appendPlainText("Written: " + result);

		socket->disconnectFromHost();
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		socket->deleteLater();
	});
}

void FakeDevice::sendCommandToServer(QVariantMap cmdMap)
{
	QTcpSocket *socket = new QTcpSocket(this);
	//serverIP = "192.168.1.7";
	

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

	ui.plainTextEdit->appendPlainText("SendCmdToServer: " + doc.toJson());

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		QByteArray byteArray = socket->readAll();
		QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray);
		QJsonObject jsonObj = jsonDoc.object();

		ui.plainTextEdit->appendPlainText("Cmd Return Result: " + jsonObj["status"].toInt());

		if (jsonObj["status"].toInt() == 200)
		{
			QVariantMap retMap = jsonObj.toVariantMap();
			ui.plainTextEdit->appendPlainText(QString::fromLatin1(byteArray));
		}

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
	ui.plainTextEdit->appendPlainText("Cmd Sent: " + result);
}
