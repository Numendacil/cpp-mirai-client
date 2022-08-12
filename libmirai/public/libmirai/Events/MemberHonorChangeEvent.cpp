#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MemberHonorChangeEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void MemberHonorChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_action = Utils::GetValue(data, "action", "");
	this->_honor = Utils::GetValue(data, "honor", "");
	this->_member = Utils::GetValue(data, "member", GroupMember{});
}

}