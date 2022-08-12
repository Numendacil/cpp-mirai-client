#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "AudioMessage.hpp"

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

}