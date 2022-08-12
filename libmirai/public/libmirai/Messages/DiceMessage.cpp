#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "DiceMessage.hpp"


namespace Mirai
{
	
using json = nlohmann::json;

bool DiceMessage::isValid() const
{
	return this->_value > 0 && this->_value <= 6;
}

void DiceMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_value = Utils::GetValue(data, "value", -1);
}

json DiceMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["value"] = this->_value;
	return data;
}

}