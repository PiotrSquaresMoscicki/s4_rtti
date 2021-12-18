#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

namespace object_test {

    //*************************************************************************************************
    //*************************************************************************************************
    //*************************************************************************************************
    class TestClass {
    public:
        CLASS(object_test::TestClass)
        END_CLASS

        virtual ~TestClass() { *m_destructed = true; }
        void init(bool* destructed) { m_destructed = destructed; }

    private:
        bool* m_destructed;

    }; // class TestClass

} // namespace object_test

using namespace rtti;
using namespace object_test;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
TEST_CASE( "Default ObjectRef", "[ObjectRef]" ) {
    ObjectRef ref;
    REQUIRE( ref.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "Initialized ObjectRef", "[ObjectRef]" ) {
    int test_variable = 5;
    ObjectRef ref(&test_variable);
    REQUIRE( ref.is_valid() == true );
    REQUIRE( ref.value().ok() == &test_variable );
    REQUIRE( *reinterpret_cast<const int*>(ref.value().ok()) == test_variable );
    REQUIRE( ref.type().ok() == static_type<int>() );
}

//*************************************************************************************************
TEST_CASE( "ObjectRef going out of scope", "[ObjectRef]" ) {
    bool destructed = false;
    TestClass* test_obj = new TestClass();
    test_obj->init(&destructed);
    
    {
        ObjectRef ref(test_obj);
    }
    
    REQUIRE( destructed == false );
}

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
TEST_CASE( "Default Object", "[Object]" ) {
    Object obj;
    REQUIRE( obj.is_valid() == false );
}

//*************************************************************************************************
TEST_CASE( "Initialized Object", "[Object]" ) {
    int* test_variable = new int(5);
    Object obj(test_variable);
    REQUIRE( obj.is_valid() == true );
    REQUIRE( obj.value().ok() == test_variable );
    REQUIRE( *reinterpret_cast<const int*>(obj.value().ok()) == *test_variable );
    REQUIRE( obj.type().ok() == static_type<int>() );
}

//*************************************************************************************************
TEST_CASE( "Object going out of scope", "[Object]" ) {
    bool destructed = false;
    TestClass* test_obj = new TestClass();
    test_obj->init(&destructed);
    
    {
        Object obj(test_obj);
    }
    
    REQUIRE( destructed == true );
}
