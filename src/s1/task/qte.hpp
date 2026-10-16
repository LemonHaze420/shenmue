#pragma once

#include <cstdint>

namespace shenmue::s1 {




struct QteRequest {
    std::uint16_t expectedStatus{};
    std::int16_t timeoutFrames{};
    std::uint8_t promptMode{};
    std::uint8_t bonusFrames{};
};

// Ordinary Shenmue I QTE boundary used by SCNF slots 4 and 5. Successful
// input publishes expectedStatus | 0x2000, a wrong input publishes 0, and a
// timeout publishes 0xa000.
void EVT_AT_QTE(const QteRequest &request);
void EVT_AT_ResultQTE(std::int16_t result);




bool EV_GetCurrentQteStatusWhileOpen(std::int16_t &result);
bool EV_GetCurrentQteStatusIfIdle(std::int16_t &result);

} // namespace shenmue::s1
