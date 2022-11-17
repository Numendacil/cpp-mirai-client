#include "MessageBase.hpp"


#include <nlohmann/json.hpp>

namespace Mirai
{

using json = nlohmann::json;
void MessageBase::Serializable::from_json(const json& j, MessageBase& p)
{
	p.Deserialize(&j);
}

void MessageBase::Serializable::to_json(json& j, const MessageBase& p)
{
	p.Serialize(&j);
}

}