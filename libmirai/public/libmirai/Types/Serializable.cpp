#include <nlohmann/json.hpp>

#include "Serializable.hpp"

namespace Mirai
{

using json = nlohmann::json;

void to_json(json &j, const Serializable &p)
{
	j = p.ToJson();
}

void from_json(const json &j, Serializable &p)
{
	p.FromJson(j);
}

}