#include "database.hpp"

#include "type.hpp"
#include "function.hpp"

using namespace rtti;
using namespace core::str;
using namespace core::util;

//*************************************************************************************************
Res<TypePtr, Database::ErrRegisterType> Database::register_type(TypePtr type) {
    const StringId new_type_name(type->name());
    const auto registered_type_it = get_name_to_type().find(new_type_name);

    if (type == nullptr)
        return Err(ErrRegisterType::UNDEFINED);
    else if (registered_type_it == get_name_to_type().end()) {
        get_name_to_type()[new_type_name] = type;
        return Ok(type);
    } else if (type->size() == registered_type_it->second->size())
        return Ok(registered_type_it->second);
    else
        return Err(ErrRegisterType::UNDEFINED);
}

//*************************************************************************************************
Res<const Function*, Database::ErrRegisterFunction> Database
    ::register_function(const Function* function) 
{
    const StringId new_function_name(function->name());
    const auto registered_function_it = get_name_to_function().find(new_function_name);

    if (function == nullptr)
        return Err(ErrRegisterFunction::UNDEFINED);
    else if (registered_function_it == get_name_to_function().end()) {
        get_name_to_function()[new_function_name] = function;
        return Ok(function);
    }
    else
        return Err(ErrRegisterFunction::UNDEFINED);
}

//*************************************************************************************************
std::map<StringId, TypePtr>& Database::get_name_to_type() {
    static std::map<StringId, TypePtr> result;
    return result;
}

//*************************************************************************************************
std::map<StringId, const Function*>& Database::get_name_to_function() {
    static std::map<StringId, const Function*> result;
    return result;
}