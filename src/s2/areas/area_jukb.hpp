#pragma once
#include "s2/area_event.hpp"
#include "s2/math.hpp"
#include <cstddef>
#include <cstdint>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
inline constexpr shen_name JUKB_TASK{SHEN_NAME(EVNT)};
inline constexpr shen_name JUKB_PLAYER{SHEN_NAME(RYO_)};
inline constexpr shen_name JUKB_BOX{SHEN_NAME(JBOX)};
inline constexpr shen_name JUKB_FINGER{SHEN_NAME(YUBI)};
inline constexpr shen_name JUKB_COIN{SHEN_NAME(JUCO)};
inline constexpr shen_name JUKB_INPUT{SHEN_NAME(BTEV)};
inline constexpr shen_name JUKB_AKS1{SHEN_NAME(AKS1)};
inline constexpr shen_name JUKB_QTB3{SHEN_NAME(QTB3)};
inline constexpr shen_name JUKB_ARA0{SHEN_NAME(ARA0)};
inline constexpr shen_name JUKB_AKT0{SHEN_NAME(AKT0)};
inline constexpr shen_name JUKB_QCFE{SHEN_NAME(QCFE)};
inline constexpr shen_name JUKB_WECF{SHEN_NAME(WECF)};
inline constexpr shen_name JUKB_QB00{SHEN_NAME(QB00)};
inline constexpr shen_name JUKB_QCAE{SHEN_NAME(QCAE)};
inline constexpr shen_name JUKB_ARC0{SHEN_NAME(ARC0)};
inline constexpr shen_name JUKB_WES1{SHEN_NAME(WES1)};
inline constexpr shen_name JUKB_WEG0{SHEN_NAME(WEG0)};
inline constexpr shen_name JUKB_QA22{SHEN_NAME(QA22)};
inline constexpr shen_name JUKB_TARGETS[9]{
    SHEN_NAME(TJT1), SHEN_NAME(TJT2), SHEN_NAME(TJT3), SHEN_NAME(TJT4), SHEN_NAME(TJT5), SHEN_NAME(TJT6), SHEN_NAME(TJT7), SHEN_NAME(TJT8), SHEN_NAME(TJT9)};
#pragma pack(push, 4)
struct JukbTaskData {
    HLib::Task *self;
    std::uint64_t reserved08;
    std::uint32_t jukeboxIndex, mainState, frame;
    std::uint32_t leftPhase, rightPhase, buttonPhase, selection, selected, selectFrame, animationFrame;
    std::uint32_t reserved38, timedOut, reserved40, reserved44;
    void *dialogue;
    std::byte reserved50[16];
};
#pragma pack(pop)
static_assert(sizeof(JukbTaskData) == 96);
static_assert(offsetof(JukbTaskData, leftPhase) == 28);
static_assert(offsetof(JukbTaskData, dialogue) == 72);
void AreaJUKBInit();
void AreaJUKBMain(std::uintptr_t parameter);
void JUKBCheckCallback(void *data);
void JUKBMainCallback(void *data);
void JUKBWaitAnswerCallback(void *data);
void JUKBWaitAnswerLoopCallback(void *data);
void JUKBUseMoneyCallback(void *data);
void JUKBSelectInitCallback(void *data);
void JUKBSelectLoopCallback(void *data);
void JUKBOpenMechanismCallback(void *data);
void JUKBChangeRecordCallback(void *data);
void JUKBCloseMechanismCallback(void *data);
void JUKBStartRecordCallback(void *data);
void JUKBStopRecordCallback(void *data);
void JUKBExitCallback(void *data);
extern const ScriptAreaDefinition areaJUKBScriptDefinition;
} // namespace shenmue::s2::areas
