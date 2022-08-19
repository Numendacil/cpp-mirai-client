#ifndef _MIRAI_GROUP_ALLOW_ANONYMOUS_CHAT_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_ANONYMOUS_CHAT_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊开启/关闭匿名聊天事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowAnonymousChatEvent::_group` | `Group{}`
 * `GroupAllowAnonymousChatEvent::_origin` | `false`
 * `GroupAllowAnonymousChatEvent::_current` | `false`
 * `GroupAllowAnonymousChatEvent::_operator` | `std::nullopt`
 */
class GroupAllowAnonymousChatEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupAllowAnonymousChatEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupAllowAnonymousChatEvent* Clone() const override
	// {
	//	return new GroupAllowAnonymousChatEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本匿名聊天是否开启
	bool GetOriginal() const { return this->_origin; }
	/// 现在匿名聊天是否开启
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif