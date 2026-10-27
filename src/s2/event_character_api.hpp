#pragma once
#include "shenmue.h"
#include "s2/event_transform_flags.hpp"

#include "common/math.hpp"
#include "s2/hlib.hpp"
#include "s2/conversation.hpp"
#include "s2/flag.hpp"
#include "common/types.hpp"

#include <cstdint>
#include <array>
#include <cstddef>
#include <span>

#include "s2/scenario.hpp"

namespace shenmue::s2 {


std::uint64_t mtwkSetMotionworkScale3(shen_name characterId, const Vector3 *scale);
std::int64_t scenMotionworkUtil(shen_name characterId, std::uint32_t yaw, float angle, std::int32_t flags, const void *motionData);
std::int64_t TurnScenarioCharacterTowardPosition(
    shen_name characterId, const Vector3 *target, const std::uint16_t *customMotions, std::uint32_t *yaw, float *angularStep, std::uint32_t flags);
enum class EventCharacterTransformMode : std::uint32_t {
    Absolute = EventTransformFlags::Local,
    Recursive = EventTransformFlags::World,
};
enum class EventCharacterPartTransformMode {
    Absolute = 0,
    Relative = 1,
    Get = 2,
    Set = 3,
};

inline constexpr std::uint32_t CHARACTER_TRANSLATION_ROOT = 0xffffffffU;
void EV_GetCharaPosition(shen_name character, std::uint32_t translationBone, Vector3 *position, std::uint32_t mode);
void EV_AT_SetPos(shen_name character, std::uint32_t flags, const Vector3 &position);
std::int64_t EV_AT_SetAngY(shen_name character, std::uint32_t flags, const Vector3i &rotation, const std::uint16_t *customMotions = nullptr);
std::int32_t EV_ActivatePartyCompanion(shen_name character);
std::int32_t EV_PTYCUtils(shen_name character, std::int32_t command, std::int32_t argument);
void GetScenarioCharacterEulerAngles(shen_name character, Vector3i *rotation, std::uint32_t flags);
void EV_scenApplyPosOrRotToPartOf(shen_name character, std::uint16_t part, Vector3 *position, Vector3i *rotation, EventCharacterPartTransformMode mode);
void ApplyScenarioTranslationAndRotation(const Vector3 *position, const Vector3i *rotation, Vector3 *point);
void TransformScenarioDirection(const Vector3 *position, const Vector3i *rotation, Vector3 *direction);
void GetCharacterHandRotation(Character *character, std::int32_t side, Vector3i *rotation);
MT7Node *ApplyCharacterHandRotation(Character *character, std::int32_t side, std::uint8_t flags, const Vector3i *rotation);
void GetCharacterHandEndpoint(Character *character, std::int32_t side, std::int32_t endpoint, Vector3 *position);
std::int16_t ConfigureCharacterHandPoseSequence(Character *character, std::int32_t side, std::int16_t pose, std::int16_t first, std::int16_t last,
    std::int16_t start, std::uint8_t flags, float step, std::int16_t ticks);
std::uint64_t IsCharacterHandPoseInactive(Character *character, std::int32_t side);
std::uint8_t SetCharacterHandPoseFlags(Character *character, std::int32_t side, std::int16_t enabled, std::uint8_t flags);
void SetCharacterHandPoseStep(Character *character, std::int32_t side, float step);
std::uint64_t GetCharacterHandPoseAnimationResult(Character *character, std::int32_t side);
std::uint64_t SetCharacterHandSecondaryPoseWork(Character *character, std::int32_t side, void *work);
std::uint64_t SetScenarioCharacterFlag40(shen_name character, std::int32_t enabled);
void SetScenarioCharacterDrawControl(shen_name character, std::int32_t mode, std::int32_t argument);
void SetScenarioCharacterType(shen_name character, std::int32_t type);
bool IsScenarioCharacterDisplayEnabled(shen_name character);
bool DoesScenarioCharacterExist(shen_name character);
void SetScenarioCharacterKillMode(shen_name character, std::uint32_t mode);
void SetScenarioCharacterTransparency(shen_name character, float alpha);
ReflectorBoardParams *SetScenarioReflectorBoardValues(shen_name character, const Vector4 *values);
ObjectXFormsParams *ResetScenarioCharacterAttachment(shen_name character);
std::int64_t SetScenarioCharacterInverseChildPlacement(shen_name character, std::uint16_t childId);
std::uint64_t AttachScenarioCharacterPreservingPlacement(
    shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation);
void AttachScenarioCharacter(shen_name target, shen_name source, std::int32_t bone, const Vector3 *position, const Vector3i *rotation);
bool TestCharaPositionWithinRange(shen_name character, const Vector3 *first, const Vector3 *second, shen_name relativeTo);
void EV_SetGlobalShadowLightingEnabled(std::int32_t enabled);
void EV_AUVW_SetLayerState(std::int32_t layer, std::int32_t state);
void EV_AUVW_Initialize(std::int32_t state);
void EV_AUVW_Update();
void EV_MAZ_fishCtrl();
Character *ctrlGetCharaByName(shen_name character);
bool CheckIsArgEquZero(std::int32_t value);
void EV_SetFish(const Vector3 *minimum, const Vector3 *maximum, std::int32_t count, std::int32_t seed, Character *character);
std::uint32_t EV_GetCurrentRefCount();
void EV_SetCurrentRefCount(std::uint32_t count);
std::int32_t EV_cuffsAttach(shen_name firstCharacter, std::int32_t firstHand, shen_name secondCharacter, std::int32_t secondHand, float maximumDistance);
void EV_cuffsSetFlags();
void EV_InitializeOpeningTemporaryCharacters(HLib::Task *startupTask);
void EV_CleanupOpeningTemporaryCharacters();
void EV_SetModelDataDefaultVertexShader();
void EV_SetEntityVertexShaderType(std::int32_t modelSlot, std::int32_t value);
std::int32_t EV_SetObjectFlagByIndex(std::int32_t index, std::int32_t value);
void EV_AssignLightParams(std::int32_t value, std::int32_t modelSlot);
void EV_lightLoad(shen_name light);
void EV_ReinitLights(std::int32_t mode);
void EV_SetLightFlagForIndex(std::int32_t index, std::int32_t value);
void EV_SetIndexForLightEntry(std::int32_t index, std::int32_t value);
void EV_EnsureCharacterDraw(shen_name character);
void EV_EnableCharacterDisplay(shen_name character);
void EV_ToggleCharacterFlag(shen_name character);
void EV_DisableCharacterDisplay(shen_name character);
void EV_SetCharacterAlphaEnabled(shen_name character, bool enabled);

struct EventAuvwRuntime {
    std::int32_t initializedState{};
    std::array<std::int32_t, 2> layerStates{};
    std::uint32_t updateCount{};
};

} // namespace shenmue::s2
