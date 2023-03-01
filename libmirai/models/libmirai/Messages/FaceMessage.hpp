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

#ifndef _MIRAI_FACE_MESSAGE_HPP_
#define _MIRAI_FACE_MESSAGE_HPP_

#include <string>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief QQ表情类消息
 * 
 * Middleware for face related messages
 */
template <class Message>
class FaceMessageImpl : public IMessageImpl<Message>
{
	friend IMessageImpl<Message>;

protected:
	int64_t _id = -1;
	std::string _name{};

	bool _isValid() const override { return this->_id != -1 || !this->_name.empty(); }

public:
	FaceMessageImpl() = default;
	FaceMessageImpl(int64_t id) : _id(id) {}
	FaceMessageImpl(std::string name) : _name(std::move(name)) {}

	bool operator==(const FaceMessageImpl& rhs) { return (_id >= 0) ? this->_id == rhs._id : this->_name == rhs._name; }

	bool operator!=(const FaceMessageImpl& rhs) { return !(*this == rhs); }

	/// 获取表情id
	int64_t GetId() const { return this->_id; }

	/// 获取表情名称
	std::string GetName() const { return this->_name; }

	/// 设置表情id
	Message& SetId(int64_t id)
	{
		this->_id = id;
		return *static_cast<Message*>(this);
	}

	/// 设置表情名称。这一操作会清除已设置的id。
	Message& SetName(std::string name)
	{
		this->_id = -1;
		this->_name = std::move(name);
		return *static_cast<Message*>(this);
	}

	struct Serializable;
};



/**
 * @brief QQ表情消息
 * 
 * 表情可以通过 `id` 或 `name` 发送，优先级 `id` > `name`
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FaceMessage::_id` | `-1`
 * `FaceMessage::_name` | `""`
 */
class FaceMessage final : public FaceMessageImpl<FaceMessage>
{
	friend IMessageImpl<FaceMessage>;

protected:
	static constexpr MessageTypes _TYPE_ = MessageTypes::FACE;
	static constexpr bool _SUPPORT_SEND_ = true;

public:
	using FaceMessageImpl<FaceMessage>::FaceMessageImpl;
};

template<> struct GetType<FaceMessage::GetType()>
{
	using type = FaceMessage;
};



/**
 * @brief QQ商店表情消息
 * 
 * 仅用于接收，发送时将会被无视
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `MarketFaceMessage::_id` | `-1`
 * `MarketFaceMessage::_name` | `""`
 */
class MarketFaceMessage final : public FaceMessageImpl<MarketFaceMessage>
{
	friend IMessageImpl<MarketFaceMessage>;

protected:
	bool _isValid() const final { return true; }

	static constexpr MessageTypes _TYPE_ = MessageTypes::MARKET_FACE;
	static constexpr bool _SUPPORT_SEND_ = false;

public:
	using FaceMessageImpl<MarketFaceMessage>::FaceMessageImpl;
};

template<> struct GetType<MarketFaceMessage::GetType()>
{
	using type = MarketFaceMessage;
};

} // namespace Mirai


#endif