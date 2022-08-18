#include "AtAllMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool AtAllMessage::isValid() const
{
	return true;
}

void AtAllMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
}

json AtAllMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	return data;
}

} // namespace Mirai