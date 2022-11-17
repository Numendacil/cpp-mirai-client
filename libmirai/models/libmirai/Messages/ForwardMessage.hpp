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

#ifndef _MIRAI_FORWARD_MESSAGE_HPP_
#define _MIRAI_FORWARD_MESSAGE_HPP_

#include <ctime>
#include <optional>
#include <utility>
#include <vector>


#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 合并转发消息
 *
 * 可以使用类似于STL容器的方式访问各元素
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `ForwardMessage::_NodeList` | `{}`
 */
class ForwardMessage : public MessageBase
{
public:
	class Node;

protected:
	using NodeList = std::vector<Node>;
	NodeList _NodeList;

	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::FORWARD;

	ForwardMessage() : MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<ForwardMessage>(*this); }

	bool isValid() const final;

	/**
	 * @brief STL-like interface
	 * 
	 */
	///@{
	using value_type = NodeList::value_type;
	using allocator_type = NodeList::allocator_type;
	using size_type = NodeList::size_type;
	using difference_type = NodeList::difference_type;
	using reference = NodeList::reference;
	using const_reference = NodeList::const_reference;
	using pointer = NodeList::pointer;
	using const_pointer = NodeList::const_pointer;
	using reverse_iterator = NodeList::reverse_iterator;
	using const_reverse_iterator = NodeList::const_reverse_iterator;
	using iterator = NodeList::iterator;
	using const_iterator = NodeList::const_iterator;

	bool empty() const noexcept { return this->_NodeList.empty(); }
	size_type size() const noexcept { return this->_NodeList.size(); }
	void reserve(size_type new_cap) { return this->_NodeList.reserve(new_cap); }
	void shrink_to_fit() noexcept { return this->_NodeList.shrink_to_fit(); }
	size_type max_size() const noexcept { return this->_NodeList.max_size(); }
	size_type capacity() const noexcept { return this->_NodeList.capacity(); }

	const_reference at(size_type n) const { return this->_NodeList.at(n); }
	reference at(size_type n) { return this->_NodeList.at(n); }
	const_reference operator[](size_type n) const noexcept { return this->_NodeList[n]; }
	reference operator[](size_type n) noexcept { return this->_NodeList[n]; }

	void clear() noexcept { return this->_NodeList.clear(); }
	iterator insert(const_iterator pos, const_reference value) { return this->_NodeList.insert(pos, value); }
	iterator insert(const_iterator pos, value_type&& value) { return this->_NodeList.insert(pos, std::move(value)); }
	iterator insert(const_iterator pos, size_type count, const_reference value)
	{
		return this->_NodeList.insert(pos, count, value);
	}
	template<class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		return this->_NodeList.insert(pos, std::forward<InputIt>(first), std::forward<InputIt>(last));
	}
	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
	{
		return this->_NodeList.insert(pos, ilist);
	}
	template<class... Args > iterator emplace(const_iterator pos, Args&&... args)
	{
		return this->emplace(pos, std::forward<Args>(args)...);
	}
	iterator erase(const_iterator pos) { return this->_NodeList.erase(pos); }
	iterator erase(const_iterator first, const_iterator last) { return this->_NodeList.erase(first, last); }
	void push_back(const_reference node) { return this->_NodeList.push_back(node); }
	void push_back(value_type&& node) { return this->_NodeList.push_back(std::move(node)); }
	template< class... Args > reference emplace_back(Args&&... args)
	{
		return this->_NodeList.emplace_back(std::forward<Args>(args)...);
	}
	void pop_back() { return this->_NodeList.pop_back(); }
	void resize(size_type count) { return this->_NodeList.resize(count); }
	void resize(size_type count, const value_type& value) { return this->_NodeList.resize(count, value); }

	iterator begin() noexcept { return this->_NodeList.begin(); }
	const_iterator begin() const noexcept { return this->_NodeList.begin(); }
	const_iterator cbegin() const noexcept { return this->_NodeList.cbegin(); }
	iterator end() noexcept { return this->_NodeList.end(); }
	const_iterator end() const noexcept { return this->_NodeList.end(); }
	const_iterator cend() const noexcept { return this->_NodeList.cend(); }
	reverse_iterator rbegin() noexcept { return this->_NodeList.rbegin(); }
	const_reverse_iterator crbegin() const noexcept { return this->_NodeList.crbegin(); }
	reverse_iterator rend() noexcept { return this->_NodeList.rend(); }
	const_reverse_iterator crend() const noexcept { return this->_NodeList.crend(); }
	///@}
};

template <>
struct GetType<ForwardMessage::_TYPE_>
{
	using type = ForwardMessage;
};

} // namespace Mirai


#endif