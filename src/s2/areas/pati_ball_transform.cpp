#include "s2/areas/pati_event.hpp"
#include "s2/event_runtime.hpp"
#include "s2/math.hpp"
namespace shenmue::s2 {
// @verified - p
void UpdatePatiBallPosition(PatiBallWork *ball) {
    const float sx = sin_fixed16(static_cast<std::int16_t>(ball->boardRotation.x));
    const float cx = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(ball->boardRotation.x))));
    const Vector3 &local = ball->current.position;
    const float y = local.y * cx - local.z * sx, z = local.z * cx + local.y * sx;
    const float sy = sin_fixed16(static_cast<std::int16_t>(ball->boardRotation.y));
    const float cy = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(ball->boardRotation.y))));
    PatiBallCharacter &character = ball->game->ballCharacters[ball->game->balls];
    character.position.y = y + ball->boardPosition.y;
    character.position.x = (sy * z + cy * local.x) + ball->boardPosition.x;
    character.position.z = (cy * z - sy * local.x) + ball->boardPosition.z;
    EV_AT_SetPos(character.character, EventTransformFlags::AllAxes, character.position);
}
// @verified - p
void UpdatePatiHandPosition(PatiBallWork *ball) {
    const float sx = sin_fixed16(static_cast<std::int16_t>(ball->boardRotation.x));
    const float cx = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(ball->boardRotation.x))));
    const Vector3 &local = ball->current.position;
    const float y = local.y * cx - local.z * sx, z = local.z * cx + local.y * sx;
    const float sy = sin_fixed16(static_cast<std::int16_t>(ball->boardRotation.y));
    const float cy = sin_fixed16(static_cast<std::int16_t>(fixed16_cosine_angle(static_cast<std::uint16_t>(ball->boardRotation.y))));
    Vector3 position;
    position.y = y + ball->boardPosition.y;
    position.x = (sy * z + cy * local.x) + ball->boardPosition.x;
    position.z = (cy * z - sy * local.x) + ball->boardPosition.z;
    const shen_name hand{static_cast<std::uint32_t>(patiGameplayPlacements[0].yaw)};
    EV_AT_SetPos(hand, EventTransformFlags::AllAxes, position);
    Vector3i rotation;
    const float x = local.x * 100.0f;
    if (hand.name == PATI_HAND_MODEL.name) {
        rotation.x = 61895;
        rotation.y = ball->boardRotation.y + static_cast<std::uint16_t>(static_cast<std::int32_t>(((25.0f - x) * 8192.0f) / 45.0f));
    } else {
        rotation.x = 0;
        rotation.y = ball->boardRotation.y + static_cast<std::uint16_t>(static_cast<std::int32_t>(((115.0f - x) * 8192.0f) / 45.0f));
    }
    rotation.z = 0;
    EV_AT_SetAngY(hand, EventTransformFlags::AllAxes, rotation, nullptr);
}
} // namespace shenmue::s2
