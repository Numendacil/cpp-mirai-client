#ifndef _MIRAI_COMMAND_EXECUTED_EVENT_HPP_
#define _MIRAI_COMMAND_EXECUTED_EVENT_HPP_

#include <optional>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

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

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual CommandExecutedEvent* Clone() const override
	// {
	//	return new CommandExecutedEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	enum SenderType {FRIEND, MEMBER, CONSOLE};

	std::string GetName() const { return this->_name; }
	SenderType GetSenderType() const 
	{ 
		return this->_friend ? SenderType::FRIEND : (this->_member ? SenderType::MEMBER : SenderType::CONSOLE); 
	}
	std::optional<User> GetFriendSender() const { return this->_friend; }
	std::optional<GroupMember> GetMemberSender() const { return this->_member; }
	MessageChain GetCommandArgs() const { return this->_args; }
};

}


#endif