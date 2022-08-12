#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "CommandExecutedEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void CommandExecutedEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_name = Utils::GetValue(data, "name", "");
	this->_friend = Utils::GetOptional<User>(data, "friend");
	this->_member = Utils::GetOptional<GroupMember>(data, "member");
	this->_args = Utils::GetValue(data, "args", MessageChain{});
}

}