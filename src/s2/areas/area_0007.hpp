#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/event_utility_dc.h"
#include "s2/event_queued_dc.h"
typedef struct E0007TaskParameter {
    HLibTask *self;
    int reserved;
    short selection, state;
} E0007TaskParameter;
typedef struct E0007LoopParameter { E0007TaskParameter *parent; } E0007LoopParameter;
extern shen_name e0007DayCharacters[];
extern shen_name *e0007Characters[];
void E0007Main(void);
void E0007MainCallback(void *data);
void QueueE0007Loop(E0007TaskParameter *parent);
void E0007LoopCallback(void *data);
void ConfigureE0007Environment(void);
void CleanupE0007(E0007TaskParameter *parent);
void SetupE0007Characters(short selection);
void RestoreE0007Characters(short selection);
void PrepareE0007Character(shen_name name);
void RestoreE0007Character(shen_name name);
void HideE0007Prop(shen_name name);
void ShowE0007Prop(shen_name name);
void E0007WaveCallback(void *data);
#else
#include "s2/area_event.hpp"
#include "s2/areas/numeric_event.hpp"
namespace shenmue::s2::areas {
inline constexpr shen_name AREA_0007_AREA{SHEN_NAME(0007)}, AREA_0007_AR02{SHEN_NAME(AR02)};
inline constexpr shen_name AREA_0007_LIGHT{SHEN_NAME(0000)}, AREA_0007_RCKO{SHEN_NAME(RCKO)};
inline constexpr shen_name AREA_0007_AA39{SHEN_NAME(aa39) | 0x40000U};
inline constexpr shen_name AREA_0007_AUTHORS[]{SHEN_NAME(007A), SHEN_NAME(007B)};
inline constexpr shen_name AREA_0007_JOP{SHEN_NAME(JOP_)};
inline constexpr shen_name AREA_0007_SHI{SHEN_NAME(SHI_)};
inline constexpr shen_name AREA_0007_UNH{SHEN_NAME(UNH_)};
inline constexpr shen_name AREA_0007_GOI{SHEN_NAME(GOI_)};
inline constexpr shen_name AREA_0007_HTK{SHEN_NAME(HTK_)};
inline constexpr std::uint16_t AREA_0007_DUCK_GUTS_FLAG = 0x8b4U;
inline constexpr shen_name AREA_0007_CHARACTERS[]{
    SHEN_NAME(JOP_),
    SHEN_NAME(SHI_),
    SHEN_NAME(UNH_),
    SHEN_NAME(GOI_),
    SHEN_NAME(HTK_),
    shen_name{0xffffffffU},
};
extern const ScriptAreaDefinition area0007ScriptDefinition;
} // namespace shenmue::s2::areas

#endif
