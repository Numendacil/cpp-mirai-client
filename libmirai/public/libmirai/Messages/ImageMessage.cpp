#include "ImageMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool ImageMessage::isValid() const
{
	return this->_image.isValid();
}

void ImageMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_image = data.get<MiraiImage>();
}

json ImageMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data.update(this->_image);

	return data;
}

} // namespace Mirai