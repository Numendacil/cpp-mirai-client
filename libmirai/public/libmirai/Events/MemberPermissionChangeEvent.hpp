#ifndef _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_PERMISSION_CHANGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员权限更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberPermissionChangeEvent::_member` | `GroupMember{}`
 * `MemberPermissionChangeEvent::_origin` | `PERMISSION::UNKNOWN`
 * `MemberPermissionChangeEvent::_current` | `PERMISSION::UNKNOWN`
 *
 * 该成员不是Bot自己
 */
class MemberPermissionChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	PERMISSION _origin = PERMISSION::UNKNOWN;
	PERMISSION _current = PERMISSION::UNKNOWN;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberPermissionChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberPermissionChangeEvent* Clone() const override
	// {
	//	return new MemberPermissionChangeEvent(*this);
	// }/ }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取当前权限
	PERMISSION GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif