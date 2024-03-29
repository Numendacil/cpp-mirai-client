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

#ifndef MIRAI_SERIALIZATION_FORWARD_MESSAGE_NODE_HPP_
#define MIRAI_SERIALIZATION_FORWARD_MESSAGE_NODE_HPP_

#include <nlohmann/json.hpp>

#include <libmirai/Messages/ForwardMessageNode.hpp>
#include <libmirai/Serialization/Types/Serializable.hpp>
#include <libmirai/Serialization/Messages/MessageChain.hpp>
#include <libmirai/Serialization/Types/Types.hpp>
#include <libmirai/Utils/Common.hpp>

namespace Mirai
{

struct ForwardNode::Serializable
{

	static void from_json(const nlohmann::json& j, ForwardNode& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("senderId").get_to(p.SenderId_);
		j.at("time").get_to(p.time_);
		j.at("senderName").get_to(p.SenderName_);
		j.at("messageChain").get_to(p.message_);
		p.MessageId_ = Utils::GetOptional<MessageId_t>(j, "messageId");

		auto it = j.find("messageRef");
		if (it == j.end() || it.value().is_null())
			p.ref_ = std::nullopt;
		else
		{
			p.ref_ = {it.value().at("messageId").get<MessageId_t>(), it.value().at("target").get<int64_t>()};
		}

		MIRAI_PARSE_GUARD_END(j);
	}

	static void from_json(nlohmann::json&& j, ForwardNode& p)
	{
		MIRAI_PARSE_GUARD_BEGIN(j);

		j.at("senderId").get_to(p.SenderId_);
		j.at("time").get_to(p.time_);
		::Mirai::from_json(std::move(j.at("senderName")), p.SenderName_);
		::Mirai::from_json(std::move(j.at("messageChain")), p.message_);
		p.MessageId_ = Utils::GetOptional<MessageId_t>(j, "messageId");
		
		auto it = j.find("messageRef");
		if (it == j.end() || it.value().is_null())
			p.ref_ = std::nullopt;
		else
		{
			p.ref_ = {it.value().at("messageId").get<MessageId_t>(), it.value().at("target").get<int64_t>()};
		}

		MIRAI_PARSE_GUARD_END(j);
	}

	static void to_json(nlohmann::json& j, const ForwardNode& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		if (p.MessageId_) 
			j["messageId"] = p.MessageId_.value();
		else if (p.ref_)
		{
			j["messageRef"]["messageId"] = p.ref_->MessageId;
			j["messageRef"]["target"] = p.ref_->target;
		}
		else
		{
			j["senderId"] = p.SenderId_;
			j["time"] = p.time_;
			j["senderName"] = p.SenderName_;
			j["messageChain"] = p.message_;
		}
	}

	static void to_json(nlohmann::json& j, ForwardNode&& p)
	{
		// assert(p.valid());	// NOLINT(*-array-to-pointer-decay)

		if (p.MessageId_) 
			j["messageId"] = p.MessageId_.value();
		else if (p.ref_)
		{
			j["messageRef"]["messageId"] = p.ref_->MessageId;
			j["messageRef"]["target"] = p.ref_->target;
		}
		else
		{
			j["senderId"] = p.SenderId_;
			j["time"] = p.time_;
			j["senderName"] = std::move(p.SenderName_);
			j["messageChain"] = std::move(p.message_);
		}
	}

};

}

#endif