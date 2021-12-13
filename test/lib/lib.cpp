#include "lib.hpp"

#include <rtti/rtti.hpp>

using namespace test;

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

//*************************************************************************************************
// StringId TestInterfaceImpl::register_string(const std::string& src) {
//     return StringId(src);
// }

extern "C" test::ITestInterface* create_test_interface() {
    return new TestInterfaceImpl{};
}