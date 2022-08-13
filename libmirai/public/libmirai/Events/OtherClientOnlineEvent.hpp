#ifndef _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>


#include "EventBase.hpp"

namespace Mirai
{

class OtherClientOnlineEvent : public EventBase
{

protected:
	ClientDevice _client;
	std::optional<int64_t> _kind;

public:
	static constexpr std::string_view _TYPE_ = "OtherClientOnlineEvent";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual OtherClientOnlineEvent* Clone() const override
	// {
	//	return new OtherClientOnlineEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	ClientDevice GetClient() const { return this->_client; }
	std::optional<int64_t> GetKind() const { return this->_kind; }
};

}


#endif