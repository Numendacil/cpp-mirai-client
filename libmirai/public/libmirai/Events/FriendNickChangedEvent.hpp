#ifndef _MIRAI_FRIEND_NICK_CHANGED_EVENT_HPP_
#define _MIRAI_FRIEND_NICK_CHANGED_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友昵称改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendNickChangedEvent::_friend` | `User{}`
 * `FriendNickChangedEvent::_from` | `""`
 * `FriendNickChangedEvent::_to` | `""`
 */
class FriendNickChangedEvent : public EventBase
{
protected:
	User _friend;
	std::string _from;
	std::string _to;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendNickChangedEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual FriendNickChangedEvent* Clone() const override
	// {
	//	return new FriendNickChangedEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取好友信息
	User GetFriend() const { return this->_friend; }
	/// 获取更改前的昵称
	std::string GetOriginalNickname() const { return this->_from; }
	/// 获取当前昵称
	std::string GetNewNickname() const { return this->_to; }
};

} // namespace Mirai


#endif