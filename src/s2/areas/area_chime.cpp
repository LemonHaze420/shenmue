#include "s2/areas/area_chime.hpp"

#include "s2/date_time.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {

std::int32_t areaChimeLastSound{-1};

// @verified - p
void AreaChimeCallback(void *) {
    DateTime dateTime;
    GetCurrentDateTime(dateTime);
    const std::int32_t stream = static_cast<std::int32_t>(GetQronNextStream());
    const std::size_t tableOffset = stream == 8 ? 15U : 0U;
    const std::int32_t hour = dateTime.hour;
    const std::int32_t minute = dateTime.minute;
    std::int32_t sound = 0;

    if (minute == 0) {
        const std::int32_t hourIndex = 3 + (hour + 11) % 12;
        sound = static_cast<std::int32_t>(AREA_CHIME_SOUND_TABLE[tableOffset + static_cast<std::size_t>(hourIndex)]);
    } else {
        const std::uint8_t minuteByte = static_cast<std::uint8_t>(dateTime.minute);
        if (minuteByte <= 45U && (quarterMinuteMask & (std::uint64_t{1} << minuteByte)) != 0) {
            const std::int32_t quarterIndex = (minute / 15 - 1) % 3;
            sound = static_cast<std::int32_t>(AREA_CHIME_SOUND_TABLE[tableOffset + static_cast<std::size_t>(quarterIndex)]);
        }
    }

    if (areaChimeLastSound != sound && sound != 0) {
        if (!plymIsSearchBlocked()) {

            (void)Execute3DSoundCommand(static_cast<std::uint32_t>(sound) | 0x20000U, chimePosition, 2.0f, 1000.0f, 0.0f);
        }
        areaChimeLastSound = sound;
    }
}

} // namespace shenmue::s2::areas
