#include <catch2/catch_test_macros.hpp>
#include <core/lib/shared.hpp>
#include <rtti/rtti.hpp>
#include <s4_rtti_cmake_config.hpp>

using namespace rtti;
using namespace core::lib;

namespace rtti::test::function {
    int sum(int a, int b) { return a + b; }
}


using namespace rtti::test::function;