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

#ifndef _MIRAI_SERIALIZATION_TYPES_SERIALIZABLE_HPP_
#define _MIRAI_SERIALIZATION_TYPES_SERIALIZABLE_HPP_

#include <exception>
#include <type_traits>
#include <utility>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Exceptions/Exceptions.hpp>

namespace Mirai
{

/**
 * @brief Helper methods for serializable types
 * 
 */

namespace traits
{

template <typename T>
class _has_from_json
{
	template<typename U>
	static auto test(U*)
	-> typename std::is_same<
		decltype(U::Serializable::from_json(std::declval<const nlohmann::json&>(), std::declval<U&>())),
		void
	>::type;

	template<typename>
	static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<T>(0))::value;
};

template <typename T>
class _has_to_json
{
	template<typename U>
	static auto test(U*)
	-> typename std::is_same<
		decltype(U::Serializable::to_json(std::declval<nlohmann::json&>(), std::declval<const U&>())),
		void
	>::type;

	template<typename>
	static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<T>(0))::value;
};

}

template <typename T>
auto from_json(const nlohmann::json& j, T& p)
-> std::enable_if_t<traits::_has_from_json<T>::value>
{
	T::Serializable::from_json(j, p);
}

template <typename T>
auto to_json(nlohmann::json& j, const T& p)
-> std::enable_if_t<traits::_has_to_json<T>::value>
{
	T::Serializable::to_json(j, p);
}

#define MIRAI_DECLARE_SERIALIZABLE_JSON(_type_)	\
struct _type_::Serializable	\
{	\
	static void from_json(const nlohmann::json&, _type_&);	\
	static void to_json(nlohmann::json&, const _type_&);	\
}

#define MIRAI_DECLARE_FROM_TO_JSON(_type_)	\
void from_json(const nlohmann::json&, _type_&);	\
void to_json(nlohmann::json&, const _type_&)

#define MIRAI_PARSE_GUARD_BEGIN \
try	\
{

#define MIRAI_PARSE_GUARD_END	\
}	\
catch(const std::exception& e)	\
{	\
	throw ParseError(e.what(), j.dump());	\
}

} // namespace Mirai

#endif