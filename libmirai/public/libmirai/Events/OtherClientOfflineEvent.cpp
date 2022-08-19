#include "OtherClientOfflineEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void OtherClientOfflineEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_client = Utils::GetValue(data, "client", ClientDevice{});
}

} // namespace Mirai