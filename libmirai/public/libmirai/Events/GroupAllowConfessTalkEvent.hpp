#ifndef _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_CONFESS_TALK_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupAllowConfessTalkEvent : public EventBase
{
protected:
	bool _origin = false;
	bool _current = false;
	bool _ByBot = false;
	Group _group;

public:
	static constexpr std::string_view _TYPE_ = "GroupAllowConfessTalkEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual GroupAllowConfessTalkEvent* Clone() const override
	{
		return new GroupAllowConfessTalkEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	bool GetOriginal() const { return this->_origin; }
	bool GetCurrent() const { return this->_current; }
	bool isByBot() const { return this->_ByBot; }
	Group GetGroup() const { return this->_group; }
};

}


#endif