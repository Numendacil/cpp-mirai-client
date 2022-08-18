#include "AudioMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool AudioMessage::isValid() const
{
	return this->_audio.isValid();
}

void AudioMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_audio = data.get<MiraiAudio>();
}

json AudioMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data.update(this->_audio);

	return data;
}

} // namespace Mirai