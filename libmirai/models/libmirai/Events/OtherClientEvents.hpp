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

#ifndef _MIRAI_OTHER_CLIENT_EVENTS_HPP_
#define _MIRAI_OTHER_CLIENT_EVENTS_HPP_

#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 其它客户端消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientMessageEvent::_sender` | `ClientDevice{}`
 * `OtherClientMessageEvent::_message` | `MessageChain{}`
 */
class OtherClientMessageEvent final : public IEvent<OtherClientMessageEvent>
{
	friend IEvent<OtherClientMessageEvent>;
private:
	ClientDevice _sender;
	MessageChain _message;

	static constexpr EventTypes _TYPE_ = EventTypes::OtherClientMessage;

public:
	/// 获取发送客户端信息
	ClientDevice GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	struct Serializable;
};



/**
 * @brief 其它客户端下线事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientOfflineEvent::_client` | `ClientDevice{}`
 */
class OtherClientOfflineEvent final : public IEvent<OtherClientOfflineEvent>
{
	friend IEvent<OtherClientOfflineEvent>;

private:
	ClientDevice _client;

	static constexpr EventTypes _TYPE_ = EventTypes::OtherClientOffline;

public:
	/// 获取设备信息
	ClientDevice GetClient() const { return this->_client; }

	struct Serializable;
};



/**
 * @brief 其它客户端上线事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientOnlineEvent::_client` | `ClientDevice{}`
 * `OtherClientOnlineEvent::_kind` | `std::nullopt`
 */
class OtherClientOnlineEvent final : public IEvent<OtherClientOnlineEvent>
{
	friend IEvent<OtherClientOnlineEvent>;

private:
	ClientDevice _client;
	std::optional<int64_t> _kind = std::nullopt;

	static constexpr EventTypes _TYPE_ = EventTypes::OtherClientOnline;

public:
	/// 获取设备信息
	ClientDevice GetClient() const { return this->_client; }
	/// 获取详细设备类型，无则返回 `std::nullopt`
	std::optional<int64_t> GetKind() const { return this->_kind; }

	// TODO: replace kind with enum, see https://github.com/mamoe/mirai/blob/dev/mirai-core-api/src/commonMain/kotlin/contact/OtherClient.kt#L113

	struct Serializable;
};



#define DECLARE_TYPE_ENUM(_type_)                                                                                      \
	template<> struct GetEventType<_type_::GetType()>                                                                  \
	{                                                                                                                  \
		using type = _type_;                                                                                           \
	}

DECLARE_TYPE_ENUM(OtherClientMessageEvent);
DECLARE_TYPE_ENUM(OtherClientOfflineEvent);
DECLARE_TYPE_ENUM(OtherClientOnlineEvent);

#undef DECLARE_TYPE_ENUM

} // namespace Mirai


#endif