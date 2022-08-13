#ifndef _MIRAI_MEMBER_JOIN_REQUEST_EVENT_HPP_
#define _MIRAI_MEMBER_JOIN_REQUEST_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

class MemberJoinRequestEvent : public EventBase
{

protected:
	int64_t _EventId = 0;
	QQ_t _FromId;
	GID_t _GroupId;
	std::string _GroupName;
	std::string _nickname;
	std::string _message;

public:
	static constexpr std::string_view _TYPE_ = "MemberJoinRequestEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual MemberJoinRequestEvent* Clone() const override
	// {
	//	return new MemberJoinRequestEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	int64_t GetEventId() const { return this->_EventId; }
	QQ_t GetUserId() const { return this->_FromId; }
	std::string GetNickname() const { return this->_nickname; }
	GID_t GetGroupId() const { return this->_GroupId; }
	std::string GetGroupName() const { return this->_GroupName; }
	std::string GetMessage() const { return this->_message; }
};

}


#endif