#ifndef _MIRAI_QUOTE_MESSAGE_HPP_
#define _MIRAI_QUOTE_MESSAGE_HPP_

#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"
#include "MessageChain.hpp"

namespace Mirai
{

/**
 * @brief 引用回复消息
 *
 * 仅用于接收，发送时将会被无视。发送引用回复消息请在消息发送函数中设置 `QuoteId`
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `QuoteMessage::_QuoteId` | `-1`
 * `QuoteMessage::_GroupId` | `0_gid`
 * `QuoteMessage::_SenderId` | `0_qq`
 * `QuoteMessage::_TargetId` | `0`
 * `QuoteMessage::_origin` | `MessageChain{}`
 */
class QuoteMessage : public MessageBase
{

protected:
	MessageId_t _QuoteId;
	GID_t _GroupId;
	QQ_t _SenderId;
	int64_t _TargetId;
	MessageChain _origin;

public:
	QuoteMessage() : _QuoteId(-1), _TargetId(0) {}
	QuoteMessage(const QuoteMessage&) = default;
	QuoteMessage& operator=(const QuoteMessage&) = default;
	QuoteMessage(QuoteMessage&&) noexcept = default;
	QuoteMessage& operator=(QuoteMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Quote";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual QuoteMessage* Clone() const override { return new QuoteMessage(*this); }

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	virtual bool isValid() const override;

	/// 获取被引用消息id
	MessageId_t GetQuoteId() const { return this->_QuoteId; }
	/// 获取被引用消息所在群聊id，若为好友消息则为 `0`
	GID_t GetGroupId() const { return this->_GroupId; }
	/// 获取引用消息的发送者QQ
	QQ_t GetSenderId() const { return this->_SenderId; }
	/// 获取引用消息的接收者QQ，仅当好友消息时有效
	QQ_t GetTargetId() const
	{
		if (this->_GroupId == (GID_t)0) return QQ_t(this->_TargetId);
		else
			return QQ_t(0);
	}
	/// 获取被引用消息
	MessageChain GetOriginMessage() const { return this->_origin; }
};

} // namespace Mirai


#endif