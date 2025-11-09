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

    class TestClassNotDefaultConstructible {
    public:
        CLASS(test::TestClassNotDefaultConstructible)
        END_CLASS
        
        TestClassNotDefaultConstructible(int val) : m_val(val) {}
        TestClassNotDefaultConstructible(const TestClassNotDefaultConstructible& other) = default;
        TestClassNotDefaultConstructible(TestClassNotDefaultConstructible&& other) = default;
        TestClassNotDefaultConstructible& operator=(const TestClassNotDefaultConstructible& other) = default;
        TestClassNotDefaultConstructible& operator=(TestClassNotDefaultConstructible&& other) = default;

    private:
        int m_val = 0;
    };

    class TestClassNotCopyConstructible {
    public:
        CLASS(test::TestClassNotCopyConstructible)
        END_CLASS

        TestClassNotCopyConstructible() = default;
        TestClassNotCopyConstructible(const TestClassNotCopyConstructible& other) = delete; // DELETE
        TestClassNotCopyConstructible(TestClassNotCopyConstructible&& other) = default;
        TestClassNotCopyConstructible& operator=(const TestClassNotCopyConstructible& other) = default;
        TestClassNotCopyConstructible& operator=(TestClassNotCopyConstructible&& other) = default;
    };

    class TestClassNotMoveConstructible {
    public:
        CLASS(test::TestClassNotMoveConstructible)
        END_CLASS

        TestClassNotMoveConstructible() = default;
        TestClassNotMoveConstructible(const TestClassNotMoveConstructible& other) = default;
        TestClassNotMoveConstructible(TestClassNotMoveConstructible&& other) = delete; // DELETE
        TestClassNotMoveConstructible& operator=(const TestClassNotMoveConstructible& other) = default;
        TestClassNotMoveConstructible& operator=(TestClassNotMoveConstructible&& other) = default;
    };
}

REGISTER_CLASS(test, TestClass1)
END_REGISTER_CLASS

class TestClass2 {
public:
    CLASS(TestClass2)
    END_CLASS

    virtual ~TestClass2() = default;
};

class TestClassNotDestructible {
public:
    CLASS(TestClassNotDestructible)
    END_CLASS

private:
    ~TestClassNotDestructible() {}
};

class TestClassNotMoveAssignable {
public:
    CLASS(TestClassNotMoveAssignable)
    END_CLASS

    TestClassNotMoveAssignable() = default;
    ~TestClassNotMoveAssignable() {
        m_int_val = 0xDEADBEEF;
        if (deleted)
            *deleted = true;
    }
    TestClassNotMoveAssignable(const TestClassNotMoveAssignable& other) = default;
    TestClassNotMoveAssignable(TestClassNotMoveAssignable&& other) = default;
    TestClassNotMoveAssignable& operator=(const TestClassNotMoveAssignable& other) = default;
    TestClassNotMoveAssignable& operator=(TestClassNotMoveAssignable&& other) = delete; // DELETE
    
    unsigned int m_int_val = 0;
    bool* deleted = nullptr;
};

class TestClassNotCopyAssignable {
public:
    CLASS(TestClassNotCopyAssignable)
    END_CLASS

    TestClassNotCopyAssignable() = default;
    TestClassNotCopyAssignable(const TestClassNotCopyAssignable& other) = default;
    TestClassNotCopyAssignable(TestClassNotCopyAssignable&& other) = default;
    TestClassNotCopyAssignable& operator=(const TestClassNotCopyAssignable& other) = delete; // DELETE
    TestClassNotCopyAssignable& operator=(TestClassNotCopyAssignable&& other) {
        m_int_val = other.m_int_val;
        other.m_int_val = 0xDEADBEEF;
        return *this;
    }

    unsigned int m_int_val = 0;
};

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
        
    REQUIRE( static_type_trait<DynamicallyLoadedLibClass>::get() == test_obj->get_test_class_type() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::name", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClass1>::get()->name() == "test::TestClass1" );
    REQUIRE( static_type_trait<TestClass2>::get()->name() == "TestClass2" );
}


//*************************************************************************************************
TEST_CASE( "rtti::Class::size", "[rtti::Class]" ) {
    REQUIRE( 
        static_type_trait<TestClassNotDefaultConstructible>::get()->size() 
        == 
        sizeof(TestClassNotDefaultConstructible) );

    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->size() 
        == 
        sizeof(TestClassNotCopyAssignable) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::as_fundamental", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->as_fundamental().is_err() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::as_enum", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->as_enum().is_err() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::as_class", "[Fundamental]" ) {
    REQUIRE( 
        static_type_trait<TestClassNotDefaultConstructible>::get()->as_class().ok()
        == 
        static_type_trait<TestClassNotDefaultConstructible>::get() );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::is_default_constructible", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->is_default_constructible() == false );
    REQUIRE( static_type_trait<TestClassNotCopyAssignable>::get()->is_default_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::is_copy_constructible", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotCopyConstructible>::get()->is_copy_constructible() == false );
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->is_copy_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::is_move_constructible", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotMoveConstructible>::get()->is_move_constructible() == false );
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->is_move_constructible() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::is_copy_assignable", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotCopyAssignable>::get()->is_copy_assignable() == false );
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->is_copy_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::is_move_assignable", "[rtti::Class]" ) {
    REQUIRE( static_type_trait<TestClassNotMoveAssignable>::get()->is_move_assignable() == false );
    REQUIRE( static_type_trait<TestClassNotDefaultConstructible>::get()->is_move_assignable() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::alloc_construct", "[rtti::Class]" ) {
    Object obj = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( obj.size().ok() == sizeof(TestClassNotMoveAssignable) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::alloc_copy_construct", "[rtti::Class]" ) {
    Object src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotMoveAssignable>().ok()->m_int_val = 4;
    Object dst = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_copy_construct(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( dst.size().ok() == sizeof(TestClassNotMoveAssignable) );
    REQUIRE( dst.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 4 );

    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->alloc_copy_construct(Object()).err() 
        == 
        Type::ErrCopyConstruct::NOT_VALID_SOURCE );

    src = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->alloc_copy_construct(src).err() 
        == 
        Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::alloc_move_construct", "[rtti::Class]" ) {
    Object src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotMoveAssignable>().ok()->m_int_val = 6;
    Object dst = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_move_construct(src).ok();
    REQUIRE( dst.is_valid() == true );
    REQUIRE( dst.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( dst.size().ok() == sizeof(TestClassNotMoveAssignable) );
    REQUIRE( dst.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 6 );

    Object invalid_src;
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->alloc_move_construct(invalid_src).err() 
        ==
        Type::ErrMoveConstruct::NOT_VALID_SOURCE );

    src = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->alloc_move_construct(src).err()
        ==
        Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::can_construct", "[rtti::Class]" ) {
    Buffer src;

    REQUIRE( 
        static_type_trait<TestClassNotDefaultConstructible>::get()->can_construct(src).err() 
        == 
        Type::ErrConstruct::NOT_DEFAULT_CONSTRUCTIBLE );
    
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_construct(src).err() 
        == 
        Type::ErrConstruct::INVALID_BUFFER );

    src = Buffer(1);
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_construct(src).err() 
        == 
        Type::ErrConstruct::BUFFER_TOO_SMALL );

    src = Buffer(sizeof(TestClassNotMoveAssignable));
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_construct(src).is_ok() 
        == 
        true );

    src = Buffer(sizeof(TestClassNotMoveAssignable) + 20);
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_construct(src).is_ok() 
        == 
        true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::construct", "[rtti::Class]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type_trait<TestClassNotMoveConstructible>::get()->construct(std::move(buff_ref)).ok();
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type_trait<TestClassNotMoveConstructible>::get() );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( obj_ref.size().ok() == 30 );

    Buffer buff = Buffer(sizeof(TestClassNotMoveConstructible));
    Object obj = static_type_trait<TestClassNotMoveConstructible>::get()->construct(std::move(buff)).ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type_trait<TestClassNotMoveConstructible>::get() );
    REQUIRE( obj.size().ok() == sizeof(TestClassNotMoveConstructible) );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::can_copy_construct", "[rtti::Class]" ) {
    Buffer buff;
    Object src;
    
    REQUIRE( 
        static_type_trait<TestClassNotCopyConstructible>::get()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::NOT_COPY_CONSTRUCTIBLE );

    REQUIRE( 
        static_type_trait<TestClassNotMoveConstructible>::get()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type_trait<TestClassNotMoveConstructible>::get()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(TestClassNotMoveConstructible));
    REQUIRE( 
        static_type_trait<TestClassNotMoveConstructible>::get()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::NOT_VALID_SOURCE );

    src = static_type_trait<TestClassNotCopyConstructible>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveConstructible>::get()->can_copy_construct(buff, src).err() 
        == 
        Type::ErrCopyConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type_trait<TestClassNotMoveConstructible>::get()->alloc_construct().ok();
    REQUIRE( static_type_trait<TestClassNotMoveConstructible>::get()->can_copy_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::copy_construct", "[rtti::Class]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotMoveAssignable>().ok()->m_int_val = 57;
    ObjectRef obj_ref = static_type_trait<TestClassNotMoveAssignable>::get()->copy_construct(std::move(buff_ref), src).ok();
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( obj_ref.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 57 );

    Buffer buff = Buffer(sizeof(TestClassNotMoveAssignable));
    Object obj = static_type_trait<TestClassNotMoveAssignable>::get()->copy_construct(std::move(buff), src).ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( obj.size().ok() == sizeof(TestClassNotMoveAssignable) );
    REQUIRE( obj.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 57 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::can_move_construct", "[rtti::Class]" ) {
    Buffer buff;
    Object src;
    
    REQUIRE( 
        static_type_trait<TestClassNotMoveConstructible>::get()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::NOT_MOVE_CONSTRUCTIBLE );
    
    REQUIRE( 
        static_type_trait<TestClassNotCopyConstructible>::get()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INVALID_BUFFER );

    buff = Buffer(1);
    REQUIRE( 
        static_type_trait<TestClassNotCopyConstructible>::get()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::BUFFER_TOO_SMALL );

    buff = Buffer(sizeof(TestClassNotCopyConstructible));
    REQUIRE( 
        static_type_trait<TestClassNotCopyConstructible>::get()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::NOT_VALID_SOURCE );

    src = static_type_trait<TestClassNotMoveConstructible>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotCopyConstructible>::get()->can_move_construct(buff, src).err() 
        == 
        Type::ErrMoveConstruct::INCORRECT_SOURCE_TYPE );

    src = static_type_trait<TestClassNotCopyConstructible>::get()->alloc_construct().ok();
    REQUIRE( static_type_trait<TestClassNotCopyConstructible>::get()->can_move_construct(buff, src).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::move_construct", "[rtti::Class]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    Object src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotMoveAssignable>().ok()->m_int_val = 57;
    ObjectRef obj_ref = static_type_trait<TestClassNotMoveAssignable>::get()->move_construct(std::move(buff_ref), src).ok();
    REQUIRE( obj_ref.is_valid() == true );
    REQUIRE( obj_ref.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( obj_ref.size().ok() == 30 );
    REQUIRE( obj_ref.value().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( obj_ref.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 57 );

    Buffer buff = Buffer(sizeof(TestClassNotMoveAssignable));
    Object obj = static_type_trait<TestClassNotMoveAssignable>::get()->move_construct(std::move(buff), src).ok();
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.type().ok() == static_type_trait<TestClassNotMoveAssignable>::get() );
    REQUIRE( obj.size().ok() == sizeof(TestClassNotMoveAssignable) );
    REQUIRE( obj.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 57 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::can_destruct", "[rtti::Class]" ) {
    Object obj;

    REQUIRE( 
        static_type_trait<TestClassNotDestructible>::get()->can_destruct(obj).err() 
        == 
        Type::ErrDestruct::NOT_DESTRUCTIBLE );

    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_destruct(obj).err() 
        == 
        Type::ErrDestruct::NOT_VALID_OBJECT );
    
    obj = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->can_destruct(obj).err() 
        == 
        Type::ErrDestruct::INCORRECT_OBJECT_TYPE );
    
    obj = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    REQUIRE( static_type_trait<TestClassNotMoveAssignable>::get()->can_destruct(obj).is_ok() == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::dealloc_destruct", "[rtti::Class]" ) {
    Object obj = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    const void* obj_value_ptr = obj.value().ok();
    bool deleted = false;
    obj.value_as<TestClassNotMoveAssignable>().ok()->deleted = &deleted;
    REQUIRE( static_type_trait<TestClassNotMoveAssignable>::get()->dealloc_destruct(std::move(obj)).is_ok() == true );
    REQUIRE( deleted == true );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::destruct", "[rtti::Class]" ) {
    char src_array[30];
    BufferRef buff_ref(reinterpret_cast<void*>(src_array), 30);
    ObjectRef obj_ref = static_type_trait<TestClassNotMoveAssignable>::get()->construct(std::move(buff_ref)).ok();
    buff_ref = static_type_trait<TestClassNotMoveAssignable>::get()->destruct(std::move(obj_ref)).ok();
    REQUIRE( buff_ref.data().ok() == reinterpret_cast<void*>(src_array) );
    REQUIRE( reinterpret_cast<TestClassNotMoveAssignable*>(src_array)->m_int_val == 0xDEADBEEF );

    Object obj = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    const void* obj_value_ptr = obj.value().ok();
    Buffer buff = static_type_trait<TestClassNotMoveAssignable>::get()->destruct(std::move(obj)).ok();
    REQUIRE( buff.data().ok() == obj_value_ptr );
    REQUIRE( reinterpret_cast<TestClassNotMoveAssignable*>(const_cast<void*>(obj_value_ptr))->m_int_val == 0xDEADBEEF );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::copy_assign", "[rtti::Class]" ) {
    Object src;
    Object dst;

    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::NOT_COPY_ASSIGNABLE);
        
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_DESTINATION_OBJECT);

    dst = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_DESTINATION_OBJECT_TYPE );

    dst = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INVALID_SOURCE_OBJECT );

    src = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE(
        static_type_trait<TestClassNotMoveAssignable>::get()->copy_assign(dst, src).err() 
        == 
        Type::ErrCopy::INCORRECT_SOURCE_OBJECT_TYPE );

    src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotMoveAssignable>().ok()->m_int_val = 6565656;
    REQUIRE( static_type_trait<TestClassNotMoveAssignable>::get()->copy_assign(dst, src).is_ok() == true );
    REQUIRE( dst.value_as<TestClassNotMoveAssignable>().ok()->m_int_val == 6565656 );
}

//*************************************************************************************************
TEST_CASE( "rtti::Class::move_assign", "[rtti::Class]" ) {
    Object src;
    Object dst;

    REQUIRE( 
        static_type_trait<TestClassNotMoveAssignable>::get()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::NOT_MOVE_ASSIGNABLE);
        
    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_DESTINATION_OBJECT);
        
    dst = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_DESTINATION_OBJECT_TYPE );
    
    dst = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INVALID_SOURCE_OBJECT );
        
    src = static_type_trait<TestClassNotMoveAssignable>::get()->alloc_construct().ok();
    REQUIRE( 
        static_type_trait<TestClassNotCopyAssignable>::get()->move_assign(dst, src).err() 
        == 
        Type::ErrMove::INCORRECT_SOURCE_OBJECT_TYPE );
        
    src = static_type_trait<TestClassNotCopyAssignable>::get()->alloc_construct().ok();
    src.value_as<TestClassNotCopyAssignable>().ok()->m_int_val = 6565656;
    REQUIRE( static_type_trait<TestClassNotCopyAssignable>::get()->move_assign(dst, src).is_ok() == true );
    REQUIRE( dst.value_as<TestClassNotCopyAssignable>().ok()->m_int_val == 6565656 );
    REQUIRE( src.value_as<TestClassNotCopyAssignable>().ok()->m_int_val == 0xDEADBEEF );
}