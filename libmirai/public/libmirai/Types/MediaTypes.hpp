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

/**
 * @brief 文件基础信息
 * 
 */
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

/**
 * @brief 群文件信息
 * 
 * 可能为文件夹，此时 `GroupFileInfo::isFile = false` 且
 * `GroupFileInfo::file` 为空。'GroupFileInfo::DownloadUrl'
 * 只有当请求了下载信息时才会被设置。
 */
struct GroupFileInfo
{
	/// 文件id，唯一标识符
	std::string id;
	/// 文件路径
	std::string path;
	/// 文件名称
	std::string name;
	/// 文件父目录
	std::unique_ptr<GroupFileInfo> parent;
	bool isFile = false;
	int64_t size = 0;
	/// 文件所在的群聊
	Group contact;

	/// 文件信息
	std::optional<FileInfo> file = std::nullopt;
	/// 文件下载链接
	std::optional<std::string> DownloadUrl = std::nullopt;

	void FromJson(const nlohmann::json&);
};

void from_json(const nlohmann::json&, GroupFileInfo&);

/**
 * @brief 文件路径
 * 
 * 可由文件id或者文件位置 `/path/to/file` 指定，由于文件名
 * 可以不唯一，推荐尽量使用id。
 */
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