/// @file   Option.hpp
/// @author Mathieu Pagé
/// @date   January 2017
/// @brief  Contains the declarations of the Option class that represent a single option.

#ifndef M8_OPTION_HPP_
#define M8_OPTION_HPP_

#include <string>

#include <boost/lexical_cast.hpp>

namespace m8
{
    /// Represent an option
    class Option
    {
    public:
        /// Constructor
        Option(const std::string& name, const std::string& description)
            : name_(name), description_(description)
        {}

        std::string name() const { return name_; }
        std::string description() const { return description_; }

        virtual std::string ToString() const = 0;
        virtual void set_value(const std::string& value) = 0;

    private:
        std::string name_;
        std::string description_;
    };

    template<class T>
    class TypedOption : public Option
    {
    public:
        /// Constructor
        TypedOption(const std::string& name, const std::string& description, T value)
            : Option(name, description), value_(value)
        {}

        T value() const { return value_; }
        void set_value(T value) { value_ = value; }

        std::string ToString() const { return boost::lexical_cast<std::string>(value_); }
        void set_value(const std::string& value) { value_ = boost::lexical_cast<T>(value); };

    private:
        T value_;
    };
}

#endif // M8_OPTION_HPP_