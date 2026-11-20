#pragma once
#include "shenmue.h"
#include <cstdint>
namespace shenmue {
namespace s2 {
void SetMinigameSeed(std::uint32_t seed) noexcept;
float IncrementMinigameRandFloat() noexcept;
} } // namespace shenmue::s2
