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
        /// Type of a method that allow to set the option.
        typedef std::function<void(T)> setter_type;

        /// Type of a method that allow to read the option.
        typedef std::function<T()> getter_type;

        /// Constructor.
        ///
        /// @param name         Name of the options.
        /// @param description  Description of the option.
        /// @param setter       Function used to set the option.
        /// @param getter       Function used to read the option.
        TypedOption(const std::string& name, const std::string& description, setter_type setter, getter_type getter)
            : Option(name, description), setter_(setter), getter_(getter)
        {}

        T value() const { return value_; }
        void set_value(T value) { value_ = value; }

        std::string ToString() const { return boost::lexical_cast<std::string>(getter_()); }
        void set_value(const std::string& value) { setter_(boost::lexical_cast<T>(value)); };

    private:
        setter_type setter_;
        getter_type getter_;
    };
}

#endif // M8_OPTION_HPP_