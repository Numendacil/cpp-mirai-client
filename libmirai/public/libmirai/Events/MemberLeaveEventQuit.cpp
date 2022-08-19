#include "MemberLeaveEventQuit.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void MemberLeaveEventQuit::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_member = Utils::GetValue(data, "member", GroupMember{});
}

} // namespace Mirai