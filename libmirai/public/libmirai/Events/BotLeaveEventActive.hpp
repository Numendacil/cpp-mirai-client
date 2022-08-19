#ifndef _MIRAI_BOT_LEAVE_EVENT_ACTIVE_HPP_
#define _MIRAI_BOT_LEAVE_EVENT_ACTIVE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

// TODO: add a base class for all BotLeaveEvent

/**
 * @brief Bot主动退群事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotLeaveEventActive::_group` | `Group{}`
 */
class BotLeaveEventActive : public EventBase
{
protected:
	Group _group;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "BotLeaveEventActive";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual BotLeaveEventActive* Clone() const override
	// {
	//	return new BotLeaveEventActive(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群聊信息
	Group GetGroup() const { return this->_group; }
};

} // namespace Mirai


#endif