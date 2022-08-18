#ifndef _MIRAI_MARKET_FACE_MESSAGE_HPP_
#define _MIRAI_MARKET_FACE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "FaceMessage.hpp"

namespace Mirai
{

/**
 * @brief QQ商店表情消息
 * 
 * 仅用于接收，发送时将会被无视
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `MarketFaceMessage::_id` | `-1`
 * `MarketFaceMessage::_name` | `""`
 */
class MarketFaceMessage : public FaceMessage
{

public:
	MarketFaceMessage() {}
	// MarketFaceMessage(int64_t id) : FaceMessage(id) {}
	// MarketFaceMessage(const std::string& name) : FaceMessage(name) {}
	MarketFaceMessage(const MarketFaceMessage&) = default;
	MarketFaceMessage& operator=(const MarketFaceMessage&) = default;
	MarketFaceMessage(MarketFaceMessage&&) noexcept = default;
	MarketFaceMessage& operator=(MarketFaceMessage&&) noexcept = default;

	static constexpr std::string_view _TYPE_ = "MarketFace";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual MarketFaceMessage* Clone() const override { return new MarketFaceMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json&) override;
	virtual nlohmann::json ToJson() const override;
};

} // namespace Mirai


#endif