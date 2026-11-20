#include "s2/screen_film.hpp"

namespace shenmue { namespace s2 {

std::uint32_t savedFilmTargetActive{};
std::array<std::uint8_t, 4> savedFilmTarget{};

// @verified - d
void ResetTaskWaitFlags() {
    savedFilmTargetActive = false;
    savedFilmTarget = {255, 0, 0, 0};
}

// @verified - pd
std::uint64_t GetIsWaitingFlag() {
    return savedFilmTargetActive;
}

// @verified - d

std::uint32_t GetFadedClearColour() {
    return static_cast<std::uint32_t>(savedFilmTarget[0]) << 24 | static_cast<std::uint32_t>(savedFilmTarget[1]) << 16 |
        static_cast<std::uint32_t>(savedFilmTarget[2]) << 8 | savedFilmTarget[3];
}

} } // namespace shenmue::s2
