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
    FIELD(m_bool)
    FIELD(m_int)
    FIELD(m_long)
END_CLASS

class TestClass2 {
    CLASS(TestClass2)

public:
    virtual ~TestClass2() = default;

    char m_char = 'c';
    short m_short = 99;
    test::TestClass1 m_test_class_1;
};

using namespace test;
