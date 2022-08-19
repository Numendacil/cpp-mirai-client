#ifndef _MIRAI_TYPES_MEDIA_TYPES_HPP_
#define _MIRAI_TYPES_MEDIA_TYPES_HPP_

#include <ctime>
#include <memory>
#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/Serializable.hpp>


namespace Mirai
{

/**
 * @brief 文件基础信息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FileInfo::sha1`		| `""`
 * `FileInfo::md5`		| `""`
 * `FileInfo::UploadTime` | `0`
 * `FileInfo::LastModifyTime` | `0`
 * `FileInfo::UploaderId` | `0_qq`
 */
struct FileInfo
{
	/// 文件SHA1
	std::string sha1;
	/// 文件MD5
	std::string md5;
	/// 文件上传时间
	std::time_t UploadTime = 0;
	/// 文件修改时间
	std::time_t LastModifyTime = 0;
	/// 上传者QQ
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
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupFileInfo::id`		| `""`
 * `GroupFileInfo::path`		| `""`
 * `GroupFileInfo::name` 		| `""`
 * `GroupFileInfo::parent` | `nullptr`
 * `GroupFileInfo::isFile` | `false`
 * `GroupFileInfo::size`		 | `size`
 * `GroupFileInfo::contact` | `Group{}`
 * `GroupFileInfo::file` | `std::nullopt`
 * `GroupFIleInfo::DownloadUrl` | `std::nullopt`
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
 * 类似于Union的结构，可由文件id或者文件位置 `/path/to/file` 指定。
 * 由于文件名可以不唯一，推荐尽量使用id。
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FilePath::_dir` | `""`
 * `FilePath::_isPath` | `false`
 */
class FilePath
{
protected:
	std::string _dir;
	bool _isPath = false;

public:
	/**
	 * @brief 构造函数
	 * 
	 */
	FilePath() : _isPath(false) {}

	/**
	 * @brief 构造函数
	 * 
	 * @param dir 文件路径，可能为id或文件名路径
	 * @param isPath 是否为文件名路径
	 */
	FilePath(const std::string& dir, bool isPath = false) : _dir(dir), _isPath(isPath) {}

	/**
	 * @brief 构造函数
	 * @param file `GroupFileInfo` 对象
	 */
	FilePath(const GroupFileInfo& file) : _dir(file.id), _isPath(false) {}

	/**
	 * @brief 判定是否为文件名路径
	 * 
	 * @return `bool` 
	 */
	bool isPath() const { return this->_isPath; }

	/**
	 * @brief 获取文件名路径
	 * 
	 * 仅当 `isPath()` 为 `true` 时有效，否则返回空字符串
	 * @return 文件名路径
	 */
	std::string GetPath() const { return this->_isPath ? this->_dir : ""; }

	/**
	 * @brief 获取文件id
	 * 
	 * 仅当 `isPath()` 为 `false` 时有效，否则返回空字符串
	 * @return 文件id
	 */
	std::string GetId() const { return this->_isPath ? "" : this->_dir; }

	/**
	 * @brief 设置文件名路径
	 * 
	 * @param path 文件名路径
	 * @return Reference to *this
	 */
	FilePath& SetPath(const std::string& path)
	{
		this->_isPath = true;
		this->_dir = path;
		return *this;
	}

	/**
	 * @brief 设置文件id
	 * 
	 * @param id 文件名id
	 * @return Reference to *this
	 */
	FilePath& SetId(const std::string& id)
	{
		this->_isPath = false;
		this->_dir = id;
		return *this;
	}
};

/**
 * @brief Mirai图片
 * 
 * 发送时可以指定图片id，图片链接，图片路径或图片的base64编码，
 * 优先级为 id > url > path > base64，所以设置某一项属性前最好确保前面的域为空。
 * Member Variable | Default Value
 * --------------- | -------------
 * `MiraiImage::id` 	| `""`
 * `MiraiImage::url` 	| `""`
 * `MiraiImage::path` 	| `""`
 * `MiraiImage::base64` 	| `""`
 * `MiraiImage::width` 	| `0`
 * `MiraiImage::height` 	| `0`
 * `MiraiImage::size` 	| `0`
 * `MiraiImage::ImageType` 	| `""`
 * `MiraiImage::isEmoji` 	| `false`
 */
struct MiraiImage : public Serializable
{
	/// 图片id，从mirai获得
	std::string id;
	/// 图片链接
	std::string url;
	/// 图片路径
	std::string path;
	/// 图片base64编码
	std::string base64;
	/// 图片宽度
	int width = 0;
	/// 图片长度
	int height = 0;
	/// 图片大小
	int64_t size = 0;
	/// 图片类型（JPEG，PNG，……）
	std::string
		ImageType; // TODO: replace with enum, see https://github.com/mamoe/mirai/blob/dev/mirai-core-api/src/commonMain/kotlin/message/data/Image.kt#L493
	/// 是否为表情
	bool isEmoji = false;

	MiraiImage(const std::string& id = {}, const std::string& url = {}, const std::string& path = {},
	           const std::string& base64 = {})
		: id(id), url(url), path(path), base64(base64)
	{
	}

	/**
	 * @brief 检查对象能否用于发送
	 * 
	 * @return `bool`
	 */
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

/**
 * @brief Mirai语音
 * 
 * 发送时可以指定语音id，语音链接，语音路径或语音的base64编码，
 * 优先级为 id > url > path > base64，所以设置某一项属性前最好确保前面的域为空。
 * 只支持amr或silk格式的音频。
 * Member Variable | Default Value
 * --------------- | -------------
 * `MiraiAudio::id` 	| `""`
 * `MiraiAudio::url` 	| `""`
 * `MiraiAudio::path` 	| `""`
 * `MiraiAudio::base64` 	| `""`
 * `MiraiAudio::length` 	| `0`
 */
struct MiraiAudio : public Serializable
{
	/// 音频id，从mirai获得
	std::string id;
	/// 音频链接
	std::string url;
	/// 音频路径
	std::string path;
	/// 音频base64编码
	std::string base64;
	/// 音频长度
	int64_t length = 0;

	MiraiAudio(const std::string& id = {}, const std::string& url = {}, const std::string& path = {},
	           const std::string& base64 = {})
		: id(id), url(url), path(path), base64(base64)
	{
	}

	/**
	 * @brief 检查对象能否用于发送
	 * 
	 * @return `bool`
	 */
	bool isValid() const
	{
		return !(this->id.empty() && this->url.empty() && this->path.empty() && this->base64.empty());
	}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;
};

// only group audio uploads are supported for now
using GroupAudio = MiraiAudio;

} // namespace Mirai

#endif