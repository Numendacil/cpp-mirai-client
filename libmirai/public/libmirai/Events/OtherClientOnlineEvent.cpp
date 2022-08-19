#include "OtherClientOnlineEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void OtherClientOnlineEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_client = Utils::GetValue(data, "client", ClientDevice{});
	this->_kind = Utils::GetOptional<int64_t>(data, "kind");
}

} // namespace Mirai