#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

class TestClass2 {
public:
    CLASS(TestClass2)
    END_CLASS

    virtual ~TestClass2() = default;
};

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::default constructor", "[rtti::BufferRef]" ) {
    Buffer buff;
    const Buffer constBuff;
    REQUIRE( buff.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::custom constructor", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));
    REQUIRE( buff.is_valid() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::move constructor", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));
    REQUIRE( buff.is_valid() == true );
}

