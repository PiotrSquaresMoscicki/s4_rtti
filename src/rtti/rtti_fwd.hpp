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

    template <typename TYPE>
    const Type* get_type();

} // namespace rtti