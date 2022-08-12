#ifndef _MIRAI_PLAIN_MESSAGE_HPP_
#define _MIRAI_PLAIN_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

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

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual PlainMessage* Clone() const override
	{
		return new PlainMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	
	bool operator==(const PlainMessage& rhs)
	{
		return this->_text == rhs._text;
	}

	bool operator!=(const PlainMessage& rhs)
	{
		return !(*this == rhs);
	}

	std::string GetText() const { return this->_text; }
	void SetText(const std::string& text) { this->_text = text; }
};

}


#endif