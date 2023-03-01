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

#include <libmirai/Messages/Messages.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Types/MessageTypes.hpp>

#include "AppMessage.hpp"
#include "AtAllMessage.hpp"
#include "AtMessage.hpp"
#include "AudioMessage.hpp"
#include "DiceMessage.hpp"
#include "FaceMessage.hpp"
#include "FileMessage.hpp"
#include "ForwardMessage.hpp"
#include "ForwardMessageNode.hpp"
#include "ImageMessage.hpp"
#include "JsonMessage.hpp"
#include "MessageChain.hpp"
#include "MiraiCodeMessage.hpp"
#include "MusicShareMessage.hpp"
#include "PlainMessage.hpp"
#include "PokeMessage.hpp"
#include "QuoteMessage.hpp"
#include "SourceMessage.hpp"
#include "XmlMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

namespace
{

template<auto Start, auto End, auto Inc, class F> constexpr void constexpr_for(F&& f)
{
	if constexpr (Start < End)
	{
		f(std::integral_constant<decltype(Start), Start>());
		constexpr_for<Start + Inc, End, Inc>(f);
	}
}

constexpr std::array<MessageElement(*)(), MessageTypesList.size()> Factory = []()
{
	std::array<MessageElement(*)(), MessageTypesList.size()> arr{};
	constexpr_for<0, arr.size(), 1>(
		[&arr](auto i)
		{
			using Type = GetType_t<MessageTypesList[i]>;
			arr.at(i) = []() -> MessageElement { Type m; return MessageElement(m); };
		});
	return arr;
}();


} // namespace

void MessageElement::Serializable::from_json(const json& j, MessageElement& p)
{
	p.visit([&j](auto&& m)
	{
		Mirai::from_json(j, m);
	});
}

void MessageElement::Serializable::to_json(json& j, const MessageElement& p)
{
	p.visit([&j](auto&& m)
	{
		using Type = std::decay_t<decltype(m)>;
		if constexpr (Type::isSendSupported())
			Mirai::to_json(j, m);
	});
}


void MessageChain::Serializable::from_json(const json& j, MessageChain& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	if (!j.is_array()) return;
	p._message.clear();
	p._message.reserve(j.size());
	for (const auto& msg : j)
	{
		if (!msg.is_object() || !msg.contains("type") || !msg.at("type").is_string()) continue;
		MessageTypes type = msg.at("type").get<MessageTypes>();
		auto& m = p._message.emplace_back(Factory.at(static_cast<size_t>(type))());
		msg.get_to(m);
	}
	p._message.shrink_to_fit();

	MIRAI_PARSE_GUARD_END(j);
}

void MessageChain::Serializable::to_json(json& j, const MessageChain& p)
{
	j = json::array();
	for (const auto& msg : p._message)
	{
		if (msg.allowSend())
			j += p;
	}
}

} // namespace Mirai