#ifndef FAKEDEVICE_H
#define FAKEDEVICE_H

#include <QMainWindow>
#include "ui_fakedevice.h"

class QUdpSocket;
class QTcpServer;

class FakeDevice : public QMainWindow
{
	Q_OBJECT

public:
	FakeDevice(QWidget *parent = 0);
	~FakeDevice();

signals:
	void BroadcastCaught();
	
private:
	void ReadIncomeData();
	void ReplyUDPServerInfo();
	void processDeviceCommandSocket();
	void sendCommandToServer(QString command);

private:
	Ui::FakeDeviceClass ui;
	QUdpSocket *broadCastSendSocket;
	QUdpSocket *broadCastRecieveSocket;
	QTcpServer *deviceCmdServer;
	QString serverIP;
	int temperature;
	bool connected;
};

#endif // FAKEDEVICE_H
