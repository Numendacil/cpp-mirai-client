#ifndef _MIRAI_MESSAGE_SERVER_IMPL_HPP_
#define _MIRAI_MESSAGE_SERVER_IMPL_HPP_

#include <string>
#include <vector>
#include <functional>

#include <ixwebsocket/IXWebSocket.h>

namespace Mirai::Details
{

class MessageClientImpl
{
protected:
	friend class MiraiClient;
	
	ix::WebSocket _client;
	static bool _init;	// For calling WSAstartup once per program

public:
	template<typename ...Args>
	using Callback = std::function<void(Args...)>;

	MessageClientImpl(
		std::chrono::seconds HeartbeatInterval = std::chrono::seconds(60), 
		std::chrono::seconds HandshakeTimeout = std::chrono::seconds(60), 
		bool EnablePong = true,
		bool EnableDeflate =
		#ifdef IXWEBSOCKET_USE_ZLIB
			true
		#else
			false
		#endif
		,
		bool AutoReconnect = true, 
		std::chrono::milliseconds MinRetryInterval = std::chrono::seconds(1),
		std::chrono::milliseconds MaxRetryInterval = std::chrono::seconds(60)
	);
	MessageClientImpl(const MessageClientImpl&) = delete;
	MessageClientImpl(MessageClientImpl&&) noexcept = delete;
	MessageClientImpl& operator=(const MessageClientImpl&) = delete;
	MessageClientImpl& operator=(MessageClientImpl&&) noexcept = delete;

	~MessageClientImpl();

	void Connect(const std::string& url);
	bool isConnected() const { return this->_client.getReadyState() == ix::ReadyState::Open; };
	void Disconnect();

	void OnText(Callback<const std::string&> TextCallback) { this->_TextCallback = TextCallback; }
	void OnBinary(Callback<const std::string&> BinaryCallback) { this->_BinaryCallback = BinaryCallback; }
	void OnOpen(Callback<const ix::WebSocketOpenInfo&> OpenCallback) { this->_OpenCallback = OpenCallback; }
	void OnError(Callback<const ix::WebSocketErrorInfo&> ErrorCallback) { this->_ErrorCallback = ErrorCallback; }
	void OnClose(Callback<const ix::WebSocketCloseInfo&> CloseCallback) { this->_CloseCallback = CloseCallback; }
	void OnPing(Callback<const std::string&> PingCallback) { this->_PingCallback = PingCallback; }
	void OnPong(Callback<const std::string&> PongCallback) { this->_PongCallback = PongCallback; }


protected:
	Callback<const std::string&> _TextCallback;
	Callback<const std::string&> _BinaryCallback;
	Callback<const ix::WebSocketOpenInfo&> _OpenCallback;
	Callback<const ix::WebSocketErrorInfo&> _ErrorCallback;
	Callback<const ix::WebSocketCloseInfo&> _CloseCallback;
	Callback<const std::string&> _PingCallback;
	Callback<const std::string&> _PongCallback;
};
}

#endif