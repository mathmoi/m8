/// @file ModifiableOption.hpp
/// @author Mathieu Pagé (m@mathieupage.com)
/// @date November 2022
/// @brief Contains a class that represents an option that can be modified at runtime

#ifndef M8_OPTIONS_MODIFICABLE_OPTION_HPP_
#define M8_OPTIONS_MODIFICABLE_OPTION_HPP_

#include <string>
#include <functional>

namespace m8::options
{
    /// Represent an option
    class ModifiableOption
    {
    public:
        /// Constructor
        ModifiableOption(const std::string& name, const std::string& description)
            : name_(name), description_(description)
        {}

        std::string name() const { return name_; }
        std::string description() const { return description_; }

        virtual std::string ToString() const = 0;
        virtual void set_value(std::string_view value) = 0;

    private:
        std::string name_;
        std::string description_;
    };

    template<class T>
    class TypedModifiableOption : public ModifiableOption
    {
    public:
        /// Constructor.
        ///
        /// @param name          Name of the options.
        /// @param description   Description of the option.
        /// @param value_storage Reference to an emplacement where the value of this 
        ///                      option is stored
        TypedModifiableOption(const std::string& name, const std::string& description, T& value_storage)
            : ModifiableOption(name, description), value_storage_(value_storage)
        {}

        /// Return the option's value
        std::string ToString() const { return boost::lexical_cast<std::string>(value_storage_); }

        /// Set the value of the option
        void set_value(std::string_view value) { value_storage_ = boost::lexical_cast<T>(std::string(value)); };

    private:
        T& value_storage_;
    };

    template<>
    class TypedModifiableOption<bool> : public ModifiableOption
    {
    public:
        /// Constructor.
        ///
        /// @param name          Name of the options.
        /// @param description   Description of the option.
        /// @param value_storage Reference to an emplacement where the value of this 
        ///                      option is stored
        TypedModifiableOption(const std::string& name, const std::string& description, bool& value_storage)
            : ModifiableOption(name, description), value_storage_(value_storage)
        {}

        /// Return the option's value
        std::string ToString() const { return boost::lexical_cast<std::string>(value_storage_); }

        /// Set the value of the option
        void set_value(std::string_view value)
        {
            if (value == "true")
            {
                value_storage_ = true;
                return;
            }

            if (value == "false")
            {
                value_storage_ = false;
                return;
            }

            value_storage_ = boost::lexical_cast<bool>(std::string(value));
        };

    private:
        bool& value_storage_;
    };
}

#endif // M8_OPTIONS_MODIFICABLE_OPTION_HPP_