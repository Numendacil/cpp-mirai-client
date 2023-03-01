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

#ifndef _MIRAI_SERIALIZATION_MUSICSHARE_MESSAGE_HPP_
#define _MIRAI_SERIALIZATION_MUSICSHARE_MESSAGE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/MusicShareMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>

namespace Mirai
{

struct MusicShareMessage::Serializable
{

	static void from_json(const nlohmann::json& j, MusicShareMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MusicShareMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("kind").get_to(p._kind);
		j.at("title").get_to(p._title);
		j.at("summary").get_to(p._summary);
		j.at("jumpUrl").get_to(p._JumpUrl);
		j.at("pictureUrl").get_to(p._PictureUrl);
		j.at("musicUrl").get_to(p._MusicUrl);
		j.at("brief").get_to(p._brief);

		MIRAI_PARSE_GUARD_END(j)(j);
	}

	static void to_json(nlohmann::json& j, const MusicShareMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MusicShareMessage::GetType();
		j["kind"] = p._kind;
		j["title"] = p._title;
		j["summary"] = p._summary;
		j["jumpUrl"] = p._JumpUrl;
		j["pictureUrl"] = p._PictureUrl;
		j["musicUrl"] = p._MusicUrl;
		j["brief"] = p._brief;
	}

};

} // namespace Mirai

#endif