#include "s1/task/kdan.hpp"
#include "s1/boot.hpp"
#include "s1/collision.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/motion.hpp"
#include "s1/player_control.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/came.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/film.hpp"
#include "s1/task/mmtk.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/save.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/wexe.hpp"
#include "s1/ui_runtime.hpp"

#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <vector>

namespace shenmue::s1 {

extern std::int32_t g_currentSearchState;
extern std::int32_t g_searchCandidateId;
extern std::uint32_t g_searchTemporaryFocusedCharacter;

KaidanOccupancyEntry *g_kaidanOccupancyHead = nullptr;
KaidanOccupancyEntry *g_kaidanFreeListHead = nullptr;
KaidanOccupancyEntry *g_kaidanData = nullptr;
KaidanOccupancyEntry *g_kaidanIterator = nullptr;
std::int32_t g_playerRawStairGuideEntryId = -1;
std::int32_t g_playerLatchedStairGuideEntryId = -1;
std::int32_t g_playerInStairPrivateVolume = 0;
std::int32_t g_playerStairGuidePairConflict = 0;

namespace {

constexpr std::uint32_t kStairToken = SHEN_NAME(STIR);
constexpr std::uint32_t kPrivateToken = SHEN_NAME(PRIV);

MapControlData *KaidanMapControl() {
    return g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

bool KaidanOccupantOnStairs(const KaidanOccupancyEntry &entry) {
    const CycleManager::NpcRecord *const npc = entry.npc;
    if (!npc || !npc->moveConfigOffset || !CycleManager::FileOffset(npc->cycle->cycleCommandPtr) || entry.charaId.name != npc->cycle->moveConfig->obj_id.name)
        return false;
    std::uint32_t occupancy = npc->cycle->currentOpcode == CycleOpcode_FollowStairPath2D ? 1U : 0U;
    const std::vector<std::uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    std::uint32_t command = 0;
    if (static_cast<std::size_t>(CycleManager::FileOffset(npc->cycle->cycleCommandPtr)) + sizeof(command) <= bytes.size())
        std::memcpy(&command, bytes.data() + CycleManager::FileOffset(npc->cycle->cycleCommandPtr), sizeof(command));
    if (command == CycleOpcode_FollowStairPath2D)
        occupancy |= 2U;
    if (entry.stairGuideEntryId == 42 || entry.stairGuideEntryId == 44) {
        const float x = npc->cycle->pos.x - -38.849998f;
        const float y = npc->cycle->pos.y - 3.95f;
        const float z = npc->cycle->pos.z - 39.0f;
        if (y * y + x * x + z * z < 4.0f)
            occupancy |= 4U;
    }
    return occupancy != 0;
}

} // namespace



Vector3 RotateAroundYAndTransformWorldCoords(const Vector3 &localOffset, const Vector3 &worldOrigin, std::uint16_t yaw) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(worldOrigin.x, worldOrigin.y, worldOrigin.z);
    const std::uint16_t folded = yaw <= 0x8000U ? yaw : static_cast<std::uint16_t>(-yaw);
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yaw)), cosine);
    Vector3 world{};
    Matrix4x4::TransformPoint(localOffset, world);
    Matrix4x4::Pop();
    return world;
}

void KDAN_Init() {
    g_kaidanOccupancyHead = nullptr;
    g_kaidanFreeListHead = nullptr;

    HLib::TaskData *block = g_pFirstTaskData;
    while (block && (block->status == 0x20455355 || block->content_size < 0xcc0)) {
        block = block->next_ptr;
    }
    if (!block) {
        g_kaidanData = nullptr;
        return;
    }

    HLib::TaskData *const next = block->next_ptr;
    const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
    if (oldSize > 0xd00) {
        auto *const remainder = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + 0xcc0);
        remainder->prev_ptr = block;
        remainder->next_ptr = next;
        remainder->status = SHEN_NAME(fREe);
        remainder->taskToken = SHEN_NAME(fREe);
        remainder->content_size = static_cast<std::int64_t>(oldSize - 0xcc0);
        block->next_ptr = remainder;
        if (next) {
            next->prev_ptr = remainder;
        }
        block->content_size = 0xcc0;
    }
    block->taskToken = SHEN_NAME(KDAN);
    block->status = 0x20455355;
    block->alignment = 0;
    g_kaidanData = reinterpret_cast<KaidanOccupancyEntry *>(block + 1);
}

void KDAN_ResetOccupancy() {
    g_kaidanFreeListHead = nullptr;
    KaidanOccupancyEntry *next = nullptr;
    if (KaidanOccupancyEntry *entry = g_kaidanData) {
        for (std::int32_t index = 0; index < 100; ++index, ++entry) {
            entry->freeNext = next;
            next = entry;
        }
        g_kaidanFreeListHead = next;
    }
    if (next)
        g_kaidanFreeListHead = next->freeNext;
    g_kaidanOccupancyHead = next;
    if (next) {
        next->stairGuideEntryId = -1;
        next->charaId.name = SHEN_NAME(NULL);
        next->npc = nullptr;
        next->activeNext = nullptr;
        next->freeNext = nullptr;
    }
}

void KDAN_RemoveOccupancyByCharacterId(std::uint32_t characterId) {
    KaidanOccupancyEntry *head = g_kaidanOccupancyHead;
    KaidanOccupancyEntry *freeHead = g_kaidanFreeListHead;
    for (KaidanOccupancyEntry *entry = head; entry; entry = entry->activeNext) {
        if (entry->charaId.name != characterId)
            continue;
        g_kaidanFreeListHead = entry;
        if (entry == head) {
            head = entry->activeNext;
            g_kaidanOccupancyHead = head;
            head->freeNext = nullptr;
        } else {
            KaidanOccupancyEntry *const previous = entry->freeNext;
            entry->activeNext->freeNext = previous;
            previous->activeNext = entry->activeNext;
        }
        entry->freeNext = freeHead;
        freeHead = entry;
    }
}

void KDAN_PruneStaleOccupancyEntries() {
    KaidanOccupancyEntry *head = g_kaidanOccupancyHead;
    if (!head)
        return;
    KaidanOccupancyEntry *freeHead = g_kaidanFreeListHead;
    for (KaidanOccupancyEntry *entry = head; entry; entry = entry->activeNext) {
        if (entry->stairGuideEntryId == -1 || KaidanOccupantOnStairs(*entry))
            continue;
        if (entry == head) {
            entry->activeNext->freeNext = nullptr;
            head = entry->activeNext;
        } else {
            entry->activeNext->freeNext = entry->freeNext;
            entry->freeNext->activeNext = entry->activeNext;
        }
        entry->freeNext = freeHead;
        freeHead = entry;
    }
    g_kaidanFreeListHead = freeHead;
    g_kaidanOccupancyHead = head;
}

bool KDAN_IsCharacterOccupyingStairs(std::uint32_t characterId) {
    for (KaidanOccupancyEntry *entry = g_kaidanOccupancyHead;; entry = entry->activeNext) {
        g_kaidanIterator = entry;
        if (!entry)
            return false;
        if (entry->stairGuideEntryId == -1) {
            g_kaidanIterator = nullptr;
            return false;
        }
        if (entry->charaId.name == characterId)
            return true;
    }
}

std::int32_t CheckSTIR_case(const Vector3 &position, std::uint16_t yaw) {
    float height = 0.0f;
    std::int32_t value8 = 0;
    std::int32_t valueC = 0;
    std::int32_t guideEntryId = 0;
    const CollisionPointQueryOutputs outputs{&height, &value8, &valueC, &guideEntryId};
    std::int32_t result = -1;
    Vector3 local{};
    bool hit = false;
    for (float distance = 0.0f;;) {
        local.z = distance > 1.5f ? distance - 1.5f : -distance;
        if (CheckIsPointIntersectWith(RotateAroundYAndTransformWorldCoords(local, position, yaw), &outputs, kStairToken)) {
            hit = true;
            break;
        }
        distance = distance + 0.30000001f;
        if (distance > 2.7f)
            break;
    }
    if (hit) {
        COLS_ResolveSelectedCollisionFunction();
        result = guideEntryId;
        if (guideEntryId != -1)
            return result;
    }
    local.z = 0.0f;
    for (float distance = 0.0f;;) {
        local.x = distance > 1.5f ? distance - 1.5f : -distance;
        if (CheckIsPointIntersectWith(RotateAroundYAndTransformWorldCoords(local, position, yaw), &outputs, kStairToken))
            break;
        distance = distance + 0.30000001f;
        if (distance > 3.0f)
            return result;
    }
    COLS_ResolveSelectedCollisionFunction();
    return guideEntryId;
}

bool Angle16_IsWithinMaxDelta(std::int32_t angle, std::int32_t otherAngle, std::int32_t maxAngle) {
    const std::int32_t delta = otherAngle <= angle ? angle - otherAngle : otherAngle - angle;
    if (delta < maxAngle)
        return true;
    const double degrees = static_cast<double>(static_cast<float>(maxAngle) / 182.04445f);
    return delta > static_cast<std::uint16_t>(static_cast<std::int32_t>((360.0 - degrees) * 8192.0 / 45.0));
}

std::int32_t CameraStair_QueryGuideCollision(HLib::Task *scenarioTask, std::uint32_t charaId, std::uint32_t guideCount, const CameraStairGuideEntry *guides,
    const CameraStairEventEntry *events, std::uint32_t maxYawDelta) {
    if (!charaId)
        return -1;
    Character *const character = CHID_Find(scenarioTask, charaId);
    if (!character)
        return -1;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const std::int32_t yaw = charaGetWorldYaw(character);
    SelectCharacterMapQuery(character);
    MapControlData *const control = KaidanMapControl();
    const bool searchBlocked = ((g_currentSearchState - 2) & ~3) == 0 && g_currentSearchState != static_cast<std::int32_t>(SearchState::AutoCharacterLockOn) &&
        g_searchCandidateId < 0;
    if (!ShouldRun() || g_isPlayerGameStateActive == 0 || g_notebookHelpOverrideActive || g_searchCandidateId >= 0 || searchBlocked ||
        g_searchTemporaryFocusedCharacter || camCtrlGetState() > 0 || !UI_ICSpIsIdle() || g_saveDisplayModuleState || g_NumFramesProcessed ||
        reinterpret_cast<const FilmColourInterpolation *>(reinterpret_cast<const std::byte *>(control->filmTask->task_data_ptr) + 4)->current[0] ||
        SCEN_HasActiveScript()) {
        control->cycleMask[1] = static_cast<std::byte>(std::to_integer<std::uint8_t>(control->cycleMask[1]) | 4U);
        return -1;
    }

    std::int32_t rawGuideEntryId;
    std::int32_t guideEntryId;
    std::int32_t inPrivateVolume;
    if (CheckIsPointIntersectWith(position, nullptr, kPrivateToken)) {
        g_playerInStairPrivateVolume = 1;
        g_temporaryKaidanId = g_playerLatchedStairGuideEntryId == -1 ? 10 : g_playerLatchedStairGuideEntryId;
        rawGuideEntryId = CheckSTIR_case(position, static_cast<std::uint16_t>(yaw));
        guideEntryId = g_temporaryKaidanId;
        inPrivateVolume = g_playerInStairPrivateVolume;
    } else {
        rawGuideEntryId = CheckSTIR_case(position, static_cast<std::uint16_t>(yaw));
        guideEntryId = rawGuideEntryId;
        g_temporaryKaidanId = rawGuideEntryId;
        inPrivateVolume = 0;
        g_playerInStairPrivateVolume = 0;
    }
    g_playerRawStairGuideEntryId = rawGuideEntryId;
    if (guideEntryId == -1) {
        if ((std::to_integer<std::uint8_t>(control->cycleMask[1]) & 4U) != 0)
            control->cycleMask[1] = static_cast<std::byte>(std::to_integer<std::uint8_t>(control->cycleMask[1]) & ~4U);
        g_playerKaidanGuideActive = 0;
        return -1;
    }

    g_playerStairGuidePairConflict = 0;
    KDAN_PruneStaleOccupancyEntries();
    const std::int32_t pairedEntryId = guideEntryId + (guideEntryId % 2 ? -1 : 1);
    for (KaidanOccupancyEntry *entry = g_kaidanOccupancyHead; entry; entry = entry->activeNext) {
        if (entry->stairGuideEntryId == guideEntryId || entry->stairGuideEntryId == pairedEntryId) {
            if (!g_playerKaidanGuideActive) {
                control->cycleMask[1] = static_cast<std::byte>(std::to_integer<std::uint8_t>(control->cycleMask[1]) | 4U);
                return -1;
            }
            break;
        }
    }
    if (g_kaidanOccupancyHead) {
        bool pairConflict = false;
        for (KaidanOccupancyEntry *entry = g_kaidanOccupancyHead; entry; entry = entry->activeNext) {
            if (!(guideEntryId % 2) && entry->stairGuideEntryId == guideEntryId + 1)
                pairConflict = entry->charaId.name != entry->activeNext->charaId.name;
        }
        if (pairConflict && g_playerKaidanGuideActive && inPrivateVolume) {
            control->cycleMask[1] = static_cast<std::byte>(std::to_integer<std::uint8_t>(control->cycleMask[1]) | 4U);
            g_playerStairGuidePairConflict = 1;
            return -1;
        }
    }
    control->cycleMask[1] = static_cast<std::byte>(std::to_integer<std::uint8_t>(control->cycleMask[1]) & ~4U);

    const float sine = sin_fixed16(static_cast<std::int16_t>(yaw));
    const std::uint16_t folded = static_cast<std::uint16_t>(yaw) <= 0x8000U ? static_cast<std::uint16_t>(yaw) : static_cast<std::uint16_t>(-yaw);
    for (std::uint32_t index = 0; index < guideCount; ++index) {
        const CameraStairGuideEntry &guide = guides[index];
        if (static_cast<double>(guide.y) + 0.4 < static_cast<double>(position.y) || static_cast<double>(position.y) < static_cast<double>(guide.y) - 0.4)
            continue;
        float probeHalfWidth = 0.40000001f;
        if ((guide.flags & 0x100U) != 0 && events->kind != 3U) {
            const CameraStairEventEntry *event = events;
            while (event->kind != 0U || event->guideIndex != index) {
                ++event;
                if (event->kind == 3U)
                    break;
            }
            if (event->kind != 3U)
                probeHalfWidth = event->probeHalfWidth;
        }
        const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
        const float z = position.z - guide.z;
        const float x = position.x - guide.x;
        const float localX = x * cosine - z * sine;
        const float localZ = z * cosine + x * sine;
        if (probeHalfWidth > localX && localX > -probeHalfWidth && localZ < 0.60000002f && localZ > -0.60000002f &&
            Angle16_IsWithinMaxDelta(yaw, static_cast<std::int32_t>(guide.yawAngle16), static_cast<std::int32_t>(maxYawDelta))) {
            g_fCurrMaxDistance = 0.5f;
            g_playerLatchedStairGuideEntryId = guideEntryId;
            g_playerKaidanGuideActive = 1;
            return static_cast<std::int32_t>(index);
        }
    }
    return -1;
}

std::uint32_t CameraStair_BuildDynamicGuideFromSTIR(HLib::Task *scenarioTask, std::uint32_t charaId, CameraStairGuideEntry *guide, std::int32_t maxAngle) {
    if (!charaId)
        return 0;
    Character *const character = CHID_Find(scenarioTask, charaId);
    if (!character)
        return 0;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const std::int32_t yaw = charaGetWorldYaw(character);
    auto *const bytes = reinterpret_cast<std::byte *>(guide);
    const CollisionPointQueryOutputs outputs{bytes + 4, bytes + 20, bytes + 16, bytes + 24};
    if (!CheckIsPointIntersectWith(position, &outputs, kStairToken))
        return 0;
    const std::uint32_t *const selected = COLS_ResolveSelectedCollisionFunction();
    if (selected[0] != 5U)
        return 0;
    float shape[7]{};
    std::memcpy(shape, selected, sizeof(shape));
    const float centreZ = (shape[4] + shape[6]) * 0.5f;
    guide->x = (shape[3] + shape[5]) * 0.5f + shape[1];
    guide->z = -(centreZ + shape[2]);
    const std::uint16_t guideYaw = atan2_fixed16(-shape[5], shape[6]);
    guide->yawAngle16 = guideYaw;
    if (!Angle16_IsWithinMaxDelta(yaw, guideYaw, maxAngle))
        return 0;
    g_fCurrMaxDistance = 0.5f;
    return 1;
}

void CameraStair_ApplyGuideFacingCorrection(HLib::Task *scenarioTask, std::uint32_t charaId, std::uint32_t guideIndex, const CameraStairGuideEntry *guides) {
    if (!charaId)
        return;
    Character *const character = CHID_Find(scenarioTask, charaId);
    if (!character)
        return;
    const Vector3 position = GetPositionForEntity(character->entity.node);
    const CameraStairGuideEntry &guide = guides[guideIndex];
    const std::uint16_t guideYaw = static_cast<std::uint16_t>(guide.yawAngle16);
    const float offset = static_cast<float>(guide.stepCount) * -0.30000001f;
    const float sine = sin_fixed16(static_cast<std::int16_t>(guideYaw));
    const std::uint16_t folded = guideYaw <= 0x8000U ? guideYaw : static_cast<std::uint16_t>(-guideYaw);
    const float targetX = sine * offset + guide.x;
    const float cosine = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
    float direction =
        static_cast<float>(static_cast<std::int32_t>(atan2_fixed16(-(targetX - position.x), -(cosine * offset + guide.z - position.z)))) / 182.04445f;
    const float guideDegrees = static_cast<float>(static_cast<std::int32_t>(guide.yawAngle16)) / 182.04445f;
    float delta;
    if (direction <= guideDegrees) {
        if (guideDegrees - direction > 180.0f)
            direction = direction + 360.0f;
        delta = direction - guideDegrees;
    } else {
        delta = direction - guideDegrees;
        if (delta > 180.0f)
            delta = delta + 360.0f;
    }
    if (delta >= 360.0f)
        delta = delta + -360.0f;
    if (delta < -5.0f)
        delta = -5.0f;
    else if (delta <= 180.0f)
        delta = std::fmin(5.0f, delta);
    else if (delta < 355.0f)
        delta = -5.0f;
    const Vector3i rotation{0, static_cast<std::uint16_t>(static_cast<std::int32_t>((guideDegrees + delta) * 8192.0f / 45.0f)), 0};
    ApplyEulerAnglesToNode(character->entity.node, rotation);
}


void KDAN_AddOccupancy(std::int32_t guideEntryId, CycleManager::NpcRecord &npc) {
    KaidanOccupancyEntry *const head = g_kaidanOccupancyHead;
    for (KaidanOccupancyEntry *entry = head; entry; entry = entry->activeNext) {
        if (entry->stairGuideEntryId == guideEntryId && entry->charaId.name == npc.cycle->moveConfig->obj_id.name)
            return;
    }
    KaidanOccupancyEntry *const entry = g_kaidanFreeListHead;
    if (!entry)
        return;
    g_kaidanFreeListHead = entry->freeNext;
    entry->stairGuideEntryId = guideEntryId;
    g_kaidanOccupancyHead = entry;
    entry->charaId.name = npc.cycle->moveConfig->obj_id.name;
    entry->npc = &npc;
    entry->activeNext = head;
    if (head)
        head->freeNext = entry;
}

namespace {

std::uint32_t KaidanNextCommand(const CycleManager::NpcRecord &npc) {
    std::uint32_t command = 0;
    const std::vector<std::uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (static_cast<std::size_t>(CycleManager::FileOffset(npc.cycle->cycleCommandPtr)) + sizeof(command) <= bytes.size())
        std::memcpy(&command, bytes.data() + CycleManager::FileOffset(npc.cycle->cycleCommandPtr), sizeof(command));
    return command;
}


float Vector3_SubtractInto(const Vector3 &from, const Vector3 &to) {
    Vector3 delta{from.x - to.x, from.y - to.y, from.z - to.z};
    if (0.001 > static_cast<double>(std::fabs(delta.x)) && 0.001 > static_cast<double>(std::fabs(delta.y)) && 0.001 > static_cast<double>(std::fabs(delta.z)))
        return 0.0f;
    return Vector3::NormalizeInPlace(delta);
}

const Vector3 &KaidanTargetPoint(const CycleManager::NpcRecord &npc) {
    const std::uint16_t index = static_cast<std::uint16_t>(npc.cycle->pathPointIndex);
    return index < npc.cycle->pathPointCount ? npc.cycle->pathPoints[static_cast<std::size_t>(npc.cycle->pathPointIndex)] : npc.cycle->pathPoints[1];
}


void ConvertAngles(CycleManager::NpcRecord &npc) {

    const Vector3 &point = KaidanTargetPoint(npc);
    npc.cycle->moveTargetPos = {npc.cycle->pos.x - point.x, npc.cycle->pos.y - point.y, npc.cycle->pos.z - point.z};
    if (!(0.05f < std::fabs(npc.cycle->moveTargetPos.x)) && !(0.05f < std::fabs(npc.cycle->moveTargetPos.z))) {
        npc.cycle->pathSegmentDistanceOrWork = 0.0f;
        npc.cycle->moveTargetPos = {};
        npc.cycle->pos = point;
        return;
    }
    if (0.001 > static_cast<double>(std::fabs(npc.cycle->moveTargetPos.x)) && 0.001 > static_cast<double>(std::fabs(npc.cycle->moveTargetPos.y)) &&
        0.001 > static_cast<double>(std::fabs(npc.cycle->moveTargetPos.z)))
        npc.cycle->pathSegmentDistanceOrWork = 0.0f;
    else
        npc.cycle->pathSegmentDistanceOrWork = Vector3::NormalizeInPlace(npc.cycle->moveTargetPos);
    if (static_cast<double>(std::fabs(npc.cycle->moveTargetPos.x)) >= 0.001 || static_cast<double>(std::fabs(npc.cycle->moveTargetPos.z)) >= 0.001)
        npc.cycle->desiredPathYaw = static_cast<std::int16_t>(atan2_fixed16(npc.cycle->moveTargetPos.x, npc.cycle->moveTargetPos.z));
    else
        npc.cycle->pathSegmentDistanceOrWork = 0.0f;
}



float cmGetRelativeMovementZFor(CycleManager::NpcRecord &npc, bool steeringActive) {
    if (steeringActive) {
        const float speed = MOTN_CalculateRootMotionSpeedZ(npc);
        npc.cycle->animMoveSpeedScale = npc.cycle->pathPointIndex == 1 ? speed * 1.25f : speed;
        if (npc.cycle->animMoveSpeedScale > 0.0f)
            return npc.cycle->pathMoveSpeed * npc.cycle->animMoveSpeedScale;
    }
    if (npc.cycle->pathPointIndex == 1) {
        npc.cycle->animMoveSpeedScale = 0.018518519f;
        return npc.cycle->pathMoveSpeed * npc.cycle->animMoveSpeedScale;
    }
    npc.cycle->animMoveSpeedScale = 0.041666668f;
    return npc.cycle->pathMoveSpeed * npc.cycle->animMoveSpeedScale;
}



bool kaidanHasPathRemaining(const CycleManager::NpcRecord &npc, const CollisionPointQueryOutputs &outputs) {
    if (npc.stairGuideEntryId != -1)
        return false;
    Vector3 local{};
    for (float distance = 0.0f;;) {
        local.z = distance > 2.4000001f ? distance - 2.4000001f : -distance;
        if (CheckIsPointIntersectWith(
                RotateAroundYAndTransformWorldCoords(local, npc.cycle->pos, static_cast<std::uint16_t>(npc.cycle->desiredPathYaw)), &outputs, kStairToken))
            return true;
        distance = distance + 0.30000001f;
        if (distance > 3.3f)
            return false;
    }
}



bool npcUpdateMoKaidan(CycleManager::NpcRecord &npc) {
    float height = 0.0f;
    std::int32_t value8 = 0;
    std::int32_t direction = 0;
    std::int32_t guideEntryId = 0;
    const CollisionPointQueryOutputs outputs{&height, &value8, &direction, &guideEntryId};
    if (kaidanHasPathRemaining(npc, outputs)) {
        const std::int32_t ascending = 0.0f > npc.cycle->pathPoints[0].y - npc.cycle->pathPoints[1].y ? 0 : 8;
        if (ascending == direction) {
            npc.stairGuideEntryId = guideEntryId;
            KDAN_AddOccupancy(guideEntryId, npc);
            if (KaidanNextCommand(npc) == CycleOpcode_FollowStairPath2D)
                KDAN_AddOccupancy((guideEntryId & 1) == 0 ? guideEntryId + 2 : guideEntryId - 2, npc);
        }
        if (!npc.cycle->pathPointIndex) {
            (void)Vector3_SubtractInto(npc.cycle->pos, npc.cycle->pathPoints[0]);
            npc.cycle->pathSegmentDistanceOrWork = 0.0f;
        }
    }
    return npc.cycle->pathPointIndex == 1;
}


bool npcKaidanIsStairBlocked(const CycleManager::NpcRecord &npc) {
    KaidanOccupancyEntry *const head = g_kaidanOccupancyHead;
    const std::int32_t guide = npc.stairGuideEntryId;
    if (!head || head->stairGuideEntryId == -1 || guide == -1)
        return false;
    std::uint32_t holder = 0;
    for (const KaidanOccupancyEntry *entry = head; entry; entry = entry->activeNext) {
        if (entry->stairGuideEntryId == guide || entry->stairGuideEntryId == guide + ((guide & 1) == 0 ? 1 : -1))
            holder = entry->charaId.name;
    }
    const std::uint32_t self = npc.cycle->moveConfig->obj_id.name;
    bool blocked = self != holder;
    if (g_playerStairGuidePairConflict) {
        for (const KaidanOccupancyEntry *entry = head; entry; entry = entry->activeNext) {
            if (entry->stairGuideEntryId == g_temporaryKaidanId + 1)
                holder = entry->charaId.name;
        }
        if (self != holder)
            return true;
        blocked = false;
    } else if (self != holder) {
        return blocked;
    }
    if (g_playerKaidanGuideActive) {
        std::int32_t pair = g_temporaryKaidanId % 2 == 0 ? 1 : -1;
        if (KaidanNextCommand(npc) == CycleOpcode_FollowStairPath2D)
            pair *= 3;
        else if (g_playerInStairPrivateVolume)
            pair = 0;
        if (guide == g_temporaryKaidanId || guide == g_temporaryKaidanId + pair)
            return true;
    }
    return blocked;
}



bool npcKaidanYieldToRyo(CycleManager::NpcRecord &npc, bool steeringActive) {
    MapControlData *const control = KaidanMapControl();
    Character *const ryo = control ? static_cast<Character *>(control->ryo) : nullptr;
    if (npc.cycle->pathPointIndex)
        return false;
    const Vector3 first = npc.cycle->pathPoints[0];
    const Vector3 second = npc.cycle->pathPoints[1];
    const Vector3 ryoPosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const std::int32_t ryoYaw = charaGetWorldYaw(ryo);
    const float toCycle = (ryoPosition.x - npc.cycle->pos.x) * (ryoPosition.x - npc.cycle->pos.x) +
        (ryoPosition.y - npc.cycle->pos.y) * (ryoPosition.y - npc.cycle->pos.y) + (ryoPosition.z - npc.cycle->pos.z) * (ryoPosition.z - npc.cycle->pos.z);
    if (static_cast<std::uint32_t>(ryoYaw - npc.cycle->desiredPathYaw - 16384) > 32768U || npc.stairProbeTravel > 0.60000002f || toCycle > 1.0f) {
        const float toFirst = (ryoPosition.x - first.x) * (ryoPosition.x - first.x) + (ryoPosition.y - first.y) * (ryoPosition.y - first.y) +
            (ryoPosition.z - first.z) * (ryoPosition.z - first.z);
        if (toFirst > 12.25f) {
            return (ryoPosition.x - second.x) * (ryoPosition.x - second.x) + (ryoPosition.y - second.y) * (ryoPosition.y - second.y) +
                (ryoPosition.z - second.z) * (ryoPosition.z - second.z) <=
                12.25f;
        }
        return !ShouldRun() ||
            (first.y - ryoPosition.y) * (first.y - ryoPosition.y) + (first.x - ryoPosition.x) * (first.x - ryoPosition.x) +
                (first.z - ryoPosition.z) * (first.z - ryoPosition.z) <
            (first.y - npc.cycle->pos.y) * (first.y - npc.cycle->pos.y) + (first.x - npc.cycle->pos.x) * (first.x - npc.cycle->pos.x) +
                (first.z - npc.cycle->pos.z) * (first.z - npc.cycle->pos.z);
    }
    const float previous = npc.cycle->pathSegmentDistanceOrWork;
    const Vector3 pushed = RotateAroundYAndTransformWorldCoords({0.0f, 0.0f, -0.029999999f}, npc.cycle->pos, static_cast<std::uint16_t>(ryoYaw));
    npc.cycle->pos.x = pushed.x;
    npc.cycle->pos.z = pushed.z;
    (void)cmGetRelativeMovementZFor(npc, steeringActive);
    ConvertAngles(npc);
    npc.stairProbeTravel = npc.cycle->pathSegmentDistanceOrWork - previous + npc.stairProbeTravel;
    return true;
}



bool npcKaidanAdvanceStairSegment(CycleManager::NpcRecord &npc, float distance) {
    const std::uint16_t index = static_cast<std::uint16_t>(npc.cycle->pathPointIndex);
    const float threshold = index <= 1u ? 0.2f : distance;
    if (npc.cycle->pathSegmentDistanceOrWork > threshold) {
        ConvertAngles(npc);
        npc.cycle->pos.x = npc.cycle->pos.x - distance * npc.cycle->moveTargetPos.x;
        npc.cycle->pos.y = npc.cycle->pos.y - distance * npc.cycle->moveTargetPos.y;
        npc.cycle->pos.z = npc.cycle->pos.z - distance * npc.cycle->moveTargetPos.z;
        return true;
    }
    const std::int32_t guide = npc.stairGuideEntryId;
    const std::uint32_t nextCommand = KaidanNextCommand(npc);
    npc.cycle->pathPointIndex = static_cast<std::int16_t>(index + 1);
    if (static_cast<std::uint16_t>(index + 1) < npc.cycle->pathPointCount) {
        npc.cycle->pos = npc.cycle->pathPoints[0];
        ConvertAngles(npc);
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        return true;
    }
    const std::int32_t rawGuide = g_playerRawStairGuideEntryId;
    npc.cycle->pos = npc.cycle->pathPoints[1];
    npc.cycle->pathSegmentDistanceOrWork = 0.0f;
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
    if (rawGuide != -1 && (guide == 25 || guide == 43) && guide == rawGuide + (rawGuide % 2 == 0 ? 1 : -1)) {
        npc.cycle->pathPointIndex = 2;
        return true;
    }
    if (nextCommand != CycleOpcode_FollowStairPath2D && rawGuide == guide) {
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
        if (npc.cycle->pathPointIndex == 2) {
            npc.cycle->pathPoints[1] =
                RotateAroundYAndTransformWorldCoords({0.0f, 0.0f, -0.5f}, npc.cycle->pathPoints[1], static_cast<std::uint16_t>(npc.cycle->desiredPathYaw));
            npc.cycle->pathPointIndex = 3;
            npc.cycle->pathSegmentDistanceOrWork = 1.0f;
            return true;
        }
    }
    npc.cycle->pathMoveSpeed = npc.cycle->moveConfig->motionRadiusTimingScalar;
    if (nextCommand != CycleOpcode_FollowStairPath2D && guide != 44)
        KDAN_RemoveOccupancyByCharacterId(npc.cycle->moveConfig->obj_id.name);
    npc.stairGuideEntryId = -1;
    return false;
}

} // namespace



bool npcKaidanUpdateStairWalk(CycleManager::NpcRecord &npc, bool steeringActive) {
    if (CycleManager::CurrentAreaId() != npc.cycle->areaID.name)
        return false;
    const std::int16_t opcode = static_cast<std::int16_t>(npc.cycle->currentOpcode);
    if (opcode == CycleOpcode_RemoveCharacter || opcode >= 0x3e)
        return false;
    if (steeringActive && charaHasBlockingTPMRequest(npc.cycle->obj))
        return true;
    if (npc.stairStage == 0) {
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        npc.stairStage = 1;
        return true;
    }
    if (npc.stairStage == 1) {
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
        npc.stairStage = 2;
        (void)cmGetRelativeMovementZFor(npc, steeringActive);
        ConvertAngles(npc);
        return true;
    }
    if (npcKaidanYieldToRyo(npc, steeringActive) || (KDAN_PruneStaleOccupancyEntries(), npcKaidanIsStairBlocked(npc))) {
        if (npc.cycle->activeAnimId != npc.cycle->moveConfig->motionParamIds[0])
            npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[0];
        return true;
    }
    npc.cycle->flags &= ~1U;
    if (npc.cycle->pathPointIndex)
        npc.cycle->flags &= npc.cycle->pathPointIndex == 1 ? ~0x1EU : ~4U;
    if (npcUpdateMoKaidan(npc)) {
        if (static_cast<std::uint32_t>(npc.cycle->pathPointIndex) < npc.cycle->pathPointCount)
            npc.cycle->activeAnimId = static_cast<std::uint16_t>(npc.stairPackedAnimIds);
        if (npc.stairGuideEntryId == -1) {
            KDAN_RemoveOccupancyByCharacterId(npc.cycle->moveConfig->obj_id.name);
            return false;
        }
    } else {
        npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
    }
    return npcKaidanAdvanceStairSegment(npc, cmGetRelativeMovementZFor(npc, steeringActive));
}


void npcKaidanSetStairMotion(CycleManager::NpcRecord &npc) {
    MotionInformationWork *const motion = npc.cycle->mot;
    if (!motion)
        return;
    if (npc.cycle->obj) {
        if (MotionManagerWork *const manager = MOTM_GetWork(npc.cycle->obj))
            manager->flags &= ~1U;
    }
    motion->playbackRate = npc.cycle->pathMoveSpeed;
    if (motion->currentAnimId != npc.cycle->activeAnimId && npc.cycle->obj) {
        motion->activeBasePoseAnimIndex = 7;
        freeTalkConv_SetMoveAndFacingConstraint(*npc.cycle->obj, npc.cycle->pos, npc.cycle->desiredPathYaw, npc.cycle->activeAnimId, 0.0f,
            npc.cycle->moveConfig->motionRadiusTimingScalar, 0.0f, false);
    }
}
namespace CycleManager {




bool InitStairPath2D(CycleManager::NpcRecord &npc, std::uint32_t recordOffset) {
    const std::vector<std::uint8_t> &bytes = g_cycleManagerRuntime.bytes;
    if (static_cast<std::size_t>(recordOffset) + 20U > bytes.size())
        return false;
    std::uint32_t packedAnim = 0;
    std::uint16_t count = 0;
    std::uint32_t offset = 0;
    float work = 0.0f;
    std::memcpy(&packedAnim, bytes.data() + recordOffset + 4U, sizeof(packedAnim));
    std::memcpy(&count, bytes.data() + recordOffset + 8U, sizeof(count));
    std::memcpy(&offset, bytes.data() + recordOffset + 12U, sizeof(offset));
    std::memcpy(&work, bytes.data() + recordOffset + 16U, sizeof(work));
    npc.cycle->pathPoints = reinterpret_cast<Vector3 *>(CycleManager::FilePointer(offset));
    npc.cycle->pathPointCount = count;
    npc.cycle->pathPointIndex = 0;
    npc.stairPackedAnimIds = packedAnim;
    npc.stairGuideEntryId = -1;
    npc.stairCommandWork = work;
    npc.stairStage = 0;
    npc.stairProbeTravel = 0.0f;
    npc.cycle->pathSegmentProgress = 0.0f;
    const float distance = Vector3_SubtractInto(npc.cycle->pos, npc.cycle->pathPoints[0]);
    if (npc.cycle->moveConfig->obj_id.name == SHEN_NAME(ONO_)) {
        if (npc.stairPackedAnimIds == 0x8099U)
            npc.stairPackedAnimIds = 0x8194U;
        if (npc.stairPackedAnimIds == 0x8091U)
            npc.stairPackedAnimIds = 0x818FU;
    }
    npc.cycle->currentOpcode = CycleOpcode_FollowStairPath2D;
    if (0.1f > distance) {
        npc.cycle->pos = npc.cycle->pathPoints[0];
        npc.cycle->pathPointIndex = 1;
    } else if (distance > 50.0f) {
        npc.cycle->currentOpcode = CycleOpcode_AdvanceScript;
        return false;
    } else {
        npc.cycle->pathPointIndex = 0;
    }
    npc.cycle->activeAnimId = npc.cycle->moveConfig->motionParamIds[8];
    npc.cycle->pathMoveSpeed = 1.0f;
    return true;
}
} // namespace CycleManager


} // namespace shenmue::s1
