#include "rtti.hpp"

using namespace rtti;

Res<const Function*, ErrFunction> function(StringId name) {
    return Err(ErrFunction::NOT_FOUND);
}

int sum(int a, int b) {
    return a + b;
}

// float sum(float a, float b) {
//     return a + b;
// }

REGISTER_FUNCTION(int, sum, int a COMMA int b)
//REGISTER_FUNCTION(float, sum, float a COMMA float b)