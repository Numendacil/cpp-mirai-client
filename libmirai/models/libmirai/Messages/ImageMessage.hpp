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

#ifndef MIRAI_IMAGE_MESSAGE_HPP_
#define MIRAI_IMAGE_MESSAGE_HPP_

#include <string>
#include <utility>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/MediaTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 图片类消息
 *
 * Middleware for image related messages
 */
template <class MessageImpl>
class ImageMessageImpl : public IMessageImpl<MessageImpl>
{
protected:
	MiraiImage image_{};

	void clear_() noexcept { this->image_ = {}; }

	bool isValid_() const override { return this->image_.valid(); }

public:
	ImageMessageImpl() = default;
	ImageMessageImpl(MiraiImage image) : image_(std::move(image)) {}
	ImageMessageImpl(std::string ImageId, std::string url, std::string path, std::string base64)
		: image_(ImageId, url, path, base64)
	{
	}

	/**
	 * @brief 获取消息中的图片
	 * 
	 * @return `MiraiImage` 
	 */
	MiraiImage GetImage() const { return this->image_; }

	/**
	 * @brief 设置图片id
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param ImageId 图片id
	 */
	MessageImpl& SetImageId(std::string ImageId)
	{
		this->clear_();
		this->image_.id = std::move(ImageId);
		return *static_cast<MessageImpl*>(this);
	}

	/**
	 * @brief 设置图片链接
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param url 图片链接
	 */
	MessageImpl& SetUrl(std::string url)
	{
		this->clear_();
		this->image_.url = std::move(url);
		return *static_cast<MessageImpl*>(this);
	}

	/**
	 * @brief 设置图片路径
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param path 图片路径
	 */
	MessageImpl& SetPath(std::string path)
	{
		this->clear_();
		this->image_.path = std::move(path);
		return *static_cast<MessageImpl*>(this);
	}

	/**
	 * @brief 设置图片base64编码
	 * 
	 * 发送图片只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param base64 图片base64编码
	 */
	MessageImpl& SetBase64(std::string base64)
	{
		this->clear_();
		this->image_.base64 = std::move(base64);
		return *static_cast<MessageImpl*>(this);
	}

	/// 由 `MiraiImage` 设置图片内容
	MessageImpl& SetImage(MiraiImage image)
	{
		this->clear_();
		this->image_ = std::move(image);
		return *static_cast<MessageImpl*>(this);
	}

	struct Serializable;
};



/**
 * @brief 图片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `ImageMessage::image_` | `MiraiImage{}`
 */
class ImageMessage final : public ImageMessageImpl<ImageMessage>
{
	friend IMessageImpl<ImageMessage>;

protected:
	static constexpr MessageTypes TYPE_ = MessageTypes::IMAGE;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	using ImageMessageImpl<ImageMessage>::ImageMessageImpl;
};

template<> struct GetType<ImageMessage::GetType()>
{
	using type = ImageMessage;
};



/**
 * @brief 闪照消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FlashImageMessage::image_` | `MiraiImage{}`
 */
class FlashImageMessage final : public ImageMessageImpl<FlashImageMessage>
{
	friend IMessageImpl<FlashImageMessage>;

protected:
	static constexpr MessageTypes TYPE_ = MessageTypes::FLASH_IMAGE;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	using ImageMessageImpl<FlashImageMessage>::ImageMessageImpl;
};

template<> struct GetType<FlashImageMessage::GetType()>
{
	using type = FlashImageMessage;
};

} // namespace Mirai


#endif