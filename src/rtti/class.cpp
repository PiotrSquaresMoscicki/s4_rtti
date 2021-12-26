#include "class.hpp"

using namespace rtti;

//*************************************************************************************************
Res<const Method*, Class::ErrMethod> Class::method(StringId name) {
    const auto result = m_methods_by_name.find(name);
    if (result == m_methods_by_name.end())
        return Err(ErrMethod::METHOD_NOT_FOUND);
    else
        return Ok(result->second);
}