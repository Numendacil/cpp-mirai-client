#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "StrangerMessageEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void StrangerMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_sender = Utils::GetValue(data, "sender", User{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

}