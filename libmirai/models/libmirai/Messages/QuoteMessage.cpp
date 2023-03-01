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
: _QuoteId(rhs._QuoteId), _GroupId(rhs._GroupId), _SenderId(rhs._SenderId), _TargetId(rhs._TargetId)
{
	if (rhs._origin)
		this->_origin = std::make_unique<MessageChain>(*rhs._origin);
}

QuoteMessage& QuoteMessage::operator=(const QuoteMessage& rhs)
{
	if (&rhs != this)
	{
		this->_QuoteId = rhs._QuoteId;
		this->_GroupId = rhs._GroupId;
		this->_SenderId = rhs._SenderId;
		this->_TargetId = rhs._TargetId;
		if (rhs._origin)
		{
			if (!this->_origin)
				this->_origin = std::make_unique<MessageChain>(*rhs._origin);
			else
				*this->_origin = *rhs._origin;
		}
		else
			this->_origin = nullptr;
	}
	return *this;
}

QuoteMessage::QuoteMessage(QuoteMessage&&) = default;
QuoteMessage& QuoteMessage::operator=(QuoteMessage&&) = default;

MessageChain QuoteMessage::GetOriginMessage() const
{
	if (!this->_origin)
		return {};
	return *this->_origin;
}

} // namespace Mirai