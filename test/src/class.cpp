#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

namespace test {
    
    class TestClass1 {
    public:
        bool m_bool = false;
        int m_int = 5;
        long m_long = 455;

        short sub(short first, short second) { return first - second; }
    };

}

REGISTER_CLASS(test, TestClass1)
    REGISTER_FIELD(m_bool)
    REGISTER_FIELD(m_int)
    REGISTER_FIELD(m_long)
    REGISTER_METHOD(sub, (first second))
END_CLASS

class TestClass2 {
    CLASS(TestClass2)
        REGISTER_FIELD(m_char)
        REGISTER_FIELD(m_short)
        REGISTER_METHOD(sum, (a b))

public:
    virtual ~TestClass2() = default;

    char m_char = 'c';
    short m_short = 99;

    REGISTER_FIELD(m_test_class_1)
    test::TestClass1 m_test_class_1;

    FIELD(char, m_another_char) = 'd';
    //METHOD(int, div, (int a CM int b)) const;

    REGISTER_METHOD(first_method, ())
    void first_method() {}

    int sum(int a, int b) { return a + b; }
};

using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Class::members", "[rtti::Class]" ) {
    REQUIRE( static_class<TestClass1>()->members().size() == 3 );
    REQUIRE( static_class<TestClass2>()->members().size() == 4 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::methods", "[rtti::Class]" ) {
    REQUIRE( static_class<TestClass1>()->methods().size() == 1 );
    REQUIRE( static_class<TestClass2>()->methods().size() == 2 );
}
//*************************************************************************************************
TEST_CASE( "rtti::Class::name", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<TestClass1>()->name() == "test::TestClass1" );
    REQUIRE( static_type<TestClass2>()->name() == "TestClass2" );
}

