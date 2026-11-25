#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"
#include "s2/legacy_sprite_types.hpp"

namespace shenmue { namespace s2 {

static const shen_name TASK_COLB = SHEN_NAME(COLB);

struct ColourBarWork {
    std::int32_t textures[2];
    std::uint32_t selection;
    LegacySpritePacket packet;
};
typedef char Layout_colour_bar_dc_0[(sizeof(ColourBarWork) == 0x64) ? 1 : -1];
typedef char Layout_colour_bar_dc_1[(offsetof(ColourBarWork, packet) == 0x0c) ? 1 : -1];

void COLB_Module(void *taskData);

} } // namespace shenmue::s2

#endif
