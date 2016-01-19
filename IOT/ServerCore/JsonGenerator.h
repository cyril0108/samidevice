#pragma once
#include "servercore_global.h"
#include "common.h"
#include <QString>
#include <QVariantMap>

class SERVERCORE_EXPORT JsonGenerator
{
public:
    static QString GenerateJsonCommand(TCP_COMMAND_TYPE cmdType, QVariantMap paramMap);
    static QString GenerateJsonReply(TCP_REPLY_TYPE replyType, QVariantMap paramMap);
};

