#pragma once
#include "servercore_global.h"
#include <QObject>

class QTcpSocket;

enum DES_Func
{
	DES_Decrypt = 0,
	DES_Encrypt = 1
};

enum class Device_Config_Func
{
	MODIFY_DEV_NAME,
	MODIFY_USER_PW,
	MODIFY_ADMIN_PW,
	DEL_DEV_NAME
};

class SERVERCORE_EXPORT ServerTCPCommand :
	public QObject
{
	Q_OBJECT

public:
	ServerTCPCommand();
	ServerTCPCommand( QString deviceIP, qint32 devicePort, qint32 deviceSeq, QString deviceUID);
	~ServerTCPCommand();
	void LoginToDevice();
	void LogoutDevice(bool encryptMsg = false);
	void ControlDevice(QString command, QString param, QString data, bool encryptMsg = false);
	void MobusConnect(QString serverAddress, qint32 serverPort);
	void MobusDisable();
	void ResetDevice();
	void ResetWIFI();
	void SystemReboot();
	void GetSystemStatus();
	void ConfigDevice(Device_Config_Func func, QString param);
	void ParseIncomingMessage();

//private:
	void ConnectToDevice();
	void DisconnectFromDevice();
	QString tcn1();
	QByteArray ReadMsg();
	void SendMsg(QByteArray sentMsg);
	static QString Bin2Ascii(QByteArray binData);
	static QString EncryptJSONObject(QJsonObject jsonObj, QString encryptKey);
	static QJsonObject DecryptJSONObject(QString cipher, QString decryptKey);
	static QString DES(QString input, QString desKey, DES_Func func);

signals:
	void MessageRecieved(QString message);

private:
	QTcpSocket *socket;

	QString deviceIP;
	qint32 devicePort;
	qint32 deviceSeq;
	QString deviceUID;

	QString username;
	QString password;
	qint32 randomNum;
	QString lang;

	qint32 deviceCID;
};

