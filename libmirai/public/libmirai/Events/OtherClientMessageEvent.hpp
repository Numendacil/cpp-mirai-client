#ifndef _MIRAI_OTHER_CLIENT_MESSAGE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class OtherClientMessageEvent : public EventBase
{

protected:
	ClientDevice _sender;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "OtherClientMessage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual OtherClientMessageEvent* Clone() const override
	// {
	//	return new OtherClientMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	ClientDevice GetSender() const { return this->_sender; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif