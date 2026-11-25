#include "s2/events/event_api.h"
#include "area_chime.hpp"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
static unsigned int AREA_CHIME_SOUND_TABLE[2][15] = {
#include "chime_sound_table.inl"
};
static int areaChimeLastSound;
void StartAreaChime(void) {
    switch (g_currentMap) {
    case SHEN_NAME4('Q', 'E', '0', '0'):
    case SHEN_NAME4('Q', 'E', '0', '1'):
    case SHEN_NAME4('Q', 'E', '0', '6'):
        FTBL.HLib_EnqueueTaskWithoutParameter(AreaChimeCallback, 4, 0x0b, SHEN_NAME4('c', 'h', 'i', 'm'));
        areaChimeLastSound = 100;
        break;
    }
}
#else

#include "s2/date_time.hpp"
#include "s2/map_control.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {

std::int32_t areaChimeLastSound{-1};

#endif

// @verified - p
void AreaChimeCallback(void *taskData) {
#if defined(SHENMUE_EVENT_OVERLAY)
    EventDateTime dateTime;
    Vector3 position = {301.3f, 128.8f, 73.2f};
    int stream, type, hourIndex, quarterIndex, sound;
    signed char minute;
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    stream = FTBL.GetQronNextStream();
    hourIndex = (dateTime.hour + 11) % 12;
    minute = dateTime.minute;
    quarterIndex = ((signed char)(minute / 15) - 1) % 3;
    type = stream == 8;
    switch (dateTime.minute) {
    case 0:
        sound = AREA_CHIME_SOUND_TABLE[type][hourIndex + 3];
        break;
    case 15:
    case 30:
    case 45:
        sound = AREA_CHIME_SOUND_TABLE[type][quarterIndex];
        break;
    default:
        sound = 0;
        break;
    }
#else
    DateTime dateTime;
    shenGetCurrentDateTime(dateTime);
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

#endif
    if (areaChimeLastSound != sound && sound != 0) {
        if (!FTBL.plymIsSearchBlocked()) {

#if defined(SHENMUE_EVENT_OVERLAY)
            FTBL.Shenmue_Execute3DSoundCommand(sound | 0x20000, &position, 2.0f, 1000.0f);
#else
            (void)Execute3DSoundCommand(static_cast<std::uint32_t>(sound) | 0x20000U, chimePosition, 2.0f, 1000.0f, 0.0f);
#endif
        }
        areaChimeLastSound = sound;
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("-- chime [%2d:%2d, code %d, type %d] --\n", dateTime.hour, dateTime.minute, sound, type);
#endif
    }
}

#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace shenmue::s2::areas
#endif
