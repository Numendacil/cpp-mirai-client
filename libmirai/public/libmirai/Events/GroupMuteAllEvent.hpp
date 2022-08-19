#ifndef _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_
#define _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群聊开启/关闭全体禁言事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `false`
 * `BotGroupPermissionChangeEvent::_current` | `false`
 * `BotGroupPermissionChangeEvent::_operator` | `std::nullopt`
 */
class GroupMuteAllEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupMuteAllEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupMuteAllEvent* Clone() const override
	// {
	//	return new GroupMuteAllEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本是否处于全员禁言状态
	bool GetOriginal() const { return this->_origin; }
	/// 现在是否处于全员禁言
	bool GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif