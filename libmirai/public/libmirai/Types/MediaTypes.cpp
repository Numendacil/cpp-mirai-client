#include "MediaTypes.hpp"

#include <memory>
#include <optional>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void FileInfo::FromJson(const json& data)
{
	this->md5 = Utils::GetValue(data, "md5", "");
	this->sha1 = Utils::GetValue(data, "sha1", "");
	this->UploaderId = Utils::GetValue(data, "uploaderId", QQ_t{});
	this->UploadTime = Utils::GetValue(data, "uploadTime", (std::time_t)0);
	this->LastModifyTime = Utils::GetValue(data, "lastModifyTime", (std::time_t)0);
}

void from_json(const json& j, FileInfo& p)
{
	p.FromJson(j);
}

void GroupFileInfo::FromJson(const json& data)
{
	this->name = Utils::GetValue(data, "name", "");
	this->id = Utils::GetValue(data, "id", "");
	this->path = Utils::GetValue(data, "path", "");
	this->contact = Utils::GetValue(data, "contact", Group{});
	if (Utils::HasValue(data, "parent"))
	{
		this->parent = std::make_unique<GroupFileInfo>();
		this->parent->FromJson(data.at("parent"));
	}
	else
		this->parent = nullptr;
	this->isFile = Utils::GetValue(data, "isFile", false);
	if (this->isFile)
	{
		this->file = FileInfo{};
		this->file->FromJson(data);
	}
	else
		this->file = std::nullopt;

	if (Utils::HasValue(data, "downloadInfo"))
		this->DownloadUrl = Utils::GetOptional<std::string>(data.at("downloadInfo"), "url");
	else
		this->DownloadUrl = std::nullopt;
}

void from_json(const json& j, GroupFileInfo& p)
{
	p.FromJson(j);
}


void MiraiImage::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "imageId", "");
	this->url = Utils::GetValue(data, "url", "");
	this->path = Utils::GetValue(data, "path", "");
	this->base64 = Utils::GetValue(data, "base64", "");
	this->width = Utils::GetValue(data, "width", 0);
	this->height = Utils::GetValue(data, "height", 0);
	this->size = Utils::GetValue(data, "size", (int64_t)0);
	this->ImageType = Utils::GetValue(data, "imageType", "");
	this->isEmoji = Utils::GetValue(data, "isEmoji", false);
}

json MiraiImage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	if (!this->id.empty()) data["imageId"] = this->id;
	else if (!this->url.empty())
		data["url"] = this->url;
	else if (!this->path.empty())
		data["path"] = this->path;
	else if (!this->base64.empty())
		data["base64"] = this->base64;

	return data;
}
void MiraiAudio::FromJson(const json& data)
{
	this->id = Utils::GetValue(data, "voiceId", "");
	this->url = Utils::GetValue(data, "url", "");
	this->path = Utils::GetValue(data, "path", "");
	this->base64 = Utils::GetValue(data, "base64", "");
	this->length = Utils::GetValue(data, "length", (int64_t)0);
}

json MiraiAudio::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	if (!this->id.empty()) data["voiceId"] = this->id;
	else if (!this->url.empty())
		data["url"] = this->url;
	else if (!this->path.empty())
		data["path"] = this->path;
	else if (!this->base64.empty())
		data["base64"] = this->base64;

	return data;
}

} // namespace Mirai