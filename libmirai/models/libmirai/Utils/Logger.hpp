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

#ifndef MIRAI_LOGGER_HPP_
#define MIRAI_LOGGER_HPP_

#include <atomic>
#include <mutex>
#include <string>
#include <type_traits>

#ifdef TRACE
#undef TRACE
#endif
#ifdef DEBUG
#undef DEBUG
#endif
#ifdef INFO
#undef INFO
#endif
#ifdef WARN
#undef WARN
#endif
#ifdef ERROR
#undef ERROR
#endif
#ifdef FATAL
#undef FATAL
#endif
#ifdef OFF
#undef OFF
#endif

namespace Mirai
{

/// 日志等级
enum class LoggingLevels : uint8_t
{
	TRACE = 0,
	DEBUG,
	INFO,
	WARN,
	ERROR,
	FATAL,
	OFF
};

/**
 * @brief 日志接口类
 * 
 */
class ILogger
{
protected:
	std::atomic<LoggingLevels> level_ = LoggingLevels::INFO;

public:
	ILogger() = default;
	ILogger(LoggingLevels level) : level_(level) {}
	ILogger(const ILogger&) = delete;
	ILogger& operator=(const ILogger&) = delete;
	ILogger(ILogger&&) = delete;
	ILogger& operator=(ILogger&&) = delete;

	/// 设置日志等级
	void SetLoggingLevel(LoggingLevels level) { this->level_ = level; }

	/**
	 * @brief 检查该等级的日志是否会被输出
	 * 
	 * @param level 日志等级
	 * @return `bool`
	 */
	bool CheckLoggingLevel(LoggingLevels level) { return !(this->level_ > level); }

	/**
	 * @brief 输出日志
	 * 
	 * 不推荐直接调用该函数。应该使用对应的输出宏 `LOG_INFO`, `LOG_TRACE` 等先调用 `CheckLoggingLevel` 判断后再调用，
	 * 这样可以防止不必要的字符串计算。
	 * @param msg 日志内容
	 * @param level 日志等级
	 */
	virtual void log(const std::string& msg, LoggingLevels level) = 0;
	virtual ~ILogger() = default;
};

/**
 * @brief 什么都不干的日志类
 * 
 */
class NullLogger : public ILogger
{
public:
	NullLogger() : ILogger(LoggingLevels::OFF) {}
	void log(const std::string& /*msg*/, LoggingLevels /*level*/) override {}
};

} // namespace Mirai

#define MIRAI_LOGGING_LEVELS_TRACE 0
#define MIRAI_LOGGING_LEVELS_DEBUG 1
#define MIRAI_LOGGING_LEVELS_INFO 2
#define MIRAI_LOGGING_LEVELS_WARN 3
#define MIRAI_LOGGING_LEVELS_ERROR 4
#define MIRAI_LOGGING_LEVELS_FATAL 5
#define MIRAI_LOGGING_LEVELS_OFF 6

#ifndef MIRAI_LOGGING_LEVEL
#define MIRAI_LOGGING_LEVEL MIRAI_LOGGING_LEVELS_INFO
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_TRACE
#define LOG_TRACE(logger, msg)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::TRACE))                                                 \
			(logger).log((msg), Mirai::LoggingLevels::TRACE);                                                      \
	} while (0)
#else
#define LOG_TRACE(logger, msg) (void)0
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_DEBUG
#define LOG_DEBUG(logger, msg)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::DEBUG))                                                 \
			(logger).log((msg), Mirai::LoggingLevels::DEBUG);                                                      \
	} while (0)
#else
#define LOG_DEBUG(logger, msg) (void)0
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_INFO
#define LOG_INFO(logger, msg)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::INFO))                                                  \
			(logger).log((msg), Mirai::LoggingLevels::INFO);                                                       \
	} while (0)
#else
#define LOG_INFO(logger, msg) (void)0
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_WARN
#define LOG_WARN(logger, msg)                                                                                      \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::WARN))                                                  \
			(logger).log((msg), Mirai::LoggingLevels::WARN);                                                       \
	} while (0)
#else
#define LOG_WARN(logger, msg) (void)0
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_ERROR
#define LOG_ERROR(logger, msg)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::ERROR))                                                 \
			(logger).log((msg), Mirai::LoggingLevels::ERROR);                                                      \
	} while (0)
#else
#define LOG_ERROR(logger, msg) (void)0
#endif

#if MIRAI_LOGGING_LEVEL <= MIRAI_LOGGING_LEVELS_FATAL
#define LOG_FATAL(logger, msg)                                                                                     \
	do                                                                                                                 \
	{                                                                                                                  \
		if ((logger).CheckLoggingLevel(Mirai::LoggingLevels::FATAL))                                                 \
			(logger).log((msg), Mirai::LoggingLevels::FATAL);                                                      \
	} while (0)
#else
#define LOG_FATAL(logger, msg) (void)0
#endif

#endif