#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "TempSyncMessageEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void TempSyncMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_subject = Utils::GetValue(data, "subject", GroupMember{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

}