#pragma once

#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"

#define AREA_0008_RYO SHEN_NAME4('R', 'Y', 'O', '_')
#define AREA_0008_A012 SHEN_NAME4('a', '0', '1', '2')
#define AREA_0008_A195 SHEN_NAME4('a', '1', '9', '5')
#define AREA_0008_HNG SHEN_NAME4('H', 'N', 'G', '_')
#define AREA_0008_SYO SHEN_NAME4('S', 'Y', 'O', '_')

typedef struct E0008TaskParameter {
    HLibTask *self;
    void *motion;
    void *handMotion;
    short selection, state;
} E0008TaskParameter;
typedef struct E0008LoopParameter {
    E0008TaskParameter *parent;
    unsigned short currentFrame;
} E0008LoopParameter;

extern shen_name e0008Characters[];
void E0008Main(void);
void E0008MainCallback(void *taskData);
void QueueE0008Loop(E0008TaskParameter *parent);
void E0008LoopCallback(void *taskData);
void ConfigureE0008Environment(void);
void CleanupE0008(E0008TaskParameter *parent);
void SetupE0008Characters(void);
void RestoreE0008Characters(void);
void PrepareE0008Character(shen_name name);
void RestoreE0008Character(shen_name name);
void HideE0008Prop(shen_name name);
void ShowE0008Prop(shen_name name);
void E0008WaveCallback(void *taskData);
#else
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"

namespace shenmue::s2::areas {

inline constexpr shen_name AREA_0008_AREA{SHEN_NAME(0008)};
inline constexpr shen_name AREA_0008_A012{SHEN_NAME(a012)};
inline constexpr shen_name AREA_0008_A195{SHEN_NAME(a195)};
inline constexpr shen_name AREA_0008_SYO_{SHEN_NAME(SYO_)};
inline constexpr shen_name AREA_0008_HNG_{SHEN_NAME(HNG_)};
inline constexpr shen_name AREA_0008_GAU_{SHEN_NAME(GAU_)};
inline constexpr shen_name AREA_0008_0000{SHEN_NAME(0000)};
inline constexpr shen_name AREA_0008_CHARACTERS[]{AREA_0008_SYO_, AREA_0008_HNG_, AREA_0008_GAU_, AREA_AUTHOR_INVALID};
inline constexpr char AREA_0008_MOTION[] = "0008A.MOT", AREA_0008_CONVERSATION[] = "0008";
inline constexpr const char *AREA_0008_DEBUG_NAMES[]{"0008A000.c\n", "0008A012.c\n"};
inline constexpr char AREA_0008_HAND_MOTION[] = "HM_0008E.BIN";
struct E0008TaskParameter {
    HLib::Task *self;
    std::uint32_t motion, reserved0c;
    void *handMotion;
    std::int16_t selection, state;
    std::uint32_t reserved1c;
};
struct E0008LoopParameter {
    E0008TaskParameter *parent;
    std::uint16_t currentFrame;
};
static_assert(sizeof(E0008TaskParameter) == 0x20);
static_assert(offsetof(E0008TaskParameter, handMotion) == 0x10);
static_assert(offsetof(E0008TaskParameter, selection) == 0x18);
static_assert(offsetof(E0008TaskParameter, state) == 0x1a);
static_assert(sizeof(E0008LoopParameter) == 0x10);
extern const ScriptAreaDefinition area0008ScriptDefinition;

} // namespace shenmue::s2::areas
#endif
