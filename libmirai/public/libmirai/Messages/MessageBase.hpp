#ifndef _MIRAI_MESSAGE_BASE_HPP_
#define _MIRAI_MESSAGE_BASE_HPP_


#include <memory>
#include <string>

#include <nlohmann/json_fwd.hpp>

#include <libmirai/Types/Serializable.hpp>

namespace Mirai
{

/**
 * @brief Base class for all message types
 * 
 * 所有有效的派生类均需要定义 `_TYPE_`，对应于mirai-api-http传来的JSON消息中的"type"属性，并在 `GetType()` 中返回 `_TYPE_`
 *
 * TODO: Consider switching to typeid?
 */
class MessageBase : public Serializable
{
public:
	// static constexpr std::string_view _TYPE_ = "MessageBase";

	/**
	 * @brief Return the type of the class
	 * 
	 * Used for RTTI and message parsing, it should always return
	 * the value of the static `_TYPE_` member
	 */
	virtual std::string_view GetType() const = 0;

	/// Clone the class, used for copying polymorphic objects
	virtual MessageBase* Clone() const = 0;

	/// Wrapper of `Clone()` using smart pointers, recommmended over `Clone()`
	virtual std::unique_ptr<MessageBase> CloneUnique() const { return std::unique_ptr<MessageBase>(this->Clone()); }

	/**
	 * @brief 检查消息是否有效
	 * 
	 * 发送无效消息( `isValid() = false` )会导致mirai-api-http返回400或500错误，并抛出异常。
	 * 检测的内容靠测试经验和mirai-api-http源码确定， `isValid() = true` 不保证一定能发送成功。
	 * @return `bool`
	 */
	virtual bool isValid() const = 0;

	virtual ~MessageBase() = default;
};

} // namespace Mirai


#endif