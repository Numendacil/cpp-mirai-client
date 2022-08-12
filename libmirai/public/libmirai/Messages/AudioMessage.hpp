#ifndef _MIRAI_AUDIO_MESSAGE_HPP_
#define _MIRAI_AUDIO_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/MediaTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class AudioMessage : public MessageBase
{
protected:
	MiraiAudio _audio;

	void _clear() noexcept
	{
		this->_audio = {};
	}

public:
	AudioMessage(const MiraiAudio& audio = {})
	: _audio(audio) {}
	AudioMessage(std::string AudioId, std::string url, std::string path, std::string base64)
	: _audio(AudioId, url, path, base64) {}
	AudioMessage(const AudioMessage&) = default;
	AudioMessage& operator=(const AudioMessage&) = default;
	AudioMessage(AudioMessage&&) noexcept = default;
	AudioMessage& operator=(AudioMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Voice";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual AudioMessage* Clone() const override
	{
		return new AudioMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	MiraiAudio GetAudio() const { return this->_audio; }

	void SetAudioId(const std::string& AudioId) { this->_clear(); this->_audio.id = AudioId; }
	void SetUrl(const std::string& url) { this->_clear(); this->_audio.url = url; }
	void SetPath(const std::string& path) { this->_clear(); this->_audio.path = path; }
	void SetBase64(const std::string& base64) { this->_clear(); this->_audio.base64 = base64; }
	void SetAudio(const MiraiAudio& audio) { this->_audio = audio; }

};

}


#endif