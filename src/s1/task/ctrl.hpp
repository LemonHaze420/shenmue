#pragma once

#include "common/types.hpp"
#include "s1/boot.hpp"
#include "s1/file_state.hpp"
#include "s1/task/char.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {
struct Character;

struct MapControlData {
    void *ryo;
    void *padCaptureEntry;
    HLib::Task *mapEventTask;
    HLib::Task *scenarioTask;
    HLib::Task *cameraTask;
    HLib::Task *objectTask;
    HLib::Task *scrollTask;
    HLib::Task *modelLoaderTask;
    HLib::Task *filmTask;
    std::uint64_t field_048;
    HLib::Task *unknownTask;
    HLib::Task *clipTask;
    void *renderTask;
    HLib::Task *windTask;
    HLib::Task *dwmzTask;
    HLib::Task *mbstTask;
    std::uint64_t field_080;
    void *mapInfo;
    void *mapQueryRuntimeTable;
    std::array<void *, 32> mapQuerySetsBySection;
    std::uint32_t flags;
    std::uint32_t areaId;
    std::int32_t sceneId;
    std::uint32_t entryPoint;
    SystemModule module;
    std::uint32_t mapRuntimeFlags;
    std::array<CharacterIdTransform, 32> characterIdTable;
    std::array<void *, 13> characterBuckets;
    std::int32_t pendingCharacterRetirements;
    std::byte cycleMask[0x20];
    std::uint32_t direction;
};

static_assert(offsetof(MapControlData, flags) == 0x198);
static_assert(offsetof(MapControlData, modelLoaderTask) == 0x38);
static_assert(offsetof(MapControlData, clipTask) == 0x58);
static_assert(offsetof(MapControlData, renderTask) == 0x60);
static_assert(offsetof(MapControlData, windTask) == 0x68);
static_assert(offsetof(MapControlData, dwmzTask) == 0x70);
static_assert(offsetof(MapControlData, mbstTask) == 0x78);
static_assert(offsetof(MapControlData, mapQueryRuntimeTable) == 0x90);
static_assert(offsetof(MapControlData, mapQuerySetsBySection) == 0x98);
static_assert(offsetof(MapControlData, module) == 0x1a8);
static_assert(offsetof(MapControlData, characterBuckets) == 0x730);
static_assert(sizeof(MapControlData) == 0x7c0);

extern std::uint32_t g_areaId;
extern std::int32_t g_sceneId;
extern std::uint32_t g_entryPoint;
extern HLib::Task *g_mapControlTask;
extern std::uint32_t g_mapInfoSize;
extern FileState *g_mapInfoFileState;

void *MAPINFO_Load();
void MAPINFO_InitializeSceneParameters(const void *mapInfo);
const std::byte *MAPINFO_FindChunkPayload(std::uint32_t token, std::uint32_t *payloadSize);
const std::byte *MAPINFO_FindChunkIn(const void *mapInfo, std::uint32_t mapInfoSize, std::uint32_t token);
const std::byte *MAPINFO_FindChunkPayloadIn(const void *mapInfo, std::uint32_t mapInfoSize, std::uint32_t token, std::uint32_t *payloadSize);
std::uint32_t MAPINFO_ApplyDynamicMapDisableChunk();
void MAPINFO_Release();
bool MAPCTRL_GetCharacterIdTransform(std::uint32_t index, std::array<std::uint32_t, 3> &position, std::array<std::uint32_t, 3> &rotation);
bool MAPCTRL_SetCharacterIdPackCollision(std::int32_t index, const void *collisionData);
bool MAPCTRL_SetCharacterIdData(std::int32_t index, const std::array<std::uint32_t, 3> &position, const std::array<std::uint32_t, 3> &rotation,
    std::uint32_t field20, const void *collisionData);
std::uint16_t CTRL_EvaluateWeatherSoundEnvironment(Character *character, float x, float z);
void LoadGameModule(SystemModule module, std::int32_t sceneId, std::uint32_t areaId, std::uint32_t entryPoint);
void MapControlInternal();
void CEWP_RetireGroup(std::int32_t groupId, bool immediate);
void HLTaskFunc_MapControlTask_Callback(HLib::TaskData *taskData);

void MapControlTask_Init(HLib::TaskData *taskData);
void HLTaskFunc_EnqueueMapControlTask(HLib::TaskData *taskData);

void MapControl_UpdateMapEvents();
void MapEvent_AllocateComponent(Character *character, bool tokenMapEvent, bool tokenNoMapEvent);

bool ClipScreenVisibility(const Vector3 &position, float radius, float range);
std::uint32_t shenGetNextCharaFromTable(HLib::Task *scenarioTask, Character *character);

struct AreaIdleStatsProfile {
    std::uint32_t areaId;
    std::int32_t counterIndex;
};

inline constexpr AreaIdleStatsProfile kAreaIdleStatsProfiles[]{
    {SHEN_NAME(D000), 99},
    {SHEN_NAME(JD00), 19},
    {SHEN_NAME(JU00), 15},
    {SHEN_NAME(JHD0), 16},
    {SHEN_NAME(MN01), 85},
    {SHEN_NAME(MO01), 95},
    {SHEN_NAME(YDB1), 14},
    {SHEN_NAME(MS08), 76},
    {SHEN_NAME(DSBA), 49},
    {SHEN_NAME(DCHA), 41},
    {SHEN_NAME(DPIA), 56},
    {SHEN_NAME(DKPA), 57},
    {SHEN_NAME(DRSA), 46},
    {SHEN_NAME(DAZA), 40},
    {SHEN_NAME(DBHB), 42},
    {SHEN_NAME(JABE), 23},
    {SHEN_NAME(DRME), 58},
    {SHEN_NAME(DSKI), 38},
    {SHEN_NAME(DSLI), 55},
    {SHEN_NAME(DMAJ), 53},
    {SHEN_NAME(DCBN), 50},
    {SHEN_NAME(DURN), 52},
    {SHEN_NAME(JOMO), 0},
    {SHEN_NAME(DBYO), 43},
    {SHEN_NAME(TATQ), 45},
    {SHEN_NAME(MS8S), 83},
    {SHEN_NAME(DSUS), 51},
    {SHEN_NAME(DGCT), 37},
    {SHEN_NAME(DRHT), 47},
    {SHEN_NAME(DSLT), 54},
    {SHEN_NAME(MKYU), 97},
    {SHEN_NAME(DTKY), 48},
    {SHEN_NAME(MKSG), 70},
    {SHEN_NAME(DKTY), 39},
    {SHEN_NAME(DJAZ), 44},
    {SHEN_NAME(DYKZ), 59},
    {SHEN_NAME(MFSY), 100},
    {0, -1},
};

} // namespace shenmue::s1
