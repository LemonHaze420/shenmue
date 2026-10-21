#include "s1/task/srch.hpp"
#include "s1/collision.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/matrix_stack.hpp"

#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/player_control.hpp"
#include "s1/subtitles.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/motion.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/came.hpp"
#include "s1/task/conv.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/scen.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <cstdint>

namespace shenmue::s1 {

std::uint32_t g_searchLockOnFlags = 0;
float g_searchTargetDistance = 0.0f;
float g_searchAccumulatedOffset = 0.0f;
SearchEntry *g_searchEntries = nullptr;
std::int32_t g_searchEntrySelectorFilter = -1;
std::int32_t g_searchPendingSelectedIdentifier = -1;
std::int32_t g_searchCandidateYawConeWidth = 0;
float g_maxCharacterSearchExtent = 0.0f;
float g_fallbackSearchExtent = 0.0f;
std::int32_t g_searchAnyInputPressedLatched = 0;
std::int32_t g_searchAPressedLatched = 0;
std::int32_t g_currentSearchState = 0;
std::int32_t g_searchCurrentIdentifier = -1;
std::int32_t g_searchCandidateId = -1;
std::int32_t g_searchPendingExplicitMapInfoId = 0;
std::int32_t g_searchCameraTransitionActive = 0;
std::uint32_t g_searchTemporaryFocusedCharacter = 0;
const SearchFocusMapping *g_searchFocusedItem = nullptr;
std::int32_t g_searchFocusedItemState = 0;
std::int32_t g_searchUpdatesThisFrame = 0;
std::int32_t g_searchTickCount = 0;
std::int32_t g_searchCachedResult = -1;
std::int32_t g_searchCachedFocused = -1;
std::int32_t g_searchCachedUpdateResult = -1;
std::int16_t g_searchModeEnterCount = 0;
std::int16_t g_searchSelectCount = 0;

std::array<std::int16_t, 5> g_searchPromptIconSlots{-1, -1, -1, -1, -1};
std::array<std::int16_t, 5> g_searchRequestedPromptIconSlots{-1, -1, -1, -1, -1};

std::uint8_t g_searchPreviousHelpToken = 0;
std::uint8_t g_searchPreviousHelpButton = 0;
HLib::Task *g_searchTask = nullptr;

namespace {

constexpr std::int32_t kMaximumSearchEntries = 10000;
constexpr std::int32_t kAuthorScripted = 9;
std::int16_t g_searchSelectedEntryType = 0;
std::int16_t g_searchPendingId = 0;
std::int32_t g_searchLockCameraPhase = 0;
std::int16_t g_searchRetargetDelay = 0;
std::int16_t g_searchStateFlag = 0;
float g_searchDistanceParameter = 0.0f;
float g_searchSmoothingFactor = 0.0f;
std::uint16_t g_searchDirectionInputLatched = 0;
std::uint16_t g_searchLastProcessedDirectionInput = 0;
std::int32_t g_searchPreviousGameState = 0;

std::int32_t g_searchSavedAuthorState = 0;
Vector3 g_searchScriptTarget{};
std::int32_t g_searchScriptTargetYaw = 0;
Vector3i g_searchCameraOffsets{};
Vector3 g_searchCharacterCameraCurrent{};
std::array<Vector3, 5> g_searchCharacterCameraSamples{};

Character *g_searchLockTargetCharacter = nullptr;
std::int32_t g_searchBlockedByCycleObject = 0;
std::int16_t g_searchStickXDelta = 0;
std::int16_t g_searchStickYDelta = 0;
std::uint16_t g_searchTriggerPressure = 0;
Vector3 g_searchEyeWorldPosition{};
Vector3 g_searchCurrentCameraEye{};
Vector3 g_searchCurrentCameraTarget{};
Vector3 g_searchTargetObjectPosition{};
std::int32_t g_searchLockOnYaw = 0;
std::int32_t g_searchLockOnPitch = 0;
std::int16_t g_searchOrbitYaw = 0;
std::int32_t g_searchOrbitPitch = 0;
Vector3 g_searchOrbitCenter{};
float g_searchEyeToTargetDistance = 0.0f;
float g_searchTargetDistanceFromRadius = 0.0f;
float g_searchEyeDistanceStep = 0.0f;
float g_searchTargetDistanceStep = 0.0f;
std::int32_t g_searchEyeYawDelta = 0;
std::int32_t g_searchEyePitchDelta = 0;
std::int32_t g_searchTargetYawDelta = 0;
std::int32_t g_searchTargetPitchDelta = 0;
std::int16_t g_searchFocusedItemTimeoutFrames = 0;


void ResetPromptSlots() {
    g_searchPromptIconSlots.fill(-1);
    g_searchRequestedPromptIconSlots.fill(-1);
    SRCH_ApplySearchPromptSlots();
}

MapControlData *SearchControl() {
    return g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

Character *SearchRyo() {
    MapControlData *const control = SearchControl();
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

PadCaptureEntry *SearchPad() {
    MapControlData *const control = SearchControl();
    return control ? static_cast<PadCaptureEntry *>(control->padCaptureEntry) : nullptr;
}


Character *LookupCharacter(std::uint32_t actorId) {
    MapControlData *const control = SearchControl();
    return control && control->scenarioTask ? CHID_Find(control->scenarioTask, actorId) : nullptr;
}

const SearchFocusMapping *GetSearchMappingForSearchItem(std::int32_t identifier) {
    const SearchFocusMapping *mapping = nullptr;
    switch (g_areaId) {
    case SHEN_NAME(JOMO):
        mapping = kSearchMappingJOMO;
        break;
    case SHEN_NAME(MKYU):
        mapping = kSearchMappingMKYU;
        break;
    case SHEN_NAME(MFSY):
        mapping = kSearchMappingMFSY;
        break;
    case SHEN_NAME(DKTY):
        mapping = kSearchMappingDKTY;
        break;
    case SHEN_NAME(DCBN):
        mapping = kSearchMappingDCBN;
        break;
    case SHEN_NAME(D000):
        mapping = kSearchMappingD000;
        break;
    case SHEN_NAME(JD00):
        mapping = kSearchMappingJD00;
        break;
    case SHEN_NAME(JU00):
        mapping = kSearchMappingJU00;
        break;
    case SHEN_NAME(JHD0):
        mapping = kSearchMappingJHD0;
        break;
    case SHEN_NAME(JABE):
        mapping = kSearchMappingJABE;
        break;
    default:
        return nullptr;
    }
    for (; (mapping->identifier & 0x80000000U) == 0; ++mapping) {
        if (identifier == static_cast<std::int32_t>(mapping->identifier))
            return mapping;
    }
    return nullptr;
}

void PresentSearchFocusedItem(bool visible) {
    MapControlData *const control = SearchControl();
    UI_PresentSearchFocusedItem(control ? control->scenarioTask : nullptr, g_searchFocusedItem->uiToken, visible);
}

bool IsActiveCewp(const Character *character) {
    return character->entity.name == SHEN_NAME(CEWP) &&
        ((character->flags.value & 1U) != 0 || ((character->flags.value & 2U) != 0 && (character->motionFlags & 1U) != 0));
}

std::int16_t AbsAngle16(std::int32_t angle) {
    const auto value = static_cast<std::uint16_t>(angle);
    return static_cast<std::int16_t>(value <= 0x8000U ? value : -static_cast<std::int16_t>(value));
}

void SetRyoYawAgainstParents(Character *ryo, std::int32_t yaw) {
    for (const CharacterNode *node = ryo->entity.node.child; node; node = node->child)
        yaw -= node->rotation.y;
    ryo->entity.node.rotation.y = static_cast<std::int16_t>(yaw);
}

std::int32_t SearchListCount() {
    std::int32_t count = 0;
    for (const std::uint32_t name : SCEN_GetSearchCharacterList()) {
        if (!name)
            break;
        ++count;
    }
    return count;
}

} // namespace



bool IsOutsideViewCone(const Vector3 &destination, const Vector3 &source) {
    const float xDifference = destination.x - source.x;
    const float zDifference = destination.z - source.z;
    const float scalar = static_cast<float>(0.15 / static_cast<double>(std::sqrt(zDifference * zDifference + xDifference * xDifference)));
    const float zOffset = zDifference * scalar;
    const float xOffset = xDifference * scalar;
    if (coneCheck(destination, {source.x - zOffset, source.y, source.z + xOffset}))
        return true;
    return coneCheck(destination, {zOffset + source.x, source.y, source.z - xOffset}) != 0;
}



std::uint32_t srchFindClosestCharacterToScreenCenter(Vector3 &worldStart, const Vector3 &destination) {
    std::int32_t maxDistance = 99999999;
    const std::uint32_t screenWidth = g_originalScreenWidth;
    const std::uint32_t screenHeight = g_originalScreenHeight;
    g_searchTemporaryFocusedCharacter = 0;
    Matrix4x4_Push();
    Matrix4x4_RotateAndPush();
    for (const std::uint32_t name : SCEN_GetSearchCharacterList()) {
        if (!name)
            break;
        Character *const character = LookupCharacter(name);
        if (!character)
            continue;
        mtwkGetCharaBonePosition(character, 0x2f, &worldStart);
        worldStart.y = static_cast<float>(static_cast<double>(worldStart.y) + 0.05);
        if (IsOutsideViewCone(destination, worldStart))
            continue;
        Vector2 screen{};
        WorldToScreen(worldStart, screen);
        const float x = screen.x - static_cast<float>(screenWidth) * 0.5f;
        const float y = screen.y - static_cast<float>(screenHeight) * 0.5f;
        if (x > -180.0f && x < 180.0f && y > -140.0f && y < 140.0f && y * y + x * x < static_cast<float>(maxDistance)) {
            maxDistance = static_cast<std::int32_t>(y * y + x * x);
            g_searchTemporaryFocusedCharacter = name;
        }
    }
    Matrix4x4::Pop(1);
    return g_searchTemporaryFocusedCharacter;
}

namespace {



std::int32_t srchGetTransformedEntityPosition(Vector3 &position, const Vector3 &origin) {
    std::int32_t result = -1;
    SearchEntry *entry = g_searchEntries;
    if (!entry || !g_searchPendingExplicitMapInfoId)
        return result;
    const auto explicitName = static_cast<std::uint32_t>(g_searchPendingExplicitMapInfoId);
    Character *character = LookupCharacter(explicitName);
    if (character && !IsActiveCewp(character))
        character = nullptr;
    for (std::int32_t index = 0; entry->id >= 0; ++entry, ++index) {
        if ((g_searchEntrySelectorFilter >= 0 && g_searchEntrySelectorFilter != static_cast<std::uint8_t>(entry->id)) || entry->name != explicitName)
            continue;
        if (entry->entryType < 0) {
            position = entry->position;
            result = index;
            break;
        }
        if (!character)
            continue;
        position = GetPositionForEntity(character->entity.node);
        Matrix4x4_Push();
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(position.x, position.y, position.z);
        if (entry->useRotationY) {
            const Vector3i euler = GetEulerAnglesForEntity(character->entity.node);
            Matrix4x4::SetViewMatrixFromEuler(euler.y, euler.x, euler.z);
        } else {
            position.x -= origin.x;
            position.z -= origin.z;
            Matrix4x4::RotateAroundY_Wrapper(atan2_fixed16(position.x, position.z));
        }
        Matrix4x4::preTranslate(entry->position.x, entry->position.y, entry->position.z);
        position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::Pop(1);
        result = index;
        break;
    }
    g_searchPendingExplicitMapInfoId = 0;
    return result;
}





std::int32_t FindCandidateInWorldOrScreen(const Vector3 &cameraPoint, Vector3 &position, const Vector3 &yawReference, const Vector3 &origin,
    std::int16_t searchCenterYaw, std::int16_t cameraYaw, bool screenSpace) {
    std::int32_t result = -1;
    SearchEntry *entry = g_searchEntries;
    if (!entry)
        return result;
    Matrix4x4_Push();
    const float coneDot = cos_fixed16(static_cast<std::uint16_t>(g_searchCandidateYawConeWidth / 2));
    float outerDot = sin_fixed16(5462);
    const float innerDot = sin_fixed16(14200);
    float maxDistance = 0.0f;
    float secondaryDistance = 0.0f;
    Vector3 viewPosition{};
    if (screenSpace) {
        maxDistance = 10000000.0f;
        Matrix4x4_RotateAndPush();
        secondaryDistance = coneDot;
    } else {
        maxDistance = 1.0f;
        workTransformMatrix = g_tmp_cam_xform;
        secondaryDistance = 1.0f;
        Matrix4x4::TransformPoint(position, viewPosition);
    }
    for (std::int32_t index = 0; entry->id >= 0; ++entry, ++index) {
        if (g_searchEntrySelectorFilter >= 0 && g_searchEntrySelectorFilter != static_cast<std::uint8_t>(entry->id))
            continue;
        Vector3 candidate{};
        float extent = 0.0f;
        if (entry->entryType < 0) {
            candidate = entry->position;
            extent = g_fallbackSearchExtent;
        } else {
            Character *const character = LookupCharacter(entry->name);
            if (!character || !IsActiveCewp(character) || (character->motionFlags & 0x3fU) != 0x3fU || static_cast<double>(character->renderOpacity) < 0.01)
                continue;
            candidate = GetPositionForEntity(character->entity.node);
            extent = g_maxCharacterSearchExtent;
            Matrix4x4_Push();
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(candidate.x, candidate.y, candidate.z);
            if (entry->useRotationY) {
                const Vector3i euler = GetEulerAnglesForEntity(character->entity.node);
                Matrix4x4::SetViewMatrixFromEuler(euler.y, euler.x, euler.z);
            } else {
                Matrix4x4::RotateAroundY_Wrapper(atan2_fixed16(candidate.x - origin.x, candidate.z - origin.z));
            }
            Matrix4x4::preTranslate(entry->position.x, entry->position.y, entry->position.z);
            candidate = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop(1);
        }
        if (origin.y > candidate.y || static_cast<double>(candidate.y) > static_cast<double>(origin.y) + 2.5)
            continue;
        const auto candidateYaw = static_cast<std::int16_t>(atan2_fixed16(yawReference.x - candidate.x, yawReference.z - candidate.z));
        if (coneDot > cos_fixed16(static_cast<std::uint16_t>(candidateYaw - cameraYaw)))
            continue;
        if (entry->useRotationY) {
            const float facingDot = cos_fixed16(static_cast<std::uint16_t>(candidateYaw - entry->rotationY));
            if (cos_fixed16(static_cast<std::uint16_t>(entry->useRotationY / 2)) > facingDot)
                continue;
        }
        const float x = candidate.x - cameraPoint.x;
        const float y = candidate.y - cameraPoint.y;
        const float z = candidate.z - cameraPoint.z;
        if (screenSpace) {
            if (y * y + x * x + z * z > extent * extent)
                continue;
            Vector2 screen{};
            WorldToScreen(candidate, screen);
            const float screenY = screen.y - static_cast<float>(g_originalScreenHeight) * 0.5f;
            const float screenX = screen.x - static_cast<float>(g_originalScreenWidth) * 0.5f;
            if (screenX > -180.0f && screenX < 180.0f && screenY > -140.0f && screenY < 140.0f) {
                const float distance = screenY * screenY + screenX * screenX;
                if (maxDistance > distance) {
                    maxDistance = distance;
                    position = candidate;
                    result = index;
                }
            }
            continue;
        }
        Vector3 viewCandidate{};
        Matrix4x4::TransformPoint(candidate, viewCandidate);
        const float viewX = viewPosition.x - viewCandidate.x;
        const float viewY = viewPosition.y - viewCandidate.y;
        const float distance = viewY * viewY + viewX * viewX + (viewPosition.z - viewCandidate.z) * (viewPosition.z - viewCandidate.z);
        if (static_cast<double>(distance) <= 0.001 || maxDistance <= distance)
            continue;
        const auto direction = static_cast<std::int16_t>(atan2_fixed16(viewX, viewY));
        const float directionDot = cos_fixed16(static_cast<std::uint16_t>(direction - searchCenterYaw));
        if (directionDot <= outerDot)
            continue;
        if (directionDot <= innerDot) {
            if (secondaryDistance <= distance)
                continue;
            secondaryDistance = distance;
        } else {
            outerDot = innerDot;
            maxDistance = distance;
        }
        if (distance > 0.0f) {
            position = candidate;
            result = index;
        }
    }
    Matrix4x4::Pop(1);
    return result;
}




void srchFindPosRotForId(Vector3 &eye, Vector3 &cameraPosition, const Vector3 &base, std::int32_t angleY, std::int32_t id) {
    std::int32_t index = id;
    float distance = 2.2f;
    float blend = 0.25f;
    if (index < 0) {
        const auto state = static_cast<std::uint32_t>(g_currentSearchState);
        if (((state - 2U) & ~3U) == 0 && state != 4U)
            index = ~id;
    }
    if (index >= 0 && g_searchEntries && (static_cast<std::uint16_t>(g_searchEntries[index].id) & 0x100U) != 0) {
        distance = 1.1f;
        blend = 0.34999999f;
    }
    if (g_searchCameraTransitionActive)
        blend = static_cast<float>(g_searchAccumulatedOffset * 0.003125 + blend);
    if (Character *const ryo = SearchRyo())
        mtwkGetCharaBonePosition(ryo, 0x2f, &cameraPosition);
    else
        cameraPosition = base;
    const std::int32_t inverseYaw = -angleY;
    const float x = base.x - sin_fixed16(static_cast<std::int16_t>(inverseYaw)) * distance;
    cameraPosition.x = x;
    cameraPosition.y = static_cast<float>(cameraPosition.y + 0.15);
    cameraPosition.z = cos_fixed16(static_cast<std::uint16_t>(inverseYaw)) * distance + base.z;
    eye.x = (x - base.x) * blend + base.x;
    eye.y = (cameraPosition.y - base.y) * blend + base.y;
    eye.z = (cameraPosition.z - base.z) * blend + base.z;
}



bool GetDistanceBetweenEntities(Character *character, const Vector3 &start, float deltaX, float deltaZ, float lengthSquared) {
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const float offsetX = position.x - start.x;
    const float offsetZ = position.z - start.z;
    const float along = (deltaX * offsetX + deltaZ * offsetZ) / lengthSquared;
    const float distanceSquared = offsetZ * offsetZ + offsetX * offsetX;
    const float perpendicularX = offsetX - deltaX * along;
    const float perpendicularZ = offsetZ - deltaZ * along;
    return static_cast<double>(along) > 0.1 && lengthSquared > distanceSquared && perpendicularZ * perpendicularZ + perpendicularX * perpendicularX < 0.6f &&
        character != g_searchLockTargetCharacter;
}



bool srchCheckCanLockon(std::int32_t id, const Vector3 &cameraPosition, const Vector3 &target) {
    g_searchBlockedByCycleObject = 0;
    if (g_areaId == SHEN_NAME(YDB1) || g_areaId == SHEN_NAME(DCBN))
        return false;
    if (id < 0) {
        const auto state = static_cast<std::uint32_t>(g_currentSearchState);
        if (((state - 2U) & ~3U) == 0 && state != 4U)
            id = ~id;
    }
    g_searchLockTargetCharacter = id >= 0 && g_searchEntries && g_searchEntries[id].entryType >= 0 ? LookupCharacter(g_searchEntries[id].name) : nullptr;
    const float deltaX = target.x - cameraPosition.x;
    const float deltaZ = target.z - cameraPosition.z;
    const float lengthSquared = deltaZ * deltaZ + deltaX * deltaX;
    MapControlData *const control = SearchControl();
    if (!control)
        return false;
    constexpr std::uint32_t kMotionManager = SHEN_NAME(MOTM);
    for (void *const bucketHead : control->characterBuckets) {
        for (auto *bucket = static_cast<CharacterBucketNode *>(bucketHead); bucket; bucket = bucket->next) {
            Character *const character = bucket->character;
            if (!character || !IsActiveCewp(character) || character == control->ryo)
                continue;
            const CharacterComponentNode *node = character->componentTree;
            while (node && node->componentId != kMotionManager)
                node = node->componentId < kMotionManager ? node->greaterChild : node->lessChild;
            if (node) {
                if (GetDistanceBetweenEntities(character, cameraPosition, deltaX, deltaZ, lengthSquared))
                    return true;
            } else if (COLS_SearchSegmentHitsCycleCharacter(*character, cameraPosition, target)) {
                g_searchBlockedByCycleObject = 1;
                return true;
            }
        }
    }
    return false;
}



void srchFindClosestPointOnMapWithinAngle(std::int32_t center) {
    float coneMinDot = sin_fixed16(-1820);
    const float coneMaxDot = sin_fixed16(12744);
    float overallDistance = 100000000.0f;
    float coneDistance = 100000000.0f;
    std::uint32_t closest = 0;
    const std::uint32_t focused = g_searchTemporaryFocusedCharacter;
    Matrix4x4_Push();
    workTransformMatrix = g_tmp_cam_xform;
    const Matrix4x4 &m = workTransformMatrix;
    const Vector3 &current = g_searchCharacterCameraCurrent;
    const float currentX = m.rows[1].x * current.y + m.rows[0].x * current.x + m.rows[2].x * current.z + m.rows[3].x;
    const float currentZ = m.rows[1].z * current.y + m.rows[0].z * current.x + m.rows[2].z * current.z + m.rows[3].z;
    for (const std::uint32_t name : SCEN_GetSearchCharacterList()) {
        if (!name)
            break;
        if (name == focused)
            continue;
        Character *const character = LookupCharacter(name);
        if (!character)
            continue;
        Vector3 head{};
        mtwkGetCharaBonePosition(character, 0x2f, &head);
        head.y = static_cast<float>(head.y + 0.05);
        const float deltaX = currentX - (head.x * m.rows[0].x + m.rows[1].x * head.y + head.z * m.rows[2].x + m.rows[3].x);
        const float deltaZ = currentZ - (m.rows[0].z * head.x + m.rows[1].z * head.y + m.rows[2].z * head.z + m.rows[3].z);
        const float distance = deltaZ * deltaZ + deltaX * deltaX;
        if (overallDistance <= distance)
            continue;
        if ((center & 0xffff0000) != 0) {
            overallDistance = distance;
            closest = name;
            continue;
        }
        const auto direction = static_cast<std::int16_t>(atan2_fixed16(deltaX, -deltaZ));
        const float dot = cos_fixed16(static_cast<std::uint16_t>(direction - center));
        if (dot <= coneMinDot)
            continue;
        if (dot <= coneMaxDot) {
            if (coneDistance <= distance)
                continue;
            coneDistance = distance;
        } else {
            overallDistance = distance;
            coneMinDot = coneMaxDot;
        }
        if (distance > 0.0f)
            closest = name;
    }
    Matrix4x4::Pop(1);
    g_searchTemporaryFocusedCharacter = closest ? closest : focused;
}


void srchResolveFocusedTarget() {
    for (const std::uint32_t name : SCEN_GetSearchCharacterList()) {
        if (!name)
            break;
        if (name == g_searchTemporaryFocusedCharacter) {
            if (LookupCharacter(name))
                return;
            break;
        }
    }
    g_searchTemporaryFocusedCharacter = 0;
    srchFindClosestPointOnMapWithinAngle(-1);
}


bool srchUpdateLockOnCamTargetPosRot() {
    g_searchPendingSelectedIdentifier = -1;
    if (g_searchCandidateId < 0)
        return false;
    const SearchEntry &entry = g_searchEntries[g_searchCandidateId];
    g_searchPendingSelectedIdentifier = static_cast<std::int32_t>(entry.name);
    g_searchPendingId = static_cast<std::uint8_t>(entry.id);
    g_searchSelectedEntryType = entry.entryType;
    if (entry.entryType < 0) {
        g_searchTargetObjectPosition = entry.position;
    } else {
        Character *const character = LookupCharacter(entry.name);
        if (!character)
            return false;
        g_searchTargetObjectPosition = GetPositionForEntity(character->entity.node);
        g_searchTargetObjectPosition.x += entry.position.x;
        g_searchTargetObjectPosition.y += entry.position.y;
        g_searchTargetObjectPosition.z += entry.position.z;
    }
    const float x = g_searchTargetObjectPosition.x - g_CameraEyePos.x;
    const float y = g_searchTargetObjectPosition.y - g_CameraEyePos.y;
    const float z = g_searchTargetObjectPosition.z - g_CameraEyePos.z;
    const float length = std::sqrt(y * y + x * x + z * z);
    if (static_cast<double>(length) <= 0.001) {
        g_searchLockOnYaw = 0;
        g_searchLockOnPitch = 0;
    } else {
        g_searchLockOnYaw = -static_cast<std::int16_t>(atan2_fixed16(x, -z));
        g_searchLockOnPitch = asin_fixed16(y / length);
    }
    return true;
}


void srchComputeCameraForLockOnObject() {
    const auto yaw0 = static_cast<std::int16_t>(g_searchLockOnYaw);
    const auto pitch0 = static_cast<std::int16_t>(g_searchLockOnPitch);
    const Vector3 object = g_searchTargetObjectPosition;
    const float targetY = object.y - g_CameraTargetPos.y;
    const float targetZ = object.z - g_CameraTargetPos.z;
    const float targetX = object.x - g_CameraTargetPos.x;
    g_searchOrbitYaw = yaw0;
    g_searchOrbitCenter = object;
    g_searchOrbitPitch = g_searchLockOnPitch;
    const float targetLength = std::sqrt(targetY * targetY + targetX * targetX + targetZ * targetZ);
    std::int32_t targetYaw = 0;
    std::int16_t targetPitch = 0;
    if (static_cast<double>(targetLength) > 0.001) {
        targetYaw = -static_cast<std::int16_t>(atan2_fixed16(targetX, -targetZ));
        targetPitch = asin_fixed16(targetY / targetLength);
    }
    const float eyeX = object.x - g_CameraEyePos.x;
    const float eyeY = object.y - g_CameraEyePos.y;
    const float eyeZ = object.z - g_CameraEyePos.z;
    g_searchEyeToTargetDistance = targetLength;
    g_searchEyeYawDelta = static_cast<std::int16_t>(targetYaw - yaw0);
    g_searchEyePitchDelta = static_cast<std::int16_t>(targetPitch - pitch0);
    const float eyeLength = std::sqrt(eyeY * eyeY + eyeX * eyeX + eyeZ * eyeZ);
    std::int32_t eyeYaw = 0;
    std::int16_t eyePitch = 0;
    if (static_cast<double>(eyeLength) > 0.001) {
        eyeYaw = -static_cast<std::int16_t>(atan2_fixed16(eyeX, -eyeZ));
        eyePitch = asin_fixed16(eyeY / eyeLength);
    }
    g_searchLockOnFlags |= 1U;
    g_searchTargetYawDelta = static_cast<std::int16_t>(eyeYaw - yaw0);
    g_searchTargetDistanceFromRadius = std::fmax(eyeLength - g_searchTargetDistance, 0.0f);
    g_searchTargetPitchDelta = static_cast<std::int16_t>(eyePitch - pitch0);
    g_searchEyeDistanceStep = 0.0f;
    g_searchTargetDistanceStep = 0.0f;
    g_searchLockCameraPhase = 0;
}



void srchComputeOrbitPosition(std::int16_t yawDelta, std::int16_t pitchDelta, float distance, Vector3 &output) {
    output = {0.0f, 0.0f, distance};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(g_searchOrbitCenter.x, g_searchOrbitCenter.y, g_searchOrbitCenter.z);
    const auto yaw = static_cast<std::int16_t>(yawDelta + g_searchOrbitYaw);
    Matrix4x4::RotateAroundY(sin_fixed16(yaw), cos_fixed16(static_cast<std::uint16_t>(yaw)));
    const auto pitch = static_cast<std::int16_t>(pitchDelta + g_searchOrbitPitch);
    Matrix4x4::RotateAroundX(sin_fixed16(pitch), cos_fixed16(static_cast<std::uint16_t>(pitch)));
    Matrix4x4::TransformPoint(output, output);
    Matrix4x4::Pop(1);
}

float AdvanceLockOnDistance(float &remaining, float &step) {
    if (static_cast<double>(remaining) < 0.01)
        return remaining;
    float next = remaining * 0.125f;
    if (next - step > 0.0f)
        next = static_cast<float>(step * 0.8999999985098839 + next * 0.1f);
    if (next < 0.01f)
        next = 0.0099999998f;
    float advanced = std::fmin(remaining, next);
    if (advanced > 0.3f)
        advanced = 0.30000001f;
    step = advanced;
    remaining -= advanced;
    return remaining;
}


bool srchUpdateLockOnCam() {
    const bool reached = static_cast<double>(g_searchTargetDistanceFromRadius) < 0.02;
    const float radius = AdvanceLockOnDistance(g_searchTargetDistanceFromRadius, g_searchTargetDistanceStep);
    AdvanceLockOnDistance(g_searchEyeToTargetDistance, g_searchEyeDistanceStep);
    const auto damp = [](std::int32_t &delta) {
        const auto value = static_cast<std::int16_t>(delta);
        delta = static_cast<std::int16_t>(value - value / 20);
    };
    damp(g_searchTargetYawDelta);
    damp(g_searchTargetPitchDelta);
    damp(g_searchEyeYawDelta);
    damp(g_searchEyePitchDelta);
    Vector3 eye{};
    Vector3 target{};
    srchComputeOrbitPosition(
        static_cast<std::int16_t>(g_searchTargetYawDelta), static_cast<std::int16_t>(g_searchTargetPitchDelta), g_searchTargetDistance + radius, eye);
    srchComputeOrbitPosition(
        static_cast<std::int16_t>(g_searchEyeYawDelta), static_cast<std::int16_t>(g_searchEyePitchDelta), g_searchEyeToTargetDistance, target);
    TranslateCameraRelative(eye);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    return reached;
}


void srchUpdateForCurrentDetected() {
    PadCaptureEntry *const pad = SearchPad();
    const std::uint16_t triggers = pad ? pad->triggers : 0;
    SRCH_UpdatePromptState(0, triggers, ResolveCurrentInputForGame(-1), g_applicationSettings.savedInput.swapTriggers != 0);
    if (pad) {
        if ((pad->pressedPriority & 0x400U) != 0)
            g_searchAPressedLatched = 1;
        if ((pad->pressedPriority & 0xf6c6U) != 0)
            g_searchAnyInputPressedLatched = 1;
    }
}


void srchUpdate() {
    std::int16_t retargetDelay = 30;
    std::int32_t candidate = -1;
    float accumulatedOffset = 0.0f;
    std::uint32_t previousFocused = 0;
    if (g_currentSearchState) {
        retargetDelay = g_searchRetargetDelay;
        candidate = g_searchCandidateId;
        accumulatedOffset = g_searchAccumulatedOffset;
        previousFocused = g_searchTemporaryFocusedCharacter;
    } else {
        g_searchCandidateId = -1;
        g_searchRetargetDelay = 30;
        g_searchAccumulatedOffset = 0.0f;
        g_currentSearchState = static_cast<std::int32_t>(SearchState::IdleAwaitingTarget);
        g_searchCameraTransitionActive = 0;
        g_searchTemporaryFocusedCharacter = 0;
        g_searchStateFlag = 0;
    }

    Character *const ryo = SearchRyo();
    if (!ryo)
        return;
    Vector3 ryoHead{};
    mtwkGetCharaBonePosition(ryo, 0x2f, &ryoHead);
    ryoHead.y = static_cast<float>(ryoHead.y + 0.15);
    const Vector3 ryoPosition = GetPositionForEntity(ryo->entity.node);
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive)
        return;

    PadCaptureEntry *const pad = SearchPad();
    std::int16_t stickX = 0;
    std::int16_t stickY = 0;
    std::uint16_t primaryTrigger = 0;
    std::uint16_t pressed = 0;
    std::uint16_t held = 0;
    std::uint16_t highTrigger = 0;
    std::uint16_t lowTrigger = 0;
    if (pad) {
        stickX = static_cast<std::int16_t>(static_cast<std::uint8_t>(pad->rightStick) - 128);
        stickY = static_cast<std::int16_t>(128 - static_cast<std::uint8_t>(pad->rightStick >> 8U));
        const bool swapped = g_applicationSettings.savedInput.swapTriggers != 0 && g_keyboardActive != 1;
        primaryTrigger = swapped ? static_cast<std::uint8_t>(pad->triggers) : static_cast<std::uint8_t>(pad->triggers >> 8U);
        held = pad->held;
        pressed = pad->pressedPriority;
        highTrigger = std::min<std::uint16_t>(static_cast<std::uint8_t>(pad->triggers >> 8U), 240);
        lowTrigger = std::min<std::uint16_t>(static_cast<std::uint8_t>(pad->triggers), 240);
    }
    std::int32_t desiredYaw = -1;
    if (AbsAngle16(stickX) > 10 || AbsAngle16(stickY) > 10)
        desiredYaw = atan2_fixed16(-static_cast<float>(stickX), -static_cast<float>(stickY));
    else if (primaryTrigger > 192 && (held & 0xf000U) != 0 && (held & 0x200U) != 0)
        desiredYaw = 0;


    Vector3 eye = g_CameraEyePos;
    Vector3 target = g_CameraTargetPos;
    g_searchStickXDelta = stickX;
    g_searchEyeWorldPosition = ryoPosition;
    g_searchCurrentCameraTarget = eye;
    g_searchCurrentCameraEye = target;
    g_searchCurrentIdentifier = -1;
    g_searchStickYDelta = stickY;
    g_searchTriggerPressure = primaryTrigger;

    Vector3 position{};
    Vector3 lockAt{};
    Vector3 cameraPosition{};
    bool entering = false;
    bool writeCamera = false;
    switch (static_cast<SearchState>(g_currentSearchState)) {
    case SearchState::IdleAwaitingTarget: {
        writeCamera = true;
        if (retargetDelay > 0)
            g_searchRetargetDelay = --retargetDelay;
        if ((!ShouldRun() || static_cast<double>(ryo->renderOpacity) > 0.3 || primaryTrigger < 192 || g_nAuthorState) && retargetDelay <= 0) {
            retargetDelay = 1;
            g_searchRetargetDelay = 1;
        }
        std::int32_t current = srchGetTransformedEntityPosition(position, ryoPosition);
        if (current < 0 && retargetDelay <= 0 && !SCEN_HasActiveScript()) {
            position = {target.x, target.y, target.z};
            const auto cameraYaw = static_cast<std::int16_t>(atan2_fixed16(eye.x - target.x, eye.z - target.z));
            current = FindCandidateInWorldOrScreen(eye, position, eye, ryoPosition, -1, cameraYaw, true);
        }
        g_searchCurrentIdentifier = current;
        bool characterSearch = current < 0;
        if (current >= 0) {
            const SearchEntry &entry = g_searchEntries[current];
            const std::int32_t rotationY = entry.useRotationY ? entry.rotationY : atan2_fixed16(ryoPosition.x - position.x, ryoPosition.z - position.z);
            srchFindPosRotForId(lockAt, cameraPosition, position, rotationY, current);
            if (srchCheckCanLockon(current, cameraPosition, position)) {
                retargetDelay = g_searchRetargetDelay;
                if (g_searchBlockedByCycleObject)
                    g_searchCurrentIdentifier = -1;
                characterSearch = true;
            } else {
                g_searchScriptTarget = position;
                g_currentSearchState = static_cast<std::int32_t>(SearchState::PendingScriptedLockOn);
                g_searchCandidateId = ~current;
                g_searchScriptTargetYaw = rotationY;
                entering = true;
            }
        }
        if (characterSearch) {
            if (retargetDelay > 0 || !srchFindClosestCharacterToScreenCenter(position, ryoHead))
                break;
            g_searchDistanceParameter = 15.0f;
            g_searchCharacterCameraCurrent = position;
            g_searchCharacterCameraSamples.fill(position);
            g_currentSearchState = static_cast<std::int32_t>(SearchState::AutoCharacterLockOn);
            g_searchSmoothingFactor = 0.0f;
            entering = true;
        }
        break;
    }
    case SearchState::PendingScriptedLockOn:
        g_searchDistanceParameter = 15.0f;
        g_currentSearchState = static_cast<std::int32_t>(SearchState::ScriptedLockOnActive);
        g_searchSmoothingFactor = 0.0f;
        if (g_pControlPadController)
            g_pControlPadController->busy = 1;
        // HelpToken_SearchQuest replaces the help while a scripted lock-on is up.
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_defaultHelpTokenId = 1;
        g_currentHelpTokenId = 1;
        g_helpButtonId = 0;
        g_currentHelpIconState = 0;
        g_searchCurrentIdentifier = candidate;
        SetRyoYawAgainstParents(ryo, g_searchScriptTargetYaw);
        if (candidate < 0)
            g_searchCurrentIdentifier = -1 - candidate;
        writeCamera = true;
        break;
    case SearchState::ScriptedLockOnActive: {
        if (g_pControlPadController)
            g_pControlPadController->busy = 1;
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_defaultHelpTokenId = 1;
        g_currentHelpTokenId = 1;
        g_helpButtonId = 0;
        g_currentHelpIconState = 0;
        float smoothing = 0.0f;
        if (g_searchDistanceParameter < 5.25f) {
            smoothing = static_cast<float>(g_searchSmoothingFactor - 0.07000000000000001);
            g_searchSmoothingFactor = smoothing;
            if (smoothing < 0.1f) {
                g_searchSmoothingFactor = 0.1f;
                smoothing = 0.1f;
            }
            if (g_searchCameraTransitionActive) {
                float step = static_cast<float>(highTrigger - lowTrigger) * 0.125f - accumulatedOffset;
                if (step > 3.0f || step < -3.0f)
                    step = static_cast<float>(static_cast<double>(step) * 0.03333);
                else if (step > 0.1f)
                    step = 0.1f;
                else if (step < -0.1f)
                    step = -0.1f;
                g_searchAccumulatedOffset = accumulatedOffset + step;
            } else {
                if (highTrigger < 10)
                    g_searchCameraTransitionActive = lowTrigger < 10;
                g_searchAccumulatedOffset = 0.0f;
            }
        } else {
            smoothing = static_cast<float>(g_searchSmoothingFactor + 0.07000000000000001);
            g_searchSmoothingFactor = smoothing;
            if (smoothing > 1.0f) {
                g_searchSmoothingFactor = 1.0f;
                smoothing = 1.0f;
            }
            if (accumulatedOffset > 3.0f)
                g_searchAccumulatedOffset = accumulatedOffset - 3.0f;
            else if (accumulatedOffset < -3.0f)
                g_searchAccumulatedOffset = accumulatedOffset + 3.0f;
            else
                g_searchAccumulatedOffset = 0.0f;
        }
        float blendDistance = g_searchDistanceParameter - smoothing;
        g_searchDistanceParameter = blendDistance;
        if (static_cast<double>(blendDistance) < 0.00001) {
            blendDistance = 0.0f;
            g_searchDistanceParameter = 0.0f;
        }
        const auto targetYaw = static_cast<std::int16_t>(g_searchScriptTargetYaw);
        srchFindPosRotForId(lockAt, cameraPosition, g_searchScriptTarget, g_searchScriptTargetYaw, candidate);
        const auto blend = static_cast<float>(2.0 / (blendDistance + 2.0));
        eye.x += (lockAt.x - eye.x) * blend;
        eye.z += (lockAt.z - eye.z) * blend;
        eye.y += (lockAt.y - eye.y) * blend;
        target.x += (g_searchScriptTarget.x - target.x) * blend;
        target.z += (g_searchScriptTarget.z - target.z) * blend;
        target.y += (g_searchScriptTarget.y - target.y) * blend;
        if (static_cast<double>(ryo->renderOpacity) > 0.001) {
            float opacity = ryo->renderOpacity - blend * ryo->renderOpacity;
            if (opacity < 0.001f)
                opacity = 0.0f;
            ryo->renderOpacity = opacity;
        }
        if (blendDistance > 0.0f)
            desiredYaw = -1;
        if (blendDistance < 3.0f && candidate < 0) {
            candidate = -1 - candidate;
            g_searchCandidateId = candidate;
        }
        std::uint16_t direction = pressed & 0xf000U;
        if (!direction)
            direction = g_searchDirectionInputLatched;
        if (direction != g_searchLastProcessedDirectionInput && direction && blendDistance > 0.0f) {
            g_searchDirectionInputLatched = direction;
            direction = 0;
        } else if (direction) {
            g_searchLastProcessedDirectionInput = direction;
            g_searchDirectionInputLatched = 0;
        }
        switch (direction) {
        case 0x1000U:
            desiredYaw = 0x8000;
            break;
        case 0x2000U:
            desiredYaw = 0;
            break;
        case 0x4000U:
            desiredYaw = 0x4000;
            break;
        case 0x8000U:
            desiredYaw = 0xc000;
            break;
        default:
            break;
        }
        position = g_searchScriptTarget;
        std::int32_t next = srchGetTransformedEntityPosition(position, ryoPosition);
        if (next < 0 && (desiredYaw & 0xffff0000) == 0)
            next = FindCandidateInWorldOrScreen(eye, position, cameraPosition, ryoPosition, static_cast<std::int16_t>(desiredYaw), targetYaw, false);
        if (next >= 0) {
            const SearchEntry &entry = g_searchEntries[next];
            const std::int32_t rotationY = entry.useRotationY ? entry.rotationY : atan2_fixed16(ryoPosition.x - position.x, ryoPosition.z - position.z);
            Vector3 nextLockAt{};
            Vector3 nextCameraPosition{};
            srchFindPosRotForId(nextLockAt, nextCameraPosition, position, rotationY, next);
            if (srchCheckCanLockon(next, nextCameraPosition, position)) {
                candidate = g_searchCandidateId;
            } else {
                candidate = ~next;
                g_searchScriptTarget = position;
                g_searchCandidateId = ~next;
                g_currentSearchState = static_cast<std::int32_t>(SearchState::PendingScriptedLockOn);
                g_searchScriptTargetYaw = rotationY;
            }
            blendDistance = g_searchDistanceParameter;
        }
        g_searchCurrentIdentifier = candidate < 0 ? -1 - candidate : candidate;
        if (((pressed & 0x200U) != 0 && primaryTrigger < 0xc0) ||
            (blendDistance <= 0.0f && srchCheckCanLockon(candidate, cameraPosition, g_searchScriptTarget))) {
            g_currentSearchState = static_cast<std::int32_t>(SearchState::ExitAndRestoreState);
        }
        if (g_nAuthorState != kAuthorScripted) {
            g_searchCandidateId = -1;
            g_currentSearchState = 0;
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
            g_searchStateFlag = 0;
            return;
        }
        writeCamera = true;
        break;
    }
    case SearchState::AutoCharacterLockOn: {
        g_searchCurrentIdentifier = 99999;
        if (g_pControlPadController)
            g_pControlPadController->busy = 1;
        float smoothing = 0.0f;
        if (g_searchDistanceParameter >= 5.25f) {
            smoothing = static_cast<float>(g_searchSmoothingFactor + 0.07000000000000001);
            g_searchSmoothingFactor = smoothing;
            if (smoothing > 1.0f) {
                g_searchSmoothingFactor = 1.0f;
                smoothing = 1.0f;
            }
        } else {
            smoothing = static_cast<float>(g_searchSmoothingFactor - 0.07000000000000001);
            g_searchSmoothingFactor = smoothing;
            if (smoothing < 0.1f) {
                smoothing = 0.1f;
                g_searchSmoothingFactor = 0.1f;
            }
        }
        float remaining = g_searchDistanceParameter - smoothing;
        g_searchDistanceParameter = remaining;
        if (static_cast<double>(remaining) < 0.00001) {
            g_searchDistanceParameter = 0.0f;
            remaining = 0.0f;
        }
        if ((pressed & 0xf000U) != 0) {
            switch (held & 0xf000U) {
            case 0x1000U:
                desiredYaw = 0x8000;
                break;
            case 0x2000U:
                desiredYaw = 0;
                break;
            case 0x4000U:
                desiredYaw = 0x4000;
                break;
            case 0x5000U:
                desiredYaw = 0x6000;
                break;
            case 0x6000U:
                desiredYaw = 0x2000;
                break;
            case 0x8000U:
                desiredYaw = 0xc000;
                break;
            case 0x9000U:
                desiredYaw = 0xa000;
                break;
            case 0xa000U:
                desiredYaw = 0xe000;
                break;
            default:
                break;
            }
        }
        if ((desiredYaw & 0xffff0000) != 0 || remaining > 0.0f)
            srchResolveFocusedTarget();
        else
            srchFindClosestPointOnMapWithinAngle(desiredYaw);
        const std::uint32_t focused = g_searchTemporaryFocusedCharacter;
        if (focused != previousFocused) {
            remaining = 15.0f;
            g_searchDistanceParameter = 15.0f;
        }
        Character *const character = focused ? LookupCharacter(focused) : nullptr;
        if (!character || ((pressed & 0x200U) != 0 && primaryTrigger < 0xc0)) {
            g_currentSearchState = static_cast<std::int32_t>(SearchState::ExitAndRestoreState);
        } else {
            Vector3 head{};
            mtwkGetCharaBonePosition(character, 0x2f, &head);
            head.y = static_cast<float>(head.y + 0.05);
            const float headDistance = std::sqrt((head.z - ryoPosition.z) * (head.z - ryoPosition.z) + (head.x - ryoPosition.x) * (head.x - ryoPosition.x));
            const Vector3 body = GetPositionForEntity(character->entity.node);
            const float distance = std::fmax(
                0.0f, headDistance - std::sqrt((body.z - ryoPosition.z) * (body.z - ryoPosition.z) + (body.x - ryoPosition.x) * (body.x - ryoPosition.x)));
            std::int32_t policy = -1;
            if (CycleManager::GetCycleFlagForSpecificChara(focused)) {
                policy = 2;
            } else {
                for (const ConversationCyclePolicy &entry : kConvCamCycleFilterPolicyByCharaId) {
                    if (entry.characterId == focused) {
                        policy = static_cast<std::int8_t>(entry.policy);
                        break;
                    }
                }
            }
            std::int32_t samples = 1;
            float weight = 0.33333299f;
            float pull = 0.25f;
            float radius = static_cast<float>(distance + 0.55);
            if (policy == 1) {
                weight = 0.2f;
                samples = 3;
                pull = 0.40000001f;
                radius = static_cast<float>(distance + 0.7);
            } else if (policy) {
                weight = 0.142857f;
                samples = 5;
                pull = 0.55000001f;
                radius = static_cast<float>(distance + 0.9);
            }
            float sumX = head.x;
            float sumZ = head.z;
            for (std::int32_t index = samples; index >= 0; --index) {
                const Vector3 &sample = index ? g_searchCharacterCameraSamples[static_cast<std::size_t>(index - 1)] : g_searchCharacterCameraCurrent;
                sumX += sample.x;
                sumZ += sample.z;
            }
            float sumY = g_searchCharacterCameraCurrent.y + head.y;
            for (std::size_t index = g_searchCharacterCameraSamples.size(); index-- > 0;)
                sumY += g_searchCharacterCameraSamples[index].y;
            for (std::size_t index = g_searchCharacterCameraSamples.size() - 1; index > 0; --index)
                g_searchCharacterCameraSamples[index] = g_searchCharacterCameraSamples[index - 1];
            g_searchCharacterCameraSamples[0] = g_searchCharacterCameraCurrent;
            g_searchCharacterCameraCurrent = head;
            const Vector3 average{sumX * weight, static_cast<float>(sumY * 0.142857), sumZ * weight};
            if (SearchListCount() > 1) {
                pull = 0.80000001f;
                radius = 0.0f;
            }
            float offsetX = (ryoHead.x - average.x) * pull;
            float offsetZ = (ryoHead.z - average.z) * pull;
            float offsetY = (ryoHead.y - average.y) * pull;
            const float lengthSquared = offsetZ * offsetZ + offsetX * offsetX;
            if (radius * radius > lengthSquared) {
                const float scale = radius / std::sqrt(lengthSquared);
                offsetX *= scale;
                offsetY *= scale;
                offsetZ *= scale;
            }
            const auto blend = static_cast<float>(2.0 / (remaining + 2.0));
            target.z += (average.z - target.z) * blend;
            eye.z += (offsetZ + average.z - eye.z) * blend;
            eye.x += (offsetX + average.x - eye.x) * blend;
            target.x += (average.x - target.x) * blend;
            eye.y += (offsetY + average.y - eye.y) * blend;
            target.y += (average.y - target.y) * blend;
            SetRyoYawAgainstParents(ryo, static_cast<std::int16_t>(atan2_fixed16(eye.x - target.x, eye.z - target.z)));
            if (static_cast<double>(ryo->renderOpacity) > 0.001) {
                float opacity = ryo->renderOpacity - blend * ryo->renderOpacity;
                if (opacity < 0.001f)
                    opacity = 0.0f;
                ryo->renderOpacity = opacity;
            }
            if (IsOutsideViewCone(ryoHead, average))
                g_searchTemporaryFocusedCharacter = 0;
        }
        if (g_nAuthorState != kAuthorScripted) {
            g_searchTemporaryFocusedCharacter = 0;
            g_currentSearchState = 0;
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
            g_searchStateFlag = 0;
            return;
        }
        writeCamera = true;
        break;
    }
    case SearchState::RecenterAndFlipToScript:
        g_searchDistanceParameter = 15.0f;
        g_searchSmoothingFactor = 0.0f;
        if (candidate >= 0)
            g_searchCandidateId = -1 - candidate;
        g_currentSearchState = static_cast<std::int32_t>(SearchState::ScriptedLockOnActive);
        writeCamera = true;
        break;
    case SearchState::ExitAndRestoreState:
        g_temporaryGameState = g_searchPreviousGameState;
        g_bIsConversationCamInactive = 0;
        ApplyPendingGameState(0);
        SetNewAuthorGameState(g_searchSavedAuthorState);
        // Leaving search mode puts back the help saved on entry; tokens from 68 up become HelpToken_FreeQuest.
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_currentHelpIconState = 0;
        g_defaultHelpTokenId = g_searchPreviousHelpToken < 68 ? g_searchPreviousHelpToken : 0;
        g_currentHelpTokenId = g_defaultHelpTokenId;
        g_helpButtonId = g_searchPreviousHelpButton;
        g_searchStateFlag = 0;
        g_searchRetargetDelay = 10;
        g_searchCameraTransitionActive = 0;
        g_searchAccumulatedOffset = 0.0f;
        g_searchCandidateId = -1;
        g_currentSearchState = static_cast<std::int32_t>(SearchState::IdleAwaitingTarget);
        g_searchTemporaryFocusedCharacter = 0;
        ResetPromptSlots();
        return;
    default:
        g_currentSearchState = 0;
        writeCamera = true;
        break;
    }

    if (entering) {
        g_searchAnyInputPressedLatched = 0;
        g_searchAPressedLatched = 0;
        g_searchSavedAuthorState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
        g_searchDirectionInputLatched = 0;
        g_searchLastProcessedDirectionInput = 0;
        g_searchPreviousGameState = g_temporaryGameState;
        g_temporaryGameState = 0;
        ApplyPendingGameState(0);
        SetNewAuthorGameState(kAuthorScripted);
        g_searchCameraOffsets = {};
        g_searchPreviousHelpToken = g_defaultHelpTokenId;
        g_searchPreviousHelpButton = g_helpButtonId;
        g_searchStateFlag = 1;
        if (static_cast<SearchState>(g_currentSearchState) == SearchState::PendingScriptedLockOn) {
            g_previousHelpTokenId = g_currentHelpTokenId;
            g_defaultHelpTokenId = 1;
            g_currentHelpTokenId = 1;
            g_helpButtonId = 0;
            g_currentHelpIconState = 0;
        }
        if (g_pControlPadController)
            g_pControlPadController->busy = 1;
        if (g_searchModeEnterCount != -1)
            ++g_searchModeEnterCount;
    } else if (!writeCamera || !g_searchStateFlag) {
        return;
    }

    g_searchCameraOffsets.x = static_cast<std::uint16_t>(g_searchCameraOffsets.x + 728);
    g_searchCameraOffsets.y = static_cast<std::uint16_t>(g_searchCameraOffsets.y + 546);
    g_searchCameraOffsets.z = static_cast<std::uint16_t>(g_searchCameraOffsets.z + 364);
    const Vector3 jittered{static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_searchCameraOffsets.x))) * 0.005 + eye.x),
        static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_searchCameraOffsets.y))) * 0.005 + eye.y),
        static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_searchCameraOffsets.z))) * 0.005 + eye.z)};
    TranslateCameraRelative(jittered);
    if (!bLockCamera) {
        g_CameraTargetPos = target;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
}

void HideFocusedItem() {
    if (g_searchFocusedItem)
        PresentSearchFocusedItem(false);
    g_searchFocusedItem = nullptr;
    g_searchFocusedItemState = 0;
}

void ResetLockCameraWork() {
    g_searchLockCameraPhase = 0;
    g_searchTargetObjectPosition = {};
    g_searchOrbitCenter = {};
    g_searchLockOnYaw = 0;
    g_searchLockOnPitch = 0;
    g_searchOrbitYaw = 0;
    g_searchOrbitPitch = 0;
    g_searchEyeToTargetDistance = 0.0f;
    g_searchTargetDistanceFromRadius = 0.0f;
    g_searchEyeDistanceStep = 0.0f;
    g_searchTargetDistanceStep = 0.0f;
    g_searchEyeYawDelta = 0;
    g_searchEyePitchDelta = 0;
    g_searchTargetYawDelta = 0;
    g_searchTargetPitchDelta = 0;
}

} // namespace


void SRCH_SetState(SearchState state, bool cameraTransitionActive) {
    g_currentSearchState = static_cast<std::int32_t>(state);
    g_searchCameraTransitionActive = cameraTransitionActive ? 1 : 0;
}

SearchState SRCH_GetState() {
    return static_cast<SearchState>(g_currentSearchState);
}


void SRCH_UpdateCameraEyeAndTarget(Vector3 &eye, Vector3 &target) {
    g_searchCameraOffsets = {};
    const auto state = static_cast<std::uint32_t>(g_currentSearchState);
    if (state - 2U > 1U)
        return;
    Vector3 searchEye{};
    Vector3 cameraPosition{};
    const Vector3 base = g_searchScriptTarget;
    srchFindPosRotForId(searchEye, cameraPosition, base, g_searchScriptTargetYaw, g_searchCandidateId);
    eye = searchEye;
    target = base;
}

bool shenHasFocusedNotLockedOn() {
    const auto state = static_cast<std::uint32_t>(g_currentSearchState);
    return g_searchCandidateId >= 0 || (((state - 2U) & ~3U) == 0 && state != 4U) || g_searchTemporaryFocusedCharacter != 0 || camCtrlGetState() > 0;
}

bool SRCH_AllowsIconInputPriority() {
    return SRCH_GetState() != SearchState::ScriptedLockOnActive || g_searchCameraTransitionActive != 0;
}

void SRCH_UpdatePromptState(std::uint16_t pressedPriority, std::uint16_t triggers, bool alternatePromptLayout, bool swapTriggers) {
    std::int16_t prompt0 = 45;
    std::int16_t prompt1 = 44;
    if (alternatePromptLayout)
        std::swap(prompt0, prompt1);
    const bool triggerActive = swapTriggers ? static_cast<std::uint8_t>(triggers) != 0 : static_cast<std::uint8_t>(triggers >> 8U) != 0;
    if (triggerActive) {
        if (alternatePromptLayout)
            prompt0 = -1;
        else
            prompt1 = -1;
    }
    g_searchPromptIconSlots = {prompt0, prompt1, -1, -1, -1};
    g_searchRequestedPromptIconSlots = g_searchPromptIconSlots;
    SRCH_ApplySearchPromptSlots();
    if ((pressedPriority & 0x400U) != 0)
        g_searchAPressedLatched = 1;
    if ((pressedPriority & 0xf6c6U) != 0)
        g_searchAnyInputPressedLatched = 1;
}

const std::array<std::int16_t, 5> &SRCH_GetPromptIconSlots() {
    return g_searchPromptIconSlots;
}

const std::array<std::int16_t, 5> &SRCH_GetRequestedPromptIconSlots() {
    return g_searchRequestedPromptIconSlots;
}

void SRCH_SetCharacterSearchExtent(bool primary, float value) {
    if (primary)
        g_maxCharacterSearchExtent = value;
    else
        g_fallbackSearchExtent = value;
}

std::int32_t SRCH_FindCandidate(bool screenSpace) {
    Character *const ryo = SearchRyo();
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    Vector3 position = g_CameraTargetPos;
    const Vector3 eye = g_CameraEyePos;
    const std::int32_t explicitTarget = srchGetTransformedEntityPosition(position, ryoPosition);
    if (explicitTarget >= 0)
        return explicitTarget;
    const auto cameraYaw = static_cast<std::int16_t>(atan2_fixed16(eye.x - g_CameraTargetPos.x, eye.z - g_CameraTargetPos.z));
    return FindCandidateInWorldOrScreen(eye, position, eye, ryoPosition, -1, cameraYaw, screenSpace);
}

void SRCH_ReleaseLockOnCamera() {
    if ((g_searchLockOnFlags & 1U) != 0) {
        g_searchLockOnFlags &= ~7U;
        SRCH_ExecuteCommand(SearchCommand::Reset);
    }
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
}



std::uint32_t SRCH_ExecuteCommand(SearchCommand command, std::int32_t operand) {
    if (command == SearchCommand::Reset) {
        SRCH_Reset();
        return 0;
    }
    if (command == SearchCommand::GetCurrentTargetIndex) {
        if (g_searchCandidateId >= 0)
            return static_cast<std::uint32_t>(g_searchCandidateId);
        const auto state = static_cast<std::uint32_t>(g_currentSearchState);
        if (((state - 2U) & ~3U) != 0 || state == 4U)
            return static_cast<std::uint32_t>(-1);
        return static_cast<std::uint32_t>(~g_searchCandidateId);
    }
    if (command == SearchCommand::SetEntrySelectorFilter) {
        g_searchEntrySelectorFilter = operand;
        return static_cast<std::uint32_t>(-1);
    }
    if (command == SearchCommand::SetExplicitTargetMapInfoId) {
        g_searchPendingExplicitMapInfoId = operand;
        return static_cast<std::uint32_t>(-1);
    }
    if (command == SearchCommand::ReturnFocused)
        return g_searchTemporaryFocusedCharacter;
    if (command != SearchCommand::UpdateNormal && command != SearchCommand::UpdateAndGetFocused) {
        return 0;
    }

    const bool focused = command == SearchCommand::UpdateAndGetFocused;
    std::int32_t focusedUpdatesThisFrame = g_searchUpdatesThisFrame;
    const std::int32_t tickCount = ++g_searchTickCount;
    if (focused)
        focusedUpdatesThisFrame = ++g_searchUpdatesThisFrame;

    std::uint32_t result = static_cast<std::uint32_t>(-1);
    if ((g_searchLockOnFlags & 1U) != 0) {
        if (tickCount > 1)
            return static_cast<std::uint32_t>(g_searchCachedResult);
        srchUpdateForCurrentDetected();
        switch (g_searchLockCameraPhase) {
        case 0:
            if (srchUpdateLockOnCam())
                ++g_searchLockCameraPhase;
            break;
        case 1:
            result = static_cast<std::uint32_t>(g_searchPendingSelectedIdentifier);
            g_searchLockOnFlags |= 2U;
            g_searchPendingSelectedIdentifier = -1;
            g_searchLockCameraPhase = 2;
            break;
        case 2:
            if (g_searchAnyInputPressedLatched)
                g_searchLockCameraPhase = 3;
            break;
        case 3:
            g_searchLockOnFlags &= ~7U;
            g_currentSearchState = static_cast<std::int32_t>(SearchState::RecenterAndFlipToScript);
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
            break;
        default:
            break;
        }
        g_searchCachedResult = static_cast<std::int32_t>(result);
        return result;
    }

    if (tickCount <= 1) {
        const auto state = static_cast<std::uint32_t>(g_currentSearchState);
        if (((state - 2U) & ~3U) != 0 || state == 4U) {
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
        } else {
            srchUpdateForCurrentDetected();
        }
        srchUpdate();
    } else if (focused) {
        if (focusedUpdatesThisFrame > 1)
            return static_cast<std::uint32_t>(g_searchCachedFocused);
    } else if (tickCount - focusedUpdatesThisFrame > 1) {
        return static_cast<std::uint32_t>(g_searchCachedUpdateResult);
    }

    const std::int32_t authorState = g_nAuthorState;
    const bool authorAllowsSelection = authorState == 0 || authorState == 9 || authorState == 17;
    if ((g_searchAPressedLatched != 0 || focused) && authorAllowsSelection && SearchRyo() && srchUpdateLockOnCamTargetPosRot()) {
        if (!focused) {
            g_searchAnyInputPressedLatched = 0;
            g_searchAPressedLatched = 0;
            if (g_searchSelectCount != -1)
                ++g_searchSelectCount;
        }
        if (g_searchSelectedEntryType == 1 || g_searchSelectedEntryType == -2) {
            srchComputeCameraForLockOnObject();
        } else {
            result = static_cast<std::uint32_t>(g_searchPendingSelectedIdentifier);
            g_searchPendingSelectedIdentifier = -1;
        }
    }

    if (focused)
        g_searchCachedFocused = static_cast<std::int32_t>(result);
    else
        g_searchCachedUpdateResult = static_cast<std::int32_t>(result);
    return result;
}

std::int32_t SRCH_GetCurrentIdentifier() {
    return g_searchCurrentIdentifier;
}


void SRCH_InitEx(const SearchEntry *entries) {
    g_searchLockOnFlags &= ~7U;
    g_searchTargetDistance = 0.60000002f;
    g_searchAccumulatedOffset = 0.0f;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_currentSearchState = 0;
    g_searchPendingExplicitMapInfoId = 0;
    g_searchCameraTransitionActive = 0;
    g_searchTemporaryFocusedCharacter = 0;
    g_searchFocusedItem = nullptr;
    g_searchFocusedItemState = 0;
    g_searchEntries = const_cast<SearchEntry *>(entries);
    g_searchEntrySelectorFilter = -1;
    g_searchPendingSelectedIdentifier = -1;
    g_searchCandidateYawConeWidth = 27306;
    g_maxCharacterSearchExtent = 2.5f;
    g_fallbackSearchExtent = 2.5f;
    g_searchCurrentIdentifier = -1;
    g_searchCandidateId = -1;
}


void SRCH_Init() {
    g_searchLockOnFlags &= ~7U;
    g_searchTargetDistance = 0.60000002f;
    g_searchAccumulatedOffset = 0.0f;
    g_searchEntries = nullptr;
    g_searchEntrySelectorFilter = -1;
    g_searchPendingSelectedIdentifier = -1;
    g_searchCandidateYawConeWidth = 27306;
    g_maxCharacterSearchExtent = 2.5f;
    g_fallbackSearchExtent = 2.5f;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    g_currentSearchState = 0;
    g_searchCurrentIdentifier = -1;
    g_searchCandidateId = -1;
    g_searchPendingExplicitMapInfoId = 0;
    g_searchCameraTransitionActive = 0;
    g_searchTemporaryFocusedCharacter = 0;
    g_searchFocusedItem = nullptr;
    g_searchFocusedItemState = 0;
    g_searchUpdatesThisFrame = 0;
    g_searchTickCount = 0;
    g_searchCachedResult = -1;
    g_searchCachedFocused = -1;
    g_searchCachedUpdateResult = -1;


    g_searchTask = HLib_EnqueueTaskWithoutParameter(SRCH_Callback, 8, 2, SHEN_NAME(SRCH));
}

HLib::Task *SRCH_GetTask() {
    return g_searchTask;
}

std::int32_t SRCH_GetBoundEntryCount() {
    if (!g_searchEntries)
        return 0;
    std::int32_t count = 0;
    while (count < kMaximumSearchEntries && g_searchEntries[count].id >= 0)
        ++count;
    return count;
}


void SRCH_Reset() {
    const std::uint32_t state = static_cast<std::uint32_t>(g_currentSearchState);
    if (state - 2U <= 4U) {
        g_previousHelpTokenId = g_currentHelpTokenId;
        g_currentHelpIconState = 0;
        g_defaultHelpTokenId = g_searchPreviousHelpToken < 68 ? g_searchPreviousHelpToken : 0;
        g_currentHelpTokenId = g_defaultHelpTokenId;
        g_helpButtonId = g_searchPreviousHelpButton;
    }
    g_currentSearchState = 0;
    g_searchCandidateId = -1;
    g_searchTemporaryFocusedCharacter = 0;
    g_searchAnyInputPressedLatched = 0;
    g_searchAPressedLatched = 0;
    ResetPromptSlots();
}
void SRCH_ClearLockAndHandleReset() {
    g_searchLockOnFlags &= ~7U;
    (void)SRCH_ExecuteCommand(SearchCommand::Reset);
}
void SRCH_Shutdown() {
    SRCH_Reset();
    HideFocusedItem();
    g_searchLockOnFlags &= ~7U;
    ResetLockCameraWork();

    if (g_searchTask && g_searchTask->mainTaskCallbackFnPtr == SRCH_Callback)
        HLib_DestroyTask(g_searchTask);
    g_searchTask = nullptr;
    g_searchEntries = nullptr;
}

namespace {

std::int32_t CurrentSearchItemIdentifier(std::int32_t identifier) {
    if (identifier < 0 || identifier >= 10000 || !g_searchEntries)
        return -1;
    return static_cast<std::int32_t>(g_searchEntries[identifier].name);
}

std::int32_t SpecialAreaSearchItemIdentifier() {
    if (g_areaId != SEARCH_AREA_JABE.name && g_areaId != SEARCH_AREA_DCBN.name && g_areaId != SEARCH_AREA_MKYU.name) {
        return -1;
    }
    const std::uint8_t identifier = g_scenarioStepLut.steps[7];
    return identifier != 0 ? static_cast<std::int32_t>(identifier) : -1;
}



std::int32_t srchCheckEntityInScreenRange() {
    std::int32_t index = -1;
    const auto screenWidth = static_cast<float>(g_originalScreenWidth);
    const auto screenHeight = static_cast<float>(g_originalScreenHeight);
    float maxExtent = 10000000.0f;
    const auto cameraYaw = static_cast<std::int16_t>(
        atan2_fixed16(g_searchCurrentCameraTarget.x - g_searchCurrentCameraEye.x, g_searchCurrentCameraTarget.z - g_searchCurrentCameraEye.z));
    Matrix4x4_RotateAndPush();
    for (std::int32_t current = 0; kSearchScreenConstraints[current].value >= 0; ++current) {
        const SearchScreenConstraint &constraint = kSearchScreenConstraints[current];
        const Vector3 world{constraint.x, constraint.value == 2000 ? 3.45f : 1.8f, constraint.z};
        const float deltaX = g_searchEyeWorldPosition.x - constraint.x;
        const float deltaZ = g_searchEyeWorldPosition.z - constraint.z;
        if (deltaZ * deltaZ + deltaX * deltaX > 49.0f)
            continue;
        const auto direction = static_cast<std::int16_t>(atan2_fixed16(deltaX, deltaZ));
        if (cos_fixed16(static_cast<std::uint16_t>(direction - cameraYaw)) < 0.25881901f)
            continue;
        if (cos_fixed16(static_cast<std::uint16_t>(direction - constraint.angle)) < 0.25881901f)
            continue;
        Vector2 screen{};
        WorldToScreen(world, screen);
        float x = screen.x;
        if (constraint.value == 2000) {
            screen.y -= screenHeight * 0.5f;
            if (screen.y < -140.0f || screen.y > 140.0f)
                x = -100.0f;
        }
        const float centered = x - screenWidth * 0.5f;
        if (centered > -180.0f && centered < 180.0f && maxExtent > centered * centered) {
            maxExtent = centered * centered;
            index = current;
        }
    }
    return index < 0 ? index : kSearchScreenConstraints[index].value;
}

bool SearchInputActive() {
    return AbsAngle16(g_searchStickXDelta) > 10 || AbsAngle16(g_searchStickYDelta) > 10 || static_cast<std::int16_t>(g_searchTriggerPressure) > 10;
}

} // namespace

void SRCH_Callback(HLib::TaskData *) {
    g_searchCachedResult = -1;
    g_searchUpdatesThisFrame = 0;
    g_searchTickCount = 0;
    g_searchCachedFocused = -1;
    g_searchCachedUpdateResult = -1;
    srchUpdateFocusedItem();
}


void srchUpdateFocusedItem() {
    const std::int32_t focusedItemState = g_searchFocusedItemState;
    if (focusedItemState == 0) {
        std::int32_t itemIdentifier = -1;
        bool bypassScriptAndRunChecks = false;
        if (g_searchCurrentIdentifier >= 0) {
            itemIdentifier = CurrentSearchItemIdentifier(g_searchCurrentIdentifier);
        } else {
            itemIdentifier = SpecialAreaSearchItemIdentifier();
            if (itemIdentifier >= 0)
                bypassScriptAndRunChecks = true;
            else if (SearchInputActive())
                itemIdentifier = srchCheckEntityInScreenRange();
        }
        if (!bypassScriptAndRunChecks && (SCEN_HasActiveScript() || !ShouldRun()))
            itemIdentifier = -1;
        const SearchFocusMapping *const item = GetSearchMappingForSearchItem(itemIdentifier);
        if (item) {
            g_searchFocusedItem = item;
            g_searchFocusedItemState = 1;
        }
        g_searchCurrentIdentifier = -1;
        return;
    }

    if (focusedItemState == 1) {
        if (g_searchFocusedItem)
            PresentSearchFocusedItem(true);
        g_searchFocusedItemState = focusedItemState + 1;
        g_searchCurrentIdentifier = -1;
        return;
    }

    std::int16_t timeout = 0;
    bool bypassScriptAndRunChecks = false;
    const SearchFocusMapping *candidateItem = nullptr;
    if (g_searchCurrentIdentifier >= 0) {
        const std::int32_t itemIdentifier = g_searchCurrentIdentifier < 10000 ? CurrentSearchItemIdentifier(g_searchCurrentIdentifier) : -1;
        candidateItem = GetSearchMappingForSearchItem(itemIdentifier);
        if (!candidateItem)
            candidateItem = reinterpret_cast<const SearchFocusMapping *>(0x10);
    } else {
        std::int32_t itemIdentifier = SpecialAreaSearchItemIdentifier();
        g_searchCurrentIdentifier = itemIdentifier;
        if (itemIdentifier >= 0) {
            bypassScriptAndRunChecks = true;
        } else if (SearchInputActive()) {
            itemIdentifier = srchCheckEntityInScreenRange();
            g_searchCurrentIdentifier = itemIdentifier;
        }
        candidateItem = GetSearchMappingForSearchItem(itemIdentifier);
        if (!candidateItem)
            timeout = g_searchFocusedItemTimeoutFrames;
    }
    if (candidateItem)
        timeout = candidateItem == g_searchFocusedItem ? 90 : 0;
    if (!bypassScriptAndRunChecks && (SCEN_HasActiveScript() || !ShouldRun()))
        timeout = 0;
    g_searchFocusedItemTimeoutFrames = static_cast<std::int16_t>(timeout - 1);
    if (g_searchFocusedItemTimeoutFrames <= 0)
        HideFocusedItem();
    g_searchCurrentIdentifier = -1;
}

} // namespace shenmue::s1
