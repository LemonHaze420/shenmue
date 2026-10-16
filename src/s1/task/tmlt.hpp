#pragma once

#include "s1/hlib.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

inline constexpr shen_name TASK_TMLT{SHEN_NAME(TMLT)};

enum class TMLTControlMode : std::int32_t {
    Init = 0,
    Update = 1,
    Disable = 2,
    Destroy = 3,
    GetTimeLeft = 4,
    InitAndReset = 5,
};

struct TMLTControlRequest {
    TMLTControlMode mode;
    std::int32_t seconds;
};

static_assert(sizeof(TMLTControlRequest) == 8);
static_assert(offsetof(TMLTControlRequest, mode) == 0);
static_assert(offsetof(TMLTControlRequest, seconds) == 4);




extern HLib::Task *g_TMLT_Task;
extern std::int16_t g_tmp_tmlt_textures_initialised;
extern std::array<std::int32_t, 10> g_tmltDigitPVRs;
extern std::int32_t g_tmltDotPVR0;
extern std::int32_t g_tmltDotPVR1;
extern std::int16_t g_BeepIntervalFrames;
extern std::int16_t g_BeepFrameCounter;
extern std::uint8_t g_tmlt_Active;
extern std::int16_t g_FrameCounter30;
extern std::int32_t g_curr_time_left;

void InitTMLT(std::int16_t texturesInitialised);
void FreeTMLT();
void TMLT_Callback(HLib::TaskData *taskData);

} // namespace shenmue::s1
