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

#include "MediaTypes.hpp"

#include <memory>
#include <optional>

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Types/BasicTypes.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void from_json(const json& j, FileInfo& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	j.at("md5").get_to(p.md5);
	j.at("sha1").get_to(p.sha1);
	j.at("uploaderId").get_to(p.UploaderId);
	j.at("uploadTime").get_to(p.UploadTime);
	j.at("lastModifyTime").get_to(p.LastModifyTime);

	MIRAI_PARSE_GUARD_END;
}

void from_json(const json& j, GroupFileInfo& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

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

	MIRAI_PARSE_GUARD_END;
}

void from_json(const json& j, MiraiImage& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	p.id = Utils::GetValue(j, "imageId", "");
	p.url = Utils::GetValue(j, "url", "");
	p.path = Utils::GetValue(j, "path", "");
	p.base64 = Utils::GetValue(j, "base64", "");
	p.width = Utils::GetValue(j, "width", 0);
	p.height = Utils::GetValue(j, "height", 0);
	p.size = Utils::GetValue(j, "size", (int64_t)0);
	p.ImageType = Utils::GetValue(j, "imageType", "");
	p.isEmoji = Utils::GetValue(j, "isEmoji", false);

	MIRAI_PARSE_GUARD_END;
}

void to_json(json& j, const MiraiImage& p)
{
	// assert(p.isValid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["imageId"] = p.id;
	else if (!p.url.empty())
		j["url"] = p.url;
	else if (!p.path.empty())
		j["path"] = p.path;
	else if (!p.base64.empty())
		j["base64"] = p.base64;
}
void from_json(const json& j, MiraiAudio& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	p.id = Utils::GetValue(j, "voiceId", "");
	p.url = Utils::GetValue(j, "url", "");
	p.path = Utils::GetValue(j, "path", "");
	p.base64 = Utils::GetValue(j, "base64", "");
	p.length = Utils::GetValue(j, "length", (int64_t)0);

	MIRAI_PARSE_GUARD_END;
}

void to_json(json& j, const MiraiAudio& p)
{
	// assert(p.isValid());	// NOLINT(*-array-to-pointer-decay)

	if (!p.id.empty()) j["voiceId"] = p.id;
	else if (!p.url.empty())
		j["url"] = p.url;
	else if (!p.path.empty())
		j["path"] = p.path;
	else if (!p.base64.empty())
		j["base64"] = p.base64;
}

} // namespace Mirai