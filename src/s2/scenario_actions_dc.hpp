#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "common/math.hpp"
#include <cstddef>

namespace shenmue {
namespace s2 {

struct Character;

struct ScenarioActionQueue {
    shen_name name;
    std::uint32_t count;
    std::byte *read;
    std::byte *write;
    std::byte storage[0x70];
};
typedef char Layout_scenario_actions_dc_0[(sizeof(ScenarioActionQueue) == 0x80) ? 1 : -1];
typedef char Layout_scenario_actions_dc_1[(offsetof(ScenarioActionQueue, count) == 4) ? 1 : -1];
typedef char Layout_scenario_actions_dc_2[(offsetof(ScenarioActionQueue, read) == 8) ? 1 : -1];
typedef char Layout_scenario_actions_dc_3[(offsetof(ScenarioActionQueue, write) == 0x0c) ? 1 : -1];
typedef char Layout_scenario_actions_dc_4[(offsetof(ScenarioActionQueue, storage) == 0x10) ? 1 : -1];

extern ScenarioActionQueue *scenarioActionQueueStorage;
void InitializeScenarioActionQueues(void *storage);

#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::int8_t g_scenarioLookCameraState;
extern std::uint32_t g_scenarioParticipantFaceLooks[8];
extern Vector3 g_scenarioDefaultLookLocation;
Character *ResolveScenarioTalkCharacter(shen_name name);
void SolveRyoMatrix(shen_name origin, shen_name other, std::int32_t yaw, Vector3 *result);
void scenProcessCharacterActionQueue();
void ResetScenarioInteractionState();
std::int32_t AllocateScenarioActionQueue(shen_name name);
std::uint32_t ReadScenarioActionQueue(std::uint32_t index, void *output, std::int32_t size);
std::int32_t WriteScenarioActionQueue(std::uint32_t index, const void *input, std::uint32_t size);
void FlushScenarioActionQueue();
#endif

} // namespace s2
} // namespace shenmue

#endif
