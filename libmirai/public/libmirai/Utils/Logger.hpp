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

#ifndef _MIRAI_LOGGER_HPP_
#define _MIRAI_LOGGER_HPP_

#include <atomic>
#include <mutex>
#include <string>
#include <type_traits>

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
	std::atomic<LoggingLevels> _level = LoggingLevels::INFO;

public:
	ILogger() = default;
	ILogger(LoggingLevels level) : _level(level) {}
	ILogger(const ILogger&) = delete;
	ILogger& operator=(const ILogger&) = delete;
	ILogger(ILogger&&) = delete;
	ILogger& operator=(ILogger&&) = delete;

	/// 设置日志等级
	void SetLoggingLevel(LoggingLevels level) { this->_level = level; }

	/**
	 * @brief 检查该等级的日志是否会被输出
	 * 
	 * @param level 日志等级
	 * @return `bool`
	 */
	bool CheckLoggingLevel(LoggingLevels level) { return !(this->_level > level);}

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

}

#define MIRAI_LOGGING_LEVELS_TRACE 0
#define MIRAI_LOGGING_LEVELS_DEBUG 1
#define MIRAI_LOGGING_LEVELS_INFO 2
#define MIRAI_LOGGING_LEVELS_WARN 3
#define MIRAI_LOGGING_LEVELS_ERROR 4
#define MIRAI_LOGGING_LEVELS_FATAL 5
#define MIRAI_LOGGING_LEVELS_OFF 6

#ifndef MIRAI_LOGGING_LEVELS
#define MIRAI_LOGGING_LEVELS MIRAI_LOGGING_LEVELS_INFO
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_TRACE
	#define LOG_TRACE(_logger_, _msg_)	\
	do	\
	{	\
		if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::TRACE))	\
			(_logger_).log((_msg_), Mirai::LoggingLevels::TRACE);	\
	} while(0)
#else
	#define LOG_TRACE(_logger_, _msg_) (void)0
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_DEBUG
#define LOG_DEBUG(_logger_, _msg_)	\
do	\
{	\
	if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::DEBUG))	\
		(_logger_).log((_msg_), Mirai::LoggingLevels::DEBUG);	\
} while(0)
#else
	#define LOG_DEBUG(_logger_, _msg_) (void)0
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_INFO
#define LOG_INFO(_logger_, _msg_)	\
do	\
{	\
	if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::INFO))	\
		(_logger_).log((_msg_), Mirai::LoggingLevels::INFO);	\
} while(0)
#else
	#define LOG_INFO(_logger_, _msg_) (void)0
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_WARN
#define LOG_WARN(_logger_, _msg_)	\
do	\
{	\
	if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::WARN))	\
		(_logger_).log((_msg_), Mirai::LoggingLevels::WARN);	\
} while(0)
#else
	#define LOG_WARN(_logger_, _msg_) (void)0
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_ERROR
#define LOG_ERROR(_logger_, _msg_)	\
do	\
{	\
	if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::ERROR))	\
		(_logger_).log((_msg_), Mirai::LoggingLevels::ERROR);	\
} while(0)
#else
	#define LOG_ERROR(_logger_, _msg_) (void)0
#endif

#if MIRAI_LOGGING_LEVELS <= MIRAI_LOGGING_LEVELS_FATAL
#define LOG_FATAL(_logger_, _msg_)	\
do	\
{	\
	if ((_logger_).CheckLoggingLevel(Mirai::LoggingLevels::FATAL))	\
		(_logger_).log((_msg_), Mirai::LoggingLevels::FATAL);	\
} while(0)
#else
	#define LOG_FATAL(_logger_, _msg_) (void)0
#endif

#endif