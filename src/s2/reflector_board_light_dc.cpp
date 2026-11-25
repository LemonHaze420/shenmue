#include "s2/reflector_board_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/motion_yaw_dc.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

// @verified - d
void SetReflectorBoardDirectionalLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot) {
    if ((origin.x < 0.0f ? -origin.x : origin.x) < 0.00001f)
        origin.x = 0.0f;
    if ((origin.y < 0.0f ? -origin.y : origin.y) < 0.00001f)
        origin.y = 0.0f;
    if ((origin.z < 0.0f ? -origin.z : origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction;
    direction.x = origin.x - cameraPosition.x;
    direction.y = origin.y - cameraPosition.y;
    direction.z = origin.z - cameraPosition.z;
    am2Vector3NormalizeFast(&direction);
    SetSelectedLightDirection(slot, direction);
}

// @verified - d
void shenAssignAttachedLightFor(Vector3 &origin, const Vector3 &cameraPosition, std::int32_t yaw, std::uint32_t slot) {
    Vector3 target;
    Vector3 direction;
    Vector3 projected;
    camCtrlGetTargetPosition(&target);
    const float distance = CalculatePlanarDistanceBetweenPoints(&target, &cameraPosition);
    const std::uint32_t angle = static_cast<std::uint32_t>(yaw) - static_cast<std::uint32_t>(camCtrlGetNextRotY());
    projected.x = target.x - CalculateFixedSine(angle) * distance;
    projected.z = CalculateFixedCosine(angle) * distance + target.z;
    projected.y = target.y;
    if ((origin.x < 0.0f ? -origin.x : origin.x) < 0.00001f)
        origin.x = 0.0f;
    if ((origin.y < 0.0f ? -origin.y : origin.y) < 0.00001f)
        origin.y = 0.0f;
    if ((origin.z < 0.0f ? -origin.z : origin.z) < 0.00001f)
        origin.z = 0.0f;
    direction.x = origin.x - projected.x;
    direction.y = origin.y - projected.y;
    direction.z = origin.z - projected.z;
    am2Vector3NormalizeFast(&direction);
    SetSelectedLightDirection(slot, direction);
}

// @verified - d
void SetReflectorBoardOrbitPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::int32_t yaw, std::uint32_t slot) {
    Vector3 position;
    Vector3 target;
    Vector3 direction;
    Vector3 projected;
    if ((origin.x < 0.0f ? -origin.x : origin.x) < 0.00001f)
        origin.x = 0.0f;
    if ((origin.y < 0.0f ? -origin.y : origin.y) < 0.00001f)
        origin.y = 0.0f;
    if ((origin.z < 0.0f ? -origin.z : origin.z) < 0.00001f)
        origin.z = 0.0f;
    camCtrlGetTargetPosition(&target);
    const float distance = CalculatePlanarDistanceBetweenPoints(&target, &cameraPosition);
    const std::uint32_t angle = static_cast<std::uint32_t>(yaw) - static_cast<std::uint32_t>(camCtrlGetNextRotY());
    projected.x = target.x - CalculateFixedSine(angle) * distance;
    projected.z = CalculateFixedCosine(angle) * distance + target.z;
    projected.y = target.y;
    direction.x = origin.x - projected.x;
    direction.y = origin.y - projected.y;
    direction.z = origin.z - projected.z;
    am2Vector3NormalizeFast(&direction);
    position.x = origin.x - direction.x * 0.4f;
    position.y = origin.y - direction.y * 0.4f;
    position.z = origin.z - direction.z * 0.4f;
    SetSelectedLightPosition(slot, position);
    SetSelectedLightFalloff(slot, 0.25f);
    SetSelectedLightRange(slot, 1.5f);
    SetLightKind(slot, 3);
}

// @verified - d
void SetReflectorBoardPointLight(Vector3 &origin, const Vector3 &cameraPosition, std::uint32_t slot) {
    Vector3 position;
    if ((origin.x < 0.0f ? -origin.x : origin.x) < 0.00001f)
        origin.x = 0.0f;
    if ((origin.y < 0.0f ? -origin.y : origin.y) < 0.00001f)
        origin.y = 0.0f;
    if ((origin.z < 0.0f ? -origin.z : origin.z) < 0.00001f)
        origin.z = 0.0f;
    Vector3 direction;
    direction.x = origin.x - cameraPosition.x;
    direction.y = origin.y - cameraPosition.y;
    direction.z = origin.z - cameraPosition.z;
    am2Vector3NormalizeFast(&direction);
    position.x = origin.x - direction.x * 0.4f;
    position.y = origin.y - direction.y * 0.4f;
    position.z = origin.z - direction.z * 0.4f;
    SetSelectedLightPosition(slot, position);
    SetSelectedLightFalloff(slot, 0.25f);
    SetSelectedLightRange(slot, 1.5f);
    SetLightKind(slot, 3);
}

// @verified - d
std::uint32_t ResolveReflectorBoardCharacterAngle(Character *character) {
    const std::uint32_t cameraYaw = 0U - static_cast<std::uint32_t>(camCtrlGetNextRotY());
    Vector3 cameraDirection;
    cameraDirection.x = -am2NegativeSineFixed(cameraYaw);
    cameraDirection.z = am2NegativeCosineFixed(cameraYaw);
    cameraDirection.y = 0.0f;
    am2Vector3NormalizeFast(&cameraDirection);
    const std::uint32_t boneYaw = mtwkGetBoneYaw(charFindMotionwork(character), 7);
    Vector3 characterDirection;
    characterDirection.x = am2NegativeSineFixed(boneYaw);
    characterDirection.z = am2NegativeCosineFixed(boneYaw);
    characterDirection.y = 0.0f;
    am2Vector3NormalizeFast(&characterDirection);
    float angle = -((am2Vector3Dot(&cameraDirection, &characterDirection) * 180.0f) - 180.0f);
    angle *= 0.5f;
    if (static_cast<std::int16_t>(boneYaw + cameraYaw) < 0)
        angle = -angle;
    if (angle > 90.0f)
        angle = ((angle) * (0.5f) + (-90.0f));
    else if (angle < -90.0f)
        angle = ((angle) * (0.5f) + (90.0f));
    else
        angle = -angle * 0.5f;
    return static_cast<std::int32_t>((8192.0f * angle) / 45.0f) & 0xffffU;
}

} // namespace s2
} // namespace shenmue

#endif
