#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "BotUnmuteEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void BotUnmuteEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_operator = Utils::GetValue(data, "operator", GroupMember{});
}

}