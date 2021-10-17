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
    class Class {
    public:
        Class(std::string name, size_t size, std::unique_ptr<std::vector<Attribute>> attributes) {}

        bool is_instance_of_template(const std::string& template_name) const;
        const std::vector<const Member*> members() const;
        const std::vector<const Method*> methods() const;

        const Class* as_class() const { return this; }
        const Enum* as_enum() const { return nullptr; }
        const Fundamental* as_fundamental() { return nullptr; }

    } // class Class

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename CLASS>
    class ClassInstance : public Class {
    public:
        ClassInstance(std::string name) : Class(std::move(name), sizeof(CLASS)) {}

        Object new_object() const override;
        Object call_constructor(Buffer&& buff) const override;
        void delete_object(Object&& obj) const override;
        Buffer call_destructor(Object&& obj) const override;
        void copy(ObjectRef& dst, const ObjectRef& src) const override;
        Object copy_construct(const ObjectRef& src) const override;
        void move(ObjectRef& dst, ObjectRef& src) const override;
        Object move_construct(ObjectRef& src) const override;

    } // class ClassInstance

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance::new_object() const {
        return Object(new CLASS());
    }

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance::call_constructor(Buffer&& buff) const {
        assert(buff.size() == size());
        return Object(new(move_data(buff)) CLASS);
    }

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance::delete_object(Object&& obj) const {}

    //*********************************************************************************************
    template <typename CLASS>
    Buffer ClassInstance::call_destructor(Object&& obj) const {}

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance::copy(ObjectRef& dst, const ObjectRef& src) const {}

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance::copy_construct(const ObjectRef& src) const {}

    //*********************************************************************************************
    template <typename CLASS>
    void ClassInstance::move(ObjectRef& dst, ObjectRef& src) const {}

    //*********************************************************************************************
    template <typename CLASS>
    Object ClassInstance::move_construct(ObjectRef& src) const {}


} // namespace rtti