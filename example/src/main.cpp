#include <rtti/rtti.hpp>

namespace ecs {

    enum class ETimeSpeed {
        NORMAL,
        FAST,
        FASTER,
        FASTEST,
    };

    // class Entity {
    //     CLASS(ecs::Entity)

    // public:
    //     virtual ~Entity() = default;

    //     bool m_bool = false;
    //     FIELD(m_bool)

    // }; // class Entity

} // namespace ecs

// REGISTER_ENUM(ecs::ETimeSpeed)
//     ENUM_VALUE(NORMAL)
//     ENUM_VALUE(FAST)
//     ENUM_VALUE(FASTER)
//     ENUM_VALUE(FASTEST)
// END_ENUM

//****************************************************************************
int main() {
    // const rtti::Type* bool_type = rtti::static_type_trait<bool>::get();
    // const rtti::Type* entity_type = rtti::static_type_trait<ecs::Entity>::get();
    // const rtti::Class* entity_class = rtti::static_class_trait<ecs::Entity>::get();

    // assert(bool_type != entity_type);
    // assert(entity_class == entity_type);

    return 0;
}