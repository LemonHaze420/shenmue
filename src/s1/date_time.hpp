#pragma once

#include <cstdint>

namespace shenmue::s1 {

#pragma pack(push, 1)
struct GameDateTime {
    std::uint8_t year;
    std::uint8_t month;
    std::uint8_t day;
    std::uint8_t dayOfWeek;
    std::uint8_t hours;
    std::uint8_t minutes;
    std::uint8_t seconds;
    std::uint8_t reserved;
};
#pragma pack(pop)

static_assert(sizeof(GameDateTime) == 8);

extern GameDateTime g_currentDateTime;

} // namespace shenmue::s1
