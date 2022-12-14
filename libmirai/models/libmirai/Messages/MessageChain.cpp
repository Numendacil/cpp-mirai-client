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

#include "MessageChain.hpp"

#include <algorithm>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>

#include <libmirai/Utils/Common.hpp>
#include <libmirai/Utils/Factory.hpp>

#include "ForwardMessage.hpp"
#include "Messages.hpp"

namespace Mirai
{

using json = nlohmann::json;

MessageChain::MessageChain(const MessageChain& rhs)
{
	MessageContainer v;
	v.reserve(rhs._message.size());
	for (const auto& p : rhs._message)
	{
		v.push_back(p->CloneUnique());
	}
	this->_message.swap(v);
}

MessageChain& MessageChain::operator=(const MessageChain& rhs)
{
	if (this != &rhs)
	{
		MessageContainer v;
		v.reserve(rhs._message.size());
		for (const auto& p : rhs._message)
		{
			v.push_back(p->CloneUnique());
		}
		this->_message.swap(v);
	}
	return *this;
}

MessageChain::MessageChain(MessageChain&& rhs) noexcept : _message(std::move(rhs._message)) {}

MessageChain& MessageChain::operator=(MessageChain&& rhs) noexcept
{
	if (this != &rhs) this->_message = std::move(rhs._message);
	return *this;
}


MessageChain& MessageChain::operator+=(const MessageChain& rhs)
{
	this->_message.reserve(this->_message.size() + rhs._message.size());
	for (const auto& p : rhs._message)
	{
		this->_message.push_back(p->CloneUnique());
	}
	return *this;
}

MessageChain& MessageChain::operator+=(const MessageBase& rhs)
{
	this->_message.push_back(rhs.CloneUnique());
	return *this;
}

MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs)
{
	MessageChain m;
	m.reserve(lhs.size() + rhs.size());
	for (const auto& p : lhs)
	{
		m.push_back(p->CloneUnique());
	}
	for (const auto& p : rhs)
	{
		m.push_back(p->CloneUnique());
	}
	return m;
}

MessageChain operator+(const MessageChain& lhs, const MessageBase& rhs)
{
	MessageChain m{lhs};
	m += rhs;
	return m;
}

MessageChain operator+(const MessageBase& lhs, const MessageChain& rhs)
{
	return rhs + lhs;
}


std::optional<MessageChain::SourceInfo> MessageChain::GetSourceInfo() const
{
	for (const auto& p : this->_message)
	{
		if (p->GetType() == SourceMessage::_TYPE_)
		{
			auto s = static_cast<SourceMessage*>(p.get());
			return SourceInfo{s->GetMessageId(), s->GetTimestamp()};
		}
	}
	return std::nullopt;
}


namespace
{

Utils::Factory<MessageBase> InitFactory()
{
	Utils::Factory<MessageBase> f;

#ifdef NDEBUG
#define _REGISTER_(_class_) f.Register<_class_>(std::string(_class_::_TYPE_))
#else
#define _REGISTER_(_class_) assert(f.Register<_class_>(std::string(_class_::_TYPE_)))
#endif

	_REGISTER_(AppMessage);        // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(AtAllMessage);      // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(AtMessage);         // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(AudioMessage);      // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(DiceMessage);       // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(FaceMessage);       // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(FileMessage);       // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(FlashImageMessage); // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(ForwardMessage);    // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(ImageMessage);      // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(JsonMessage);       // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(MarketFaceMessage); // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(MiraiCodeMessage);  // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(MusicShareMessage); // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(PlainMessage);      // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(PokeMessage);       // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(QuoteMessage);      // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(SourceMessage);     // NOLINT(*-array-to-pointer-decay)
	_REGISTER_(XmlMessage);        // NOLINT(*-array-to-pointer-decay)

#undef _REGISTER_

	return f;
}

const Utils::Factory<MessageBase> MessageFactory{std::move(InitFactory())};

} // namespace

void MessageChain::RemoveInvalid()
{
	this->_message.erase(
		std::remove_if(this->_message.begin(), this->_message.end(), [](const value_type& p) { return !p->isValid(); }),
		this->_message.end());
}

bool MessageChain::isValid() const
{
	bool empty = true;
	for (const auto& p : this->_message)
	{
		if (!p->isValid()) return false;
		if (empty && !p->ToJson().empty()) empty = false;
	}
	return !empty;
}

void MessageChain::FromJson(const json& data)
{
	if (!data.is_array()) return;
	this->_message.clear();
	this->_message.reserve(data.size());
	for (const auto& p : data)
	{
		if (!p.is_object() || !p.contains("type") || !p["type"].is_string()) continue;
		std::string type = p["type"].get<std::string>();
		if (!MessageFactory.Exist(type)) continue;
		this->_message.emplace_back(MessageFactory.Make(type))->FromJson(p);
	}
}

json MessageChain::ToJson(bool ignoreInvalid) const
{
	json data = json::array();
	for (const auto& p : this->_message)
	{
		if (ignoreInvalid && !p->isValid()) continue;
		json msg = p->ToJson();
		if (!msg.empty() && msg.contains("type")) data += msg;
	}
	return data;
}

MessageChain& MessageChain::Forward(ForwardMessage&& forward)
{
	return this->Append(std::forward<ForwardMessage>(forward));
}

void to_json(json& j, const MessageChain& p)
{
	j = p.ToJson();
}

void from_json(const json& j, MessageChain& p)
{
	p.FromJson(j);
}

} // namespace Mirai