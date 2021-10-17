#pragma once

#include "class.hpp"
#include "member.hpp"
#include "field.hpp"

#define DECLARE_CLASS(CLASS)\
public:
    using This = CLASS;\
private:\
    template <typename TYPE> friend const TYPE* get_type();\
    static inline const ClassInstance<This> m_class = ClassInstance<This>(#CLASS);