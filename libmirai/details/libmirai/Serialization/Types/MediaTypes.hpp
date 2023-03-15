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

#ifndef MIRAI_SERIALIZATION_TYPES_MEDIA_TYPES_HPP_
#define MIRAI_SERIALIZATION_TYPES_MEDIA_TYPES_HPP_

#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Types/BasicTypes.hpp>
#include <libmirai/Serialization/Types/Serializable.hpp>
#include <libmirai/Types/MediaTypes.hpp>
#include <libmirai/Utils/Common.hpp>


namespace Mirai
{

inline void from_json(const nlohmann::json& j, FileInfo& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	j.at("md5").get_to(p.md5);
	j.at("sha1").get_to(p.sha1);
	j.at("uploaderId").get_to(p.UploaderId);
	j.at("uploadTime").get_to(p.UploadTime);
	j.at("lastModifyTime").get_to(p.LastModifyTime);

	MIRAI_PARSE_GUARD_END(j);
}


inline void from_json(const nlohmann::json& j, GroupFileInfo& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	j.at("name").get_to(p.name);
	j.at("id").get_to(p.id);
	j.at("path").get_to(p.path);
	j.at("contact").get_to(p.contact);
	if (Utils::HasValue(j, "parent"))
	{
		p.parent = std::make_unique<GroupFileInfo>();
		j.at("parent").get_to(*p.parent);
	}
	else
		p.parent = nullptr;
	j.at("isFile").get_to(p.isFile);
	if (p.isFile)
	{
		p.file = FileInfo{};
		j.get_to(*p.file);
	}
	else
		p.file = std::nullopt;

	if (Utils::HasValue(j, "downloadInfo"))
		p.DownloadUrl = Utils::GetOptional<std::string>(j.at("downloadInfo"), "url");
	else
		p.DownloadUrl = std::nullopt;

	MIRAI_PARSE_GUARD_END(j);
}

inline void from_json(nlohmann::json&& j, GroupFileInfo& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	from_json(std::move(j.at("name")), p.name);
	from_json(std::move(j.at("id")), p.id);
	from_json(std::move(j.at("path")), p.path);
	from_json(std::move(j.at("contact")), p.contact);

	if (Utils::HasValue(j, "parent"))
	{
		p.parent = std::make_unique<GroupFileInfo>();
		from_json(std::move(j.at("parent")), *p.parent);
	}
	else
		p.parent = nullptr;
	j.at("isFile").get_to(p.isFile);
	if (p.isFile)
	{
		p.file = FileInfo{};
		j.get_to(*p.file);
	}
	else
		p.file = std::nullopt;

	if (Utils::HasValue(j, "downloadInfo"))
		p.DownloadUrl = Utils::GetOptional<std::string>(std::move(j.at("downloadInfo")), "url");
	else
		p.DownloadUrl = std::nullopt;

	MIRAI_PARSE_GUARD_END(j);
}


inline void from_json(const nlohmann::json& j, MiraiImage& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p.id = Utils::GetValue(j, "imageId", "");
	p.url = Utils::GetValue(j, "url", "");
	p.path = Utils::GetValue(j, "path", "");
	p.base64 = Utils::GetValue(j, "base64", "");
	p.width = Utils::GetValue(j, "width", 0);
	p.height = Utils::GetValue(j, "height", 0);
	p.size = Utils::GetValue(j, "size", (int64_t)0);
	p.ImageType = Utils::GetValue(j, "imageType", "");
	p.isEmoji = Utils::GetValue(j, "isEmoji", false);

	MIRAI_PARSE_GUARD_END(j);
}

inline void from_json(nlohmann::json&& j, MiraiImage& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p.id = Utils::GetValue(std::move(j), "imageId", "");
	p.url = Utils::GetValue(std::move(j), "url", "");
	p.path = Utils::GetValue(std::move(j), "path", "");
	p.base64 = Utils::GetValue(std::move(j), "base64", "");
	p.width = Utils::GetValue(std::move(j), "width", 0);
	p.height = Utils::GetValue(std::move(j), "height", 0);
	p.size = Utils::GetValue(std::move(j), "size", (int64_t)0);
	p.ImageType = Utils::GetValue(std::move(j), "imageType", "");
	p.isEmoji = Utils::GetValue(std::move(j), "isEmoji", false);

	MIRAI_PARSE_GUARD_END(j);
}

inline void to_json(nlohmann::json& j, const MiraiImage& p)
{
	// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["imageId"] = p.id;
	else if (!p.url.empty())
		j["url"] = p.url;
	else if (!p.path.empty())
		j["path"] = p.path;
	else if (!p.base64.empty())
		j["base64"] = p.base64;
}

inline void to_json(nlohmann::json& j, MiraiImage&& p)
{
	// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["imageId"] = std::move(p.id);
	else if (!p.url.empty())
		j["url"] = std::move(p.url);
	else if (!p.path.empty())
		j["path"] = std::move(p.path);
	else if (!p.base64.empty())
		j["base64"] = std::move(p.base64);
}


inline void from_json(const nlohmann::json& j, MiraiAudio& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p.id = Utils::GetValue(j, "voiceId", "");
	p.url = Utils::GetValue(j, "url", "");
	p.path = Utils::GetValue(j, "path", "");
	p.base64 = Utils::GetValue(j, "base64", "");
	p.length = Utils::GetValue(j, "length", (int64_t)0);

	MIRAI_PARSE_GUARD_END(j);
}

inline void from_json(nlohmann::json&& j, MiraiAudio& p)
{
	MIRAI_PARSE_GUARD_BEGIN(j);

	p.id = Utils::GetValue(std::move(j), "voiceId", "");
	p.url = Utils::GetValue(std::move(j), "url", "");
	p.path = Utils::GetValue(std::move(j), "path", "");
	p.base64 = Utils::GetValue(std::move(j), "base64", "");
	p.length = Utils::GetValue(std::move(j), "length", (int64_t)0);

	MIRAI_PARSE_GUARD_END(j);
}

inline void to_json(nlohmann::json& j, const MiraiAudio& p)
{
	// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["voiceId"] = p.id;
	else if (!p.url.empty())
		j["url"] = p.url;
	else if (!p.path.empty())
		j["path"] = p.path;
	else if (!p.base64.empty())
		j["base64"] = p.base64;
}

inline void to_json(nlohmann::json& j, MiraiAudio&& p)
{
	// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["voiceId"] = std::move(p.id);
	else if (!p.url.empty())
		j["url"] = std::move(p.url);
	else if (!p.path.empty())
		j["path"] = std::move(p.path);
	else if (!p.base64.empty())
		j["base64"] = std::move(p.base64);
}

} // namespace Mirai

#endif