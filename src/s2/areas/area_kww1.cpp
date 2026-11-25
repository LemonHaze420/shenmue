#include "s2/events/event_api.h"
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_queued_dc.h"
void AreaCleanup(int afternoon);
#else
#include "s2/areas/area_kww1.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/scenario_window.hpp"
#include <cstdint>
namespace shenmue::s2::areas {
void AreaKWW1Init() {
}
#endif

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaMain(unsigned int startParameters) {
    EventDateTime dateTime;
#else
void AreaKWW1Main(std::uint32_t) {
#endif
    FTBL.EV_FreeAuthorPak(1, "0547");
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.EV_SetPlayerFlag(12, 1495, 0);
    FTBL.GetCurrentDateTime((DateTime *)&dateTime);
    if ((unsigned char)(dateTime.hour - 8) > 14) {
#else
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1495, 0);
    DateTime dateTime{};
    shenGetCurrentDateTime(dateTime);
    const std::uint8_t hourDelta = static_cast<std::uint8_t>(static_cast<std::uint8_t>(dateTime.hour) - 8U);
    if (hourDelta > 14U) {
#endif
        dateTime.hour = 8;
        dateTime.minute = 0;
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&dateTime);
#else
        SetCurrentDateTimeTo(dateTime);
#endif
        FTBL.dbgPrintEventMessage("不当な強制睡眠を実行しました。");
    }
#if defined(SHENMUE_EVENT_OVERLAY)
    AreaCleanup(0);
#endif
}

#if defined(SHENMUE_EVENT_OVERLAY)
void AreaCleanup(int afternoon) {
#else
void AreaKWW1Cleanup(std::int32_t, std::uint32_t) {
#endif
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const GameplayAreaDefinition areaKWW1GameplayDefinition{SHEN_NAME(KWW1), 0, AreaKWW1Init, AreaKWW1Main, AreaKWW1Cleanup};
} // namespace shenmue::s2::areas
#endif
