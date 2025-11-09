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
    };

    // class TestClassNotDefaultConstructible {
    // public:
    //     CLASS(TestClassNotDefaultConstructible)
    //     END_CLASS
        
    //     TestClassNotDefaultConstructible(int val) : m_val(val) {}
        
    // private:
    //     int m_val = 0;
    // };

    // class TestClassNotCopyConstructible {
    // public:
    //     CLASS(TestClassNotCopyConstructible)
    //     END_CLASS

    //     TestClassNotCopyConstructible() = default;
    //     TestClassNotCopyConstructible(const TestClassNotCopyConstructible& other) = delete;
    // };

    // class TestClassNotMoveConstructible {
    // public:
    //     CLASS(TestClassNotMoveConstructible)
    //     END_CLASS
    //     TestClassNotMoveConstructible() = default;
    //     TestClassNotMoveConstructible(TestClassNotMoveConstructible&& other) = delete;
    // };
}

REGISTER_CLASS(test, TestClass1)
END_REGISTER_CLASS

class TestClass2 {
public:
    CLASS(TestClass2)
    END_CLASS

    virtual ~TestClass2() = default;
};

// class TestClassNotMoveAssignable {
// public:
//     CLASS(TestClassNotMoveAssignable)
//     END_CLASS

//     TestClassNotMoveAssignable() = default;
//     TestClassNotMoveAssignable(TestClassNotMoveAssignable&& other) = delete;
// };

// class TestClassNotCopyAssignable {
// public:
//     CLASS(TestClassNotCopyAssignable)
//     END_CLASS

//     TestClassNotCopyAssignable() = default;
//     TestClassNotCopyAssignable(const TestClassNotCopyAssignable& other) = delete;
// };

using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Class::== from differend dll", "[rtti::Class]" ) {
    std::string shared_lib_path;

    #ifdef __APPLE__
        shared_lib_path = "libs4_rtti_test_lib.dylib";
    #elif __linux__
        shared_lib_path = "libs4_rtti_test_lib.so";
    #elif _WIN32
        shared_lib_path = "Debug/s4_rtti_test_lib.dll";
    #endif 
    
    
    Shared lib = Shared::open(std::string(S4_RTTI_PROJECT_FULL_DIST_DIR) + "/" + shared_lib_path).ok();
    ITestInterface* test_obj 
       = reinterpret_cast<ITestInterface*(*)()>(lib.symbol("create_test_interface").ok())();
        
    REQUIRE( static_type<DynamicallyLoadedLibClass>() == test_obj->get_test_class_type() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::name", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<TestClass1>()->name() == "test::TestClass1" );
    REQUIRE( static_type<TestClass2>()->name() == "TestClass2" );
}

