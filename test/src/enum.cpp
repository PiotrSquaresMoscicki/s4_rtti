#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

namespace test {
    
    enum class TestEnum1 {
        VAL_1,
        VAL_2,
        VAL_3
    };

}

REGISTER_ENUM(test::TestEnum1)
    ENUM_VALUE(VAL_1)
    ENUM_VALUE(VAL_2)
    ENUM_VALUE(VAL_3)
END_ENUM
    
enum class TestEnum2 {
    VAL_4,
    VAL_5,
    VAL_6
};

REGISTER_ENUM(TestEnum2)
    ENUM_VALUE(VAL_4)
    ENUM_VALUE(VAL_5)
    ENUM_VALUE(VAL_6)
END_ENUM

//*************************************************************************************************
TEST_CASE( "rtti::Enum::==", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<TestEnum2>() == static_type<TestEnum2>() );
    REQUIRE( static_type<TestEnum2>() != static_type<float>() );
}