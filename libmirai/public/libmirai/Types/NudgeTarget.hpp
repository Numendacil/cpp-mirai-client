#ifndef _MIRAI_TYPES_NUDGE_TARGET_HPP_
#define _MIRAI_TYPES_NUDGE_TARGET_HPP_

#include <string>
#include <array>

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

/**
 * @brief QQ移动端头像戳一戳动作的对象
 * 
 */
class NudgeTarget
{
public:
	/**
	 * @brief 戳一戳类型
	 * 
	 */
	enum TargetKind : std::size_t { FRIEND = 0, GROUP, STRANGER, UNKNOWN };

protected:
	TargetKind _kind;
	QQ_t _target;
	int64_t _subject;

	static constexpr std::array<std::string_view, static_cast<std::size_t>(TargetKind::UNKNOWN)> _TargetKindStr = 
	{
		"Friend",
		"Group",
		"Stranger"
	};

	static constexpr std::string_view _to_string(const TargetKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _TargetKindStr.size())
			return _TargetKindStr[i];
		else
			return "";
	}

	static constexpr TargetKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _TargetKindStr.size(); i++)
			if (_TargetKindStr[i] == s)
				return static_cast<TargetKind>(i);

		return TargetKind::UNKNOWN;
	}

public:
	/**
	 * @brief 构造函数
	 * 
	 */
	NudgeTarget() : _kind(TargetKind::UNKNOWN), _target(0), _subject(0) {}

	/**
	 * @brief 构造函数
	 * 
	 * @param kind 戳一戳的类型
	 * @param target 戳一戳的对象
	 * @param subject 戳一戳的发送主体，群聊消息时为群号，私聊时为对方QQ号码
	 */
	NudgeTarget(TargetKind kind, QQ_t target, UID_t subject)
	: _kind(kind), _target(target), _subject((int64_t)subject) {}

	/**
	 * @brief 返回戳一戳类型
	 * 
	 * @return 戳一戳类型 `enum`
	 */
	TargetKind GetTargetKind() const { return this->_kind; }

	/**
	 * @brief 返回戳一戳类型字符串
	 * 
	 * @return 戳一戳类型 `std::string`
	 */
	std::string GetTargetKindStr() const { return std::string(_to_string(this->_kind)); }

	/**
	 * @brief 返回戳一戳的对象
	 * 
	 * @return 戳一戳对象的QQ号
	 */
	QQ_t GetTarget() const { return this->_target; }
	
	/**
	 * @brief 返回戳一戳所在的群聊
	 * 
	 * 仅在类型为 `TargetKind::GROUP` 时有效，否则返回 `0`
	 * @return 戳一戳的群号
	 */
	GID_t GetGroup() const { return (this->_kind == TargetKind::GROUP)? (GID_t)this->_subject : (GID_t)0; }

	/**
	 * @brief 设置戳一戳类型
	 * 
	 * @param kind 戳一戳类型 `enum`
	 */
	void SetTargetKind(TargetKind kind) { this->_kind = kind; }

	/**
	 * @brief 设置戳一戳类型
	 * 
	 * @param kind 戳一戳类型 `std::string`
	 */
	void SetTargetKind(const std::string& kind) { this->_kind = _to_enum(kind); }

	/**
	 * @brief 设置戳一戳对象
	 * 
	 * @param target 戳一戳对象QQ
	 */
	void SetTarget(QQ_t target) { this->_target = target; }

	/**
	 * @brief 设置戳一戳的发送主体
	 * 
	 * 不推荐，考虑使用 `NudgeFriend()`, `NudgeGroupMember()` 与 `NudgeStranger()`
	 * @param subject 戳一戳的发送主体
	 */
	void SetSubject(int64_t subject) { this->_subject = subject; }


	/**
	 * @brief 设置为好友戳一戳消息
	 * 
	 * @param target 好友QQ 
	 */
	void NudgeFriend(QQ_t target)
	{
		this->_kind = TargetKind::FRIEND;
		this->_target = target;
	}

	/**
	 * @brief 设置为群聊戳一戳消息
	 * 
	 * @param target 群友QQ
	 * @param group 群聊号码
	 */
	void NudgeGroupMember(QQ_t target, GID_t group)
	{
		this->_kind = TargetKind::GROUP;
		this->_target = target;
		this->_subject = (int64_t)group;
	}

	/**
	 * @brief 设置为单向好友戳一戳消息
	 * 
	 * @param target 陌生人QQ
	 */
	void NudgeStranger(QQ_t target)
	{
		this->_kind = TargetKind::STRANGER;
		this->_target = target;
	}

};

}

#endif