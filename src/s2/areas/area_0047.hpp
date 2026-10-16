#pragma once

#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0047_AREA{SHEN_NAME(0047)};
inline constexpr shen_name AREA_0047_CHO_{SHEN_NAME(CHO_)};
inline constexpr shen_name AREA_0047_SBO_{SHEN_NAME(SBO_)};
inline constexpr shen_name AREA_0047_MRC_{SHEN_NAME(MRC_)};
inline constexpr shen_name AREA_0047_SCX_{SHEN_NAME(SCX_)};
inline constexpr shen_name AREA_0047_CHR_{SHEN_NAME(CHR_)};
inline constexpr shen_name AREA_0047_A020{SHEN_NAME(a020)};
inline constexpr shen_name AREA_0047_A022{SHEN_NAME(a022)};
inline constexpr shen_name AREA_0047_A021{SHEN_NAME(a021)};
inline constexpr shen_name AREA_0047_A418{SHEN_NAME(a418)};
inline constexpr shen_name AREA_0047_A420{SHEN_NAME(a420)};
inline constexpr shen_name AREA_0047_A421{SHEN_NAME(a421)};
inline constexpr shen_name AREA_0047_A412{SHEN_NAME(a412)};
inline constexpr shen_name AREA_0047_AA42{SHEN_NAME(aa42)};
inline constexpr shen_name AREA_0047_AA43{SHEN_NAME(aa43)};
inline constexpr shen_name AREA_0047_CHARACTERS[]{AREA_0047_CHO_, AREA_0047_SBO_, AREA_0047_MRC_, AREA_0047_SCX_, AREA_0047_CHR_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0047_MOTION[] = "0047A.MOT", AREA_0047_CONVERSATION[] = "0047";
inline constexpr const char *AREA_0047_DEBUG_NAMES[]{"0047A000.c\n"};

inline constexpr shen_name AREA_0047_TOFB{SHEN_NAME(TOFB)}, AREA_0047_DYNAMICS_CHARACTER{SHEN_NAME(a418)};
inline constexpr std::int32_t AREA_0047_DYNAMICS_PARTS[]{152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,
    172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
    203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 228, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233,
    234, 235};
inline constexpr std::uint16_t AREA_0047_PALETTE[]{
    0x311, 0x311, 0x411, 0x511, 0x612, 0x712, 0xa32, 0xb32, 0xb42, 0xc50, 0xd70, 0xe70, 0xe80, 0xfa0, 0xfc3, 0xfd6};
extern const ScriptAreaDefinition area0047ScriptDefinition;

} // namespace shenmue::s2::areas
