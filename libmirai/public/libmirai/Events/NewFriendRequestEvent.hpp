#ifndef _MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_
#define _MIRAI_NEW_FRIEND_REQUEST_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class NewFriendRequestEvent : public EventBase
{

protected:
	int64_t _EventId = 0;
	QQ_t _FromId;
	GID_t _GroupId;
	std::string _nickname;
	std::string _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "NewFriendRequestEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual NewFriendRequestEvent* Clone() const override
	// {
	//	return new NewFriendRequestEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	int64_t GetEventId() const { return this->_EventId; }
	QQ_t GetUserId() const { return this->_FromId; }
	std::string GetNickname() const { return this->_nickname; }
	GID_t GetGroupId() const { return this->_GroupId; }
	bool isFromGroup() const { return this->_GroupId != (GID_t)0;}
	std::string GetMessage() const { return this->_message; }
};

}


#endif