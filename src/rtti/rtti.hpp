#pragma once

#include "fundamental.hpp"
#include "enum.hpp"
#include "class.hpp"
#include "field.hpp"

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define REGISTER_FUNDAMENTAL(ARG_TYPE)\
    template <> inline const ::rtti::Type* ::rtti::static_type<ARG_TYPE>() {\
        static ::rtti::FundamentalInstance<ARG_TYPE> result(#ARG_TYPE);\
        return &result;\
    }

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define REGISTER_ENUM(ARG_ENUM)\
    template <>\
    inline const ::rtti::Type* ::rtti::static_type<ARG_ENUM>() {\
        using EnumType = ::ARG_ENUM;\
        static ::rtti::EnumInstance<EnumType> result = ::rtti::EnumInstance<EnumType>(#ARG_ENUM, \
            std::vector<::rtti::EnumValue> {

//*************************************************************************************************
#define ENUM_VALUE(ARG_VALUE)\
     ::rtti::EnumValue(#ARG_VALUE, static_cast<size_t>(EnumType::ARG_VALUE)),

//*************************************************************************************************
#define END_ENUM\
             ::rtti::EnumValue() /* dummy enum value so we can use , in ENUM_VALUE macro */\
        });\
        return &result; \
    }\

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define CLASS(ARG_CLASS, ...)\
    public:\
        using This = ::ARG_CLASS;\
        static const ::rtti::Class* static_class() { return &m_rtti_class_impl; }\
        virtual const ::rtti::Class* dynamic_class() const { return &m_rtti_class_impl; }\
    private:\
        static inline const ::rtti::ClassInstance<This> m_rtti_class_impl\
            = ::rtti::ClassInstance<This>(#ARG_CLASS __VA_OPT__(,) __VA_ARGS__);
        
//*************************************************************************************************
#define REGISTER_CLASS(ARG_CLASS, ...)\
    class ARG_CLASS##TypeImpl_internal;\
    \
    template<>\
    const Type* ::rtti::static_type<::CLASS>() {\
        return ::rtti::static_type<ARG_CLASS##TypeImpl_internal>();\
    }\
    template<>\
    const Type* ::rtti::static_class<::CLASS>() {\
        return ::rtti::static_type<ARG_CLASS##TypeImpl_internal>();\
    }\
    \
    class ARG_CLASS##TypeImpl_internal {\
        CLASS(ARG_CLASS __VA_OPT__(,) __VA_ARGS__);

//*************************************************************************************************
#define END_CLASS\
    };

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define FIELD(ARG_NAME, ...)\
    static inline const ::rtti::FieldInstance<This, decltype(ARG_NAME)> ARG_NAME##_field_info = \
        ::rtti::FieldInstance<This, decltype(ARG_NAME)>\
            (#ARG_NAME, &This::ARG_NAME __VA_OPT__(,) __VA_ARGS__);

//*************************************************************************************************
#define PROPERTY(SETTER, GETTER, ATTRS)

//*************************************************************************************************
#define METHOD(NAME, PARAMS_NAMES, ATTRS)