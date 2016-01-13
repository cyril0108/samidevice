//#include "ServerUtil.h"
#include <QTime>

template <typename T>
bool ServerUtil<T>::srandOnce = true;

template <typename T>
T ServerUtil<T>::GenerateRandomNumber()
{
	if (srandOnce)
	{
		qsrand(static_cast<uint>(QTime::currentTime().msec()));
		srandOnce = false;
	}
	
	return (qrand() % std::numeric_limits<T>::max());
}