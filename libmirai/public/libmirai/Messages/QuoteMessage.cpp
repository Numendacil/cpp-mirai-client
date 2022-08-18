#include "QuoteMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool QuoteMessage::isValid() const
{
	return true;
}

void QuoteMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_QuoteId = Utils::GetValue(data, "id", (MessageId_t)-1);
	this->_GroupId = Utils::GetValue(data, "groupId", GID_t{});
	this->_SenderId = Utils::GetValue(data, "senderId", QQ_t{});
	this->_TargetId = Utils::GetValue(data, "targetId", (int64_t)0);
	this->_origin = Utils::GetValue(data, "origin", MessageChain{});
}

json QuoteMessage::ToJson() const
{
	// assert(this->isValid());

	// json data = json::object();
	// data["type"] = this->GetType();
	// data["id"] = this->id;
	// data["name"] = this->name;
	// data["size"] = this->size;
	// return data;

	// Not allowed for sending
	return json::object();
}

} // namespace Mirai
