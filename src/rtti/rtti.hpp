#pragma once

#include <tuple>

#include "ctti/ctti.hpp"

#include "attributes.hpp"
#include "fundamental.hpp"
#include "enum.hpp"
#include "class.hpp"
#include "template_instance.hpp"
#include "field.hpp"
#include "method.hpp"
        
#define STR(ARG) #ARG

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define DECLARE_FUNDAMENTAL(ARG_TYPE)\
    template <>\
    S4_RTTI_EXPORT const ::rtti::Type* ::rtti::static_type<ARG_TYPE>();

#define DEFINE_FUNDAMENTAL(ARG_TYPE)\
    template <> S4_RTTI_EXPORT const ::rtti::Type* ::rtti::static_type<ARG_TYPE>() {\
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
#define DECLARE_CLASS(ARG_CLASS) \
    virtual const ::rtti::Class* dynamic_class() const { return static_class(); }\
    static const ::rtti::Class* static_class();\
    static void initialize_class(::rtti::Class& instance);

#define DEFINE_CLASS(ARG_CLASS, ...) \
    const ::rtti::Class* ::ARG_CLASS::static_class() {\
        static bool initialized = false;\
        static ::rtti::ClassInstance<::ARG_CLASS> instance(#ARG_CLASS __VA_OPT__(,) __VA_ARGS__);\
        if (!initialized) {\
            initialized = true;\
            initialize_class(instance);\
        }\
        return &instance;\
    }\
    void ::ARG_CLASS::initialize_class(::rtti::Class& instance)

//*************************************************************************************************
#define DECLARE_CLASS_EXTERN(ARG_CLASS)\
    template <>\
    inline const ::rtti::Type* ::rtti::static_type<::ARG_CLASS>();\
    template <>\
    inline const ::rtti::Class* ::rtti::static_class<::ARG_CLASS>();\
    template <>\
    void ::rtti::initialize_class<::ARG_CLASS>(::rtti::Class& instance);

#define DEFINE_CLASS_EXTERN(ARG_CLASS, ...) \
    template <>\
    inline const ::rtti::Type* ::rtti::static_type<::ARG_CLASS>() { \
        return ::rtti::static_class<::ARG_CLASS>();\
    }\
    template <>\
    inline const ::rtti::Class* ::rtti::static_class<::ARG_CLASS>() {\
        static bool initialized = false;\
        static ::rtti::ClassInstance<::ARG_CLASS> instance(#ARG_CLASS __VA_OPT__(,) __VA_ARGS__);\
        if (!initialized) {\
            initialized = true;\
            initialize_class<::ARG_CLASS>(instance);\
        }\
        return &instance;\
    }\
    template <>\
    void ::rtti::initialize_class<::ARG_CLASS>(::rtti::Class& instance)

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
#define TEMPLATE_INTERNAL(ARG_TEMPLATE, ARG_DECLARING_TEMPLATE, ARG_PARAMS, ...)\
        using This = ARG_TEMPLATE;\
        using DeclaringClass = ARG_DECLARING_TEMPLATE;\
        using ParamsTuple = ::std::tuple ARG_PARAMS;\
        virtual const ::rtti::Class* dynamic_class() const { return static_class(); }\
        static const ::rtti::Class* static_class() {\
            static bool initialized = false;\
            std::vector<std::string> out_params_names;\
            static ::rtti::TemplateInstanceInstance<This, DeclaringClass> instance(STR(ARG_TEMPLATE ARG_PARAMS), out_params_names __VA_OPT__(,) __VA_ARGS__);\
            if (!initialized) {\
                initialized = true;

#define END_TEMPLATE_INTERNAL\
            }\
            return &instance;\
        }

//*************************************************************************************************
#define TEMPLATE(ARG_TEMPLATE, ARG_PARAMS, ...)\
    TEMPLATE_INTERNAL(ARG_TEMPLATE, ARG_TEMPLATE, ARG_PARAMS, __VA_OPT__(,) __VA_ARGS__)

#define END_TEMPLATE\
    END_TEMPLATE_INTERNAL

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
namespace rtti::internal {
    
    class StaticInitializer {
    public:
        StaticInitializer(void (*fn)()) { fn(); }

    }; // class StaticInitializer

} // namespace rtti::internal

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

DECLARE_FUNDAMENTAL(bool)

DECLARE_FUNDAMENTAL(signed char)
DECLARE_FUNDAMENTAL(unsigned char)
DECLARE_FUNDAMENTAL(char)
DECLARE_FUNDAMENTAL(wchar_t)
DECLARE_FUNDAMENTAL(char16_t)
DECLARE_FUNDAMENTAL(char32_t)
DECLARE_FUNDAMENTAL(char8_t)

DECLARE_FUNDAMENTAL(short int)
DECLARE_FUNDAMENTAL(unsigned short int)
DECLARE_FUNDAMENTAL(int)
DECLARE_FUNDAMENTAL(unsigned int)
DECLARE_FUNDAMENTAL(long int)
DECLARE_FUNDAMENTAL(unsigned long int)
DECLARE_FUNDAMENTAL(long long int)
DECLARE_FUNDAMENTAL(unsigned long long int)

DECLARE_FUNDAMENTAL(float)
DECLARE_FUNDAMENTAL(double)
DECLARE_FUNDAMENTAL(long double)

