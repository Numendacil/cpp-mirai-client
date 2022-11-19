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

ForwardMessage::ForwardMessage() : MessageBase(_TYPE_) {}

std::unique_ptr<MessageBase> ForwardMessage::CloneUnique() const
{
	return std::make_unique<ForwardMessage>(*this);
}

bool ForwardMessage::isValid() const
{
	for (const auto& n : this->_NodeList)
		if (!n.isValid()) return false;
	return true;
}

bool ForwardMessage::empty() const noexcept
{
	return this->_NodeList.empty();
}
ForwardMessage::size_type ForwardMessage::size() const noexcept
{
	return this->_NodeList.size();
}
void ForwardMessage::reserve(size_type new_cap)
{
	return this->_NodeList.reserve(new_cap);
}
void ForwardMessage::shrink_to_fit() noexcept
{
	return this->_NodeList.shrink_to_fit();
}
ForwardMessage::size_type ForwardMessage::max_size() const noexcept
{
	return this->_NodeList.max_size();
}
ForwardMessage::size_type ForwardMessage::capacity() const noexcept
{
	return this->_NodeList.capacity();
}

ForwardMessage::const_reference ForwardMessage::at(size_type n) const
{
	return this->_NodeList.at(n);
}
ForwardMessage::reference ForwardMessage::at(size_type n)
{
	return this->_NodeList.at(n);
}
ForwardMessage::const_reference ForwardMessage::operator[](size_type n) const noexcept
{
	return this->_NodeList[n];
}
ForwardMessage::reference ForwardMessage::operator[](size_type n) noexcept
{
	return this->_NodeList[n];
}

void ForwardMessage::clear() noexcept
{
	return this->_NodeList.clear();
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, const_reference value)
{
	return this->_NodeList.insert(pos, value);
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, value_type&& value)
{
	return this->_NodeList.insert(pos, std::move(value));
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, size_type count, const_reference value)
{
	return this->_NodeList.insert(pos, count, value);
}
ForwardMessage::iterator ForwardMessage::insert(const_iterator pos, std::initializer_list<value_type> ilist)
{
	return this->_NodeList.insert(pos, ilist);
}
ForwardMessage::iterator ForwardMessage::erase(const_iterator pos)
{
	return this->_NodeList.erase(pos);
}
ForwardMessage::iterator ForwardMessage::erase(const_iterator first, const_iterator last)
{
	return this->_NodeList.erase(first, last);
}
void ForwardMessage::push_back(const_reference node)
{
	return this->_NodeList.push_back(node);
}
void ForwardMessage::push_back(value_type&& node)
{
	return this->_NodeList.push_back(std::move(node));
}
void ForwardMessage::pop_back()
{
	return this->_NodeList.pop_back();
}
void ForwardMessage::resize(size_type count)
{
	return this->_NodeList.resize(count);
}
void ForwardMessage::resize(size_type count, const value_type& value)
{
	return this->_NodeList.resize(count, value);
}

ForwardMessage::iterator ForwardMessage::begin() noexcept
{
	return this->_NodeList.begin();
}
ForwardMessage::const_iterator ForwardMessage::begin() const noexcept
{
	return this->_NodeList.begin();
}
ForwardMessage::const_iterator ForwardMessage::cbegin() const noexcept
{
	return this->_NodeList.cbegin();
}
ForwardMessage::iterator ForwardMessage::end() noexcept
{
	return this->_NodeList.end();
}
ForwardMessage::const_iterator ForwardMessage::end() const noexcept
{
	return this->_NodeList.end();
}
ForwardMessage::const_iterator ForwardMessage::cend() const noexcept
{
	return this->_NodeList.cend();
}
ForwardMessage::reverse_iterator ForwardMessage::rbegin() noexcept
{
	return this->_NodeList.rbegin();
}
ForwardMessage::const_reverse_iterator ForwardMessage::crbegin() const noexcept
{
	return this->_NodeList.crbegin();
}
ForwardMessage::reverse_iterator ForwardMessage::rend() noexcept
{
	return this->_NodeList.rend();
}
ForwardMessage::const_reverse_iterator ForwardMessage::crend() const noexcept
{
	return this->_NodeList.crend();
}

} // namespace Mirai