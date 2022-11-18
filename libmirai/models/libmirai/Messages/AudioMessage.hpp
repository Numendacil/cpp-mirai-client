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

#ifndef _MIRAI_AUDIO_MESSAGE_HPP_
#define _MIRAI_AUDIO_MESSAGE_HPP_

#include <string>
#include <utility>


#include <libmirai/Types/MediaTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 语音消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `AudioMessage::_audio` | `MiraiAudio{}`
 */
class AudioMessage : public MessageBase
{
protected:
	MiraiAudio _audio{};

	void _clear() noexcept { this->_audio = {}; }

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::AUDIO;

	AudioMessage() : MessageBase(_TYPE_) {}
	AudioMessage(MiraiAudio audio) : _audio(std::move(audio)), MessageBase(_TYPE_) {}
	AudioMessage(std::string AudioId, std::string url, std::string path, std::string base64)
		: _audio{std::move(AudioId), std::move(url), std::move(path), std::move(base64)},
		MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<AudioMessage>(*this); }

	bool isValid() const final
	{
		return this->_audio.isValid();
	}

	/**
	 * @brief 获取消息中的音频内容
	 * 
	 * @return MiraiAudio 
	 */
	MiraiAudio GetAudio() const { return this->_audio; }

	/**
	 * @brief 设置音频id
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param AudioId 音频id
	 */
	AudioMessage& SetAudioId(std::string AudioId)
	{
		this->_clear();
		this->_audio.id = std::move(AudioId);
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
		this->_clear();
		this->_audio.url = std::move(url);
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
		this->_clear();
		this->_audio.path = std::move(path);
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
		this->_clear();
		this->_audio.base64 = std::move(base64);
		return *this;
	}

	/// 由 `MiraiAudio` 设置音频内容
	AudioMessage& SetAudio(MiraiAudio audio)
	{
		this->_audio = std::move(audio);
		return *this;
	}
};

template <>
struct GetType<AudioMessage::_TYPE_>
{
	using type = AudioMessage;
};

} // namespace Mirai


#endif