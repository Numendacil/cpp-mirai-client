#include "PokeMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool PokeMessage::isValid() const
{
	return this->_kind != PokeKind::UNKNOWN;
}

void PokeMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_kind = _to_enum(Utils::GetValue(data, "name", ""));
}

json PokeMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["name"] = _to_string(this->_kind);
	return data;
}

} // namespace Mirai