#ifndef _MIRAI_OTHER_CLIENT_MESSAGE_EVENT_HPP_
#define _MIRAI_OTHER_CLIENT_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 其它客户端消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `OtherClientMessageEvent::_sender` | `ClientDevice{}`
 * `OtherClientMessageEvent::_message` | `MessageChain{}`
 */
class OtherClientMessageEvent : public EventBase
{
protected:
	ClientDevice _sender;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "OtherClientMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual OtherClientMessageEvent* Clone() const override
	// {
	//	return new OtherClientMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送客户端信息
	ClientDevice GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }
};

} // namespace Mirai


#endif