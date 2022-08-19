#ifndef _MIRAI_TEMP_MESSAGE_EVENT_HPP_
#define _MIRAI_TEMP_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 临时会话消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `TempMessageEvent::_sender` | `GroupMember{}`
 * `TempMessageEvent::_message` | `MessageChain{}`
 */
class TempMessageEvent : public EventBase
{

protected:
	GroupMember _sender;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "TempMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual TempMessageEvent* Clone() const override
	// {
	//	return new TempMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送者资料
	GroupMember GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply
};

} // namespace Mirai


#endif