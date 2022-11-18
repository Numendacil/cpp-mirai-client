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

#ifndef _MIRAI_FLASHIMAGE_MESSAGE_HPP_
#define _MIRAI_FLASHIMAGE_MESSAGE_HPP_

#include <string>


#include <libmirai/Types/BasicTypes.hpp>

#include "ImageMessage.hpp"

namespace Mirai
{

/**
 * @brief 闪照消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `FlashImageMessage::_image` | `MiraiImage{}`
 */
class FlashImageMessage : public ImageMessage
{
public:
	static constexpr MessageTypes _TYPE_ = MessageTypes::FLASH_IMAGE;
	
	FlashImageMessage() : ImageMessage() { this->_type = _TYPE_; }
	FlashImageMessage(const MiraiImage& image) : ImageMessage(image) { this->_type = _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<FlashImageMessage>(*this); }
};

template <>
struct GetType<FlashImageMessage::_TYPE_>
{
	using type = FlashImageMessage;
};

} // namespace Mirai


#endif