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

#include "ForwardMessageNode.hpp"

namespace Mirai
{

ForwardMessage::ForwardMessage() = default;
ForwardMessage::ForwardMessage(const ForwardMessage&) = default;
ForwardMessage& ForwardMessage::operator= (const ForwardMessage&) = default;
ForwardMessage::ForwardMessage(ForwardMessage&&) = default;
ForwardMessage& ForwardMessage::operator= (ForwardMessage&&) = default;
ForwardMessage::~ForwardMessage() = default;

bool ForwardMessage::isValid_() const
{
	for (const auto& n : this->NodeList_)
		if (!n.valid()) return false;
	return true;
}

bool ForwardMessage::empty() const noexcept
{
	return this->NodeList_.empty();
}
ForwardMessage::size_type ForwardMessage::size() const noexcept
{
	return this->NodeList_.size();
}
void ForwardMessage::reserve(size_type new_cap)
{
	return this->NodeList_.reserve(new_cap);
}
void ForwardMessage::shrink_to_fit() noexcept
{
	return this->NodeList_.shrink_to_fit();
}
ForwardMessage::size_type ForwardMessage::max_size() const noexcept
{
	return this->NodeList_.max_size();
}
ForwardMessage::size_type ForwardMessage::capacity() const noexcept
{
	return this->NodeList_.capacity();
}

ForwardMessage::const_reference ForwardMessage::at(size_type n) const
{
	return this->NodeList_.at(n);
}
ForwardMessage::reference ForwardMessage::at(size_type n)
{
	return this->NodeList_.at(n);
}
ForwardMessage::const_reference ForwardMessage::operator[](size_type n) const noexcept
{
	return this->NodeList_[n];
}
ForwardMessage::reference ForwardMessage::operator[](size_type n) noexcept
{
	return this->NodeList_[n];
}
ForwardMessage::const_reference ForwardMessage::back() const
{
	return this->NodeList_.back();
}
ForwardMessage::reference ForwardMessage::back()
{
	return this->NodeList_.back();
}
ForwardMessage::const_reference ForwardMessage::front() const
{
	return this->NodeList_.front();
}
ForwardMessage::reference ForwardMessage::front()
{
	return this->NodeList_.front();
}

void ForwardMessage::clear() noexcept
{
	return this->NodeList_.clear();
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, const_reference value)
{
	return this->NodeList_.insert(pos, value);
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, value_type&& value)
{
	return this->NodeList_.insert(pos, std::move(value));
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, size_type count, const_reference value)
{
	return this->NodeList_.insert(pos, count, value);
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, std::initializer_list<value_type> ilist)
{
	return this->NodeList_.insert(pos, ilist);
}
ForwardMessage::iterator ForwardMessage::erase(const_iterator pos)
{
	return this->NodeList_.erase(pos);
}
ForwardMessage::iterator ForwardMessage::erase(const_iterator first, const_iterator last)
{
	return this->NodeList_.erase(first, last);
}
void ForwardMessage::push_back(const_reference node)
{
	return this->NodeList_.push_back(node);
}
void ForwardMessage::push_back(value_type&& node)
{
	return this->NodeList_.push_back(std::move(node));
}
void ForwardMessage::pop_back()
{
	return this->NodeList_.pop_back();
}
void ForwardMessage::resize(size_type count)
{
	return this->NodeList_.resize(count);
}
void ForwardMessage::resize(size_type count, const value_type& value)
{
	return this->NodeList_.resize(count, value);
}

ForwardMessage::iterator ForwardMessage::begin() noexcept
{
	return this->NodeList_.begin();
}
ForwardMessage::const_iterator ForwardMessage::begin() const noexcept
{
	return this->NodeList_.begin();
}
ForwardMessage::const_iterator ForwardMessage::cbegin() const noexcept
{
	return this->NodeList_.cbegin();
}
ForwardMessage::iterator ForwardMessage::end() noexcept
{
	return this->NodeList_.end();
}
ForwardMessage::const_iterator ForwardMessage::end() const noexcept
{
	return this->NodeList_.end();
}
ForwardMessage::const_iterator ForwardMessage::cend() const noexcept
{
	return this->NodeList_.cend();
}
ForwardMessage::reverse_iterator ForwardMessage::rbegin() noexcept
{
	return this->NodeList_.rbegin();
}
ForwardMessage::const_reverse_iterator ForwardMessage::crbegin() const noexcept
{
	return this->NodeList_.crbegin();
}
ForwardMessage::reverse_iterator ForwardMessage::rend() noexcept
{
	return this->NodeList_.rend();
}
ForwardMessage::const_reverse_iterator ForwardMessage::crend() const noexcept
{
	return this->NodeList_.crend();
}

} // namespace Mirai