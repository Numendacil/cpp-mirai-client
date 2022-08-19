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

#ifndef _MIRAI_MIRAI_CLIENT_EVENTS_HPP_
#define _MIRAI_MIRAI_CLIENT_EVENTS_HPP_

#include <exception>
#include <map>
#include <string>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

/**
 * @brief 连接建立事件
 * 
 * 会在连接成功建立后， `MiraiClient::Connect()` 返回之前广播。
 * `MiraiClient` 的mirai相关api应在此事件之后才能被正常使用。
 */
struct ClientConnectionEstablishedEvent
{
	/// 连接地址
	std::string uri;
	/// 接收的文件头信息
	std::map<std::string, std::string> headers;
	/// 协议信息
	std::string protocol;
	/// 本次连接的session key
	std::string SessionKey;
	/// 获取到的的Bot资料
	User BotProfile;
};

/**
 * @brief 连接错误事件
 * 
 * 通常出现于网络问题导致的无法连接目标服务器。
 * 连接建立后出现的错误将导致 `ClientConnectionClosedEvent`
 */
struct ClientConnectionErrorEvent
{
	/// 当前重试次数
	uint32_t RetryCount = 0;
	/// 下一次重试前的等待时间
	double WaitTime = 0;
	/// Http连接状态
	int HttpStatus = 0;
	/// 错误原因
	std::string reason;
	/// 是否为解压缩编码错误
	bool DecompressionError = false;
};

/**
 * @brief 连接丢失事件
 * 
 * 只可能在 `ClientConnectionEstablishedEvent` 之后发生，表明已建立的连接因为某种原因关闭了。
 * 若是 `MiraiClient::Disconnect()` 导致的连接关闭，该事件将会在函数返回之前被广播。
 * 连接关闭后不再应该进行任何mirai api的使用，直到下次连接建立接收到 `ClientConnectionEstablishedEvent` 为止。
 * 若开启了自动重连选项，`MiraiClient` 会在非客户端主动关闭导致的连接丢失后尝试自动重连。重连过程中的错误将会触发 `ClientConnectionErrorEvent`
 */
struct ClientConnectionClosedEvent
{
	/// 信息码
	uint16_t code;
	/// 连接关闭原因
	std::string reason;
	/// 是否由远程服务器关闭的连接
	bool remote;
};

/**
 * @brief 消息解析错误事件
 * 
 * 从mirai-api-http接收到了格式错误的信息，可能是版本不同或bug导致
 */
struct ClientParseErrorEvent
{
	/// 错误消息
	ParseError error;
	/// 接收到的原消息
	std::string message;
};


} // namespace Mirai

#endif