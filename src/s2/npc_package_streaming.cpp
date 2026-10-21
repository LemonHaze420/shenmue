#include "s2/npc_package.hpp"

#include "s2/archive_data.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/door.hpp"
#include "s2/face.hpp"
#include "s2/field_collision.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/hand.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion_pose.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_area_route.hpp"
#include "s2/player.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "s2/npc_package_internal.hpp"

namespace shenmue::s2 {
using namespace npc_package_detail;

namespace npc_package_detail {

std::int32_t FindNativeNpcHumanPackageIndex(shen_name name) {
    if (!npcHumanIndex)
        return -1;
    const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
    for (std::int32_t index = 0; index < npcHumanCount; ++index)
        if (names[index] == name.name)
            return index;
    return -1;
}

bool ReleaseNativeNpcHumanPackage(shen_name name) {
    const std::int32_t index = FindNativeNpcHumanPackageIndex(name);
    if (index < 0)
        return false;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    if (slot.state == NpcHumanPakState::Empty || slot.state == NpcHumanPakState::Requested) {
        slot.state = NpcHumanPakState::Empty;
        return false;
    }
    if (slot.state != NpcHumanPakState::Ready && slot.state != NpcHumanPakState::Attached)
        return false;
    DestroyPakFileAFS(slot.pak);
    slot.pak = nullptr;
    slot.state = NpcHumanPakState::Empty;
    return true;
}

bool ReleaseReadyNpcHumanPackage(shen_name name) {
    const std::int32_t index = FindNativeNpcHumanPackageIndex(name);
    if (index < 0 || npcHumanSlots[index].state != NpcHumanPakState::Ready)
        return false;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    DestroyPakFileAFS(slot.pak);
    slot.pak = nullptr;
    slot.state = NpcHumanPakState::Empty;
    return true;
}

void ReleaseNpcTransitionHumanPackages() {
    for (NpcRouteResetGroupHeader *group = npcRouteResetGroup; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name != npcLoadedArea.name)
                continue;
            ReleaseNativeNpcHumanPackage(entry->character);
            entry->nodeOffset = 0;
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        if (!slot.character.name)
            continue;
        ReleaseNativeNpcHumanPackage(slot.character);
        slot.node = nullptr;
    }
}

void ReleaseReadyNpcTransitionHumanPackages() {
    for (NpcRouteResetGroupHeader *group = npcRouteResetGroup; group->area.name;) {
        NpcRouteResetEntry *entry = reinterpret_cast<NpcRouteResetEntry *>(group + 1);
        for (; entry->character.name; ++entry) {
            if (group->area.name == npcLoadedArea.name && ReleaseReadyNpcHumanPackage(entry->character))
                entry->nodeOffset = 0;
        }
        group = reinterpret_cast<NpcRouteResetGroupHeader *>(reinterpret_cast<std::byte *>(entry) + 4);
    }
    for (NpcTransitionRuntimeSlot &slot : npcTransitionRuntimeSlots) {
        if (slot.character.name && ReleaseReadyNpcHumanPackage(slot.character))
            slot.node = nullptr;
    }
}

bool IsNpcGloballyBlocked(const NpcRouteLiveEntry &entry) {
    if (entry.stateContext.flags & 1U) {
        if ((entry.stateContext.flags & 0x300U) || (entry.stateContext.routeFlags & 0x10U))
            return false;
        if (!(entry.flags & 0x4000000U))
            return true;
    }
    return (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x10U) != 0;
}

std::int32_t LoadNativeNpcHuman(shen_name packageName, shen_name characterName, bool fromHumans) {
    if (!npcHumanSlots)
        return 0;
    const std::int32_t index = FindNativeNpcHumanPackageIndex(packageName);
    if (index < 0)
        return -1;
    NpcHumanPakSlot &slot = npcHumanSlots[index];
    if (slot.state == NpcHumanPakState::Attached)
        return 1;
    if (slot.state == NpcHumanPakState::Ready) {
        const std::int32_t readiness = GetPakReadiness(slot.pak);
        if (readiness < 0) {
            ReleaseNativeNpcHumanPackage(packageName);
            return -6;
        }
        if (!readiness)
            return 0;
        MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        Character *const character = FindScenarioCharacter(control ? control->scenarioTask : nullptr, characterName);
        if (!character) {
            ReleaseNativeNpcHumanPackage(packageName);
            return -5;
        }
        char filename[9] = {};
        std::memcpy(filename, &characterName.name, 4);
        std::memcpy(filename + 4, ".bin", 5);
        AttachScenarioFreeTalkCharacter(character, FileLoaderView(slot.pak, filename, HAND_BANK_EXTENSION));
        slot.state = NpcHumanPakState::Attached;
        return 1;
    }
    if (slot.state == NpcHumanPakState::Empty)
        slot.state = NpcHumanPakState::Requested;
    if (slot.state != NpcHumanPakState::Requested)
        return -8;
    if (PakLoadWait())
        return 0;
    const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
    const std::int32_t requested = static_cast<std::int32_t>(names[index + npcHumanCount] + 3096U);
    if (!requested) {
        slot.state = NpcHumanPakState::Empty;
        return -2;
    }
    const std::uint64_t size = static_cast<std::uint64_t>(static_cast<std::int64_t>(requested));
    std::uint64_t totalFree;
    std::uint64_t fragmentation;
    HLib::TryGetFreeHeapStats(&totalFree, &fragmentation);
    if (size > totalFree) {
        slot.state = NpcHumanPakState::Empty;
        return -4;
    }
    if (!npcHumansInUse || !npcHasCachedPak) {
        HLib::AnalyzeFreeHeap(&totalFree, &npcLargestDynamicBlock, &fragmentation);
        npcLargestDynamicBlock = npcLargestDynamicBlock > 65536U ? npcLargestDynamicBlock - 65536U : 0;
        if (totalFree < 3862528U)
            npcLargestDynamicBlock = 0;
        HLib::AnalyzeFreeList(&totalFree, &npcLargestSceneBlock, &fragmentation);
        npcLargestSceneBlock = npcLargestSceneBlock > 65536U ? npcLargestSceneBlock - 65536U : 0;
        if (totalFree < 1572864U)
            npcLargestSceneBlock = 0;
        npcHasCachedPak = true;
    }
    if (size > npcLargestDynamicBlock || size > npcLargestSceneBlock) {
        slot.state = NpcHumanPakState::Empty;
        return -3;
    }
    slot.pak = RequestPakFromAfs(npcHumansAfs, index * 2, static_cast<std::int16_t>(index * 2 + 1), fromHumans ? 1U : 0U);
    if (slot.pak) {
        slot.state = NpcHumanPakState::Ready;
        npcHasCachedPak = false;
    }
    return 0;
}
void NpcRegistryTask_Callback(void *) {
    if (HLib::GetCurrentTask() != npcRegistryTask.current) {
        HLib::DestroyCurrentTask();
        return;
    }
    npcHumansInUse = 1;
    npcHasCachedPak = false;
    TickNpcRouteLiveEntries();
    DrawNpcTransitions();
    npcHumansInUse = 0;
}

std::uint16_t ReadU16(const std::byte *source) {
    std::uint16_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

std::uint32_t ReadU32(const std::byte *source) {
    std::uint32_t value{};
    std::memcpy(&value, source, sizeof(value));
    return value;
}

void ReleaseNativeNpcHumanPackages() {
    if (npcHumanIndex && npcHumanSlots) {
        const std::uint32_t *const names = reinterpret_cast<const std::uint32_t *>(npcHumanIndex + 4);
        for (std::int32_t index = 0; index < npcHumanCount; ++index)
            ReleaseNativeNpcHumanPackage(shen_name{names[index]});
    }
    npcHasCachedPak = false;
}

void FreeNativeNpcHumans() {
    ReleaseNativeNpcHumanPackages();
    HLib::FreeTaskData(npcHumanSlots);
    HLib::FreeTaskData(npcHumanIndex);
    npcHumanSlots = nullptr;
    npcHumanIndex = nullptr;
    npcHumanCount = 0;
    npcHasCachedPak = false;
}


bool AllocateNPCH() {
    char directory[32];
    std::snprintf(directory, sizeof(directory), "scene/%02d/npc", g_loadedActNumber);
    std::uint32_t indexSize = 0;
    void *indexData = LoadLegacySceneFile(directory, "HUMANS.idx", NPC_HUMAN_INDEX_MEMORY, &indexSize);
    if (!indexData)
        return false;
    npcHumanIndex = static_cast<std::byte *>(indexData);
    if (indexSize < 4) {
        FreeNativeNpcHumans();
        return false;
    }
    npcHumanCount = static_cast<std::int32_t>(ReadU32(npcHumanIndex));
    if (npcHumanCount <= 0 || 4ULL + static_cast<std::uint64_t>(npcHumanCount) * 8U > indexSize) {
        FreeNativeNpcHumans();
        return false;
    }
    npcHumanSlots = static_cast<NpcHumanPakSlot *>(HLib::AllocateTaskData(sizeof(NpcHumanPakSlot) * npcHumanCount, NPC_HUMAN_SLOT_MEMORY));
    if (!npcHumanSlots) {
        FreeNativeNpcHumans();
        return false;
    }
    HLib::SetTaskDataMotionFlag(npcHumanIndex);
    HLib::SetTaskDataMotionFlag(npcHumanSlots);
    for (std::int32_t index = 0; index < npcHumanCount; ++index) {
        npcHumanSlots[index].state = NpcHumanPakState::Empty;
        npcHumanSlots[index].pak = nullptr;
    }
    return true;
}


void npcSetupHumans() {
    if (!npcHumanSlots)
        return;
    char directory[32];
    std::snprintf(directory, sizeof(directory), "scene/%02d/npc", g_actNumber);
    npcHumansAfs = MountPakAfs(directory, "HUMANS.afs");
    SetupPakSpace(1024);
    for (std::int32_t index = 0; index < npcHumanCount; ++index) {
        npcHumanSlots[index].state = NpcHumanPakState::Empty;
        npcHumanSlots[index].pak = nullptr;
    }
}

std::int16_t FindNpcHumanPackageIndex(shen_name name) {
    for (std::size_t index = 0; index < npcSchedule.humanPackages.size(); ++index) {
        if (npcSchedule.humanPackages[index].name != name.name)
            continue;
        if (index > static_cast<std::size_t>((std::numeric_limits<std::int16_t>::max)())) {
            return -1;
        }
        return static_cast<std::int16_t>(index);
    }
    return -1;
}

void ReleaseNpcRouteStream(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, bool clearPackageAlias) {
    ReleaseNativeNpcHumanPackage(ReadNpcLiveField<shen_name>(entry, npcLoadHumanNameOffset));
    npcActiveStreamCost -= entry.streamCost;
    if (entry.flags & 0x10U) {
        entry.liveState = entry.previousLiveState;
        entry.flags &= ~0x00004010U;
        entry.specialMotionPhase = 0;
    }
    entry.flags &= ~0xa2000017U;
    entry.node = nullptr;
    entry.motionWork = nullptr;
    if (entry.flags & 0x400U)
        static_cast<NpcTimedMotionSlot *>(entry.timedMotionHandle)->character = nullptr;
    npcSlotFlags[entry.poolIndex] &= ~1U;
    entry.streamRouteChangeKind = 0;
    entry.field335 = 0;
    if (entry.stateContext.flags & 0x101U) {
        entry.stateContext.flags &= 0xff9efbffU;
        entry.stateContext.motionWork = nullptr;
    }
    entry.loadHumanResult = 8;
    runtime.character = nullptr;
    runtime.packageAlias = {};
    runtime.streamState = NpcHumanStreamState::Inactive;
    if (clearPackageAlias) {
        const shen_name noAlias{};
        WriteNpcLiveField(entry, npcLoadHumanNameOffset, noAlias);
    }
}

void BeginNpcRouteStreamOut(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime) {
    if (runtime.streamState == NpcHumanStreamState::Inactive && !runtime.character && !entry.node) {
        return;
    }
    entry.flags &= ~NpcRouteStreamLive;
    entry.flags |= NpcRouteStreamFadeOut;
    runtime.streamState = NpcHumanStreamState::ReleasePending;
}

bool PublishExistingNpcCharacter(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime, Character *character) {
    if (!character)
        return false;
    MotionWork *const motionWork = FindMotionWork(character);
    if (!motionWork)
        return false;
    const std::uint32_t streamFlags = entry.flags;
    entry.node = character;
    entry.motionWork = motionWork;
    runtime.character = character;
    runtime.streamState = NpcHumanStreamState::Live;
    entry.flags &= ~(NpcRouteStreamRequested | NpcRouteStreamFadeOut | NpcRouteStreamStartHidden);
    entry.flags |= NpcRouteStreamLive;
    npcSlotFlags[entry.poolIndex] |= 1U;
    entry.streamRouteChangeKind = 0;
    entry.streamRadius = 0.30000001f;
    InitializeStreamedNpcRoute(entry);
    // HandleCharaStates -> UpdateStreamedNpcMotion publishes the MTWK update bit;
    // StreamNpcCharacters then applies its two explicit hidden-entry gates.
    motionWork->runtime.callbackFlags |= 1U;
    SetFaceFlagsForChara_0(character, (streamFlags & (NpcRouteAbstractActive | NpcRouteStreamStartHidden)) != NpcRouteStreamStartHidden);
    if (entry.name.name == SHEN_NAME(SIN_)) {
        SetLowQualityHands(character);
        SetCharacterHandRegionEnabled(character, 0, true, 1);
        SetCharacterHandRegionEnabled(character, 1, true, 1);
        SetCharacterType(character, CharacterTypeStencilShadow);
    }
    const bool startHidden = (streamFlags & (NpcRouteAbstractActive | NpcRouteStreamForceHidden)) == 0 && (streamFlags & NpcRouteStreamStartHidden) != 0;
    if ((streamFlags & NpcRouteStreamForceHidden) != 0 || startHidden) {
        character->renderFlags &= ~CRF_All;
        if (CharacterControlModel *const control = FindCharacterControlModel(character)) {
            control->flags &= ~CharacterControlVoxelSweep;
        }
        if (startHidden)
            entry.flags |= NpcRouteStreamLoadedHidden;
    }
    const std::int8_t loaded = 1;
    WriteNpcLiveField(entry, npcLoadHumanResultOffset, loaded);
    BindNpcRouteCharacter(entry.name, character);
    return true;
}

bool EnsureNpcRouteStreamReady(NpcRouteLiveEntry &entry, NpcRouteRuntimeRecord &runtime) {
    const shen_name alias = ReadNpcLiveField<shen_name>(entry, npcLoadHumanNameOffset);
    runtime.packageAlias = alias;
    const std::int32_t result = LoadNativeNpcHuman(alias, entry.name, (entry.flags & NpcRouteStreamUseHumansAfs) != 0);
    npcLastHumanLoadResult = result;
    WriteNpcLiveField(entry, npcLoadHumanResultOffset, static_cast<std::int8_t>(result));
    if (!result) {
        runtime.streamState = NpcHumanStreamState::Requested;
        return false;
    }
    entry.streamRouteChangeKind = 0;
    if (result < 0) {
        runtime.streamState = NpcHumanStreamState::Missing;
        return false;
    }
    return PublishExistingNpcCharacter(entry, runtime, FindCharacterByScenarioName(entry.name));
}

float NpcDistanceXZ(const Vector3 &first, const Vector3 &second) {
    const float x = first.x - second.x;
    const float z = first.z - second.z;
    return std::sqrt(x * x + z * z);
}

bool IsInsideNpcNoSpawnRadius(const Vector3 &position, float radius) {
    std::int32_t remaining = npcRegistryTask.areaCount;
    for (const NpcNoSpawnArea &area : npcRegistryTask.areas) {
        if (!remaining || !area.name.name)
            break;
        if (area.radius + radius > NpcDistanceXZ(area.center, position))
            return true;
        --remaining;
    }
    return false;
}

std::int32_t AdjustedNpcStreamTarget() {
    if (npcPoolMinimum != -1 && npcPoolMinimum <= npcPoolTarget)
        return std::max(npcPoolMinimum, npcPoolTarget - static_cast<std::int32_t>(static_cast<float>(npcPoolTarget) * npcCrowdDensityScale * 0.5f));
    return npcPoolTarget;
}

bool RequestNpcStreamLoad(NpcRouteLiveEntry &entry) {
    NpcRouteRuntimeRecord &runtime = *GetNpcRuntimeRecord(entry);
    npcLastHumanLoadResult = 0;
    if (!entry.streamRouteChangeKind && (entry.flags & 2U)) {
        npcLastHumanLoadResult = 1;
        return true;
    }
    if (!entry.streamRouteChangeKind && (entry.flags & 4U)) {
        entry.streamRouteChangeKind = 0;
        entry.field335 = 0;
        entry.flags = (entry.flags & ~0x1006U) | 2U;
        runtime.streamState = NpcHumanStreamState::Live;
        if (!(entry.flags & 0x80000000U))
            npcSetAlphaOnOff(entry.node, true);
        npcLastHumanLoadResult = 1;
        return true;
    }
    if (!entry.streamRouteChangeKind) {
        if (!npcNewLoadState && (entry.flags & 0x1000U)) {
            entry.flags &= ~1U;
            return true;
        }
        if (AdjustedNpcStreamTarget() <= npcActiveStreamCost && !(entry.flags & 8U))
            return true;
        entry.flags |= 1U;
        entry.field342 = 0;
        npcActiveStreamCost += entry.streamCost;
        entry.streamRouteChangeKind = 1;
    }
    if (entry.streamRouteChangeKind != 1)
        return false;
    if (EnsureNpcRouteStreamReady(entry, runtime))
        return true;
    if (runtime.streamState == NpcHumanStreamState::Missing) {
        entry.streamRouteChangeKind = 0;
        entry.flags &= ~1U;
        npcActiveStreamCost -= entry.streamCost;
        npcWaitSawLoadFailure = true;
    }
    return false;
}

std::int32_t MeasureNpcStreamCost(NpcRouteLiveEntry &entry, bool reload) {
    if (entry.flags & (reload ? 3U : 7U))
        return entry.streamCost;
    if ((entry.flags & 0x20800028U) || (!npcColdResetMode && IsNpcGloballyBlocked(entry)))
        return 0;
    Vector3 floor = entry.localPosition;
    if (entry.flags & 0x2000U)
        entry.localPosition.y = entry.forcedY;
    else if (!FindNpcIntersectionInScene(floor, entry.packedMoveFlags) && !(entry.flags & 0x1000000U))
        return 0;
    if (reload) {
        if ((entry.stateContext.flags & 1U) && IsInsideNpcNoSpawnRadius(entry.localPosition, 0.40000001f))
            return 0;
        std::int8_t countdown = static_cast<std::int8_t>(entry.streamingCountdown);
        if (countdown) {
            --countdown;
            entry.streamingCountdown = static_cast<std::uint16_t>((entry.streamingCountdown & 0xff00U) | static_cast<std::uint8_t>(countdown));
            if (countdown > 0)
                return 0;
            entry.streamingCountdown &= 0xff00U;
        }
        entry.flags |= 0x1000U;
        RequestNpcStreamLoad(entry);
    }
    return entry.streamCost;
}

void RankNpcStreamEntry(NpcRouteLiveEntry &entry, std::uint8_t &slotFlags, const Vector3 &cameraPosition) {
    Vector3 position;
    if (std::abs(entry.localPosition.y) >= 0.00000011920929f)
        position = TransformNpcPoint(entry.localPosition);
    else
        position = {cameraPosition.x - entry.localPosition.x, cameraPosition.y - entry.localPosition.y, cameraPosition.z - entry.localPosition.z};
    position.z += 4.5f;
    const float first = std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    position.z += npcStreamThrottleWithBias;
    float distance = first + std::sqrt(position.x * position.x + position.y * position.y + position.z * position.z);
    if (distance >= 50.0f)
        slotFlags &= ~2U;
    else
        slotFlags |= 2U;
    if (entry.streamDistanceBias != 0.0f)
        distance = std::fmax(0.0f, distance - entry.streamDistanceBias);
    const float key = (entry.flags & 0x200U) ? 0.0f : distance + 0.0099999998f;
    NpcRouteLiveEntry *insertion = &entry;
    if (entry.previous) {
        if (entry.previous->streamSortKey <= key && entry.next) {
            for (NpcRouteLiveEntry *cursor = entry.next; cursor && cursor->streamSortKey < key; cursor = cursor->next)
                insertion = cursor;
        } else {
            for (NpcRouteLiveEntry *cursor = entry.previous; cursor && key < cursor->streamSortKey; cursor = cursor->previous)
                insertion = cursor;
        }
    } else {
        for (NpcRouteLiveEntry *cursor = entry.next; cursor && cursor->streamSortKey < key; cursor = cursor->next)
            insertion = cursor;
    }
    entry.streamSortKey = key;
    if (insertion == &entry || insertion->next == &entry)
        return;
    if (entry.previous)
        entry.previous->next = entry.next;
    else
        npcCrowdHead = entry.next;
    if (entry.next)
        entry.next->previous = entry.previous;
    entry.next = insertion->next;
    entry.previous = insertion;
    if (entry.next)
        entry.next->previous = &entry;
    insertion->next = &entry;
}

void UpdateNpcStreamFade(NpcRouteLiveEntry &entry) {
    if (!IsOfThisVisibilityType(entry.node, 5))
        entry.field335 = 15;
    if (!entry.field335) {
        npcSetAlphaOnOff(entry.node, false);
        ++entry.field335;
    } else if (entry.field335 == 15) {
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
    } else if (std::abs(entry.node->alpha) >= 0.00000011920929f) {
        npcSetAlphaOnOff(entry.node, false);
    } else
        entry.field335 = 15;
}

void EvaluateNpcMotionAttachments(NpcRouteLiveEntry &entry) {
    for (NpcMotionAttachment &attachment : entry.motionAttachments) {
        if (!attachment.motion) {
            attachment.flags = 0;
            continue;
        }
        attachment.frame = -12.0f;
        for (std::uint32_t region = 1; region < 5; ++region) {
            if (!(attachment.flags & (1U << region)) || !SetMotionRegionFlag(entry.motionWork, 72, 1, static_cast<MotionRegion>(region)) ||
                GetMotionAnimationId(entry.motionWork, static_cast<MotionRegion>(region)) != attachment.motion)
                continue;
            attachment.frame = GetMotionFrame(entry.motionWork, static_cast<MotionRegion>(region));
            if (SetMotionRegionFlag(entry.motionWork, 68, 1, static_cast<MotionRegion>(region)))
                attachment.frame -= 12.0f;
            break;
        }
    }
    bool completed[3]{};
    for (std::uint32_t region = 1; region < 5; ++region) {
        std::int32_t selected = -1;
        for (std::int32_t slot = 0; slot < 3; ++slot) {
            const NpcMotionAttachment &attachment = entry.motionAttachments[slot];
            if (attachment.motion && !(attachment.flags & 0x40U) && (attachment.flags & (1U << region))) {
                selected = slot;
                break;
            }
        }
        if (selected == -1) {
            RequestMotionRegionDetach(entry.motionWork, static_cast<MotionRegion>(region));
            continue;
        }
        const NpcMotionAttachment &attachment = entry.motionAttachments[selected];
        std::uint16_t active = GetMotionAnimationId(entry.motionWork, static_cast<MotionRegion>(region));
        if (!SetMotionRegionFlag(entry.motionWork, 72, 1, static_cast<MotionRegion>(region)))
            active = 0;
        const std::uint16_t queued = GetQueuedMotionAnimationId(entry.motionWork, static_cast<MotionRegion>(region));
        if ((!active && !queued) || (active && active != attachment.motion) || (queued && queued != attachment.motion)) {
            const MotnSequence *const sequence = FindLoadedMotionSequence(attachment.motion);
            const float last = sequence ? static_cast<float>(sequence->frameCount) - 1.0f : -1.0f;
            if (last < 0.0f)
                continue;
            float frame = (attachment.frame + 12.0f) * GetMotionSpeed(entry.motionWork, static_cast<MotionRegion>(region));
            if (attachment.flags & 0x20U) {
                while (frame > last)
                    frame -= last;
            } else
                frame = std::fmin(last, frame);
            mtwkRequestMotion(entry.motionWork, attachment.motion, static_cast<MotionRegion>(region));
            SetMotionRegionEvalTime(entry.motionWork, frame, static_cast<MotionRegion>(region));
            mtwkSetRequestedRootMode(entry.motionWork, 1);
            SetMotionFrameTime(entry.motionWork, 12, static_cast<MotionRegion>(region));
        } else if (!(attachment.flags & 0x20U) && SetMotionRegionFlag(entry.motionWork, 65, 1, static_cast<MotionRegion>(region))) {
            RequestMotionRegionDetach(entry.motionWork, static_cast<MotionRegion>(region));
            completed[selected] = true;
        }
    }
    for (std::int32_t slot = 0; slot < 3; ++slot)
        if (completed[slot])
            ClearNpcMotionAttachment(entry, slot);
}

void UpdateNpcMotionAttachments(NpcRouteLiveEntry &entry) {
    if (entry.controlFlags & 0x20U)
        return;
    if (!entry.node) {
        entry.idleCounter = 0;
        return;
    }
    if (mtwkSetExtraFlag(entry.motionWork, 60, 1))
        return;
    NpcMotionAttachment &first = entry.motionAttachments[0];
    if ((entry.controlFlags & 0x10U) || (entry.flags & 0x100000U))
        first.flags |= 0x40U;
    if (npcCrowdDensityScale >= 0.15f) {
        const std::uint16_t rain = RemapNpcMotionForRig(GetCharacterBoneId(entry.node), 0xe353);
        if ((first.motion && first.motion != rain) || (!first.motion && !(entry.flags & 0x10U)))
            SetNpcMotionAttachment(entry, rain, 40, 0);
    } else if (first.motion && !(entry.flags & 0x10U))
        ClearNpcMotionAttachment(entry, 0);
    if (entry.controlFlags & 3U) {
        RequestMotionRegionDetach(entry.motionWork, MotionRegion::LowerBody);
        if (entry.controlFlags & 1U) {
            ClearNpcMotionAttachment(entry, 0);
            ClearNpcMotionAttachment(entry, 1);
            ClearNpcMotionAttachment(entry, 2);
        }
    } else {
        EvaluateNpcMotionAttachments(entry);
        if ((entry.controlFlags & 4U) && !entry.motionAttachments[2].motion) {
            if (++entry.idleCounter <= 90)
                return;
            if (IncrementSharedPrngFloat() * 3.0f <= 1.0f && !SetNpcMotionAttachment(entry, 32861, 20, 2))
                ClearNpcMotionAttachment(entry, 2);
        }
    }
    entry.idleCounter = 0;
}

// @verified - p
bool CheckNpcCachedVisibility(NpcRouteLiveEntry &entry, const Vector3 &position, float radius) {
    if (entry.node->field99)
        return (entry.node->field99 & 1U) != 0;
    if (!camCtrlIsInsideHorizontalDepthRect(position, IsCharacterMirroredForDraw(entry.node) ? 5.0f : 0.60000002f))
        return false;
    const bool visible = camCtrlCheckPosition(position, radius);
    entry.node->field99 = visible ? 3 : 2;
    return visible;
}

void UpdateNpcFrameCulling(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    SetMotionWorkCulled(entry.motionWork, false);
    if (npcUsePositionCullCallback) {
        if ((entry.flags & 0x1058U) || (entry.stateContext.flags & 1U) || (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x20U))
            return;
        if (CheckNpcCachedVisibility(entry, GetCharacterWorldPosition(entry.node), 0.60000002f))
            return;
        SetMotionWorkCulled(entry.motionWork, true);
        entry.node->renderFlags &= ~CRF_InRange;
        entry.node->position = {};
        return;
    }
    if ((entry.flags & 0x10U) || (entry.routeMotionState != 16 && !(entry.flags & 0x8000U)))
        return;
    Vector3 position = GetCharacterWorldPosition(entry.node);
    if (!CheckNpcCachedVisibility(entry, position, 0.40000001f)) {
        SetMotionWorkCulled(entry.motionWork, true);
        entry.node->renderFlags &= ~CRF_InRange;
        entry.node->position = {};
    } else {
        if (!GetMotionAnimationId(entry.motionWork, MotionRegion::LowerBody))
            return;
        position = TransformNpcPoint(position);
        if (position.x * position.x + position.y * position.y + position.z * position.z <= 729.0f)
            return;
        SetMotionWorkCulled(entry.motionWork, true);
        entry.node->renderFlags |= CRF_InRange;
    }
    const std::uint16_t counter = static_cast<std::uint16_t>((entry.field2d8 >> 16) + 1);
    entry.field2d8 = (entry.field2d8 & ~0xffff0000ULL) | (static_cast<std::uint64_t>(counter) << 16);
}
} // namespace npc_package_detail

std::int32_t SetNpcStreamLifecycleState(NpcRouteLiveEntry &entry, std::int32_t mode) {
    if (entry.loadHumanResult < 0)
        entry.loadHumanResult = 8;
    if (entry.flags & 8U)
        return 2;
    if (entry.flags & NpcRouteStreamPinned)
        return 3;
    if (!(entry.flags & 7U))
        return 4;
    FlushQueuedLegacyDrawPasses();
    if (!mode || (mode == 2 && (entry.flags & 1U))) {
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
        return 1;
    }
    if (mode != 1)
        return 1;
    const std::int32_t pending = entry.flags & 1U;
    if (pending)
        ReleaseNpcRouteStream(entry, *GetNpcRuntimeRecord(entry), false);
    if ((entry.flags & 0x1aU) == 2) {
        entry.field335 = 0;
        entry.flags = (entry.flags & ~6U) | 4U;
    }
    if (entry.flags & 4U)
        UpdateNpcStreamFade(entry);
    return pending;
}

bool RequestNpcStreamOut(NpcRouteLiveEntry &entry, std::int32_t type) {
    if (!(entry.flags & 2U) || (entry.flags & 0x18U))
        return false;
    entry.flags = (entry.flags & ~6U) | 4U;
    if (type == 1)
        entry.flags |= 0x20000000U;
    else if (type == 2)
        entry.flags |= 0x22000000U;
    entry.field335 = 0;
    return true;
}
bool CanRevealNpcStream(const NpcRouteLiveEntry &entry) {
    if (entry.streamRadius + npcPlayerRadius > NpcDistanceXZ(npcPlayerCenter, entry.streamCenter))
        return false;
    if (entry.streamCost == 1)
        return true;
    if (IsInsideNpcNoSpawnRadius(entry.streamCenter, entry.streamRadius))
        return false;
    std::int32_t cost = npcActiveStreamCost - entry.streamCost;
    for (const NpcRouteLiveEntry *other = npcCrowdHead; other && cost; other = other->next) {
        if (other == &entry || !(other->flags & 7U))
            continue;
        cost -= other->streamCost;
        if (!(other->flags & 0x80000041U) && other->streamCost != 1 && (!(other->flags & 0x8000000U) || !(entry.flags & 0x8000000U)) &&
            std::abs(entry.localPosition.y - other->localPosition.y) <= 1.8f &&
            other->streamRadius + entry.streamRadius > NpcDistanceXZ(other->streamCenter, entry.streamCenter))
            return false;
    }
    return true;
}

bool HasNpcRuntimeEntry(shen_name character) {
    return FindActiveNpcEntry(character) != nullptr;
}

// @verified - p
std::uint64_t IsNpcRouteLoadedOrVisible(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags >> 3) & 1U;
    }
    return 0;
}

std::int32_t QueryNpcRouteLiveState(shen_name character) {
    const NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node)
        return -1;
    const std::uint32_t flags = entry->flags;
    if (!(flags & 2U) || entry->liveState == 7 || (flags & 0x140U))
        return -1;
    if (flags & 8U) {
        if (!(flags & 0x80U))
            return -1;
    } else if (flags & 0x80000U)
        return -1;
    return entry->eyeControlMode ? static_cast<std::int32_t>(entry->eyeControlMode) : static_cast<std::int32_t>(entry->routeDirectionMode);
}

bool CanNpcEnterScenario(shen_name character) {
    const NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node || !(entry->flags & 2U))
        return false;
    const std::uint32_t flags = entry->flags;
    if ((flags & 0x4010U) != 0x10U) {
        if (entry->liveState == 7 || (flags & 0x140U))
            return false;
        if (flags & 8U) {
            if (!(flags & 0x80U))
                return false;
        } else if (flags & 0x80000U)
            return false;
        if (GetNpcAreaSpecialMotionFlags(*entry) & 0x80U)
            return false;
    }
    const PartyCompanion *const party = FindPartyCompanion(FindCharacterByScenarioName(character));
    return !party || !(party->flags & 2U) || party->behaviorState != 8;
}

bool BeginNpcSpecialMotion(shen_name character) {
    if (!npcRegistryTask.current)
        return false;
    NpcRouteLiveEntry *entry{};
    const std::int32_t acquired = AcquireNpcRuntimeEntryForScript(character, &entry);
    if (!acquired)
        return false;
    if (acquired == -1)
        return true;
    SetPartyConversationPaused(character, true);
    entry->specialMotionPhase = 0;
    if (entry->previousLiveState == 8) {
        if (GetNpcAreaSpecialMotionFlags(*entry) & 2U)
            SetMotionSpeed(entry->motionWork, 0.0f, MotionRegion::LowerBody);
        entry->commandFlags |= 0x40U;
    } else if (entry->previousLiveState != 1) {
        SetMotionSpeed(entry->motionWork, entry->previousLiveState == 4 ? 0.0f : 1.0f, MotionRegion::LowerBody);
    }
    entry->liveState = 2;
    entry->counter416 = 0;
    return true;
}

bool EndNpcSpecialMotion(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    SetPartyConversationPaused(character, false);
    UpdateMotionWorkWithPosition(entry->motionWork, nullptr, 0);
    RequestMotionTargetFacing(entry->motionWork, nullptr, 0);
    if (entry->flags & 8U) {
        entry->flags &= ~0x4010U;
        entry->liveState = entry->previousLiveState;
        entry->specialMotionPhase = 0;
        if (entry->motionWork)
            SetMotionTimeRelated(entry->motionWork, 0, 0, 0);
    } else {
        entry->specialMotionPhase = 10;
        entry->flags |= 0x4000U;
    }
    return true;
}

bool RequestNpcSpecialMotionTarget(shen_name character, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 3;
        entry->specialMotionTarget = *target;
    }
    return true;
}

bool ResetNpcSpecialMotionTarget(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 5;
        UpdateMotionWorkWithPosition(entry->motionWork, nullptr, 0);
    }
    return true;
}

bool RequestNpcSpecialMotionLookTarget(shen_name character, const Vector3 *target) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U)) {
        entry->specialMotionPhase = 6;
        UpdateMotionWorkWithPosition(entry->motionWork, nullptr, 2);
        entry->specialMotionTarget = *target;
    }
    return true;
}

bool ResetNpcSpecialMotionLookTarget(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !(entry->flags & 0x10U))
        return false;
    if (!(entry->flags & 0x200000U))
        entry->specialMotionPhase = 7;
    return true;
}
bool QueryNpcSceneCheck(shen_name character) {
    NpcRouteLiveEntry *const entry = character.name ? FindActiveNpcEntry(character) : nullptr;
    return entry && MoveForNpcInData(*entry, 0, false);
}

// @verified - p
std::uint64_t IsNpcSceneMotionPlaying(shen_name character) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry || !entry->node)
        return 0;
    for (std::uint32_t region = 1; region < 5; ++region) {
        if (SetMotionRegionFlag(entry->motionWork, 72, 1, static_cast<MotionRegion>(region)) &&
            GetMotionAnimationId(entry->motionWork, static_cast<MotionRegion>(region)) ==
                RemapNpcMotionForRig(GetCharacterBoneId(entry->node), NPC_SCENE_MOTION))
            return 1;
    }
    return 0;
}

// @verified - p
std::uint64_t QueryNpcSceneMotionPlaying(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    return IsNpcSceneMotionPlaying(character);
}

// @verified - p
std::uint64_t QueryNpcCameraMotionProfile(shen_name character, std::uint8_t mask) {
    NpcRouteLiveEntry *const entry = FindActiveNpcEntry(character);
    if (!entry)
        return 0;
    std::uint32_t result = 0;
    if (mask & 1U) {
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_ONE_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == NPC_CAMERA_PROFILE_ONE_MOTIONS[index]) {
                result = 1;
                break;
            }
        }
    }
    if (mask & 2U) {
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_TWO_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == NPC_CAMERA_PROFILE_TWO_MOTIONS[index]) {
                result = 1;
                break;
            }
        }
    }
    if ((mask & 8U) && entry->node) {
        const std::int32_t boneId = GetCharacterBoneId(entry->node);
        for (std::size_t index = 0; NPC_CAMERA_PROFILE_REMAPPED_MOTIONS[index]; ++index) {
            if (entry->currentMotionId == RemapNpcMotionForRig(boneId, NPC_CAMERA_PROFILE_REMAPPED_MOTIONS[index])) {
                result = 1;
                break;
            }
        }
    }
    if (mask & 4U) {
        if (entry->routeMotionState == 23 || !entry->node || static_cast<std::uint32_t>(GetCharacterBoneId(entry->node)) - 3U > 2U) {
            for (std::size_t index = 0; NPC_CAMERA_PROFILE_FOUR_MOTIONS[index]; ++index) {
                if (entry->currentMotionId == NPC_CAMERA_PROFILE_FOUR_MOTIONS[index])
                    return 1;
            }
        } else if (GetNpcAreaSpecialMotionFlags(*entry) & 1U)
            return 1;
    }
    return result;
}

void NpcRegistryTask_Nullsub(void *) {
}




bool EnqueueNpcRegistryTask(std::uint8_t nextFunctionIndex) {
    npcLoadedArea = g_currentMap;
    npcNewLoadState = 0;
    npcRegistryTask.current = nullptr;
    npcRegistryTask.pending = HLib::EnqueueTaskWithoutParameter(NpcRegistryTask_Callback, nextFunctionIndex, 2, static_cast<std::int32_t>(NPC_ROUTE_TASK.name));
    npcSetupHumans();
    if (!IsNpcRouteArea(g_currentMap)) {
        HLib::AssignCallbackForTask(npcRegistryTask.pending, NpcRegistryTask_Nullsub);
        npcRegistryTask.pending = nullptr;
        FreeNpcRouteLiveEntries();
        return true;
    }
    return npcRegistryTask.pending != nullptr;
}

bool PublishNpcRegistryTask() {
    if (!npcRegistryTask.pending)
        return false;
    // NPC_Route_Control retains this producer pointer after publication;
    // FTBL729 remains admitted until NPCT teardown, including cached reloads.
    npcRegistryTask.current = npcRegistryTask.pending;
    npcRegistryTask.areas = {};
    npcRegistryTask.areaCount = 0;
    if (!LoadCurrentNpcSchedule(true))
        return false;
    PublishNpcRouteLiveEntries(npcRouteEntries);
    EV_NPC_NewLoadSw(1);
    return true;
}

bool npcIsNpcTaskActive() {
    return npcRegistryTask.current != nullptr;
}

// @verified - p
bool NpcWait() {
    if (!npcRegistryTask.current)
        return false;
    if (!npcReloadPassStarted || npcWaitBypassActive)
        return true;
    if (npcWaitSawLoadFailure || static_cast<std::int32_t>(++npcWaitFrames) > 150)
        return false;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (!(entry.flags & NpcRouteAbstractActive) && (entry.flags & NpcRouteStreamRequested))
            return true;
    }
    return false;
}

void FreeNpcRegistryTask() {
    FreeNpcRouteLiveEntries();
    areaNpcLoadCharacters = nullptr;
    areaNpcLoadStartupTask = nullptr;
    areaNpcLoadPhase = 0;
    HLib::Task *const pending = npcRegistryTask.pending;
    HLib::Task *const current = npcRegistryTask.current;
    npcRegistryTask = {};
    if (pending)
        HLib::DestroyTask(pending);
    if (current && current != pending)
        HLib::DestroyTask(current);
}

std::int32_t npcSetAreaName(shen_name area, const Vector3 *center, float radius, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;

    if (enabled == 1) {
        for (NpcNoSpawnArea &slot : npcRegistryTask.areas) {
            if (slot.name.name != 0)
                continue;
            ++npcRegistryTask.areaCount;
            slot = NpcNoSpawnArea{area, *center, radius};
            return 1;
        }
        return 0;
    }

    for (NpcNoSpawnArea &slot : npcRegistryTask.areas) {
        if (slot.name.name != area.name)
            continue;
        --npcRegistryTask.areaCount;
        slot.name = {};
        return 1;
    }
    return 0;
}

std::int32_t npcGetPosFor(shen_name character, Vector3 *position) {
    if (!npcRegistryTask.current)
        return 0;
    for (const NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!npcPositionQueryMode) {
            if (!entry.node)
                return 0;
            npcPositionQueryMode = 1;
        }
        *position = entry.worldPosition;
        position->x -= npcSchedule.areaOrigin.x;
        position->z -= npcSchedule.areaOrigin.z;
        return 1;
    }
    return 0;
}

std::int64_t MarkNpcRoutePartyControl(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NpcRouteAbstractActive))
            return 0;
        entry.flags |= NpcRouteFlag80;
        return 1;
    }
    return 0;
}

std::int64_t ClearNpcRoutePartyControl(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NpcRouteAbstractActive))
            return 0;
        entry.flags &= ~NpcRouteFlag80;
        return 1;
    }
    return 0;
}

// @verified - p
NpcRouteLiveEntry *WarmStartNpcRoute(shen_name character) {
    if (!npcRegistryTask.current)
        return nullptr;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & NpcRouteAbstractActive)) {
            npcPoolTarget -= entry.streamCost;
            if ((entry.flags & (NpcRouteStreamRequested | NpcRouteStreamStartHidden)) == (NpcRouteStreamRequested | NpcRouteStreamStartHidden))
                ReleaseNpcRouteStream(entry, npcRouteRuntimeRecords[index], false);
        }
        entry.flags |= NpcRouteAbstractActive | NpcRouteFlag80;
        entry.controlFlags |= 2U;
        if (entry.node) {
            FindMotionWork(entry.node)->runtime.callbackFlags &= ~8U;
            SetFaceFlagsForChara_0(entry.node, FacialNodeReplacement);
        }
        return &entry;
    }
    return nullptr;
}

std::int64_t EV_CheckWarmStartNpcRoute(shen_name character) {
    return WarmStartNpcRoute(character) != nullptr;
}

// Warm-starts the route and selects the HUMANS AFS load path.
std::int64_t npcWarmStartHumanRoute(shen_name character) {
    NpcRouteLiveEntry *const entry = WarmStartNpcRoute(character);
    if (!entry)
        return 0;
    entry->flags |= NpcRouteStreamUseHumansAfs;
    return 1;
}

std::int64_t EV_ForceLoadNpc(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NpcRouteStreamStartHidden;
            return RequestNpcStreamLoad(entry);
        }
    }
    return LoadNativeNpcHuman(character, character, false) > 0;
}

std::uint64_t EV_npcStreamOrLoadHuman(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NpcRouteStreamStartHidden;
            RequestNpcStreamLoad(entry);
            return static_cast<std::uint32_t>(npcLastHumanLoadResult);
        }
    }
    npcLastHumanLoadResult = LoadNativeNpcHuman(character, character, false);
    return static_cast<std::uint32_t>(npcLastHumanLoadResult);
}

void ResumeNpcMotionAfterUnlock(NpcRouteLiveEntry &entry) {
    if (!entry.node)
        return;
    for (std::uint32_t region = 1; region < 5; ++region)
        entry.motionWork->runtime.regionFlags[region] |= 0x400U;
    mtwkSetRequestedRootMode(entry.motionWork, 1);
    SetMotionFrameTime(entry.motionWork, 20, MotionRegion::LowerBody);
    switch (entry.liveState) {
    case 1:
        entry.currentMotionId = SelectNpcLocomotion(entry.stateContext, (entry.stateContext.flags & 0x10U) ? 1 : 0);
        break;
    case 2:
        return;
    case 3:
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        if (entry.flags & 0x400U) {
            Vector3i rotation;
            std::memcpy(&rotation, &entry.euler, sizeof(rotation));
            SetCharacterWorldRotation(entry.node, rotation);
            return;
        }
        break;
    case 4:
        if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
            control->flags &= ~(CharacterControlApplyGravity | CharacterControlEvaluateMotion);
        shenApplyTranslationToAll(&entry.node->entity.node, entry.localPosition);
        break;
    case 5:
        ResumeNpcScheduleMotion(entry);
        break;
    case 6:
        entry.currentMotionId = SelectNpcSeatedIdleMotion(entry);
        break;
    case 8: {
        NpcAreaRouteState &state = *GetNpcAreaPointRouteState(entry.currentAreaPoint);
        if (entry.specialAnimationContext)
            entry.currentMotionId = ResolveNpcAreaAnimation(state, entry, false);
        if (!entry.specialAnimationContext)
            entry.currentMotionId = ResolveNpcAreaAnimation(state, entry, false);
        if (!entry.currentMotionId)
            entry.currentMotionId = SelectNpcIdleMotion(entry);
        ApplyNpcAreaMotion(entry, entry.currentMotionId);
        if (((entry.routeMotionState - 16) & 0xfb) == 0) {
            if (CharacterControlModel *const control = FindCharacterControlModel(entry.node))
                control->flags |= CharacterControlStairMode;
            entry.commandFlags |= 0x800U;
        }
        break;
    }
    default:
        entry.currentMotionId = SelectNpcIdleMotion(entry);
        break;
    }
    mtwkRequestMotion(entry.motionWork, entry.currentMotionId, MotionRegion::LowerBody);
    Vector3i rotation;
    std::memcpy(&rotation, &entry.euler, sizeof(rotation));
    SetCharacterWorldRotation(entry.node, rotation);
}

// @verified - p
std::int64_t npcSetLock(shen_name character, std::int32_t preserveMotion) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (ReadU32(entry.recordBase) == SHEN_NAME(MOVE))
            return 0;
        if (entry.flags & NpcRouteAbstractActive) {
            npcPoolTarget += entry.streamCost;
            entry.flags &= ~(NpcRouteAbstractActive | NpcRouteStreamUseHumansAfs);
            entry.controlFlags &= ~2U;
            if (!preserveMotion)
                ResumeNpcMotionAfterUnlock(entry);
            if (entry.node)
                SetFaceFlagsForChara_0(entry.node, FacialNodeOriginal);
        }
        return 1;
    }
    return 0;
}

std::int64_t EV_npcLockUnlock(shen_name character) {
    return npcSetLock(character, 0);
}

std::int64_t EV_ReleaseNpcLockPreservingMotion(shen_name character) {
    return npcSetLock(character, 1);
}

std::int64_t EV_npcFree(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            if (entry.flags & (NpcRouteStreamRequested | NpcRouteStreamLive | NpcRouteStreamFadeOut)) {
                ReleaseNpcRouteStream(entry, npcRouteRuntimeRecords[index], false);
            }
            return 1;
        }
    }
    return ReleaseNativeNpcHumanPackage(character);
}

void EV_SetAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask, bool visible, bool lowQualityHands) {
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadCharacters = characters;
    areaNpcLoadStartupTask = startupTask;
    areaNpcLoadCharactersVisible = visible;
    areaNpcLoadLowQualityHands = lowQualityHands;
}
void SetAreaNpcLowQualityHands(bool enabled) {
    areaNpcLoadLowQualityHands = enabled;
}
void SetAreaNpcCharactersVisible(bool enabled) {
    areaNpcLoadCharactersVisible = enabled;
}
void AssignAreaNpcLoadContext(const shen_name *characters, HLib::Task *startupTask) {
    areaNpcLoadPhase = 0;
    areaNpcLoadCharacters = characters;
    areaNpcLoadStartupTask = startupTask;
}
namespace npc_package_detail {

std::int32_t FindConditionalNpcLoadSlot(shen_name name) {
    for (std::size_t index = 0; index < conditionalNpcLoadSlots.size(); ++index)
        if (conditionalNpcLoadSlots[index].character.name == name.name)
            return static_cast<std::int32_t>(index);
    return -1;
}
void ConditionalNpcLoadCheckCallback(void *data) {
    auto &work = *static_cast<ConditionalNpcLoadWork *>(data);
    for (std::int32_t index = 0; work.entries[index].character.name != 0xffffffffU; ++index) {
        const auto &entry = work.entries[index];
        const std::int32_t slot = FindConditionalNpcLoadSlot(entry.character);
        if (slot == -1 && !work.admitted[index] && (entry.storyFlag <= 0 || EV_GetShenmueFlag(entry.storyFlag)) && (!entry.condition || entry.condition())) {
            work.phase = 0;
            work.entryIndex = 0;
            EV_NPC_NewLoadSw(0);
            for (std::int32_t other = 0; work.entries[other].character.name != 0xffffffffU; ++other) {
                const shen_name name = work.entries[other].character;
                if (IsNpcLoadedOrVisible(name)) {
                    EV_npcLockUnlock(name);
                    EV_npcFree(name);
                    work.admitted[other] = 0;
                }
            }
            conditionalNpcLoadSlots.fill({});
            HLib::SetCurrentTaskTo(ConditionalNpcLoadCallback);
            return;
        }
        if (entry.storyFlag != -1) {
            const auto retire = [&] {
                const std::int32_t owned = FindConditionalNpcLoadSlot(entry.character);
                if (owned == -1)
                    return;
                if (conditionalNpcLoadSlots[owned].entryIndex == index) {
                    conditionalNpcLoadSlots[owned].character = {};
                    EV_npcLockUnlock(entry.character);
                    EV_npcFree(entry.character);
                }
                work.admitted[index] = 0;
            };
            if (!EV_GetShenmueFlag(entry.storyFlag))
                retire();
            if (entry.condition && !entry.condition())
                retire();
        }
    }
    conditionalNpcLoadsReady = 1;
}
void ConditionalNpcLoadCallback(void *data) {
    auto &work = *static_cast<ConditionalNpcLoadWork *>(data);
    const auto *entry = work.entries + work.entryIndex;
    if (!work.phase) {
        if (PrepareNpcAllFromArray())
            ++work.phase;
        return;
    }
    if (work.phase == 1) {
        for (; entry->character.name != 0xffffffffU; ++entry, ++work.entryIndex) {
            if (entry->storyFlag > 0 && !EV_GetShenmueFlag(entry->storyFlag))
                continue;
            if (!DoesScenarioCharacterExist(entry->character) || FindConditionalNpcLoadSlot(entry->character) == -1) {
                if (!entry->condition || entry->condition()) {
                    ++work.phase;
                    EV_CheckWarmStartNpcRoute(entry->character);
                    return;
                }
                work.admitted[work.entryIndex] = 0;
            }
        }
        EV_NPC_NewLoadSw(1);
        HLib::SetCurrentTaskTo(ConditionalNpcLoadCheckCallback);
        return;
    }
    if (work.phase == 2 && EV_ForceLoadNpc(entry->character)) {
        std::size_t slot = 0;
        while (conditionalNpcLoadSlots[slot].character.name)
            ++slot;
        conditionalNpcLoadSlots[slot] = {entry->character, work.entryIndex};
        EV_EnableCharacterDisplay(entry->character);
        EV_AT_SetPos(entry->character, EventTransformFlags::World | EventTransformFlags::AllAxes, entry->position);
        EV_AT_SetAngY(entry->character, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{0, entry->yaw, 0});
        if (entry->motion != 0xffffffffU)
            mtwkRequestMotion(
                FindMotionWork(FindCharacterByScenarioName(entry->character)), static_cast<std::uint16_t>(entry->motion), MotionRegion::LowerBody);
        work.admitted[work.entryIndex++] = 1;
        work.phase = 1;
    }
}
} // namespace npc_package_detail

void ResetConditionalNpcLoads() {
    conditionalNpcLoadsReady = 0;
    conditionalNpcLoadSlots.fill({});
}
std::uint32_t AreConditionalNpcLoadsPending() {
    return conditionalNpcLoadsReady == 0;
}
HLib::Task *QueueConditionalNpcLoads(const ConditionalNpcLoadEntry *entries) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ConditionalNpcLoadCheckCallback, 4, 0x0b, sizeof(ConditionalNpcLoadWork), SHEN_NAME(EVNT));
    auto *const work = static_cast<ConditionalNpcLoadWork *>(HLib::GetTaskParameter(task));
    if (work) {
        *work = {};
        work->entries = entries;
        ResetConditionalNpcLoads();
        HLib::SetupTimedTask(task, 5);
    } else
        HLib::DestroyTaskInternal(task);
    return task;
}

void AssignGenericNpcLoadContext(const shen_name *characters, HLib::Task *startupTask) {
    genericNpcLoadPhase = 0;
    genericNpcLoadCharacters = characters;
    genericNpcLoadStartupTask = startupTask;
}

void SetGenericNpcLowQualityHands(bool enabled) {
    genericNpcLoadLowQualityHands = enabled;
}

void ResetGenericEventNpcLoadState() {
    genericNpcLoadCharacters = nullptr;
    genericNpcLoadPhase = 0;
    genericNpcLoadLowQualityHands = 1;
    genericNpcLoadCharactersVisible = 1;
    genericNpcLoadStartupTask = nullptr;
}

HLib::Task *Queue_EVT_UTL_LoadNPCChara(const shen_name *characters, HLib::Task *startupTask) {
    AssignGenericNpcLoadContext(characters, startupTask);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startupTask)
        HLib::SetupTimedTask(startupTask, 0);
    return HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_1, 6, 0x0b, static_cast<std::int32_t>(TASK_NPC_LOAD.name));
}

void ResetCompiledEventNpcLoadState() {
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadLowQualityHands = 1;
    areaNpcLoadCharactersVisible = 1;
    areaNpcLoadStartupTask = nullptr;
    areaNpcLoadCharacters = nullptr;
}

HLib::Task *Queue_EVT_UTL_LoadNPCChara_3(const shen_name *characters, HLib::Task *startupTask) {
    areaNpcLoadCharacters = characters;
    areaNpcLoadPhase = 0;
    g_usingCustomAuthorCharacterList = 0;
    areaNpcLoadStartupTask = startupTask;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (startupTask)
        HLib::SetupTimedTask(startupTask, 0);
    return HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(TASK_NPC_LOAD.name));
}

// @verified - p
void EVT_UTL_LoadNPCChara_Callback_1(void *) {
    const shen_name character = *genericNpcLoadCharacters;
    if (!genericNpcLoadPhase) {
        EV_CheckWarmStartNpcRoute(character);
        (void)EV_ForceLoadNpc(character);
        genericNpcLoadPhase = 1;
        return;
    }
    if (!EV_ForceLoadNpc(character))
        return;
    if (genericNpcLoadCharactersVisible) {
        EV_EnableCharacterDisplay(character);
        EV_EnsureCharacterDraw(character);
    } else {
        EV_DisableCharacterDisplay(character);
        EV_ToggleCharacterFlag(character);
    }
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
    if (genericNpcLoadLowQualityHands)
        SetLowQualityHands(FindCharacterByScenarioName(character));
    ++genericNpcLoadCharacters;
    genericNpcLoadPhase = 0;
    if (genericNpcLoadCharacters->name == 0xffffffffU) {
        if (genericNpcLoadStartupTask)
            HLib::ResolveDeferredCallback(genericNpcLoadStartupTask);
        HLib::DestroyCurrentTask();
    }
}

// @verified - p
void EnableAuthoredCharacterFlagsAndDisplay(shen_name character) {
    EV_EnableCharacterDisplay(character);
    EV_EnsureCharacterDraw(character);
    EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetRefBoardParamsFor(character, 1);
    EV_SetCharacterAlphaEnabled(character, false);
}

void EVT_UTL_LoadNPCChara_Callback_2(void *taskData) {
    (void)taskData;
    const shen_name character = *areaNpcLoadCharacters;

    if (areaNpcLoadPhase != 0) {
        if (!EV_ForceLoadNpc(character))
            return;

        if (areaNpcLoadCharactersVisible != 0) {
            EnableAuthoredCharacterFlagsAndDisplay(character);
        } else {
            EV_DisableCharacterDisplay(character);
            EV_ToggleCharacterFlag(character);
            EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetRefBoardParamsFor(character, 1);
            EV_SetCharacterAlphaEnabled(character, false);
        }

        if (areaNpcLoadLowQualityHands != 0) {
            SetLowQualityHands(FindCharacterByScenarioName(character));
        }

        ++areaNpcLoadCharacters;
        areaNpcLoadPhase = 0;
        if (areaNpcLoadCharacters->name == 0xffffffffU) {
            g_usingCustomAuthorCharacterList = 1;
            if (areaNpcLoadStartupTask)
                HLib::ResolveDeferredCallback(areaNpcLoadStartupTask);
            HLib::DestroyCurrentTask();
        }
        return;
    }

    EV_CheckWarmStartNpcRoute(character);
    EV_ForceLoadNpc(character);
    areaNpcLoadPhase = 1;
}



std::uint32_t npcReleaseStreamsNearDoor(const Vector3 *point, float radius) {
    std::uint32_t released = 1;
    if (!npcRegistryTask.current)
        return released;
    if (npcNewLoadState)
        EV_NPC_NewLoadSw(0);
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    std::int32_t cost = npcActiveStreamCost;
    const float radiusSquared = radius * radius;
    for (NpcRouteLiveEntry *entry = npcCrowdHead; entry && cost; entry = entry->next) {
        if (!(entry->flags & 7U))
            continue;
        cost -= entry->streamCost;
        const float dx = entry->localPosition.x - point->x;
        const float dz = entry->localPosition.z - point->z;
        if (dx * dx + dz * dz <= radiusSquared && SetNpcStreamLifecycleState(*entry, 1) == 0)
            released = 0;
    }
    return released;
}



shen_name npcFindAreaPointOccupant(shen_name owner) {
    for (std::uint16_t point = 0; point < npcSchedule.areaPointCount; ++point) {
        const std::byte *const record = GetNpcAreaPoint(point);
        if (!record || ReadU32(record + 0x30) != owner.name || static_cast<std::uint16_t>(ReadU32(record)) != static_cast<std::uint16_t>(npcLoadedArea.name))
            continue;
        for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
            const NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (!(entry.flags & 0x800U) || entry.currentAreaPoint != point)
                continue;
            if (npcSchedule.floor != -1000 && npcSchedule.floor != static_cast<std::int8_t>(entry.packedMoveFlags >> 8U))
                continue;
            return entry.name;
        }
    }
    return {};
}



std::int64_t npcSetDoorAnswerMode(shen_name character, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;
    for (NpcRouteLiveEntry &entry : npcRouteEntries) {
        if (entry.name.name != character.name)
            continue;
        const std::uint32_t flags = entry.flags;
        if (!enabled) {
            if (flags & 0x200000U) {
                const auto saved = static_cast<std::int8_t>(entry.field338);
                if (saved >= 0)
                    entry.routeDirectionMode = saved;
                else
                    entry.eyeControlMode = static_cast<std::int8_t>(-saved);
                entry.flags = flags & ~0x200000U;
            }
        } else if (!(flags & 0x200000U)) {
            const auto eye = static_cast<std::uint8_t>(entry.eyeControlMode);
            if (eye) {
                entry.eyeControlMode = 13;
                entry.field338 = static_cast<std::uint8_t>(-eye);
            } else {
                entry.field338 = static_cast<std::uint8_t>(entry.routeDirectionMode);
                entry.routeDirectionMode = 13;
            }
            entry.flags = flags | 0x200000U;
        }
        return 1;
    }
    return 0;
}

std::int64_t EV_ResetFreeTalkFlags() {
    if (!npcRegistryTask.current)
        return 1;
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (entry.loadHumanResult < 0) {
            entry.loadHumanResult = 8;
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        if (!(entry.flags & (NpcRouteAbstractActive | NpcRouteStreamPinned)) && (entry.flags & 7U)) {
            FlushQueuedLegacyDrawPasses();
            ReleaseNpcRouteStream(entry, runtime, false);
        }
    }
    return 1;
}

// @verified - p
std::int64_t ResetNpcColdStreams() {
    if (!npcRegistryTask.current)
        return 1;
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.stateContext.flags & 1U) {
            if ((entry.stateContext.flags & 0x300U) || (entry.stateContext.routeFlags & 0x10U))
                continue;
            if (!(entry.flags & 0x4000000U)) {
                SetNpcStreamLifecycleState(entry, 1);
                continue;
            }
        }
        if (static_cast<std::uint8_t>(entry.recordBase[0x0b]) & 0x10U)
            SetNpcStreamLifecycleState(entry, 1);
    }
    return 1;
}

// @verified - p
std::uint64_t SetNpcColdResetMode(std::uint32_t mode) {
    if (!mode)
        ResetNpcColdStreams();
    npcColdResetMode = mode;
    return mode;
}

std::uint64_t CountNpcLiveStreams() {
    if (!npcRegistryTask.current || !npcReloadPassStarted)
        return 0;
    std::uint32_t count = 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index)
        if (npcRouteEntries[index].flags & NpcRouteStreamLive)
            ++count;
    return count;
}

std::uint64_t IsNpcLoadedOrVisible(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags & NpcRouteAbstractActive) != 0;
    }
    return 0;
}

std::uint64_t IsNpcUsingHumansAfs(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name == character.name)
            return (entry.flags & NpcRouteStreamUseHumansAfs) != 0;
    }
    return 0;
}

std::int64_t EV_ForceLoadNpcFromHumans(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NpcRouteStreamStartHidden;
            return RequestNpcStreamLoad(entry);
        }
    }
    return LoadNativeNpcHuman(character, character, true) > 0;
}

std::uint64_t EV_npcStreamOrLoadHumanFromHumans(shen_name character) {
    if (npcRegistryTask.current) {
        for (std::size_t index = 0; index < npcRouteEntries.size(); ++index) {
            NpcRouteLiveEntry &entry = npcRouteEntries[index];
            if (entry.name.name != character.name)
                continue;
            entry.flags &= ~NpcRouteStreamStartHidden;
            RequestNpcStreamLoad(entry);
            return static_cast<std::uint32_t>(npcLastHumanLoadResult);
        }
    }
    npcLastHumanLoadResult = LoadNativeNpcHuman(character, character, true);
    return static_cast<std::uint32_t>(npcLastHumanLoadResult);
}

// @verified - p
std::uint64_t SetNpcRouteControlHold(shen_name character, std::int32_t enabled) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        const std::uint32_t previous = entry.controlFlags;
        entry.controlFlags = enabled ? previous | 0x20U : previous & ~0x20U;
        return (previous & 0x20U) != 0;
    }
    return 0;
}

// @verified - p
std::int64_t EV_npcUnknownFunc(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & 8U))
            return 0;
        entry.flags &= ~0x80U;
        return 1;
    }
    return 0;
}

// @verified - p
std::int64_t EV_npcUpdateFlags(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if (!(entry.flags & 8U))
            return 0;
        entry.flags |= 0x80U;
        return 1;
    }
    return 0;
}

// @verified - p
std::uint32_t GetNpcScriptRouteCompletion(shen_name character) {
    if (!npcRegistryTask.current)
        return 0;
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        const NpcRouteLiveEntry &entry = npcRouteEntries[index];
        if (entry.name.name != character.name)
            continue;
        if ((entry.flags & 0x88U) != 0x88U)
            return 0;
        return (GetNpcAreaSpecialMotionFlags(entry) ^ 0x80U) >> 7;
    }
    return 0;
}

// Releases every unpinned, non-abstract stream that is still only requested.
std::int32_t npcReleaseUnusedStreams() {
    if (!npcRegistryTask.current)
        return 1;
    ReleaseReadyNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (ReadNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset) < 0) {
            WriteNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset, 8);
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        if ((entry.flags & (NpcRouteAbstractActive | NpcRouteStreamPinned)) == 0 &&
            (entry.flags & (NpcRouteStreamRequested | NpcRouteStreamLive | NpcRouteStreamFadeOut)) != 0) {
            FlushQueuedLegacyDrawPasses();
            if (entry.flags & NpcRouteStreamRequested)
                ReleaseNpcRouteStream(entry, runtime, false);
        }
    }
    return 1;
}

// Returns 1 once no unpinned, non-abstract stream is still live or fading out.
std::int32_t PrepareNpcAllFromArray() {
    std::int32_t released = 1;
    if (!npcRegistryTask.current)
        return released;
    if (GetNpcNewLoadState() != 0)
        EV_NPC_NewLoadSw(0);
    FlushQueuedLegacyDrawPasses();
    FlushQueuedStripType3Batches();
    ReleaseNpcTransitionHumanPackages();
    for (std::size_t index = 0; index < npcAuthoredEntryCount; ++index) {
        NpcRouteLiveEntry &entry = npcRouteEntries[index];
        NpcRouteRuntimeRecord &runtime = npcRouteRuntimeRecords[index];
        if (ReadNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset) < 0) {
            WriteNpcLiveField<std::int8_t>(entry, npcLoadHumanResultOffset, 8);
            if (runtime.streamState == NpcHumanStreamState::Missing)
                runtime.streamState = NpcHumanStreamState::Inactive;
        }
        std::uint32_t pending = 4;
        if ((entry.flags & NpcRouteAbstractActive) != 0) {
            pending = 2;
        } else if ((entry.flags & NpcRouteStreamPinned) != 0) {
            pending = 3;
        } else if ((entry.flags & (NpcRouteStreamRequested | NpcRouteStreamLive | NpcRouteStreamFadeOut)) != 0) {
            FlushQueuedLegacyDrawPasses();
            pending = entry.flags & NpcRouteStreamRequested;
            if (pending != 0)
                ReleaseNpcRouteStream(entry, runtime, false);
            if ((entry.flags & (NpcRouteStreamLive | NpcRouteAbstractActive | 0x10U)) == NpcRouteStreamLive) {
                entry.field335 = 0;
                entry.flags = (entry.flags & ~(NpcRouteStreamLive | NpcRouteStreamFadeOut)) | NpcRouteStreamFadeOut;
            }
            if ((entry.flags & NpcRouteStreamFadeOut) != 0)
                UpdateNpcStreamFade(entry);
        }
        if (pending == 0)
            released = 0;
    }
    return released;
}

// @verified - p
void EV_ReconfigureNpcRoutesForStoryStep() {
    if (!npcRegistryTask.current)
        return;
    EV_ResetFreeTalkFlags();
    WarmBootstrapNpcRoutes();
}

std::uint64_t GetNpcNewLoadState() {
    return npcNewLoadState;
}
} // namespace shenmue::s2
