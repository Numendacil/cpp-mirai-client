#ifndef _MIRAI_MEMBER_SPECIAL_TITLE_CHANGE_EVENT_HPP_
#define _MIRAI_MEMBER_SPECIAL_TITLE_CHANGE_EVENT_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "EventBase.hpp"

namespace Mirai
{

/**
 * @brief 群成员头衔更改事件
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MemberSpecialTitleChangeEvent::_member` | `GroupMember{}`
 * `MemberSpecialTitleChangeEvent::_origin` | `""`
 * `MemberSpecialTitleChangeEvent::_current` | `""`
 */
class MemberSpecialTitleChangeEvent : public EventBase
{
protected:
	GroupMember _member;
	std::string _origin;
	std::string _current;

public:
	using EventBase::EventBase;
	static constexpr std::string_view _TYPE_ = "MemberSpecialTitleChangeEvent";

	virtual std::string_view GetType() const override { return _TYPE_; }

	// virtual MemberSpecialTitleChangeEvent* Clone() const override
	// {
	//	return new MemberSpecialTitleChangeEvent(*this);
	// }

	virtual void FromJson(const nlohmann::json& data) override;

	/// 获取群成员资料
	GroupMember GetMember() const { return this->_member; }
	/// 获取更改前的群头衔
	std::string GetOriginal() const { return this->_origin; }
	/// 获取当前群头衔
	std::string GetCurrent() const { return this->_current; }
};

} // namespace Mirai


#endif