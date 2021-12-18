#include "database.hpp"

#include "type.hpp"

using namespace rtti;
using namespace core::str;
using namespace core::util;

std::map<StringId, const Type*> Database::m_name_to_type;

//*************************************************************************************************
Res<const Type*, Database::ErrRegisterType> Database::register_type(const Type* type) {
    const StringId new_type_name(type->name());
    const auto registered_type_it = m_name_to_type.find(new_type_name);

    if (type == nullptr)
        return Err(ErrRegisterType::UNDEFINED);
    else if (registered_type_it == m_name_to_type.end()) {
        m_name_to_type[new_type_name] = type;
        return Ok(type);
    } else if (type->size() == registered_type_it->second->size())
        return Ok(registered_type_it->second);
    else
        return Err(ErrRegisterType::UNDEFINED);
}