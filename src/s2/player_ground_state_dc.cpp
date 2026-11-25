#include "s2/player_ground_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/map_control_dc.hpp"
#include "s2/player_model_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

std::int32_t shenGetRotYFromNode(const Character *character);

// @verified - d
std::int32_t playerGetRotY() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<std::int16_t>(static_cast<std::uint16_t>(shenGetRotYFromNode(control->ryo)));
}

// @verified - d
Vector3 *plymUpdatePlayerSearchLocation(Vector3 *position) {
    if (position) {
        const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        GetCharacterWorldPosition(control->ryo, position);
        return 0;
    }
    std::byte *const model = reinterpret_cast<std::byte *>(g_playerRuntimePointer->model);
    if (!model)
        return 0;
    return reinterpret_cast<Vector3 *>(model + kPlayerSearchLocationOffset);
}

// @verified - d
std::uint32_t plymGetAuxiliaryState() {
    const PlayerModelState *const model = g_playerRuntimePointer->model;
    return model ? model->searchRegionKey : 0;
}

Character *plymGetLimitingCollisionCharacter() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    return model ? model->limitingCollisionCharacter : 0;
}

float plymGetLimitingCollisionDistance() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    return model ? model->limitingCollisionDistance : 10.0f;
}

// @verified - d
float plymGetSpeedZ() {
    const std::byte *const model = reinterpret_cast<const std::byte *>(g_playerRuntimePointer->model);
    if (!model)
        return 0.0f;
    const std::int32_t state = *reinterpret_cast<const std::int32_t *>(model + kPlayerMotionStateOffset);
    if (!state || state == 9)
        return 0.0f;
    return *reinterpret_cast<const float *>(model + kPlayerSpeedZOffset);
}

// @verified - d
float plymGetInterpolationValue() {
    PlayerModelState *model = g_playerRuntimePointer->model;
    return model ? model->interpolationValue : 0.0f;
}

static const std::size_t kPlayerControllerStateOffset = 0x1bc;

// @verified - d
void plymGetControllerVerticalAxis(float *first, float *second) {
    *second = 0.0f;
    *first = 0.0f;
    const std::byte *const model = reinterpret_cast<const std::byte *>(g_playerRuntimePointer->model);
    if (model) {
        const std::byte *const controller = model + kPlayerControllerStateOffset;
        std::size_t offset = kPlayerControllerVerticalAxisOffset - kPlayerControllerStateOffset;
        *first = *reinterpret_cast<const float *>(controller + offset);
        offset += 4;
        *second = *reinterpret_cast<const float *>(controller + offset);
    }
}

// @verified - d
void plymGetControllerGroundBlendedXZ(float *first, float *second) {
    *second = 0.0f;
    *first = 0.0f;
    const std::byte *const model = reinterpret_cast<const std::byte *>(g_playerRuntimePointer->model);
    if (model) {
        const std::byte *const controller = model + kPlayerControllerStateOffset;
        std::size_t offset = kPlayerControllerGroundBlendOffset - kPlayerControllerStateOffset;
        *first = *reinterpret_cast<const float *>(controller + offset);
        offset += 4;
        *second = *reinterpret_cast<const float *>(controller + offset);
    }
}

// @verified - d
void plymGetControllerDeltaXZ(Vector3 *first, Vector3 *second) {
    const std::byte *model = reinterpret_cast<const std::byte *>(g_playerRuntimePointer->model);
    if (model) {
        model += kPlayerControllerStateOffset;
        std::size_t offset = kPlayerFirstControllerDeltaOffset - kPlayerControllerStateOffset;
        const Vector3 *delta = reinterpret_cast<const Vector3 *>(model + offset);
        *first = *delta;
        offset += sizeof(Vector3);
        delta = reinterpret_cast<const Vector3 *>(model + offset);
        *second = *delta;
    }
}

} // namespace s2
} // namespace shenmue

#endif
