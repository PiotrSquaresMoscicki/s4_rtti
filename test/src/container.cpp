#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;

//*************************************************************************************************
TEST_CASE( "rtti::Container::==", "[rtti::Container]" ) {
    REQUIRE( static_type<std::vector<int>>::get() == static_type<std::vector<int>>::get() );
    REQUIRE( static_type<std::vector<int>>::get() != static_type<std::vector<float>>::get() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::name", "[rtti::Container]" ) {
    REQUIRE( 
        static_type<std::vector<int>>::get()->name() 
        == 
        "std::vector<int,std::allocator<int>>" );

    REQUIRE( 
        static_type<std::vector<float>>::get()->name() 
        == 
        "std::vector<float,std::allocator<float>>" );
}