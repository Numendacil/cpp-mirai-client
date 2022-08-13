#ifndef _MIRAI_MUSIC_SHARE_MESSAGE_HPP_
#define _MIRAI_MUSIC_SHARE_MESSAGE_HPP_

#include <string>
#include <array>
#include <nlohmann/json_fwd.hpp>
#include <libmirai/Types/BasicTypes.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

class MusicShareMessage : public MessageBase
{
public:
	enum MusicKind : std::size_t 
	{ 
		NETEASECLOUDMUSIC = 0,
		QQMUSIC,
		MIGUMUSIC,
		KUGOUMUSIC,
		KUWOMUSIC,
		UNKNOWN
	};

protected:
	MusicKind _kind;
	std::string _title;
	std::string _summary;
	std::string _JumpUrl;
	std::string _PictureUrl;
	std::string _MusicUrl;
	std::string _brief;

	
	static constexpr std::array<std::string_view, static_cast<std::size_t>(MusicKind::UNKNOWN)> _MusicKindStr = 
	{
		"NeteaseCloudMusic",
		"QQMusic",
		"MiguMusic",
		"KugouMusic",
		"KuwoMusic"
	};

	static constexpr std::string_view _to_string(const MusicKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _MusicKindStr.size())
			return _MusicKindStr[i];
		else
			return "";
	}

	static constexpr MusicKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _MusicKindStr.size(); i++)
			if (_MusicKindStr[i] == s)
				return static_cast<MusicKind>(i);

		return MusicKind::UNKNOWN;
	}

public:
	static constexpr std::string_view _TYPE_ = "MusicShare";

	MusicShareMessage() {}
	MusicShareMessage(MusicKind kind, const std::string& title, const std::string& summary,
	const std::string& JumpUrl, const std::string& PictureUrl, const std::string& MusicUrl, const std::string& brief) 
	: _kind(kind), _title(title), _summary(summary), 
	_JumpUrl(JumpUrl), _PictureUrl(PictureUrl), _MusicUrl(MusicUrl), _brief(brief) {}
	MusicShareMessage(const MusicShareMessage&) = default;
	MusicShareMessage& operator=(const MusicShareMessage&) = default;
	MusicShareMessage(MusicShareMessage&&) noexcept = default;
	MusicShareMessage& operator=(MusicShareMessage&&) noexcept = default;

	virtual std::string_view GetType() const override
	{
		return _TYPE_;
	}

	virtual MusicShareMessage* Clone() const override
	{
		return new MusicShareMessage(*this);
	}

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	MusicKind GetKind() const { return this->_kind; }
	std::string GetTitle() const { return this->_title; }
	std::string GetSummary() const { return this->_summary; }
	std::string GetJumpUrl() const { return this->_JumpUrl; }
	std::string GetPictureUrl() const { return this->_PictureUrl; }
	std::string GetMusicUrl() const { return this->_MusicUrl; }
	std::string GetBrief() const { return this->_brief; }

	void SetKind(MusicKind kind) { this->_kind = kind; }
	void SetTitle(const std::string& title) { this->_title = title; }
	void SetSummary(const std::string& summary) { this->_summary = summary; }
	void SetJumpUrl(const std::string& JumpUrl) { this->_JumpUrl = JumpUrl; }
	void SetPictureUrl(const std::string& PictureUrl) { this->_PictureUrl = PictureUrl; }
	void SetMusicUrl(const std::string& MusicUrl) { this->_MusicUrl = MusicUrl; }
	void SetBrief(const std::string& brief) { this->_brief = brief; }

};


}


#endif