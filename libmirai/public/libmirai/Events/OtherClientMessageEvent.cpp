#include "OtherClientMessageEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void OtherClientMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_sender = Utils::GetValue(data, "sender", ClientDevice{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

} // namespace Mirai