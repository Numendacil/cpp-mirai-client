#ifndef _MIRAI_GROUP_ENTRANCE_ANNOUNCEMENT_CHANGE_EVENT_HPP_
#define _MIRAI_GROUP_ENTRANCE_ANNOUNCEMENT_CHANGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupEntranceAnnouncementChangeEvent : public EventBase
{
protected:
	std::string _origin;
	std::string _current;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupEntranceAnnouncementChangeEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual GroupEntranceAnnouncementChangeEvent* Clone() const override
	// {
	//	return new GroupEntranceAnnouncementChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	std::string GetOriginal() const { return this->_origin; }
	std::string GetCurrent() const { return this->_current; }
	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif