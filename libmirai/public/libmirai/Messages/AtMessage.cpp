#include "AtMessage.hpp"

#include <cassert>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool AtMessage::isValid() const
{
	return this->_target != QQ_t();
}

void AtMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_target = Utils::GetValue(data, "target", QQ_t{});
	this->_display = Utils::GetValue(data, "display", "");
}

json AtMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["target"] = _target;

	// Not used for sending
	// data["display"] = this->display;

	return data;
}

} // namespace Mirai