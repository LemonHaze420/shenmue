#pragma once

#include "common/types.hpp"

#include <cstdint>
#include <cstddef>
#include <span>

namespace shenmue::s2 {

inline constexpr shen_name TASK_UPDB{SHEN_NAME(UPDB)};


std::uint32_t GetCurrentStep();
void SetSTEP(std::int32_t step);
void ResetSTEP();
void UpdateStepDebugCounters() noexcept;
std::uint32_t GetStepElapsedFrames() noexcept;
// SCNT game-data id 6 reads this signed 32-bit owner; its money animation clamps the target.
std::int32_t GetCurrentCash();
void SetCurrentCash(std::int32_t cash);
// UpdateGameplayFlags refreshes the ten-word owner once per initialized SCEN frame,
// after interaction processing. Operand reads/writes do not initialize it.
void UpdateGameplayFlags();
void CopyShenmueFlagsTo(std::span<std::byte, 0x34> output) noexcept;
void SetShenmueFlagsTo(std::span<const std::byte, 0x34> input) noexcept;
// SCNT modes 7-14 address the first three words: object ids 5 and 6 select
// words one and two; every other id selects word zero.
bool ResolveScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t &value);
bool AssignScenarioEventStateOperand(std::uint8_t mode, std::uint16_t objectId, std::int32_t value);

extern std::int32_t g_storyStep;

} // namespace shenmue::s2
