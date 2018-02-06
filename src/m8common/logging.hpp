/// @file   logging.hpp
/// @author Mathieu Pag�
/// @date   Feburary 2018
/// @brief  Contains functionalities to log informations to a log file.

#ifndef M8_LOGGING_HPP_
#define M8_LOGGING_HPP_

#include <boost/thread/thread.hpp>

#include <boost/log/trivial.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/logger.hpp>

// This should be "called" at the begining of each thread to read the ThreadID for 
// logging purposes.
#define M8_LOG_SCOPE_THREAD() \
            BOOST_LOG_SCOPED_THREAD_TAG("ThreadId", boost::this_thread::get_id());



namespace m8
{
#define M8_SEVERITY_TRACE       7
#define M8_SEVERITY_DEBUG       6
#define M8_SEVERITY_INPUT       5
#define M8_SEVERITY_OUTPUT      4
#define M8_SEVERITY_INFO        3
#define M8_SEVERITY_WARNING     2
#define M8_SEVERITY_ERROR       1
#define M8_SEVERITY_FATAL       0

    enum severity_level
    {
        fatal   = M8_SEVERITY_FATAL,
        error   = M8_SEVERITY_ERROR,
        warning = M8_SEVERITY_WARNING,
        info    = M8_SEVERITY_INFO,
        output  = M8_SEVERITY_OUTPUT,
        input   = M8_SEVERITY_INPUT,
        debug   = M8_SEVERITY_DEBUG,
        trace   = M8_SEVERITY_TRACE
    };

    typedef boost::log::sources::severity_logger_mt<severity_level> LoggerType;

    BOOST_LOG_GLOBAL_LOGGER(logger, LoggerType)

    std::ostream& operator<<(std::ostream& out, severity_level level);
}

#define M8_LOG(level)   BOOST_LOG_SEV(m8::logger::get(), level)

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_TRACE
#   define M8_TRACE        M8_LOG(::m8::trace)
#else
#   define M8_TRACE        if (false) M8_LOG(::m8::trace)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_DEBUG
#   define M8_DEBUG        M8_LOG(::m8::debug)
#else
#   define M8_DEBUG        if (false) M8_LOG(::m8::debug)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_INPUT
#   define M8_INPUT        M8_LOG(::m8::input)
#else
#   define M8_INPUT        if (false) M8_LOG(::m8::input)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_OUTPUT
#   define M8_OUTPUT       M8_LOG(::m8::output)
#else
#   define M8_OUTPUT       if (false) M8_LOG(::m8::output)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_INFO
#   define M8_INFO         M8_LOG(::m8::info)
#else
#   define M8_INFO         if (false) M8_LOG(::m8::info)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_WARNING
#   define M8_WARNING      M8_LOG(::m8::warning)
#else
#   define M8_WARNING      if (false) M8_LOG(::m8::warning)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_ERROR
#   define M8_ERROR        M8_LOG(::m8::error)
#else
#   define M8_ERROR        if (false) M8_LOG(::m8::error)
#endif

#if M8_MAX_LOG_SEVERITY >= M8_SEVERITY_FATAL
#   define M8_FATAL        M8_LOG(::m8::fatal)
#else
#   define M8_FATAL        if (false) M8_LOG(::m8::fatal)
#endif

#endif // M8_LOGGING_HPP_
