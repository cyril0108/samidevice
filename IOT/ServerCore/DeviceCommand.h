#pragma once
#include "servercore_global.h"
#include "common.h"
#include <QString>
#include <QDataStream>

class SERVERCORE_EXPORT DeviceCommand
{
public:
    DeviceCommand();
    DeviceCommand( QString commandName, QString commandDisplayName = "undefined command", PARAM_TYPE paramType = PARAM_TYPE::NONE,
		QString paramMin = "", QString paramMax = "");
	~DeviceCommand();

	bool isValidParam(QString param) const;

	QString getCommandName() const { return commandName; }
	QString getCommandDisplayName() const { return commandDisplayName; }
	PARAM_TYPE getParamType() const { return paramType; }
	QString getParamMin() const { return paramMin; }
	QString getParamMax() const { return paramMax; }

	friend QDataStream& operator<<(QDataStream& out, const DeviceCommand& cmd);
	friend QDataStream& operator>>(QDataStream& in, DeviceCommand& cmd);

private:
	QString commandName;
	QString commandDisplayName;
	PARAM_TYPE paramType;
	QString paramMin;
	QString paramMax;
};

