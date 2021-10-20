#pragma once

#include <cassert>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class Fundamental : public Type {
    public:
        Fundamental(std::string name, size_t size)
            : Type(std::move(name), size, {}) {}

        const Class* as_class() const override { return nullptr; }
        const Enum* as_enum() const override { return nullptr; }
        const Fundamental* as_fundamental() const override { return this; }

    }; // class Fundamental

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    class FundamentalInstance : public Fundamental {
    public:
        FundamentalInstance(std::string name);

        Object new_object() const override;
        Object call_constructor(Buffer&& buff) const override;
        void delete_object(Object&& obj) const override;
        Buffer call_destructor(Object&& obj) const override;
        void copy(ObjectRef& dst, const ObjectRef& src) const override;
        Object copy_construct(const ObjectRef& src) const override;
        void move(ObjectRef& dst, ObjectRef& src) const override;
        Object move_construct(ObjectRef& src) const override;

    }; // class FundamentalInstance

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    FundamentalInstance<FUNDAMENTAL>::FundamentalInstance(std::string name)
        : Fundamental(std::move(name), sizeof(FUNDAMENTAL)) 
    {}

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::new_object() const {
        return Object(new FUNDAMENTAL());
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::call_constructor(Buffer&& buff) const {
        assert(buff.size() == size());
        //return Object(new(move_data(buff)) FUNDAMENTAL);
        return {};
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::delete_object(Object&&) const {}

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Buffer FundamentalInstance<FUNDAMENTAL>::call_destructor(Object&&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::copy(ObjectRef&, const ObjectRef&) const {}

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::copy_construct(const ObjectRef&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::move(ObjectRef&, ObjectRef&) const {}

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    Object FundamentalInstance<FUNDAMENTAL>::move_construct(ObjectRef&) const {
        return {};
    }

} // namespace rtti