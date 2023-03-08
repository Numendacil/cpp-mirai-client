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

#include "QuoteMessage.hpp"

#include <memory>

#include "MessageChain.hpp"

namespace Mirai
{

QuoteMessage::QuoteMessage() = default;

QuoteMessage::QuoteMessage(const QuoteMessage& rhs) 
: QuoteId_(rhs.QuoteId_), GroupId_(rhs.GroupId_), SenderId_(rhs.SenderId_), TargetId_(rhs.TargetId_)
{
	if (rhs.origin_)
		this->origin_ = std::make_unique<MessageChain>(*rhs.origin_);
}

QuoteMessage& QuoteMessage::operator=(const QuoteMessage& rhs)
{
	if (&rhs != this)
	{
		this->QuoteId_ = rhs.QuoteId_;
		this->GroupId_ = rhs.GroupId_;
		this->SenderId_ = rhs.SenderId_;
		this->TargetId_ = rhs.TargetId_;
		if (rhs.origin_)
		{
			if (!this->origin_)
				this->origin_ = std::make_unique<MessageChain>(*rhs.origin_);
			else
				*this->origin_ = *rhs.origin_;
		}
		else
			this->origin_ = nullptr;
	}
	return *this;
}

QuoteMessage::QuoteMessage(QuoteMessage&&) = default;
QuoteMessage& QuoteMessage::operator=(QuoteMessage&&) = default;

MessageChain QuoteMessage::GetOriginMessage() const
{
	if (!this->origin_)
		return {};
	return *this->origin_;
}

} // namespace Mirai