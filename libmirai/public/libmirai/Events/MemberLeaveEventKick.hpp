#ifndef _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_
#define _MIRAI_MEMBER_LEAVE_EVENT_KICK_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all MemberLeaveEvent

/**
 * @brief 成员被踢出群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberLeaveEventKick::_member` | `GroupMember{}`
 * `MemberLeaveEventKick::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberLeaveEventKick : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberLeaveEventKick";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberLeaveEventKick* Clone() const override
	// {
	//	return new MemberLeaveEventKick(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif