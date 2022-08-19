#include "GroupAllowMemberInviteEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void GroupAllowMemberInviteEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_origin = Utils::GetValue(data, "origin", false);
	this->_current = Utils::GetValue(data, "current", false);
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

} // namespace Mirai