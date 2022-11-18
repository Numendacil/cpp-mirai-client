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

#include <libmirai/Messages/FaceMessage.hpp>

#include <nlohmann/json.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void FaceMessage::Deserialize(const void* data)
{
	const auto& j = *static_cast<const json*>(data);

	assert(j.at("type").get<MessageTypes>() == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	this->_id = Utils::GetValue(j, "faceId", (int64_t)-1);
	this->_name = Utils::GetValue(j, "name", "");
}

void FaceMessage::Serialize(void* data) const
{
	auto& j = *static_cast<json*>(data);
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	j["type"] = this->GetType();
	if (this->_id != -1) j["faceId"] = this->_id;
	else
		j["name"] = this->_name;
}

} // namespace Mirai