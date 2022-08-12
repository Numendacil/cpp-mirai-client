#include <cstdint>
#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "NewFriendRequestEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void NewFriendRequestEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_EventId = Utils::GetValue(data, "eventId", (int64_t)0);
	this->_FromId = Utils::GetValue(data, "fromId", QQ_t{});
	this->_GroupId = Utils::GetValue(data, "groupId", GID_t{});
	this->_nickname = Utils::GetValue(data, "nick", "");
	this->_message = Utils::GetValue(data, "message", "");
}

}