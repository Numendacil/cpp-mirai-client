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

#ifndef _MIRAI_MESSAGE_SERVER_IMPL_HPP_
#define _MIRAI_MESSAGE_SERVER_IMPL_HPP_

#include <functional>
#include <string>
#include <vector>

#include <ixwebsocket/IXWebSocket.h>

namespace Mirai::Details
{

class MessageClientImpl
{
protected:
	friend class MiraiClient;

	ix::WebSocket _client;
	// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
	static bool _init; // For calling WSAstartup once per program

public:
	template<typename... Args> using Callback = std::function<void(Args...)>;

	MessageClientImpl(std::chrono::seconds HeartbeatInterval = std::chrono::seconds(60),
	                  std::chrono::seconds HandshakeTimeout = std::chrono::seconds(60), bool EnablePong = true,
	                  bool EnableDeflate =
#ifdef IXWEBSOCKET_USE_ZLIB
	                      true
#else
	                      false
#endif
	                  ,
	                  bool AutoReconnect = true, std::chrono::milliseconds MinRetryInterval = std::chrono::seconds(1),
	                  std::chrono::milliseconds MaxRetryInterval = std::chrono::seconds(60));
	MessageClientImpl(const MessageClientImpl&) = delete;
	MessageClientImpl(MessageClientImpl&&) noexcept = delete;
	MessageClientImpl& operator=(const MessageClientImpl&) = delete;
	MessageClientImpl& operator=(MessageClientImpl&&) noexcept = delete;

	~MessageClientImpl();

	void Connect(const std::string& url);
	bool isConnected() const
	{
		return this->_client.getReadyState() == ix::ReadyState::Open;
	};
	void Disconnect();

	void OnText(Callback<const std::string&> TextCallback)
	{
		this->_TextCallback = TextCallback;
	}
	void OnBinary(Callback<const std::string&> BinaryCallback)
	{
		this->_BinaryCallback = BinaryCallback;
	}
	void OnOpen(Callback<const ix::WebSocketOpenInfo&> OpenCallback)
	{
		this->_OpenCallback = OpenCallback;
	}
	void OnError(Callback<const ix::WebSocketErrorInfo&> ErrorCallback)
	{
		this->_ErrorCallback = ErrorCallback;
	}
	void OnClose(Callback<const ix::WebSocketCloseInfo&> CloseCallback)
	{
		this->_CloseCallback = CloseCallback;
	}
	void OnPing(Callback<const std::string&> PingCallback)
	{
		this->_PingCallback = PingCallback;
	}
	void OnPong(Callback<const std::string&> PongCallback)
	{
		this->_PongCallback = PongCallback;
	}


protected:
	Callback<const std::string&> _TextCallback;
	Callback<const std::string&> _BinaryCallback;
	Callback<const ix::WebSocketOpenInfo&> _OpenCallback;
	Callback<const ix::WebSocketErrorInfo&> _ErrorCallback;
	Callback<const ix::WebSocketCloseInfo&> _CloseCallback;
	Callback<const std::string&> _PingCallback;
	Callback<const std::string&> _PongCallback;
};
} // namespace Mirai::Details

#endif