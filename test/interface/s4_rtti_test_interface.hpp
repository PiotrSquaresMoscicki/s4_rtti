#pragma once

namespace rtti {
    class Type;
}

namespace test {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class ITestInterface {
    public:
        virtual ~ITestInterface() = default;

        virtual const rtti::Type* get_bool_type() const = 0;

    }; // class ITestInterface

} // namespace test