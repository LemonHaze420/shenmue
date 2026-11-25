#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/sdk.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/player_ground_state_dc.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

std::int32_t AngleDifference16(std::int32_t from, std::int32_t to);
void mtwkSampleQueuedMotionRoot(MotionWork *work, float frame, Vector3 *output);

std::int32_t StepCharacterYawTowardPoint(Character *character, const Vector3 *target, std::int32_t step) {
    Vector3 position;
    Vector3i rotation;
    GetCharacterLocalPosition(character, &position);
    GetCharacterLocalRotation(character, &rotation);
    const std::uint32_t angle = dc::sdk::ArcTan2Fixed(target->x - position.x, target->z - position.z);
    const std::int32_t difference = AngleDifference16(static_cast<std::uint16_t>(angle + 0x8000), rotation.y);
    std::int32_t turning;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int32_t stepLimit = step;
    if (difference < 0 ? -difference > step : difference > stepLimit) {
#else
    if (difference >= 0 ? difference > step : -difference > step) {
#endif
        if (difference < 0)
            rotation.y += step;
        else
            rotation.y -= step;
        turning = 1;
    } else {
        turning = 0;
    }
    SetCharacterLocalRotation(character, &rotation);
    return turning;
}

// @verified - d
void SetCharacterDisplayEnabled(Character *character, std::int32_t enabled) {
    EV_SetCharDisplay(character, CVM_DisplayEnabled, enabled);
}

// @verified - d
void vfPlacePairFromMotionRoots(Character *first, Character *second) {
    MotionWork *const firstWork = charFindMotionwork(first);
    MotionWork *const secondWork = charFindMotionwork(second);
    Vector3 firstRotated, secondRotated, firstPosition, secondPosition;
    mtwkSampleQueuedMotionRoot(firstWork, 1.0f, &firstPosition);
    mtwkSampleQueuedMotionRoot(secondWork, 1.0f, &secondPosition);
    const float firstDistance = std::sqrt(firstPosition.x * firstPosition.x + firstPosition.z * firstPosition.z);
    const float secondDistance = std::sqrt(secondPosition.x * secondPosition.x + secondPosition.z * secondPosition.z);
    const std::int16_t yaw = static_cast<std::int16_t>(shenGetRotYFromNode(second));
    vfRotateFightVector(yaw, &firstPosition, &firstRotated);
    vfRotateFightVector(yaw, &secondPosition, &secondRotated);
    GetCharacterLocalPosition(first, &firstPosition);
    GetCharacterLocalPosition(second, &secondPosition);
    if (secondDistance > firstDistance) {
        secondPosition.x = firstPosition.x - secondRotated.x;
        secondPosition.z = firstPosition.z - secondRotated.z;
        firstPosition.x = firstPosition.x - firstRotated.x;
        firstPosition.z = firstPosition.z - firstRotated.z;
    } else {
        firstPosition.x = secondPosition.x - firstRotated.x;
        firstPosition.z = secondPosition.z - firstRotated.z;
        secondPosition.x = secondPosition.x - secondRotated.x;
        secondPosition.z = secondPosition.z - secondRotated.z;
    }
    SetCharacterLocalPosition(first, &firstPosition);
    SetCharacterLocalPosition(second, &secondPosition);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void vfPlacementNullsub() {
}
#endif

} // namespace s2
} // namespace shenmue

#endif
