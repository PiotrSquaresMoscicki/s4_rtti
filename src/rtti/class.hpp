#pragma once

#include <cassert>
#include <vector>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Class : public Type {
    public:
        Class(std::string name, size_t size, Attributes attributes) 
            : Type(std::move(name), size, std::move(attributes)) {}

        bool is_instance_of_template(const std::string& template_name) const;
        const std::vector<const Member*>& members() const { return m_members; }
        const std::vector<const Method*>& methods() const { return m_methods; }

        const Fundamental* as_fundamental() const override { return nullptr; }
        const Enum* as_enum() const override { return nullptr; }
        const Class* as_class() const override { return this; }

    private:
        std::vector<const Member*> m_members;
        std::vector<const Method*> m_methods;

    }; // class Class

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS>
    class S4_RTTI_EXPORT ClassInstance : public Class {
    public:
        ClassInstance(std::string name);
        ClassInstance(std::string name, Attributes attributes);

        Object new_object() const override;
        Object call_constructor(Buffer&& buff) const override;
        void delete_object(Object&& obj) const override;
        Buffer call_destructor(Object&& obj) const override;
        void copy(ObjectRef& dst, const ObjectRef& src) const override;
        Object copy_construct(const ObjectRef& src) const override;
        void move(ObjectRef& dst, ObjectRef& src) const override;
        Object move_construct(ObjectRef& src) const override;

    }; // class ClassInstance

    //*********************************************************************************************
    template <typename CLASS>
    ClassInstance<CLASS>::ClassInstance(std::string name)
        : Class(std::move(name), sizeof(CLASS), {}) 
    {}

    //*********************************************************************************************
    template <typename CLASS>
    ClassInstance<CLASS>::ClassInstance(std::string name, Attributes attributes)
        : Class(std::move(name), sizeof(CLASS), std::move(attributes)) 
    {}

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance<CLASS>::new_object() const {
        return Object(new CLASS());
    }

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance<CLASS>::call_constructor(Buffer&& buff) const {
        assert(buff.size() == size());
        //return Object(new(move_data(buff)) CLASS);
        return {};
    }

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance<CLASS>::delete_object(Object&& obj) const {
        assert(obj.type() == this);
        delete reinterpret_cast<CLASS*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename CLASS>
    Buffer ClassInstance<CLASS>::call_destructor(Object&&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance<CLASS>::copy(ObjectRef&, const ObjectRef&) const {}

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance<CLASS>::copy_construct(const ObjectRef&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance<CLASS>::move(ObjectRef&, ObjectRef&) const {}

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance<CLASS>::move_construct(ObjectRef&) const {
        return {};
    }

} // namespace rtti