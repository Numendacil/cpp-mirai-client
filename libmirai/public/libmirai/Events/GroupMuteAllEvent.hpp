#ifndef _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_
#define _MIRAI_GROUP_MUTE_ALL_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupMuteAllEvent : public EventBase
{
protected:
	bool _origin = false;
	bool _current = false;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "GroupMuteAllEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual GroupMuteAllEvent* Clone() const override
	// {
	//	return new GroupMuteAllEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	bool GetOriginal() const { return this->_origin; }
	bool GetCurrent() const { return this->_current; }
	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif