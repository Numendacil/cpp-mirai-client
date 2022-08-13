#ifndef _MIRAI_OTHER_CLIENT_OFFLINE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_OFFLINE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class OtherClientOfflineEvent : public EventBase
{

protected:
	ClientDevice _client;

public:
	static constexpr std::string_view _TYPE_ = "OtherClientOfflineEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual OtherClientOfflineEvent* Clone() const override
	// {
	//	return new OtherClientOfflineEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	ClientDevice GetClient() const { return this->_client; }
};

}


#endif