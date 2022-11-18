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

#include "MessageChain.hpp"

#include <algorithm>
#include <memory>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Messages/MessageBase.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

#include <libmirai/Messages/Messages.hpp>
#include <libmirai/Types/MessageTypes.hpp>

namespace Mirai
{

using json = nlohmann::json;

namespace
{

template <auto Start, auto End, auto Inc, class F>
constexpr void constexpr_for(F&& f)
{
	if constexpr (Start < End)
	{
		f(std::integral_constant<decltype(Start), Start>());
		constexpr_for<Start + Inc, End, Inc>(f);
	}
}

constexpr std::array<std::unique_ptr<MessageBase>(*)(), MessageTypesList.size()> Factory =
[]() {
	std::array<std::unique_ptr<MessageBase>(*)(), MessageTypesList.size()> arr{};
	constexpr_for<0, arr.size(), 1>([&arr](auto i){
		using type = GetType_t<MessageTypesList[i]>;
		arr.at(i) = []() -> std::unique_ptr<MessageBase> { return std::make_unique<type>(); };
	});
	return arr;
}();



} // namespace

void MessageChain::Serializable::to_json(json& j, const MessageChain& p)
{
	j = json::array();
	for (const auto& msg : p._message)
	{
		// if (!msg->isValid()) 
		// 	continue;
		if (!msg->isSendSupported())
			continue;
		j += *msg;
	}
}

void MessageChain::Serializable::from_json(const json& j, MessageChain& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	if (!j.is_array()) return;
	p._message.clear();
	p._message.reserve(j.size());
	for (const auto& msg : j)
	{
		if (!msg.is_object() || !msg.contains("type") || !msg.at("type").is_string()) 
			continue;
		MessageTypes type = msg.at("type").get<MessageTypes>();
		msg.get_to(
			*(p._message.emplace_back(
				Factory.at(static_cast<size_t>(type))()
			))
		);
	}
	p._message.shrink_to_fit();

	MIRAI_PARSE_GUARD_END;
}

} // namespace Mirai