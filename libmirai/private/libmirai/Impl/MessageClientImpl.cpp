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

#include "MessageClientImpl.hpp"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocketMessageType.h>

namespace Mirai::Details
{

bool MessageClientImpl::_init = false;	// NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MessageClientImpl::MessageClientImpl(std::chrono::seconds HeartbeatInterval, std::chrono::seconds HandshakeTimeout,
                                     bool EnablePong, bool EnableDeflate, bool AutoReconnect,
                                     std::chrono::milliseconds MinRetryInterval,
                                     std::chrono::milliseconds MaxRetryInterval)
{
	if (!MessageClientImpl::_init)
	{
		ix::initNetSystem();
		MessageClientImpl::_init = true;
	}
	this->_client.setPingInterval(static_cast<int>(HeartbeatInterval.count()));
	this->_client.setHandshakeTimeout(static_cast<int>(HandshakeTimeout.count()));
	if (!EnablePong) this->_client.disablePong();
	if (EnableDeflate) this->_client.enablePerMessageDeflate();
	if (AutoReconnect)
	{
		this->_client.enableAutomaticReconnection();
		this->_client.setMinWaitBetweenReconnectionRetries(MinRetryInterval.count());
		this->_client.setMaxWaitBetweenReconnectionRetries(MaxRetryInterval.count());
	}
	else
		this->_client.disableAutomaticReconnection();
}

MessageClientImpl::~MessageClientImpl()
{
	if (this->isConnected()) this->_client.stop();
	if (MessageClientImpl::_init)
	{
		ix::uninitNetSystem();
		MessageClientImpl::_init = false;
	}
}

void MessageClientImpl::Connect(const std::string& url)
{
	if (this->isConnected()) return;
	this->_client.setUrl(url);
	this->_client.setOnMessageCallback(
		[this](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Open:
				if (this->_OpenCallback) this->_OpenCallback(msg->openInfo);
				break;
			case ix::WebSocketMessageType::Message:
				if (msg->binary)
				{
					if (this->_BinaryCallback)
					{
						this->_BinaryCallback(msg->str);
					}
				}
				else
				{
					if (this->_TextCallback)
					{
						this->_TextCallback(msg->str);
					}
				}
				break;
			case ix::WebSocketMessageType::Ping:
				if (this->_PingCallback) this->_PingCallback(msg->str);
				break;
			case ix::WebSocketMessageType::Pong:
				if (this->_PongCallback) this->_PongCallback(msg->str);
				break;
			case ix::WebSocketMessageType::Error:
				if (this->_ErrorCallback) this->_ErrorCallback(msg->errorInfo);
				break;
			case ix::WebSocketMessageType::Fragment:
				// do nothing
				break;
			case ix::WebSocketMessageType::Close:
				if (this->_CloseCallback) this->_CloseCallback(msg->closeInfo);
				break;
			}
		});
	this->_client.start();
}

void MessageClientImpl::Disconnect()
{
	this->_client.stop();
}

} // namespace Mirai::Details