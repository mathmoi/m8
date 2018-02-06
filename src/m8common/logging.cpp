/// @file   logging.cpp
/// @author Mathieu Pagé
/// @date   Feburary 2018
/// @brief  Contains functionalities to log informations to a log file.


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

#include "../m8/options/Options.hpp"

#include "logging.hpp"

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
        static const char values[] = "FEWI<>DT";

        if (static_cast< std::size_t >(level) < (sizeof(values) - 1) / sizeof(char))
            strm << values[level];
        else
            strm << static_cast< int >(level);

        return strm;
    }

    bool FilterLogRecord(logging::value_ref< severity_level, tag::severity > const& level)
    {
        return level <= Options::get().max_log_severity().value();
    }

    void FormatLogRecord(logging::record_view const& rec, logging::formatting_ostream& strm)
    {
        auto date_formater = expr::stream << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S.%f ");

        date_formater(rec, strm);
        strm << rec[thread_id] <<' ';
        strm << rec[severity] <<' ';
        strm << rec[expr::smessage];
    }

    //Defines a global logger initialization routine
    BOOST_LOG_GLOBAL_LOGGER_INIT(logger, logger_type)
    {
        logger_type lg;

        logging::add_common_attributes();

        boost::shared_ptr<sinks::text_file_backend> backend =
            boost::make_shared<sinks::text_file_backend>
            (
                keywords::file_name = "m8_%Y%m%d%H%M%S.%N.log"
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

