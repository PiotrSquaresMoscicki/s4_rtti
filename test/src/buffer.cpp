#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
class TestClass {
public:
    CLASS(TestClass)
    END_CLASS

    TestClass() = default;

    TestClass(bool* deleted) 
        : m_deleted(deleted)
    {
        *m_deleted = false;
    }

    virtual ~TestClass() {
        *m_deleted = true;
    }

private:
    bool* m_deleted = nullptr;
};

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
TEST_CASE( "rtti::BufferRef::default constructor", "[rtti::BufferRef]" ) {
    BufferRef buff;
    const BufferRef constBuff;

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

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
TEST_CASE( "rtti::Buffer::default constructor", "[rtti::Buffer]" ) {
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
TEST_CASE( "rtti::Buffer::custom constructor", "[rtti::Buffer]" ) {
    int* obj1 = new int();
    int* obj2 = new int();

    Buffer buff(obj1, sizeof(int));
    const Buffer constBuff(obj2, sizeof(int));

    REQUIRE( buff.is_valid() == true );
    REQUIRE( buff.size().is_ok() == true );
    REQUIRE( buff.data().is_ok() == true );
    REQUIRE( buff.steal_data().is_ok() == true );

    REQUIRE( constBuff.is_valid() == true );
    REQUIRE( constBuff.size().is_ok() == true );
    REQUIRE( constBuff.data().is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Buffer::move constructor", "[rtti::Buffer]" ) {
    int* obj1 = new int();

    Buffer buff(obj1, sizeof(int));
    const Buffer constBuff(std::move(buff));

    REQUIRE( buff.is_valid() == false );

    REQUIRE( constBuff.is_valid() == true );
    REQUIRE( constBuff.size().is_ok() == true );
    REQUIRE( constBuff.data().is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Buffer::move assignment", "[rtti::Buffer]" ) {
    char* obj1 = new char[5];

    Buffer srcBuff(obj1, sizeof(char[5]));
    Buffer buff;
    
    buff = std::move(srcBuff);

    REQUIRE( srcBuff.is_valid() == false );

    REQUIRE( buff.is_valid() == true );
    REQUIRE( buff.size().is_ok() == true );
    REQUIRE( buff.data().is_ok() == true );
}
