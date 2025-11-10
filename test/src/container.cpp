#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;
using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Container::==", "[rtti::Container]" ) {
    REQUIRE( static_type<std::vector<int>>::get() == static_type<std::vector<int>>::get() );
    REQUIRE( static_type<std::vector<int>>::get() != static_type<std::vector<float>>::get() );
    REQUIRE( static_type<std::map<int, float>>::get() == static_type<std::map<int, float>>::get() );
    REQUIRE( static_type<std::map<int, float>>::get() != static_type<std::map<float, int>>::get() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::== from different dll", "[rtti::Container]" ) {
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
        
    REQUIRE( static_type<std::vector<int>>::get() == test_obj->get_std_vector_int_type() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::name", "[rtti::Container]" ) {
    REQUIRE( 
        static_type<std::vector<int>>::get()->name() 
        == 
        "std::vector<int,std::allocator<int>>" );

    REQUIRE( 
        static_type<std::vector<float>>::get()->name() 
        == 
        "std::vector<float,std::allocator<float>>" );

// TODO ideally it should be
//      "std::map<int,float,std::less<int>,std::allocator<std::pair<int,float>>>"
//      but since I don't differentiate between const and non const types yet 
//      i'll leave it like this for now 
    REQUIRE( 
        static_type<std::map<int, float>>::get()->name() 
        == 
        "std::map<int,float,std::less<int>,std::allocator<std::pair<int,float>>>" );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::size", "[rtti::Container]" ) {
    REQUIRE( static_type<std::vector<int>>::get()->size() == sizeof(std::vector<int>) );
    REQUIRE( static_type<std::map<int, float>>::get()->size() == sizeof(std::map<int, float>) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::as_container", "[rtti::Container]" ) {
    REQUIRE( 
        static_type<std::vector<int>>::get()->as_container().ok() 
        == 
        static_type<std::vector<int>>::get() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::as_enum", "[rtti::Container]" ) {
    REQUIRE( static_type<std::vector<int>>::get()->as_enum().is_err() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Container::as_class", "[Container]" ) {
    REQUIRE( static_type<std::vector<int>>::get()->as_class().is_err() );
}