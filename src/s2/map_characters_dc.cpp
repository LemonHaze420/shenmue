#include "s2/map_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/map_info.hpp"
#include "shenmue.h"
#include "s2/game_system_dc.hpp"
#include "s2/map_event_transition_dc.hpp"
#include "s2/event_character_dc.hpp"
#include "s2/map_event_character_dc.hpp"
#include "platform/dc/mt7.hpp"
#include "s2/area_jump_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/conversation_dc.hpp"
#include "s2/directory_cache.hpp"
#include "s2/door_control_dc.hpp"
#include "s2/field_collision_dc.hpp"
#include "s2/font.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/lighting_dc.hpp"
#include "s2/map_load_dc.hpp"
#include "s2/model_image_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/screen_film.hpp"
#include "s2/shadow_dc.hpp"
#include "s2/title_dc.hpp"
#include "s2/texture_dc.hpp"
#include "s2/weather_dc.hpp"
#include "s2/window.hpp"
#include <cstdio>
#include <cstring>
#include "s2/area_event.hpp"
#include "s2/scenario_control_dc.hpp"
#include "common/math.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/hlib.hpp"
#include "platform/dc/sdk.hpp"
#include "s2/camera_dc.hpp"
#include "s2/game_save_area.hpp"
#include "s2/input_dc.hpp"
#include "s2/system_modules_dc.hpp"
#include "s2/ui_state_dc.hpp"
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "s2/calendar_dc.hpp"
#include "s2/debug_screen_dc.hpp"
#include "s2/save_screen_state_dc.hpp"
#include "s2/scenario_flags_dc.hpp"
#include "s2/story_state.hpp"
#include <cstdarg>
#endif
#include "s2/model_image_component_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/character_components_dc.hpp"
#include "s2/character_pose_dc.hpp"
#include "s2/scenario_character_query_dc.hpp"
#include "platform/dc/matrix.hpp"
#include "s2/pad_control_dc.hpp"
#include "s2/player_state_dc.hpp"
#include "s2/character_yaw_dc.hpp"
#include "s2/math.hpp"
#define SHENMUE_CHARACTER_POSE_NULL 0

namespace shenmue {
namespace s2 {

struct PadControlEntry;

void CleanupMapControlCharacters();
void ReadMapCharacterTable(const char *name, std::int32_t entry);
void SetDirtAngle(std::int32_t angle);
void InitializeMapModelNullsub();
void InitializeMapPlayerNullsub();
HLib::Task *UNUSED_Task_Enqueue(std::uint8_t nextFunctionIndex);

void EventBase_Wrapper(void *mapInfo);
void EventBase_Wrapper(void *mapInfo);
void InitializeMapCharacterNullsub();
void *SceneMapinfoReader(std::int32_t actNumber, shen_name areaName);
void MAPINFORead(void *mapInfo);
HLib::Task *g_mapControlTask;
std::int32_t g_worldDrawingEnabled = 0;
void ResetMapRenderState();
HLib::Task *Queue_PrePadControlTask(std::int8_t index);
void Queue_PadControlTask(std::int8_t index);
HLib::Task *Queue_ButtonMediation(std::uint8_t index);
void plymSetDefaultFlags();
HLib::Task *npc_Callback_Enqueue(std::int8_t index, std::int32_t act, shen_name area);
void ExecPause();
void EnqueueESOP();
PadControlEntry *AllocatePadControlEntry();
PadControlEntry *BindPadControlEntry(PadControlEntry *entry);
void SetupDefaultFogParams();

#if !defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void npcInit();
#endif

void ResetModelDrawState();
void resetHandcuffTask();
void telCodeInit();
bool SNDP_SoundProgramReader(const MapInfoChunk *chunk, std::int32_t act, shen_name area);
void SNDD_Reader(const MapInfoChunk *chunk, std::int32_t act, std::int32_t entry);
void KanaFontLoad(const char *name);
HLib::Task *Queue_Callback_MapEventTaskEntry(std::uint8_t index);
HLib::Task *ScenarioTaskEntry_Enqueue(std::int8_t index);
HLib::Task *Queue_CameraCtrl(std::uint8_t index);
void WRN_CtrlWarning_Enqueue();
void Queue_ADV_CtrlAdvice();
void GUI_Tasks_Enqueue();
void InitTIMR();
void ST_LoadTitleFadeIn();
void AllocateMotionBanks(std::int32_t actNumber, std::uint32_t map);
void FOCK_Reader(const MapInfoChunk *chunk);
void SetupRefboard();
void initTimeProgram();
void initCtrlSystemManage();
void initCtrlAreaJump();
std::int32_t initlcdTask();
void init_search_ctrl();
void authResetLoadFlag();
void SCEX_Reader(const MapInfoChunk *chunk);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void tree_Callback_Enqueue(char index);
#else
void tree_Callback_Enqueue(std::uint8_t index);
#endif
void blurInit();
void fightInit(std::int32_t mode);
void Queue_EI_Startup_EventFade1();
HLib::Task *QueueObjectDrawTask(std::uint8_t index);
HLib::Task *Queue_CtrlAir(std::uint8_t index);
void CharDispCheck_Callback(void *data);
HLib::Task *CharDraw_Human_Object_Enqueue(std::uint8_t index);
std::int32_t Queue_ccCharaColiTask(std::int32_t index);
void InitEffectControlTaskData();
void EF_Callback_EffectCtrl_Enqueue();
void InitEffectPoints();
void ST_LoadTitleFade();
void Queue_mot_Callback_MotFileManageTaskEntry(std::uint8_t index);
std::int32_t Queue_FLAG_CheckTask(shen_name area);
void qron_task_Enqueue();
void elevator_task_Enqueue();
void InitMagicRoomPtrs();
void InitializeMazeRuntime(std::int32_t act, shen_name area);
void InitVFM();
void NPC_Route_Control();

// @verified - d
void MapControlTask_Callback(void *data) {
    if (static_cast<MapControlData *>(data)->pendingCharacterCleanupCount)
        CleanupMapControlCharacters();
}

// @verified - d
void MapControlTask_Enqueue(std::int8_t nextFunctionIndex) {
    char directory[64];
    std::uint32_t name[2];
    std::int32_t i;
    CharacterNode *node;
    ResetMapRenderState();
    InitializeMapModelNullsub();
    ResetDirectoryCache(0);
    name[0] = g_currentMap;
    name[1] = 0;
    std::sprintf(directory, "/scene/%02d/%s", g_actNumber, reinterpret_cast<char *>(name));
    AllocateDIRE(directory, 0, 30);
    g_mapControlTask = HLib::EnqueueTaskWithParameter(MapControlTask_Callback, nextFunctionIndex, 2, sizeof(MapControlData), MAP_CONTROL_TASK);
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    control->mapEventTask = 0;
    control->scenarioTask = 0;
    control->cameraTask = 0;
    control->retainedTaskA = 0;
    control->retainedTaskB = 0;
    control->modelManagerTask = 0;
    control->filmTask = 0;
    control->retainedStateA = 0;
    control->retainedStateB = 0;
    control->clip = 0;
    control->retainedTaskC = 0;
    control->retainedTaskD = 0;
    control->retainedStateC = 0;
    control->retainedStateD = 0;
    control->unusedTask = 0;
    control->mapInfo = SceneMapinfoReader(g_actNumber, g_currentMap);
    Queue_PrePadControlTask(2);
    Queue_PadControlTask(2);
    Queue_ButtonMediation(2);
    plymSetDefaultFlags();
    InitializeMapPlayerNullsub();
    CVS_CtrlConversTask_Enqueue(1);
    PakLoadTaskEntry_Enqueue(3, 0);
    npc_Callback_Enqueue(8, g_actNumber, g_currentMap);
    if (!g_gamePauseState)
        ExecPause();
    for (i = 0; i < 13; ++i)
        control->characterBuckets[i] = 0;
    control->pendingCharacterCleanupCount = 0;
    control->ryo = 0;
    control->flags = 0;
    control->statusFlags = 0;
    SetDirtAngle(0);
    node = control->characterControls;
    for (i = 0; i < 32; ++i, ++node) {
        node->parent = 0;
        node->position.x = node->position.y = node->position.z = 0.0f;
        node->rotation.x = node->rotation.y = node->rotation.z = 0;
        node->layer = 0;
        node->collision = 0;
    }
    for (i = 0; i < 32; ++i)
        reinterpret_cast<std::uint32_t *>(control->retainedAreaData)[i] = 0;
    std::memset(control->retainedCharacterState, 0xff, sizeof(control->retainedCharacterState));
    EnqueueESOP();
    InitializeMapCharacterNullsub();
    control->padControl = AllocatePadControlEntry();
    BindPadControlEntry(static_cast<PadControlEntry *>(control->padControl));
    control->retainedTaskD = QueueWindowTask(2);
    SetupDefaultFogParams();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    LoadShadowSprite();
#else
    npcInit();
#endif
    ResetModelDrawState();
    resetHandcuffTask();
    telCodeInit();
    MAPINFORead(control->mapInfo);
    SM_Init_Enqueue(0);
    QueueSoundProgramControl();
    if (!SNDP_SoundProgramReader(FindNativeChunk("SNDP", static_cast<MapInfoChunk *>(control->mapInfo)), g_actNumber, g_currentMap))
        SNDD_Reader(FindNativeChunk("SNDD", static_cast<MapInfoChunk *>(control->mapInfo)), g_actNumber, g_entryPoint);
    KanjiFontLoad("DC_KANJI.FON");
    KanaFontLoad("DC_KANA.FON");
    control->mapEventTask = Queue_Callback_MapEventTaskEntry(3);
    control->scenarioTask = ScenarioTaskEntry_Enqueue(3);
    control->modelManagerTask = EnqueueModelManagerTask(3);
    control->cameraTask = Queue_CameraCtrl(7);
    control->filmTask = Queue_SCF_Callback_ScreenFilmTaskEntry(13);
    control->unusedTask = UNUSED_Task_Enqueue(2);
    WRN_CtrlWarning_Enqueue();
    Queue_ADV_CtrlAdvice();
    GUI_Tasks_Enqueue();
    InitTIMR();
    InitST_TitleFunc();
    FLDD_ReadFieldData(FindNativeChunk("FLDD", static_cast<MapInfoChunk *>(control->mapInfo)));
    if (!g_gamePauseState)
        ST_LoadTitleFadeIn();
    SetLoadingDrawFlags(1);
    AllocateMotionBanks(g_actNumber, g_currentMap);
    const MapInfoChunk *const state = FindNativeChunk("MSTS", static_cast<MapInfoChunk *>(control->mapInfo));
    if (state) {
        std::uint32_t &destination = control->statusFlags;
        destination = *reinterpret_cast<const std::uint32_t *>(state + 1);
    }
    MSTS_Reader(0, 0, 0);
    TXMO_Reader(FindNativeChunk("TXMO", static_cast<MapInfoChunk *>(control->mapInfo)), g_entryPoint);
    FOCK_Reader(FindNativeChunk("FOCK", static_cast<MapInfoChunk *>(control->mapInfo)));
    InitializeMapLighting();
    MVTR_Reader();
    SHRG_Reader(FindNativeChunk("SHRG", static_cast<MapInfoChunk *>(control->mapInfo)));
    SetupRefboard();
    if (!g_gamePauseState)
        initTimeProgram();
    initCtrlSystemManage();
    initCtrlAreaJump();
    initlcdTask();
    init_search_ctrl();
    authResetLoadFlag();
    SCEX_Reader(FindNativeChunk("SCEX", static_cast<MapInfoChunk *>(control->mapInfo)));
    control->retainedMapState = 0;
    Mapinfo_MAP_Reader(g_actNumber, g_currentMap, control->mapInfo);
    drInitEnter_Enqueue(FindNativeChunk("DOOR", static_cast<MapInfoChunk *>(control->mapInfo)));
    tree_Callback_Enqueue(8);
    ReadMapCharacterTable("CHRD", g_entryPoint);
    QueueWeatherExecution(FindNativeChunk("WTHR", static_cast<MapInfoChunk *>(control->mapInfo)));
    blurInit();
    fightInit(0);
    WaitForNextTask_0(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->filmTask, 1, 0);
    if (FindNativeChunk("EVFD", static_cast<MapInfoChunk *>(control->mapInfo)))
        Queue_EI_Startup_EventFade1();
    EventBase_Wrapper(control->mapInfo);
    SetUnusedFlag();
    control->retainedTaskA = QueueObjectDrawTask(13);
    control->retainedTaskB = Queue_CtrlAir(13);
    control->clip = HLib::EnqueueTaskWithoutParameter(CharDispCheck_Callback, 13, 2, CHARACTER_DISPLAY_TASK);
    control->retainedTaskC = CharDraw_Human_Object_Enqueue(13);
    Queue_ccCharaColiTask(12);
    InitEffectControlTaskData();
    EF_Callback_EffectCtrl_Enqueue();
    InitEffectPoints();
    if (!g_gamePauseState)
        ST_LoadTitleFade();
    Queue_mot_Callback_MotFileManageTaskEntry(13);
    dc::sdk::SetMT7VertexColourType(1);
    Queue_FLAG_CheckTask(g_currentMap);
    qron_task_Enqueue();
    elevator_task_Enqueue();
    InitMagicRoomPtrs();
    InitializeMazeRuntime(g_actNumber, g_currentMap);
    InitVFM();
    NPC_Route_Control();
    RetireLoadingDraw();
}

std::int32_t IsMapControlTaskActive() {
    return HLib::HasActiveTaskCallback(g_mapControlTask);
}

// @verified - d
MapInfoChunk *GetCurrentMapInfo() {
    return static_cast<MapInfoChunk *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->mapInfo);
}

// @verified - d
std::int32_t ctrlGetActive() {
    if (reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->flags & 1U)
        return 1;
    return 0;
}

// @verified - d
SystemModule UpdateCurrentSceneInfoFromTask(std::int32_t &actNumber, shen_name &areaId, std::int32_t &entryId) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    actNumber = control->actNumber;
    areaId = control->areaId;
    entryId = control->entryId;
    return control->module;
}

// @verified - d
void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t actNumber, shen_name areaId, std::int32_t entryId) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    control->module = module;
    control->actNumber = actNumber;
    control->areaId = areaId;
    control->entryId = entryId;
    control->flags |= 1U;
}

// @verified - d
void SetMapControlStatusFlag(std::int32_t bit, std::int32_t enabled) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::uint32_t flags = control->statusFlags;
    if (enabled)
        flags |= 1U << bit;
    else
        flags &= ~(1U << bit);
    control->statusFlags = flags;
#else
    const std::uint32_t shift = static_cast<std::uint32_t>(bit);
    const std::uint32_t mask = bit < 0 ? ((shift & 31U) ? 1U >> (32U - (shift & 31U)) : 0U) : 1U << (shift & 31U);
    if (enabled)
        control->statusFlags |= mask;
    else
        control->statusFlags &= ~mask;
#endif
}

// @verified - d
std::uint32_t GetMapControlStatusFlags() {
    return reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->statusFlags;
}

// @verified - d
PadControlEntry *GetMapPadControlEntry() {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return static_cast<PadControlEntry *>(control->padControl);
}

std::byte *GetMapRetainedCharacterState() {
    return static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->retainedCharacterState;
}

// @verified - d
CharacterNode *GetCharacterControlSlot(std::int32_t index) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    return &control->characterControls[index];
}

// @verified - d
std::int32_t GetCharacterControlSlotIndex(const CharacterNode *node) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uintptr_t address = reinterpret_cast<std::uintptr_t>(node);
    const std::uintptr_t first = reinterpret_cast<std::uintptr_t>(&control->characterControls[0]);
    if (address < first || address >= first + sizeof(control->characterControls))
        return -1;
    const std::uint32_t product = static_cast<std::uint32_t>(address - first) * 0x38e38e39U;
    std::int32_t signedProduct;
    std::memcpy(&signedProduct, &product, sizeof(signedProduct));
    return signedProduct >> 2;
}

Character *SetMapControlRyo(Character *character) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const previous = control->ryo;
    control->ryo = character;
    return previous;
}

// @verified - d
Character *AddCharacterHandler(std::int32_t type) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    HLib::Task *task;
    Character *const character = EnqueueCharacterHandler(characterHandlerIndices[type], &task);
    if (!character)
        return 0;
    const std::uint32_t bucket = reinterpret_cast<std::uintptr_t>(character) % 13U;
    CharacterMapEntry *const entry = static_cast<CharacterMapEntry *>(HLib::AllocateTaskData(sizeof(CharacterMapEntry), TASK_CHAR));
    entry->next = control->characterBuckets[bucket];
    control->characterBuckets[bucket] = entry;
    entry->task = task;
    entry->character = character;
    return character;
}

// @verified - d
HLib::Task *GetCharacterHandlerTask(Character *character) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const std::uint32_t bucket = reinterpret_cast<std::uintptr_t>(character) % 13U;
    for (CharacterMapEntry *entry = control->characterBuckets[bucket]; entry; entry = entry->next) {
        if (entry->character == character)
            return entry->task;
    }
    return 0;
}

// @verified - d
void ResetCharacterScenarioBinding(MapControlData *control, Character *character) {
    UnassignScenarioCharacter(control->scenarioTask, character);
}

// @verified - d
void IncrementPendingCharacterCleanup() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    ++control->pendingCharacterCleanupCount;
}

// @verified - d
void CleanupMapControlCharacters() {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::int32_t bucket = 0; bucket < 13; ++bucket) {
        CharacterMapEntry **link = &control->characterBuckets[bucket];
        while (*link) {
            CharacterMapEntry *const entry = *link;
            const std::int32_t valid = IsCharacterValid(entry->character);
            if (!valid) {
                ResetCharacterScenarioBinding(control, entry->character);
                if (entry->character == control->ryo)
                    control->ryo = 0;
                FreeModelComponents(entry->character);
                HLib::DestroyTaskInternal(entry->task);
                *link = entry->next;
                HLib::FreeTaskData(entry);
                --control->pendingCharacterCleanupCount;
            } else
                link = &entry->next;
        }
    }
}

// @verified - d
std::int32_t ForEachLiveCharacterInMapOrder(void *context, std::int32_t (*callback)(Character *, void *), std::uint32_t flags) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::int32_t bucket = 0; bucket < 13; ++bucket) {
        for (CharacterMapEntry *entry = control->characterBuckets[bucket]; entry; entry = entry->next) {
            if ((flags & 1U) == 0 && !IsCharacterValid(entry->character))
                continue;
            if (!callback(entry->character, context))
                return 0;
        }
    }
    return 1;
}

// @verified - d
bool BeginMapCharacterIteration(MapCharacterIterator *iterator, std::uint32_t flags) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (std::int32_t bucket = 0; bucket < 13; ++bucket) {
        for (CharacterMapEntry *entry = control->characterBuckets[bucket]; entry; entry = entry->next) {
            if ((flags & 1U) == 0 && !IsCharacterValid(entry->character))
                continue;
            iterator->flags = flags;
            iterator->bucketIndex = bucket;
            iterator->entry = entry;
            iterator->character = entry->character;
            return true;
        }
    }
    iterator->entry = 0;
    iterator->character = 0;
    return false;
}

// @verified - d
bool NextMapCharacter(MapCharacterIterator *iterator) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    CharacterMapEntry *entry = iterator->entry;
    if (!entry)
        return false;
    entry = entry->next;
scan:
    while (entry) {
        if ((iterator->flags & 1U) || IsCharacterValid(entry->character)) {
            iterator->entry = entry;
            iterator->character = entry->character;
            return true;
        }
        entry = entry->next;
    }
    if (++iterator->bucketIndex <= 12) {
        entry = control->characterBuckets[iterator->bucketIndex];
        goto scan;
    }
    iterator->entry = 0;
    iterator->character = 0;
    return false;
}

// @verified - d
bool IsCharacterInCurrentMap(const Character *character) {
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    if (character)
        for (std::int32_t bucket = 0; bucket < 13; ++bucket)
            for (const CharacterMapEntry *entry = control->characterBuckets[bucket]; entry; entry = entry->next)
                if (IsCharacterValid(entry->character) && character == entry->character)
                    return true;
    return false;
}

// @verified - d
void KillCharactersByModelName(shen_name name, std::int32_t mode) {
    MapCharacterIterator iterator;
    BeginMapCharacterIteration(&iterator, 0);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    while (iterator.character) {
        Character *const character = iterator.character;
        if (GetCharacterResourceOwner(character) == name)
            MarkCharacterForCleanup(character, mode);
        NextMapCharacter(&iterator);
    }
#else
    while (iterator.character) {
        if (GetCharacterResourceOwner(iterator.character) == name)
            MarkCharacterForCleanup(iterator.character, mode);
        NextMapCharacter(&iterator);
    }
#endif
}

// @verified - d
void ReadMapCharacterTable(const char *name, std::int32_t entry) {
    const MapControlData *const control = reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    const MapInfoChunk *const chunk = FindNativeChunk(name, static_cast<MapInfoChunk *>(control->mapInfo));
#if SHENMUE_S2_GCC295 && defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    union {
        shen_name packed;
        char characters[4];
    } allocationName;
    allocationName.characters[0] = name[0];
    allocationName.characters[1] = name[1];
    allocationName.characters[2] = name[2];
    allocationName.characters[3] = name[3];
    LoadCharacters(chunk + 1, entry, allocationName.packed);
#else
    LoadCharacters(chunk + 1, entry, shen_name(make_shen_name(name[0], name[1], name[2], name[3])));
#endif
}

// @verified - d
void SetDirtAngle(std::int32_t angle) {
    static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->sunLocation = angle;
}

// @verified - d
std::int32_t GetDirtAngle() {
    return reinterpret_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->sunLocation;
}

// @verified - d
void InitializeMapModelNullsub() {
}

void vfActorMotionNullsub(CharacterNode *) {
}

void vfActorMotionNullsubAlt() {
}

std::int32_t vfStageObjectReturnZero(void *, std::byte *) {
    return 0;
}

// @verified - d
void InitializeMapPlayerNullsub() {
}

std::int32_t InitializeMapPlayerReturnZero() {
    return 0;
}

void InitializeMapPlayerNullsubAlt() {
}

void vfFightNullsub2() {
}

// @verified - d
HLib::Task *UNUSED_Task_Enqueue(std::uint8_t) {
    return 0;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void UNUSED_TaskNullsub0() {
}

void UNUSED_TaskNullsub1() {
}

std::int32_t UNUSED_TaskNullsub2() {
    return 0;
}

void UNUSED_TaskNullsub3() {
}
#endif

} // namespace s2
} // namespace shenmue

#endif
