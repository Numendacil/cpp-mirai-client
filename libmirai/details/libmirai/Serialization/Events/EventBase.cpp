#include "EventBase.hpp"

#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;
void EventBase::Serializable::from_json(const json& j, EventBase& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p.Deserialize(&j);

	MIRAI_PARSE_GUARD_END(j);
}

// Not defined
// void EventBase::Serializable::to_json(json& j, const EventBase& p)
// {
// 	p.Serialize(&j);
// }

} // namespace Mirai