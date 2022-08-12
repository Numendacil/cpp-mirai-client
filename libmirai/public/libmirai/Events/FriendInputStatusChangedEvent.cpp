#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "FriendInputStatusChangedEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void FriendInputStatusChangedEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_friend = Utils::GetValue(data, "friend", User{});
	this->_inputting = Utils::GetValue(data, "inputting", false);
}

}