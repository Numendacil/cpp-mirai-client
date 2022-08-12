#ifndef _MIRAI_GROUP_RECALL_EVENT_HPP_
#define _MIRAI_GROUP_RECALL_EVENT_HPP_

#include <ctime>
#include <optional>
#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class GroupRecallEvent : public EventBase
{
protected:
	QQ_t _AuthorId;
	MessageId_t _MessageId = 0;
	std::time_t _time = 0;
	Group _group;
	std::optional<GroupMember> _operator = std::nullopt;

public:
	static constexpr std::string_view _TYPE_ = "GroupRecallEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual GroupRecallEvent* Clone() const override
	{
		return new GroupRecallEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	QQ_t GetSender() const { return this->_AuthorId; }
	MessageId_t GetMessageId() const { return this->_MessageId; }
	std::time_t GetSendTime() const { return this->_time; }
	Group GetGroup() const { return this->_group; }
	std::optional<GroupMember> GetOperator() const { return this->_operator; }
};

}


#endif