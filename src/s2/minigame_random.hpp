#pragma once
#include <cstdint>
namespace shenmue::s2 {
void SetMinigameSeed(std::uint32_t seed) noexcept;
float IncrementMinigameRandFloat() noexcept;
} // namespace shenmue::s2
