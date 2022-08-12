#ifndef _MIRAI_ATALL_MESSAGE_HPP_
#define _MIRAI_ATALL_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class AtAllMessage : public MessageBase
{
public:
	AtAllMessage() {}
	AtAllMessage(const AtAllMessage&) = default;
	AtAllMessage& operator=(const AtAllMessage&) = default;
	AtAllMessage(AtAllMessage&&) noexcept = default;
	AtAllMessage& operator=(AtAllMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "AtAll";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual AtAllMessage* Clone() const override
	{
		return new AtAllMessage(*this);
	}

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;
};

}


#endif