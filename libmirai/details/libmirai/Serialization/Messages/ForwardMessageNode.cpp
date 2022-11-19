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

#include "ForwardMessageNode.hpp"

#include <nlohmann/json.hpp>

#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

using json = nlohmann::json;

void ForwardMessage::Node::Serializable::from_json(const json& j, ForwardMessage::Node& p)
{
	MIRAI_PARSE_GUARD_BEGIN;

	j.at("senderId").get_to(p._SenderId);
	j.at("time").get_to(p._time);
	j.at("senderName").get_to(p._SenderName);
	j.at("messageChain").get_to(p._message);
	p._MessageId = Utils::GetOptional<MessageId_t>(j, "messageId");

	MIRAI_PARSE_GUARD_END;
}

void ForwardMessage::Node::Serializable::to_json(nlohmann::json& j, const ForwardMessage::Node& p)
{
	// assert(p.isValid());	// NOLINT(*-array-to-pointer-decay)

	if (p._MessageId.has_value()) j["messageId"] = p._MessageId.value();
	else
	{
		j["senderId"] = p._SenderId;
		j["time"] = p._time;
		j["senderName"] = p._SenderName;
		j["messageChain"] = p._message;
	}
}

} // namespace Mirai