#ifndef _MIRAI_DICE_MESSAGE_HPP_
#define _MIRAI_DICE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 骰子消息
 * 
 * Member Variable | Default Value
 * --------------- | -------------
 * `DiceMessage::_value` | `-1`
 */
class DiceMessage : public MessageBase
{
protected:
	int _value;

public:
	DiceMessage() : _value(-1) {}
	DiceMessage(int value)
	{
		if (value > 0 && value <= 6) this->_value = value;
		else
			this->_value = -1;
	}
	DiceMessage(const DiceMessage&) = default;
	DiceMessage& operator=(const DiceMessage&) = default;
	DiceMessage(DiceMessage&&) noexcept = default;
	DiceMessage& operator=(DiceMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Dice";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual DiceMessage* Clone() const override { return new DiceMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;


	bool operator==(const DiceMessage& rhs) { return this->_value == rhs._value; }

	bool operator!=(const DiceMessage& rhs) { return !(*this == rhs); }

	/// 获取骰子点数
	int GetValue() const { return this->_value; }

	/// 设置骰子点数，不在1~6之间的值将不会被设置
	DiceMessage& SetValue(int value)
	{
		if (value > 0 && value <= 6) this->_value = value;
		return *this;
	}
};

} // namespace Mirai


#endif