#include "DeviceCommand.h"


DeviceCommand::DeviceCommand(QString commandName, QString commandDisplayName, PARAM_TYPE paramType,
	QString paramMin, QString paramMax) : commandName(commandName), commandDisplayName(commandDisplayName)
	, paramType(paramType), paramMin(paramMin), paramMax(paramMax)
{
}


DeviceCommand::~DeviceCommand()
{
}

bool DeviceCommand::isValidParam(QString param) const
{
	bool ret = false;

	switch (paramType)
	{
	case PARAM_TYPE::NONE:
	case PARAM_TYPE::TEXT:
		ret = true;
		break;
	case PARAM_TYPE::INTEGER:
	{
		int intMax = paramMax.toInt();
		int intMin = paramMin.toInt();
		bool ok = false;
		int intParam = param.toInt(&ok);
		ret = ok && (intParam >= intMin && intParam <= intMax);
	}
		break;
		
	case PARAM_TYPE::FLOAT:
	{
		float floatMax = paramMax.toFloat();
		float floatMin = paramMin.toFloat();
		bool ok = false;
		float floatParam = param.toFloat(&ok);
		ret = ok && (floatParam >= floatMin && floatParam <= floatMax);
	}
		break;
	
	default:
		break;
	}

	return ret;
}

QDataStream& operator<<(QDataStream& out, const DeviceCommand& cmd)
{
	out << cmd.getCommandName() << cmd.getCommandDisplayName() << (int)cmd.getParamType()
		<< cmd.getParamMin() << cmd.getParamMax();
	return out;
}

QDataStream& operator>>(QDataStream& in, DeviceCommand& cmd)
{
	int param_type = 0;
	in >> cmd.commandName >> cmd.commandDisplayName >> param_type >> cmd.paramMin >> cmd.paramMax;
	cmd.paramType = (PARAM_TYPE)param_type;
	return in;
}
