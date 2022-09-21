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

#include "SourceMessage.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool SourceMessage::isValid() const
{
	return true;
}

void SourceMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType()); // NOLINT(*-array-to-pointer-decay)

	this->_id = Utils::GetValue(data, "id", -1);
	this->_timestamp = Utils::GetValue(data, "time", (std::time_t)0);
}

json SourceMessage::ToJson() const
{
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	// json data = json::object();
	// data["type"] = this->GetType();
	// data["id"] = this->id;
	// data["time"] = this->timestamp;
	// return data;
	return json::object();
}

} // namespace Mirai