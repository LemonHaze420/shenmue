#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "s2/character_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/system_modules.hpp"

#define SHENMUE_MAP_CONTROL_LAYOUT_NAME_(line) ShenmueMapControlLayoutLayoutCheck##line
#define SHENMUE_MAP_CONTROL_LAYOUT_NAME(line) SHENMUE_MAP_CONTROL_LAYOUT_NAME_(line)
#define SHENMUE_MAP_CONTROL_LAYOUT(condition) typedef char SHENMUE_MAP_CONTROL_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {
using ::shenmue::shen_name;

struct CharacterMapEntry {
    CharacterMapEntry *next;
    Character *character;
    HLib::Task *task;
};
struct MapCharacterIterator {
    std::uint32_t flags;
    std::int32_t bucketIndex;
    CharacterMapEntry *entry;
    Character *character;
};
struct MapEventComponentState {
    std::uint32_t flags, currentRegion, mode;
    std::uint8_t enabledRegions[32];
};
SHENMUE_MAP_CONTROL_LAYOUT(sizeof(MapEventComponentState) == 0x2c);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapEventComponentState, enabledRegions) == 0x0c);
struct MapControlData {
    Character *ryo;
    void *padControl;
    HLib::Task *mapEventTask, *scenarioTask, *cameraTask, *retainedTaskA, *retainedTaskB, *modelManagerTask, *filmTask;
    std::uint32_t retainedStateA, retainedStateB;
    void *clip;
    HLib::Task *retainedTaskC, *retainedTaskD;
    std::uint32_t retainedStateC, retainedStateD;
    HLib::Task *unusedTask;
    void *mapInfo;
    std::uint32_t retainedMapState;
    std::byte retainedAreaData[0x80];
    std::uint32_t flags;
    shen_name areaId;
    std::int32_t actNumber, entryId;
    SystemModule module;
    std::uint32_t statusFlags;
    CharacterNode characterControls[32];
    CharacterMapEntry *characterBuckets[13];
    std::uint32_t pendingCharacterCleanupCount;
    std::byte retainedCharacterState[0x20];
    std::int32_t sunLocation;
};
SHENMUE_MAP_CONTROL_LAYOUT(sizeof(CharacterMapEntry) == 12);
SHENMUE_MAP_CONTROL_LAYOUT(sizeof(MapCharacterIterator) == 16);
SHENMUE_MAP_CONTROL_LAYOUT(sizeof(MapControlData) == 0x5c0);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, mapEventTask) == 8);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, modelManagerTask) == 0x1c);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, clip) == 0x2c);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, mapInfo) == 0x44);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, retainedAreaData) == 0x4c);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, flags) == 0xcc);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, areaId) == 0xd0);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, actNumber) == 0xd4);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, entryId) == 0xd8);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, module) == 0xdc);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, statusFlags) == 0xe0);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, characterControls) == 0xe4);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, characterBuckets) == 0x564);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, pendingCharacterCleanupCount) == 0x598);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, retainedCharacterState) == 0x59c);
SHENMUE_MAP_CONTROL_LAYOUT(offsetof(MapControlData, sunLocation) == 0x5bc);
extern HLib::Task *g_mapControlTask;
extern std::int32_t g_worldDrawingEnabled;
#define MAP_CONTROL_TASK shen_name(SHEN_NAME4('C', 'T', 'R', 'L'))
#define CHARACTER_DISPLAY_TASK shen_name(SHEN_NAME4('C', 'L', 'I', 'P'))
std::int32_t ctrlGetActive();
std::int32_t SetAreaFogParams();
std::int32_t mapSetupLighting(std::int32_t enabled);
std::uint32_t GetMapControlStatusFlags();
std::int32_t GetDirtAngle();
SystemModule UpdateCurrentSceneInfoFromTask(std::int32_t &actNumber, shen_name &areaId, std::int32_t &entryId);
void MapControlTask_Enqueue(std::int8_t nextFunctionIndex);
void MapControlTask_Callback(void *data);
void ReadMapCharacterTable(const char *name, std::int32_t entry);
void SetDirtAngle(std::int32_t angle);
void EventBase_Wrapper(void *mapInfo);
struct PakGlobalData;
CharacterNode *GetCharacterControlSlot(std::int32_t index);
HLib::Task *GetCharacterHandlerTask(Character *character);
bool BeginMapCharacterIteration(MapCharacterIterator *iterator, std::uint32_t flags);
bool NextMapCharacter(MapCharacterIterator *iterator);
bool IsCharacterInCurrentMap(const Character *character);
std::int32_t ForEachLiveCharacterInMapOrder(void *context, std::int32_t (*callback)(Character *, void *), std::uint32_t flags);
void IncrementPendingCharacterCleanup();
void ResetCharacterScenarioBinding(MapControlData *control, Character *character);
void UnassignScenarioCharacter(HLib::Task *scenarioTask, Character *character);
Character *scenFindChara(HLib::Task *scenarioTask, shen_name name);
void KillCharactersByModelName(shen_name name, std::int32_t mode);
void CleanupMapControlCharacters();
void ReleasePackageModelImages(HLib::Task *modelManagerTask, PakGlobalData *package);
void SetMapEventRegionEnabled(MapEventComponentState *state, std::uint32_t region, std::int32_t enabled);
void MEVW_Callback(Character *character, void *data);
void AllocateMapEventComponent(Character *character);
void ParseMapEv(Character *character, std::int32_t enabled);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MAP_CONTROL_LAYOUT_NAME
#undef SHENMUE_MAP_CONTROL_LAYOUT_NAME_
#undef SHENMUE_MAP_CONTROL_LAYOUT

#endif
