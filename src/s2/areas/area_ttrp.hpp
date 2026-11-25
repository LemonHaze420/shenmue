#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#include "s2/event_utility_dc.h"
#define TTRP_TASK SHEN_NAME4('t', 't', 'r', 'p')
#define TTRP_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define TTRP_REN SHEN_NAME4('R', 'E', 'N', '_')
#define TTRP_CHR SHEN_NAME4('C', 'H', 'R', '_')
#define TTRP_SCX SHEN_NAME4('S', 'C', 'X', '_')
#define TTRP_TGY SHEN_NAME4('T', 'G', 'Y', '_')
#define TTRP_A202 SHEN_NAME4('a', '2', '0', '2')
#define TTRP_A203 SHEN_NAME4('a', '2', '0', '3')
#define TTRP_AUT1 SHEN_NAME4('A', 'U', 'T', '1')
#define TTRP_AUT2 SHEN_NAME4('A', 'U', 'T', '2')
#define TTRP_LIGHT SHEN_NAME4('L', 'G', 'T', '1')
#define TTRP_INVALID 0xffffffffU
extern const shen_name TTRP_CHARACTERS[];
extern const shen_name TTRP_FREE_CHARACTERS[];
void TtrpMain(unsigned int argument);
void TtrpPlayAuthorCallback(void *taskData);
void TtrpWaitFirstAuthorCallback(void *taskData);
void TtrpWaitSecondAuthorCallback(void *taskData);
void TtrpEndCallback(void *taskData);
void PrepareTtrpCharacters(void);
void FreeTtrpCharacters(void);
#else

#include "s2/area_event.hpp"
#include "common/math.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2 {

inline constexpr shen_name TTRP_TASK{SHEN_NAME(ttrp)};
inline constexpr shen_name TTRP_RYO{SHEN_NAME(RYO_)}, TTRP_REN{SHEN_NAME(REN_)}, TTRP_CHR{SHEN_NAME(CHR_)};
inline constexpr shen_name TTRP_SCX{SHEN_NAME(SCX_)}, TTRP_TGY{SHEN_NAME(TGY_)};
inline constexpr shen_name TTRP_AUT1{SHEN_NAME(AUT1)}, TTRP_AUT2{SHEN_NAME(AUT2)}, TTRP_LIGHT{SHEN_NAME(LGT1)};
inline constexpr shen_name TTRP_A202{SHEN_NAME(a202)}, TTRP_A203{SHEN_NAME(a203)}, TTRP_INVALID{0xffffffffU};
inline constexpr shen_name TTRP_CHARACTERS[]{TTRP_REN, TTRP_CHR, TTRP_SCX, TTRP_TGY, TTRP_INVALID};
inline constexpr Vector4 TTRP_REFLECTOR{.x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
void TtrpInit();
void TtrpDeveloperNullsub();
void TtrpMain(std::uintptr_t argument);
void PrepareTtrpCharacters();
void TtrpPlayAuthorCallback(void *taskData);
void TtrpWaitFirstAuthorCallback(void *taskData);
void TtrpWaitSecondAuthorCallback(void *taskData);
void TtrpEndCallback(void *taskData);
extern const ScriptAreaDefinition ttrpScriptDefinition;

} // namespace shenmue::s2

#endif
