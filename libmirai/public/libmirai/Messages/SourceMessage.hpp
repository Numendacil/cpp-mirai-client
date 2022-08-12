#ifndef _MIRAI_SOURCE_MESSAGE_HPP_
#define _MIRAI_SOURCE_MESSAGE_HPP_

#include <string>
#include <ctime>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class SourceMessage : public MessageBase
{
protected:
	MessageId_t _id;
	std::time_t _timestamp;

public:

	SourceMessage() : _id(-1), _timestamp(0) {}
	SourceMessage(const SourceMessage&) = default;
	SourceMessage& operator=(const SourceMessage&) = default;
	SourceMessage(SourceMessage&&) noexcept = default;
	SourceMessage& operator=(SourceMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "Source";
	
	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual SourceMessage* Clone() const override
	{
		return new SourceMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;


	MessageId_t GetMessageId() const { return this->_id; }
	std::time_t GetTimestamp() const { return this->_timestamp; }
};

}


#endif