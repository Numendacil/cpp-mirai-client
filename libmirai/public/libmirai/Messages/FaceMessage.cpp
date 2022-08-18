#include "FaceMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>


namespace Mirai
{

using json = nlohmann::json;

bool FaceMessage::isValid() const
{
	return this->_id != -1 || !this->_name.empty();
}

void FaceMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_id = Utils::GetValue(data, "faceId", (int64_t)-1);
	this->_name = Utils::GetValue(data, "name", "");
}

json FaceMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	if (this->_id != -1) data["faceId"] = this->_id;
	else
		data["name"] = this->_name;
	return data;
}

} // namespace Mirai