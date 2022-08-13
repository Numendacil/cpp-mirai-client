#ifndef _MIRAI_STRANGER_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_STRANGER_SYNC_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class StrangerSyncMessageEvent : public EventBase
{

protected:
	User _subject;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "StrangerSyncMessage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual StrangerSyncMessageEvent* Clone() const override
	// {
	//	return new StrangerSyncMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	User GetStranger() const { return this->_subject; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif