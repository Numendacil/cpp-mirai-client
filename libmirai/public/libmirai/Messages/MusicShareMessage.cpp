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

#include "MusicShareMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool MusicShareMessage::isValid() const
{
	return !(this->_kind == MusicKind::UNKNOWN || this->_title.empty() || this->_summary.empty()
	         || this->_JumpUrl.empty() || this->_PictureUrl.empty() || this->_MusicUrl.empty() || this->_brief.empty());
}

void MusicShareMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType()); // NOLINT(*-array-to-pointer-decay)
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
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

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

} // namespace Mirai