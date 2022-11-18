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

#include "SourceMessage.hpp"

namespace Mirai
{

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
		m._message.push_back(p->CloneUnique());
	}
	for (const auto& p : rhs)
	{
		m._message.push_back(p->CloneUnique());
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
		if (p->isSendSupported()) empty = false;
	}
	return !empty;
}

} // namespace Mirai