#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "JsonMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool JsonMessage::isValid() const
{
	return !this->_content.empty();
}

void JsonMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_content = Utils::GetValue(data, "json", "");
}

json JsonMessage::ToJson() const
{
	// assert(this->isValid());
	
	json data = json::object();
	data["type"] = this->GetType();
	data["json"] = this->_content;
	return data;
}

json JsonMessage::GetJson() const
{
	return json::parse(this->_content);
}

}