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

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/MediaTypes.hpp>
#include <utility>

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

public:
	AudioMessage() = default;
	AudioMessage(MiraiAudio audio) : _audio(std::move(audio)) {}
	AudioMessage(std::string AudioId, std::string url, std::string path, std::string base64)
		: _audio{std::move(AudioId), std::move(url), std::move(path), std::move(base64)}
	{
	}

	static constexpr std::string_view _TYPE_ = "Voice";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<AudioMessage>(*this); }

	bool isValid() const override;

	void FromJson(const nlohmann::json& data) override;
	nlohmann::json ToJson() const override;

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
	AudioMessage& SetAudioId(const std::string& AudioId)
	{
		this->_clear();
		this->_audio.id = AudioId;
		return *this;
	}

	/**
	 * @brief 设置音频链接
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param url 音频链接
	 */
	AudioMessage& SetUrl(const std::string& url)
	{
		this->_clear();
		this->_audio.url = url;
		return *this;
	}

	/**
	 * @brief 设置音频路径
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param path 音频路径
	 */
	AudioMessage& SetPath(const std::string& path)
	{
		this->_clear();
		this->_audio.path = path;
		return *this;
	}

	/**
	 * @brief 设置音频内容的base64编码
	 * 
	 * 发送音频只需要id、链接、路径、base64编码中的一个，因此该方法会清空其它的属性
	 * @param base64 音频base64编码
	 */
	AudioMessage& SetBase64(const std::string& base64)
	{
		this->_clear();
		this->_audio.base64 = base64;
		return *this;
	}

	/// 由 `MiraiAudio` 设置音频内容
	AudioMessage& SetAudio(const MiraiAudio& audio)
	{
		this->_audio = audio;
		return *this;
	}
};

} // namespace Mirai


#endif