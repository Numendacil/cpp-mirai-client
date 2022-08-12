#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "StrangerSyncMessageEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void StrangerSyncMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_subject = Utils::GetValue(data, "subject", User{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

}