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

#include "ForwardMessage.hpp"

#include <optional>
#include <vector>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>


namespace Mirai
{

using json = nlohmann::json;

bool ForwardMessage::Node::isValid() const
{
	return this->_MessageId.has_value() || (!this->_SenderName.empty() && this->_message.isValid());
}

void ForwardMessage::Node::FromJson(const json& data)
{
	this->_SenderId = Utils::GetValue(data, "senderId", QQ_t{});
	this->_time = Utils::GetValue(data, "time", (std::time_t)0);
	this->_SenderName = Utils::GetValue(data, "senderName", "");
	this->_message = Utils::GetValue(data, "messageChain", MessageChain{});
	this->_MessageId = Utils::GetOptional<MessageId_t>(data, "messageId");
}

json ForwardMessage::Node::ToJson() const
{
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	json data = json::object();
	if (this->_MessageId.has_value()) data["messageId"] = this->_MessageId.value();
	else
	{
		data["senderId"] = _SenderId;
		data["time"] = this->_time;
		data["senderName"] = this->_SenderName;
		data["messageChain"] = this->_message;
	}
	return data;
}

bool ForwardMessage::isValid() const
{
	for (const auto& n : this->_NodeList)
		if (!n.isValid()) return false;
	return true;
}

void ForwardMessage::FromJson(const json& data)
{
	assert(Utils::GetValue(data, "type", "") == this->GetType()); // NOLINT(*-array-to-pointer-decay)
	json nodelist = Utils::GetValue(data, "nodeList", json::array());
	assert(nodelist.is_array()); // NOLINT(*-array-to-pointer-decay)

	this->_NodeList = nodelist.get<std::vector<Node>>();
}

json ForwardMessage::ToJson() const
{
	// assert(this->isValid());	// NOLINT(*-array-to-pointer-decay)

	json data = json::object();
	data["type"] = this->GetType();
	data["nodeList"] = this->_NodeList;
	return data;
}

} // namespace Mirai