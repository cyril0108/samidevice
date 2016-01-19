#include "EventSender.h"

EventSender::EventSender(QObject* pParent)
: QObject( pParent )
{
}

EventSender::~EventSender()
{
}

void EventSender::Connect(EventSender* pToSender)
{
    connect(this, &EventSender::commandReturned, pToSender, &EventSender::commandReturned, Qt::UniqueConnection);
}

void EventSender::Disconnect(EventSender* pToSender)
{
    disconnect(this, &EventSender::commandReturned, pToSender, &EventSender::commandReturned);
}
