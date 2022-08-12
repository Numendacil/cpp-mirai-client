#ifndef _MIRAI_FRIEND_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_FRIEND_SYNC_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class FriendSyncMessageEvent : public EventBase
{

protected:
	User _subject;
	MessageChain _message;

public:
	static constexpr std::string_view _TYPE_ = "FriendSyncMessageEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FriendSyncMessageEvent* Clone() const override
	{
		return new FriendSyncMessageEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	User GetFriend() const { return this->_subject; }
	MessageChain GetMessage() const { return this->_message; }
};

}


#endif