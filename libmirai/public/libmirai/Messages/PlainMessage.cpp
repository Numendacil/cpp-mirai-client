#include <cassert>
#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "PlainMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool PlainMessage::isValid() const
{
	return !this->_text.empty();
}

void PlainMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_text = Utils::GetValue(data, "text", "");
}

json PlainMessage::ToJson() const
{
	// assert(this->isValid());
	
	json data = json::object();
	data["type"] = this->GetType();
	data["text"] = this->_text;
	return data;
}

}