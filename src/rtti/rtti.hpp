#pragma once

#include <tuple>

#include "ctti/ctti.hpp"

#include "database.hpp"
#include "attributes.hpp"
#include "fundamental.hpp"
#include "enum.hpp"
#include "class.hpp"
#include "template_instance.hpp"
#include "field.hpp"
#include "method.hpp"
        
#define STR(ARG) #ARG
#define COMMA ,

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define REGISTER_FUNDAMENTAL(ARG_TYPE)\
    template <> S4_RTTI_EXPORT inline const ::rtti::Type* ::rtti::static_type<ARG_TYPE>() {\
        static const ::rtti::Type* result = nullptr;\
        static ::rtti::FundamentalInstance<ARG_TYPE> instance(#ARG_TYPE);\
        if (result == nullptr)\
            result = ::rtti::Database::register_type(&instance).ok();\
        return result;\
    }

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define REGISTER_ENUM(ARG_ENUM)\
    template <>\
    inline const ::rtti::Type* ::rtti::static_type<ARG_ENUM>() {\
        using EnumType = ::ARG_ENUM;\
        static ::rtti::EnumInstance<EnumType> instance = ::rtti::EnumInstance<EnumType>(#ARG_ENUM, \
            std::vector<::rtti::EnumValue> {

//*************************************************************************************************
#define ENUM_VALUE(ARG_VALUE)\
     ::rtti::EnumValue(#ARG_VALUE, static_cast<size_t>(EnumType::ARG_VALUE)),

//*************************************************************************************************
#define END_ENUM\
             ::rtti::EnumValue() /* dummy enum value so we can use , in ENUM_VALUE macro */\
        });\
        static const ::rtti::Enum* result = nullptr;\
        if (result == nullptr) {\
            result = static_cast<const ::rtti::Enum*>(\
                ::rtti::Database::register_type(&instance).ok());\
        }\
        return result; \
    }\

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define CLASS_INTERNAL(ARG_CLASS, ARG_DECLARING_CLASS, ...)\
        using This = ::ARG_CLASS;\
        using DeclaringClass = ::ARG_DECLARING_CLASS;\
        virtual const ::rtti::Class* dynamic_class() const { return static_class(); }\
        static const ::rtti::Class* static_class() {\
            static const ::rtti::Class* result = nullptr;\
            static ::rtti::ClassInstance<This> instance(#ARG_CLASS __VA_OPT__(,) __VA_ARGS__);\
            if (result == nullptr) {

#define END_CLASS_INTERNAL\
                result = static_cast<const ::rtti::Class*>(\
                    ::rtti::Database::register_type(&instance).ok());\
            }\
            return result;\
        }

//*************************************************************************************************
#define CLASS(ARG_CLASS, ...)\
    CLASS_INTERNAL(ARG_CLASS, ARG_CLASS __VA_OPT__(,) __VA_ARGS__)

#define END_CLASS\
    END_CLASS_INTERNAL

//*************************************************************************************************
#define REGISTER_CLASS(NAMESPACE, ARG_CLASS, ...)\
    namespace NAMESPACE {\
        class ARG_CLASS##TypeImpl_internal;\
    }\
    \
    template<>\
    const Type* ::rtti::static_type<::NAMESPACE::ARG_CLASS>() {\
        return ::rtti::static_type<::NAMESPACE::ARG_CLASS##TypeImpl_internal>();\
    }\
    template<>\
    const Class* ::rtti::static_class<::NAMESPACE::ARG_CLASS>() {\
        return ::rtti::static_class<::NAMESPACE::ARG_CLASS##TypeImpl_internal>();\
    }\
    \
    namespace NAMESPACE {\
        class ARG_CLASS##TypeImpl_internal {\
        public:\
            CLASS_INTERNAL(NAMESPACE::ARG_CLASS, NAMESPACE::ARG_CLASS##TypeImpl_internal __VA_OPT__(,) __VA_ARGS__)

#define END_REGISTER_CLASS\
            END_CLASS_INTERNAL\
        };\
    } // namespace NAMESPACE

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define TEMPLATE_INTERNAL(ARG_TEMPLATE, ARG_DECLARING_TEMPLATE, ARG_PARAMS, ...)\
        using This = ARG_TEMPLATE ARG_PARAMS;\
        using DeclaringClass = ARG_DECLARING_TEMPLATE ARG_PARAMS;\
        using ParamsTuple = ::std::tuple ARG_PARAMS;\
        virtual const ::rtti::Class* dynamic_class() const { return static_class(); }\
        static const ::rtti::Class* static_class() {\
            static bool initialized = false;\
            std::vector<std::string> out_params_names;\
            static const ::rtti::Class* result = nullptr;\
            static ::rtti::TemplateInstanceInstance<This, DeclaringClass> instance(\
                STR(ARG_TEMPLATE ARG_PARAMS), out_params_names __VA_OPT__(,) __VA_ARGS__);\
            if (result == nullptr) {

#define END_TEMPLATE_INTERNAL\
                result = static_cast<const ::rtti::Class*>(\
                    ::rtti::Database::register_type(&instance).ok());\
            }\
            return result;\
        }

//*************************************************************************************************
#define TEMPLATE(ARG_TEMPLATE, ARG_PARAMS, ...)\
    TEMPLATE_INTERNAL(ARG_TEMPLATE, ARG_TEMPLATE, ARG_PARAMS, __VA_OPT__(,) __VA_ARGS__)

#define END_TEMPLATE\
    END_TEMPLATE_INTERNAL

//*************************************************************************************************
#define REGISTER_FIELD(ARG_NAME, ...)\
    static inline void ARG_NAME##_init_field_info() {\
        static ::rtti::FieldInstance<This, DeclaringClass, decltype(This::ARG_NAME)> instance(\
            #ARG_NAME, &This::ARG_NAME __VA_OPT__(,) __VA_ARGS__);\
    }\
    static inline ::rtti::internal::StaticInitializer ARG_NAME##_field_info_initializer\
        = ::rtti::internal::StaticInitializer(&DeclaringClass::ARG_NAME##_init_field_info);

#define FIELD(ARG_TYPE, ARG_NAME, ...)\
    REGISTER_FIELD(ARG_NAME __VA_OPT__(,) __VA_ARGS__)\
    ARG_TYPE ARG_NAME

//*************************************************************************************************
#define PROPERTY(SETTER, GETTER, ATTRS)

//*************************************************************************************************
#define REGISTER_METHOD(ARG_NAME, ARG_PARAMS_NAMES, ...)\
    void ARG_NAME##_init_method_info_helper() {}\
    static inline void ARG_NAME##_init_method_info() {\
        static ::rtti::MethodInstance instance(\
            #ARG_NAME, #ARG_PARAMS_NAMES, &This::ARG_NAME\
            , &DeclaringClass::ARG_NAME##_init_method_info_helper __VA_OPT__(,) __VA_ARGS__);\
    }\
    static inline ::rtti::internal::StaticInitializer ARG_NAME##_method_info_initializer\
        = ::rtti::internal::StaticInitializer(&DeclaringClass::ARG_NAME##_init_method_info);

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
template <> inline const ::rtti::Type* ::rtti::static_type<void>() {
    return nullptr;
}

REGISTER_FUNDAMENTAL(bool)

REGISTER_FUNDAMENTAL(signed char)
REGISTER_FUNDAMENTAL(unsigned char)
REGISTER_FUNDAMENTAL(char)
REGISTER_FUNDAMENTAL(wchar_t)
REGISTER_FUNDAMENTAL(char16_t)
REGISTER_FUNDAMENTAL(char32_t)
REGISTER_FUNDAMENTAL(char8_t)

REGISTER_FUNDAMENTAL(short int)
REGISTER_FUNDAMENTAL(unsigned short int)
REGISTER_FUNDAMENTAL(int)
REGISTER_FUNDAMENTAL(unsigned int)
REGISTER_FUNDAMENTAL(long int)
REGISTER_FUNDAMENTAL(unsigned long int)
REGISTER_FUNDAMENTAL(long long int)
REGISTER_FUNDAMENTAL(unsigned long long int)

REGISTER_FUNDAMENTAL(float)
REGISTER_FUNDAMENTAL(double)
REGISTER_FUNDAMENTAL(long double)

