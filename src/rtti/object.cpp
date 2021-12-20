#include "object.hpp"

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
ObjectRef::ObjectRef(void* obj, TypePtr type, size_t size)
    : m_value(obj)
    , m_type(type)
    , m_size(size)
{}

//*************************************************************************************************
ObjectRef::ObjectRef(ObjectRef&& other)
    : m_value(other.m_value)
    , m_type(other.m_type)
    , m_size(other.m_size)
{
    other.m_value = nullptr;
    other.m_type = nullptr;
    other.m_size = 0;
}

//*************************************************************************************************
ObjectRef& ObjectRef::operator=(ObjectRef&& other)
{
    m_value = other.m_value;
    m_type = other.m_type;
    m_size = other.m_size;
    other.m_value = nullptr;
    other.m_type = nullptr;
    other.m_size = 0;
    return *this;
}

//*************************************************************************************************
Res<void*, ObjectRef::ErrValue> ObjectRef::value() {
    if (is_valid())
        return Ok(m_value);
    else
        return Err(ErrValue::INVALID_OBJECT);
}

//*************************************************************************************************
Res<const void*, ObjectRef::ErrValue> ObjectRef::value() const {
    if (is_valid())
        return Ok(const_cast<const void*>(m_value));
    else
        return Err(ErrValue::INVALID_OBJECT);
}

//*************************************************************************************************
Res<void*, ObjectRef::ErrValue> ObjectRef::steal_value() && {
    if (!is_valid())
        return Err(ErrValue::INVALID_OBJECT);

    void* res = m_value;
    m_value = nullptr;
    m_type = nullptr;
    m_size = 0;
    return Ok(res);
}

//*************************************************************************************************
Res<TypePtr, ObjectRef::ErrType> ObjectRef::type() const {
    if (is_valid())
        return Ok(m_type);
    else
        return Err(ErrType::INVALID_OBJECT);
}

//*************************************************************************************************
Res<size_t, ObjectRef::ErrSize> ObjectRef::size() const {
    if (is_valid())
        return Ok(m_size);
    else
        return Err(ErrSize::INVALID_OBJECT);
}

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
Object::~Object() {
    if (is_valid())
        m_type->delete_object(std::move(*this));
}