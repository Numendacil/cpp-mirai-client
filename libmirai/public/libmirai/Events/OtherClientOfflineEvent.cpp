#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "OtherClientOfflineEvent.hpp"

namespace Mirai
{

using json = nlohmann::json;

void OtherClientOfflineEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_client = Utils::GetValue(data, "client", ClientDevice{});
}

}