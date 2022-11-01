#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

#include <s4_rtti_test_interface.hpp>

using namespace rtti;
using namespace core::lib;
using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::==", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>() == static_type<bool>() );
    REQUIRE( static_type<int>() != static_type<float>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::== from differend dll", "[rtti::Fundamental]" ) {
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
        
    REQUIRE( static_type<bool>() == test_obj->get_bool_type() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::name", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->name() == "bool" );
    REQUIRE( static_type<double>()->name() == "double" );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::size", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->size() == sizeof(bool) );
    REQUIRE( static_type<float>()->size() == sizeof(float) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::as_fundamental", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_fundamental().ok() == static_type<bool>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::as_enum", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_enum().is_err() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::as_class", "[Fundamental]" ) {
    REQUIRE( static_type<bool>()->as_class().is_err() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::is_default_constructible", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<double>()->is_default_constructible() == true );
    REQUIRE( static_type<float>()->is_default_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::is_copy_constructible", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<double>()->is_copy_constructible() == true );
    REQUIRE( static_type<float>()->is_copy_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::is_move_constructible", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<double>()->is_move_constructible() == true );
    REQUIRE( static_type<float>()->is_move_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::is_copy_assignable", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<double>()->is_copy_assignable() == true );
    REQUIRE( static_type<float>()->is_copy_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::is_move_assignable", "[rtti::Fundamental]" ) {
    REQUIRE( static_type<double>()->is_move_assignable() == true );
    REQUIRE( static_type<float>()->is_move_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::new_default", "[rtti::Fundamental]" ) {
    Object obj = static_type<float>()->new_default().ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<float>() );
    REQUIRE( obj.size().ok() == sizeof(float) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::new_copy", "[rtti::Fundamental]" ) {
    Object src = static_type<int>()->new_default().ok();
    *src.value_as<int>().ok() = 4;
    Object dst = static_type<int>()->new_copy(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type<int>() );
    REQUIRE( dst.size().ok() == sizeof(int) );
    REQUIRE( *dst.value_as<int>().ok() == 4 );

    REQUIRE( static_type<int>()->new_copy(Object()).err() == Type::ErrNewCopy::NOT_VALID_SOURCE );

    src = static_type<float>()->new_default().ok();
    REQUIRE( static_type<int>()->new_copy(src).err() == Type::ErrNewCopy::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::new_move", "[rtti::Fundamental]" ) {
    Object src = static_type<int>()->new_default().ok();
    *src.value_as<int>().ok() = 6;
    Object dst = static_type<int>()->new_move(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type<int>() );
    REQUIRE( dst.size().ok() == sizeof(int) );
    REQUIRE( *dst.value_as<int>().ok() == 6 );

    Object invalid_src;
    REQUIRE( static_type<int>()->new_move(invalid_src).err() == Type::ErrNewMove::NOT_VALID_SOURCE );

    src = static_type<float>()->new_default().ok();
    REQUIRE( static_type<int>()->new_move(src).err() == Type::ErrNewMove::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::can_delete_object", "[rtti::Fundamental]" ) {
    Object src = static_type<int>()->new_default().ok();
    REQUIRE( static_type<int>()->can_delete_object(src).is_ok() == true );

    Object invalid_src;
    REQUIRE( 
        static_type<int>()->can_delete_object(invalid_src).err() 
        == 
        Type::ErrDeleteObject::NOT_VALID_SOURCE );

    src = static_type<float>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->can_delete_object(src).err() 
        == 
        Type::ErrDeleteObject::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::delete_object", "[rtti::Fundamental]" ) {
    Object src = static_type<int>()->new_default().ok();
    static_type<int>()->delete_object(std::move(src));
    REQUIRE( src.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::can_construct", "[rtti::Fundamental]" ) {
    Buffer src;
    REQUIRE( static_type<int>()->can_construct(src).err() == Type::ErrConstruct::INVALID_BUFFER );

    src = Buffer(1);
    REQUIRE( static_type<int>()->can_construct(src).err() == Type::ErrConstruct::BUFFER_TOO_SMALL );

    src = Buffer(sizeof(int));
    REQUIRE( static_type<int>()->can_construct(src).is_ok() == true );

    src = Buffer(sizeof(int) + 20);
    REQUIRE( static_type<int>()->can_construct(src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::construct", "[rtti::Fundamental]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type<long>()->construct(std::move(buff_ref));
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<long>() );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( obj_ref.size().ok() == 30 );

    Buffer buff = Buffer(sizeof(int));
    Object obj = static_type<int>()->construct(std::move(buff));
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<int>() );
    REQUIRE( obj.size().ok() == sizeof(int) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::can_copy_construct", "[rtti::Fundamental]" ) {
    Buffer buff;
    Object src;
    REQUIRE( 
        static_type<int>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type<int>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(int));
    REQUIRE( 
        static_type<int>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::NOT_VALID_SOURCE );

    src = static_type<float>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type<int>()->new_default().ok();
    REQUIRE( static_type<int>()->can_copy_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::copy_construct", "[rtti::Fundamental]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type<long>()->new_default().ok();
    *src.value_as<long>().ok() = 57;
    ObjectRef obj_ref = static_type<long>()->copy_construct(std::move(buff_ref), src);
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<long>() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( *obj_ref.value_as<long>().ok() == 57 );

    Buffer buff = Buffer(sizeof(long));
    Object obj = static_type<long>()->copy_construct(std::move(buff), src);
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<long>() );
    REQUIRE( obj.size().ok() == sizeof(long) );
    REQUIRE( *obj.value_as<long>().ok() == 57 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::can_move_construct", "[rtti::Fundamental]" ) {
    Buffer buff;
    Object src;
    REQUIRE( 
        static_type<int>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type<int>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(int));
    REQUIRE( 
        static_type<int>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::NOT_VALID_SOURCE );

    src = static_type<float>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type<int>()->new_default().ok();
    REQUIRE( static_type<int>()->can_move_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::move_construct", "[rtti::Fundamental]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type<long>()->new_default().ok();
    *src.value_as<long>().ok() = 57;
    ObjectRef obj_ref = static_type<long>()->move_construct(std::move(buff_ref), src);
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<long>() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( *obj_ref.value_as<long>().ok() == 57 );

    Buffer buff = Buffer(sizeof(long));
    Object obj = static_type<long>()->move_construct(std::move(buff), src);
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<long>() );
    REQUIRE( obj.size().ok() == sizeof(long) );
    REQUIRE( *obj.value_as<long>().ok() == 57 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::can_destruct", "[rtti::Fundamental]" ) {
    Object obj;
    REQUIRE( static_type<int>()->can_destruct(obj).err() == Type::ErrDestruct::NOT_VALID_OBJECT );
    
    obj = static_type<long>()->new_default().ok();
    REQUIRE( static_type<int>()->can_destruct(obj).err() == Type::ErrDestruct::INCORRECT_OBJECT_TYPE );
    
    obj = static_type<int>()->new_default().ok();
    REQUIRE( static_type<int>()->can_destruct(obj).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::destruct", "[rtti::Fundamental]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type<int>()->construct(std::move(buff_ref));
    buff_ref = static_type<int>()->destruct(std::move(obj_ref));
    REQUIRE( buff_ref.data().ok() == reinterpret_cast<void*>(src_array) );

    Object obj = static_type<int>()->new_default().ok();
    const void* obj_value_ptr = obj.value().ok();
    Buffer buff = static_type<int>()->destruct(std::move(obj));
    REQUIRE( buff.data().ok() == obj_value_ptr );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::copy_assign", "[rtti::Fundamental]" ) {
    Object src;
    Object dst;
    REQUIRE( 
        static_type<int>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_DESTINATION_OBJECT );
        
    dst = static_type<long>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE );
    
    dst = static_type<int>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_SOURCE_OBJECT );
        
    src = static_type<long>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE );
        
    src = static_type<int>()->new_default().ok();
    *src.value_as<int>().ok() = 6565656;
    REQUIRE( static_type<int>()->copy_assign(dst, src).is_ok() == true );
    REQUIRE( *dst.value_as<int>().ok() == 6565656 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Fundamental::move_assign", "[rtti::Fundamental]" ) {
    Object src;
    Object dst;
    REQUIRE( 
        static_type<int>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_DESTINATION_OBJECT );
        
    dst = static_type<long>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE );
    
    dst = static_type<int>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_SOURCE_OBJECT );
        
    src = static_type<long>()->new_default().ok();
    REQUIRE( 
        static_type<int>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE );
        
    src = static_type<int>()->new_default().ok();
    *src.value_as<int>().ok() = 6565656;
    REQUIRE( static_type<int>()->move_assign(dst, src).is_ok() == true );
    REQUIRE( *dst.value_as<int>().ok() == 6565656 );
}