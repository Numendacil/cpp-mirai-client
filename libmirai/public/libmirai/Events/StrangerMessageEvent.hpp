#ifndef _MIRAI_STRANGER_MESSAGE_EVENT_HPP_
#define _MIRAI_STRANGER_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 陌生人消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `StrangerMessageEvent::_sender` | `User{}`
 * `StrangerMessageEvent::_message` | `MessageChain{}`
 */
class StrangerMessageEvent : public EventBase
{

protected:
	User _sender;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "StrangerMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual StrangerMessageEvent* Clone() const override
	// {
	//	return new StrangerMessageEvent(*this);
	// }/ }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送者资料
	User GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply
};

} // namespace Mirai


#endif