#pragma once

#include <core/collections/tuple.hpp>
#include <core/str/constexpr_string.hpp>

#include "ctti/attr/name.hpp"

namespace ctti::attr {

	using namespace core::collections;
	using namespace core::str;

	//*********************************************************************************************
	//*********************************************************************************************
	//*********************************************************************************************
	template <typename T, typename S, typename... Attrs>
	class Property : public Tuple<Attrs...> {
	public:
		constexpr Property(Attrs... attrs) 
			: Tuple<Attrs...>(std::forward<Attrs>(attrs)...) {}

		constexpr const ConstexprString& get_name() const { 
			return this->template get_one<attr::Name>(); 
		}

		constexpr T& get(S& structure) const {}
		constexpr const T& get(const S& structure) const {}



	}; // class Property

	//*********************************************************************************************
	//*********************************************************************************************
	//*********************************************************************************************
	template <typename T, typename S>
	class PropertyFactory
	{
	public:
		template <typename... Attrs>
		static constexpr Property<T, S, Attrs...> get(Attrs... attrs) {
			return Property<T, S, Attrs...>(std::forward<Attrs>(attrs)...);
		}

	}; // class PropertyFactory

	//*********************************************************************************************
	//*********************************************************************************************
	//*********************************************************************************************
	template <typename T, typename S>
	class FieldPointer {
	public:
		constexpr FieldPointer(T S::* p) : m_ptr(p) {}

		constexpr T& get(S& s) const { return s.*m_ptr; }
		constexpr const T& get(const S& s) const { return s.*m_ptr; }

		void set(S& s, T val) const { s.*m_ptr = std::move(val); }
		void set(S& s, const T& val) const { s.*m_ptr = val; }

	private:
		T S::* const m_ptr = nullptr;

	}; // class FieldPointer

} // namespace ctti::ctti

//*************************************************************************************************
#define CONSTEXPR_DECLARE_PROPERTY(FIELD, ...)\
	::ctti::attr::PropertyFactory<\
		decltype(CttiDeclarationTypename::FIELD), CttiDeclarationTypename>::get\
	(\
		::ctti::attr::Name(#FIELD), \
		::ctti::attr::FieldPointer(&CttiDeclarationTypename::FIELD)\
		__VA_OPT__(,) __VA_ARGS__\
	),

