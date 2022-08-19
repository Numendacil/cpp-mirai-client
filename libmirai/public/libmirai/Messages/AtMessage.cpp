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

#include "AtMessage.hpp"

#include <cassert>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

bool AtMessage::isValid() const
{
	return this->_target != QQ_t();
}

void AtMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType());

	this->_target = Utils::GetValue(data, "target", QQ_t{});
	this->_display = Utils::GetValue(data, "display", "");
}

json AtMessage::ToJson() const
{
	// assert(this->isValid());

	json data = json::object();
	data["type"] = this->GetType();
	data["target"] = _target;

	// Not used for sending
	// data["display"] = this->display;

	return data;
}

} // namespace Mirai