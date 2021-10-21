#include "rtti/rtti.hpp"

FUNDAMENTAL(bool)

namespace ecs {

    enum class ETimeSpeed {
        NORMAL,
        FAST,
        FASTER,
        FASTEST,
    };

    //ENUM(ecs::ETimeSpeed, )

    class Entity {
        CLASS(ecs::Entity)

    public:
        bool m_bool = false;
        FIELD(m_bool)

    }; // class Entity

} // namespace ecs

//****************************************************************************
int main() {
    const rtti::Type* bool_type = rtti::static_type<bool>();
    const rtti::Type* entity_type = rtti::static_type<ecs::Entity>();
    const rtti::Class* entity_class = rtti::static_class<ecs::Entity>();

    assert(bool_type != entity_type);
    assert(entity_class == entity_type);
}