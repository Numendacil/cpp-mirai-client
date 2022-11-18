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
#include <utility>

#include "MessageBase.hpp"

#include <libmirai/Types/BasicTypes.hpp>

namespace Mirai
{

/**
 * @brief 音乐分享卡片消息
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `MusicShareMessage::_kind` | `MusicShareType::ENUM_END`
 * `MusicShareMessage::_title` | `""`
 * `MusicShareMessage::_summary` | `""`
 * `MusicShareMessage::_JumpUrl` | `""`
 * `MusicShareMessage::_PictureUrl` | `""`
 * `MusicShareMessage::_MusicUrl` | `""`
 * `MusicShareMessage::_brief` | `""`
 */
class MusicShareMessage : public MessageBase
{
protected:
	MusicShareType _kind = MusicShareType::ENUM_END;
	std::string _title{};
	std::string _summary{};
	std::string _JumpUrl{};
	std::string _PictureUrl{};
	std::string _MusicUrl{};
	std::string _brief{};


	void Serialize(void*) const final;
	void Deserialize(const void*) final;

public:
	MusicShareMessage() : MessageBase(_TYPE_) {}

	MusicShareMessage(MusicShareType kind, std::string title, std::string summary, std::string JumpUrl,
	                  std::string PictureUrl, std::string MusicUrl, std::string brief)
		: _kind(kind)
		, _title(std::move(title))
		, _summary(std::move(summary))
		, _JumpUrl(std::move(JumpUrl))
		, _PictureUrl(std::move(PictureUrl))
		, _MusicUrl(std::move(MusicUrl))
		, _brief(std::move(brief))
		, MessageBase(_TYPE_)
	{
	}

	static constexpr MessageTypes _TYPE_ = MessageTypes::MUSIC_SHARE;

	std::unique_ptr<MessageBase> CloneUnique() const final { return std::make_unique<MusicShareMessage>(*this); }

	bool isValid() const final
	{
		return !(this->_kind == MusicShareType::ENUM_END || this->_title.empty() || this->_summary.empty()
	         || this->_JumpUrl.empty() || this->_PictureUrl.empty() || this->_MusicUrl.empty() || this->_brief.empty());
	}

	/// 获取分享种类
	MusicShareType GetKind() const { return this->_kind; }
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
	MusicShareMessage& SetKind(MusicShareType kind)
	{
		this->_kind = kind;
		return *this;
	}
	/// 设置标题
	MusicShareMessage& SetTitle(std::string title)
	{
		this->_title = std::move(title);
		return *this;
	}
	/// 设置介绍
	MusicShareMessage& SetSummary(std::string summary)
	{
		this->_summary = std::move(summary);
		return *this;
	}
	/// 设置转跳链接
	MusicShareMessage& SetJumpUrl(std::string JumpUrl)
	{
		this->_JumpUrl = std::move(JumpUrl);
		return *this;
	}
	/// 设置封面图片连接
	MusicShareMessage& SetPictureUrl(std::string PictureUrl)
	{
		this->_PictureUrl = std::move(PictureUrl);
		return *this;
	}
	/// 设置音乐链接
	MusicShareMessage& SetMusicUrl(std::string MusicUrl)
	{
		this->_MusicUrl = std::move(MusicUrl);
		return *this;
	}
	/// 设置简介
	MusicShareMessage& SetBrief(std::string brief)
	{
		this->_brief = std::move(brief);
		return *this;
	}
};

template <>
struct GetType<MusicShareMessage::_TYPE_>
{
	using type = MusicShareMessage;
};


} // namespace Mirai


#endif