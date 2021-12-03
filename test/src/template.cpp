#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

namespace test {
    
    template <typename T>
    class TestTemplate1 {
    public:
        bool m_bool = false;
        int m_int = 5;
        long m_long = 455;

        short sub(short first, short second) { return first - second; }
    };

}

//REGISTER_CLASS(test, TestTemplate1)
    // REGISTER_FIELD(m_bool)
    // REGISTER_FIELD(m_int)
    // REGISTER_FIELD(m_long)
    // REGISTER_METHOD(sub, (first second))
//END_REGISTER_CLASS

template <typename T>
class TestTemplate2 {
public:
    TEMPLATE(TestTemplate2, <T>)
    END_TEMPLATE

        // REGISTER_FIELD(m_char)
        // REGISTER_FIELD(m_short)
        // REGISTER_METHOD(sum, (a b))

    virtual ~TestTemplate2() = default;

    char m_char = 'c';
    short m_short = 99;

    //REGISTER_FIELD(m_test_class_1)
    test::TestTemplate1<int> m_test_template_1;

    //FIELD(char, m_another_char) = 'd';
    //METHOD(int, div, (int a CM int b)) const;

    //REGISTER_METHOD(first_method, ())
    void first_method() {}

    int sum(int a, int b) { return a + b; }
};

using namespace test;

// // *************************************************************************************************
// TEST_CASE( "rtti::TemplateInstance::members", "[rtti::Template]" ) {
//     REQUIRE( static_class<TestTemplate1>()->members().size() == 3 );
//     TestTemplate2<int> ss;
//     TestTemplate2<int>::m_char_init_field_info();
//     REQUIRE( static_class<TestTemplate2<int>>()->members().size() == 4 );
// }

// // *************************************************************************************************
// TEST_CASE( "rtti::TemplateInstance::methods", "[rtti::Template]" ) {
//     REQUIRE( static_class<TestTemplate1>()->methods().size() == 1 );
//     REQUIRE( static_class<TestTemplate2<int>>()->methods().size() == 2 );
// }

//*************************************************************************************************
TEST_CASE( "rtti::TemplateInstance::name", "[rtti::Template]" ) {
    //REQUIRE( static_type<TestTemplate1>()->name() == "test::TestTemplate1" );
    //REQUIRE( static_type<TestTemplate2<int>>()->name() == "TestTemplate2<T>" );
}
