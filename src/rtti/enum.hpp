#pragma once

#include <vector>

#include "type.hpp"

namespace rtti {

    class EnumValue;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Enum : public: Type {
    public:
        Enum(std::string name, size_t size, std::vector<const EnumValue*> values)
            : Type(std::move(name), size {}), m_values(std::move(values)) {}

        const std::vector<const EnumValue*>& values() const { return m_values; }

        const Fundamental* as_fundamental() const override { return nullptr; }
        const Enum* as_enum() const override { return this; }
        const Class* as_class() const override { return nullptr; }

    private:
        std::vector<const EnumValue*> m_values;

    }; // class Enum

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class EnumValue {
    public:
        EnumValue(std::string name, size_t value, const Enum* declaring_enum)
            : m_name(std::move(name)), m_value(value), m_declaring_enum(declaring_enum) {}

        const std::string& name() const { return m_name; }
        size_t value() const { return m_value; }
        const Enum* declaring_enum() const { return m_declaring_enum; }

    private:
        std::string m_name;
        size_t m_value = 0;
        const Enum* m_declaring_enum = 0;

    }; // class EnumValue

    template <typename ENUM>
    class EnumInstance : public Enum {
    public:
        ClassInstance(std::string name, ENUM... values);

        Object new_object() const override;
        Object call_constructor(Buffer&& buff) const override;
        void delete_object(Object&& obj) const override;
        Buffer call_destructor(Object&& obj) const override;
        void copy(ObjectRef& dst, const ObjectRef& src) const override;
        Object copy_construct(const ObjectRef& src) const override;
        void move(ObjectRef& dst, ObjectRef& src) const override;
        Object move_construct(ObjectRef& src) const override;
    }

} // namespace rtti