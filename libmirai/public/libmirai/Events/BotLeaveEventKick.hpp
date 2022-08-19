#ifndef _MIRAI_BOT_LEAVE_EVENT_KICK_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_KICK_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all BotLeaveEvent

/**
 * @brief Bot被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventKick::_group` | `Group{}`
 * `BotLeaveEventKick::_operator` | `GroupMember{}`
 */
class BotLeaveEventKick : public EventBase
{
protected:
	Group _group;
	GroupMember _operator;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventKick";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotLeaveEventKick* Clone() const override
	// {
	//	return new BotLeaveEventKick(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员信息，若无则返回 `std::nullopt`
	GroupMember GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif