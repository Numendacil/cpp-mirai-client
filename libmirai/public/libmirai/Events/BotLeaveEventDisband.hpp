#ifndef _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_DISBAND_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all BotLeaveEvent

/**
 * @brief 群聊解散导致Bot退群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventDisband::_group` | `Group{}`
 * `BotLeaveEventDisband::_operator` | `GroupMember{}`
 */
class BotLeaveEventDisband : public EventBase
{
protected:
	Group _group;
	GroupMember _operator;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventDisband";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotLeaveEventDisband* Clone() const override
	// {
	//	return new BotLeaveEventDisband(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取操作员（群主）信息
	GroupMember GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif