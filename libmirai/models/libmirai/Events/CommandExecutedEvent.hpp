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

#ifndef MIRAI_COMMAND_EXECUTED_EVENT_HPP_
#define MIRAI_COMMAND_EXECUTED_EVENT_HPP_

#include <optional>
#include <string>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "IEvent.hpp"

namespace Mirai
{

/**
 * @brief 命令执行事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `CommandExecutedEvent::sender_` | `""`
 * `CommandExecutedEvent::friend_` | `std::nullopt`
 * `CommandExecutedEvent::member_` | `std::nullopt`
 * `CommandExecutedEvent::args_` | `MessageChain{}`
 */
class CommandExecutedEvent final : public IEvent<CommandExecutedEvent>
{
	friend IEvent<CommandExecutedEvent>;
private:
	std::string name_;
	std::optional<User> friend_ = std::nullopt;
	std::optional<GroupMember> member_ = std::nullopt;
	MessageChain args_;

	static constexpr EventTypes TYPE_ = EventTypes::CommandExecuted;

public:
	/// 获取命令名称（不含前缀 `/` ）
	std::string GetName() const { return this->name_; }
	/// 获取发送者类型
	CommandSender GetSenderType() const
	{
		return this->friend_ ? CommandSender::FRIEND : (this->member_ ? CommandSender::MEMBER : CommandSender::CONSOLE);
	}
	/// 获取好友发送者QQ，非好友发送时返回 `std::nullopt`
	std::optional<User> GetFriendSender() const { return this->friend_; }
	/// 获取群成员发送者资料，非群聊发送时返回 `std::nullopt`
	std::optional<GroupMember> GetMemberSender() const { return this->member_; }
	/// 获取命令参数内容
	MessageChain GetCommandArgs() const { return this->args_; }

	struct Serializable;
};

template<>
struct GetEventType<CommandExecutedEvent::GetType()>
{
	using type = CommandExecutedEvent;
};

} // namespace Mirai


#endif