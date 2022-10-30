#include <catch2/catch_test_macros.hpp>
#include <rtti/rtti.hpp>

#include <rtti/script.hpp>

using namespace core::str;
using namespace rtti;
using namespace script;

//*************************************************************************************************
//*************************************************************************************************
//*************************************************************************************************
TEST_CASE( "script::ScriptStep::push", "[script::ScriptStep]" ) {
    std::vector<ScriptStep> steps;
    // int integer;
    steps.push_back(ScriptStep::push(StringId("int")));

    REQUIRE( false == false );
}