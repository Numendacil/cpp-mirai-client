#ifndef _MIRAI_QUOTE_MESSAGE_HPP_
#define _MIRAI_QUOTE_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageChain.hpp"
#include "MessageBase.hpp"

namespace Mirai
{

class QuoteMessage : public MessageBase
{

protected:
	MessageId_t _QuoteId;
	GID_t _GroupId;
	QQ_t _SenderId;
	int64_t _TargetId;
	MessageChain _origin;

public:
	QuoteMessage() : _QuoteId(-1) {}
	QuoteMessage(const QuoteMessage&) = default;
	QuoteMessage& operator=(const QuoteMessage&) = default;
	QuoteMessage(QuoteMessage&&) noexcept = default;
	QuoteMessage& operator=(QuoteMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Quote";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual QuoteMessage* Clone() const override
	{
		return new QuoteMessage(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;


	MessageId_t GetQuoteId() const { return this->_QuoteId; }
	GID_t GetGroupId() const { return this->_GroupId; }
	QQ_t GetSenderId() const { return this->_SenderId; }
	// Only valid when GroupId = 0
	QQ_t GetTargetId() const
	{
		if (this->_GroupId == (GID_t)0)
			return QQ_t(this->_TargetId);
		else
			return QQ_t(0);
	}
	MessageChain GetOriginMessage() const { return this->_origin; }
};

}


#endif