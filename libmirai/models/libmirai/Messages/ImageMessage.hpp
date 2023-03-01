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
	friend IMessageImpl<MessageImpl>;

protected:
	MiraiImage _image{};

	void _clear() noexcept { this->_image = {}; }

	bool _isValid() const override { return this->_image.valid(); }

public:
	ImageMessageImpl() = default;
	ImageMessageImpl(MiraiImage image) : _image(std::move(image)) {}
	ImageMessageImpl(std::string ImageId, std::string url, std::string path, std::string base64)
		: _image(ImageId, url, path, base64)
	{
	}

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
	MessageImpl& SetImageId(std::string ImageId)
	{
		this->_clear();
		this->_image.id = std::move(ImageId);
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
		this->_clear();
		this->_image.url = std::move(url);
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
		this->_clear();
		this->_image.path = std::move(path);
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
		this->_clear();
		this->_image.base64 = std::move(base64);
		return *static_cast<MessageImpl*>(this);
	}

	/// 由 `MiraiImage` 设置图片内容
	MessageImpl& SetImage(MiraiImage image)
	{
		this->_clear();
		this->_image = std::move(image);
		return *static_cast<MessageImpl*>(this);
	}

	struct Serializable;
};



/**
 * @brief 图片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `ImageMessage::_image` | `MiraiImage{}`
 */
class ImageMessage final : public ImageMessageImpl<ImageMessage>
{
	friend IMessageImpl<ImageMessage>;

protected:
	static constexpr MessageTypes _TYPE_ = MessageTypes::IMAGE;
	static constexpr bool _SUPPORT_SEND_ = true;

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
 * `FlashImageMessage::_image` | `MiraiImage{}`
 */
class FlashImageMessage final : public ImageMessageImpl<FlashImageMessage>
{
	friend IMessageImpl<FlashImageMessage>;

protected:
	static constexpr MessageTypes _TYPE_ = MessageTypes::FLASH_IMAGE;
	static constexpr bool _SUPPORT_SEND_ = true;

public:
	using ImageMessageImpl<FlashImageMessage>::ImageMessageImpl;
};

template<> struct GetType<FlashImageMessage::GetType()>
{
	using type = FlashImageMessage;
};

} // namespace Mirai


#endif