#pragma once

#include "s2/osage_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue { namespace s2 {

static const OsageSpringParameters kDefaultOsageSpringParameters= {
    SHEN_FIELD(mode) 0, SHEN_FIELD(length) 1.0f, SHEN_FIELD(radius) 0.001f, SHEN_FIELD(damping) 0.65f, SHEN_FIELD(stiffness) 0.0f, SHEN_FIELD(gravity) 0.01f};
typedef std::uint32_t OsageCollisionGroup;
enum { OsageCollisionGroup_Shenhua, OsageCollisionGroup_Xiuying, OsageCollisionGroup_Joy, OsageCollisionGroup_Ren, OsageCollisionGroup_Default, OsageCollisionGroup_Ksy, OsageCollisionGroup_Kxy, OsageCollisionGroup_Ryc, OsageCollisionGroup_Mn5, OsageCollisionGroup_Mmh, OsageCollisionGroup_Shc, OsageCollisionGroup_Tgy, OsageCollisionGroup_Bkn, OsageCollisionGroup_Sf3 };
#define OSAGE_CHARACTER_REN SHEN_NAME4('R', 'E', 'N', '_')
#define OSAGE_CHARACTER_SYE SHEN_NAME4('S', 'Y', 'E', '_')
#define OSAGE_CHARACTER_SYP SHEN_NAME4('S', 'Y', 'P', '_')
#define OSAGE_CHARACTER_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define OSAGE_CHARACTER_JOY SHEN_NAME4('J', 'O', 'Y', '_')
#define OSAGE_CHARACTER_RYC SHEN_NAME4('R', 'Y', 'C', '_')
#define OSAGE_CHARACTER_KYG SHEN_NAME4('K', 'Y', 'G', '_')
#define OSAGE_CHARACTER_MN5 SHEN_NAME4('M', 'N', '5', '_')
#define OSAGE_CHARACTER_MMH SHEN_NAME4('M', 'M', 'H', '_')
#define OSAGE_CHARACTER_SHC SHEN_NAME4('S', 'H', 'C', '_')
#define OSAGE_CHARACTER_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define OSAGE_CHARACTER_BKN SHEN_NAME4('B', 'K', 'N', '_')
#define OSAGE_CHARACTER_SF3 SHEN_NAME4('S', 'F', '3', '_')
#define OSAGE_CHARACTER_KSY SHEN_NAME4('K', 'S', 'Y', '_')
#define OSAGE_CHARACTER_KXY SHEN_NAME4('K', 'X', 'Y', '_')
extern OsageCollisionPoint osageCollisionPointGroups[14][28];

} } // namespace shenmue::s2

#endif

