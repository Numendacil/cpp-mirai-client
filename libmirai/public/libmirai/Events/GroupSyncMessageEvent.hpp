#ifndef _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class GroupSyncMessageEvent : public EventBase
{

protected:
	Group _subject;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "GroupSyncMessageEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual GroupSyncMessageEvent* Clone() const override
	{
		return new GroupSyncMessageEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	Group GetGroup() const { return this->_subject; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif