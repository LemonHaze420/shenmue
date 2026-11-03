#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/numeric_event_dc.h"
#include "s2/event_sleep_dc.h"

#define AREA_0308_A209 SHEN_NAME4('a', '2', '0', '9')
#define AREA_0308_A207 SHEN_NAME4('a', '2', '0', '7')
#define AREA_0308_A208 SHEN_NAME4('a', '2', '0', '8')
#define AREA_0308_A270 SHEN_NAME4('a', '2', '7', '0')
#define AREA_0308_A047 SHEN_NAME4('a', '0', '4', '7')
#define AREA_0308_A520 SHEN_NAME4('a', '5', '2', '0')
#define AREA_0308_A014 SHEN_NAME4('a', '0', '1', '4')
#define AREA_0308_CLJ SHEN_NAME4('C', 'L', 'J', '_')
#define AREA_0308_WON SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0308_JOP SHEN_NAME4('J', 'O', 'P', '_')
#define AREA_0308_REN SHEN_NAME4('R', 'E', 'N', '_')
#define AREA_0308_LIGHT SHEN_NAME4('l', '0', '0', '1')
#define AREA_0308_RETURN SHEN_NAME4('Q', 'R', 'R', '0')
extern shen_name area0308Characters[];
void Start0308(void);
void Update0308(void *taskData);
void Queue0308Author(AreaAuthorTaskParameter *parent);
void Update0308Author(void *taskData);
void Setup0308Environment(void);
void Cleanup0308(AreaAuthorTaskParameter *parent);
void Prepare0308Characters(void);
void Restore0308Characters(void);
#else
#pragma once

#include "s2/area_event.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0308_RYO_{SHEN_NAME(RYO_)};
inline constexpr shen_name AREA_0308_a209{SHEN_NAME(a209)};
inline constexpr shen_name AREA_0308_a207{SHEN_NAME(a207)};
inline constexpr shen_name AREA_0308_a208{SHEN_NAME(a208)};
inline constexpr shen_name AREA_0308_a270{SHEN_NAME(a270)};
inline constexpr shen_name AREA_0308_a047{SHEN_NAME(a047)};
inline constexpr shen_name AREA_0308_a520{SHEN_NAME(a520)};
inline constexpr shen_name AREA_0308_a014{SHEN_NAME(a014)};
inline constexpr shen_name AREA_0308_CLJ_{SHEN_NAME(CLJ_)};
inline constexpr shen_name AREA_0308_WON_{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0308_JOP_{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0308_REN_{SHEN_NAME(REN_)};
inline constexpr shen_name AREA_0308_CHARACTERS[]{AREA_0308_CLJ_, AREA_0308_WON_, AREA_0308_JOP_, AREA_0308_REN_, AUTHOR_VIEWER_INVALID_CHARACTER};
inline constexpr char AREA_0308_MOTION[] = "0308A.MOT", AREA_0308_CONVERSATION[] = "0308";

inline constexpr shen_name AREA_0308_LIGHT{SHEN_NAME(l001)}, AREA_0308_RETURN{SHEN_NAME(QRR0)};
struct Area0308LoopParameter {
    AuthorViewerTaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(Area0308LoopParameter) == 16);
static_assert(offsetof(Area0308LoopParameter, currentFrame) == 8);
void Initialize0308();
void Start0308(std::uintptr_t argument);
void Update0308(void *taskData);
void Cleanup0308(AuthorViewerTaskParameter *parent);
std::int64_t Update0308Author(Area0308LoopParameter *child);
void Update0308AuthorTask(void *taskData);
void Prepare0308Characters();
void Restore0308Characters();
extern const ScriptAreaDefinition area0308ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
