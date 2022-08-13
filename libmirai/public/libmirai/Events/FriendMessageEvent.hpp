#ifndef _MIRAI_FRIEND_MESSAGE_EVENT_HPP_
#define _MIRAI_FRIEND_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class FriendMessageEvent : public EventBase
{

protected:
	User _sender;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "FriendMessage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual FriendMessageEvent* Clone() const override
	// {
	//	return new FriendMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	User GetSender() const { return this->_sender; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif