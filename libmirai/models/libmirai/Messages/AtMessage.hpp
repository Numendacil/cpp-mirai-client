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

#ifndef MIRAI_AT_MESSAGE_HPP_
#define MIRAI_AT_MESSAGE_HPP_

#include <string>

#include <libmirai/Types/BasicTypes.hpp>

#include "IMessage.hpp"

namespace Mirai
{

/**
 * @brief At用户消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `AtMessage::target_`	 | `0_qq`
 * `AtMessage::display_` | `""`
 */
class AtMessage final : public IMessageImpl<AtMessage>
{
	friend class IMessageImpl<AtMessage>;

private:
	QQ_t target_{};
	std::string display_{};

	bool isValid_() const final { return this->target_ != QQ_t(); }

	static constexpr MessageTypes TYPE_ = MessageTypes::AT;
	static constexpr bool SUPPORT_SEND_ = true;

public:
	AtMessage() = default;
	AtMessage(QQ_t target) : target_(target) {}

	bool operator==(const AtMessage& rhs) { return this->target_ == rhs.target_; }

	bool operator!=(const AtMessage& rhs) { return !(*this == rhs); }

	/// 获取At对象QQ
	QQ_t GetTarget() const { return this->target_; }

	/// 获取At时显示的文字，发送时无法设置
	std::string GetDisplayName() { return this->display_; }

	/// 设置At对象QQ
	AtMessage& SetTarget(QQ_t target)
	{
		this->target_ = target;
		return *this;
	}

	struct Serializable;
};

template<> struct GetType<AtMessage::GetType()>
{
	using type = AtMessage;
};

} // namespace Mirai

#endif