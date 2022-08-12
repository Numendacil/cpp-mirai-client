#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "FriendMessageEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void FriendMessageEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_sender = Utils::GetValue(data, "sender", User{});
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
}

}