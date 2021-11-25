#pragma once

#include <cassert>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Fundamental : public Type {
    public:
        Fundamental(std::string name, size_t size)
            : Type(std::move(name), size, {}) {}

        const Fundamental* as_fundamental() const override { return this; }
        const Enum* as_enum() const override { return nullptr; }
        const Class* as_class() const override { return nullptr; }

    }; // class Fundamental

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    class S4_RTTI_EXPORT FundamentalInstance : public Fundamental {
    public:
        FundamentalInstance(std::string name);

        Object new_object() const override;
        Object call_constructor(BufferRef&& buff) const override;
        void delete_object(Object&& obj) const override;
        BufferRef call_destructor(Object&& obj) const override;
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
    Object FundamentalInstance<FUNDAMENTAL>::call_constructor(BufferRef&& buff) const {
        assert(buff.size() == size());
        return Object(new(buff.data().ok()) FUNDAMENTAL);
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::delete_object(Object&& obj) const {
        assert(obj.type() == this);
        delete reinterpret_cast<FUNDAMENTAL*>(obj.m_value);
        obj.m_value = nullptr;
        obj.m_type = nullptr;
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    BufferRef FundamentalInstance<FUNDAMENTAL>::call_destructor(Object&&) const {
        return {};
    }

    //*********************************************************************************************
    template <typename FUNDAMENTAL>
    void FundamentalInstance<FUNDAMENTAL>::copy(ObjectRef& dst, const ObjectRef& src) const {
        *reinterpret_cast<FUNDAMENTAL*>(dst.value()) 
            = *reinterpret_cast<const FUNDAMENTAL*>(src.value());
    }

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