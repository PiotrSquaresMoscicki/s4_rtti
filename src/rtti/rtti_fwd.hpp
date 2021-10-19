#pragma once

namespace rtti {

    class Type;
    class Class;
    class Enum;
    class Fundamental;

    class ObjectRef;
    class Object;
    class Buffer;

    class Member;
    class Field;
    class Property;
    class Method;

    class Attributes;

    template <typename TYPE>
    inline const Type* static_type() { return TYPE::static_type(); }
    
    template <typename CLASS>
    inline const Type* dynamic_type(CLASS& obj) { return obj.dynamic_type(); }

} // namespace rtti