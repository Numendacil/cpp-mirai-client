#ifndef _MIRAI_COMMAND_EXECUTED_EVENT_HPP_
#define _MIRAI_COMMAND_EXECUTED_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 命令执行事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `CommandExecutedEvent::_sender` | `""`
 * `CommandExecutedEvent::_friend` | `std::nullopt`
 * `CommandExecutedEvent::_member` | `std::nullopt`
 * `CommandExecutedEvent::_args` | `MessageChain{}`
 */
class CommandExecutedEvent : public EventBase
{
protected:
	std::string _name;
	std::optional<User> _friend = std::nullopt;
	std::optional<GroupMember> _member = std::nullopt;
	MessageChain _args;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "CommandExecutedEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual CommandExecutedEvent* Clone() const override
	// {
	//	return new CommandExecutedEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 发送者类型: 好友/群聊/控制台
	enum SenderType
	{
		FRIEND,
		MEMBER,
		CONSOLE
	};

	/// 获取命令名称（不含前缀 `/` ）
	std::string GetName() const { return this->_name; }
	/// 获取发送者类型
	SenderType GetSenderType() const
	{
		return this->_friend ? SenderType::FRIEND : (this->_member ? SenderType::MEMBER : SenderType::CONSOLE);
	}
	/// 获取好友发送者QQ，非好友发送时返回 `std::nullopt`
	std::optional<User> GetFriendSender() const { return this->_friend; }
	/// 获取群成员发送者资料，非群聊发送时返回 `std::nullopt`
	std::optional<GroupMember> GetMemberSender() const { return this->_member; }
	/// 获取命令参数内容
	MessageChain GetCommandArgs() const { return this->_args; }
};

} // namespace Mirai


#endif