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

#ifndef MIRAI_FACE_MESSAGE_HPP_
#define MIRAI_FACE_MESSAGE_HPP_

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
protected:
	int64_t id_ = -1;
	std::string name_{};

	bool isValid_() const override { return this->id_ != -1 || !this->name_.empty(); }

public:
	FaceMessageImpl() = default;
	FaceMessageImpl(int64_t id) : id_(id) {}
	FaceMessageImpl(std::string name) : name_(std::move(name)) {}

	bool operator==(const FaceMessageImpl& rhs) { return (id_ >= 0) ? this->id_ == rhs.id_ : this->name_ == rhs.name_; }

	bool operator!=(const FaceMessageImpl& rhs) { return !(*this == rhs); }

	/// 获取表情id
	int64_t GetId() const { return this->id_; }

	/// 获取表情名称
	std::string GetName() const { return this->name_; }

	/// 设置表情id
	Message& SetId(int64_t id)
	{
		this->id_ = id;
		return *static_cast<Message*>(this);
	}

	/// 设置表情名称。这一操作会清除已设置的id。
	Message& SetName(std::string name)
	{
		this->id_ = -1;
		this->name_ = std::move(name);
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
 * `FaceMessage::id_` | `-1`
 * `FaceMessage::name_` | `""`
 */
class FaceMessage final : public FaceMessageImpl<FaceMessage>
{
	friend IMessageImpl<FaceMessage>;

protected:
	static constexpr MessageTypes TYPE_ = MessageTypes::FACE;
	static constexpr bool SUPPORT_SEND_ = true;

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
 * `MarketFaceMessage::id_` | `-1`
 * `MarketFaceMessage::name_` | `""`
 */
class MarketFaceMessage final : public FaceMessageImpl<MarketFaceMessage>
{
	friend IMessageImpl<MarketFaceMessage>;

protected:
	bool isValid_() const final { return true; }

	static constexpr MessageTypes TYPE_ = MessageTypes::MARKET_FACE;
	static constexpr bool SUPPORT_SEND_ = false;

public:
	using FaceMessageImpl<MarketFaceMessage>::FaceMessageImpl;
};

template<> struct GetType<MarketFaceMessage::GetType()>
{
	using type = MarketFaceMessage;
};

} // namespace Mirai


#endif