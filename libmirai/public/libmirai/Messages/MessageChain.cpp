#include <algorithm>
#include <nlohmann/json.hpp>

#include <optional>
#include <string>
#include <libmirai/Utils/Factory.hpp>
#include <libmirai/Utils/Common.hpp>

#include "Messages.hpp"
#include "MessageChain.hpp"
#include "ForwardMessage.hpp"

namespace Mirai
{

using json = nlohmann::json;

MessageChain::MessageChain(const MessageChain& rhs)
{
	MessageContainer v;
	v.reserve(rhs._message.size());
	for (const auto& p : rhs._message)
	{
		v.push_back(p->CloneUnique());
	}
	this->_message.swap(v);
}

MessageChain& MessageChain::operator=(const MessageChain& rhs)
{
	if(this != &rhs)
	{
		MessageContainer v;
		v.reserve(rhs._message.size());
		for (const auto& p : rhs._message)
		{
			v.push_back(p->CloneUnique());
		}
		this->_message.swap(v);
	}
	return *this;
}

MessageChain::MessageChain(MessageChain&& rhs) noexcept
: _message(std::move(rhs._message)) {}

MessageChain& MessageChain::operator=(MessageChain&& rhs) noexcept
{
	if(this != &rhs)
		this->_message = std::move(rhs._message);
	return *this;
}


MessageChain& MessageChain::operator+=(const MessageChain& rhs)
{
	this->_message.reserve(this->_message.size() + rhs._message.size());
	for (const auto& p : rhs._message)
	{
		this->_message.push_back(p->CloneUnique());
	}
	return *this;
}

MessageChain& MessageChain::operator+=(const MessageBase& rhs)
{
	this->_message.push_back(rhs.CloneUnique());
	return *this;
}

MessageChain operator+(const MessageChain& lhs, const MessageChain& rhs)
{
	MessageChain m;
	m.reserve(lhs.size() + rhs.size());
	for (const auto& p : lhs)
	{
		m.push_back(p->CloneUnique());
	}
	for (const auto& p : rhs)
	{
		m.push_back(p->CloneUnique());
	}
	return m;
}

MessageChain operator+(const MessageChain& lhs, const MessageBase& rhs)
{
	MessageChain m{lhs};
	m += rhs;
	return m;
}

MessageChain operator+(const MessageBase& lhs, const MessageChain& rhs)
{
	return rhs + lhs;
}


std::optional<MessageChain::SourceInfo> MessageChain::GetSourceInfo() const
{
	for (const auto& p : this->_message)
	{
		if (p->GetType() == SourceMessage::_TYPE_)
		{
			auto s = static_cast<SourceMessage*>(p.get());
			return SourceInfo{s->GetMessageId(), s->GetTimestamp()};
		}
	}
	return std::nullopt;
}


namespace
{

Utils::Factory<MessageBase> InitFactory()
{
	Utils::Factory<MessageBase> f;

	#ifdef NDEBUG
	#define  _REGISTER_(_class_) f.Register<_class_>(std::string(_class_::_TYPE_))
	#else
	#define _REGISTER_(_class_) assert(f.Register<_class_>(std::string(_class_::_TYPE_)))
	#endif

	_REGISTER_(AppMessage);
	_REGISTER_(AtAllMessage);
	_REGISTER_(AtMessage);
	_REGISTER_(AudioMessage);
	_REGISTER_(DiceMessage);
	_REGISTER_(FaceMessage);
	_REGISTER_(FileMessage);
	_REGISTER_(FlashImageMessage);
	_REGISTER_(ForwardMessage);
	_REGISTER_(ImageMessage);
	_REGISTER_(JsonMessage);
	_REGISTER_(MarketFaceMessage);
	_REGISTER_(MiraiCodeMessage);
	_REGISTER_(MusicShareMessage);
	_REGISTER_(PlainMessage);
	_REGISTER_(PokeMessage);
	_REGISTER_(QuoteMessage);
	_REGISTER_(SourceMessage);
	_REGISTER_(XmlMessage);

	#undef _REGISTER_
	
	return f;
}

Utils::Factory<MessageBase> MessageFactory{std::move(InitFactory())};

}

void MessageChain::RemoveInvalid()
{
	this->_message.erase(std::remove_if(this->_message.begin(), this->_message.end(),  
		[](const value_type& p) { return !p->isValid(); }), this->_message.end());
			
}

bool MessageChain::isValid() const
{
	for (const auto& p : this->_message)
		if (!p->isValid())
			return false;
	return true;
}

void MessageChain::FromJson(const json& data)
{
	if (!data.is_array())
		return;
	this->_message.clear();
	this->_message.reserve(data.size());
	for (const auto& p : data)
	{
		if (!p.is_object() || !p.contains("type") || !p["type"].is_string())
			continue;
		std::string type = p["type"].get<std::string>();
		if (!MessageFactory.Exist(type))
			continue;
		this->_message.emplace_back(MessageFactory.Make(type))->FromJson(p);
	}
}

json MessageChain::ToJson(bool ignoreInvalid) const
{
	json data = json::array();
	for (const auto& p : this->_message)
	{
		if (ignoreInvalid && !p->isValid())
			continue;
		json msg = p->ToJson();
		if (!msg.empty() && msg.contains("type"))
			data += msg;
	}
	return data;
}

MessageChain& MessageChain::Forward(ForwardMessage&& forward)
{
	return this->Append(std::forward<ForwardMessage>(forward));
}

void to_json(json &j, const MessageChain &p)
{
	j = p.ToJson();
}

void from_json(const json &j, MessageChain &p)
{
	p.FromJson(j);
}

}