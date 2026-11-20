#include "shenmue.h"
#include "s2/minigame_random.hpp"
#define SHENMUE_MINIGAME_NOEXCEPT noexcept
namespace shenmue { namespace s2 {
namespace {
std::uint32_t minigameSeed;
}
// @verified - p
void SetMinigameSeed(std::uint32_t seed) SHENMUE_MINIGAME_NOEXCEPT {
    minigameSeed = seed;
}
// @verified - p
float IncrementMinigameRandFloat() SHENMUE_MINIGAME_NOEXCEPT {
    minigameSeed = 0x41c64e6dU * minigameSeed + 12345U;
    return static_cast<float>((minigameSeed >> 16) & 0x7fffU) * 0.000030517578125f;
}
} } // namespace shenmue::s2

#undef SHENMUE_MINIGAME_NOEXCEPT
