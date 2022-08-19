#ifndef _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_
#define _MIRAI_GROUP_SYNC_MESSAGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Messages/MessageChain.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群聊消息同步事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `GroupSyncMessageEvent::_subject` | `Group{}`
 * `GroupSyncMessageEvent::_message` | `MessageChain{}`
 */
class GroupSyncMessageEvent : public EventBase
{

protected:
	Group _subject;
	MessageChain _message;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "GroupSyncMessage";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual GroupSyncMessageEvent* Clone() const override
	// {
	//	return new GroupSyncMessageEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取目标群聊信息
	Group GetGroup() const { return this->_subject; }
	/// 获取消息内容
	MessageChain GetMessage() const { return this->_message; }
};

} // namespace Mirai


#endif