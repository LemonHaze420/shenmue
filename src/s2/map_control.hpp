#pragma once

#include "s2/hlib.hpp"
#include "s2/system_modules.hpp"
#include "common/types.hpp"
#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2 {
struct Character;

inline constexpr shen_name mapEventComponentToken{SHEN_NAME(MEVW)};
inline constexpr shen_name TASK_CTRL{SHEN_NAME(CTRL)}, TASK_CLIP{SHEN_NAME(CLIP)};
using MapEventCallback = void (*)(std::uintptr_t argument);

#pragma pack(push, 4)
struct MapEventCallbackEntry {
    MapEventCallback callback;
    std::int32_t eventId;
    std::uint32_t reserved;
    std::uintptr_t argument;
};

struct MapEventComponentState {
    std::uint32_t flags;
    std::uint32_t currentRegion;
    std::uint32_t mode;
    std::array<std::uint8_t, 32> enabledRegions;
};
#pragma pack(pop)

static_assert(sizeof(MapEventCallbackEntry) == 0x18);
static_assert(offsetof(MapEventCallbackEntry, eventId) == 0x08);
static_assert(offsetof(MapEventCallbackEntry, argument) == 0x10);
static_assert(sizeof(MapEventComponentState) == 0x2c);
static_assert(offsetof(MapEventComponentState, flags) == 0x00);
static_assert(offsetof(MapEventComponentState, currentRegion) == 0x04);
static_assert(offsetof(MapEventComponentState, mode) == 0x08);
static_assert(offsetof(MapEventComponentState, enabledRegions) == 0x0c);

struct MapEventTransition {
    std::int32_t eventId;
    shen_name areaId;
    std::int32_t entryId;
    Vector3 position;
    std::int16_t yaw;
    std::uint16_t reserved1a;
    std::uint32_t scenarioFlags;
};

struct MapEventAreaDefinition {
    shen_name areaId;
    std::uint32_t reserved04;
    const MapEventTransition *transitions;
    std::uint32_t startupMode;
    char startupPayload[20];
};

static_assert(sizeof(MapEventTransition) == 0x20);
static_assert(offsetof(MapEventTransition, position) == 0x0c);
static_assert(offsetof(MapEventTransition, scenarioFlags) == 0x1c);
static_assert(sizeof(MapEventAreaDefinition) == 0x28);
static_assert(offsetof(MapEventAreaDefinition, transitions) == 0x08);
static_assert(offsetof(MapEventAreaDefinition, startupPayload) == 0x14);

#pragma pack(push, 1)
struct CharacterControlSlot {
    std::uint64_t field00;
    std::uint32_t field08;
    std::uint64_t field0c;
    std::uint32_t field14;
    std::uint64_t field18;
    std::uint32_t field20;
    std::uint64_t field24;
};
#pragma pack(pop)

static_assert(sizeof(CharacterControlSlot) == 0x2c);

#pragma pack(push, 1)
struct MapControlData {
    void *ryo;
    void *padControl;
    HLib::Task *mapEventTask;
    HLib::Task *scenarioTask;
    HLib::Task *cameraControlTask;
    HLib::Task *objectTask;
    HLib::Task *airTask;
    HLib::Task *modelManagerTask;
    HLib::Task *screenFilmTask;
    HLib::Task *characterDisplayTask;
    HLib::Task *characterDrawTask;
    HLib::Task *windowTask;
    void *unknown60;
    void *unknown68;
    void *mapInfo;
    void *unknown78;
    std::byte mapRuntimeData[0x100];
    std::uint32_t flags;
    shen_name areaId;
    std::int32_t actNumber;
    std::int32_t entryId;
    SystemModule module;
    std::uint32_t statusFlags;
    std::array<CharacterControlSlot, 32> characterControlTable;
    std::array<void *, 13> characterTable;
    std::uint32_t pendingCharacterCleanupCount;
    std::int64_t currentCharacter;
    std::int64_t field78c;
    std::int64_t field794;
    std::int64_t field79c;
    std::int32_t sunLocation;
};
#pragma pack(pop)

static_assert(offsetof(MapControlData, ryo) == 0x00);
static_assert(offsetof(MapControlData, scenarioTask) == 0x18);
static_assert(offsetof(MapControlData, mapInfo) == 0x70);
static_assert(offsetof(MapControlData, flags) == 0x180);
static_assert(offsetof(MapControlData, statusFlags) == 0x194);
static_assert(offsetof(MapControlData, characterControlTable) == 0x198);
static_assert(offsetof(MapControlData, characterTable) == 0x718);
static_assert(offsetof(MapControlData, pendingCharacterCleanupCount) == 0x780);
static_assert(sizeof(MapControlData) == 0x7a8);
static_assert(offsetof(MapControlData, characterDisplayTask) == 0x48);
static_assert(offsetof(MapControlData, characterDrawTask) == 0x50);

void GameSys(void *taskData);
Character *ctrlGetRootNode();
MapControlData *SetMapControlStatusFlag(std::uint8_t bit, std::int32_t enabled);
void ctrlSetKillAndIncrementCounter(Character *character, std::uint32_t mode);
bool IsCharacterInCurrentMap(const Character *character);
void KillCharactersByModelName(shen_name owner, std::uint32_t mode);
void CleanupMapControlCharacters();
extern std::uint32_t g_fockEnabled;
void FOCK_Reader(const void *chunk);
void MapControlTask_Enqueue(std::uint8_t nextFunctionIndex);
void shenQueueChangeModuleAndScene(SystemModule module, std::int32_t actNumber, shen_name areaId, std::int32_t entryId);
HLib::Task *Queue_Callback_MapEventTaskEntry(std::uint8_t nextFunctionIndex);
void MapEventTaskEntry_Callback(void *taskData);
void Exec_MapEventSceneChange_Callback(void *taskData);
bool AttachMapEventComponent(Character *character);
MapEventComponentState *ParseMapEv(Character *character, std::int32_t enabled);
std::uint8_t EnableScenarioCharacterDisplay(shen_name character);
std::uint8_t DisableScenarioCharacterDisplay(shen_name character);
void EnsureScenarioCharacterDraw(shen_name character);
void ToggleScenarioCharacterFlag(shen_name character);
MapEventComponentState *SetCharacterMapEventRegionEnabled(shen_name character, std::uint32_t region, std::uint8_t enabled);
std::uint64_t IsCharacterMapEventRegionEnabled(shen_name character, std::uint32_t region);
std::uint64_t GetCharacterMapEventRegion(shen_name character);
void SetMapEventCallbackTable(const MapEventCallbackEntry *callbacks);
std::int32_t GetShenmue1SaveAreaId(shen_name areaId) noexcept;
const MapEventAreaDefinition *GetMapEventAreaDefinitions();
const MapEventAreaDefinition &GetCurrentMapEventAreaDefinition();
const MapEventTransition *FindCurrentMapEventTransition(std::int32_t eventId);
bool GameSysWasStarted();
void gameSysInit();
std::uint64_t gameSysGetNeedsReset();
void RequestGameSystemReset();
bool IsWorldDrawingEnabled();
extern bool g_worldDrawingEnabled;
bool GameMapWasLoaded();
shen_name GetAreaDefinitionMapNameForCurrentMap();

struct MT7Node;
struct QronFloorProp {
    shen_name name;
    std::uint32_t floorMasks[4];
    std::int32_t aboveEnabled;
    std::int32_t currentEnabled;
    std::int32_t belowEnabled;
    MT7Node *node;
};
using QronFloorCallback = void (*)(Vector3 *position, std::uint32_t floor);
struct QronFloorDescriptor {
    QronFloorCallback callback;
    QronFloorProp *props;
};
static_assert(sizeof(QronFloorProp) == 0x28 && offsetof(QronFloorProp, node) == 0x20);
static_assert(sizeof(QronFloorDescriptor) == 0x10);
bool SetQronFloorDescriptor(const QronFloorDescriptor *descriptor);
std::uint32_t GetQronCurrentFlags() noexcept;
std::int32_t GetQronFloorAtPosition(const Vector3 &position) noexcept;
float GetQronFloorHeight(std::int32_t floor) noexcept;
float GetQronTemporaryFloorHeight() noexcept;
float FindQronIntersectionHeight(const Vector3 &position, float maximumY, std::int32_t floor);
void SetQronEntryFloor(std::int32_t floor) noexcept;
bool SetQronFloor(std::int32_t floor);
bool RepositionPlayerForQronFloor(std::int32_t floor);
inline constexpr shen_name TASK_QRON{SHEN_NAME(QRON)};
HLib::Task *QueueQronElevatorSceneChange(shen_name area, std::int32_t entry, std::int16_t floor, const Vector3 &position, std::int32_t yaw);
void ResetQronSceneChange() noexcept;
std::uint64_t GetQronNextStream() noexcept;


bool SetQronMapLightingEnabled(std::int32_t enabled) noexcept;
std::int32_t ResolveQronEntryLocation(Vector3 &position, std::int32_t &yaw) noexcept;

extern HLib::Task *g_mapControlTask;
extern shen_name g_currentMap;
extern std::int32_t g_actNumber;
extern std::int32_t g_entryPoint;
extern std::int32_t g_currentShenmue1SaveAreaId;

void ReleaseMapControlOwners();
std::int32_t ctrlGetActive();
struct PadControlEntry;
PadControlEntry *ctrlGetPadData();

std::int32_t GetGameSysActivityCounter(std::int32_t index);
void SetGameSysActivityCounter(std::int32_t index, std::int32_t value);
SystemModule UpdateCurrentSceneInfoFromTask(std::int32_t &actNumber, shen_name &areaId, std::int32_t &entryId);
void SetTempMapIDs();
void SetDirtAngle(std::int32_t dirtAngle);
void EventBase_Wrapper(void *mapInfo);

} // namespace shenmue::s2
