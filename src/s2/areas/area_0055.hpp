#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0059.hpp"

#define AREA_0055_AB00 SHEN_NAME4('A', 'B', '0', '0')
#define AREA_0055_WON SHEN_NAME4('W', 'O', 'N', '_')
#define AREA_0055_NDAY SHEN_NAME4('N', 'D', 'A', 'Y')
#define AREA_0055_A126 SHEN_NAME4('a', '1', '2', '6')
#define AREA_0055_A142 SHEN_NAME4('a', '1', '4', '2')
#define AREA_0055_A506 SHEN_NAME4('a', '5', '0', '6')
#define AREA_0055_AA50 SHEN_NAME4('a', 'a', '5', '0')
#define AREA_0055_A504 SHEN_NAME4('a', '5', '0', '4')
#define AREA_0055_A016 SHEN_NAME4('a', '0', '1', '6')
#define AREA_0055_A298 SHEN_NAME4('a', '2', '9', '8')
typedef struct E0055TaskParameter {
    HLibTask *self;
    void *motion;
    short selection, state;
    void *voice;
} E0055TaskParameter;
typedef struct E0055LoopTaskParameter {
    E0055TaskParameter *parent;
    unsigned short currentFrame;
} E0055LoopTaskParameter;
extern shen_name area0055Characters[];
void E0055Main(void);
void E0055MainCallback(void *taskData);
void QueueE0055Loop(E0055TaskParameter *parent);
void E0055LoopCallback(void *taskData);
void SetupE0055Environment(void);
void CleanupE0055(E0055TaskParameter *parent);
void SetupE0055Evening(void);
void SetupE0055Morning(void);
void SetupE0055Characters(short selection);
void RestoreE0055Characters(short selection);
void HideAuthorCharacterDisplay(shen_name character);
void ShowAuthorCharacterDisplay(shen_name character);
void HideAuthorPropDisplay(shen_name prop);
void ShowAuthorPropDisplay(shen_name prop);
void HarborAuthorShipCallback(void *taskData);
#else
#pragma once
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/conversation.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0055_AREA{SHEN_NAME(0055)}, AREA_0055_AB00{SHEN_NAME(AB00)}, AREA_0055_WON{SHEN_NAME(WON_)};
inline constexpr shen_name AREA_0055_LIGHT{SHEN_NAME(0000)}, AREA_0055_NDAY{SHEN_NAME(NDAY)};
inline constexpr shen_name AREA_0055_A126{SHEN_NAME(a126)}, AREA_0055_A142{SHEN_NAME(a142)}, AREA_0055_A506{SHEN_NAME(a506)};
inline constexpr shen_name AREA_0055_AA50{SHEN_NAME(aa50)}, AREA_0055_A504{SHEN_NAME(a504)}, AREA_0055_A016{SHEN_NAME(a016)};
inline constexpr shen_name AREA_0055_A298{SHEN_NAME(a298)};
inline constexpr shen_name AREA_0055_CHARACTERS[]{AREA_0055_WON, AREA_AUTHOR_INVALID};
struct E0055TaskParameter {
    AreaAuthorTaskParameter author;
    ConversationBuffer *voice;
};
static_assert(sizeof(E0055TaskParameter) == 24);
static_assert(offsetof(E0055TaskParameter, voice) == 16);
extern const ScriptAreaDefinition area0055ScriptDefinition;
} // namespace shenmue::s2::areas
#endif
