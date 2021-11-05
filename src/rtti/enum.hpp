#pragma once

#include <vector>
#include <cassert>

#include "type.hpp"
#include "object.hpp"
#include "buffer.hpp"

namespace rtti {

    class EnumValue;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Enum : public Type {
    public:
        Enum(std::string name, size_t size, std::vector<EnumValue> values);

        const std::vector<const EnumValue*>& values() const { return m_values_ptrs; }

        const Fundamental* as_fundamental() const override { return nullptr; }
        const Enum* as_enum() const override { return this; }
        const Class* as_class() const override { return nullptr; }

    private:
        std::vector<EnumValue> m_values;
        std::vector<const EnumValue*> m_values_ptrs;

    }; // class Enum

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT EnumValue {
    public:
        friend Enum;

        EnumValue();
        EnumValue(std::string name, size_t value)
            : m_name(std::move(name)), m_value(value) {}

        const std::string& name() const { return m_name; }
        size_t value() const { return m_value; }
        const Enum* declaring_enum() const { return m_declaring_enum; }

    private:
        void set_declaring_enum(const Enum* declaring_enum) { m_declaring_enum = declaring_enum; }

        std::string m_name;
        size_t m_value = 0;
        const Enum* m_declaring_enum = 0;

    }; // class EnumValue

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename ENUM>
    class EnumInstance : public Enum {
    public:
        EnumInstance(std::string name, std::vector<EnumValue> values);

        Object new_object() const override;
        Object call_constructor(Buffer&& buff) const override;
        void delete_object(Object&& obj) const override;
        Buffer call_destructor(Object&& obj) const override;
        void copy(ObjectRef& dst, const ObjectRef& src) const override;
        Object copy_construct(const ObjectRef& src) const override;
        void move(ObjectRef& dst, ObjectRef& src) const override;
        Object move_construct(ObjectRef& src) const override;

    }; // class EnumInstance

    //*********************************************************************************************
    template <typename ENUM>
    EnumInstance<ENUM>::EnumInstance(std::string name, std::vector<EnumValue> values)
        : Enum(std::move(name), sizeof(ENUM), std::move(values)) 
    {}

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::new_object() const {
        return Object(new ENUM());
    }

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::call_constructor(Buffer&& buff) const {
        assert(buff.size() == size());
        //return Object(new(move_data(buff)) ENUM);
        return {};
    }

    //*********************************************************************************************
    template <typename ENUM>
    void EnumInstance<ENUM>::delete_object(Object&&) const {}

    //*********************************************************************************************
    template <typename ENUM>
    Buffer EnumInstance<ENUM>::call_destructor(Object&&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename ENUM>
    void EnumInstance<ENUM>::copy(ObjectRef&, const ObjectRef&) const {}

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::copy_construct(const ObjectRef&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename ENUM>
    void EnumInstance<ENUM>::move(ObjectRef&, ObjectRef&) const {}

    //*********************************************************************************************
    template <typename ENUM>
    Object EnumInstance<ENUM>::move_construct(ObjectRef&) const {
        return {};
    }

} // namespace rtti