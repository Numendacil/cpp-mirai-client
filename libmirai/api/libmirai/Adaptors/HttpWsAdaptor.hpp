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

#ifndef MIRAI_HTTP_WEBSOCKET_ADAPTOR_HPP_
#define MIRAI_HTTP_WEBSOCKET_ADAPTOR_HPP_

#include <memory>

#include "IAdaptor.hpp"

/// 所有mirai相关的对象的命名空间
namespace Mirai
{

struct HttpWsAdaptorConfig
{
	/// Http adapter地址
	std::string HttpUrl = "http://localhost:8080";
	/// 连接超时时间
	std::chrono::milliseconds ConnectionTimeout = std::chrono::seconds(30);
	/// 读取超时时间
	std::chrono::milliseconds ReadTimeout = std::chrono::seconds(5);
	/// 写入超时时间
	std::chrono::milliseconds WriteTimeout = std::chrono::seconds(5);

	/// Websocket adapter地址
	std::string WebsocketUrl = "ws://localhost:8080";
	/// 心跳信息间隔
	std::chrono::seconds HeartbeatInterval = std::chrono::seconds(60);
	/// 握手超时时间
	std::chrono::seconds HandshakeTimeout = std::chrono::seconds(60);
	/// 开启自动回复ping
	bool EnablePong = true;
	/// 开启数据压缩
	bool EnableDeflate =
#ifdef IXWEBSOCKET_USE_ZLIB
		true
#else
		false
#endif
		;
	/// 开启自动重连
	bool AutoReconnect = true;
	/// 最小重连时间间隔
	std::chrono::milliseconds MinRetryInterval = std::chrono::seconds(1);
	/// 最大重连时间间隔
	std::chrono::milliseconds MaxRetryInterval = std::chrono::seconds(30);

	/// BotQQ
	QQ_t BotQQ = 12345_qq;
	/// 验证密钥
	std::string VerifyKey = "";

	/// 从JSON文件中读取配置
	void FromJsonFile(const std::string& path);

	/// 从TOML文件中读取配置
	void FromTOMLFile(const std::string& path);
};

std::unique_ptr<IAdaptor> MakeHttpWsAdaptor(HttpWsAdaptorConfig config);

} // namespace Mirai

#endif