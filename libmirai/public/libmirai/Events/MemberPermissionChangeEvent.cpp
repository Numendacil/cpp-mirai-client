#include "MemberPermissionChangeEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void MemberPermissionChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_origin = Utils::GetValue(data, "origin", PERMISSION::UNKNOWN);
	this->_current = Utils::GetValue(data, "current", PERMISSION::UNKNOWN);
	this->_member = Utils::GetValue(data, "member", GroupMember{});
}

} // namespace Mirai