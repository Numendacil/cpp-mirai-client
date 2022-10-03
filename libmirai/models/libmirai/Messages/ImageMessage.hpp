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

#ifndef _MIRAI_IMAGE_MESSAGE_HPP_
#define _MIRAI_IMAGE_MESSAGE_HPP_

#include <string>
#include <utility>

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
	MiraiImage _image{};

	void _clear() noexcept { this->_image = {}; }

public:
	ImageMessage() = default;
	ImageMessage(MiraiImage image) : _image(std::move(image)) {}
	ImageMessage(std::string ImageId, std::string url, std::string path, std::string base64)
		: _image(ImageId, url, path, base64)
	{
	}

	static constexpr std::string_view _TYPE_ = "Image";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<ImageMessage>(*this); }

	bool isValid() const override;

	void FromJson(const nlohmann::json& data) override;
	nlohmann::json ToJson() const override;

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