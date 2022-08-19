// Copyright (C) 2022 Numendacil and contributors
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _MIRAI_MUSIC_SHARE_MESSAGE_HPP_
#define _MIRAI_MUSIC_SHARE_MESSAGE_HPP_

#include <array>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 音乐分享卡片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `MusicShareMessage::_kind` | `MusicKind::UNKNOWN`
 * `MusicShareMessage::_title` | `""`
 * `MusicShareMessage::_summary` | `""`
 * `MusicShareMessage::_JumpUrl` | `""`
 * `MusicShareMessage::_PictureUrl` | `""`
 * `MusicShareMessage::_MusicUrl` | `""`
 * `MusicShareMessage::_brief` | `""`
 */
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


	static constexpr std::array<std::string_view, static_cast<std::size_t>(MusicKind::UNKNOWN)> _MusicKindStr = {
		"NeteaseCloudMusic", "QQMusic", "MiguMusic", "KugouMusic", "KuwoMusic"};

	static constexpr std::string_view _to_string(const MusicKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _MusicKindStr.size()) return _MusicKindStr[i];
		else
			return "";
	}

	static constexpr MusicKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _MusicKindStr.size(); i++)
			if (_MusicKindStr[i] == s) return static_cast<MusicKind>(i);

		return MusicKind::UNKNOWN;
	}

public:
	static constexpr std::string_view _TYPE_ = "MusicShare";

	MusicShareMessage() {}
	MusicShareMessage(MusicKind kind, const std::string& title, const std::string& summary, const std::string& JumpUrl,
	                  const std::string& PictureUrl, const std::string& MusicUrl, const std::string& brief)
		: _kind(kind)
		, _title(title)
		, _summary(summary)
		, _JumpUrl(JumpUrl)
		, _PictureUrl(PictureUrl)
		, _MusicUrl(MusicUrl)
		, _brief(brief)
	{
	}
	MusicShareMessage(const MusicShareMessage&) = default;
	MusicShareMessage& operator=(const MusicShareMessage&) = default;
	MusicShareMessage(MusicShareMessage&&) noexcept = default;
	MusicShareMessage& operator=(MusicShareMessage&&) noexcept = default;

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual MusicShareMessage* Clone() const override { return new MusicShareMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	/// 获取分享种类
	MusicKind GetKind() const { return this->_kind; }
	/// 获取标题
	std::string GetTitle() const { return this->_title; }
	/// 获取介绍
	std::string GetSummary() const { return this->_summary; }
	/// 获取转跳链接
	std::string GetJumpUrl() const { return this->_JumpUrl; }
	/// 获取封面图片连接
	std::string GetPictureUrl() const { return this->_PictureUrl; }
	/// 获取音乐链接
	std::string GetMusicUrl() const { return this->_MusicUrl; }
	/**
	 * @brief 获取简介
	 * 
	 * 简介为未打开会话窗口时显示的简要文字消息
	 */
	std::string GetBrief() const { return this->_brief; }

	/// 设置分享种类
	MusicShareMessage& SetKind(MusicKind kind)
	{
		this->_kind = kind;
		return *this;
	}
	/// 设置标题
	MusicShareMessage& SetTitle(const std::string& title)
	{
		this->_title = title;
		return *this;
	}
	/// 设置介绍
	MusicShareMessage& SetSummary(const std::string& summary)
	{
		this->_summary = summary;
		return *this;
	}
	/// 设置转跳链接
	MusicShareMessage& SetJumpUrl(const std::string& JumpUrl)
	{
		this->_JumpUrl = JumpUrl;
		return *this;
	}
	/// 设置封面图片连接
	MusicShareMessage& SetPictureUrl(const std::string& PictureUrl)
	{
		this->_PictureUrl = PictureUrl;
		return *this;
	}
	/// 设置音乐链接
	MusicShareMessage& SetMusicUrl(const std::string& MusicUrl)
	{
		this->_MusicUrl = MusicUrl;
		return *this;
	}
	/// 设置简介
	MusicShareMessage& SetBrief(const std::string& brief)
	{
		this->_brief = brief;
		return *this;
	}
};


} // namespace Mirai


#endif