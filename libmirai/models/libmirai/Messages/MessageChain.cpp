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
		this->message_ = rhs.message_;
	}
	return *this;
}

MessageChain::MessageChain(MessageChain&& rhs) noexcept : message_(std::move(rhs.message_)) {}

MessageChain& MessageChain::operator=(MessageChain&& rhs) noexcept
{
	if (this != &rhs) this->message_ = std::move(rhs.message_);
	return *this;
}


MessageChain& MessageChain::operator+=(const MessageChain& rhs)
{
	this->message_.reserve(this->message_.size() + rhs.message_.size());
	this->message_.insert(this->message_.end(), rhs.message_.begin(),  rhs.message_.end());
	return *this;
}

MessageChain& MessageChain::operator+=(MessageChain&& rhs)
{
	this->message_.reserve(this->message_.size() + rhs.message_.size());
	this->message_.insert(
		this->message_.end(), 
		std::make_move_iterator(rhs.message_.begin()),  
		std::make_move_iterator(rhs.message_.end())
	);
	rhs.message_.clear();
	return *this;
}


MessageChain& MessageChain::operator+=(const MessageElement& m)
{
	this->message_.push_back(m);
	return *this;
}

MessageChain& MessageChain::operator+=(MessageElement&& m)
{
	this->message_.push_back(std::move(m));
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
	for (const auto& p : this->message_)
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
	this->message_.erase(std::remove_if(this->message_.begin(), this->message_.end(),
	                                    [](const MessageElement& p)
	                                    { return !p.valid(); }),
	                     this->message_.end());
}

bool MessageChain::valid() const
{
	bool empty = true;
	for (const auto& p : this->message_)
	{
		if (!p.valid()) return false;
		if (p.allowSend()) empty = false;
	}
	return !empty;
}

} // namespace Mirai