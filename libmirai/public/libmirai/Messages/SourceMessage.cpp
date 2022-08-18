#include "SourceMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool SourceMessage::isValid() const
{
	return true;
}

void SourceMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_id = Utils::GetValue(data, "id", -1);
	this->_timestamp = Utils::GetValue(data, "time", (std::time_t)0);
}

json SourceMessage::ToJson() const
{
	// assert(this->isValid());

	// json data = json::object();
	// data["type"] = this->GetType();
	// data["id"] = this->id;
	// data["time"] = this->timestamp;
	// return data;
	return json::object();
}

} // namespace Mirai