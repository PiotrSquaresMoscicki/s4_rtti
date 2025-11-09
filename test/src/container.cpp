#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;

//*************************************************************************************************
TEST_CASE( "rtti::Container::==", "[rtti::Container]" ) {
    // REQUIRE( static_type_trait<std::vector<int>>::get() == static_type_trait<std::vector<int>>::get() );
}
