#include "s1/task/ctrl.hpp"
#include "s1/chrd.hpp"
#include "s1/d3t/model.hpp"
#include "s1/task/cdtt.hpp"
#include "s1/task/disp.hpp"
#include "s1/task/modu.hpp"
#include "s1/task/meve.hpp"
#include "s1/task/char.hpp"
#include "s1/task/cuff.hpp"
#include "s1/task/scrl.hpp"
#include "s1/task/film.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/ppad.hpp"
#include "s1/task/paus.hpp"
#include "s1/task/cach.hpp"
#include "s1/task/came.hpp"
#include "s1/task/tmex.hpp"
#include "s1/task/cstc.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/dswk.hpp"
#include "s1/task/dwmz.hpp"
#include "s1/task/effe.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/kdan.hpp"
#include "s1/task/gull.hpp"
#include "s1/task/hato.hpp"
#include "s1/task/krep.hpp"
#include "s1/task/kuti.hpp"
#include "s1/task/lcdm.hpp"
#include "s1/task/mbst.hpp"
#include "s1/task/metk.hpp"
#include "s1/task/mdlm.hpp"
#include "s1/task/lamp.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/motf.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/fbgg.hpp"
#include "s1/task/fork.hpp"
#include "s1/task/save.hpp"
#include "s1/task/sond.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/trwv.hpp"
#include "s1/task/time.hpp"
#include "s1/task/vfma.hpp"
#include "s1/task/warn.hpp"
#include "s1/task/wind.hpp"
#include "s1/collision.hpp"
#include "s1/dynamic_map.hpp"
#include "s1/face.hpp"
#include "s1/frame_pipeline.hpp"
#include "s1/input.hpp"
#include "s1/initialization.hpp"
#include "s1/player_control.hpp"
#include "s1/player_stats.hpp"
#include "s1/lighting.hpp"
#include "s1/motion_registry.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/skinning_cache.hpp"
#include "s1/file_state.hpp"
#include "s1/file_loader.hpp"
#include "s1/ui_runtime.hpp"
#include "s1/save_format.hpp"

#include <algorithm>

#include <array>
#include <cmath>
#include <bit>
#include <cstddef>
#include <cstdio>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {
extern HLib::Task *g_effeTask;
extern bool g_effectQueueReplayedCommandStream;
extern std::array<std::uint32_t, 5> g_saveDataRelated;
extern std::uint8_t g_globalFlags[1];
void FOG_InitFog();
std::int64_t ForEachCewpEntityApplyCallback(std::int64_t reserved, void (*callback)(void *));

namespace {
constexpr std::uint32_t kMapEventComponent = SHEN_NAME(MEVW);
constexpr std::uint32_t kCharacterHandlerType = SHEN_NAME(CEWP);
constexpr std::uint16_t kActionButton = 0x0400U;

const AreaIdleStatsProfile &FindAreaIdleStatsProfile(std::uint32_t areaId) {
    for (const AreaIdleStatsProfile &profile : kAreaIdleStatsProfiles) {
        if (profile.areaId == areaId || profile.areaId == 0)
            return profile;
    }
    return kAreaIdleStatsProfiles[std::size(kAreaIdleStatsProfiles) - 1];
}

} // namespace

std::uint32_t g_areaId = 0;
std::int32_t g_sceneId = 0;
std::uint32_t g_entryPoint = 0;
HLib::Task *g_mapControlTask = nullptr;
void HLTaskFunc_InternalInit();
extern void *g_scrollCurrentInfo;
extern std::int32_t g_bShouldFreeActiveScroll;

std::uint16_t CTRL_EvaluateWeatherSoundEnvironment(Character *character, float x, float z) {
    if (!SelectCharacterMapQuery(character))
        return 0;
    return COLS_EvaluateWeatherSoundEnvironment(x, z);
}

bool MAPCTRL_GetCharacterIdTransform(std::uint32_t index, std::array<std::uint32_t, 3> &position, std::array<std::uint32_t, 3> &rotation) {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr || index >= 32) {
        return false;
    }
    const auto *const control = reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr);
    const CharacterIdTransform &entry = control->characterIdTable[index];
    std::memcpy(position.data(), &entry.node.position, sizeof(position));
    std::memcpy(rotation.data(), &entry.node.rotation, sizeof(rotation));
    return true;
}

bool MAPCTRL_SetCharacterIdPackCollision(std::int32_t index, const void *collisionData) {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr || index < 0 || index >= 32) {
        return false;
    }
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    control->characterIdTable[static_cast<std::size_t>(index)].collisionData = collisionData;
    return true;
}

bool MAPCTRL_SetCharacterIdData(std::int32_t index, const std::array<std::uint32_t, 3> &position, const std::array<std::uint32_t, 3> &rotation,
    std::uint32_t field20, const void *collisionData) {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr || index < 0 || index >= 32) {
        return false;
    }
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    CharacterIdTransform &entry = control->characterIdTable[static_cast<std::size_t>(index)];
    std::memcpy(&entry.node.position, position.data(), sizeof(position));
    std::memcpy(&entry.node.rotation, rotation.data(), sizeof(rotation));
    entry.layer = field20;
    entry.collisionData = collisionData;
    return true;
}

void LoadGameModule(SystemModule module, std::int32_t sceneId, std::uint32_t areaId, std::uint32_t entryPoint) {

    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    control->flags |= 1U;
    control->module = module;
    control->sceneId = sceneId;
    control->areaId = areaId;
    control->entryPoint = entryPoint;

    const bool supportsTemporaryClearColour = module == SystemModule::Map || module == SystemModule::Harrier || module == SystemModule::HangOn;
    g_overrideClearColour = 0xff000000U;
    if (supportsTemporaryClearColour && g_shouldUseTemporaryClearColour) {
        g_overrideClearColour = (static_cast<std::uint32_t>(g_temporaryClearColour.red) << 24U) |
            (static_cast<std::uint32_t>(g_temporaryClearColour.green) << 16U) | (static_cast<std::uint32_t>(g_temporaryClearColour.blue) << 8U) |
            static_cast<std::uint32_t>(g_temporaryClearColour.alpha);
    }
    g_shouldOverrideClearColour = 1;

    const std::uint32_t moduleIndex = static_cast<std::uint32_t>(module);
    (void)RecordGameModuleLoadStats(moduleIndex);
    if (module == SystemModule::Harrier) {
        UpdateSteamStat(25);
        UpdateSteamAchievement(25);
        UpdateSteamStat(22);
    } else if (module == SystemModule::Credits) {
        UpdateSteamStat(9);
        UpdateSteamAchievement(9);
    } else if (module == SystemModule::HangOn) {
        UpdateSteamStat(26);
        UpdateSteamAchievement(26);
        UpdateSteamStat(22);
    }
}

void MapControlInternal() {
    auto *const control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    for (void *&bucketHead : control->characterBuckets) {
        auto **link = reinterpret_cast<CharacterBucketNode **>(&bucketHead);
        while (*link) {
            CharacterBucketNode *const node = *link;
            Character *const character = node->character;
            const bool preserve = character && character->entity.name == SHEN_NAME(CEWP) &&
                (((character->flags.value & 1U) != 0U) || ((character->flags.value & 2U) != 0U && (character->motionFlags & 1U) != 0U));
            if (preserve) {
                link = &node->next;
                continue;
            }

            SCEN_UnlinkCharacterIndexNode(control->scenarioTask, character);
            if (character == control->ryo)
                control->ryo = nullptr;
            CHAR_DestroyAndFreeComponents(character);
            HLib_DestroyTask(node->task);
            *link = node->next;
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(node), 0);
            --control->pendingCharacterRetirements;
        }
    }
}


void CEWP_RetireGroup(std::int32_t groupId, bool immediate) {
    auto *const control = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if (!control)
        return;
    for (void *bucketHead : control->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(bucketHead); node; node = node->next) {
            Character *const character = node->character;
            if (!character || character->entity.name != SHEN_NAME(CEWP) ||
                !((character->flags.value & 1U) != 0U || ((character->flags.value & 2U) != 0U && (character->motionFlags & 1U) != 0U)))
                continue;
            if (character->groupId != groupId)
                continue;
            if (!immediate)
                character->flags.value |= 2U;
            character->flags.value &= ~1U;
            ++control->pendingCharacterRetirements;
        }
    }
}

void HLTaskFunc_MapControlTask_Callback(HLib::TaskData *taskData) {
    const auto *const control = reinterpret_cast<const MapControlData *>(taskData);
    if (control->pendingCharacterRetirements != 0)
        MapControlInternal();
}

HLib::Task *FILM_Init(std::uint16_t nextFunctionIndex);

namespace {

constexpr std::int32_t kMapEventTaskToken = SHEN_NAME(MEVE);

MapControlData *CurrentControl() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    return reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
}

} // namespace

namespace {

struct MapInfoSubchunk {
    std::uint32_t token;
    std::int32_t size;
};

struct MapQuerySet {
    MapInfoSubchunk *collision;
    std::uint32_t collisionSize;
    std::uint32_t reserved_0c;
    MapInfoSubchunk *height;
    std::uint32_t heightSize;
    std::uint32_t reserved_1c;
    MapInfoSubchunk *event;
    std::uint32_t eventSize;
    std::uint32_t reserved_2c;
    MapInfoSubchunk *undo;
    std::uint32_t undoSize;
    std::uint32_t reserved_3c;
    MapInfoSubchunk *sound;
    std::uint32_t soundSize;
    std::uint32_t reserved_4c;
    MapInfoSubchunk *property;
    std::uint32_t propertySize;
    std::uint32_t reserved_5c;
    std::uint32_t *sectionOffsetTable;
};

static_assert(sizeof(MapQuerySet) == 0x68);

#pragma pack(push, 1)
struct MapEventWork {
    std::uint32_t flags;
    std::int32_t eventId;
    std::uint32_t kind;
    std::uint8_t eventFlags[32];
};
#pragma pack(pop)

static_assert(sizeof(MapEventWork) == 0x2c);
static_assert(offsetof(MapEventWork, eventFlags) == 0x0c);

MapEventWork *FindMapEventWork(Character *character) {
    CharacterComponentNode *const node = HLib_GetCharacterComponent(character, kMapEventComponent);
    return node ? reinterpret_cast<MapEventWork *>(node + 1) : nullptr;
}

void MapEV_ComponentCallback(Character *character, void *rawWork);

MapEventWork *AllocateMapEventWork(Character *character) {
    if (!character)
        return nullptr;

    MapEventWork *const work =
        static_cast<MapEventWork *>(AllocateHeapMemoryMODL(character, kMapEventComponent, sizeof(MapEventWork), MapEV_ComponentCallback));
    if (!work)
        return nullptr;
    std::memset(work, 0, sizeof(*work));
    work->flags = 3U;
    std::memset(work->eventFlags, 0xff, sizeof(work->eventFlags));
    work->eventFlags[0] &= static_cast<std::uint8_t>(~1U);
    if (MapEventWork *const first = FindMapEventWork(character))
        first->flags |= 2U;
    return work;
}

bool IsRetainedCharacter(const Character *character) {
    if (!character || character->entity.name != kCharacterHandlerType)
        return false;
    const std::uint32_t flags = character->flags.value;
    return (flags & 1U) != 0U || ((flags & 2U) != 0U && (character->motionFlags & 1U) != 0U);
}

void MapEV_Callback(Character *character, MapEventWork *work) {
    if (!character || !work)
        return;
    if (const MotionManagerWork *const motion = MOTM_GetWork(character);
        motion && static_cast<std::uint8_t>(motion->motion.poseEvaluationStage - 2U) <= 1U && (character->motionFlags & 0x10U) == 0U) {
        return;
    }
    const Vector3 position = character->entity.node.position;
    (void)SelectCharacterMapQuery(character);
    const auto *const query = static_cast<const MapQuerySet *>(g_activeMapQuerySet);
    if (!query || !query->event)
        return;

    const std::uint32_t packed = mevwCheckEventArea(position.x, position.z);
    const std::uint32_t eventId = packed & 0xffffU;
    if (static_cast<std::uint32_t>(work->eventId) == eventId)
        return;

    const std::uint32_t oldFlags = work->flags;
    work->flags = ((oldFlags & 2U) != 0U || eventId == 0xffU) ? oldFlags & ~1U : oldFlags | 1U;
    work->eventId = static_cast<std::int32_t>(eventId);
    work->kind = packed >> 16U;
}

void MapEV_ComponentCallback(Character *character, void *rawWork) {
    MapEV_Callback(character, static_cast<MapEventWork *>(rawWork));
}
} // namespace

std::uint32_t shenGetNextCharaFromTable(HLib::Task *scenarioTask, Character *character) {
    auto *const scenario = reinterpret_cast<ScenarioTaskData *>(HLib_GetTaskDataForTask(scenarioTask));
    const std::size_t bucket = reinterpret_cast<std::uintptr_t>(character) % scenario->characterBuckets.size();
    CharacterIdLookupNode **link = &scenario->characterBuckets[bucket];
    while (*link) {
        CharacterIdLookupNode *const node = *link;
        if (node->character == character) {
            *link = node->bucketNext;
            node->bucketNext = scenario->characterBuckets[bucket];
            scenario->characterBuckets[bucket] = node;
            return node->id;
        }
        link = &node->bucketNext;
    }
    return 0;
}

namespace {

void ParseMapEventScenario(ScenarioWindow *window, std::int32_t eventId, std::uint32_t actorId) {
    if (eventId >= window->entryCount)
        return;
    const std::uint32_t entryOffset = window->entryOffsets[eventId];
    auto *const entry = reinterpret_cast<std::byte *>(window->activeInfo) + entryOffset;
    const std::array<std::uint32_t, 2> arguments{actorId, static_cast<std::uint32_t>(eventId)};
    ScenarioStack *const stack = scenAllocateScntForScript(window, nullptr, entry, arguments.data(), static_cast<std::int32_t>(arguments.size()));
    scenParseByPtr2Frames(window, stack);
}

} // namespace

void MapEventScenario_Parse(Character *character, HLib::Task *scenarioTask) {
    if (!character)
        return;
    const CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kMapEventComponent);
    if (!component || (component->flags & 1U) == 0U)
        return;

    auto *const event = reinterpret_cast<MapEventWork *>(const_cast<CharacterComponentNode *>(component) + 1);
    if ((event->flags & 1U) != 0U && event->kind != MEK_WhileInside) {
        return;
    }

    const std::uint32_t eventId = static_cast<std::uint32_t>(event->eventId);
    if (eventId == 0xffU) {
        character->eventGroup = event->kind;
        event->flags |= 1U;
        return;
    }

    const std::uint8_t mask = static_cast<std::uint8_t>(1U << (eventId & 7U));
    std::uint8_t &eventByte = event->eventFlags[eventId >> 3U];
    if ((eventByte & mask) == 0U)
        return;

    const auto parseEvent = [&] {
        const std::uint32_t actorId = shenGetNextCharaFromTable(scenarioTask, character);
        auto *const scenario = reinterpret_cast<ScenarioTaskData *>(HLib_GetTaskDataForTask(scenarioTask));
        (void)ParseMapEventScenario(scenario->scenarioWindow, static_cast<std::int32_t>(eventId), actorId);
    };
    const auto actionPressed = [&] {
        if (g_NumFramesProcessed != 0)
            return false;
        auto *control = reinterpret_cast<MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
        if (!control || !control->ryo || !g_pControlPadController || g_tmex_val > 1U || g_pControlPadController->blockedCanControl != 1) {
            return false;
        }
        control = reinterpret_cast<MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
        const auto *const pad = static_cast<const PadCaptureEntry *>(control->padCaptureEntry);
        return (pad->pressedPriority & kActionButton) != 0U;
    };

    switch (event->kind) {
    case MEK_OnEnter:
    case MEK_WhileInside:
        parseEvent();
        event->flags |= 1U;
        return;
    case MEK_OnAction:
        if (actionPressed())
            parseEvent();
        return;
    case MEK_OnEnterOnce:
        eventByte = static_cast<std::uint8_t>(eventByte & static_cast<std::uint8_t>(~mask));
        parseEvent();
        event->flags |= 1U;
        return;
    case MEK_OnActionOnce:
        if (!actionPressed())
            return;
        eventByte = static_cast<std::uint8_t>(eventByte & static_cast<std::uint8_t>(~mask));
        parseEvent();
        event->flags |= 1U;
        return;
    default:
        return;
    }
}

void MapControl_UpdateMapEvents() {
    auto *control = reinterpret_cast<MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
    HLib::Task *const scenarioTask = control->scenarioTask;
    if (!g_mapControlTask)
        return;
    control = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    if (!control)
        return;

    std::int32_t bucket = 0;
    CharacterBucketNode *node = static_cast<CharacterBucketNode *>(control->characterBuckets[0]);
    while (bucket < 13) {
        while (node) {
            Character *const character = node->character;
            if (IsRetainedCharacter(character)) {
                MapEventScenario_Parse(character, scenarioTask);
                control = reinterpret_cast<MapControlData *>(g_mapControlTask ? g_mapControlTask->task_data_ptr : nullptr);
            }
            node = node->next;
        }
        if (++bucket >= 13)
            return;
        node = static_cast<CharacterBucketNode *>(control->characterBuckets[static_cast<std::size_t>(bucket)]);
    }
}

void MapEvent_AllocateComponent(Character *character, bool tokenMapEvent, bool tokenNoMapEvent) {
    if (!tokenMapEvent)
        return;
    (void)AllocateMapEventWork(character);
    if (tokenNoMapEvent) {
        if (CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kMapEventComponent)) {
            component->flags &= ~1U;
        }
    }
}

bool ReadAlternateMapSelection(std::array<std::int32_t, 32> &mapIds) {
    std::uint32_t payloadSize = 0;
    const std::byte *cursor = MAPINFO_FindChunkPayload(SHEN_NAME(MAPE), &payloadSize);
    if (!cursor)
        return false;
    const std::byte *const end = cursor + payloadSize;
    const auto readValue = [&cursor, end](std::int32_t &value) {
        if (static_cast<std::size_t>(end - cursor) < sizeof(value))
            return false;
        std::memcpy(&value, cursor, sizeof(value));
        cursor += sizeof(value);
        return true;
    };

    while (cursor < end) {
        std::int32_t value = -1;
        if (!readValue(value) || value == -1)
            return false;
        bool matchesEntryPoint = static_cast<std::uint32_t>(value) == g_entryPoint;
        while (readValue(value) && value != -1) {
            matchesEntryPoint = matchesEntryPoint || static_cast<std::uint32_t>(value) == g_entryPoint;
        }
        if (value != -1)
            return false;

        std::size_t mapCount = 0;
        while (readValue(value) && value != -1) {
            if (matchesEntryPoint && mapCount + 1 < mapIds.size())
                mapIds[mapCount++] = value;
        }
        if (value != -1)
            return false;
        if (matchesEntryPoint) {
            mapIds[mapCount] = -1;
            return true;
        }
    }
    return false;
}
bool InitializeMapEnvironmentParameters() {
    SCRL_SetLuminanceChannels(0, 0, 0);
    bool luminanceEnabled = false;
    std::uint32_t payloadSize = 0;
    if (const std::byte *const payload = MAPINFO_FindChunkPayload(SHEN_NAME(TXMO), &payloadSize); payload && payloadSize >= sizeof(std::int32_t)) {
        std::int32_t entry = 0;
        std::memcpy(&entry, payload, sizeof(entry));
        if (entry == -1) {
            luminanceEnabled = true;
        } else {
            for (std::uint32_t offset = 0; offset + sizeof(entry) <= payloadSize; offset += sizeof(entry)) {
                std::memcpy(&entry, payload + offset, sizeof(entry));
                if (entry == -1)
                    break;
                if (static_cast<std::uint32_t>(entry) == g_entryPoint) {
                    luminanceEnabled = true;
                    break;
                }
            }
        }
    }
    SCRL_SetLuminanceEnabled(luminanceEnabled);

    bool snowParticleBudgetEnabled = true;
    payloadSize = 0;
    if (const std::byte *const payload = MAPINFO_FindChunkPayload(SHEN_NAME(FOCK), &payloadSize); payload && payloadSize >= sizeof(std::uint32_t)) {
        std::uint32_t flags = 0;
        std::memcpy(&flags, payload, sizeof(flags));
        snowParticleBudgetEnabled = (flags & 1U) != 0U;
    }
    return snowParticleBudgetEnabled;
}

void InitializeMapShadowParameters() {
    g_shadowStrength = 127;
    g_shadowBlackness = 0.5f;
    g_shadowFlag = 0;
    std::uint32_t payloadSize = 0;

    float shadowRange = 14.0f;
    payloadSize = 0;
    if (const std::byte *const payload = MAPINFO_FindChunkPayload(SHEN_NAME(SHRG), &payloadSize); payload && payloadSize >= sizeof(shadowRange)) {
        std::memcpy(&shadowRange, payload, sizeof(shadowRange));
    }
    lightSetShadowRange(std::fmax(0.1f, shadowRange));
}

void ApplyMapPreparationChunk() {
    std::uint32_t payloadSize = 0;
    const std::byte *const payload = MAPINFO_FindChunkPayload(SHEN_NAME(PRER), &payloadSize);
    if (!payload)
        return;
    for (std::uint32_t offset = 0; offset + sizeof(std::int32_t) <= payloadSize; offset += sizeof(std::int32_t)) {
        std::int32_t mapIndex = -1;
        std::memcpy(&mapIndex, payload + offset, sizeof(mapIndex));
        if (mapIndex == -1)
            return;
        if (mapIndex == -2)
            PACK_EnsureAllMapPreparationStorage();
        else
            PACK_EnsureMapPreparationStorage(mapIndex);
    }
}

bool ClipScreenVisibility(const Vector3 &position, float radius, float range) {
    const Vector3 view{position.x * workTransformMatrix.rows[0].x + position.y * workTransformMatrix.rows[1].x + position.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        position.x * workTransformMatrix.rows[0].y + position.y * workTransformMatrix.rows[1].y + position.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        position.x * workTransformMatrix.rows[0].z + position.y * workTransformMatrix.rows[1].z + position.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z};
    const float depthFarEdge = -view.z + radius;
    if (depthFarEdge < 0.0f || -view.z - radius > range)
        return false;
    const Vector2 &scale = prevScreenSpaceToWorldScale;
    const float halfWidth = depthFarEdge * scale.x + radius;
    const float halfHeight = depthFarEdge * scale.y + radius;
    return view.x <= halfWidth && -halfWidth < view.x && view.y <= halfHeight && -halfHeight < view.y;
}

void CLIP_MainCallback(void *value) {
    auto *const character = static_cast<Character *>(value);
    if (!character)
        return;

    bool shadowVisible = false;
    bool hasMotionManager = false;
    bool motionVisibility = false;
    bool inFrustum = false;
    bool withinVisibleDistance = false;
    Vector3 position{};

    if ((character->motionFlags & 4U) != 0U) {
        withinVisibleDistance = true;
        hasMotionManager = MOTM_GetWork(character) != nullptr;
        if (hasMotionManager) {
            if ((character->motionFlags & 0x10U) != 0U) {
                motionVisibility = true;
                const Vector3 viewPosition{character->viewPosition.x, character->viewPosition.y, -character->viewPosition.z};
                if (!g_shouldUseLODVisibilityThisFrame || (character->flags.value & CharacterFlagWithinVisibleDistance) != 0U ||
                    (withinVisibleDistance = shenIsViewSphereVisibleInScreenDepthTable(viewPosition, 0.5f))) {
                    inFrustum = true;
                }
            }
        } else {
            inFrustum = true;
            position = GetPositionForEntity(character->entity.node);
            const bool insideView = shenIsLocalSphereInsideViewCullVolume(position, character->clipAngle);
            if (insideView && g_shouldUseLODVisibilityThisFrame && (character->flags.value & CharacterFlagWithinVisibleDistance) == 0U) {
                withinVisibleDistance = shenIsLocalSphereVisibleInScreenDepthTable(position, character->clipAngle);
            }
            const bool wasInFrustum = (character->motionFlags & 1U) != 0U;
            if (insideView)
                character->motionFlags |= 1U;
            else
                character->motionFlags &= static_cast<std::uint8_t>(~1U);
            if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0U && wasInFrustum != ((character->motionFlags & 1U) != 0U))
                CHAR_LogNPCVisibility(character, (character->motionFlags & 1U) != 0U);
        }
        const bool wasWithinVisibleDistance = (character->motionFlags & 2U) != 0U;
        if (withinVisibleDistance)
            character->motionFlags |= 2U;
        else
            character->motionFlags &= static_cast<std::uint8_t>(~2U);
        if (g_bDebugLogVisibility && (character->flags.value & CharacterFlagHasCpct) != 0U && wasWithinVisibleDistance != ((character->motionFlags & 2U) != 0U))
            CHAR_LogNPCVisibility(character, (character->motionFlags & 2U) != 0U);
    }

    if ((inFrustum || (character->flags.value & 0x200U) != 0U) && (character->flags.value & CharacterFlagActive) != 0U) {
        const float shadowRange = lightGetShadowRange();
        if (hasMotionManager) {
            shadowVisible = motionVisibility && shadowRange > character->viewDistance - character->clipAngle * 2.0f;
        } else {
            shadowVisible = ClipScreenVisibility(position, character->clipAngle * 3.0f, shadowRange);
        }
    }

    if (shadowVisible)
        character->flags.value |= 0x10U;
    else
        character->flags.value &= ~0x10U;
}

void CLIP_Callback(HLib::TaskData *) {
    g_shouldUseLODVisibilityThisFrame = g_occlusionLUTUpdateCount;
    ForEachCewpEntityApplyCallback(0, CLIP_MainCallback);
}

void LoadSceneMapAssets() {


    LOD_Distance_0 = 140.0f;
    LOD_Distance_2 = 140.0f;
    NPCDisplayRadius = 48.0f;
    g_dynamicMapLodSnapshot = {
        .renderEnabledMask = 0xffffffffU,
        .occlusionEnabledMask = 0xffffffffU,
        .clipCellsReadyMask = 0xffffffffU,
        .lodDistance0 = 140.0f,
        .lodDistance2 = 140.0f,
        .npcDisplayRadius = 48.0f,
    };
    for (std::int32_t index = 0; index < 32; ++index) {
        DynamicMapEntry &entry = g_dynamicMapSlots[index];
        entry.renderEnabled = 1;
        entry.clipRuntime = nullptr;
        entry.sourceKind = 0;
        entry.mapAsset = nullptr;
        entry.sceneObjectCount = 0;
        entry.clipCellPointers = nullptr;
        entry.sceneObjectData = nullptr;
        entry.position = {};
        entry.field_44 = 0;
        entry.staticSceneMode = 1;
        entry.debugDrawFlag = 0;
        entry.occlusionEnabled = 1;
        entry.clipMetrics[0] = 0;
        entry.clipMetrics[1] = 0;
        entry.clipMetrics[2] = 0;
        entry.clipMetrics[3] = 0;
        entry.clipMetrics[5] = 0;
        entry.prepClone = nullptr;
        entry.scale = 1.0f;
        entry.lightingOverride01 = 0.0f;
    }
    for (DynamicMapStaticTransform &transform : g_dynamicMapStaticTransforms) {
        transform = {};
        transform.enabled = 1;
        transform.sourceMapSlot = 0x7fff;
        transform.field_18 = 1;
    }
    for (DynamicMapAuxiliary &auxiliary : g_dynamicMapAuxiliary) {
        auxiliary.clipCells = nullptr;
        auxiliary.clipCellCount = 0;
        auxiliary.clipCellsReady = 0;
        auxiliary.pathCellCount = 0;
        auxiliary.pathData = nullptr;
    }

    std::int32_t totalMapPacks = 0;
    std::uint32_t payloadSize = 0;
    if (const std::byte *const mapr = MAPINFO_FindChunkPayload(SHEN_NAME(MAPR), &payloadSize); mapr && payloadSize >= sizeof(totalMapPacks)) {
        std::int32_t value = 0;
        std::memcpy(&value, mapr, sizeof(value));
        totalMapPacks = std::clamp(value, -1, 31);
    }

    std::uint32_t rootAreaId = g_areaId;
    payloadSize = 0;
    if (const std::byte *const mapp = MAPINFO_FindChunkPayload(SHEN_NAME(MAPP), &payloadSize); mapp && payloadSize >= sizeof(rootAreaId)) {
        std::memcpy(&rootAreaId, mapp, sizeof(rootAreaId));
        g_mapAreaOverrideId = rootAreaId;
    }

    char rootDirectory[64]{};
    std::snprintf(rootDirectory, sizeof(rootDirectory), "scene/%02d/%c%c%c%c", g_sceneId, static_cast<char>(rootAreaId), static_cast<char>(rootAreaId >> 8),
        static_cast<char>(rootAreaId >> 16), static_cast<char>(rootAreaId >> 24));

    payloadSize = 0;
    std::uint32_t loadThroughPackTask = 0;
    if (const std::byte *const mapt = MAPINFO_FindChunkPayload(SHEN_NAME(MAPT), &payloadSize); mapt && payloadSize >= sizeof(loadThroughPackTask)) {
        std::memcpy(&loadThroughPackTask, mapt, sizeof(loadThroughPackTask));
    }

    std::array<std::int32_t, 32> alternateMapIds{};
    alternateMapIds.fill(-1);
    if (ReadAlternateMapSelection(alternateMapIds)) {
        LoadAlternateMapModels(rootDirectory, alternateMapIds);
    } else if (totalMapPacks >= 0) {
        if (loadThroughPackTask) {
            const std::int32_t packageCount = totalMapPacks == 0 ? 1 : totalMapPacks;
            for (std::int32_t index = 0; index < packageCount; ++index) {
                char packageName[16]{};
                std::snprintf(packageName, sizeof(packageName), "MPK%02d", index);
                if (g_packTaskData)
                    g_packTaskData->flags = 0;
                (void)PACK_QueueFile(rootDirectory, packageName);
                PACK_Drain();
            }
        } else {
            LoadMapModels(totalMapPacks, rootDirectory);
        }
    }

    MAPINFO_ApplyDynamicMapDisableChunk();
    ApplyMapPreparationChunk();
    ReadSCRL(CurrentControl()->mapInfo);
    LoadSPR_0(MAPINFO_FindChunkIn(CurrentControl()->mapInfo, g_mapInfoSize, SHEN_NAME(SCLM)));
}

static HLib::Task *EnqueueScrollTaskOwner() {
    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(SCRL_Callback_0, 12, 6, SHEN_NAME(SCRL));
    if (!task || task->task_data_ptr)
        return nullptr;
    auto *const data = static_cast<std::byte *>(HLib_AllocHeapMemBlock(0x10, SHEN_NAME(TASK)));
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    if (!data) {
        task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        return nullptr;
    }
    HLib_EnableTokenCheck(data);
    std::memset(data, 0, 0x10);
    return task;
}

void HLTaskFunc_EnqueueMapControlTask(HLib::TaskData *) {
    g_iIsInGame = 1;
    Init_0();
    InitTextureSpace();
    ReleaseCachedSkinningBuffer(g_mainSkinningRoot);
    g_mainSkinningRoot = AllocateCachedSkinningBuffer("Main");
    ReleaseCachedSkinningBuffer(g_sdOverrideSkinningRoot);
    g_sdOverrideSkinningRoot = AllocateCachedSkinningBuffer("SD Override");
    g_FramePacingFactor = g_FramePacingFactor > 0 ? 1 : 0;
    const char areaCharacters[4]{
        static_cast<char>(g_areaId),
        static_cast<char>(g_areaId >> 8),
        static_cast<char>(g_areaId >> 16),
        static_cast<char>(g_areaId >> 24),
    };
    char directoryPath[64];
    std::snprintf(directoryPath, sizeof(directoryPath), "/scene/maps/%c%c%c%c", areaCharacters[0], areaCharacters[1], areaCharacters[2], areaCharacters[3]);
    InitSkyGeometry(g_cycleManagerRuntime.skyGeometryRuntimeData);
    InitSkyGeometry(g_cycleManagerRuntime.skyProbeGeometryRuntimeData);
    ReleaseCachedSkinningBuffer(g_skySkinningRoot);
    ReleaseCachedSkinningBuffer(g_skyProbeSkinningRoot);
    g_skySkinningRoot = 0;
    g_skyProbeSkinningRoot = 0;
    LoadSky(directoryPath);
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(HLTaskFunc_MapControlTask_Callback, 2, 2, SHEN_NAME(CTRL));
    MapControlData *control = nullptr;
    if (task && !task->task_data_ptr) {
        control = static_cast<MapControlData *>(HLib_AllocHeapMemBlock(sizeof(MapControlData), SHEN_NAME(TASK)));
        task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(control);
        if (control) {
            HLib_EnableTokenCheck(control);
            std::memset(control, 0, sizeof(*control));
            std::fill_n(control->cycleMask, sizeof(control->cycleMask), std::byte{0xff});
        }
    }
    if (!control) {
        if (task) {
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
        }
        task = nullptr;
    }
    g_mapControlTask = task;

    control->mapInfo = MAPINFO_Load();

    PPAD_Init();
    PADC_Init();
    COLS_ResetQueryState();
    InitDoorSystemTask();
    g_cm_unk_flag = 1;
    g_cycleTalkSuppressed = 0;
    InitCycleStack();
    g_fGlobalTurningRadius = 0.40000001f;
    nRainState = 0;
    gRainModelPtr = nullptr;
    g_default_turning_box_half_depth_z = 0.0f;
    g_angYoffset = 0.0f;
    g_dynamicTurnCollisionVolumeCount = 0;
    g_steeringIntersectionVolumeCount = 0;
    KDAN_Init();
    PACK_Init();
    if (!DISP_IsOnMainMenu())
        PAUS_Init();
    g_characterGroundDropLimit = 0.07f;
    g_characterCollisionHead = nullptr;
    CACH_Init();
    KUTI_Init();
    control->padCaptureEntry = BindMapControlPADCEntry();
    control->windTask = WIND_Init();
    FOG_InitFog();
    g_defaultLightRadius = 10.0f;
    g_defaultLightMask = 7;
    g_shadowModelCacheCount = 0;
    g_shadowModelCache.fill({});
    g_dbg_draw_enabled = 0;
    g_is_busy_transform = 0;
    g_bCameraVehicleFlag = 0;
    g_vehicleGlobalFlagGate = 1;
    g_cameraOverrideFlipDirectionActive = 0;
    g_fbggRiderMotionState = -1;
    GULL_ClearMapState();
    if (g_mapInfoFileState) {
        FileStatAndProcessIfExists(g_mapInfoFileState);
        ReadFile_0(0, &g_mapInfoFileState->handle);
    }
    if (const std::byte *const end = MAPINFO_FindChunkIn(control->mapInfo, g_mapInfoSize, make_shen_name('E', 'N', 'D', '\0'))) {
        std::uint32_t terminalOffset = 0;
        std::memcpy(const_cast<std::byte *>(end) + 4, &terminalOffset, sizeof(terminalOffset));
    }

    SAVE_Init(0);
    SOND_Init();
    std::uint32_t payloadSize = 0;
    const std::byte *const sndd = MAPINFO_FindChunkPayload(SHEN_NAME(SNDD), &payloadSize);
    SNDD_Parse(reinterpret_cast<HLib::TaskData *>(const_cast<std::byte *>(sndd ? sndd - 8 : nullptr)));

    SCEN_ResetTimedTransitions();
    control->mapEventTask = HLib_EnqueueTaskWithoutParameter(MapEventTask_Callback, 3, 2, kMapEventTaskToken);
    control->scenarioTask = SCEN_Init();
    control->modelLoaderTask = MDLM_Init();
    control->cameraTask = CAME_Init();
    control->filmTask = FILM_Init(11);
    control->mbstTask = MBST_Init();
    UI_GUITasksEnqueue();
    WRN_CtrlWarning_Enqueue();
    if (!DISP_IsOnMainMenu())
        RenderLoadingScreen(0);
    g_maxHandRotationBlend = 0.5f;
    MOTN_ResetMapFileRegistrations();
    g_invalidatedMotionRange = nullptr;
    g_vfPlayerAggroCombat = 1;

    if (g_sceneId == 80 && g_areaId == SHEN_NAME(TITL)) {
        MOTN_ReadMOTNHeader(ReadFile("scene/80/TITL", "M_TITL.BIN", SHEN_NAME(MOTI)));
        SPSS_Init();
    }

    payloadSize = 0;
    const std::byte *const mapState = MAPINFO_FindChunkPayload(SHEN_NAME(MSTS), &payloadSize);
    if (mapState && payloadSize >= sizeof(control->mapRuntimeFlags)) {
        std::memcpy(&control->mapRuntimeFlags, mapState, sizeof(control->mapRuntimeFlags));
    }
    const bool snowParticleBudgetEnabled = InitializeMapEnvironmentParameters();
    FACE_Init();
    LGHT_Initialize(control->mapInfo, g_mapInfoSize);
    InitializeMapShadowParameters();
    EFFE_ResetScriptLightParametersForMap();
    if (!DISP_IsOnMainMenu())
        EnqueueTIME();
    HLib_EnqueueTaskWithoutParameter(WIND_Callback, 8, 2, SHEN_NAME(WIND));
    LCDM_Init();
    SRCH_Init();

    payloadSize = 0;
    const std::byte *const collision = MAPINFO_FindChunkPayload(SHEN_NAME(COLS), &payloadSize);
    COLS_Process(collision ? const_cast<std::byte *>(collision - 8) : nullptr);
    MAPINFO_InitializeSceneParameters(control->mapInfo);

    LoadSceneMapAssets();
    g_skipNavRegionFiltering = 0;
    g_navRegionListHead = nullptr;
    g_navigationRegionCount = 0;
    payloadSize = 0;
    g_navigationCollisionDataTable = MAPINFO_FindChunkPayload(SHEN_NAME(PCDT), &payloadSize);
    if (g_navigationCollisionDataTable) {
        std::uint32_t regionId = 0;
        std::memcpy(&regionId, g_navigationCollisionDataTable, sizeof(regionId));
        navCreateRegion(regionId, nullptr, 0);
    }
    g_locomotionInitState = LocomotionInit::Pending;
    CHRD_InitCurrentMapInfo();
    EFFE_Init();
    g_flmwPlayerDriveActive = 0;
    g_flmwForwardInputPressed = 0;
    g_defaultHpRegenProfile = 0;
    g_activeHpRegenProfile = 0;
    g_playerStats.vfHpRegenerationProfile = 0;
    g_vfRuntimeAccumFightTicks = 0;
    EFFE_SetSnowParticleBudgetEnabled(snowParticleBudgetEnabled);
    SCN3_Process(control->mapInfo);
    control->sceneId = g_sceneId;
    control->areaId = g_areaId;
    control->entryPoint = g_entryPoint;
    control->module = SystemModule::Map;

    S1Save_ApplyPendingMapStartState();
    g_pendingSleepResumeLoad = 0;

    control->objectTask = OBJE_Init();
    control->scrollTask = EnqueueScrollTaskOwner();
    control->clipTask = HLib_EnqueueTaskWithoutParameter(CLIP_Callback, 12, 2, SHEN_NAME(CLIP));
    Rendering_n_Init();
    control->renderTask = DRWP_GetWork();
    control->dwmzTask = DWMZ_Init();
    TRWV_Init();
    if (!DISP_IsOnMainMenu())
        CDTT_Init();
    CSTC_Init();
    MOTF_Init();
}

void MapControlTask_Init(HLib::TaskData *) {
    VFMA_TaskPtr = nullptr;
    if (g_globalFlags[0] == 80U && g_updateRenderFrameMode != 1U) {
        return;
    }
    const AreaIdleStatsProfile &areaStatsProfile = FindAreaIdleStatsProfile(g_areaId);
    g_areaIdleFrameCounterIndex = areaStatsProfile.counterIndex;
    std::fill_n(g_saveDataRelated.begin(), 4, 0U);
    g_moduleInputActivityFlags = 0;
    g_bShouldDraw = 1;
    g_moduleInputSuppressFrames = 0;
    for (FileState &file : g_files)
        ReadFile_0(areaStatsProfile.areaId, &file.handle);
    HLTaskFunc_InternalInit();

    HLTaskFunc_EnqueueMapControlTask(nullptr);
    krepEnqueue(0);
    krepEnqueue(3);
    g_ctrlInitialized = true;
    g_moduleFrameCount = 0;
    (void)PACK_HasValidPack();

    HLib::Task *const moduleTask = CurrentTask;
    if (moduleTask) {
        moduleTask->initCallbackFnPtr = nullptr;
        moduleTask->mainTaskCallbackFnPtr = MODU_Callback;
        moduleTask->autoInitTaskCallbackFnPtr = nullptr;
    }
}

} // namespace shenmue::s1
