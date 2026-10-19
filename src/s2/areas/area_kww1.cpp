#include "s2/areas/area_kww1.hpp"

#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"

#include <cstdint>

namespace shenmue::s2::areas {

void AreaKWW1Init() {
}

void AreaKWW1Main(std::uint32_t) {
    EV_FreeAuthorPak(1, "0547");
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);

    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    const std::uint8_t hourDelta = static_cast<std::uint8_t>(static_cast<std::uint8_t>(dateTime.hour) - 8U);
    if (hourDelta > 14U) {
        dateTime.hour = 8;
        dateTime.minute = 0;
        SetCurrentDateTimeTo(dateTime);
    }
}

void AreaKWW1Cleanup(std::int32_t, std::uint32_t) {
}

const GameplayAreaDefinition areaKWW1GameplayDefinition{SHEN_NAME(KWW1), 0, AreaKWW1Init, AreaKWW1Main, AreaKWW1Cleanup};

} // namespace shenmue::s2::areas
