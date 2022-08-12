#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "AppMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool AppMessage::isValid() const
{
	return !this->_content.empty();
}

void AppMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_content = Utils::GetValue(data, "content", "");
}

json AppMessage::ToJson() const
{
	// assert(this->isValid());
	
	json data = json::object();
	data["type"] = this->GetType();
	data["content"] = this->_content;
	return data;
}

}