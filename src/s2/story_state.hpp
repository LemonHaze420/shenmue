#pragma once

#include "common/types.hpp"

#include <stddef.h>
#if !SHENMUE_IS_DC
#include <span>
#endif

namespace shenmue { namespace s2 {

#if SHENMUE_IS_DC
#define TASK_UPDB SHEN_NAME(UPDB)
#else
inline constexpr shen_name TASK_UPDB{SHEN_NAME(UPDB)};
#endif

// Native STEP state consumed directly by the baked EV3 area functions.
std::uint32_t GetCurrentStep();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
void DisplayStepInfo();
#endif
void SetSTEP(std::int32_t step);
void ResetSTEP();
#if SHENMUE_IS_DC
void UpdateStepDebugCounters();
std::uint32_t GetStepElapsedFrames();
#else
void UpdateStepDebugCounters() noexcept;
std::uint32_t GetStepElapsedFrames() noexcept;
#endif
// SCNT game-data id 6 reads this signed 32-bit owner; its money animation clamps the target.
std::int32_t GetCurrentCash();
void SetCurrentCash(std::int32_t cash);
// UpdateGameplayFlags refreshes the ten-word owner once per initialized SCEN frame,
// after interaction processing. Operand reads/writes do not initialize it.
void UpdateGameplayFlags();
#if !SHENMUE_IS_DC
void CopyShenmueFlagsTo(std::span<std::byte, 0x34> output) noexcept;
void SetShenmueFlagsTo(std::span<const std::byte, 0x34> input) noexcept;
#endif
// SCNT modes 7-14 address the first three words: object ids 5 and 6 select
// words one and two; every other id selects word zero.
bool ResolveScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t &value);
bool AssignScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t value);

extern std::int32_t g_storyStep;

} } // namespace shenmue::s2
