#ifndef _MIRAI_NUDGE_EVENT_HPP_
#define _MIRAI_NUDGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>
#include <libmirai/Types/NudgeTarget.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `NudgeEvent::_FromId` | `0_qq`
 * `NudgeEvent::_target` | `NudgeTarget{}`
 * `NudgeEvent::_action` | `""`
 * `NudgeEvent::_suffix` | `""`
 */
class NudgeEvent : public EventBase
{
protected:
	QQ_t _FromId;
	NudgeTarget _target;
	std::string _action;
	std::string _suffix;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "NudgeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual NudgeEvent* Clone() const override
	// {
	//	return new NudgeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取发送者QQ
	QQ_t GetSender() const { return this->_FromId; }
	/// 获取戳一戳消息的接收对象
	NudgeTarget GetTarget() const { return this->_target; }
	/// 获取自定义戳一戳的动作消息
	std::string GetAction() const { return this->_action; }
	/// 获取自定义戳一戳的内容
	std::string GetSuffix() const { return this->_suffix; }
};

} // namespace Mirai


#endif