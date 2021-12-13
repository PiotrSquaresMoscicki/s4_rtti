#pragma once

#include "src/s4_rtti_export.h"

namespace rtti {

    class Type;
    class Enum;
    class Fundamental;
    class Class;
    class TemplateInstance;

    class ObjectRef;
    class Object;
    class BufferRef;
    class Buffer;

    class Member;
    class Field;
    class Property;
    class Method;

    class Attributes;

    template <typename TYPE>
    inline const Type* static_type() { return TYPE::static_class(); }

    template <typename CLASS>
    inline const Class* static_class() { return CLASS::static_class(); }
    
    template <typename CLASS>
    inline const Class* dynamic_class(CLASS& obj) { return obj.dynamic_class(); }

    template <typename CLASS>
    void initialize_class(Class& instance);

} // namespace rtti