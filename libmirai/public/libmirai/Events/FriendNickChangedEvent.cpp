#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "FriendNickChangedEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void FriendNickChangedEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_friend = Utils::GetValue(data, "friend", User{});
	this->_from = Utils::GetValue(data, "from", "");
	this->_to = Utils::GetValue(data, "to", "");
}

}