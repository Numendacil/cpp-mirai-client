#include "FriendRecallEvent.hpp"

#include <cstdint>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void FriendRecallEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_AuthorId = Utils::GetValue(data, "authorId", QQ_t{});
	this->_MessageId = Utils::GetValue(data, "messageId", (MessageId_t)0);
	this->_time = Utils::GetValue(data, "time", (std::time_t)0);
	this->_operator = Utils::GetValue(data, "operator", QQ_t{});
}

} // namespace Mirai