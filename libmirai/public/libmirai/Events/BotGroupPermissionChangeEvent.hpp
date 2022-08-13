#ifndef _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_
#define _MIRAI_BOT_GROUP_PERMISSION_CHANGED_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotGroupPermissionChangeEvent : public EventBase
{
protected:
	Group _group;
	PERMISSION _origin = PERMISSION::UNKNOWN;
	PERMISSION _current = PERMISSION::UNKNOWN;

public:
	static constexpr std::string_view _TYPE_ = "BotGroupPermissionChangeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotGroupPermissionChangeEvent* Clone() const override
	// {
	//	return new BotGroupPermissionChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	Group GetGroup() const { return this->_group; }
	PERMISSION GetOriginal() const { return this->_origin; }
	PERMISSION GetCurrent() const { return this->_current; }
};

}


#endif