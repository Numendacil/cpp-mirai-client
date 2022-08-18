#ifndef _MIRAI_PLAIN_MESSAGE_HPP_
#define _MIRAI_PLAIN_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 文字消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PlainMessage::_text` | `""`
 */
class PlainMessage : public MessageBase
{
protected:
	std::string _text;

public:
	PlainMessage() {}
	PlainMessage(const std::string& text) : _text(text) {}
	PlainMessage(const PlainMessage&) = default;
	PlainMessage& operator=(const PlainMessage&) = default;
	PlainMessage(PlainMessage&&) noexcept = default;
	PlainMessage& operator=(PlainMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Plain";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual PlainMessage* Clone() const override { return new PlainMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;


	bool operator==(const PlainMessage& rhs) { return this->_text == rhs._text; }

	bool operator!=(const PlainMessage& rhs) { return !(*this == rhs); }

	/// 获取文字消息
	std::string GetText() const { return this->_text; }

	/// 设置文字消息
	PlainMessage& SetText(const std::string& text)
	{
		this->_text = text;
		return *this;
	}
};

} // namespace Mirai


#endif