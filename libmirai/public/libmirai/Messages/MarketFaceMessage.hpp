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

#ifndef _MIRAI_MARKET_FACE_MESSAGE_HPP_
#define _MIRAI_MARKET_FACE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "FaceMessage.hpp"

namespace Mirai
{

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
class MarketFaceMessage : public FaceMessage
{

public:
	MarketFaceMessage() = default;

	static constexpr std::string_view _TYPE_ = "MarketFace";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<MarketFaceMessage>(*this); }

	bool isValid() const override;

	void FromJson(const nlohmann::json&) override;
	nlohmann::json ToJson() const override;
};

} // namespace Mirai


#endif