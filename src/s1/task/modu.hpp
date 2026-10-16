#pragma once

#include "s1/hlib.hpp"

namespace shenmue::s1 {

struct ScenarioContext;

extern std::uint32_t g_moduleInputActivityFlags;
extern std::uint32_t g_moduleInputActivityFrames;
extern std::uint32_t g_moduleFrameCount;
extern std::int32_t g_ctrlInitializedFrameCount;
extern bool g_ctrlInitialized;
extern std::int32_t g_moduleInputSuppressFrames;

void MODU_Callback(HLib::TaskData *taskData);

// Shenmue scenario function 391.
void MODU_Script_shenRequestGameModule(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
