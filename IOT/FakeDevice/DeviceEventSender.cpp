#include "DeviceEventSender.h"

DeviceEventSender::DeviceEventSender(QObject* pParent)
: QObject( pParent )
{
}

DeviceEventSender::~DeviceEventSender()
{
}

void DeviceEventSender::Connect(DeviceEventSender* pToSender)
{
    connect(this, &DeviceEventSender::commandReturned, pToSender, &DeviceEventSender::commandReturned, Qt::UniqueConnection);
}

void DeviceEventSender::Disconnect(DeviceEventSender* pToSender)
{
    disconnect(this, &DeviceEventSender::commandReturned, pToSender, &DeviceEventSender::commandReturned);
}
