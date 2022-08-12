#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "BotEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void BotEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_qq = Utils::GetValue(data, "qq", QQ_t{});
}

}