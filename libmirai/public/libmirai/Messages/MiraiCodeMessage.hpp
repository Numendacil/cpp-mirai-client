#ifndef _MIRAI_MIRAICODE_MESSAGE_HPP_
#define _MIRAI_MIRAICODE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

/**
 * @brief  Mirai码消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `MiraiCodeMessage::_code` | `""`
 */
namespace Mirai
{

class MiraiCodeMessage : public MessageBase
{
protected:
	std::string _code;

public:
	MiraiCodeMessage() {}
	MiraiCodeMessage(const std::string& code) : _code(code) {}
	MiraiCodeMessage(const MiraiCodeMessage&) = default;
	MiraiCodeMessage& operator=(const MiraiCodeMessage&) = default;
	MiraiCodeMessage(MiraiCodeMessage&&) noexcept = default;
	MiraiCodeMessage& operator=(MiraiCodeMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "MiraiCode";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual MiraiCodeMessage* Clone() const override { return new MiraiCodeMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;


	bool operator==(const MiraiCodeMessage& rhs) { return this->_code == rhs._code; }

	bool operator!=(const MiraiCodeMessage& rhs) { return !(*this == rhs); }

	/// 获取mirai码
	std::string GetCode() const { return this->_code; }

	/// 设置mirai码
	MiraiCodeMessage& SetCode(const std::string& code)
	{
		this->_code = code;
		return *this;
	}
};

} // namespace Mirai

#endif