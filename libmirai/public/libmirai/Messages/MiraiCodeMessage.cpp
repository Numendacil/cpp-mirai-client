#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MiraiCodeMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool MiraiCodeMessage::isValid() const
{
	return !this->_code.empty();
}

void MiraiCodeMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_code = Utils::GetValue(data, "code", "");
}

json MiraiCodeMessage::ToJson() const
{
	// assert(this->isValid());
	
	json data = json::object();
	data["type"] = this->GetType();
	data["code"] = this->_code;

	return data;
}

}
