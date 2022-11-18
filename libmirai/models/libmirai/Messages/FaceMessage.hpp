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


#include "MessageBase.hpp"

namespace Mirai
{

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
class FaceMessage : public MessageBase
{
protected:
	int64_t _id = -1;
	std::string _name{};

	void Serialize(void*) const override;
	void Deserialize(const void*) override;
public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::FACE;
	
	FaceMessage() : MessageBase(_TYPE_) {}
	FaceMessage(int64_t id) : _id(id), MessageBase(_TYPE_) {}
	FaceMessage(std::string name) : _name(std::move(name)), MessageBase(_TYPE_) {}

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<FaceMessage>(*this); }

	bool isValid() const override
	{
		return this->_id != -1 || !this->_name.empty();
	}

	bool operator==(const FaceMessage& rhs) { return (_id >= 0) ? this->_id == rhs._id : this->_name == rhs._name; }

	bool operator!=(const FaceMessage& rhs) { return !(*this == rhs); }

	/// 获取表情id
	int64_t GetId() const { return this->_id; }

	/// 获取表情名称
	std::string GetName() const { return this->_name; }

	/// 设置表情id
	FaceMessage& SetId(int64_t id)
	{
		this->_id = id;
		return *this;
	}

	/// 设置表情名称。这一操作会清除已设置的id。
	FaceMessage& SetName(std::string name)
	{
		this->_id = -1;
		this->_name = std::move(name);
		return *this;
	}
};

template <>
struct GetType<FaceMessage::_TYPE_>
{
	using type = FaceMessage;
};

} // namespace Mirai


#endif