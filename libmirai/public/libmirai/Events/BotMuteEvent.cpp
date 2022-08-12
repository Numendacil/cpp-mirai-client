#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "BotMuteEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void BotMuteEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_operator = Utils::GetValue(data, "operator", GroupMember{});
	this->_duration = Utils::GetValue(data, "durationSeconds", (std::time_t)0);
}

}