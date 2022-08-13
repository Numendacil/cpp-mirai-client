#ifndef _MIRAI_TEMP_MESSAGE_EVENT_HPP_
#define _MIRAI_TEMP_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class TempMessageEvent : public EventBase
{

protected:
	GroupMember _sender;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "TempMessage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual TempMessageEvent* Clone() const override
	// {
	//	return new TempMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetSender() const { return this->_sender; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif