#pragma once
#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/events/event_dc.h"
#define QTE_GAME_ENTRY_TASK TASK_EVNT
#define QTE_GAME_TITLE SHEN_NAME4('T', 'I', 'T', 'L')
#define QTE_GAME_MACHINE SHEN_NAME4('Q', 'T', 'E', 'M')
#else
#include "s2/hlib.hpp"
#include "common/math.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>
namespace shenmue::s2 {
inline constexpr shen_name QTE_GAME_ENTRY_TASK{SHEN_NAME(EVNT)}, QTE_GAME_TITLE{SHEN_NAME(TITL)}, QTE_GAME_MACHINE{SHEN_NAME(QTEM)};
#include "qte_title_data.inl"
#include "qte_machine_data.inl"
#endif

typedef struct QteGameEntryParameter {
#if defined(SHENMUE_EVENT_OVERLAY)
    unsigned int reserved00;
    HLibTask *self;
#else
    std::uint64_t reserved00;
    HLib::Task *self;
#endif
#if defined(SHENMUE_EVENT_OVERLAY)
    unsigned int state, exciteBest, exciteContinued, titleScore2, titleScore0, titleScore1;
#else
    std::uint32_t state;
    std::uint32_t exciteBest;
    std::uint32_t exciteContinued;
    std::uint32_t titleScore2;
    std::uint32_t titleScore0;
    std::uint32_t titleScore1;
#endif
} QteGameEntryParameter;
#if !defined(SHENMUE_EVENT_OVERLAY)
static_assert(sizeof(QteGameEntryParameter) == 0x28);
static_assert(offsetof(QteGameEntryParameter, self) == 8);
static_assert(offsetof(QteGameEntryParameter, titleScore0) == 0x20);
#endif

void InitializeQteTitleDisplay(QteGameEntryParameter *parameter);
void InitializeQteMachineDisplay(QteGameEntryParameter *parameter);
void EVT_QteGameEnter_Callback(void *parameter);
void EVT_QteGameEnter(void);
#if !defined(SHENMUE_EVENT_OVERLAY)
} // namespace shenmue::s2
#endif
