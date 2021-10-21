#include "enum.hpp"

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
Enum::Enum(std::string name, size_t size, std::vector<EnumValue> values) 
    : Type(std::move(name), size, {})
    , m_values(std::move(values))
{
    for (const EnumValue& value : m_values)
        m_values_ptrs.push_back(&value);
}