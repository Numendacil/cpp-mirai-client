#ifndef _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_CARD_CHANGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员名片更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberCardChangeEvent::_member` | `GroupMember{}`
 * `MemberCardChangeEvent::_origin` | `""`
 * `MemberCardChangeEvent::_current` | `""`
 *
 * 由于服务器并不会告知名片变动, 此事件只能由 mirai 在发现变动时才广播
 */
class MemberCardChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	std::string _origin;
	std::string _current;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberCardChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberCardChangeEvent* Clone() const override
	// {
	//	return new MemberCardChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群名片
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前的群名片
	std::string GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif