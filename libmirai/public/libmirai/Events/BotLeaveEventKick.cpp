#include "BotLeaveEventKick.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void BotLeaveEventKick::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_operator = Utils::GetValue(data, "operator", GroupMember{});
}

} // namespace Mirai