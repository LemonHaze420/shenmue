#include "s2/areas/area_bgsm.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/motion_work.hpp"
#include "s2/map_control.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
void BgsmSayDice(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    static constexpr const char *voices[]{"026", "027", "028", "029", "030", "031"};
    for (std::uint8_t die : work.dice)
        if (die >= 1 && die <= 6)
            BgsmAppendVoice(1, voices[die - 1]);
    HLib::SetCurrentTaskTo(BgsmJudgeCombination);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(BgsmWaitVoice, 6, 0x0b, sizeof(BgsmVoiceWork), BGSM_TASK);
    BgsmVoiceWork *voice = static_cast<BgsmVoiceWork *>(HLib::GetTaskParameter(task));
    voice->parent = work.self;
    voice->delay = 8;
    voice->index = 0;
    voice->reserved10 = 0;
    voice->conversationFrames = 0;
    // This producer deliberately leaves the voice flags at +0x18 unwritten.
    bgsmVoices[bgsmVoiceCount][0] = 0;
    bgsmVoiceCount = 0;
    if (voice->parent)
        HLib::SetupTimedTask(voice->parent, 0);
}
void BgsmJudgeCombination(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    const std::int32_t a = work.dice[0], b = work.dice[1], c = work.dice[2];
    const std::uint32_t sum = a + b + c;
    work.result = sum > 10 ? 16 : 8;
    if (a == b && b == c && (work.combinations & 1)) {
        work.result |= 1;
        work.pairFace = static_cast<std::int8_t>(b);
    }
    if ((a == b || b == c || a == c) && (work.combinations & 2)) {
        work.pairFace = static_cast<std::int8_t>(a == b ? a : (b == c ? b : c));
        work.result |= 2;
    }
    if (sum - 10 <= 1 && (work.combinations & 32))
        work.result |= 32;
    if (a != b && b != c && a != c && 2 * c - a - b == 3 && (work.combinations & 4))
        work.result |= 4;
    HLib::SetCurrentTaskTo(BgsmSayCombination);
}
void BgsmSayCombination(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (work.result & 1) {
        BgsmAppendVoice(1, "033");
        EVT_AT_SetSound(1226, 10);
    } else if (work.result & 2)
        BgsmAppendVoice(1, "072");
    else if (work.result & 4)
        BgsmAppendVoice(1, "032");
    else if (work.result & 32)
        BgsmAppendVoice(1, "074");
    BgsmAppendVoice(1, (work.result & 8) ? "034" : "035");
    HLib::SetCurrentTaskTo(BgsmJudge);
    BgsmQueueVoices(work.self);
}
void BgsmJudge(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    for (shen_name die : BGSM_DICE)
        EV_DisableCharacterDisplay(die);
    EV_DisableCharacterDisplay(BGSM_CASH);
    if (g_currentMap.name != BGSM_AREA.name)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(BGSM_PLAYED_FLAGS[bgsmOpponentIndex]), 1);
    EventSetPoint point{.character = work.opponent,
        .target = BGSM_RYO,
        .targetBone = 5,
        .trackingMode = 3,
        .positionMode = 0,
        .duration = 0,
        .flags = 0x20000000U,
        .frame = 0,
        .cleanup = 0,
        .delay = 0};
    EVT_AT_SetPoint_Callback(&point);
    point.character = BGSM_RYO;
    point.target = work.opponent;
    EVT_AT_SetPoint_Callback(&point);
    if (!(work.result & (1U << (static_cast<std::uint16_t>(work.choice) & 31)))) {
        HLib::SetCurrentTaskTo(BgsmLose);
        return;
    }
    switch (work.choice) {
    case 0:
        if (work.chosenFace == -1)
            HLib::SetCurrentTaskTo(BgsmTriple);
        else
            HLib::SetCurrentTaskTo(work.chosenFace == work.dice[1] ? BgsmSpecificTriple : BgsmLose);
        break;
    case 1:
        if (work.chosenFace != -1)
            HLib::SetCurrentTaskTo(work.chosenFace == work.pairFace ? BgsmSpecificPair : BgsmLose);
        break;
    case 2:
        HLib::SetCurrentTaskTo(BgsmSequence);
        break;
    case 3:
    case 4:
        HLib::SetCurrentTaskTo(BgsmNormalWin);
        break;
    case 5:
        HLib::SetCurrentTaskTo(BgsmMiddle);
        break;
    default:
        break;
    }
}
void BgsmTriple(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_LOSE_MOTIONS[bgsmOpponentPose]);
    BgsmAppendVoice(1, "065");
    BgsmAppendVoice(1, "066");
    EVT_AT_SetSound(1225, 10);
    EVT_AT_SetSound(1226, 40);
    EVT_AT_SetSound(347, 50);
    AddThisToCashOrSegaCoins(30 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmSpecificTriple(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_LOSE_MOTIONS[bgsmOpponentPose]);
    BgsmAppendVoice(1, shenGetRandomInt32(2) ? "069" : "070");
    BgsmAppendVoice(1, "071");
    EVT_AT_SetSound(1225, 20);
    EVT_AT_SetSound(1225, 40);
    EVT_AT_SetSound(346, 40);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Difficulty, 16, 1);
    AddThisToCashOrSegaCoins(200 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmSequence(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_HAPPY_MOTIONS[bgsmOpponentPose]);
    BgsmAppendVoice(1, shenGetRandomInt32(2) ? "036" : "037");
    EVT_AT_SetSound(1225, 20);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Pitiable, 16, 1);
    AddThisToCashOrSegaCoins(8 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmMiddle(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_LOSE_MOTIONS[bgsmOpponentPose]);
    EVT_AT_SetSound(1225, 20);
    BgsmAppendVoice(1, shenGetRandomInt32(2) ? "067" : "068");
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Pitiable, 16, 1);
    AddThisToCashOrSegaCoins(4 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmSpecificPair(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_LOSE_MOTIONS[bgsmOpponentPose]);
    BgsmAppendVoice(1, shenGetRandomInt32(2) ? "038" : "039");
    BgsmAppendVoice(1, "040");
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Pitiable, 16, 1);
    EVT_AT_SetSound(1226, 10);
    EVT_AT_SetSound(1225, 20);
    AddThisToCashOrSegaCoins(12 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmNormalWin(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    BgsmMotion(work.opponent, BGSM_LOSE_MOTIONS[bgsmOpponentPose]);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Pitiable, 16, 1);
    static constexpr const char *voices[]{"041", "042", "043", "044", "045"};
    BgsmAppendVoice(1, voices[shenGetRandomInt32(5)]);
    EVT_AT_SetSound(1225, 10);
    AddThisToCashOrSegaCoins(2 * work.wager, ECashOrCoins::Cash2);
    HLib::SetCurrentTaskTo(shenGetMinutesUntilNextFrameMinute() ? BgsmWin : BgsmSleep);
    BgsmQueueVoices(work.self);
}
void BgsmWin(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (GetShouldRenderIcons(RenderIconSlot::Any))
        return;
    static constexpr const char *voices[]{"050", "051", "052"};
    BgsmAppendVoice(1, voices[shenGetRandomInt32(3)]);
    if (bgsmStartingCash > static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "055" : "056");
    if (bgsmStartingCash < static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "053" : "054");
    const bool insufficient = BgsmCheckMoney(work);
    BgsmQueueVoices(work.self, insufficient ? 0 : 0x20000000U);
}
void BgsmLose(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4600);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Smile, 16, 1);
    static constexpr const char *lossVoices[]{"046", "047", "048", "049"};
    BgsmAppendVoice(1, lossVoices[shenGetRandomInt32(4)]);
    if (!shenGetMinutesUntilNextFrameMinute()) {
        HLib::SetCurrentTaskTo(BgsmSleep);
        BgsmQueueVoices(work.self);
        return;
    }
    static constexpr const char *voices[]{"050", "051", "052"};
    BgsmAppendVoice(1, voices[shenGetRandomInt32(3)]);
    if (bgsmStartingCash > static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "059" : "060");
    if (bgsmStartingCash < static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)))
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "057" : "058");
    BgsmMotion(work.opponent, BGSM_HAPPY_MOTIONS[bgsmOpponentPose]);
    EVT_AT_SetSound(1702, 10);
    const bool insufficient = BgsmCheckMoney(work);
    BgsmQueueVoices(work.self, insufficient ? 0 : 0x20000000U);
}
} // namespace shenmue::s2::areas
