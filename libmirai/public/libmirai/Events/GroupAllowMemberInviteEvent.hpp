#ifndef _MIRAI_GROUP_ALLOW_MEMBER_INVITE_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_MEMBER_INVITE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊开启/关闭允许群成员邀请好友加群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowMemberInviteEvent::_group` | `Group{}`
 * `GroupAllowMemberInviteEvent::_origin` | `false`
 * `GroupAllowMemberInviteEvent::_current` | `false`
 * `GroupAllowMemberInviteEvent::_operator` | `std::nullopt`
 */
class GroupAllowMemberInviteEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupAllowMemberInviteEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupAllowMemberInviteEvent* Clone() const override
	// {
	//	return new GroupAllowMemberInviteEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本是否允许群成员邀请好友加群
	bool GetOriginal() const { return this->_origin; }
	/// 现在是否允许群成员邀请好友加群
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif