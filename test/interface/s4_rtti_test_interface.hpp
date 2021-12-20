#pragma once

#include <rtti/rtti.hpp>

namespace test {

    enum class DynamicallyLoadedLibEnum {
        UNDEFINED
    };

    class DynamicallyLoadedLibClass {
    public:
        CLASS(test::DynamicallyLoadedLibClass)
        END_CLASS

        virtual ~DynamicallyLoadedLibClass() = default;
    };

    template <typename T>
    class DynamicallyLoadedLibTemplate {
    public:
        TEMPLATE(test::DynamicallyLoadedLibTemplate, <T>)
        END_TEMPLATE

        virtual ~DynamicallyLoadedLibTemplate() = default;
    };

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ITestInterface {
    public:
        virtual ~ITestInterface() = default;

        virtual rtti::EnumPtr get_test_enum_type() const = 0;
        virtual rtti::ClassPtr get_test_class_type() const = 0;
        virtual rtti::ClassPtr get_test_template_type() const = 0;
        virtual rtti::TypePtr get_bool_type() const = 0;

    }; // class ITestInterface

} // namespace test

REGISTER_ENUM(test::DynamicallyLoadedLibEnum)
    ENUM_VALUE(UNDEFINED)
END_ENUM