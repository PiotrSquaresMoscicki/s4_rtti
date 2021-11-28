#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

namespace test {
    
    class TestClass1 {
    public:
        bool m_bool = false;
        int m_int = 5;
        long m_long = 455;
    };

}

REGISTER_CLASS(test, TestClass1)
    REGISTER_FIELD(m_bool)
    REGISTER_FIELD(m_int)
    REGISTER_FIELD(m_long)
END_CLASS

class TestClass2 {
    CLASS(TestClass2)
        REGISTER_FIELD(m_char)
        REGISTER_FIELD(m_short)

public:
    virtual ~TestClass2() = default;

    char m_char = 'c';
    short m_short = 99;

    REGISTER_FIELD(m_test_class_1)
    test::TestClass1 m_test_class_1;

    FIELD(char, m_another_char) = 'd';

    void first_method() {}
    REGISTER_METHOD(first_method, ())
};

using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Class::members", "[rtti::Class]" ) {
    REQUIRE( static_class<TestClass1>()->members().size() == 3 );
    REQUIRE( static_class<TestClass2>()->members().size() == 4 );
}

