#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;

namespace rtti::test::method {

    class TestClass1 {
    public:
        short sub(short first, short second) { return first - second; }
    };

    class TestClass2 {
    public:
        CLASS(rtti::test::method::TestClass2)
            METHOD(first_method, ())
            METHOD(sum, (a b))
        END_CLASS

        virtual ~TestClass2() = default;
        void first_method() {}

        int sum(int a, int b) { return a + b; }
    };
}

REGISTER_CLASS(rtti::test::method, TestClass1)
    METHOD(sub, (first, second))
END_REGISTER_CLASS

using namespace rtti::test::method;
//using namespace test;

//*************************************************************************************************
// TEST_CASE( "rtti::Class::== from differend dll", "[rtti::Class]" ) {
//     std::string shared_lib_path;

//     #ifdef __APPLE__
//         shared_lib_path = "libs4_rtti_test_lib.dylib";
//     #elif __linux__
//         shared_lib_path = "libs4_rtti_test_lib.so";
//     #elif _WIN32
//         shared_lib_path = "libs4_rtti_test_lib.dll";
//     #endif 
    
    
//     Shared lib = Shared::open(std::string(S4_RTTI_PROJECT_FULL_DIST_DIR) + "/" + shared_lib_path).ok();
//     ITestInterface* test_obj 
//        = reinterpret_cast<ITestInterface*(*)()>(lib.symbol("create_test_interface").ok())();
        
//     REQUIRE( static_type<DynamicallyLoadedLibClass>() == test_obj->get_test_class_type() );
// }

//*************************************************************************************************
TEST_CASE( "rtti::Method::==", "[rtti::Method]" ) {
    //REQUIRE( static_class<TestClass2>()->method );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::methods", "[rtti::Class]" ) {
    REQUIRE( static_class<TestClass1>()->methods().size() == 1 );
    REQUIRE( static_class<TestClass2>()->methods().size() == 2 );
}

//*************************************************************************************************
// TEST_CASE( "rtti::Class::name", "[rtti::Fundamental]" ) {
//     REQUIRE( static_type<TestClass1>()->name() == "test::TestClass1" );
//     REQUIRE( static_type<TestClass2>()->name() == "TestClass2" );
// }

