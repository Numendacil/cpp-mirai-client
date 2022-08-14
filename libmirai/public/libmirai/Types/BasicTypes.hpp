#ifndef _MIRAI_TYPES_BASIC_TYPES_HPP_
#define _MIRAI_TYPES_BASIC_TYPES_HPP_

#include <chrono>
#include <string>
#include <array>
#include <ctime>

#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/Serializable.hpp>

#ifdef IGNORE	// Oh well, Windows.h again
#undef IGNORE
#endif

namespace Mirai
{

using MessageId_t = int64_t;

class UID_t
{
protected:
	int64_t _number;

	UID_t() : _number(0) {}
	explicit UID_t(int64_t num) : _number(num) {}

public:
	explicit operator int64_t() const { return this->_number; }
	std::string to_string() const { return std::to_string(this->_number); }

	bool operator < (const UID_t& uid) const { return this->_number < uid._number; }
	bool operator == (const UID_t& uid) const { return this->_number == uid._number; }

	bool operator != (const UID_t& uid) const { return !(*this == uid); }
	bool operator > (const UID_t& uid) const { return uid < *this; }
	bool operator >= (const UID_t& uid) const { return !(*this < uid); }
	bool operator <= (const UID_t& uid) const { return !(uid < *this); }

	friend void to_json(nlohmann::json&, const UID_t&);
	friend void from_json(const nlohmann::json&, UID_t&);
};

class QQ_t : public UID_t
{
public:
	QQ_t() : UID_t() {}
	explicit QQ_t(int64_t num) : UID_t(num) {}
};

inline QQ_t operator ""_qq(unsigned long long num) { return QQ_t(num); }

class GID_t : public UID_t
{
public:
	GID_t() : UID_t() {}
	explicit GID_t(int64_t num) : UID_t(num) {}
};

inline GID_t operator""_gid(unsigned long long num) { return GID_t(num); }


enum class NewFriendRequestOp {ACCEPT = 0, REFUSE, BLACKLIST};
enum class MemberJoinRequestOp {ACCEPT = 0, REFUSE, IGNORE, REFUSE_BLACKLIST, IGNORE_BLACKLIST};
enum class BotInvitedJoinGroupRequestOp {ACCEPT = 0, REFUSE};

enum class SEX : std::size_t {MALE = 0, FEMALE, UNKNOWN};

void to_json(nlohmann::json&, const SEX&);
void from_json(const nlohmann::json&, SEX&);


enum class PERMISSION : std::size_t {OWNER = 0, ADMINISTRATOR, MEMBER, UNKNOWN};

void to_json(nlohmann::json&, const PERMISSION&);
void from_json(const nlohmann::json&, PERMISSION&);


struct User : public Serializable
{
	QQ_t id;
	std::string nickname;
	std::string remark;

	User(QQ_t id = 0_qq, const std::string& nickname = "", const std::string& remark = "")
	: id(id), nickname(nickname), remark(remark) {}
	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const User& rhs) const
	{
		return this->id == rhs.id;
	}
};

struct Group : public Serializable
{
	GID_t id;
	std::string name;
	PERMISSION permission = PERMISSION::UNKNOWN;

	Group(GID_t id = 0_gid, const std::string& name = "", PERMISSION permission = PERMISSION::UNKNOWN)
	: id(id), name(name), permission(permission) {}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const Group& rhs) const
	{
		return this->id == rhs.id && this->permission == rhs.permission;
	}
};

struct GroupMember : public Serializable
{
	QQ_t id;
	std::string MemberName;
	PERMISSION permission = PERMISSION::UNKNOWN;
	std::string SpecialTitle;
	std::time_t JoinTimestamp = 0;
	std::time_t LastSpeakTimestamp = 0;
	std::chrono::seconds MuteTimeRemaining = std::chrono::seconds(0);
	Group group;

	GroupMember(QQ_t id = 0_qq, const std::string& MemberName = "", PERMISSION permission = PERMISSION::UNKNOWN,
	const std::string& SpecialTitle = "", std::time_t JoinTimestamp = 0, std::time_t LastSpeakTimestamp = 0,
	std::chrono::seconds MuteTimeRemaining = std::chrono::seconds(0), const Group& group = {})
	: id(id), MemberName(MemberName), permission(permission), SpecialTitle(SpecialTitle),
	  JoinTimestamp(JoinTimestamp), LastSpeakTimestamp(LastSpeakTimestamp), 
	  MuteTimeRemaining(MuteTimeRemaining), group(group) {}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const GroupMember& rhs) const
	{
		return this->id == rhs.id && this->group == rhs.group;
	}
};

struct UserProfile : public Serializable
{
	std::string nickname;
	std::string email;
	int age = 0;
	int level = 0;
	std::string sign;
	SEX sex = SEX::UNKNOWN;

	UserProfile(const std::string& nickname = "", const std::string& email = "", int age = 0,
	int level = 0, const std::string& sign = "", SEX sex = SEX::UNKNOWN)
	: nickname(nickname), email(email), age(age), level(level), sign(sign), sex(sex) {}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const UserProfile& rhs) const
	{
		return this->nickname == rhs.nickname
		&& this->email == rhs.email
		&& this->age == rhs.age
		&& this->level == rhs.level
		&& this->sign == rhs.sign
		&& this->sex == rhs.sex;
	}
};

struct ClientDevice : public Serializable
{
	int64_t id = 0;
	std::string platform;

	ClientDevice(int64_t id = 0, const std::string& platform = "")
	: id(id), platform(platform) {}

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const ClientDevice& rhs) const
	{
		return this->id == rhs.id && this->platform == rhs.platform;
	}
};


}

namespace std
{

template<> 
struct hash<Mirai::GID_t>
{
	std::size_t operator() (const Mirai::GID_t& t) const {
		return hash<int64_t>{}((int64_t)t);
	}
};
template<> 
struct hash<Mirai::QQ_t>
{
	std::size_t operator() (const Mirai::QQ_t& t) const {
		return hash<int64_t>{}((int64_t)t);
	}
};

}


#endif