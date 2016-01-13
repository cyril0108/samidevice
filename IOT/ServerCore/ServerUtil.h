#pragma once
#include "servercore_global.h"

template <typename T>
class SERVERCORE_EXPORT ServerUtil
{

public:
	static T GenerateRandomNumber();

private:
	static bool srandOnce;
};

#include "ServerUtil.cpp"