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

namespace Mirai
{

MessageChain::MessageChain(const MessageChain& rhs) = default;

MessageChain& MessageChain::operator=(const MessageChain& rhs)
{
	if (this != &rhs)
	{
		this->_message = rhs._message;
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
	this->_message.insert(this->_message.end(), rhs._message.begin(),  rhs._message.end());
	return *this;
}

MessageChain& MessageChain::operator+=(MessageChain&& rhs)
{
	this->_message.reserve(this->_message.size() + rhs._message.size());
	this->_message.insert(
		this->_message.end(), 
		std::make_move_iterator(rhs._message.begin()),  
		std::make_move_iterator(rhs._message.end())
	);
	rhs._message.clear();
	return *this;
}


MessageChain& MessageChain::operator+=(const MessageElement& m)
{
	this->_message.push_back(m);
	return *this;
}

MessageChain& MessageChain::operator+=(MessageElement&& m)
{
	this->_message.push_back(std::move(m));
	return *this;
}


MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs)
{
	MessageChain msg = lhs;
	msg += rhs;
	return msg;
}

MessageChain operator+(const MessageChain& lhs, const MessageChain::MessageElement& rhs)
{
	MessageChain msg = lhs;
	msg += rhs;
	return msg;
}


std::optional<MessageChain::SourceInfo> MessageChain::GetSourceInfo() const
{
	for (const auto& p : this->_message)
	{
		if (p.type() == SourceMessage::GetType())
		{
			const auto& s = p.as<SourceMessage>();
			return SourceInfo{s.GetMessageId(), s.GetTimestamp()};
		}
	}
	return std::nullopt;
}


void MessageChain::RemoveInvalid()
{
	this->_message.erase(std::remove_if(this->_message.begin(), this->_message.end(),
	                                    [](const MessageElement& p)
	                                    { return !p.valid(); }),
	                     this->_message.end());
}

bool MessageChain::valid() const
{
	bool empty = true;
	for (const auto& p : this->_message)
	{
		if (!p.valid()) return false;
		if (p.allowSend()) empty = false;
	}
	return !empty;
}

} // namespace Mirai