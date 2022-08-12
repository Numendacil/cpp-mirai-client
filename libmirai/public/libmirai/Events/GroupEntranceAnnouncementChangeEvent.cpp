#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "GroupEntranceAnnouncementChangeEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void GroupEntranceAnnouncementChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_origin = Utils::GetValue(data, "origin", "");
	this->_current = Utils::GetValue(data, "current", "");
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

}