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

#ifndef MIRAI_SERIALIZATION_MIRAICODE_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_MIRAICODE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/MiraiCodeMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct MiraiCodeMessage::Serializable
{

	static void from_json(const nlohmann::json& j, MiraiCodeMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MiraiCodeMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("code").get_to(p.code_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, MiraiCodeMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MiraiCodeMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		::Mirai::from_json(std::move(j.at("code")), p.code_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const MiraiCodeMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MiraiCodeMessage::GetType();
		j["code"] = p.code_;
	}

	static void to_json(nlohmann::json& j, MiraiCodeMessage&& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MiraiCodeMessage::GetType();
		j["code"] = std::move(p.code_);
	}

};

} // namespace Mirai

#endif