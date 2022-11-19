#include "MessageBase.hpp"

#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;
void MessageBase::Serializable::from_json(const json& j, MessageBase& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	p.Deserialize(&j);

	MIRAI_PARSE_GUARD_END;
}

void MessageBase::Serializable::to_json(json& j, const MessageBase& p)
{
	p.Serialize(&j);
}

} // namespace Mirai