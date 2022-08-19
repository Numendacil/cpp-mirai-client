#include "GroupSyncMessageEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void GroupSyncMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_subject = Utils::GetValue(data, "subject", Group{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

} // namespace Mirai