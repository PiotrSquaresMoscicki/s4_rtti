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
TEST_CASE( "rtti::Buffer::default constructor", "[rtti::Buffer]" ) {
    Buffer buff;
    const Buffer constBuff;
    REQUIRE( buff.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "rtti::Buffer::custom constructor", "[rtti::Buffer]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));
    REQUIRE( buff.is_valid() == true );
}

