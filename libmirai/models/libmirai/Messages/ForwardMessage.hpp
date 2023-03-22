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

#ifndef MIRAI_FORWARD_MESSAGE_HPP_
#define MIRAI_FORWARD_MESSAGE_HPP_

#include <ctime>
#include <optional>
#include <utility>
#include <vector>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 合并转发消息
 *
 * 可以使用类似于STL容器的方式访问各元素
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `ForwardMessage::NodeList_` | `{}`
 * `ForwardMessage::title_` | `std::nullopt`
 * `ForwardMessage::brief_` | `std::nullopt`
 * `ForwardMessage::source_` | `std::nullopt`
 * `ForwardMessage::preview_` | `std::nullopt`
 * `ForwardMessage::summary_` | `std::nullopt`
 */
class ForwardMessage final: public IMessageImpl<ForwardMessage>
{
	friend IMessageImpl<ForwardMessage>;

public:
	class Node;

protected:
	using NodeList = std::vector<Node>;
	NodeList NodeList_;

	std::optional<std::string> title_;
	std::optional<std::string> brief_;
	std::optional<std::string> source_;
	std::optional<std::vector<std::string>> preview_;
	std::optional<std::string> summary_;

	static constexpr MessageTypes TYPE_ = MessageTypes::FORWARD;
	static constexpr bool SUPPORT_SEND_ = true;

	bool isValid_() const final;

public:
	ForwardMessage();
	ForwardMessage(const ForwardMessage&);
	ForwardMessage& operator= (const ForwardMessage&);
	ForwardMessage(ForwardMessage&&);
	ForwardMessage& operator= (ForwardMessage&&);
	~ForwardMessage() final;

	/// 获取合并转发消息的标题（"xxx的聊天记录"），为空代表默认值
	std::optional<std::string> GetDisplayTitle() const
	{
		return this->title_;
	}

	/// 获取合并转发消息的简略信息（"[聊天记录]"），为空代表默认值
	std::optional<std::string> GetDisplayBrief() const
	{
		return this->brief_;
	}

	/// 获取合并转发消息的来源显示（"聊天记录"），为空代表默认值
	std::optional<std::string> GetDisplaySource() const
	{
		return this->source_;
	}

	/// 获取合并转发消息的内容预览，为空代表默认值
	std::optional<std::vector<std::string>> GetDisplayPreview() const
	{
		return this->preview_;
	}

	/// 获取合并转发消息的内容总结（"查看x条转发消息"），为空代表默认值
	std::optional<std::string> GetDisplaySummary() const
	{
		return this->summary_;
	}


	/// 设置合并转发消息的标题（"xxx的聊天记录"）
	void SetDisplayTitle(std::string title)
	{
		this->title_ = std::move(title);
	}

	/// 设置合并转发消息的简略信息（"[聊天记录]"）
	void SetDisplayBrief(std::string brief)
	{
		this->brief_ = std::move(brief);
	}

	/// 设置合并转发消息的来源显示（"聊天记录"）
	void SetDisplaySource(std::string source)
	{
		this->source_ = std::move(source);
	}

	/// 设置合并转发消息的内容预览
	void SetDisplayPreview(std::vector<std::string> preview)
	{
		this->preview_ = std::move(preview);
	}

	/// 设置合并转发消息的内容总结（"查看x条转发消息"）
	void SetDisplaySummary(std::string summary)
	{
		this->summary_ = std::move(summary);
	}


	/**
	 * @brief STL-like interface
	 * 
	 */
	///@{

	// I hate this

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

	bool empty() const noexcept;
	size_type size() const noexcept;
	void reserve(size_type new_cap);
	void shrink_to_fit() noexcept;
	size_type max_size() const noexcept;
	size_type capacity() const noexcept;

	const_reference at(size_type n) const;
	reference at(size_type n);
	const_reference operator[](size_type n) const noexcept;
	reference operator[](size_type n) noexcept;
	const_reference back() const;
	reference back();
	const_reference front() const;
	reference front();

	void clear() noexcept;
	iterator insert(const_iterator pos, const_reference value);
	iterator insert(const_iterator pos, value_type&& value);
	iterator insert(const_iterator pos, size_type count, const_reference value);
	template<class InputIt> iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		return this->NodeList_.insert(pos, std::forward<InputIt>(first), std::forward<InputIt>(last));
	}
	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);
	template<class... Args > iterator emplace(const_iterator pos, Args&&... args)
	{
		return this->NodeList_.emplace(pos, std::forward<Args>(args)...);
	}
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	void push_back(const_reference node);
	void push_back(value_type&& node);
	template< class... Args > reference emplace_back(Args&&... args)
	{
		return this->NodeList_.emplace_back(std::forward<Args>(args)...);
	}
	void pop_back();
	void resize(size_type count);
	void resize(size_type count, const value_type& value);

	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;
	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;
	reverse_iterator rbegin() noexcept;
	const_reverse_iterator crbegin() const noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator crend() const noexcept;

	///@}

	struct Serializable;
};

using ForwardNode = ForwardMessage::Node;

template<> struct GetType<ForwardMessage::GetType()>
{
	using type = ForwardMessage;
};

} // namespace Mirai


#endif