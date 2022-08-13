#ifndef _MIRAI_BOT_JOIN_GROUP_EVENT_HPP_
#define _MIRAI_BOT_JOIN_GROUP_EVENT_HPP_

#include <string>
#include <optional>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class BotJoinGroupEvent : public EventBase
{
protected:
	Group _group;
	std::optional<GroupMember> _inviter = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "BotJoinGroupEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual BotJoinGroupEvent* Clone() const override
	// {
	//	return new BotJoinGroupEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetInviter() const { return this->_inviter; }
};

}


#endif