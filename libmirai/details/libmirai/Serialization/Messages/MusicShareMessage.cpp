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

#include <nlohmann/json.hpp>

#include <libmirai/Messages/MusicShareMessage.hpp>
#include <libmirai/Serialization/Types/Types.hpp>


namespace Mirai
{

using json = nlohmann::json;

void MusicShareMessage::Deserialize(const void* data)
{
	const auto& j = *static_cast<const json*>(data);

	assert(j.at("type").get<MessageTypes>() == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	j.at("kind").get_to(this->_kind);
	j.at("title").get_to(this->_title);
	j.at("summary").get_to(this->_summary);
	j.at("jumpUrl").get_to(this->_JumpUrl);
	j.at("pictureUrl").get_to(this->_PictureUrl);
	j.at("musicUrl").get_to(this->_MusicUrl);
	j.at("brief").get_to(this->_brief);
}

void MusicShareMessage::Serialize(void* data) const
{
	auto& j = *static_cast<json*>(data);
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	j["type"] = this->GetType();
	j["kind"] = this->_kind;
	j["title"] = this->_title;
	j["summary"] = this->_summary;
	j["jumpUrl"] = this->_JumpUrl;
	j["pictureUrl"] = this->_PictureUrl;
	j["musicUrl"] = this->_MusicUrl;
	j["brief"] = this->_brief;
}

} // namespace Mirai