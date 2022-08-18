#ifndef _MIRAI_TYPES_SERIALIZABLE_HPP_
#define _MIRAI_TYPES_SERIALIZABLE_HPP_

#include <nlohmann/json_fwd.hpp>

namespace Mirai
{

/**
 * @brief Base class for all json serializable type
 * 
 */

class Serializable
{
public:
	virtual void FromJson(const nlohmann::json&) = 0;
	virtual nlohmann::json ToJson() const = 0;

	virtual ~Serializable() = default;
};

/// Custom serialization to nlohmann::json type for all `Serializable` derived class
void to_json(nlohmann::json& j, const Serializable& p);

/// Custom serialization from nlohmann::json type for all `Serializable` derived class
void from_json(const nlohmann::json& j, Serializable& p);

} // namespace Mirai

#endif