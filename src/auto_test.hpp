#pragma once

#ifndef SHEN_AUTO_TEST_ENABLED
#define SHEN_AUTO_TEST_ENABLED 0
#endif

#if SHEN_AUTO_TEST_ENABLED
#include <cstdint>
#if defined(_WIN32)
#include <cstdlib>
#endif

struct ShenAutoTestCaptureState {
    char signature[4];
    std::uint32_t phase;
    std::uint32_t submittedFrames;
};
static_assert(sizeof(ShenAutoTestCaptureState) == 12);

extern "C" {
inline volatile ShenAutoTestCaptureState g_shenAutoTestCapture{{'A', 'T', 'S', 'T'}, 0, 0};
}

namespace shenmue::auto_test {

inline bool TitleLatched() {
    return g_shenAutoTestCapture.phase != 0;
}

inline void LatchTitleFrame(bool ready) {
    if (ready && !TitleLatched())
        g_shenAutoTestCapture.phase = 1;
}

inline bool FinishFrame() {
    if (g_shenAutoTestCapture.phase != 1)
        return false;
    g_shenAutoTestCapture.submittedFrames = g_shenAutoTestCapture.submittedFrames + 1;
    if (g_shenAutoTestCapture.submittedFrames < 8)
        return false;
    g_shenAutoTestCapture.phase = 2;
    return true;
}

#if defined(_WIN32)
inline const char *CapturePath() {
    const char *const path = std::getenv("SHEN_AUTO_TEST_CAPTURE");
    return path && *path ? path : "auto-test.png";
}
#endif

} // namespace shenmue::auto_test
#endif
