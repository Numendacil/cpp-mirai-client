// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MIRAI_EXCEPTIONS_HPP_
#define MIRAI_EXCEPTIONS_HPP_

#include <exception>
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
	int code;
	/// 错误信息
	std::string message;
	MiraiApiHttpException(int code, std::string message)
		: std::runtime_error("mirai-api-http error: " + message + " <" + std::to_string(code) + ">")
		, code(code)
		, message(std::move(message))
	{
	}
};

#define REGISTER_STATUS_CODE(name, code)                                                                           \
	class name : public MiraiApiHttpException                                                                        \
	{                                                                                                                  \
	public:                                                                                                            \
		name(std::string& message) : MiraiApiHttpException(code, message) {}                                 \
	};

/// MAH异常: 验证密钥错误
REGISTER_STATUS_CODE(AuthKeyFail, 1)
/// MAH异常: 不存在该Bot账号
REGISTER_STATUS_CODE(NoBot, 2)
/// MAH异常: 无效Session
REGISTER_STATUS_CODE(IllegalSession, 3)
/// MAH异常: Session未绑定账号
REGISTER_STATUS_CODE(NotVerifySession, 4)
/// MAH异常: 指定对象不存在
REGISTER_STATUS_CODE(NoElement, 5)
/// MAH异常: 不支持该操作
REGISTER_STATUS_CODE(NoOperateSupport, 6)
/// MAH异常: 文件不存在
REGISTER_STATUS_CODE(NoSuchFile, 6)
/// MAH异常: 缺少相关权限
REGISTER_STATUS_CODE(PermissionDenied, 10)
/// MAH异常: Bot被禁言
REGISTER_STATUS_CODE(BotMuted, 20)
/// MAH异常: 消息过长
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
	int code;
	/// 错误信息
	std::string message;
	NetworkException(int code, std::string message)
		: std::runtime_error("Network error: " + message + " <" + std::to_string(code) + ">")
		, code(code)
		, message(std::move(message))
	{
	}
};

/**
 * @brief 消息解析错误
 * 
 */
class ParseError : public std::runtime_error
{
public:
	/// 原被解析消息
	std::string message;
	/// 错误消息
	std::string error;
	ParseError(std::string error, std::string message)
		: std::runtime_error("Unable to parse \"" + message + "\": " + error), message(std::move(message)), error(std::move(error))
	{
	}
};

/**
 * @brief 类型匹配错误
 * 
 */
class TypeDismatchError : public std::runtime_error
{
public:
	/// 目标类型
	std::string expected_type;
	/// 实际类型
	std::string received_type;

	TypeDismatchError(std::string expected, std::string received)
		: std::runtime_error("Expecting type " + expected + ", but get " + received + " instead")
		, expected_type(std::move(expected))
		, received_type(std::move(received))
	{
	}
};

/**
 * @brief 未实现方法
 * 
 */
class NotImplementedError : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

} // namespace Mirai


#endif