#pragma once

#include "s1/hlib.hpp"
#include "common/math.hpp"

#include <array>
#include <cstdint>

namespace shenmue::s1 {

enum class SystemModule : std::uint32_t {
    Colbar = 0,
    Init = 1,
    SetToDisplay = 2,
    Audio = 3,
    Display = 4,
    SetToNull = 5,
    Null = 6,
    Map = 7,
    Null2 = 8,
    Harrier = 9,
    Credits = 10,
    HangOn = 11,
};

struct SystemModuleEntry {
    HLib::TaskCallback callback;
    std::int64_t reserved;
    std::int64_t reservedData;
};

struct ModuleData {
    std::int64_t data[32];
};

#pragma pack(push, 1)
struct ModuleTaskData {
    SystemModule currentModule;
    SystemModule nextModule;
    SystemModule previousModule;
    std::uint32_t previousNextModule;
    ModuleData *data;
    Vector3 position;
};
#pragma pack(pop)

static_assert(sizeof(SystemModuleEntry) == 0x18);
static_assert(sizeof(ModuleData) == 0x100);
static_assert(sizeof(ModuleTaskData) == 0x24);

extern std::array<SystemModuleEntry, 22> SystemModules;
extern HLib::Task *g_pModuleTask;
extern std::int32_t g_isRenderSuppressed;
extern std::int32_t g_perspectiveProjection;
extern std::int32_t Shenmue_nNumVBLANKs;
extern std::int32_t Shenmue_nNumRenderedFrames;
extern std::int32_t Shenmue_nPauseState;
extern std::int32_t g_updateRenderFrameMode;
extern void *VFMA_TaskPtr;
extern std::uint32_t g_ClearColour;
extern std::int64_t g_TotalSceneFileReadBytes;
extern std::uint32_t g_EnvPrngSeed;

int Shenmue_Main(int argc, char **argv);
int Initialization(int argc, char **argv);
std::int64_t HLib_UpdateTaskQueue(SystemModule module);
void InitMemory_callback(HLib::TaskData *);
void Initialization_2();

} // namespace shenmue::s1
