#include "MemberHonorChangeEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void MemberHonorChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_action = _to_enum(Utils::GetValue(data, "action", ""));
	this->_honor = Utils::GetValue(data, "honor", "");
	this->_member = Utils::GetValue(data, "member", GroupMember{});
}

} // namespace Mirai