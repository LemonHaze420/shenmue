#include "s2/minigame_random.hpp"
namespace shenmue::s2 {
namespace {
std::uint32_t minigameSeed;
}
// @verified - p
void SetMinigameSeed(std::uint32_t seed) noexcept {
    minigameSeed = seed;
}
// @verified - p
float IncrementMinigameRandFloat() noexcept {
    minigameSeed = 0x41c64e6dU * minigameSeed + 12345U;
    return static_cast<float>((minigameSeed >> 16) & 0x7fffU) * 0.000030517578125f;
}
} // namespace shenmue::s2
