#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "GroupAllowConfessTalkEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void GroupAllowConfessTalkEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_origin = Utils::GetValue(data, "origin", false);
	this->_current = Utils::GetValue(data, "current", false);
	this->_ByBot = Utils::GetValue(data, "isByBot", false);
	this->_group = Utils::GetValue(data, "group", Group{});
}

}