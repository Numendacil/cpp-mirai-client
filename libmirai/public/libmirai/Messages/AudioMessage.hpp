#ifndef _MIRAI_AUDIO_MESSAGE_HPP_
#define _MIRAI_AUDIO_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

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
	MiraiAudio _audio;

	void _clear() noexcept { this->_audio = {}; }

public:
	AudioMessage(const MiraiAudio& audio = {}) : _audio(audio) {}
	AudioMessage(std::string AudioId, std::string url, std::string path, std::string base64)
		: _audio(AudioId, url, path, base64)
	{
	}
	AudioMessage(const AudioMessage&) = default;
	AudioMessage& operator=(const AudioMessage&) = default;
	AudioMessage(AudioMessage&&) noexcept = default;
	AudioMessage& operator=(AudioMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Voice";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual AudioMessage* Clone() const override { return new AudioMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

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