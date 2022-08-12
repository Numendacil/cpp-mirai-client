#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MarketFaceMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool MarketFaceMessage::isValid() const
{
	return true;
}

void MarketFaceMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_id = Utils::GetValue(data, "id", (int64_t)-1);
	this->_name = Utils::GetValue(data, "name", "");
}

json MarketFaceMessage::ToJson() const
{
	// assert(this->isValid());

	// json data = json::object();
	// data["type"] = this->GetType();
	// data["id"] = this->id;
	// data["name"] = this->name;
	// return data;

	// Sending MarketFace Message is currently not supported
	return json::object();
}

}