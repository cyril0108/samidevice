#pragma once
#include "servercore_global.h"
#include "common.h"
#include <QString>
#include <QVariantMap>

class SERVERCORE_EXPORT JsonGenerator
{
public:
	static QString JsonGenerator::GenerateJsonCommand(TCP_COMMAND_TYPE cmdType, QVariantMap paramMap);
	static QString JsonGenerator::GenerateJsonReply(TCP_REPLY_TYPE replyType, QVariantMap paramMap);
};

