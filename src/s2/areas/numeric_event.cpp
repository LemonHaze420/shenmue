#include "s2/areas/numeric_event.hpp"
#include "s2/character.hpp"

#include "s2/event_runtime.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"

namespace shenmue::s2::areas {

// @verified - p
void ResetAreaAuthorCharacter(shen_name character, float) {
    const Vector3 position{.x = 0.0f, .y = 0.0f, .z = 0.0f};
    EV_AT_SetPos(character, AREA_AUTHOR_ROOT_POSITION_FLAGS, position);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}

// @verified - p
void EnableCharDisplayAndSetMotionworkData(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    mtwkSetExtraFlag(scenFindMotionwork(character), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
}

// @verified - p
void SetupAreaRyoSinAuthorCharacters() {
    EV_EnableCharacterDisplay(AREA_AUTHOR_RYO);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_EnableCharacterDisplay(AREA_AUTHOR_SIN);
    EV_SetCharaControllerFlags(AREA_AUTHOR_SIN, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_AUTHOR_SIN, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_SIN), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_SIN, false);
}

} // namespace shenmue::s2::areas
