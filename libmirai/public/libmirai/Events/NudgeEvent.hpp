#ifndef _MIRAI_NUDGE_EVENT_HPP_
#define _MIRAI_NUDGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/NudgeTarget.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class NudgeEvent : public EventBase
{
protected:
	QQ_t _FromId;
	NudgeTarget _target;
	std::string _action;
	std::string _suffix;

public:
	static constexpr std::string_view _TYPE_ = "NudgeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual NudgeEvent* Clone() const override
	{
		return new NudgeEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	QQ_t GetSender() const { return this->_FromId; }
	NudgeTarget GetTarget() const { return this->_target; }
	std::string GetAction() const { return this->_action; }
	std::string GetSuffix() const { return this->_suffix; }
};

}


#endif