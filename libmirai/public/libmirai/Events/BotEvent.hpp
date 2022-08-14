#ifndef _MIRAI_BOT_EVENT_HPP_
#define _MIRAI_BOT_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

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

	QQ_t GetQQ() const { return this->_qq; }
};

}


#endif