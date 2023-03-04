// Copyright (C) 2022 Numendacil and contributors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
	if (!j.is_object() || !j.contains(key) || j.at(key).is_null())
	{
		return std::nullopt;
	}
	return j.at(key).template get<ValueType>();
}

template<typename ValueType, typename KeyType>
void GetOptional(const nlohmann::json& j, KeyType&& key, std::optional<ValueType>& p)
{
	if (!j.is_object() || !j.contains(key) || j.at(key).is_null())
	{
		p = std::nullopt;
	}
	else
		p = j.at(key).template get<ValueType>();
}

template<typename KeyType> bool HasValue(const nlohmann::json& j, KeyType&& key)
{
	return !(!j.is_object() || !j.contains(key) || j.at(key).is_null());
}

} // namespace Mirai::Utils

#endif