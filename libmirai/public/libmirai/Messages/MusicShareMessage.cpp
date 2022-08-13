#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include "MusicShareMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

bool MusicShareMessage::isValid() const
{
	return !(this->_kind == MusicKind::UNKNOWN || this->_title.empty() || this->_summary.empty()
	|| this->_JumpUrl.empty() || this->_PictureUrl.empty() || this->_MusicUrl.empty()
	|| this->_brief.empty());
}

void MusicShareMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());
	this->_kind = _to_enum(Utils::GetValue(data, "kind", ""));
	this->_title = Utils::GetValue(data, "title", "");
	this->_summary = Utils::GetValue(data, "summary", "");
	this->_JumpUrl = Utils::GetValue(data, "jumpUrl", "");
	this->_PictureUrl = Utils::GetValue(data, "pictureUrl", "");
	this->_MusicUrl = Utils::GetValue(data, "musicUrl", "");
	this->_brief = Utils::GetValue(data, "brief", "");
}

json MusicShareMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["kind"] = _to_string(this->_kind);
	data["title"] = this->_title;
	data["summary"] = this->_summary;
	data["jumpUrl"] = this->_JumpUrl;
	data["pictureUrl"] = this->_PictureUrl;
	data["musicUrl"] = this->_MusicUrl;
	data["brief"] = this->_brief;
	return data;
}

}