#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstdint>
#include "s2/hlib.hpp"

namespace shenmue { namespace s2 {

extern std::uint8_t g_easyOptionActive;
bool IsEasyOptionActive();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
extern std::uint8_t g_easyOptionMode;
struct PosePreviewRecord {
    void *source, *model;
    char characterName[16];
    std::uint16_t state, retained1a;
};
typedef char NativePosePreviewRecordSizeCheck[sizeof(PosePreviewRecord) == 0x1c ? 1 : -1];
extern PosePreviewRecord posePreviewRecords[8];
void EnqueueESOP();
void SetNewEasyOptionState(std::int32_t mode);
#endif

} } // namespace shenmue::s2

#endif
