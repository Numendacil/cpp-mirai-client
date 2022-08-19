#ifndef _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_ONLINE_EVENT_HPP_

#include <optional>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 其它客户端上线事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientOnlineEvent::_client` | `ClientDevice{}`
 * `OtherClientOnlineEvent::_kind` | `std::nullopt`
 */
class OtherClientOnlineEvent : public EventBase
{

protected:
	ClientDevice _client;
	std::optional<int64_t> _kind = std::nullopt;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "OtherClientOnlineEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual OtherClientOnlineEvent* Clone() const override
	// {
	//	return new OtherClientOnlineEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取设备信息
	ClientDevice GetClient() const { return this->_client; }
	/// 获取详细设备类型，无则返回 `std::nullopt`
	std::optional<int64_t> GetKind() const { return this->_kind; }

	// TODO: replace kind with enum, see https://github.com/mamoe/mirai/blob/dev/mirai-core-api/src/commonMain/kotlin/contact/OtherClient.kt#L113
};

} // namespace Mirai


#endif