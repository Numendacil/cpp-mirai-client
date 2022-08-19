#ifndef _MIRAI_MEMBER_JOIN_EVENT_HPP_
#define _MIRAI_MEMBER_JOIN_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 新成员加入群聊事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberJoinEvent::_member` | `GroupMember{}`
 * `MemberJoinEvent::_inviter` | `std::nullopt`
 */
class MemberJoinEvent : public EventBase
{
protected:
	GroupMember _member;
	std::optional<GroupMember> _inviter = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberJoinEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberJoinEvent* Clone() const override
	// {
	//	return new MemberJoinEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取邀请人信息，若无则返回 `std::nullopt`
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }
};

} // namespace Mirai


#endif