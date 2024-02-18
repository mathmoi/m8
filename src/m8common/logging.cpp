/// @file   logging.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2018
/// @brief  Contains functionalities to log informations to a log file.

#ifdef _MSC_VER
#   include <process.h>
#endif // _MSC_VER

#ifdef __GNUC__
#   include <unistd.h>
#endif //  __GNUC__

#pragma warning(push)
#pragma warning(disable: 4244)

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>

#pragma warning (pop)

#include "m8common/options/Options.hpp"
#include "m8common/logging.hpp"
#include "m8common/Utils.hpp"

namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;

namespace m8
{
    BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", attrs::current_thread_id::value_type)
    BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)

    std::ostream& operator<<(std::ostream& strm, severity_level level)
    {
        static const std::map<m8::severity_level, std::string> map =
        {
            { m8::severity_level::none, "none" },
            { m8::severity_level::fatal, "fatal" },
            { m8::severity_level::error, "error" },
            { m8::severity_level::warning, "warning" },
            { m8::severity_level::info, "info" },
            { m8::severity_level::output, "output" },
            { m8::severity_level::input, "input" },
            { m8::severity_level::debug, "debug" },
            { m8::severity_level::trace, "trace" }
        };

        auto it = map.find(level);
        if (it != map.end())
            strm << it->second;
        else
            strm << static_cast< int >(level);

        return strm;
    }

    bool FilterLogRecord(logging::value_ref< severity_level, tag::severity > const& level)
    {
        return level <= options::Options::get().max_log_severity;
    }

    void FormatLogRecord(logging::record_view const& rec, logging::formatting_ostream& strm)
    {
        static const std::map<const m8::severity_level, char> map_severity =
        {
            { m8::severity_level::none,    '_' },
            { m8::severity_level::fatal,   'F' },
            { m8::severity_level::error,   'E' },
            { m8::severity_level::warning, 'W' },
            { m8::severity_level::info,    'I' },
            { m8::severity_level::output,  '<' },
            { m8::severity_level::input,   '>' },
            { m8::severity_level::debug,   'D' },
            { m8::severity_level::trace,   'T' }
        };

        auto date_formater = expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f ");

        date_formater(rec, strm);
        strm << rec[thread_id] <<' ';
        strm << map_severity.at(rec[severity].get()) <<' ';
        strm << rec[expr::smessage];
    }

    //Defines a global logger initialization routine
    BOOST_LOG_GLOBAL_LOGGER_INIT(logger, LoggerType)
    {
        LoggerType lg;

        logging::add_common_attributes();

#ifdef _MSC_VER
        auto pid = _getpid();
#endif // _MSC_VER

#ifdef __GNUC__
        auto pid = getpid();
#endif //  __GNUC__

        boost::shared_ptr<sinks::text_file_backend> backend =
            boost::make_shared<sinks::text_file_backend>
            (
                keywords::file_name = std::string("m8_%Y%m%d%H%M%S.") + ToString(pid) + ".log"
            );

        // Create the sink to a log file
        typedef sinks::synchronous_sink<sinks::text_file_backend> text_sink;
        boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>(backend);

        sink->set_formatter(&FormatLogRecord);

        namespace phoenix = boost::phoenix;
        sink->set_filter(phoenix::bind(&FilterLogRecord, severity.or_none()));

        logging::core::get()->add_sink(sink);

        return lg;
    }
}

