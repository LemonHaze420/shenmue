#pragma once

#include <cstdint>

namespace shenmue::s1 {

struct ScenarioContext;

extern std::uint32_t g_qtePrngSeed;

void CycleStack_ResetLists();

// Shenmue scenario function 390.
void Script_IncrementQTEMinigameRand16(ScenarioContext *context, void *rawParams);

// Shenmue scenario function 458.
void Script_StartNozomiBikeSeq(ScenarioContext *context, void *rawParams);

} // namespace shenmue::s1
