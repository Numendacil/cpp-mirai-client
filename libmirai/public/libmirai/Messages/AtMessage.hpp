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

#ifndef _MIRAI_AT_MESSAGE_HPP_
#define _MIRAI_AT_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief At用户消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `AtMessage::_target`	 | `0_qq`
 * `AtMessage::_display` | `""`
 */
class AtMessage : public MessageBase
{
protected:
	QQ_t _target{};
	std::string _display{};

public:
	AtMessage() = default;
	AtMessage(QQ_t target) : _target(target) {};

	static constexpr std::string_view _TYPE_ = "At";

	std::string_view GetType() const override { return _TYPE_; }

	std::unique_ptr<MessageBase> CloneUnique() const override { return std::make_unique<AtMessage>(*this); }

	bool isValid() const override;

	void FromJson(const nlohmann::json& data) override;
	nlohmann::json ToJson() const override;


	bool operator==(const AtMessage& rhs) { return this->_target == rhs._target; }

	bool operator!=(const AtMessage& rhs) { return !(*this == rhs); }

	/// 获取At对象QQ
	QQ_t GetTarget() const { return this->_target; }

	/// 获取At时显示的文字，发送时无法设置
	std::string GetDisplayName() { return this->_display; }

	/// 设置At对象QQ
	AtMessage& SetTarget(QQ_t target)
	{
		this->_target = target;
		return *this;
	}
};

} // namespace Mirai

#endif