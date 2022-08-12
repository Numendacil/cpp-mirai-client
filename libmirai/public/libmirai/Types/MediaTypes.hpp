#ifndef _MIRAI_TYPES_MEDIA_TYPES_HPP_
#define _MIRAI_TYPES_MEDIA_TYPES_HPP_

#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <array>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/Serializable.hpp>
#include <libmirai/Types/BasicTypes.hpp>


namespace Mirai
{

struct FileInfo
{
	std::string sha1;
	std::string md5;
	std::time_t UploadTime = 0;
	std::time_t LastModifyTime = 0;
	QQ_t UploaderId;

	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, FileInfo&);

struct GroupFileInfo
{
	std::string id;
	std::string path;
	std::string name;
	std::unique_ptr<GroupFileInfo> parent;
	bool isFile = false;
	int64_t size = 0;
	Group contact;

	std::optional<FileInfo> file = std::nullopt;
	std::optional<std::string> DownloadUrl = std::nullopt;

	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupFileInfo&);

class FilePath
{
protected:
	std::string _dir;
	bool _isPath = false;

public:
	FilePath() : _isPath(false) {}
	FilePath(const std::string& dir, bool isPath = false)
	: _dir(dir), _isPath(isPath) {}
	FilePath(const GroupFileInfo& file)
	: _dir(file.id), _isPath(false) {}

	bool isPath() const { return this->_isPath; }
	std::string GetPath() const { return this->_isPath? this->_dir : ""; }
	std::string GetId() const { return this->_isPath? "" : this->_dir; }

	FilePath& SetPath(const std::string& path)
	{
		this->_isPath = true;
		this->_dir = path;
		return *this;
	}
	FilePath& SetId(const std::string& id)
	{
		this->_isPath = false;
		this->_dir = id;
		return *this;
	}
	
};

struct MiraiImage : public Serializable
{
	std::string id;
	std::string url;
	std::string path;
	std::string base64;
	int width = 0;
	int height = 0;
	int64_t size = 0;
	std::string ImageType;	// Better replace with enum
	bool isEmoji = false;

	MiraiImage(const std::string& id = {}, const std::string& url = {}, 
		const std::string& path = {}, const std::string& base64 = {})
	: id(id), url(url), path(path), base64(base64) {}

	bool isValid() const
	{
		return !(this->id.empty() && this->url.empty() && this->path.empty() && this->base64.empty());
	}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;
};

using FriendImage = MiraiImage;
using GroupImage = MiraiImage;
using TempImage = MiraiImage;

struct MiraiAudio : public Serializable
{
	std::string id;
	std::string url;
	std::string path;
	std::string base64;
	int64_t length = 0;

	MiraiAudio(const std::string& id = {}, const std::string& url = {}, 
		const std::string& path = {}, const std::string& base64 = {})
	: id(id), url(url), path(path), base64(base64) {}

	bool isValid() const
	{
		return !(this->id.empty() && this->url.empty() && this->path.empty() && this->base64.empty());
	}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;
};

// only group audio uploads are supported for now
using GroupAudio = MiraiAudio;

}

#endif