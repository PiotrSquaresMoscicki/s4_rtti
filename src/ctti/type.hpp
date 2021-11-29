#pragma once

#include <core/collections/tuple.hpp>

#include "attr/name.hpp"
#include "attr/size.hpp"
#include "attr/struct.hpp"
#include "attr/fundamental.hpp"
#include "attr/property.hpp"

namespace ctti {

	using namespace core::collections;
	using namespace core::str;

	//*********************************************************************************************
	//*********************************************************************************************
	//*********************************************************************************************
	template <typename T, typename... Attrs>
	class Type : public Tuple<Attrs...> {
	public:
		constexpr Type(Attrs... attrs) : Tuple<Attrs...>(std::forward<Attrs>(attrs)...) {}

		constexpr const ConstexprString& get_name() const { 
			return this->template get_one<attr::Name>(); 
		}

		constexpr size_t size() const { return this->template get_one<attr::Size>().m_size; }
		
		constexpr bool is_struct() const { return this->template has<attr::Struct>(); }
		constexpr bool is_fundamental() const { return this->template has<attr::Fundamental>(); }

		template<typename = std::enable_if_t<std::is_void<void>::value>>
		constexpr auto properties() const { return this->template get_all<attr::Property>();}

	}; // class Type

	//*********************************************************************************************
	//*********************************************************************************************
	//*********************************************************************************************
	template <typename T>
	class TypeFactory
	{
	public:
		template <typename... Attrs>
		static constexpr Type<T, Attrs...> get(Attrs... attrs) {
			return Type<T, Attrs...>(std::forward<Attrs>(attrs)...);
		}
		
	}; // class TypeFactory

} // namespace ctti
