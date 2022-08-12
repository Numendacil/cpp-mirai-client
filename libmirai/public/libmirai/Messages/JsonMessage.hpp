#ifndef _MIRAI_JSON_MESSAGE_HPP_
#define _MIRAI_JSON_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class JsonMessage : public MessageBase
{

protected:
	std::string _content;


public:
	JsonMessage() {}
	JsonMessage(const std::string& content) : _content(content) {}
	JsonMessage(const JsonMessage&) = default;
	JsonMessage& operator=(const JsonMessage&) = default;
	JsonMessage(JsonMessage&&) noexcept = default;
	JsonMessage& operator=(JsonMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Json";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual JsonMessage* Clone() const override
	{
		return new JsonMessage(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;


	bool operator==(const JsonMessage& rhs)
	{
		return this->_content == rhs._content;
	}

	bool operator!=(const JsonMessage& rhs)
	{
		return !(*this == rhs);
	}

	std::string GetContent() const { return this->_content; }
	nlohmann::json GetJson() const;
	void SetContent(const std::string& content) { this->_content = content; }
};

}


#endif