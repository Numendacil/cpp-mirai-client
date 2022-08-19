#ifndef _MIRAI_OTHER_CLIENT_OFFLINE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_OFFLINE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 其它客户端下线事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientOfflineEvent::_client` | `ClientDevice{}`
 */
class OtherClientOfflineEvent : public EventBase
{

protected:
	ClientDevice _client;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "OtherClientOfflineEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual OtherClientOfflineEvent* Clone() const override
	// {
	//	return new OtherClientOfflineEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取设备信息
	ClientDevice GetClient() const { return this->_client; }
};

} // namespace Mirai


#endif