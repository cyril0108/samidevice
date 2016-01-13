#pragma once
#include "servercore_global.h"
#include <QObject>
#include <QMap>
#include "ServerTCPCommand.h"

class SERVERCORE_EXPORT TCPConnectManager :
	public QObject
{
	Q_OBJECT

public:
	static TCPConnectManager *GetInstance();

private:
	static TCPConnectManager *thePointer;

public:
	TCPConnectManager();
	~TCPConnectManager();

private:
	QMap <QString, ServerTCPCommand> connectionMap;
};

