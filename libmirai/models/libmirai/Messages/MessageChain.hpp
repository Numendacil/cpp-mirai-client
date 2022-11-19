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

#ifndef _MIRAI_MESSAGECHAIN_HPP_
#define _MIRAI_MESSAGECHAIN_HPP_

#include <cstddef>
#include <ctime>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <libmirai/Exceptions/Exceptions.hpp>

#include "AppMessage.hpp"
#include "AtAllMessage.hpp"
#include "AtMessage.hpp"
#include "AudioMessage.hpp"
#include "DiceMessage.hpp"
#include "FaceMessage.hpp"
#include "FlashImageMessage.hpp"
#include "ForwardMessage.hpp"
#include "ImageMessage.hpp"
#include "JsonMessage.hpp"
#include "MessageBase.hpp"
#include "MiraiCodeMessage.hpp"
#include "PlainMessage.hpp"
#include "PokeMessage.hpp"
#include "XmlMessage.hpp"

namespace Mirai
{

/**
 * @brief 消息链对象，由一系列消息元素组成
 * 
 * 可以使用类似STL容器的操作来访问各元素（ `std::unique_ptr<MessageBase>` ）
 */
class MessageChain
{
protected:
	using MessageContainer = std::vector<std::unique_ptr<MessageBase>>;
	MessageContainer _message;

	template<typename T> class _has_type_
	{
		using yes_type = char;
		using no_type = long;
		template<typename U> static yes_type test(decltype(&U::_TYPE_));
		template<typename U> static no_type test(...);

	public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
	};

	template<typename Message> constexpr static void _type_check_()
	{
		static_assert(std::is_base_of<MessageBase, Message>::value,
		              "Message must be a derived class of MessageBase"); // NOLINT(*-array-to-pointer-decay)
		static_assert(_has_type_<Message>::value,
		              "Message must contain a static atrribute _TYPE_"); // NOLINT(*-array-to-pointer-decay)
		static_assert(
			std::is_same_v<GetType_t<Message::_TYPE_>, Message>,
			"The resulting type from GetType does not match the original type"); // NOLINT(*-array-to-pointer-decay)
	};

public:
	MessageChain() = default;

	MessageChain(const MessageChain& rhs);
	MessageChain& operator=(const MessageChain& rhs);
	MessageChain(MessageChain&& rhs) noexcept;
	MessageChain& operator=(MessageChain&& rhs) noexcept;
	~MessageChain() = default;

	/**
	 * @brief STL-like interface
	 * 
	 */
	///@{

	using value_type = MessageContainer::value_type;
	using allocator_type = MessageContainer::allocator_type;
	using size_type = MessageContainer::size_type;
	using difference_type = MessageContainer::difference_type;
	using reference = MessageContainer::reference;
	using const_reference = MessageContainer::const_reference;
	using pointer = MessageContainer::pointer;
	using const_pointer = MessageContainer::const_pointer;
	using reverse_iterator = MessageContainer::reverse_iterator;
	using const_reverse_iterator = MessageContainer::const_reverse_iterator;
	using iterator = MessageContainer::iterator;
	using const_iterator = MessageContainer::const_iterator;

	bool empty() const noexcept { return this->_message.empty(); }
	size_type size() const noexcept { return this->_message.size(); }
	void reserve(size_type new_cap) { return this->_message.reserve(new_cap); }
	void shrink_to_fit() noexcept { return this->_message.shrink_to_fit(); }
	size_type max_size() const noexcept { return this->_message.max_size(); }
	size_type capacity() const noexcept { return this->_message.capacity(); }

	void clear() noexcept { return this->_message.clear(); }
	iterator erase(const_iterator pos) { return this->_message.erase(pos); }
	iterator erase(const_iterator first, const_iterator last) { return this->_message.erase(first, last); }
	void pop_back() { return this->_message.pop_back(); }
	void resize(size_type count) { return this->_message.resize(count); }

	iterator begin() noexcept { return this->_message.begin(); }
	const_iterator begin() const noexcept { return this->_message.begin(); }
	const_iterator cbegin() const noexcept { return this->_message.cbegin(); }
	iterator end() noexcept { return this->_message.end(); }
	const_iterator end() const noexcept { return this->_message.end(); }
	const_iterator cend() const noexcept { return this->_message.cend(); }
	reverse_iterator rbegin() noexcept { return this->_message.rbegin(); }
	const_reverse_iterator crbegin() const noexcept { return this->_message.crbegin(); }
	reverse_iterator rend() noexcept { return this->_message.rend(); }
	const_reverse_iterator crend() const noexcept { return this->_message.crend(); }


	const MessageBase& operator[](size_type n) const noexcept { return *this->_message[n]; }
	MessageBase& operator[](size_type n) noexcept { return *this->_message[n]; }

	///@}

	// Element Insertion

	/**
	 * @brief 在消息链结尾添加消息元素
	 * 
	 * 类似 `std::vector::push_back()`。
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性
	 * @tparam MessageType 消息类型
	 * @param m 要添加的消息
	 * @return reference to *this
	 */
	template<typename MessageType> MessageChain& Append(MessageType&& m)
	{
		_type_check_<MessageType>();

		this->_message.push_back(std::make_unique<MessageType>(std::forward<MessageType>(m)));
		return *this;
	}

	/**
	 * @brief 在消息链结尾直接构造消息元素
	 * 
	 * 类似 `std::vector::emplace_back()`。
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性
	 * @tparam MessageType 消息类型
	 * @tparam Args 参数类型
	 * @param args 构造消息元素的参数
	 * @return reference to *this
	 */
	template<typename MessageType, typename... Args> MessageChain& Append(Args&&... args)
	{
		_type_check_<MessageType>();

		this->_message.push_back(std::make_unique<MessageType>(std::forward<Args>(args)...));
		return *this;
	}

	/**
	 * @brief 在消息链结尾直接构造消息元素
	 * 
	 * 类似 `std::vector::emplace_back()`。
	 * @tparam Type `MessageTypes`
	 * @tparam Args 参数类型
	 * @param args 构造消息元素的参数
	 * @return reference to *this
	 */
	template<MessageTypes Type, typename... Args> MessageChain& Append(Args&&... args)
	{
		return this->Append<GetType_t<Type>>(std::forward<Args>(args)...);
	}

	/**
	 * @brief 在消息链中插入元素
	 * 
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性
	 * @tparam MessageType 消息类型
	 * @param pos 插入位置
	 * @param m 要添加的消息
	 * @return 插入后新元素所在的位置
	 */
	template<typename MessageType> iterator Insert(const_iterator pos, MessageType&& m)
	{
		_type_check_<MessageType>();
		return this->_message.insert(pos, std::make_unique<MessageType>(std::forward<MessageType>(m)));
	}

	/**
	 * @brief 在消息链中插入元素
	 * 
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性
	 * @tparam MessageType 消息类型
	 * @tparam Args 参数类型
	 * @param pos 插入位置
	 * @param args 构造对应消息的参数
	 * @return 插入后新元素所在的位置 
	 */
	template<typename MessageType, typename... Args> iterator Insert(const_iterator pos, Args&&... args)
	{
		_type_check_<MessageType>();
		return this->_message.insert(pos, std::make_unique<MessageType>(std::forward<Args>(args)...));
	}

	/**
	 * @brief 在消息链中插入元素
	 * 
	 * @tparam Type `MessageTypes`
	 * @tparam Args 参数类型
	 * @param pos 插入位置
	 * @param args 构造对应消息的参数
	 * @return 插入后新元素所在的位置 
	 */
	template<MessageTypes Type, typename... Args> iterator Insert(const_iterator pos, Args&&... args)
	{
		return this->Insert<GetType_t<Type>>(pos, std::forward<Args>(args)...);
	}

	/// 拼接两个消息链
	MessageChain& operator+=(const MessageChain& rhs);
	/// 添加消息元素到消息链结尾
	MessageChain& operator+=(const MessageBase& rhs);
	/// 拼接两个消息链
	friend MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs);
	/// 添加消息元素到消息链结尾
	friend MessageChain operator+(const MessageChain& lhs, const MessageBase& rhs);
	/// 添加消息元素到消息链结尾
	friend MessageChain operator+(const MessageBase& lhs, const MessageChain& rhs);

	// Element Access

	/**
	 * @brief 获取指定下标出的消息
	 * 
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性。
	 * 若消息类型与实际类型不匹配会抛出 `TypeDismatch` 异常
	 * @tparam MessageType 消息类型
	 * @param i 下标
	 * @return 该消息元素的引用
	 */
	template<typename MessageType> MessageType& GetAt(size_type i) const
	{
		_type_check_<MessageType>();

		const auto& m = this->_message.at(i);
		if (!m) throw std::runtime_error("Unexpected null pointer in MessageChain::at");
		if (m->GetType() != MessageType::_TYPE_)
			throw TypeDismatch(to_string(m->GetType()), to_string(MessageType::_TYPE_));
		return *static_cast<MessageType*>(m.get());
	}

	/**
	 * @brief 获取指定下标出的消息
	 * 
	 * 若消息类型与实际类型不匹配会抛出 `TypeDismatch` 异常
	 * @tparam Type `MessageTypes`
	 * @param i 下标
	 * @return 该消息元素的引用
	 */
	template<MessageTypes Type> GetType_t<Type>& GetAt(size_type i) const { return this->GetAt<GetType_t<Type>>(i); }

	/**
	 * @brief 获取指定下标出的消息
	 * 
	 * 约等于 `MessageChain::GetAt<MessageBase>(i)`
	 * @param i 下标
	 * @return 该消息元素的引用，类型为 `MessageBase&`
	 */
	MessageBase& GetAt(size_type i) const
	{
		const auto& m = this->_message.at(i);
		if (!m) throw std::runtime_error("Unexpected null pointer in MessageChain::GetAt");
		return *m;
	}

	/**
	 * @brief 获取所有指定类型的消息元素
	 * 
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性。
	 * 该方法将拷贝消息元素
	 * @tparam MessageType 消息类型
	 * @return `std::vector<MessageType>` 
	 */
	template<typename MessageType> std::vector<MessageType> GetAll() const
	{
		_type_check_<MessageType>();

		std::vector<MessageType> v;
		for (const auto& p : this->_message)
		{
			if (p->GetType() == MessageType::_TYPE_) v.emplace_back(*static_cast<MessageType*>(p.get()));
		}
		return v;
	}

	/**
	 * @brief 获取所有指定类型的消息元素指针
	 * 
	 * 模版参数 `MessageType` 必须为 `MessageBase` 的派生类，且定义了 `MessageType::_TYPE_` 属性。
	 * @tparam MessageType 消息类型
	 * @return `std::vector<MessageType*>` 
	 */
	template<typename MessageType> std::vector<MessageType*> GetAllPtr() const
	{
		_type_check_<MessageType>();

		std::vector<MessageType*> v;
		for (const auto& p : this->_message)
		{
			if (p->GetType() == MessageType::_TYPE_) v.emplace_back(static_cast<MessageType*>(p.get()));
		}
		return v;
	}

	/**
	 * @brief 消息来源信息
	 * 
	 * 一般只存在于直接接收到的消息链中，转发消息与引用消息中的消息链不会含有该信息
	 */
	struct SourceInfo
	{
		/// 消息id
		MessageId_t id = 0;
		/// 消息时间戳
		std::time_t time = 0;
	};

	/**
	 * @brief 获取消息链中的来源信息
	 * 
	 * @return 来源信息，若无则返回 `std::nullopt` 
	 */
	std::optional<SourceInfo> GetSourceInfo() const;

	// Serialization

	/**
	 * @brief 去除消息链中的无效消息
	 * 
	 * 无效消息指 `MessageBase::isValid() = false` 的消息
	 */
	void RemoveInvalid();

	/**
	 * @brief 检查消息链是否有效
	 * 
	 * 要求所有消息元素均有效，且消息不为空（存在至少一条可以发送的消息元素）
	 * @return `bool`
	 */
	bool isValid() const;

	// Helper functions

	/// 在消息链结尾构造 `AppMessage`，返回自身的引用
	template<typename... Args> MessageChain& App(Args&&... args)
	{
		return this->Append<AppMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AtAllMessage`，返回自身的引用
	template<typename... Args> MessageChain& AtAll(Args&&... args)
	{
		return this->Append<AtAllMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AtMessage`，返回自身的引用
	template<typename... Args> MessageChain& At(Args&&... args)
	{
		return this->Append<AtMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AudioMessage`，返回自身的引用
	template<typename... Args> MessageChain& Audio(Args&&... args)
	{
		return this->Append<AudioMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `DiceMessage`，返回自身的引用
	template<typename... Args> MessageChain& Dice(Args&&... args)
	{
		return this->Append<DiceMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `FaceMessage`，返回自身的引用
	template<typename... Args> MessageChain& Face(Args&&... args)
	{
		return this->Append<FaceMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `FlashImageMessage`，返回自身的引用
	template<typename... Args> MessageChain& Flash(Args&&... args)
	{
		return this->Append<FlashImageMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `ImageMessage`，返回自身的引用
	template<typename... Args> MessageChain& Image(Args&&... args)
	{
		return this->Append<ImageMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `JsonMessage`，返回自身的引用
	template<typename... Args> MessageChain& Json(Args&&... args)
	{
		return this->Append<JsonMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `MiraiCodeMessage`，返回自身的引用
	template<typename... Args> MessageChain& MiraiCode(Args&&... args)
	{
		return this->Append<MiraiCodeMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `PlainMessage`，返回自身的引用
	template<typename... Args> MessageChain& Plain(Args&&... args)
	{
		return this->Append<PlainMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `PokeMessage`，返回自身的引用
	template<typename... Args> MessageChain& Poke(Args&&... args)
	{
		return this->Append<PokeMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `XmlMessage`，返回自身的引用
	template<typename... Args> MessageChain& Xml(Args&&... args)
	{
		return this->Append<XmlMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾插入 `ForwardMessage`，返回自身的引用
	template<typename... Args> MessageChain& Forward(Args&&... args)
	{
		return this->Append<ForwardMessage>(std::forward<Args>(args)...);
	}


	struct Serializable;
	;
};

} // namespace Mirai

#endif