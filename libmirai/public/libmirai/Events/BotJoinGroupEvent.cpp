#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "BotJoinGroupEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void BotJoinGroupEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_inviter = Utils::GetOptional<GroupMember>(data, "invitor");
}

}