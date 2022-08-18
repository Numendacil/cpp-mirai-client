#include "FileMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool FileMessage::isValid() const
{
	return true;
}

void FileMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_id = Utils::GetValue(data, "id", "");
	this->_name = Utils::GetValue(data, "name", "");
	this->_size = Utils::GetValue(data, "size", 0);
}

json FileMessage::ToJson() const
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
