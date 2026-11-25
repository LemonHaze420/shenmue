#pragma once
#include "shenmue.h"
#if SHENMUE_S2_GCC295
#include "common/legacy_types.hpp"
#else
#include <cstdint>
#endif
namespace shenmue {
namespace s2 {
#if SHENMUE_S2_GCC295
void SetMinigameSeed(std::uint32_t seed);
float IncrementMinigameRandFloat();
#else
void SetMinigameSeed(std::uint32_t seed) noexcept;
float IncrementMinigameRandFloat() noexcept;
#endif
} } // namespace shenmue::s2
