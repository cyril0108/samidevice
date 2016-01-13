#include "serverapp.h"
#include "ServerCore/servercore.h"
#include "ServerCore/ServerBroadCast.h"
#include "ServerCore/ServerTCPCommand.h"
#include "ServerCore/Constant.h"

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

ServerApp::ServerApp(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QObject::connect(ui.LoginButton, &QPushButton::clicked, [=]
	{
		QVariantMap paramMap;
		paramMap.insert("command", "adjustTemperature");
		paramMap.insert("param", "35");
		paramMap.insert("seq", 1234);
		ServerCore::GetInstance()->sendCommandToDevice("az0000", paramMap);
	});

	QObject::connect(ui.GetStatusButton, &QPushButton::clicked, [=]
	{
		QVariantMap paramMap;
		paramMap.insert("command", "queryDisplayInfo");
		ServerCore::GetInstance()->sendCommandToDevice("az0000", paramMap);
	});

	QObject::connect(ServerCore::GetInstance(), &ServerCore::commandReturned, [=](QString uid, QVariantMap retMap)
	{
		ui.plainTextEdit->appendPlainText(uid + " returns: ");

		for (QString key : retMap.keys())
		{
			ui.plainTextEdit->appendPlainText(key + ": " + retMap[key].toString());
		}
		
	});

	ServerCore::GetInstance()->startServer();
	
	/*
	ServerCore::BroadCastModule()->StartBroadCastingMessage();
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "cake");
	cryptObj.insert("param", "choco");
	cryptObj.insert("data", "0123");
	QString cipher = ServerTCPCommand::EncryptJSONObject(cryptObj, "test1234");
	qDebug() << "Cipher:" << cipher;
	QJsonObject decryptObj = ServerTCPCommand::DecryptJSONObject(cipher, "test1234");

	QJsonDocument doc(decryptObj);
	qDebug() << "Json:" << doc.toJson();

	ServerTCPCommand *tcpCmd = new ServerTCPCommand("192.168.1.77", PORT_SERVER_BROADCAST, 1234, "MT768124E7022630");

	QObject::connect(tcpCmd, &ServerTCPCommand::MessageRecieved, [=](QString message)
	{
		ui.plainTextEdit->appendPlainText(message);
	});

	QObject::connect(ui.LoginButton, &QPushButton::clicked, [=]
	{
		tcpCmd->LoginToDevice();
	});

	QObject::connect(ui.GetStatusButton, &QPushButton::clicked, [=]
	{
		tcpCmd->GetSystemStatus();
	});

	QObject::connect(ui.ResetDefaultButton, &QPushButton::clicked, [=]
	{
		tcpCmd->ResetDevice();
	});

	QObject::connect(ui.ResetWIFIButton, &QPushButton::clicked, [=]
	{
		tcpCmd->ResetWIFI();
	});

	QObject::connect(ui.RebootButton, &QPushButton::clicked, [=]
	{
		tcpCmd->SystemReboot();
	});

	QObject::connect(ui.ChangeDeviceNameButton, &QPushButton::clicked, [=]
	{
		tcpCmd->ConfigDevice(Device_Config_Func::MODIFY_DEV_NAME, ui.DeviceNameLineEdit->text());
	});

	QObject::connect(ui.DeleteDeviceName, &QPushButton::clicked, [=]
	{
		tcpCmd->ConfigDevice(Device_Config_Func::DEL_DEV_NAME, "");
	});

	QObject::connect(ui.ChangeAdminPWButton, &QPushButton::clicked, [=]
	{
		tcpCmd->ConfigDevice(Device_Config_Func::MODIFY_ADMIN_PW, ui.DeviceNameLineEdit->text());
	});

	QObject::connect(ui.ChangeUserPWButton, &QPushButton::clicked, [=]
	{
		tcpCmd->ConfigDevice(Device_Config_Func::MODIFY_USER_PW, ui.DeviceNameLineEdit->text());
	});
	*/
}

ServerApp::~ServerApp()
{
	ServerCore::GetInstance()->stopServer();
}
