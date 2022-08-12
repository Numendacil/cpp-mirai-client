#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MemberJoinEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void MemberJoinEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_member = Utils::GetValue(data, "member", GroupMember{});
	this->_inviter = Utils::GetOptional<GroupMember>(data, "invitor");
}

}