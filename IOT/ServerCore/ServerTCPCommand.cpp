#include "ServerTCPCommand.h"
#include "ServerUtil.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QDebug>
#include <openssl/des.h>
#include <openssl/rand.h>
#define BUFSIZE (64)

ServerTCPCommand::ServerTCPCommand()
{

}

ServerTCPCommand::ServerTCPCommand(QString deviceIP, int devicePort, qint32 deviceSeq, QString deviceUID)
: deviceIP(deviceIP), devicePort(devicePort), deviceSeq(deviceSeq), deviceUID(deviceUID)
{
	socket = new QTcpSocket(this);

	QObject::connect(socket, &QTcpSocket::readyRead, [=]
	{
		ParseIncomingMessage();
	});

	QObject::connect(socket, &QTcpSocket::disconnected, [=]
	{
		qDebug() << "Disconnected.";
	});

	deviceCID = -1;
	randomNum = 0;

	username = "admin";
	password = "999999";
	lang = "en";
}


ServerTCPCommand::~ServerTCPCommand()
{
	if (socket)
	{
		DisconnectFromDevice();
		socket->deleteLater();
	}
}

void ServerTCPCommand::ConnectToDevice()
{
	socket->connectToHost(deviceIP, devicePort, QIODevice::ReadWrite);
}

void ServerTCPCommand::DisconnectFromDevice()
{
	if (socket)
	{
		socket->disconnectFromHost();
	}
}

void ServerTCPCommand::LoginToDevice()
{
	ConnectToDevice();

	randomNum = ServerUtil<qint32>::GenerateRandomNumber();

	QJsonObject jsonObject;
	jsonObject.insert("seq", deviceSeq);
	jsonObject.insert("cmd", "login");
	jsonObject.insert("uid", deviceUID);
	jsonObject.insert("username", username);
	jsonObject.insert("RN", randomNum);
	jsonObject.insert("tcn1", tcn1());
	jsonObject.insert("lang", lang);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();
	
	SendMsg(byteData);
}

void ServerTCPCommand::LogoutDevice(bool encryptMsg)
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);

	QJsonObject cryptObj;
	cryptObj.insert("cmd", "logout");

	if (encryptMsg)
	{
		QString encryptMsg = ServerTCPCommand::EncryptJSONObject(cryptObj, password);
		jsonObject.insert("crypto", encryptMsg);
	}
	else
	{
		jsonObject.insert("crypto", cryptObj);
	}

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::ControlDevice(QString command, QString param, QString data, bool encryptMsg)
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);

	QJsonObject cryptObj;
	cryptObj.insert("cmd", command);
	cryptObj.insert("param", param);
	cryptObj.insert("data", data);

	if (encryptMsg)
	{
		QString encryptMsg = ServerTCPCommand::EncryptJSONObject(cryptObj, password);
		jsonObject.insert("crypto", encryptMsg);
	}
	else
	{
		jsonObject.insert("crypto", cryptObj);
	}

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::MobusConnect(QString serverAddress, qint32 serverPort)
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "mobus");

	QString paramText = "-a" + serverAddress + " -p" + QString::number(serverPort) 
		+ " -c" + QString::number(deviceCID) + " -g0 -r0 -t1 -b0 -s";
	cryptObj.insert("param", paramText);
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::MobusDisable()
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "modbus");

	jsonObject.insert("crypto", cryptObj);
	cryptObj.insert("param", "-g1 - s");
	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::ResetDevice()
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "default");
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::ResetWIFI()
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "ap");
	cryptObj.insert("param", "-c");
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::SystemReboot()
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "reboot");
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::GetSystemStatus()
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "stat");
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::ConfigDevice(Device_Config_Func func, QString param)
{
	ConnectToDevice();

	QJsonObject jsonObject;
	jsonObject.insert("cid", deviceCID);
	jsonObject.insert("seq", deviceSeq);
	QJsonObject cryptObj;
	cryptObj.insert("cmd", "cfg");

	QString paramText;

	switch (func)
	{
	case Device_Config_Func::MODIFY_DEV_NAME:
		paramText = "-e28 -c" + param;
		break;
	case Device_Config_Func::MODIFY_USER_PW:
		paramText = "-e18 -c" + param;
		break;
	case Device_Config_Func::MODIFY_ADMIN_PW:
		paramText = "-e20 -c" + param;
		break;
	case Device_Config_Func::DEL_DEV_NAME:
		paramText = "-d28";
		break;
	default:

		break;
	}


	cryptObj.insert("param", paramText);
	jsonObject.insert("crypto", cryptObj);

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();

	SendMsg(byteData);
}

void ServerTCPCommand::ParseIncomingMessage()
{
	QByteArray incomeMsg = ReadMsg();

	QJsonDocument jsonDoc = QJsonDocument::fromJson(incomeMsg);
	QJsonObject jsonObj = jsonDoc.object();
	if (jsonObj.contains("cid"))
	{
		deviceCID = jsonObj.take("cid").toInt();
	}

	qDebug() << "TCP: " << incomeMsg;

	Q_EMIT MessageRecieved(incomeMsg);
}

QByteArray ServerTCPCommand::ReadMsg()
{
	QByteArray dataArray = socket->readAll();
	return dataArray;
}

void ServerTCPCommand::SendMsg(QByteArray sentMsg)
{
	socket->write(sentMsg);
}

QString ServerTCPCommand::tcn1()
{
	QString plainText = deviceUID + password + QString::number(randomNum);
	QByteArray tcn1 = QCryptographicHash::hash((plainText.toLocal8Bit()), QCryptographicHash::Md5);
	return ServerTCPCommand::Bin2Ascii(tcn1);
}


/////////////////////////////////////////////////////////////////

QString ServerTCPCommand::Bin2Ascii(QByteArray binData)
{
	QByteArray ascii_num = "0123456789abcdef";
	QByteArray retArray;

	char data[3] = { '\0' };

	for (int i = 0; i < binData.size(); ++i)
	{
		data[0] = ascii_num[(binData[i] & 0xf0) >> 4];
		data[1] = ascii_num[(binData[i] & 0x0f)];
		retArray.append(data[0]);
		retArray.append(data[1]);
	}

	return QString::fromLocal8Bit(retArray);
}

QString ServerTCPCommand::EncryptJSONObject(QJsonObject jsonObj, QString encryptKey)
{
	QJsonDocument jsonDoc(jsonObj);
	QByteArray byteData = jsonDoc.toJson();
	return ServerTCPCommand::DES(byteData, encryptKey, DES_Encrypt).toLatin1().toBase64();
}

QJsonObject ServerTCPCommand::DecryptJSONObject(QString cipher, QString decryptKey)
{
	QString base64Cipher = QString::fromLatin1(QByteArray::fromBase64(cipher.toLatin1()));
	QJsonDocument jsonDoc = QJsonDocument::fromJson(ServerTCPCommand::DES(base64Cipher, decryptKey, DES_Decrypt).toLatin1());
	return jsonDoc.object();
}

QString ServerTCPCommand::DES(QString input, QString desKey, DES_Func func)
{
	unsigned char out[BUFSIZE], tmp[BUFSIZE];

	const unsigned char *key_ptr = (const unsigned char*)desKey.toLocal8Bit().constData();

	DES_cblock key = { key_ptr[0], key_ptr[1], key_ptr[2], key_ptr[3], key_ptr[4], key_ptr[5], key_ptr[6], key_ptr[7] };

	DES_key_schedule keysched;

	memset(tmp, 0, sizeof(out));
	memset(out, 0, sizeof(out));

	//RAND_seed(seed, sizeof(DES_cblock));
	//DES_random_key(&key);

	DES_set_key((C_Block *)key, &keysched);

	/* 8 bytes of plaintext */

	QString retString;
	
	for (int i = 0; i < input.size(); i += 8)
	{
		memset(out, 0, sizeof(out));
		memset(tmp, 0, sizeof(tmp));
		QString block = input.mid(i, 8);
		strcpy((char*)tmp, block.toLatin1().constData());
		DES_ecb_encrypt((C_Block *)tmp, (C_Block *)out, &keysched, func);
		retString.append(QString::fromLatin1((char*)out));
	}

	return retString;
}
