#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MemberCardChangeEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void MemberCardChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_origin = Utils::GetValue(data, "origin", "");
	this->_current = Utils::GetValue(data, "current", "");
	this->_member = Utils::GetValue(data, "member", GroupMember{});
}

}