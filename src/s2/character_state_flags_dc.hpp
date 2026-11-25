#pragma once

#include "s2/character_control_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

std::uint32_t AssignCharacterControlFlags(Character *character, std::uint32_t flags);
void SetCharacterDrawControlEnabled(Character *character, std::int32_t enabled);
void charaToggleFlag(Character *character, std::int32_t enabled);
std::uint8_t *figpAssignModeForCharacter(Character *character, std::uint8_t mode);
void SetClothFlag(Character *character, std::uint8_t flags, std::int32_t enabled);

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
struct FigurePoseControlPrefix {
    std::uint8_t mode, startTime, motionScale, direction, flags;
};
typedef char Layout_character_state_flags_dc_0[(sizeof(FigurePoseControlPrefix) == 5) ? 1 : -1];
typedef char Layout_character_state_flags_dc_1[(offsetof(FigurePoseControlPrefix, flags) == 4) ? 1 : -1];
std::uint32_t figpAssignMotScaleAndDirFor(Character *character, std::uint8_t scale, std::uint8_t direction);
struct MotionWork;
void figpToggleMotionworkFlag(Character *character, MotionWork *work);
FigurePoseControlPrefix *EV_figpAssignFlagsFor(Character *character, std::uint8_t bitIndex, std::uint8_t enabled);
std::uint32_t figpGetFlagMask(Character *character, std::uint8_t bitIndex);
#endif


} } // namespace shenmue::s2

#endif
