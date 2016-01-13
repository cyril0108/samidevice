#include "ServerBroadCast.h"
#include "ServerUtil.h"
#include "Constant.h"
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

ServerBroadCast* ServerBroadCast::thePointer = nullptr;

ServerBroadCast* ServerBroadCast::GetInstance()
{
	if (thePointer == nullptr)
	{
		thePointer = new ServerBroadCast;
	}
	return thePointer;
}

ServerBroadCast::ServerBroadCast()
{
	broadCastSendSocket = new QUdpSocket(this);
	broadCastSendSocket->open(QIODevice::WriteOnly);
	broadCastRecieveSocket = new QUdpSocket(this);
	broadCastRecieveSocket->open(QIODevice::ReadOnly);
	//broadCastSendSocket->bind(QHostAddress::LocalHost, PORT_SERVER_BROADCAST, QUdpSocket::ShareAddress);
	
	testSocket = new QUdpSocket(this);
	//testSocket->open(QIODevice::ReadWrite);
	//testSocket->connectToHost("192.168.0.125", PORT_SERVER_BROADCAST);

	broadCastRecieveSocket->bind(PORT_SERVER_BROADCAST, QUdpSocket::ShareAddress);
	QObject::connect(broadCastRecieveSocket, &QUdpSocket::readyRead, [=]
	{
		ReadIncomeData();
	});

	QObject::connect(testSocket, &QUdpSocket::readyRead, [=]
	{
		ReadIncomeData();
	});


	broadCastTimer = new QTimer(this);
	broadCastSeq = ServerUtil<qint32>::GenerateRandomNumber();
	broadCastTimer->setInterval(INTERVAL_SERVER_BROADCAST);
	QObject::connect(broadCastTimer, &QTimer::timeout, [=]
	{
		QJsonObject json;
		json[JSON_KEY_SEQ] = broadCastSeq;// QString::number(broadCastSeq);
		json[JSON_KEY_COMMAND] = "query";
		QJsonDocument jsonDoc(json);
		QByteArray byteData = jsonDoc.toJson();
		//QByteArray byteData = "{ \"seq\":1234,\"cmd\":\"query\"}";
		//QByteArray byteData = "{\"CID\":\"001ZPYZTAW9RTB9A\",\"IOTID\":\"00TKUHRPEBC2WUY0\",\"TYPEID\":\"TF-L14DG-WI\",\"UID\":\"MT768124E7022630\"}";
		qDebug() << byteData;
		BroadCastOnce(&byteData);
		BroadCastTest(&byteData);
	});
}

ServerBroadCast::~ServerBroadCast()
{
	broadCastSendSocket->deleteLater();
	broadCastRecieveSocket->deleteLater();
	broadCastTimer->deleteLater();
}

void ServerBroadCast::BroadCastOnce(QByteArray *packet)
{
	QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

	for (int i = 0; i < ifaces.size(); ++i)
	{
		QList<QNetworkAddressEntry> addrs = ifaces.at(i).addressEntries();

		for (int j = 0; j < addrs.size(); j++)
		{
			if ((addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol) && (addrs[j].broadcast().toString() != ""))
			{
				qDebug() << addrs[j].ip() << " " << packet << " " << addrs[j].broadcast().toString();
				quint64 nn = broadCastSendSocket->writeDatagram(packet->data(), packet->size(), QHostAddress::Broadcast, PORT_SERVER_BROADCAST);
				qDebug() << nn << " " << broadCastSendSocket->errorString() << " " << broadCastSendSocket->state();
			}
		}
	}
}

void ServerBroadCast::BroadCastTest(QByteArray *packet)
{
	testSocket->write(*packet);
}

void ServerBroadCast::StartBroadCastingMessage()
{
	broadCastTimer->start();
}

void ServerBroadCast::StopBroadCastingMessage()
{
	broadCastTimer->stop();
}

void ServerBroadCast::ReadIncomeData()
{
	while (broadCastRecieveSocket->hasPendingDatagrams()) {
		QByteArray datagram;
		datagram.resize(broadCastRecieveSocket->pendingDatagramSize());
		broadCastRecieveSocket->readDatagram(datagram.data(), datagram.size());

		QJsonDocument jsonDoc = QJsonDocument::fromJson(datagram);
		QVariantMap jsonMap = jsonDoc.object().toVariantMap();

		Q_EMIT ClientDiscovered(jsonMap);
		qDebug() << "Read: " << datagram;
	}

	//QByteArray testData;
	//testData = testSocket->readAll();
	//qDebug() << testData;

	/*
	QByteArray datagram;
	datagram.resize(broadCastSocket->pendingDatagramSize());
	QHostAddress sender;
	quint16 senderPort;

	broadCastSocket->readDatagram(datagram.data(), datagram.size(),
		&sender, &senderPort);
		*/
	//QByteArray datagram = broadCastSocket->readAll();
	//qDebug() << "Read: " << datagram;
}
