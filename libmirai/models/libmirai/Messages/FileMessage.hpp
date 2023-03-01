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

#ifndef _MIRAI_FILE_MESSAGE_HPP_
#define _MIRAI_FILE_MESSAGE_HPP_

#include <string>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief 文件消息
 * 
 * 仅用于接收，发送时将会被无视
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FileMessage::_id` | `""`
 * `FileMessage::_name` | `""`
 * `FileMessage::_size` | `0`
 */
class FileMessage final : public IMessageImpl<FileMessage>
{
	friend IMessageImpl<FileMessage>;

protected:
	std::string _id{};
	std::string _name{};
	int64_t _size = 0;

	bool _isValid() const final { return true; }

	static constexpr MessageTypes _TYPE_ = MessageTypes::FILE;
	static constexpr bool _SUPPORT_SEND_ = false;

public:
	FileMessage() = default;

	bool operator==(const FileMessage& rhs) { return this->_id == rhs._id; }

	bool operator!=(const FileMessage& rhs) { return !(*this == rhs); }

	/// 获取文件id
	std::string GetId() const { return this->_id; }
	/// 获取文件名
	std::string GetName() const { return this->_name; }
	/// 获取文件大小
	int64_t GetSize() const { return this->_size; }

	struct Serializable;
};

template<> struct GetType<FileMessage::GetType()>
{
	using type = FileMessage;
};

} // namespace Mirai

#endif