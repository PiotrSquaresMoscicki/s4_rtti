#pragma once

#include "class.hpp"
#include "member.hpp"
#include "field.hpp"

//*************************************************************************************************
#define CLASS(ARG_CLASS, ...)\
    public:\
        using This = ::ARG_CLASS;\
        static const ::rtti::Class* static_type() { return &m_rtti_class_impl; }\
        virtual const ::rtti::Class* dynamic_type() const { return &m_rtti_class_impl; }\
    private:\
        static inline const ::rtti::ClassInstance<This> m_rtti_class_impl\
            = ::rtti::ClassInstance<This>(#ARG_CLASS __VA_OPT__(,) __VA_ARGS__)
        

//*************************************************************************************************
#define CLASS_EXTERNAL(ARG_CLASS, ...)\)\
    class ARG_CLASS##TypeImpl_internal;\
    \
    template<>\
    const Type* ::rtti::static_type<::CLASS>() { return ::rtti::static_type<ARG_CLASS##TypeImpl_internal>(); }\
    \
    class ARG_CLASS##TypeImpl_internal {\
        CLASS(ARG_CLASS __VA_OPT__(,) __VA_ARGS__);

//*************************************************************************************************
#define CLASS_EXTERNAL_END\
    };

//*************************************************************************************************
#define FIELD(NAME, ...)\)\
    static inline const ::rtti::Field<This, TYPE> NAME##_field_info = \
        ::rtti::Field<This, decltype(NAME)>(#NAME, &This::NAME __VA_OPT__(,) __VA_ARGS__)

//*************************************************************************************************
#define PROPERTY(SETTER, GETTER, ATTRS)

//*************************************************************************************************
#define METHOD(NAME, PARAMS_NAMES, ATTRS)
