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

#include "GroupNameChangeEvent.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void GroupNameChangeEvent::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	this->_origin = Utils::GetValue(data, "origin", "");
	this->_current = Utils::GetValue(data, "current", "");
	this->_group = Utils::GetValue(data, "group", Group{});
	this->_operator = Utils::GetOptional<GroupMember>(data, "operator");
}

} // namespace Mirai