#ifndef _MIRAI_EXCEPTIONS_HPP_
#define _MIRAI_EXCEPTIONS_HPP_

#include <stdexcept>
#include <string>
namespace Mirai
{

/**
 * @brief mirai-api-http相关的异常
 * 
 */
class MiraiApiHttpException : public std::runtime_error
{
public:
	/// 错误码
	const int _code;
	/// 错误信息
	const std::string _message;
	MiraiApiHttpException(int code, const std::string& message)
		: std::runtime_error("mirai-api-http error: " + message + " <" + std::to_string(code) + ">")
		, _code(code)
		, _message(message)
	{
	}

	virtual ~MiraiApiHttpException() = default;
};

#define REGISTER_STATUS_CODE(_name_, _code_)                                                                           \
	class _name_ : public MiraiApiHttpException                                                                        \
	{                                                                                                                  \
	public:                                                                                                            \
		_name_(const std::string& message) : MiraiApiHttpException(_code_, message) {}                                 \
	};

/// 验证密钥错误
REGISTER_STATUS_CODE(AuthKeyFail, 1)
/// 不存在该Bot账号
REGISTER_STATUS_CODE(NoBot, 2)
/// 无效Session
REGISTER_STATUS_CODE(IllegalSession, 3)
/// Session未绑定账号
REGISTER_STATUS_CODE(NotVerifySession, 4)
/// 指定对象不存在
REGISTER_STATUS_CODE(NoElement, 5)
/// 不支持该操作
REGISTER_STATUS_CODE(NoOperateSupport, 6)
/// 文件不存在
REGISTER_STATUS_CODE(NoSuchFile, 6)
/// 缺少相关权限
REGISTER_STATUS_CODE(PermissionDenied, 10)
/// Bot被禁言
REGISTER_STATUS_CODE(BotMuted, 20)
/// 消息过长
REGISTER_STATUS_CODE(MessageTooLarge, 30)
// REGISTER_STATUS_CODE(InvalidParameter, 400)

#undef REGISTER_STATUS_CODE

/**
 * @brief 网络通信错误
 * 
 */
class NetworkException : public std::runtime_error
{
public:
	/// 错误码
	const int _code;
	/// 错误信息
	const std::string _message;
	NetworkException(int code, const std::string& message)
		: std::runtime_error("Network error: " + message + " <" + std::to_string(code) + ">")
		, _code(code)
		, _message(message)
	{
	}

	virtual ~NetworkException() = default;
};

/**
 * @brief 消息解析错误
 * 
 */
class ParseError : public std::runtime_error
{
public:
	/// 原被解析消息
	const std::string _message;
	/// 错误消息
	const std::string _error;
	ParseError(const std::string& error, const std::string& message)
		: std::runtime_error("Unable to parse \"" + message + "\": " + error), _message(message), _error(error)
	{
	}
};

/**
 * @brief 类型匹配错误
 * 
 */
class TypeDismatch : public std::runtime_error
{
public:
	/// 目标类型
	const std::string _expected_type;
	/// 实际类型
	const std::string _received_type;

	TypeDismatch(const std::string& expected, const std::string& received)
		: std::runtime_error("Expecting type " + expected + ", but get " + received + " instead")
		, _expected_type(expected)
		, _received_type(received)
	{
	}
};

} // namespace Mirai


#endif