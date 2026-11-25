#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#define HIDE_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define HIDE_REN SHEN_NAME4('R', 'E', 'N', '_')
#define HIDE_TASK SHEN_NAME4('h', 'i', 'd', 'e')
#define HIDE_DOOR SHEN_NAME4('R', '1', '4', '8')
#define HIDE_AUTHOR SHEN_NAME4('A', 'U', 'T', 'H')
#define HIDE_LIGHT SHEN_NAME4('L', 'G', 'T', '1')
#define HIDE_PACKAGE "0214"
#define HIDE_MEDIA "0214A000"
void HIDEMain(unsigned int argument);
void SetupRyoForHIDE(void);
void HIDEPlayAuthor(void *);
void HIDEWaitAuthor(void *);
void HIDEEndRoom(void *);
void EnableHideDoor(void);
#else

#include "common/math.hpp"
#include "s2/area_event.hpp"

namespace shenmue::s2 {

inline constexpr shen_name HIDE_AREA{SHEN_NAME(HIDE)},
    HIDE_TASK{SHEN_NAME(hide)}, HIDE_RYO{SHEN_NAME(RYO_)},
    HIDE_REN{SHEN_NAME(REN_)};
inline constexpr shen_name HIDE_DOOR{SHEN_NAME(R148)},
    HIDE_AUTHOR{SHEN_NAME(AUTH)}, HIDE_LIGHT{SHEN_NAME(LGT1)};
inline constexpr Vector4 HIDE_REFBOARD{
    .x = 1.0f, .y = 0.3f, .z = 0.3f, .w = 0.3f};
inline constexpr char HIDE_MEDIA[] = "0214A000", HIDE_PACKAGE[] = "0214";
void HIDEInit();
void HIDEMain(std::uintptr_t argument);
std::uint64_t SetupRyoForHIDE();
void HIDEPlayAuthor(void *taskData);
void HIDEWaitAuthor(void *taskData);
void HIDEEndRoom(void *taskData);
extern const ScriptAreaDefinition hideScriptDefinition;

} // namespace shenmue::s2

#endif
