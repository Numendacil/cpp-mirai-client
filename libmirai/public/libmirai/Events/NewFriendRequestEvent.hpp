#ifndef _MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_
#define _MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 用户申请添加Bot好友事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NewFriendRequestEvent::_EventId` | `0`
 * `NewFriendRequestEvent::_FromId` | `0_qq`
 * `NewFriendRequestEvent::_GroupId` | `0_gid`
 * `NewFriendRequestEvent::_nickname` | `""`
 * `NewFriendRequestEvent::_message` | `""`
 */
class NewFriendRequestEvent : public EventBase
{
protected:
	int64_t _EventId = 0;
	QQ_t _FromId;
	GID_t _GroupId;
	std::string _nickname;
	std::string _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "NewFriendRequestEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual NewFriendRequestEvent* Clone() const override
	// {
	//	return new NewFriendRequestEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取事件id，唯一标识符
	int64_t GetEventId() const { return this->_EventId; }
	/// 获取申请人QQ
	QQ_t GetUserId() const { return this->_FromId; }
	/// 获取申请人昵称
	std::string GetNickname() const { return this->_nickname; }
	/// 获取申请人来自的群聊，若无则返回 `0_gid`
	GID_t GetGroupId() const { return this->_GroupId; }
	/// 申请人是否通过群聊找到Bot
	bool isFromGroup() const { return this->_GroupId != (GID_t)0; }
	/// 获取申请信息
	std::string GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick response
};

} // namespace Mirai


#endif