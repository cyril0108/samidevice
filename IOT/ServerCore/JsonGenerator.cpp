#include "JsonGenerator.h"
#include "Constant.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

QString JsonGenerator::GenerateJsonCommand(TCP_COMMAND_TYPE cmdType, QVariantMap paramMap)
{
	QJsonObject jsonObject;
	
	switch (cmdType)
	{
	case TCP_COMMAND_TYPE::QUERY_DEVICE_SUPPORT_CMDS:
	{
		jsonObject.insert(JSON_KEY_COMMAND, CMD_QUERY_DEVICE_SUPPORT_CMDS);
		jsonObject.insert(JSON_KEY_SEQ, paramMap[JSON_KEY_SEQ].toInt());
	}
		break;
	case TCP_COMMAND_TYPE::QUERY_DEVICE_DISPLAY_INFO:
	{
		jsonObject.insert(JSON_KEY_COMMAND, CMD_QUERY_DEVICE_DISPLAY_INFO);
		jsonObject.insert(JSON_KEY_SEQ, paramMap[JSON_KEY_SEQ].toInt());
	}
		break;
	case TCP_COMMAND_TYPE::SEND_COMMAND_TO_DEVICE:
	{
		jsonObject.insert(JSON_KEY_COMMAND, paramMap[JSON_KEY_COMMAND].toString());
		jsonObject.insert(JSON_KEY_PARAM, paramMap[JSON_KEY_PARAM].toString());
		jsonObject.insert(JSON_KEY_SEQ, paramMap[JSON_KEY_SEQ].toInt());
	}
		break;
	default:
		return "";
		break;
	}

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();
	return QString(byteData);
}

QString JsonGenerator::GenerateJsonReply(TCP_REPLY_TYPE replyType, QVariantMap paramMap)
{
	QJsonObject jsonObject;

	switch (replyType)
	{
	case TCP_REPLY_TYPE::QUERY_DEVICES:
	{
		jsonObject.insert(JSON_KEY_DEVICES, paramMap[JSON_KEY_DEVICES].toJsonArray());
		jsonObject.insert(JSON_KEY_STATUS, paramMap[JSON_KEY_STATUS].toInt());
	}
		break;
	case TCP_REPLY_TYPE::FORWARD_COMMAND_TO_DEVICE:
	{
		jsonObject.insert(JSON_KEY_STATUS, paramMap[JSON_KEY_STATUS].toInt());
	}
		break;
	default:
		return "";
		break;
	}

	QJsonDocument jsonDoc(jsonObject);
	QByteArray byteData = jsonDoc.toJson();
	return QString(byteData);
}