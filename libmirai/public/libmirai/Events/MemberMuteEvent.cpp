#include "MemberMuteEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void MemberMuteEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_duration = Utils::GetValue(data, "durationSeconds", (std::time_t)0);
	this->_member = Utils::GetValue(data, "member", GroupMember{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

} // namespace Mirai