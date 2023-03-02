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

#include <cassert>
#include <cstddef>
#include <ctime>
#include <functional>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/MessageTypes.hpp>

#include "AppMessage.hpp"
#include "AtAllMessage.hpp"
#include "AtMessage.hpp"
#include "AudioMessage.hpp"
#include "DiceMessage.hpp"
#include "FaceMessage.hpp"
#include "FileMessage.hpp"
#include "ForwardMessage.hpp"
#include "IMessage.hpp"
#include "ImageMessage.hpp"
#include "JsonMessage.hpp"
#include "MiraiCodeMessage.hpp"
#include "MusicShareMessage.hpp"
#include "PlainMessage.hpp"
#include "PokeMessage.hpp"
#include "QuoteMessage.hpp"
#include "SourceMessage.hpp"
#include "XmlMessage.hpp"

namespace Mirai
{

/**
 * @brief 消息链对象，由一系列消息元素组成
 * 
 * 可以使用类似STL容器的操作来访问各元素（ `std::vector<std::unique_ptr<MessageElement>>` ）
 */
class MessageChain
{
private:
	// type traits
	struct traits
	{
		template<typename... Types> struct _type_list
		{
		};

		template<typename T, typename TypeList> struct _is_message_type;

		template<typename T, size_t... I>
		struct _is_message_type<T, std::index_sequence<I...>> : public std::disjunction<std::is_same<T, GetType_t<MessageTypesList[I]>>...>
		{
		};

		template<typename T>
		using is_message_type = _is_message_type<T, std::make_index_sequence<MessageTypesList.size()>>;
	};

	template<typename MessageType> constexpr static void _type_check_()
	{
		static_assert(std::is_base_of_v<IMessage, MessageType>,
		              "MessageType is not derived from IMessage"); // NOLINT(*-array-to-pointer-decay)
		static_assert(traits::is_message_type<MessageType>::value,
		              "Unsupported messsage type"); // NOLINT(*-array-to-pointer-decay)
	};

public:
	/**
	* @brief 消息元素对象
	* 
	* 对不同消息类型的统一封装，储存着某一特定消息类型的对象
	*/
	class MessageElement
	{
	private:
		std::unique_ptr<IMessage> _msg;
		MessageTypes _type = MessageTypes::ENUM_END; // For quick reference

		template<
			std::size_t... I, typename F,
			typename Ret = std::enable_if_t<(sizeof...(I) > 0), std::result_of_t<F(GetType_t<MessageTypesList[0]>&)>> >
		auto _visit(std::index_sequence<I...>, F&& f) -> std::enable_if_t<
			std::conjunction_v<std::is_same<Ret, std::result_of_t<F(GetType_t<MessageTypesList[I]>&)>>...>, Ret >
		{
			using FP = Ret (*)(F&&, MessageElement*);
			static constexpr std::array<FP, sizeof...(I)> func = {
				[](F&& f, MessageElement* p) { return f(static_cast<GetType_t<MessageTypesList[I]>&>(*p->_msg)); }...};

			if (this->_type == MessageTypes::ENUM_END)
				throw std::runtime_error("Cannot call visit() on an empty MessageElement");
			return func[static_cast<size_t>(this->_type)](std::forward<F>(f), this);
		}

		template< std::size_t... I, typename F,
		          typename Ret =
		              std::enable_if_t<(sizeof...(I) > 0), std::result_of_t<F(const GetType_t<MessageTypesList[0]>&)>> >
		auto _cvisit(std::index_sequence<I...>, F&& f) const -> std::enable_if_t<
			std::conjunction_v<std::is_same<Ret, std::result_of_t<F(const GetType_t<MessageTypesList[I]>&)>>...>, Ret >
		{
			using FP = Ret (*)(F&&, const MessageElement*);
			static constexpr std::array<FP, sizeof...(I)> func = {[](F&& f, const MessageElement* p) {
				return f(static_cast<const GetType_t<MessageTypesList[I]>&>(*p->_msg));
			}...};

			if (this->_type == MessageTypes::ENUM_END)
				throw std::runtime_error("Cannot call visit() on an empty MessageElement");
			return func[static_cast<size_t>(this->_type)](std::forward<F>(f), this);
		}

		// WARNING: Use this only if you know what you are doing
		explicit MessageElement(std::unique_ptr<IMessage>&& msg, MessageTypes type) : _msg(std::move(msg)), _type(type)
		{
			// NOLINTNEXTLINE(*-array-to-pointer-decay)
			assert(this->_type == ((this->_msg) ? this->_msg->type() : MessageTypes::ENUM_END));
		}

	public:
		/**
		* @brief Constructors, conversion operators and destructor
		* 
		*/
		///@{

		MessageElement() = default;

		explicit MessageElement(std::unique_ptr<IMessage>&& msg) : _msg(std::move(msg))
		{
			if (_msg) this->_type = this->_msg->type();
		}

		MessageElement& operator=(std::unique_ptr<IMessage>&& msg)
		{
			this->_msg = std::move(msg);
			if (_msg) 
				this->_type = this->_msg->type();
			return *this;
		}

		MessageElement(const MessageElement& rhs) : _msg(), _type(rhs._type)
		{
			if (rhs._msg) this->_msg = rhs._msg->clone();
		}

		MessageElement& operator=(const MessageElement& rhs)
		{
			if (&rhs != this)
			{
				this->_type = rhs._type;
				if (rhs._msg) this->_msg = rhs._msg->clone();
			}
			return *this;
		}

		MessageElement(MessageElement&&) noexcept = default;
		MessageElement& operator=(MessageElement&&) noexcept = default;

		~MessageElement() = default;

		explicit operator bool() const { return (bool)this->_msg; }

		template<typename MessageType, typename... Args, typename Type = std::decay_t<MessageType>,
		         typename std::enable_if_t<traits::is_message_type<Type>::value, int> = 0>
		explicit MessageElement(std::in_place_type_t<MessageType>, Args&&... args)
			: _msg(std::make_unique<Type>(std::forward<Args>(args)...)), _type(Type::GetType())
		{
		}

		template<typename MessageType, typename Type = std::decay_t<MessageType>,
		         typename std::enable_if_t<traits::is_message_type<Type>::value, int> = 0>
		explicit MessageElement(MessageType&& m)
			: _msg(std::make_unique<Type>(std::forward<MessageType>(m))), _type(Type::GetType())
		{
		}

		template<typename MessageType, typename Type = std::decay_t<MessageType>,
		         typename std::enable_if_t<traits::is_message_type<Type>::value, int> = 0>
		MessageElement& operator=(MessageType&& m)
		{
			if (this->_msg && this->_type == Type::GetType())
			{
				static_cast<Type&>(*this->_msg) = std::forward<MessageType>(m);
			}
			else
			{
				this->_msg = std::make_unique<Type>(std::forward<MessageType>(m));
				this->_type = Type::GetType();
			}
			return *this;
		}

		// This function does not change the ownership of the input pointer
		explicit MessageElement(IMessage* m) : _msg(m->clone()), _type(m->type()) {}

		///@}

		/**
		 * @brief 获取所含消息的类型
		 * 
		 * @return `MessageTypes` 
		 */
		MessageTypes type() const { return this->_type; }

		/**
		 * @brief 检查消息是否可以用于发送
		 * 
		 * 部分消息仅支持接收
		 * @return `bool`
		 */
		bool allowSend() const
		{
			if (!this->_msg) throw std::runtime_error("MessageElement is empty");
			return this->_msg->allowSend();
		}

		/**
		 * @brief 检查消息是否有效
		 * 
		 * 发送无效消息( `valid() = false` )会导致mirai-api-http返回400或500错误，并抛出异常。
		 * 检测的内容靠测试经验和mirai-api-http源码确定， `valid() = true` 不保证一定能发送成功。
		 * @return `bool`
		 */
		bool valid() const
		{
			if (!this->_msg) throw std::runtime_error("MessageElement is empty");
			return this->_msg->valid();
		}

		/**
		 * @brief 转换为某一特定类型的消息，类型错误时将抛出异常
		 * 
		 * @tparam MessageType 消息类型
		 * @return MessageType& 对象的引用
		 */
		///@{
		template<typename MessageType> MessageType& as()
		{
			_type_check_<MessageType>();

			if (!this->_msg) throw std::runtime_error("MessageElement is empty");
			if (this->_type != MessageType::GetType())
				throw TypeDismatch(to_string(this->_type), to_string(MessageType::GetType()));
			return static_cast<MessageType&>(*this->_msg);
		}

		template<typename MessageType> const MessageType& as() const
		{
			_type_check_<std::decay_t<MessageType>>();

			if (!this->_msg) throw std::runtime_error("MessageElement is empty");
			if (this->_type != MessageType::GetType())
				throw TypeDismatch(to_string(this->_type), to_string(MessageType::GetType()));
			return static_cast<const MessageType&>(*this->_msg);
		}
		///@}

		/**
		 * @brief 对此消息元素应用visitor，类似 `std::visit`
		 * 
		 * @tparam Callable 
		 * @param f 可调用对象
		 * @return auto 所有输入的返回类型必须一致
		 */
		///@{
		template<typename Callable> auto visit(Callable&& f)
		{
			return this->_visit(std::make_index_sequence<MessageTypesList.size()>{}, std::forward<Callable>(f));
		}

		template<typename Callable> auto visit(Callable&& f) const
		{
			return this->_cvisit(std::make_index_sequence<MessageTypesList.size()>{}, std::forward<Callable>(f));
		}
		///@}

		struct Serializable;
	};


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

protected:
	using MessageContainer = std::vector<MessageElement>;
	MessageContainer _message;

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

	// I hate this

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

	const_reference operator[](size_type n) const noexcept { return this->_message[n]; }
	reference operator[](size_type n) noexcept { return this->_message[n]; }
	const_reference at(size_type n) const { return this->_message.at(n); }
	reference at(size_type n) { return this->_message.at(n); }
	const_reference back() const { return this->_message.back(); }
	reference back() { return this->_message.back(); }
	const_reference front() const { return this->_message.front(); }
	reference front() { return this->_message.front(); }

	///@}

	// Element Insertion

	/**
	 * @brief 在消息链结尾添加消息元素
	 * 
	 * 类似 `std::vector::push_back()`
	 * @tparam MessageType 消息类型
	 * @param m 要添加的消息
	 * @return reference to *this
	 */

	template<typename MessageType> MessageChain& push_back(MessageType&& m)
	{
		_type_check_<std::decay_t<MessageType>>();

		this->_message.push_back(MessageElement(std::forward<MessageType>(m)));
		return *this;
	}

	/**
	 * @brief 在消息链结尾直接构造消息元素
	 * 
	 * 类似 `std::vector::emplace_back()`。
	 * @tparam MessageType 消息类型
	 * @tparam Args 参数类型
	 * @param args 构造消息元素的参数
	 * @return reference to *this
	 */
	template<typename MessageType, typename... Args> MessageChain& emplace_back(Args&&... args)
	{
		_type_check_<MessageType>();

		this->_message.emplace_back(std::in_place_type_t<MessageType>{}, std::forward<Args>(args)...);
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
	template<MessageTypes Type, typename... Args> MessageChain& emplace_back(Args&&... args)
	{
		return this->emplace_back<GetType_t<Type>>(std::forward<Args>(args)...);
	}

	/**
	 * @brief 在消息链中插入元素
	 * 
	 * 类似 `std::vector::insert()`。
	 * @tparam MessageType 消息类型
	 * @param pos 插入位置
	 * @param m 要添加的消息
	 * @return 插入后新元素所在的位置
	 */
	template<typename MessageType> iterator insert(const_iterator pos, MessageType&& m)
	{
		_type_check_<std::decay_t<MessageType>>();

		return this->_message.insert(pos, MessageElement(std::forward<MessageType>(m)));
	}

	/**
	 * @brief 在消息链中插入元素
	 * 
	 * 类似 `std::vector::emplace()`。
	 * @tparam MessageType 消息类型
	 * @tparam Args 参数类型
	 * @param pos 插入位置
	 * @param args 构造对应消息的参数
	 * @return 插入后新元素所在的位置 
	 */
	template<typename MessageType, typename... Args> iterator emplace(const_iterator pos, Args&&... args)
	{
		_type_check_<MessageType>();
		return this->_message.emplace(pos, std::in_place_type_t<MessageType>{}, std::forward<Args>(args)...);
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
	template<MessageTypes Type, typename... Args> iterator emplace(const_iterator pos, Args&&... args)
	{
		return this->emplace<GetType_t<Type>>(pos, std::forward<Args>(args)...);
	}

	/// 拼接两个消息链
	///@{
	MessageChain& operator+=(const MessageChain& rhs);
	MessageChain& operator+=(MessageChain&& rhs);
	///@}

	/// 添加消息元素到消息链结尾
	///@{
	MessageChain& operator+=(const MessageElement& m);
	MessageChain& operator+=(MessageElement&& m);

	template<typename MessageType>
	auto operator+=(MessageType&& m)
		-> std::enable_if_t<traits::is_message_type<std::decay_t<MessageType>>::value, MessageChain&>
	{
		return *this += MessageElement(std::forward<MessageType>(m));
	}
	///@}

	/// 拼接两个消息链
	friend MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs);

	/// 添加消息元素到消息链结尾
	///@{
	friend MessageChain operator+(const MessageChain& lhs, const MessageElement& m);
	friend MessageChain operator+(const MessageElement& m, const MessageChain& rhs) { return rhs + m; }

	template<typename MessageType>
	friend auto operator+(const MessageChain& lhs, MessageType&& m)
		-> std::enable_if_t<traits::is_message_type<std::decay_t<MessageType>>::value, MessageChain>
	{
		return lhs + MessageElement(std::forward<MessageType>(m));
	}

	template<typename MessageType>
	friend auto operator+(MessageType&& m, const MessageChain& rhs)
		-> std::enable_if_t<traits::is_message_type<std::decay_t<MessageType>>::value, MessageChain>
	{
		return rhs + MessageElement(std::forward<MessageType>(m));
	}
	///@}

	// Element Access

	/**
	 * @brief 获取指定下标处的消息
	 * 
	 * 若消息类型与实际类型不匹配会抛出 `TypeDismatch` 异常
	 * @tparam MessageType 消息类型
	 * @param i 下标
	 * @return 该消息元素的引用
	 */
	///@{

	template<typename MessageType> MessageType& GetAt(size_type i) { return this->_message.at(i).as<MessageType>(); }

	template<typename MessageType> const MessageType& GetAt(size_type i) const
	{
		return this->_message.at(i).as<MessageType>();
	}

	///@}

	/**
	 * @brief 获取指定下标处的消息
	 * 
	 * 若消息类型与实际类型不匹配会抛出 `TypeDismatch` 异常
	 * @tparam Type `MessageTypes`
	 * @param i 下标
	 * @return 该消息元素的引用
	 */
	///@{

	template<MessageTypes Type> GetType_t<Type>& GetAt(size_type i) { return this->GetAt<GetType_t<Type>>(i); }

	template<MessageTypes Type> const GetType_t<Type>& GetAt(size_type i) const
	{
		return this->GetAt<GetType_t<Type>>(i);
	}

	///@}

	/**
	 * @brief 获取指定下标处的消息类型
	 * 
	 * @param i 下标
	 * @return Type `MessageTypes`
	 */
	MessageTypes GetType(size_type i) const { return this->_message.at(i).type(); }

	/**
	 * @brief 获取所有指定类型的消息元素
	 * 
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
			if (p.type() == MessageType::GetType()) v.push_back(p.as<MessageType>());
		}
		return v;
	}

	/**
	 * @brief 获取所有指定类型的消息元素
	 * 
	 * 该方法将拷贝消息元素
	 * @tparam Type 消息类型
	 * @return `std::vector<GetType_t<Type>>` 
	 */
	template<MessageTypes Type> auto GetAll() const { return this->GetAll<GetType_t<Type>>(); }

	/**
	 * @brief 获取所有指定类型的消息元素的引用
	 * 
	 * @tparam MessageType 消息类型
	 * @return `std::vector<std::reference_wrapper<MessageType>>` 
	 */
	///@{

	template<typename MessageType> std::vector<std::reference_wrapper<MessageType>> GetAllRef()
	{
		_type_check_<MessageType>();

		std::vector<std::reference_wrapper<MessageType>> v;
		for (auto& p : this->_message)
		{
			if (p.type() == MessageType::GetType()) v.push_back(std::ref(p.as<MessageType>()));
		}
		return v;
	}

	template<typename MessageType> std::vector<std::reference_wrapper<const MessageType>> GetAllRef() const
	{
		_type_check_<MessageType>();

		std::vector<std::reference_wrapper<const MessageType>> v;
		for (const auto& p : this->_message)
		{
			if (p.type() == MessageType::GetType()) v.push_back(std::cref(p.as<MessageType>()));
		}
		return v;
	}

	///@}

	/**
	 * @brief 获取所有指定类型的消息元素的引用
	 * 
	 * @tparam Type 消息类型
	 * @return `std::vector<std::reference_wrapper<GetType_t<Type>>>` 
	 */
	///@{

	template<MessageTypes Type> auto GetAllRef() { return this->GetAllRef<GetType_t<Type>>(); }

	template<MessageTypes Type> auto GetAllRef() const { return this->GetAllRef<GetType_t<Type>>(); }

	///@}


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
	 * 无效消息指 `IMessage::valid() = false` 的消息
	 */
	void RemoveInvalid();

	/**
	 * @brief 检查消息链是否有效
	 * 
	 * 要求所有消息元素均有效，且消息不为空（存在至少一条可以发送的消息元素）
	 * @return `bool`
	 */
	bool valid() const;

	// Helper functions

	/// 在消息链结尾构造 `AppMessage`，返回自身的引用
	template<typename... Args> MessageChain& App(Args&&... args)
	{
		return this->emplace<AppMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AtAllMessage`，返回自身的引用
	template<typename... Args> MessageChain& AtAll(Args&&... args)
	{
		return this->emplace<AtAllMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AtMessage`，返回自身的引用
	template<typename... Args> MessageChain& At(Args&&... args)
	{
		return this->emplace<AtMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `AudioMessage`，返回自身的引用
	template<typename... Args> MessageChain& Audio(Args&&... args)
	{
		return this->emplace<AudioMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `DiceMessage`，返回自身的引用
	template<typename... Args> MessageChain& Dice(Args&&... args)
	{
		return this->emplace<DiceMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `FaceMessage`，返回自身的引用
	template<typename... Args> MessageChain& Face(Args&&... args)
	{
		return this->emplace<FaceMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `FlashImageMessage`，返回自身的引用
	template<typename... Args> MessageChain& Flash(Args&&... args)
	{
		return this->emplace<FlashImageMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `ImageMessage`，返回自身的引用
	template<typename... Args> MessageChain& Image(Args&&... args)
	{
		return this->emplace<ImageMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `JsonMessage`，返回自身的引用
	template<typename... Args> MessageChain& Json(Args&&... args)
	{
		return this->emplace<JsonMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `MiraiCodeMessage`，返回自身的引用
	template<typename... Args> MessageChain& MiraiCode(Args&&... args)
	{
		return this->emplace<MiraiCodeMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `PlainMessage`，返回自身的引用
	template<typename... Args> MessageChain& Plain(Args&&... args)
	{
		return this->emplace<PlainMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `PokeMessage`，返回自身的引用
	template<typename... Args> MessageChain& Poke(Args&&... args)
	{
		return this->emplace<PokeMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾构造 `XmlMessage`，返回自身的引用
	template<typename... Args> MessageChain& Xml(Args&&... args)
	{
		return this->emplace<XmlMessage>(std::forward<Args>(args)...);
	}

	/// 在消息链结尾插入 `ForwardMessage`，返回自身的引用
	template<typename... Args> MessageChain& Forward(Args&&... args)
	{
		return this->emplace<ForwardMessage>(std::forward<Args>(args)...);
	}

	struct Serializable;
};

using MessageElement = MessageChain::MessageElement;

} // namespace Mirai

#endif