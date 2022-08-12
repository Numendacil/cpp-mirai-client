#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "BotGroupPermissionChangeEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void BotGroupPermissionChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_origin = Utils::GetValue(data, "origin", PERMISSION::UNKNOWN);
	this->_current = Utils::GetValue(data, "current", PERMISSION::UNKNOWN);
}

}