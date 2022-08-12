#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "TempMessageEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void TempMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_sender = Utils::GetValue(data, "sender", GroupMember{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

}