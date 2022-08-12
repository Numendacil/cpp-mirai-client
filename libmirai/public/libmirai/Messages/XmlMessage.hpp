#ifndef _MIRAI_XML_MESSAGE_HPP_
#define _MIRAI_XML_MESSAGE_HPP_


#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class XmlMessage : public MessageBase
{

protected:
	std::string _content;


public:
	XmlMessage() {}
	XmlMessage(const std::string& content) : _content(content) {}
	XmlMessage(const XmlMessage&) = default;
	XmlMessage& operator=(const XmlMessage&) = default;
	XmlMessage(XmlMessage&&) noexcept = default;
	XmlMessage& operator=(XmlMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Xml";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual XmlMessage* Clone() const override
	{
		return new XmlMessage(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;


	bool operator==(const XmlMessage& rhs)
	{
		return this->_content == rhs._content;
	}

	bool operator!=(const XmlMessage& rhs)
	{
		return !(*this == rhs);
	}

	std::string GetContent() const { return this->_content; }
	void SetContent(const std::string& content) { this->_content = content; }
};

}


#endif