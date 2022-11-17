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

#include <libmirai/Messages/XmlMessage.hpp>

#include <nlohmann/json.hpp>
#include <libmirai/Serialization/Types/Types.hpp>


namespace Mirai
{

using json = nlohmann::json;

void XmlMessage::Deserialize(const void* data)
{
	const auto& j = *static_cast<const json*>(data);

	assert(j.at("type").get<MessageTypes>() == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	j.at("xml").get_to(this->_content);
}

void XmlMessage::Serialize(void* data) const
{
	auto& j = *static_cast<json*>(data);
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	j["type"] = this->GetType();
	j["xml"] = this->_content;
}

} // namespace Mirai