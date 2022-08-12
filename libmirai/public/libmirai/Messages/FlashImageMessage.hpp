#ifndef _MIRAI_FLASHIMAGE_MESSAGE_HPP_
#define _MIRAI_FLASHIMAGE_MESSAGE_HPP_

#include <string>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include <libmirai/Utils/Common.hpp>
#include "ImageMessage.hpp"

namespace Mirai
{

class FlashImageMessage : public ImageMessage
{

public:
	FlashImageMessage(const MiraiImage& image = {})
	: ImageMessage(image) {}
	FlashImageMessage(const FlashImageMessage&) = default;
	FlashImageMessage& operator=(const FlashImageMessage&) = default;
	FlashImageMessage(FlashImageMessage&&) noexcept = default;
	FlashImageMessage& operator=(FlashImageMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "FlashImage";

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual FlashImageMessage* Clone() const override
	{
		return new FlashImageMessage(*this);
	}
};

}


#endif