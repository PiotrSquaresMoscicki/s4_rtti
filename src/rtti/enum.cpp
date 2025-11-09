#include "enum.hpp"

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
Enum::Enum(std::vector<EnumValue> values)
{
    values.pop_back();
    m_values = std::move(values);
    for (EnumValue& value : m_values) {
        value.m_declaring_enum = this;
        m_values_ptrs.push_back(&value);
    }
}