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

bool MessageClientImpl::init_ = false; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

MessageClientImpl::MessageClientImpl(std::chrono::seconds HeartbeatInterval, std::chrono::seconds HandshakeTimeout,
                                     bool EnablePong, bool EnableDeflate, bool AutoReconnect,
                                     std::chrono::milliseconds MinRetryInterval,
                                     std::chrono::milliseconds MaxRetryInterval)
{
	if (!MessageClientImpl::init_)
	{
		ix::initNetSystem();
		MessageClientImpl::init_ = true;
	}
	this->client_.setPingInterval(static_cast<int>(HeartbeatInterval.count()));
	this->client_.setHandshakeTimeout(static_cast<int>(HandshakeTimeout.count()));
	if (!EnablePong) this->client_.disablePong();
	if (EnableDeflate) this->client_.enablePerMessageDeflate();
	if (AutoReconnect)
	{
		this->client_.enableAutomaticReconnection();
		this->client_.setMinWaitBetweenReconnectionRetries(MinRetryInterval.count());
		this->client_.setMaxWaitBetweenReconnectionRetries(MaxRetryInterval.count());
	}
	else
		this->client_.disableAutomaticReconnection();
}

MessageClientImpl::~MessageClientImpl()
{
	if (this->isConnected()) this->client_.stop();
	if (MessageClientImpl::init_)
	{
		ix::uninitNetSystem();
		MessageClientImpl::init_ = false;
	}
}

void MessageClientImpl::Connect(const std::string& url)
{
	if (this->isConnected()) return;
	this->client_.setUrl(url);
	this->client_.setOnMessageCallback(
		[this](const ix::WebSocketMessagePtr& msg)
		{
			switch (msg->type)
			{
			case ix::WebSocketMessageType::Open:
				if (this->OpenCallback_) this->OpenCallback_(msg->openInfo);
				break;
			case ix::WebSocketMessageType::Message:
				if (msg->binary)
				{
					if (this->BinaryCallback_)
					{
						this->BinaryCallback_(msg->str);
					}
				}
				else
				{
					if (this->TextCallback_)
					{
						this->TextCallback_(msg->str);
					}
				}
				break;
			case ix::WebSocketMessageType::Ping:
				if (this->PingCallback_) this->PingCallback_(msg->str);
				break;
			case ix::WebSocketMessageType::Pong:
				if (this->PongCallback_) this->PongCallback_(msg->str);
				break;
			case ix::WebSocketMessageType::Error:
				if (this->ErrorCallback_) this->ErrorCallback_(msg->errorInfo);
				break;
			case ix::WebSocketMessageType::Fragment:
				// do nothing
				break;
			case ix::WebSocketMessageType::Close:
				if (this->CloseCallback_) this->CloseCallback_(msg->closeInfo);
				break;
			}
		});
	this->client_.start();
}

void MessageClientImpl::Disconnect()
{
	this->client_.stop();
}

} // namespace Mirai::Details