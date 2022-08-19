#ifndef _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_
#define _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief Bot群聊权限改变事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotGroupPermissionChangeEvent::_group` | `Group{}`
 * `BotGroupPermissionChangeEvent::_origin` | `PERMISSION::UNKNOWN`
 * `BotGroupPermissionChangeEvent::_current` | `PERMISSION::UNKNOWN`
 */
class BotGroupPermissionChangeEvent : public EventBase
{
protected:
	Group _group;
	PERMISSION _origin = PERMISSION::UNKNOWN;
	PERMISSION _current = PERMISSION::UNKNOWN;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotGroupPermissionChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotGroupPermissionChangeEvent* Clone() const override
	// {
	//	return new BotGroupPermissionChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 获取Bot更改前的权限
	PERMISSION GetOriginal() const { return this->_origin; }
	/// 获取Bot当前权限
	PERMISSION GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif