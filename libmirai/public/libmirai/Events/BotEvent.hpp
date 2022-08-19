#ifndef _MIRAI_BOT_EVENT_HPP_
#define _MIRAI_BOT_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 与Bot本身相关的事件的基类
 * 
 * 不可直接使用，仅用作其它事件的基类
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `BotEvent::_qq` | `0_qq`
 */
class BotEvent : public EventBase
{

protected:
	QQ_t _qq;

public:
	using EventBase::EventBase;
	// static constexpr std::string_view _TYPE_ = "BotEvent";

	// virtual std::string_view GetType() const override
	// {
	// 	return _TYPE_;
	// }

	// virtual BotEvent* Clone() const override
	// {
	// 	return new BotEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取BotQQ
	QQ_t GetQQ() const { return this->_qq; }
};

} // namespace Mirai


#endif