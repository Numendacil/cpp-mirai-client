#include "BotLeaveEventActive.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void BotLeaveEventActive::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_group = Utils::GetValue(data, "group", Group{});
}

} // namespace Mirai