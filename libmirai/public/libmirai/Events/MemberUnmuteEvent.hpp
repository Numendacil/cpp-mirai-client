#ifndef _MIRAI_MEMBER_UNMUTE_EVENT_HPP_
#define _MIRAI_MEMBER_UNMUTE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员被取消禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberUnmuteEvent::_member` | `GroupMember{}`
 * `MemberUnmuteEvent::_operator` | `std::nullopt`
 *
 * 该成员不是Bot自己
 */
class MemberUnmuteEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberUnmuteEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberUnmuteEvent* Clone() const override
	// {
	//	return new MemberUnmuteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif