#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;

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
    // REGISTER_FIELD(m_bool)
    // REGISTER_FIELD(m_int)
    // REGISTER_FIELD(m_long)
    // REGISTER_METHOD(sub, (first second))
END_REGISTER_CLASS

class TestClass2 {
public:
    CLASS(TestClass2)
    END_CLASS

    virtual ~TestClass2() = default;

    char m_char = 'c';
    short m_short = 99;

    test::TestClass1 m_test_class_1;
    void first_method() {}

    int sum(int a, int b) { return a + b; }
};

using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Enum::== from differend dll", "[rtti::Enum]" ) {
    std::string shared_lib_path;

    #ifdef __APPLE__
        shared_lib_path = "libs4_rtti_test_lib.dylib";
    #elif __linux__
        shared_lib_path = "libs4_rtti_test_lib.so";
    #elif _WIN32
        shared_lib_path = "libs4_rtti_test_lib.dll";
    #endif 
    
    
    Shared lib = Shared::open(std::string(S4_RTTI_PROJECT_FULL_DIST_DIR) + "/" + shared_lib_path).ok();
    ITestInterface* test_obj 
       = reinterpret_cast<ITestInterface*(*)()>(lib.symbol("create_test_interface").ok())();
        
    REQUIRE( static_type<DynamicallyLoadedLibClass>() == test_obj->get_test_class_type() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::members", "[rtti::Class]" ) {
    //REQUIRE( static_class<TestClass1>()->members().size() == 3 );
    //REQUIRE( static_class<TestClass2>()->members().size() == 4 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::methods", "[rtti::Class]" ) {
    //REQUIRE( static_class<TestClass1>()->methods().size() == 1 );
    //REQUIRE( static_class<TestClass2>()->methods().size() == 2 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::name", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<TestClass1>()->name() == "test::TestClass1" );
    REQUIRE( static_type<TestClass2>()->name() == "TestClass2" );
}

