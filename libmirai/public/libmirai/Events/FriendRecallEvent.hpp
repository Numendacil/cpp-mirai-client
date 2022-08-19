#ifndef _MIRAI_FRIEND_RECALL_EVENT_HPP_
#define _MIRAI_FRIEND_RECALL_EVENT_HPP_

#include <ctime>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 好友消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `FriendRecallEvent::_AuthorId` | `0_qq`
 * `FriendRecallEvent::_MessageId` | `0`
 * `FriendRecallEvent::_time` | `0`
 * `FriendRecallEvent::_operator` | `0_qq`
 */
class FriendRecallEvent : public EventBase
{
protected:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	QQ_t _operator;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "FriendRecallEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual FriendRecallEvent* Clone() const override
	// {
	//	return new FriendRecallEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->_AuthorId; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->_MessageId; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->_time; }
	/// 获取操作员QQ
	QQ_t GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif