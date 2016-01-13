#pragma once
#include "servercore_global.h"
#include <QUdpSocket>
#include <QTimer>
#include <QVariantMap>

class SERVERCORE_EXPORT ServerBroadCast : public QObject
{
Q_OBJECT

public:
	static ServerBroadCast *GetInstance();

private:
	static ServerBroadCast *thePointer;

public:
	~ServerBroadCast();
	void StartBroadCastingMessage();
	void StopBroadCastingMessage();

signals:
	void ClientDiscovered(QVariantMap retMap);
	

private:
	ServerBroadCast();
	void BroadCastOnce(QByteArray *packet);
	void BroadCastTest(QByteArray *packet);
	void ReadIncomeData();

private:
	QUdpSocket *broadCastSendSocket;
	QUdpSocket *broadCastRecieveSocket;
	QUdpSocket *testSocket;
	QTimer* broadCastTimer;
	qint32 broadCastSeq;
};

