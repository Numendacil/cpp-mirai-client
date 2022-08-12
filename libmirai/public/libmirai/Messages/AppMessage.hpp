#ifndef _MIRAI_APP_MESSAGE_HPP_
#define _MIRAI_APP_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class AppMessage : public MessageBase
{

protected:
	std::string _content;


public:
	AppMessage() {}
	AppMessage(const std::string& content) : _content(content) {}
	AppMessage(const AppMessage&) = default;
	AppMessage& operator=(const AppMessage&) = default;
	AppMessage(AppMessage&&) noexcept = default;
	AppMessage& operator=(AppMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "App";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual AppMessage* Clone() const override
	{
		return new AppMessage(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;


	bool operator==(const AppMessage& rhs)
	{
		return this->_content == rhs._content;
	}

	bool operator!=(const AppMessage& rhs)
	{
		return !(*this == rhs);
	}

	std::string GetContent() const { return this->_content; }
	void SetContent(const std::string& content) { this->_content = content; }
};

}


#endif