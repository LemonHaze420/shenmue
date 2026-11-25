#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include <cstdint>
#include <cstddef>
#include "s2/hlib.hpp"

namespace shenmue { namespace s2 {

struct EasyOptionState {
    std::int8_t selectedRow;
    std::uint8_t previousValue, mode, reserved03, active, showsOptions;
};
typedef char EasyOptionStateSizeCheck[sizeof(EasyOptionState) == 6 ? 1 : -1];
typedef char EasyOptionModeOffsetCheck[offsetof(EasyOptionState, mode) == 2 ? 1 : -1];
typedef char EasyOptionActiveOffsetCheck[offsetof(EasyOptionState, active) == 4 ? 1 : -1];
extern EasyOptionState g_easyOptionState;
bool IsEasyOptionActive();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
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
