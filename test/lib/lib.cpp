#include <rtti/rtti.hpp>
#include <s4_rtti_test_interface.hpp>

using namespace test;

#ifdef _WIN64
    #define S4_TEST_LIB_EXPORT __declspec(dllexport)
#else
    #define S4_TEST_LIB_EXPORT
#endif

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
class TestInterfaceImpl : public ITestInterface {
public:
    const rtti::Enum* get_test_enum_type() const override {
        return static_cast<const rtti::Enum*>(rtti::static_type<DynamicallyLoadedLibEnum>()); 
    }

    const rtti::Class* get_test_class_type() const override { 
        return rtti::static_class<DynamicallyLoadedLibClass>(); 
    }

    const rtti::Class* get_test_template_type() const override { 
        return rtti::static_class<DynamicallyLoadedLibTemplate<int>>(); 
    }

    const rtti::Type* get_bool_type() const override { return rtti::static_type<bool>(); }
    
}; // TestInterfaceImpl

extern "C" S4_TEST_LIB_EXPORT test::ITestInterface* create_test_interface() {
    return new TestInterfaceImpl{};
}