#pragma once

#include <cassert>
#include <vector>
#include <map>

#include <core/str/string_id.hpp>

#include "type.hpp"
#include "buffer.hpp"
#include "object.hpp"

namespace rtti {

    using namespace core::str;

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT ContainerElementRef : public ObjectRef {};

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT ContainerIterator {
    public:
        ContainerElementRef operator*() { return {}; }
        ContainerIterator& operator++() { return *this; }
        bool operator!=(const ContainerIterator& other) const { return false; }
    };
        
    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT Container : public virtual Type {
        // class, c-array, tuple, array
        virtual ContainerIterator begin(ObjectRef& obj) const = 0;
        virtual ContainerIterator end(ObjectRef& obj) const = 0;
        virtual size_t length(const ObjectRef& obj) const = 0;
    }; // class Container

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT DynamicContainer : public virtual Container {
    public:
        // map, set
        virtual void erase(ObjectRef& obj, ContainerIterator& it) const = 0;
        virtual void clear(ObjectRef& obj) const = 0;
        virtual void add_copy(ObjectRef& obj, const ObjectRef& value) const = 0;
        virtual void add_move(ObjectRef& obj, ObjectRef&& value) const = 0;
    }; // class DynamicContainer

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    class S4_RTTI_EXPORT DynamicSequentialContainer : public virtual DynamicContainer {
    public:
        // vector
        virtual void insert_copy(ObjectRef& obj, ContainerIterator& it, const ObjectRef& value) const = 0;
        virtual void insert_move(ObjectRef& obj, ContainerIterator& it, ObjectRef&& value) const = 0;
    }; // class DynamicSequentialContainer

    //*********************************************************************************************
    //*********************************************************************************************
    //*********************************************************************************************
    template <typename TYPE>
    class S4_RTTI_EXPORT ContainerInstance 
        : public virtual TypeInstance<TYPE>
        , public virtual Container 
    {
    public:
    }; // class ContainerInstance

} // namespace rtti