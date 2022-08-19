#ifndef _MIRAI_GROUP_NAME_CHANGE_EVENT_HPP_
#define _MIRAI_GROUP_NAME_CHANGE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊名称更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupNameChangeEvent::_group` | `Group{}`
 * `GroupNameChangeEvent::_origin` | `""`
 * `GroupNameChangeEvent::_current` | `""`
 * `GroupNameChangeEvent::_operator` | `std::nullopt`
 */
class GroupNameChangeEvent : public EventBase
{
protected:
	Group _group;
	std::string _origin;
	std::string _current;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupNameChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupNameChangeEvent* Clone() const override
	// {
	//	return new GroupNameChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取更改前的群聊名称
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群聊名称
	std::string GetCurrent() const { return this->_current; }
	/// 获取操作员信息，为Bot时返回 `std::nullopt`
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

} // namespace Mirai


#endif