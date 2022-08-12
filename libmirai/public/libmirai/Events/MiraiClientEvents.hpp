#ifndef _MIRAI_MIRAI_CLIENT_EVENTS_HPP_
#define _MIRAI_MIRAI_CLIENT_EVENTS_HPP_

#include <exception>
#include <string>
#include <map>
#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

struct ClientConnectionEstablishedEvent
{
	std::string uri;
	std::map<std::string, std::string> headers;
	std::string protocol;
	std::string SessionKey;
	User BotProfile;
};

struct ClientConnectionErrorEvent
{
	uint32_t RetryCount = 0;
	double WaitTime = 0;
	int HttpStatus = 0;
	std::string reason;
	bool DecompressionError = false;
};

struct ClientConnectionClosedEvent
{
	uint16_t code;
	std::string reason;
	bool remote;
};

struct ClientParseErrorEvent
{
	ParseError error;
	std::string message;
};


}

#endif