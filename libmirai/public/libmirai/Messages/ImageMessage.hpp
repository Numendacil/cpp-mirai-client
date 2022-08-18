#ifndef _MIRAI_IMAGE_MESSAGE_HPP_
#define _MIRAI_IMAGE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/MediaTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 图片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `ImageMessage::_image` | `MiraiImage{}`
 */
class ImageMessage : public MessageBase
{
protected:
	MiraiImage _image;

	void _clear() noexcept { this->_image = {}; }

public:
	ImageMessage(const MiraiImage& image = {}) : _image(image) {}
	ImageMessage(std::string ImageId, std::string url, std::string path, std::string base64)
		: _image(ImageId, url, path, base64)
	{
	}
	ImageMessage(const ImageMessage&) = default;
	ImageMessage& operator=(const ImageMessage&) = default;
	ImageMessage(ImageMessage&&) noexcept = default;
	ImageMessage& operator=(ImageMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Image";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual ImageMessage* Clone() const override { return new ImageMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	/**
	 * @brief 获取消息中的图片
	 * 
	 * @return `MiraiImage` 
	 */
	MiraiImage GetImage() const { return this->_image; }

	/**
	 * @brief 设置图片id
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param ImageId 图片id
	 */
	ImageMessage& SetImageId(const std::string& ImageId)
	{
		this->_clear();
		this->_image.id = ImageId;
		return *this;
	}

	/**
	 * @brief 设置图片链接
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param url 图片链接
	 */
	ImageMessage& SetUrl(const std::string& url)
	{
		this->_clear();
		this->_image.url = url;
		return *this;
	}

	/**
	 * @brief 设置图片路径
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param path 图片路径
	 */
	ImageMessage& SetPath(const std::string& path)
	{
		this->_clear();
		this->_image.path = path;
		return *this;
	}

	/**
	 * @brief 设置图片base64编码
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param base64 图片base64编码
	 */
	ImageMessage& SetBase64(const std::string& base64)
	{
		this->_clear();
		this->_image.base64 = base64;
		return *this;
	}

	/// 由 `MiraiImage` 设置图片内容
	ImageMessage& SetImage(const MiraiImage& image)
	{
		this->_clear();
		this->_image = image;
		return *this;
	}
};

} // namespace Mirai


#endif