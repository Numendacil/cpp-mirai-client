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

#ifndef _MIRAI_POKE_MESSAGE_HPP_
#define _MIRAI_POKE_MESSAGE_HPP_

#include <array>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include "MessageBase.hpp"

namespace Mirai
{

/**
 * @brief 戳一戳消息
 *
 * 原PC版窗口抖动消息，与头像戳一戳消息不同
 *
 * Member Variable | Default Value
 * --------------- | -------------
 * `PokeMessage::_kind` | `PokeKind::UNKNOWN`
 */
class PokeMessage : public MessageBase
{
public:
	enum PokeKind : std::size_t
	{
		CHUOYICHUO = 0, // 戳一戳
		BIXIN,          // 比心
		DIANZAN,        // 点赞
		XINSUI,         // 心碎
		LIULIULIU,      // 666
		FANGDAZHAO,     // 放大招
		GOUYIN,         // 勾引
		BAOBEIQIU,      // 宝贝球 (SVIP)
		ROSE,           // 玫瑰花 (SVIP)
		ZHAOHUANSHU,    // 召唤术 (SVIP)
		RANGNIPI,       // 让你皮 (SVIP)
		JIEYIN,         // 结印 (SVIP)
		SHOULEI,        // 手雷 (SVIP)
		ZHUAYIXIA,      // 抓一下 (SVIP)
		SUIPING,        // 碎屏 (SVIP)
		QIAOMEN,        // 敲门 (SVIP)
		UNKNOWN
	};

protected:
	PokeKind _kind;


	static constexpr std::array<std::string_view, static_cast<std::size_t>(PokeKind::UNKNOWN)> _PokeKindStr = {
		"ChuoYiChuo", "BiXin",       "DianZan",  "XinSui", "LiuLiuLiu", "FangDaZhao", "GouYin",  "BaoBeiQiu",
		"Rose",       "ZhaoHuanShu", "RangNiPi", "JieYin", "ShouLei",   "ZhuaYiXia",  "SuiPing", "QiaoMen"};

	static constexpr std::string_view _to_string(const PokeKind& m)
	{
		auto i = static_cast<std::size_t>(m);
		if (i < _PokeKindStr.size()) return _PokeKindStr[i];
		else
			return "";
	}

	static constexpr PokeKind _to_enum(std::string_view s)
	{
		for (std::size_t i = 0; i < _PokeKindStr.size(); i++)
			if (_PokeKindStr[i] == s) return static_cast<PokeKind>(i);

		return PokeKind::UNKNOWN;
	}

public:
	PokeMessage() {}
	PokeMessage(PokeKind kind) : _kind(kind) {}
	PokeMessage(const PokeMessage&) = default;
	PokeMessage& operator=(const PokeMessage&) = default;
	PokeMessage(PokeMessage&&) noexcept = default;
	PokeMessage& operator=(PokeMessage&&) noexcept = default;


	static constexpr std::string_view _TYPE_ = "Poke";

	virtual std::string_view GetType() const override { return _TYPE_; }

	virtual PokeMessage* Clone() const override { return new PokeMessage(*this); }

	virtual bool isValid() const override;

	virtual void FromJson(const nlohmann::json& data) override;
	virtual nlohmann::json ToJson() const override;

	bool operator==(const PokeMessage& rhs) { return this->_kind == rhs._kind; }

	bool operator!=(const PokeMessage& rhs) { return !(*this == rhs); }

	/// 获取戳一戳类型
	PokeKind GetPokeKind() const { return this->_kind; }

	/// 设置戳一戳类型
	PokeMessage& SetPokeKind(PokeKind kind)
	{
		this->_kind = kind;
		return *this;
	}
};

} // namespace Mirai


#endif