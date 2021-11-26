#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

using namespace rtti;

namespace test {
    
    enum class TestEnum1 {
        VAL_1,
        VAL_2,
        VAL_3
    };

}

REGISTER_ENUM(test::TestEnum1)
    ENUM_VALUE(VAL_1)
    ENUM_VALUE(VAL_2)
    ENUM_VALUE(VAL_3)
END_ENUM
    
enum class TestEnum2 : long {
    VAL_4,
    VAL_5,
    VAL_6
};

REGISTER_ENUM(TestEnum2)
    ENUM_VALUE(VAL_4)
    ENUM_VALUE(VAL_5)
    ENUM_VALUE(VAL_6)
END_ENUM

using namespace test;

//*************************************************************************************************
TEST_CASE( "rtti::Enum::==", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum2>() == static_type<TestEnum2>() );
    REQUIRE( static_type<TestEnum2>() != static_type<TestEnum1>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::name", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->name() == "test::TestEnum1" );
    REQUIRE( static_type<TestEnum2>()->name() == "TestEnum2" );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::size", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->size() == sizeof(TestEnum1) );
    REQUIRE( static_type<TestEnum2>()->size() == sizeof(TestEnum2) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::as_fundamental", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->as_fundamental() == nullptr );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::as_enum", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->as_enum() == static_type<TestEnum1>() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::as_class", "[Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->as_class() == nullptr );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::is_default_constructible", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->is_default_constructible() == true );
    REQUIRE( static_type<TestEnum2>()->is_default_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::is_copy_constructible", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->is_copy_constructible() == true );
    REQUIRE( static_type<TestEnum2>()->is_copy_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::is_move_constructible", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->is_move_constructible() == true );
    REQUIRE( static_type<TestEnum2>()->is_move_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::is_copy_assignable", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->is_copy_assignable() == true );
    REQUIRE( static_type<TestEnum2>()->is_copy_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::is_move_assignable", "[rtti::Enum]" ) {
    REQUIRE( static_type<TestEnum1>()->is_move_assignable() == true );
    REQUIRE( static_type<TestEnum2>()->is_move_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::new_object", "[rtti::Enum]" ) {
    Object obj = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<TestEnum1>() );
    REQUIRE( obj.size().ok() == sizeof(TestEnum1) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::new_copy", "[rtti::Enum]" ) {
    Object src = static_type<TestEnum1>()->new_object().ok();
    *src.value_as<TestEnum1>().ok() = TestEnum1::VAL_1;
    Object dst = static_type<TestEnum1>()->new_copy(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type<TestEnum1>() );
    REQUIRE( dst.size().ok() == sizeof(TestEnum1) );
    REQUIRE( *dst.value_as<TestEnum1>().ok() == TestEnum1::VAL_1 );

    REQUIRE( static_type<TestEnum1>()->new_copy(Object()).err() == Type::ErrNewCopy::NOT_VALID_SOURCE );

    src = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->new_copy(src).err() == Type::ErrNewCopy::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::new_move", "[rtti::Enum]" ) {
    Object src = static_type<TestEnum1>()->new_object().ok();
    *src.value_as<TestEnum1>().ok() = TestEnum1::VAL_3;
    Object dst = static_type<TestEnum1>()->new_move(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type<TestEnum1>() );
    REQUIRE( dst.size().ok() == sizeof(TestEnum1) );
    REQUIRE( *dst.value_as<TestEnum1>().ok() == TestEnum1::VAL_3 );

    Object invalid_src;
    REQUIRE( static_type<TestEnum1>()->new_move(invalid_src).err() == Type::ErrNewMove::NOT_VALID_SOURCE );

    src = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->new_move(src).err() == Type::ErrNewMove::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::can_delete_object", "[rtti::Enum]" ) {
    Object src = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->can_delete_object(src).is_ok() == true );

    Object invalid_src;
    REQUIRE( 
        static_type<TestEnum1>()->can_delete_object(invalid_src).err() 
        == 
        Type::ErrDeleteObject::NOT_VALID_SOURCE );

    src = static_type<float>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->can_delete_object(src).err() 
        == 
        Type::ErrDeleteObject::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::delete_object", "[rtti::Enum]" ) {
    Object src = static_type<TestEnum1>()->new_object().ok();
    static_type<TestEnum1>()->delete_object(std::move(src));
    REQUIRE( src.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::can_construct", "[rtti::Enum]" ) {
    Buffer src;
    REQUIRE( static_type<TestEnum1>()->can_construct(src).err() == Type::ErrConstruct::INVALID_BUFFER );

    src = Buffer(1);
    REQUIRE( static_type<TestEnum1>()->can_construct(src).err() == Type::ErrConstruct::BUFFER_TOO_SMALL );

    src = Buffer(sizeof(TestEnum1));
    REQUIRE( static_type<TestEnum1>()->can_construct(src).is_ok() == true );

    src = Buffer(sizeof(TestEnum1) + 20);
    REQUIRE( static_type<TestEnum1>()->can_construct(src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::construct", "[rtti::Enum]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type<TestEnum2>()->construct(std::move(buff_ref));
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<TestEnum2>() );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( obj_ref.size().ok() == 30 );

    Buffer buff = Buffer(sizeof(TestEnum1));
    Object obj = static_type<TestEnum1>()->construct(std::move(buff));
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<TestEnum1>() );
    REQUIRE( obj.size().ok() == sizeof(TestEnum1) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::can_copy_construct", "[rtti::Enum]" ) {
    Buffer buff;
    Object src;
    REQUIRE( 
        static_type<TestEnum1>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type<TestEnum1>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(TestEnum1));
    REQUIRE( 
        static_type<TestEnum1>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::NOT_VALID_SOURCE );

    src = static_type<float>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->can_copy_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::copy_construct", "[rtti::Enum]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type<TestEnum2>()->new_object().ok();
    *src.value_as<TestEnum2>().ok() = TestEnum2::VAL_4;
    ObjectRef obj_ref = static_type<TestEnum2>()->copy_construct(std::move(buff_ref), src);
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<TestEnum2>() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( *obj_ref.value_as<TestEnum2>().ok() == TestEnum2::VAL_4 );

    Buffer buff = Buffer(sizeof(TestEnum2));
    Object obj = static_type<TestEnum2>()->copy_construct(std::move(buff), src);
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<TestEnum2>() );
    REQUIRE( obj.size().ok() == sizeof(TestEnum2) );
    REQUIRE( *obj.value_as<TestEnum2>().ok() == TestEnum2::VAL_4 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::can_move_construct", "[rtti::Enum]" ) {
    Buffer buff;
    Object src;
    REQUIRE( 
        static_type<TestEnum1>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type<TestEnum1>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(TestEnum1));
    REQUIRE( 
        static_type<TestEnum1>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::NOT_VALID_SOURCE );

    src = static_type<float>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->can_move_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::move_construct", "[rtti::Enum]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type<TestEnum2>()->new_object().ok();
    *src.value_as<TestEnum2>().ok() = TestEnum2::VAL_4;
    ObjectRef obj_ref = static_type<TestEnum2>()->move_construct(std::move(buff_ref), src);
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type<TestEnum2>() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( *obj_ref.value_as<TestEnum2>().ok() == TestEnum2::VAL_4 );

    Buffer buff = Buffer(sizeof(TestEnum2));
    Object obj = static_type<TestEnum2>()->move_construct(std::move(buff), src);
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type<TestEnum2>() );
    REQUIRE( obj.size().ok() == sizeof(TestEnum2) );
    REQUIRE( *obj.value_as<TestEnum2>().ok() == TestEnum2::VAL_4 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::can_destruct", "[rtti::Enum]" ) {
    Object obj;
    REQUIRE( static_type<TestEnum1>()->can_destruct(obj).err() == Type::ErrDestruct::NOT_VALID_OBJECT );
    
    obj = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->can_destruct(obj).err() == Type::ErrDestruct::INCORRECT_OBJECT_TYPE );
    
    obj = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( static_type<TestEnum1>()->can_destruct(obj).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::destruct", "[rtti::Enum]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type<TestEnum1>()->construct(std::move(buff_ref));
    buff_ref = static_type<TestEnum1>()->destruct(std::move(obj_ref));
    REQUIRE( buff_ref.data().ok() == reinterpret_cast<void*>(src_array) );

    Object obj = static_type<TestEnum1>()->new_object().ok();
    const void* obj_value_ptr = obj.value().ok();
    Buffer buff = static_type<TestEnum1>()->destruct(std::move(obj));
    REQUIRE( buff.data().ok() == obj_value_ptr );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::copy_assign", "[rtti::Enum]" ) {
    Object src;
    Object dst;
    REQUIRE( 
        static_type<TestEnum1>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_DESTINATION_OBJECT );
        
    dst = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE );
    
    dst = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_SOURCE_OBJECT );
        
    src = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE );
        
    src = static_type<TestEnum1>()->new_object().ok();
    *src.value_as<TestEnum1>().ok() = TestEnum1::VAL_3;
    REQUIRE( static_type<TestEnum1>()->copy_assign(dst, src).is_ok() == true );
    REQUIRE( *dst.value_as<TestEnum1>().ok() == TestEnum1::VAL_3 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Enum::move_assign", "[rtti::Enum]" ) {
    Object src;
    Object dst;
    REQUIRE( 
        static_type<TestEnum1>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_DESTINATION_OBJECT );
        
    dst = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE );
    
    dst = static_type<TestEnum1>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_SOURCE_OBJECT );
        
    src = static_type<TestEnum2>()->new_object().ok();
    REQUIRE( 
        static_type<TestEnum1>()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE );
        
    src = static_type<TestEnum1>()->new_object().ok();
    *src.value_as<TestEnum1>().ok() = TestEnum1::VAL_2;
    REQUIRE( static_type<TestEnum1>()->move_assign(dst, src).is_ok() == true );
    REQUIRE( *dst.value_as<TestEnum1>().ok() == TestEnum1::VAL_2 );
}
