#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2 && SHENMUE_KIND == SHENMUE_KIND_GAMEJAM

#include "s2/flag_data.hpp"

namespace shenmue {
namespace s2 {

struct ConversationBuffer;
struct PakGlobalData;
struct SearchTarget;

struct ShenPlayerData {
    std::uint16_t fadeHoldFrames;
    std::uint16_t eventTransitionPhase;
    std::uint16_t retained04;
    std::int16_t sleepScheduleFlag;
    std::int16_t contextIndex;
    std::int16_t queryArgument;
    std::uint8_t freeQuestActive;
    std::byte retained0d[3];
    std::uint32_t flags;
    FlagRecord *firstRecord;
    FlagRecord *triggerRecord;
    std::int32_t idleEvaluations;
    PakGlobalData *pendingPack;
    std::int32_t pendingEventType;
    SearchTarget *targets;
    shen_name context;
    FlagRecord *record;
    FlagRecord *deadlineRecord;
    std::int32_t updateCount;
    std::int32_t queryId;
    shen_name callbackContext;
    FlagRecord *deferredRecord;
    std::int32_t messageFrames;
    ConversationBuffer *voice;
    std::int32_t callDepth;
    FlagRecord *returnRecords[10];
};
typedef char Ownerflag_context_dchppLayoutCheck0[(sizeof(ShenPlayerData) == 0x7c) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck1[(offsetof(ShenPlayerData, sleepScheduleFlag) == 6) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck2[(offsetof(ShenPlayerData, contextIndex) == 8) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck3[(offsetof(ShenPlayerData, flags) == 0x10) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck4[(offsetof(ShenPlayerData, firstRecord) == 0x14) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck5[(offsetof(ShenPlayerData, targets) == 0x28) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck6[(offsetof(ShenPlayerData, record) == 0x30) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck7[(offsetof(ShenPlayerData, callbackContext) == 0x40) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck8[(offsetof(ShenPlayerData, callDepth) == 0x50) ? 1 : -1];
typedef char Ownerflag_context_dchppLayoutCheck9[(offsetof(ShenPlayerData, returnRecords) == 0x54) ? 1 : -1];

extern ShenPlayerData shenPlayerData;
extern FlagRecord *g_selectedFlagCandidate;
std::int32_t shenEvaluateFlag(ShenPlayerData &data, std::int32_t mode);
std::int32_t shenUpdateFlagEvaluation(std::int32_t mode);
std::int32_t shenEvaluateFlagFor(std::int32_t subject, std::int8_t mode);
std::int32_t scenEvaluateCurrentSequence(shen_name name, std::int8_t mode);
std::int32_t ryoIsCurrentScenarioFlagSatisfied();
SearchTarget *shenFindFlagTarget(shen_name name);
std::int32_t shenUpdateFlagTargets(SearchTarget *targets);
std::int32_t flagInitContext(ShenPlayerData &data, shen_name context);
std::int32_t shenUpdatePlayerDataFlags(std::int32_t active);
void EV_stepSetFlags(std::uint8_t mode, std::int32_t flag);
std::int32_t shenGetMinutesUntilNextFrameMinute();

std::uint32_t shenIsCurfewSceneChange();
std::int16_t shenGetSleepScheduleFlag();
void shenSetFreeQuestInactive();
std::int32_t shenScheduleFlagCallback(shen_name name, std::int16_t minute);
std::int32_t npcSetCycleTimeLimitExpiry(shen_name name, std::int16_t timeLimit);

} // namespace s2
} // namespace shenmue

#endif
