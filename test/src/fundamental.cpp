#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::==", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>() == static_type<bool>() );
    REQUIRE( static_type<int>() != static_type<float>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::name", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->name() == "bool" );
    REQUIRE( static_type<double>()->name() == "double" );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::size", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->size() == sizeof(bool) );
    REQUIRE( static_type<float>()->size() == sizeof(float) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::new_object", "[rtti::Fundamental]" ) {
    Object obj = static_type<float>()->new_object();
    REQUIRE( obj.type() == static_type<float>() );
    REQUIRE( obj.is_valid() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::call_constructor", "[rtti::Fundamental]" ) {
    Buffer buf
    REQUIRE( obj.type() == static_type<float>() );
    REQUIRE( obj.is_valid() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::copy", "[rtti::Fundamental]" ) {
    Object obj1 = static_type<int>()->new_object();
    int obj2_src = 5;
    ObjectRef obj2(&obj2_src);
    static_type<int>()->copy(obj1, obj2);
    REQUIRE( *reinterpret_cast<const int*>(obj1.value()) == 5 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::Get as fundamental", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_fundamental() == static_type<bool>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::Get as enum", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_enum() == nullptr );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::Get as class", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_class() == nullptr );
}

