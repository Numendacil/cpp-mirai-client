#ifndef _MIRAI_TYPES_NUDGE_TARGET_HPP_
#define _MIRAI_TYPES_NUDGE_TARGET_HPP_

#include <string>
#include <array>

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

class NudgeTarget
{
public:
	enum TargetKind : std::size_t { Friend = 0, GROUP, STRANGER, UNKNOWN };

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
	NudgeTarget() : _kind(TargetKind::UNKNOWN), _target(0), _subject(0) {}
	NudgeTarget(TargetKind kind, QQ_t target, UID_t subject)
	: _kind(kind), _target(target), _subject((int64_t)subject) {}

	TargetKind GetTargetKind() const { return this->_kind; }
	std::string GetTargetKindStr() const { return std::string(_to_string(this->_kind)); }
	QQ_t GetTarget() const { return this->_target; }
	// Only valid when kind == TargetKind::Group
	GID_t GetGroup() const { return (this->_kind == TargetKind::GROUP)? (GID_t)this->_subject : (GID_t)0; }

	void SetTargetKind(TargetKind kind) { this->_kind = kind; }
	void SetTargetKind(const std::string& kind) { this->_kind = _to_enum(kind); }
	void SetTarget(QQ_t target) { this->_target = target; }
	void SetSubject(int64_t subject) { this->_subject = subject; }

	void NudgeFriend(QQ_t target)
	{
		this->_kind = TargetKind::Friend;
		this->_target = target;
	}

	void NudgeGroupMember(QQ_t target, GID_t group)
	{
		this->_kind = TargetKind::GROUP;
		this->_target = target;
		this->_subject = (int64_t)group;
	}

	void NudgeStranger(QQ_t target)
	{
		this->_kind = TargetKind::STRANGER;
		this->_target = target;
	}

};

}

#endif