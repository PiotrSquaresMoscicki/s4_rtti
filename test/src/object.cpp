#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

//*************************************************************************************************
TEST_CASE( "Default ObjectRef", "[ObjectRef]" ) {
    ObjectRef ref;
    REQUIRE( ref.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "Initialized ObjectRef", "[ObjectRef]" ) {
    int test_variable = 5;
    ObjectRef ref(&test_variable);
    REQUIRE( ref.is_valid() == true );
    REQUIRE( ref.value() == &test_variable );
    REQUIRE( *reinterpret_cast<const int*>(ref.value()) == test_variable );
    REQUIRE( ref.type() == static_type<int>() );
}
