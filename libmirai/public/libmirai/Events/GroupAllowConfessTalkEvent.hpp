#ifndef _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all GroupSettingChangeEvent

/**
 * @brief 群聊开启/关闭坦白说事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupAllowConfessTalkEvent::_group` | `Group{}`
 * `GroupAllowConfessTalkEvent::_origin` | `false`
 * `GroupAllowConfessTalkEvent::_current` | `false`
 * `GroupAllowConfessTalkEvent::_ByBot` | `false`
 */
class GroupAllowConfessTalkEvent : public EventBase
{
protected:
	Group _group;
	bool _origin = false;
	bool _current = false;
	bool _ByBot = false; // 无法获得操作员

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupAllowConfessTalkEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupAllowConfessTalkEvent* Clone() const override
	// {
	//	return new GroupAllowConfessTalkEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
	/// 原本坦白说是否开启
	bool GetOriginal() const { return this->_origin; }
	/// 现在坦白说是否开启
	bool GetCurrent() const { return this->_current; }
	/// 操作员是否为Bot自己（无法获得具体操作员信息）
	bool isByBot() const { return this->_ByBot; }
};

} // namespace Mirai


#endif