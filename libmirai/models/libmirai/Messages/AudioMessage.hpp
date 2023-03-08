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

#ifndef MIRAI_AUDIO_MESSAGE_HPP_
#define MIRAI_AUDIO_MESSAGE_HPP_

#include <string>
#include <utility>

#include <libmirai/Types/MediaTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 语音消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `AudioMessage::audio_` | `MiraiAudio{}`
 */
class AudioMessage final : public IMessageImpl<AudioMessage>
{
	friend IMessageImpl<AudioMessage>;

private:
	MiraiAudio audio_{};

	void clear_() noexcept { this->audio_ = {}; }

	bool isValid_() const final { return this->audio_.valid(); }

	static constexpr MessageTypes TYPE_ = MessageTypes::AUDIO;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	AudioMessage() = default;
	AudioMessage(MiraiAudio audio) : audio_(std::move(audio)) {}
	AudioMessage(std::string AudioId, std::string url, std::string path, std::string base64)
		: audio_{std::move(AudioId), std::move(url), std::move(path), std::move(base64)}
	{}

	/**
	 * @brief 获取消息中的音频内容
	 * 
	 * @return MiraiAudio 
	 */
	MiraiAudio GetAudio() const { return this->audio_; }

	/**
	 * @brief 设置音频id
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param AudioId 音频id
	 */
	AudioMessage& SetAudioId(std::string AudioId)
	{
		this->clear_();
		this->audio_.id = std::move(AudioId);
		return *this;
	}

	/**
	 * @brief 设置音频链接
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param url 音频链接
	 */
	AudioMessage& SetUrl(std::string url)
	{
		this->clear_();
		this->audio_.url = std::move(url);
		return *this;
	}

	/**
	 * @brief 设置音频路径
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param path 音频路径
	 */
	AudioMessage& SetPath(std::string path)
	{
		this->clear_();
		this->audio_.path = std::move(path);
		return *this;
	}

	/**
	 * @brief 设置音频内容的base64编码
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param base64 音频base64编码
	 */
	AudioMessage& SetBase64(std::string base64)
	{
		this->clear_();
		this->audio_.base64 = std::move(base64);
		return *this;
	}

	/// 由 `MiraiAudio` 设置音频内容
	AudioMessage& SetAudio(MiraiAudio audio)
	{
		this->audio_ = std::move(audio);
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<AudioMessage::GetType()>
{
	using type = AudioMessage;
};

} // namespace Mirai


#endif