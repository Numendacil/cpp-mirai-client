#ifndef _MIRAI_MESSAGECHAIN_HPP_
#define _MIRAI_MESSAGECHAIN_HPP_

#include <cstddef>
#include <ctime>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>
#include <memory>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Exceptions/Exceptions.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"
#include "AppMessage.hpp"
#include "AtAllMessage.hpp"
#include "AtMessage.hpp"
#include "AudioMessage.hpp"
#include "DiceMessage.hpp"
#include "FaceMessage.hpp"
#include "FlashImageMessage.hpp"
#include "ImageMessage.hpp"
#include "JsonMessage.hpp"
#include "MiraiCodeMessage.hpp"
#include "PokeMessage.hpp"
#include "PlainMessage.hpp"
#include "XmlMessage.hpp"

namespace Mirai
{

class ForwardMessage;	// Avoid circular reference

class MessageChain
{
protected:
	using MessageContainer = std::vector<std::unique_ptr<MessageBase>>;
	MessageContainer _message;

	template <typename T>
	class _has_type_
	{
		typedef char yes_type;
		typedef long no_type;
		template <typename U> static yes_type test(decltype(&U::_TYPE_));
		template <typename U> static no_type test(...);

		public:
		static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes_type);
	};

	template <typename Message>
	constexpr static void _type_check_()
	{
		static_assert(std::is_base_of<MessageBase, Message>::value, "Message must be a derived class of MessageBase");
		static_assert(_has_type_<Message>::value, "Message must contain a static atrribute _TYPE_");
	};

public:
	MessageChain() = default;

	MessageChain(const MessageChain& rhs);
	MessageChain& operator=(const MessageChain& rhs);
	MessageChain(MessageChain&& rhs) noexcept;
	MessageChain& operator=(MessageChain&& rhs) noexcept;
	~MessageChain() = default;

	// STL-like types

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

	// Element Insertion

	template<typename MessageType>
	MessageChain& Append(MessageType&& m)
	{
		_type_check_<MessageType>();

		this->_message.push_back(std::make_unique<MessageType>(std::forward<MessageType>(m)));
		return *this;
	}

	template<typename MessageType, typename ...Args>
	MessageChain& Append(Args&& ...args)
	{
		_type_check_<MessageType>();

		this->_message.push_back(std::make_unique<MessageType>(std::forward<Args>(args)...));
		return *this;
	}

	template<typename MessageType>
	iterator Insert(const_iterator pos, MessageType&& m)
	{
		_type_check_<MessageType>();
		return this->_message.insert(pos, std::make_unique<MessageType>(std::forward<MessageType>(m)));
	}
	
	template<typename MessageType, typename ...Args>
	iterator Insert(const_iterator pos, Args&& ...args)
	{
		_type_check_<MessageType>();
		return this->_message.insert(pos, std::make_unique<MessageType>(std::forward<Args>(args)...));
	}

	MessageChain& operator+=(const MessageChain& rhs);
	MessageChain& operator+=(const MessageBase& rhs);
	friend MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs);
	friend MessageChain operator+(const MessageChain& lhs, const MessageBase& rhs);
	friend MessageChain operator+(const MessageBase& lhs, const MessageChain& rhs);

	// Element Access

	template<typename MessageType>
	MessageType& GetAt(size_type i) const
	{
		_type_check_<MessageType>();

		const auto& m = this->_message.at(i);
		if (!m)
			throw std::runtime_error("Unexpected null pointer in MessageChain::at");
		if (m->GetType() != MessageType::_TYPE_)
			throw TypeDismatch(std::string(m->GetType()), std::string(MessageType::_TYPE_));
		return *static_cast<MessageType*>(m.get());
	}
	
	MessageBase& GetAt(size_type i) const
	{
		const auto& m = this->_message.at(i);
		if (!m)
			throw std::runtime_error("Unexpected null pointer in MessageChain::at");
		return *m;
	}

	template<typename MessageType>
	std::vector<MessageType> GetAll() const
	{
		_type_check_<MessageType>();

		std::vector<MessageType> v;
		for (const auto& p : this->_message)
		{
			if (p->GetType() == MessageType::_TYPE_)
				v.emplace_back(*static_cast<MessageType*>(p.get()));
		}
		return v;
	}

	struct SourceInfo
	{
		MessageId_t id = 0;
		std::time_t time = 0;
	};

	std::optional<SourceInfo> GetSourceInfo() const;

	// Serialization

	void RemoveInvalid();
	bool isValid() const;
	void FromJson(const nlohmann::json& data);
	nlohmann::json ToJson(bool ignoreInvalid = false) const;

	// Helper functions

	template<typename ...Args>
	MessageChain& App(Args&& ...args)
	{
		return this->Append<AppMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& AtAll(Args&& ...args)
	{
		return this->Append<AtAllMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& At(Args&& ...args)
	{
		return this->Append<AtMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Audio(Args&& ...args)
	{
		return this->Append<AudioMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Dice(Args&& ...args)
	{
		return this->Append<DiceMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Face(Args&& ...args)
	{
		return this->Append<FaceMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Flash(Args&& ...args)
	{
		return this->Append<FlashImageMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Image(Args&& ...args)
	{
		return this->Append<ImageMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Json(Args&& ...args)
	{
		return this->Append<JsonMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& MiraiCode(Args&& ...args)
	{
		return this->Append<MiraiCodeMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Plain(Args&& ...args)
	{
		return this->Append<PlainMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Poke(Args&& ...args)
	{
		return this->Append<PokeMessage>(std::forward<Args>(args)...);
	}

	template<typename ...Args>
	MessageChain& Xml(Args&& ...args)
	{
		return this->Append<XmlMessage>(std::forward<Args>(args)...);
	}

	MessageChain& Forward(ForwardMessage&& forward);


	// STL-like interface

	bool empty() const noexcept { return this->_message.empty(); }
	size_type size() const noexcept { return this->_message.size(); }
	void reserve(size_type new_cap) { return this->_message.reserve(new_cap); }
	void shrink_to_fit() noexcept { return this->_message.shrink_to_fit(); }
	size_type max_size() const noexcept { return this->_message.max_size(); }
	size_type capacity() const noexcept { return this->_message.capacity(); }

	const_reference at(size_type n) const { return this->_message.at(n); }
	reference at(size_type n) { return this->_message.at(n); }
	const_reference operator[](size_type n) const noexcept { return this->_message[n]; }
	reference operator[](size_type n) noexcept { return this->_message[n]; }

	void clear() noexcept { return this->_message.clear(); }
	iterator insert(const_iterator pos, value_type&& value) { return this->_message.insert(pos, std::move(value)); }
	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) { return this->_message.insert(pos, std::forward<InputIt>(first), std::forward<InputIt>(last)); }
	template<class... Args >
	iterator emplace(const_iterator pos, Args&&... args) { return this->emplace(pos, std::forward<Args>(args)...); }
	iterator erase(const_iterator pos) { return this->_message.erase(pos); }
	iterator erase(const_iterator first, const_iterator last) { return this->_message.erase(first, last); }
	void push_back(value_type&& node) { return this->_message.push_back(std::move(node)); }
	template< class... Args >
	reference emplace_back(Args&&... args) { return this->_message.emplace_back(std::forward<Args>(args)...); }
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
};

void to_json(nlohmann::json &j, const MessageChain &p);

void from_json(const nlohmann::json &j, MessageChain &p);

}

#endif