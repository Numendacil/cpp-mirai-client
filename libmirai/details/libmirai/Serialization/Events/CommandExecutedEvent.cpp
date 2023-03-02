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

#include <libmirai/Events/CommandExecutedEvent.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void CommandExecutedEvent::Deserialize(const void* data)
{
	const auto& j = *static_cast<const json*>(data);
	assert(j.at("type").get<std::string>() == this->_TYPE_); // NOLINT(*-array-to-pointer-decay)

	j.at("name").get_to(this->_name);
	this->_friend = Utils::GetOptional<User>(j, "friend");
	this->_member = Utils::GetOptional<GroupMember>(j, "member");
	j.at("args").get_to(this->_args);
}

} // namespace Mirai