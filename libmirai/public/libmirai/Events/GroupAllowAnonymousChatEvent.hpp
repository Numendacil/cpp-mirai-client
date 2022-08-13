#ifndef _MIRAI_GROUP_ALLOW_ANONYMOUS_CHAT_EVENT_HPP_
#define _MIRAI_GROUP_ALLOW_ANONYMOUS_CHAT_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupAllowAnonymousChatEvent : public EventBase
{
protected:
	bool _origin = false;
	bool _current = false;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "GroupAllowAnonymousChatEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual GroupAllowAnonymousChatEvent* Clone() const override
	// {
	//	return new GroupAllowAnonymousChatEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	bool GetOriginal() const { return this->_origin; }
	bool GetCurrent() const { return this->_current; }
	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif