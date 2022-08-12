#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "GroupRecallEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void GroupRecallEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_AuthorId = Utils::GetValue(data, "authorId", QQ_t{});
	this->_MessageId = Utils::GetValue(data, "messageId", (MessageId_t)0);
	this->_time = Utils::GetValue(data, "time", (std::time_t)0);
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

}