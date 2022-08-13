#ifndef _MIRAI_EVENT_BASE_HPP_
#define _MIRAI_EVENT_BASE_HPP_

#include <string>
#include <memory>

#include <nlohmann/json_fwd.hpp>

namespace Mirai
{

class MiraiClient;

class EventBase
{
protected:
	friend class MiraiClient;
	MiraiClient* _client;

public:
	// static constexpr std::string_view _TYPE_ = "EventBase";

	virtual std::string_view GetType() const = 0;
 
	// virtual EventBase* Clone() const = 0;

	// virtual std::shared_ptr<EventBase> CloneShared() const
	// {
	// 	return std::shared_ptr<EventBase>(this->Clone());
	// }

	// virtual std::unique_ptr<EventBase> CloneUnique() const
	// {
	// 	return std::unique_ptr<EventBase>(this->Clone());
	// }

	virtual void FromJson(const nlohmann::json& data) = 0;

	virtual ~EventBase() = default;

	MiraiClient& GetMiraiClient() const { return *_client; }
};

} // namespace Mirai

#endif