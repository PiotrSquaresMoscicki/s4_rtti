#pragma once

#include "ctti_fwd.hpp"
#include "type.hpp"

//*************************************************************************************************
#define DECLARE_STRUCT(STRUCT)\
	friend constexpr auto ::ctti::constexpr_type<::STRUCT>();\
	template <typename CttiDeclarationTypename>\
	static constexpr auto constexpr_type()\
	{\
		return ::ctti::TypeFactory<CttiDeclarationTypename>::get\
		(\
			::ctti::attr::Name(#STRUCT),\
			::ctti::attr::Sizeof(sizeof(STRUCT)),

//*************************************************************************************************
#define END_STRUCT\
			::ctti::attr::Struct()\
		);\
	}

//*************************************************************************************************
#define CTTI_DECLARE_FUNDAMENTAL(TYPE)\
	template <>\
	constexpr auto ::ctti::constexpr_type<TYPE>()\
	{\
		return ::ctti::TypeFactory<TYPE>::get(\
            ::ctti::attr::Name(#TYPE),\
			::ctti::attr::Size(sizeof(TYPE)),\
			::ctti::attr::Fundamental()\
        );\
	}

CTTI_DECLARE_FUNDAMENTAL(bool)

CTTI_DECLARE_FUNDAMENTAL(short int)
CTTI_DECLARE_FUNDAMENTAL(unsigned short int)
CTTI_DECLARE_FUNDAMENTAL(int)
CTTI_DECLARE_FUNDAMENTAL(unsigned int)
CTTI_DECLARE_FUNDAMENTAL(long int)
CTTI_DECLARE_FUNDAMENTAL(unsigned long int)
CTTI_DECLARE_FUNDAMENTAL(long long int)
CTTI_DECLARE_FUNDAMENTAL(unsigned long long int)

CTTI_DECLARE_FUNDAMENTAL(signed char)
CTTI_DECLARE_FUNDAMENTAL(unsigned char)
CTTI_DECLARE_FUNDAMENTAL(char)
CTTI_DECLARE_FUNDAMENTAL(wchar_t)
CTTI_DECLARE_FUNDAMENTAL(char8_t)
CTTI_DECLARE_FUNDAMENTAL(char16_t)
CTTI_DECLARE_FUNDAMENTAL(char32_t)

CTTI_DECLARE_FUNDAMENTAL(float)
CTTI_DECLARE_FUNDAMENTAL(double)
CTTI_DECLARE_FUNDAMENTAL(long double)

// #pragma once

// template <typename... Args>
// constexpr core::ConstexprString getFullTemplateStructName(core::ConstexprString className)
// {
// 	core::ConstexprString args = getFullTemplateStructName<Args...>();

// 	//auto classStatic = StringFactory<className.length()>(className.get());
// 	//auto argsStatic = StringFactory<args.length()>(args.get());

// 	//return core::ConstexprString((classStatic + "<" + argsStatic + ">").Get());
// 	return core::ConstexprString("asdf");
// }

// template <typename First, typename... Rest>
// constexpr const char* getFullTemplateStructName()
// {
// 	core::ConstexprString first = ctti::get<First>().template get<attr::Name>();
// 	core::ConstexprString rest = getFullTemplateStructName<Rest...>();
// 	auto firstStatic = StringFactory<first.length()>(first.get());
// 	auto restStatic = StringFactory<rest.length()>(rest.get());
// 	return (firstStatic + ", " + restStatic).Get();
// }

// template <typename Last>
// constexpr const char* getFullTemplateStructName()
// {
// 	return ctti::get<Last>().get<attr::Name>().get();
// }

/*
#define CTTI_DECLARE_TEMPLATE_STRUCT(STRUCT, ...)\
	friend constexpr auto ::ctti::get<::STRUCT<__VA_ARGS__>>();\
	template <typename CttiDeclarationTypename>\
	static constexpr auto getTypeInfo()\
	{\
		return ::ctti::AttrArrayFactory<CttiDeclarationTypename>::get\
		(\
			::attr::Name(#STRUCT),

*/