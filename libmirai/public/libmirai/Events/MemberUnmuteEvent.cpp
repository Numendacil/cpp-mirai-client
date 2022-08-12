#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MemberUnmuteEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void MemberUnmuteEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_member = Utils::GetValue(data, "member", GroupMember{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

}