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

#ifndef MIRAI_SERIALIZATION_TYPES_SERIALIZABLE_HPP_
#define MIRAI_SERIALIZATION_TYPES_SERIALIZABLE_HPP_

#include <exception>
#include <type_traits>
#include <utility>

#include <nlohmann/json.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>

namespace Mirai
{

/**
 * @brief Helper methods for serializable types
 * 
 */

namespace traits
{

template<typename... Args> 
class has_from_json_;

template<typename Field, typename Ret, typename... Args> 
class has_from_json_<Field, Ret(Args...)>
{
	template<typename F, typename R, typename... A>
	static auto test(void*) -> std::enable_if_t<
		std::is_same_v< decltype(F::from_json(std::declval<A>()...)), Ret >,
		std::true_type 
	>;

	template<typename, typename> static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<Field, Ret, Args...>(0))::value;
};

template<typename Ret, typename... Args> 
class has_from_json_<Ret(Args...)>
{
	template<typename R, typename... A>
	static auto test(void*) -> std::enable_if_t<
		std::is_same_v< decltype(from_json(std::declval<A>()...)), Ret >,
		std::true_type 
	>;

	template<typename, typename> static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<Ret, Args...>(0))::value;
};


template<typename... Args> 
class has_to_json_;

template<typename Field, typename Ret, typename... Args> 
class has_to_json_<Field, Ret(Args...)>
{
	template<typename F, typename R, typename... A>
	static auto test(void*) -> std::enable_if_t<
		std::is_same_v< decltype(F::to_json(std::declval<A>()...)), Ret >,
		std::true_type 
	>;

	template<typename, typename> static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<Field, Ret, Args...>(0))::value;
};

template<typename Ret, typename... Args> 
class has_to_json_<Ret(Args...)>
{
	template<typename R, typename... A>
	static auto test(void*) -> std::enable_if_t<
		std::is_same_v< decltype(to_json(std::declval<A>()...)), Ret >,
		std::true_type 
	>;

	template<typename, typename> static std::false_type test(...);

public:
	static constexpr bool value = decltype(test<Ret, Args...>(0))::value;
};

} // namespace traits


template<typename T>
auto from_json(nlohmann::json&& j, T& p)
	-> std::enable_if_t<
		traits::has_from_json_<typename std::decay_t<T>::Serializable, void(nlohmann::json&&, T&)>::value,
		void
	>
{
	std::decay_t<T>::Serializable::from_json(std::move(j), p);
}

inline void from_json(nlohmann::json&& j, std::string& p)
{
	p = std::move(j.get_ref<std::string&>());
}

template<typename T>
auto from_json(const nlohmann::json& j, T& p)
	-> std::enable_if_t<
		traits::has_from_json_<typename std::decay_t<T>::Serializable, void(const nlohmann::json&, T&)>::value,
		void
	>
{
	std::decay_t<T>::Serializable::from_json(j, p);
}

template<typename T>
auto to_json(nlohmann::json& j, T&& p)
	-> std::enable_if_t<
		!std::is_reference_v<T>
		&& traits::has_to_json_<typename std::decay_t<T>::Serializable, void(nlohmann::json&, T&&)>::value,
		void
	>
{
	std::decay_t<T>::Serializable::to_json(j, std::forward<T>(p));
}

template<typename T>
auto to_json(nlohmann::json& j, const T& p)
	-> std::enable_if_t<
		traits::has_to_json_<typename std::decay_t<T>::Serializable, void(nlohmann::json&, const T&)>::value,
		void
	>
{
	std::decay_t<T>::Serializable::to_json(j, p);
}

#define MIRAI_DECLARE_FROM_JSON(type)                                                                               \
	void from_json(const nlohmann::json&, type&)

#define MIRAI_DECLARE_TO_JSON(type)                                                                               \
	void to_json(nlohmann::json&, const type&)

#define MIRAI_DECLARE_FROM_TO_JSON(type)                                                                               \
	MIRAI_DECLARE_FROM_JSON(type);                                                                      \
	MIRAI_DECLARE_TO_JSON(type)

#define MIRAI_DEFINE_FROM_JSON(type, namespace_)                                                                       \
	void from_json(const nlohmann::json& j, type& p)                                                                   \
	{                                                                                                                  \
		namespace_::from_json(j, p);                                                                                   \
	}
#define MIRAI_DEFINE_TO_JSON(type, namespace_)                                                                         \
	void to_json(nlohmann::json& j, const type& p)                                                                     \
	{                                                                                                                  \
		namespace_::to_json(j, p);                                                                                     \
	}
#define MIRAI_DEFINE_FROM_TO_JSON(type, namespace_)                                                                    \
	MIRAI_DEFINE_FROM_JSON(type, namespace_);                                                                          \
	MIRAI_DEFINE_TO_JSON(type, namespace_)

#define MIRAI_PARSE_GUARD_BEGIN(json_var)                                                                              \
	try                                                                                                                \
	{                                                                                                                  \
		(void)(json_var)

#define MIRAI_PARSE_GUARD_END(json_var)                                                                                \
	}                                                                                                                  \
	catch (const nlohmann::json::exception& e)                                                                                    \
	{                                                                                                                  \
		throw ParseError(e.what(), json_var.dump());                                                                   \
	}

} // namespace Mirai

#endif