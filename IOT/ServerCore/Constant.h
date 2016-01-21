#pragma once

#define JSON_KEY_COMMAND QString( "command" )
#define JSON_KEY_SEQ QString( "seq" )
#define JSON_KEY_UID QString( "uid" )
#define JSON_KEY_DISPLAYNAME QString( "DisplayName" )
#define JSON_KEY_IP QString( "ip" )
#define JSON_KEY_TYPE QString( "type" )
#define JSON_KEY_STATUS QString( "status" )
#define JSON_KEY_SUPPORTCMDS QString( "SupportCmds" )
#define JSON_KEY_CMD_DISPLAYNAME QString( "command_displayName" )
#define JSON_KEY_PARAM_TYPE QString( "param_type" )
#define JSON_KEY_PARAM_MIN QString( "param_min" )
#define JSON_KEY_PARAM_MAX QString( "param_max" )
#define JSON_KEY_DISPLAYINFO QString( "DisplayInfo" )
#define JSON_KEY_PARAM QString( "param" )
#define JSON_KEY_DEVICES QString( "devices" )
#define JSON_KEY_DEVICECMD QString( "deviceCmd" )
#define JSON_KEY_RETURN_TYPE QString( "return_type" )

#define CMD_QUERY QString( "query" )
#define CMD_QUERY_DEVICE_SUPPORT_CMDS QString( "supportCmds" )
#define CMD_QUERY_DEVICE_DISPLAY_INFO QString( "queryDisplayInfo" )

#define SERVER_CMD_LIST_DEVICES QString("listDevices")
#define SERVER_CMD_LIST_COMMANDS QString("listCommands")
#define SERVER_CMD_SEND_CMD_TO_DEVICE QString("sendCommandToDevice")

const int PORT_SERVER_BROADCAST = 3479;
const int INTERVAL_SERVER_BROADCAST = 5000;

const int STATUS_SUCCESSFUL = 200;
const int STATUS_FAIL = 404;