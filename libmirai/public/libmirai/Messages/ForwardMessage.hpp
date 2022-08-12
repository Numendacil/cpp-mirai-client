#ifndef _MIRAI_FORWARD_MESSAGE_HPP_
#define _MIRAI_FORWARD_MESSAGE_HPP_

#include <ctime>
#include <utility>
#include <vector>
#include <optional>

#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>
#include "MessageChain.hpp"

#include "MessageBase.hpp"

namespace Mirai
{

class ForwardMessage : public MessageBase
{
public:
	class Node : public Serializable
	{
	protected:
		QQ_t _SenderId;
		std::time_t _time;
		std::string _SenderName;
		MessageChain _message;
		std::optional<MessageId_t> _MessageId;
	public:
		Node() : _time(0), _MessageId(std::nullopt) {}
		Node(const Node&) = default;
		Node& operator=(const Node&) = default;
		Node(Node&&) noexcept = default;
		Node& operator=(Node&&) noexcept = default;

		bool isValid() const;
		virtual void FromJson(const nlohmann::json& data) override;
		virtual nlohmann::json ToJson() const override;

		QQ_t GetSenderId() const { return this->_SenderId; }
		std::time_t GetTimestamp() const { return this->_time; }
		std::string GetSenderName() const { return this->_SenderName; }
		MessageChain GetMessageChain() const { return this->_message; }
		bool hasMessageId() const { return this->_MessageId.has_value(); }
		MessageId_t GetMessageId() const { return this->_MessageId.value_or(0); }

		void SetSenderId(QQ_t SenderId) { this->_SenderId = SenderId; }
		void GetTimestamp(std::time_t time) { this->_time = time; }
		void SetSenderName(const std::string& SenderName) { this->_SenderName = SenderName; }
		void SetMessageChain(const MessageChain& message) { this->_message = message; }
		void SetMessageChain(MessageChain&& message) { this->_message = std::move(message); }
		void SetMessageChain(const nlohmann::json& message) { this->_message.FromJson(message); }
		void SetMessageId(std::optional<MessageId_t> MessageId) { this->_MessageId = MessageId; }
	};
protected:
	using NodeList = std::vector<Node>;
	NodeList _NodeList;

public:
	ForwardMessage() {}
	ForwardMessage(const ForwardMessage&) = default;
	ForwardMessage& operator=(const ForwardMessage&) = default;
	ForwardMessage(ForwardMessage&&) noexcept = default;
	ForwardMessage& operator=(ForwardMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Forward";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual ForwardMessage* Clone() const override
	{
		return new ForwardMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	// STL-like interface

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
	iterator insert(const_iterator pos, size_type count, const_reference value) { return this->_NodeList.insert(pos, count, value); }
	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) { return this->_NodeList.insert(pos, std::forward<InputIt>(first), std::forward<InputIt>(last)); }
	iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) { return this->_NodeList.insert(pos, ilist); }
	template<class... Args >
	iterator emplace(const_iterator pos, Args&&... args) { return this->emplace(pos, std::forward<Args>(args)...); }
	iterator erase(const_iterator pos) { return this->_NodeList.erase(pos); }
	iterator erase(const_iterator first, const_iterator last) { return this->_NodeList.erase(first, last); }
	void push_back(const_reference node) { return this->_NodeList.push_back(node); }
	void push_back(value_type&& node) { return this->_NodeList.push_back(std::move(node)); }
	template< class... Args >
	reference emplace_back(Args&&... args) { return this->_NodeList.emplace_back(std::forward<Args>(args)...); }
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
};

}


#endif