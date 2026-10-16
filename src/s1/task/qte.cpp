#include "s1/task/qte.hpp"

#include "s1/ui_runtime.hpp"

namespace shenmue::s1 {

void EVT_AT_QTE(const QteRequest &request) {
    UI_SFXOverlayBegin({
        request.expectedStatus,
        request.timeoutFrames,
        request.promptMode,
        request.bonusFrames,
    });
}

void EVT_AT_ResultQTE(std::int16_t result) {
    UI_SFXOverlayResolve(result);
}

bool EV_GetCurrentQteStatusWhileOpen(std::int16_t &result) {
    return UI_SFXOverlayConsumeResultWhileOpen(result);
}

bool EV_GetCurrentQteStatusIfIdle(std::int16_t &result) {
    return UI_SFXOverlayConsumeResultIfIdle(result);
}

} // namespace shenmue::s1
