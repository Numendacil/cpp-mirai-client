#ifndef _MIRAI_EXCEPTIONS_HPP_
#define _MIRAI_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>
namespace Mirai
{

class MiraiApiHttpException : public std::runtime_error
{
public:
	const int _code;
	const std::string _message;
	MiraiApiHttpException(int code, const std::string& message)
	: std::runtime_error("mirai-api-http error: " + message + " <" + std::to_string(code) + ">"),
	_code(code), _message(message) 
	{}

	virtual ~MiraiApiHttpException() = default;
};

#define REGISTER_STATUS_CODE(_name_, _code_)	\
class _name_ : public MiraiApiHttpException	\
{	\
public:	\
	_name_(const std::string& message) : MiraiApiHttpException(_code_, message) {}	\
};

REGISTER_STATUS_CODE(AuthKeyFail, 1)
REGISTER_STATUS_CODE(NoBot, 2)
REGISTER_STATUS_CODE(IllegalSession, 3)
REGISTER_STATUS_CODE(NotVerifySession, 4)
REGISTER_STATUS_CODE(NoElement, 5)
REGISTER_STATUS_CODE(NoOperateSupport, 6)
REGISTER_STATUS_CODE(NoSuchFile, 6)
REGISTER_STATUS_CODE(PermissionDenied, 10)
REGISTER_STATUS_CODE(BotMuted, 20)
REGISTER_STATUS_CODE(MessageTooLarge, 30)
// REGISTER_STATUS_CODE(InvalidParameter, 400)

#undef REGISTER_STATUS_CODE

class NetworkException : public std::runtime_error
{
public:
	const int _code;
	const std::string _message;
	NetworkException(int code, const std::string& message)
	: std::runtime_error("Network error: " + message + " <" + std::to_string(code) + ">"),
	_code(code), _message(message)
	{}

	virtual ~NetworkException() = default;
};

class ParseError : public std::runtime_error
{
public:
	const std::string _message;
	const std::string _error;
	ParseError(const std::string& error, const std::string& message)
	 : std::runtime_error("Unable to parse response \"" + message + "\": " + error), 
	_message(message), _error(error)
	{}
};

class TypeDismatch : public std::runtime_error
{
public:
	const std::string _expected_type;
	const std::string _received_type;

	TypeDismatch(const std::string& expected, const std::string& received)
	 : std::runtime_error("Expecting type " + expected + ", but get " + received + " instead"),
	_expected_type(expected), _received_type(received)
	{}
};

}


#endif