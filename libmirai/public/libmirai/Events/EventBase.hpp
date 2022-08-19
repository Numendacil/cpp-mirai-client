#ifndef _MIRAI_EVENT_BASE_HPP_
#define _MIRAI_EVENT_BASE_HPP_

#include <memory>
#include <string>

#include <nlohmann/json_fwd.hpp>

namespace Mirai
{

class MiraiClient;

// TODO: Consider switching to typeid?

/**
 * @brief Base class for all event types
 * 
 * 所有有效的派生类均需要定义 `_TYPE_`，对应于mirai-api-http传来的JSON消息中的"type"属性，并在 `GetType()` 中返回 `_TYPE_`
 */
class EventBase
{
protected:
	/**
	 * @brief 指向 `MiraiClient` 的指针
	 * 
	 * 对应于接收该事件的 `MiraiClient` ，方便对事件作出响应
	 */
	MiraiClient* _client;

public:
	// static constexpr std::string_view _TYPE_ = "EventBase";

	/// Sets the `MiraiClient` pointer
	EventBase(MiraiClient* client) : _client(client) {}

	/**
	 * @brief Return the type of the class
	 * 
	 * Used for RTTI and message parsing, it should always return
	 * the value of the static `_TYPE_` member
	 */
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

	/// 获取接收该事件的 `MiraiClient` 的引用
	MiraiClient& GetMiraiClient() const { return *_client; }
};

} // namespace Mirai

#endif