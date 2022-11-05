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
    REQUIRE( buff.size().err() == BufferRef::Error::INVALID_BUFFER );
    REQUIRE( buff.data().err() == BufferRef::Error::INVALID_BUFFER );
    REQUIRE( buff.steal_data().err() == BufferRef::Error::INVALID_BUFFER );

    REQUIRE( constBuff.is_valid() == false );
    REQUIRE( constBuff.size().err() == BufferRef::Error::INVALID_BUFFER );
    REQUIRE( constBuff.data().err() == BufferRef::Error::INVALID_BUFFER );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::custom constructor", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));
    const BufferRef constBuff(&i, sizeof(int));

    REQUIRE( buff.is_valid() == true );
    REQUIRE( buff.size().is_ok() == true );
    REQUIRE( buff.data().is_ok() == true );
    REQUIRE( buff.steal_data().is_ok() == true );

    REQUIRE( constBuff.is_valid() == true );
    REQUIRE( constBuff.size().is_ok() == true );
    REQUIRE( constBuff.data().is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::move constructor", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef sourceBuff(&i, sizeof(int));
    BufferRef buff(std::move(sourceBuff));

    REQUIRE( sourceBuff.is_valid() == false );
    REQUIRE( sourceBuff.size().is_err() == true );
    REQUIRE( sourceBuff.data().is_err() == true );
    REQUIRE( sourceBuff.steal_data().is_err() == true );

    REQUIRE( buff.is_valid() == true );
    REQUIRE( buff.size().is_ok() == true );
    REQUIRE( buff.data().is_ok() == true );
    REQUIRE( buff.steal_data().is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::move assign", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef sourceBuff(&i, sizeof(int));
    BufferRef buff;
    buff = std::move(sourceBuff);

    REQUIRE( sourceBuff.is_valid() == false );
    REQUIRE( sourceBuff.size().is_err() == true );
    REQUIRE( sourceBuff.data().is_err() == true );
    REQUIRE( sourceBuff.steal_data().is_err() == true );

    REQUIRE( buff.is_valid() == true );
    REQUIRE( buff.size().is_ok() == true );
    REQUIRE( buff.data().is_ok() == true );
    REQUIRE( buff.steal_data().is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::size", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));

    REQUIRE( buff.size().ok() == sizeof(int) );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::data", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));

    REQUIRE( buff.data().ok() == &i );
}

//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::steal_data", "[rtti::BufferRef]" ) {
    int i = 0;
    BufferRef buff(&i, sizeof(int));

    REQUIRE( buff.steal_data().ok() == &i );
    REQUIRE( buff.is_valid() == false );
}
