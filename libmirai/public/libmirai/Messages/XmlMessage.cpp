#include "XmlMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool XmlMessage::isValid() const
{
	return !this->_content.empty();
}

void XmlMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_content = Utils::GetValue(data, "xml", "");
}

json XmlMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["xml"] = this->_content;
	return data;
}

} // namespace Mirai