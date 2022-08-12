#ifndef _MIRAI_TEMP_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_TEMP_SYNC_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class TempSyncMessageEvent : public EventBase
{

protected:
	GroupMember _subject;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "TempSyncMessageEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual TempSyncMessageEvent* Clone() const override
	{
		return new TempSyncMessageEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	GroupMember GetGroupMember() const { return this->_subject; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif