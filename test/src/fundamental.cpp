#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

//*************************************************************************************************
TEST_CASE( "Compare fundamental types", "[Fundamental]" ) {
    REQUIRE( static_type<bool>() == static_type<bool>() );
    REQUIRE( static_type<int>() != static_type<float>() );
}

//*************************************************************************************************
TEST_CASE( "Get names of fundamental types", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->name() == "bool" );
    REQUIRE( static_type<double>()->name() == "double" );
}

//*************************************************************************************************
TEST_CASE( "Get as fundamental", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_fundamental() == static_type<bool>() );
}

//*************************************************************************************************
TEST_CASE( "Get as enum", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_enum() == nullptr );
}

//*************************************************************************************************
TEST_CASE( "Get as class", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_class() == nullptr );
}

//*************************************************************************************************
TEST_CASE( "Create new object", "[Fundamental]" ) {
    Object obj = static_type<float>()->new_object();
    REQUIRE( obj.type() == static_type<float>() );
    REQUIRE( obj.is_valid() == true );
}

