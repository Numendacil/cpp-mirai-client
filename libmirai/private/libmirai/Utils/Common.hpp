#ifndef _MIRAI_UTILS_COMMON_HPP_
#define _MIRAI_UTILS_COMMON_HPP_

#include <optional>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>


namespace httplib
{

class Result;

}

namespace Mirai::Utils
{

nlohmann::json ParseResponse(const nlohmann::json& result);
nlohmann::json ParseResponse(const std::string& result);
nlohmann::json ParseResponse(const httplib::Result& result);

template<typename ValueType, typename KeyType>
auto GetValue(const nlohmann::json& j, KeyType&& key, ValueType&& default_value)
{
	using ReturnType = decltype(std::declval<typename nlohmann::json>().value(std::forward<KeyType>(key),
	                                                                          std::forward<ValueType>(default_value)));
	if (!j.is_object() || !j.contains(key) || j.at(key).is_null())
		return (ReturnType)std::forward<ValueType>(default_value);
	return j.at(key).template get<ReturnType>();
}

template<typename ValueType, typename KeyType>
std::optional<ValueType> GetOptional(const nlohmann::json& j, KeyType&& key)
{
	if (!j.is_object() || !j.contains(key) || j.at(key).is_null()) return std::nullopt;
	return j.at(key).template get<ValueType>();
}

template<typename KeyType> bool HasValue(const nlohmann::json& j, KeyType&& key)
{
	if (!j.is_object() || !j.contains(key) || j.at(key).is_null()) return false;
	return true;
}

} // namespace Mirai::Utils

#endif