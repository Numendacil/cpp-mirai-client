#ifndef _MIRAI_FRIEND_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_FRIEND_SYNC_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendSyncMessageEvent::_subject` | `User{}`
 * `FriendSyncMessageEvent::_message` | `MessageChain{}`
 */
class FriendSyncMessageEvent : public EventBase
{

protected:
	User _subject;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendSyncMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual FriendSyncMessageEvent* Clone() const override
	// {
	//	return new FriendSyncMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取目标好友信息
	User GetFriend() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }
};

} // namespace Mirai


#endif