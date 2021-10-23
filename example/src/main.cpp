#include <rtti/rtti.hpp>

namespace ecs {

    enum class ETimeSpeed {
        NORMAL,
        FAST,
        FASTER,
        FASTEST,
    };

    class Entity {
        CLASS(ecs::Entity)

    public:
        bool m_bool = false;
        FIELD(m_bool)

    }; // class Entity

} // namespace ecs

REGISTER_ENUM(ecs::ETimeSpeed)
    ENUM_VALUE(NORMAL)
    ENUM_VALUE(FAST)
    ENUM_VALUE(FASTER)
    ENUM_VALUE(FASTEST)
END_ENUM

//****************************************************************************
int main() {
    const rtti::Type* bool_type = rtti::static_type<bool>();
    const rtti::Type* entity_type = rtti::static_type<ecs::Entity>();
    const rtti::Class* entity_class = rtti::static_class<ecs::Entity>();

    assert(bool_type != entity_type);
    assert(entity_class == entity_type);

    return 0;
}