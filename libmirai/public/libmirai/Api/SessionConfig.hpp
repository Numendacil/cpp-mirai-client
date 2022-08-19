#ifndef _MIRAI_SESSION_CONFIG_HPP_
#define _MIRAI_SESSION_CONFIG_HPP_

#include <chrono>
#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

/**
 * @brief 连接mirai-api-http的配置
 * 
 */
struct SessionConfigs
{
	/// Http Adaptor地址
	std::string HttpUrl = "http://localhost:8080";
	/// 连接超时时间
	std::chrono::milliseconds ConnectionTimeout = std::chrono::seconds(30);
	/// 读取超时时间
	std::chrono::milliseconds ReadTimeout = std::chrono::seconds(5);
	/// 写入超时时间
	std::chrono::milliseconds WriteTimeout = std::chrono::seconds(5);

	/// Websocket Adaptor地址
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

	/// 线程池大小
	std::size_t ThreadPoolSize = 12;

	/// 从JSON文件中读取配置
	void FromFile(const std::string& path);
	/// 从JSON对象中读取配置
	void FromJson(const nlohmann::json& json_config);
};

} // namespace Mirai

#endif