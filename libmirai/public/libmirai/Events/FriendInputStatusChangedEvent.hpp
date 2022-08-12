#ifndef _MIRAI_FRIEND_INPUT_STATUS_CHANGED_EVENT_HPP_
#define _MIRAI_FRIEND_INPUT_STATUS_CHANGED_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class FriendInputStatusChangedEvent : public EventBase
{
protected:
	User _friend;
	bool _inputting = false;

public:
	static constexpr std::string_view _TYPE_ = "FriendInputStatusChangedEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FriendInputStatusChangedEvent* Clone() const override
	{
		return new FriendInputStatusChangedEvent(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;

	User GetTarget() const { return this->_friend; }
	bool isInputting() const { return this->_inputting; }
};

}


#endif