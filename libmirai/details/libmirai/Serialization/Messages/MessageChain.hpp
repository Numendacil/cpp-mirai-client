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

#ifndef MIRAI_SERIALIZATION_MESSAGECHAIN_HPP_
#define MIRAI_SERIALIZATION_MESSAGECHAIN_HPP_

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Serializable.hpp>

namespace Mirai
{

struct MessageElement::Serializable
{
	static void from_json(const nlohmann::json& j, MessageElement& p);
	static void from_json(nlohmann::json&& j, MessageElement& p);
	static void to_json(nlohmann::json& j, const MessageElement& p);
	static void to_json(nlohmann::json& j, MessageElement&& p);
};

struct MessageChain::Serializable
{
	static void from_json(const nlohmann::json& j, MessageChain& p);
	static void from_json(nlohmann::json&& j, MessageChain& p);
	static void to_json(nlohmann::json& j, const MessageChain& p);
	static void to_json(nlohmann::json& j, MessageChain&& p);
};

} // namespace Mirai

#endif