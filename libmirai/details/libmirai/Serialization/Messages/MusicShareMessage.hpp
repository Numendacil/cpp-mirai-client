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

#ifndef MIRAI_SERIALIZATION_MUSICSHARE_MESSAGE_HPP_
#define MIRAI_SERIALIZATION_MUSICSHARE_MESSAGE_HPP_

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

		j.at("kind").get_to(p.kind_);
		j.at("title").get_to(p.title_);
		j.at("summary").get_to(p.summary_);
		j.at("jumpUrl").get_to(p.JumpUrl_);
		j.at("pictureUrl").get_to(p.PictureUrl_);
		j.at("musicUrl").get_to(p.MusicUrl_);
		j.at("brief").get_to(p.brief_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, MusicShareMessage& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		assert(j.at("type").get<MessageTypes>() == MusicShareMessage::GetType()); // NOLINT(*-array-to-pointer-decay)

		j.at("kind").get_to(p.kind_);
		::Mirai::from_json(std::move(j.at("title")), p.title_);
		::Mirai::from_json(std::move(j.at("summary")), p.summary_);
		::Mirai::from_json(std::move(j.at("jumpUrl")), p.JumpUrl_);
		::Mirai::from_json(std::move(j.at("pictureUrl")), p.PictureUrl_);
		::Mirai::from_json(std::move(j.at("musicUrl")), p.MusicUrl_);
		::Mirai::from_json(std::move(j.at("brief")), p.brief_);

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const MusicShareMessage& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MusicShareMessage::GetType();
		j["kind"] = p.kind_;
		j["title"] = p.title_;
		j["summary"] = p.summary_;
		j["jumpUrl"] = p.JumpUrl_;
		j["pictureUrl"] = p.PictureUrl_;
		j["musicUrl"] = p.MusicUrl_;
		j["brief"] = p.brief_;
	}

	static void to_json(nlohmann::json& j, MusicShareMessage&& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		j["type"] = MusicShareMessage::GetType();
		j["kind"] = p.kind_;
		j["title"] = std::move(p.title_);
		j["summary"] = std::move(p.summary_);
		j["jumpUrl"] = std::move(p.JumpUrl_);
		j["pictureUrl"] = std::move(p.PictureUrl_);
		j["musicUrl"] = std::move(p.MusicUrl_);
		j["brief"] = std::move(p.brief_);
	}

};

} // namespace Mirai

#endif