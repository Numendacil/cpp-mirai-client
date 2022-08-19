#ifndef _MIRAI_GROUP_RECALL_EVENT_HPP_
#define _MIRAI_GROUP_RECALL_EVENT_HPP_

#include <ctime>
#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群消息撤回事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupRecallEvent::_AuthorId` | `0_qq`
 * `GroupRecallEvent::_MessageId` | `0`
 * `GroupRecallEvent::_time` | `0`
 * `GroupRecallEvent::_group` | `Group{}`
 * `GroupRecallEvent::_operator` | `std::nullopt`
 */
class GroupRecallEvent : public EventBase
{
protected:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupRecallEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupRecallEvent* Clone() const override
	// {
	//	return new GroupRecallEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取被撤回消息的发送者QQ
	QQ_t GetSender() const { return this->_AuthorId; }
	/// 获取被撤回消息的消息id
	MessageId_t GetMessageId() const { return this->_MessageId; }
	/// 获取被撤回消息的发送时间
	std::time_t GetSendTime() const { return this->_time; }
	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif