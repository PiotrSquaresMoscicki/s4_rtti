#include "lib.hpp"

#include <rtti/rtti.hpp>

using namespace test;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
class TestInterfaceImpl : public ITestInterface {
public:
    const rtti::Type* get_bool_type() const override { return rtti::static_type<bool>(); }
    
}; // TestInterfaceImpl

//*************************************************************************************************
// StringId TestInterfaceImpl::register_string(const std::string& src) {
//     return StringId(src);
// }

extern "C" test::ITestInterface* create_test_interface() {
    return new TestInterfaceImpl{};
}