#ifndef _MIRAI_GROUP_MESSAGE_EVENT_HPP_
#define _MIRAI_GROUP_MESSAGE_EVENT_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Messages/MessageChain.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群聊消息事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupMessageEvent::_sender` | `GroupMember{}`
 * `GroupMessageEvent::_message` | `MessageChain{}`
 */
class GroupMessageEvent : public EventBase
{

protected:
	GroupMember _sender;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupMessage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	// virtual GroupMessageEvent* Clone() const override
	// {
	//	return new GroupMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送者资料
	GroupMember GetSender() const { return this->_sender; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }

	// TODO: add helper methods for quick reply
};

}


#endif