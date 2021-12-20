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
    rtti::EnumPtr get_test_enum_type() const override {
        return rtti::static_type<DynamicallyLoadedLibEnum>()->as_enum().ok(); 
    }

    rtti::ClassPtr get_test_class_type() const override { 
        return rtti::static_class<DynamicallyLoadedLibClass>(); 
    }

    rtti::ClassPtr get_test_template_type() const override { 
        return rtti::static_class<DynamicallyLoadedLibTemplate<int>>(); 
    }

    rtti::TypePtr get_bool_type() const override { return rtti::static_type<bool>().get(); }
    
}; // TestInterfaceImpl

extern "C" S4_TEST_LIB_EXPORT test::ITestInterface* create_test_interface() {
    return new TestInterfaceImpl{};
}