#include "object.hpp"

#include "type.hpp"

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
ObjectRef::ObjectRef(void* obj, const Type* type)
    : m_value(obj)
    , m_type(type)
{}

//*************************************************************************************************
Res<void*, ErrValue> ObjectRef::value() {
    if (is_valid())
        return Ok(m_value);
    else
        return Err(ErrValue::INVALID_OBJECT);
}

//*************************************************************************************************
Res<const void*, ErrValue> ObjectRef::value() const {
    if (is_valid())
        return Ok(const_cast<const void*>(m_value));
    else
        return Err(ErrValue::INVALID_OBJECT);
}

//*************************************************************************************************
Res<void*, ErrValue> ObjectRef::steal_value() && {
    if (!is_valid())
        return Err(ErrValue::INVALID_OBJECT);

    void* res = m_value;
    m_value = nullptr;
    m_type = nullptr;
    return Ok(res);
}

//*************************************************************************************************
Res<const Type*, ErrType> ObjectRef::type() const {

}

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
Object::Object(void* obj, const Type* type) 
    : ObjectRef(obj, type) 
{

}

//*************************************************************************************************
Object::~Object() {
    if (is_valid())
        m_type->delete_object(std::move(*this));
}