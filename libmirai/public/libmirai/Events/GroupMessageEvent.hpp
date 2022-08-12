#ifndef _MIRAI_GROUP_MESSAGE_EVENT_HPP_
#define _MIRAI_GROUP_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class GroupMessageEvent : public EventBase
{

protected:
	GroupMember _sender;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "GroupMessageEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual GroupMessageEvent* Clone() const override
	{
		return new GroupMessageEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetSender() const { return this->_sender; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif