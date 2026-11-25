#include "s2/character_state_flags_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_control_constructor_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/motion_work_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
void SetClothFlag(Character *character, std::uint8_t flags, std::int32_t enabled) {
    if (!character->model)
        return;
    ClothComponent *const cloth = static_cast<ClothComponent *>(charFindComp(character, CLOTH_COMPONENT));
    if (cloth) {
        if (enabled)
            cloth->mode |= flags;
        else
            cloth->mode &= ~flags;
    }
}

} // namespace s2
} // namespace shenmue

#endif
