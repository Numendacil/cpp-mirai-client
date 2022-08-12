#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "PokeMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool PokeMessage::isValid() const
{
	return !this->_name.empty();
}

void PokeMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_name = Utils::GetValue(data, "name", "");
}

json PokeMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["name"] = this->_name;
	return data;
}

}