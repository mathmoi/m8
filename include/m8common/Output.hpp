/// @file Output.hpp
/// @author Mathieu Pagé
/// @date February 2024
/// @brief Contains the class Output

#include <iostream>
#include <mutex>
#include <sstream>

#include "logging.hpp"

#ifndef M8_OUTPUT_HPP_
#define M8_OUTPUT_HPP_

namespace m8
{
    /// This implement and stream-like class that allows m8 to output on the std::cout
    /// stream and the log system in a thread-safe way.
    class Output
    {
    public:
        /// Add any data in the internal stream
        template<typename T>
        Output& operator<<(const T& data) {
            buffer_ <<data;
            return *this;
        }

        /// Overloaded insertion operator to handle manipulators like std::endl and 
        /// std::flush.
        Output& operator<<(std::ostream& (*manip)(std::ostream&)) {
            if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush)
                || manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
                std::lock_guard<std::mutex> lock(mutex);

                std::cout <<buffer_.str() <<manip;
                M8_OUTPUT <<buffer_.str() <<manip;

                buffer_.str("");
                buffer_.clear();
            } else {
                buffer_ << manip;
            }
            return *this;
        }

        /// Flush the buffer and ensure thread safe output.
        void flush() {
            auto data = buffer_.str();
            if (!data.empty())
            {
                std::lock_guard<std::mutex> lock(mutex);

                std::cout <<buffer_.str();
                std::cout.flush();
                M8_OUTPUT <<buffer_.str();

                buffer_.str("");
                buffer_.clear();
            }
        }

        /// On destruction if we have any data in the buffer we need to output it.
        ~Output()
        {
            flush();
        }

    private:
        static std::mutex mutex;
        std::ostringstream buffer_;
    };
}

#endif // M8_OUTPUT_HPP_