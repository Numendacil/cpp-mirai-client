#ifndef _MIRAI_FRIEND_RECALL_EVENT_HPP_
#define _MIRAI_FRIEND_RECALL_EVENT_HPP_

#include <ctime>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class FriendRecallEvent : public EventBase
{
protected:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	QQ_t _operator;

public:
	static constexpr std::string_view _TYPE_ = "FriendRecallEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FriendRecallEvent* Clone() const override
	{
		return new FriendRecallEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	QQ_t GetSender() const { return this->_AuthorId; }
	MessageId_t GetMessageId() const { return this->_MessageId; }
	std::time_t GetSendTime() const { return this->_time; }
	QQ_t GetOperator() const { return this->_operator; }
};

}


#endif