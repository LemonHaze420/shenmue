#pragma once

#include "s2/scenario_dc.hpp"
#include "s2/event_transform_flags.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct ObjectXFormsParams;
typedef std::int32_t EventCharacterPartTransformMode;
enum {
    EventCharacterPartTransformMode_Absolute,
    EventCharacterPartTransformMode_Relative,
    EventCharacterPartTransformMode_Get,
    EventCharacterPartTransformMode_Set
};
static const std::uint16_t scenarioTurnAnimations[] = {0xf021, 0xf022, 0xf023, 0xf024, 0xf028, 0xf027, 0xf026, 0xf003};
static const std::int32_t scenarioTurnYawOffsets[] = {0x2000, 0x4000, 0x6000, 0x8000, 0xa000, 0xc000, 0xe000};
static const shen_name SCENARIO_CHARACTER_NO_PARENT(0xffffffffU);

void EV_AT_SetPos(shen_name name, std::uint32_t flags, const Vector3 &position);
bool SetScenarioCharacterMotionTarget(shen_name name, std::uint32_t flags, std::int32_t selector, const Vector3 *position);
void EV_GetCharaPosition(shen_name name, std::uint32_t bone, Vector3 *position, std::uint32_t flags);
ObjectXFormsParams *AttachScenarioCharacterPreservingPlacement(
    shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation);
void AttachScenarioCharacter(shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation);
ObjectXFormsParams *ResetScenarioCharacterAttachment(shen_name name);
void SetScenarioCharacterInversePlacement(shen_name name, const Vector3 *position, const Vector3i *rotation);
std::int32_t SetScenarioCharacterInverseChildPlacement(shen_name name, std::int32_t childId);
std::int32_t RotateCharacter(Character *character, std::uint32_t flags, const Vector3i *rotation, const std::uint16_t *customMotions);
float AssignCharacterMotionYaw(Character *character, std::int32_t yaw, const std::uint16_t *customMotions, std::uint32_t flags);
std::int32_t EV_AT_SetAngY(shen_name name, std::uint32_t flags, const Vector3i &rotation, const std::uint16_t *customMotions);
void GetScenarioCharacterEulerAngles(shen_name name, Vector3i *rotation, std::uint32_t flags);
void AT_SetLocalBaseChara(shen_name name, shen_name base);
shen_name EV_SCEN_FindChar_Wrapper(shen_name name);
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void EV_scenApplyPosOrRotToPartOf(shen_name name, std::uint32_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode);
#else
void EV_scenApplyPosOrRotToPartOf(shen_name name, std::uint16_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode);
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t ExecuteMotionworkUtility(Character *character, std::uint32_t yaw, float angle, std::uint32_t flags, const void *motionData);
std::int32_t scenMotionworkUtil(shen_name name, std::uint32_t yaw, float angle, std::uint32_t flags, const void *motionData);
std::int32_t TurnScenarioCharacterTowardPosition(
    shen_name name, const Vector3 *target, const std::uint16_t *customMotions, std::uint32_t *yaw, float *angularStep, std::uint32_t flags);
void ApplyTranslationRotationScaleXYZToCharaByID(shen_name name, std::int32_t ordinal, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
void DrawScenarioModelChild(shen_name name, std::int32_t ordinal, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
void DrawPIN(shen_name name, std::int32_t enabled, const Vector3 *position, const Vector3i *rotation, const Vector3 *scale);
#endif

} // namespace s2
} // namespace shenmue

#endif
