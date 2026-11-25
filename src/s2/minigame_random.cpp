#include "shenmue.h"
#include "s2/minigame_random.hpp"
#if SHENMUE_S2_GCC295
#define SHENMUE_MINIGAME_NOEXCEPT
#else
#define SHENMUE_MINIGAME_NOEXCEPT noexcept
#endif
namespace shenmue {
namespace s2 {
namespace {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::uint32_t minigameSeed = 1;
#else
std::uint32_t minigameSeed;
#endif
} // namespace
// @verified - p
void SetMinigameSeed(std::uint32_t seed) SHENMUE_MINIGAME_NOEXCEPT {
    minigameSeed = seed;
}
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t NextMinigameRandom() {
    minigameSeed = 0x41c64e6dU * minigameSeed + 12345U;
    return (minigameSeed >> 16) & 0x7fffU;
}
float IncrementMinigameRandFloat() {
    return static_cast<float>(NextMinigameRandom()) * 0.000030517578125f;
}
#else
// @verified - p
float IncrementMinigameRandFloat() SHENMUE_MINIGAME_NOEXCEPT {
    minigameSeed = 0x41c64e6dU * minigameSeed + 12345U;
    return static_cast<float>((minigameSeed >> 16) & 0x7fffU) * 0.000030517578125f;
}
#endif
}
} // namespace shenmue::s2

#undef SHENMUE_MINIGAME_NOEXCEPT
