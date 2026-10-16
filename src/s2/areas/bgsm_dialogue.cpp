#include "s2/areas/area_bgsm.hpp"
#include "s2/author.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/flag.hpp"
#include "s2/motion_work.hpp"
#include "s2/save.hpp"
#include "s2/sk_event.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
void BgsmTalk(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    EventFilmControl fade{.cleanup = 0,
        .frames = 30,
        .startAlpha = 255,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 0,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = 0};
    EVT_AT_MenCtrl_Callback(&fade);
    BgsmPosition(BGSM_RYO, BGSM_BASE, {0, 0, -2});
    BgsmAngle(BGSM_RYO, BGSM_BASE, 180);
    BgsmPosition(work.opponent, BGSM_BASE, {0, 0, 1.05f});
    BgsmAngle(work.opponent, BGSM_BASE, 22.5f);
    BgsmMotion(BGSM_RYO, 0xf002, 0x40000000U);
    BgsmMotion(work.opponent, BGSM_IDLE_MOTIONS[bgsmOpponentPose]);
    BgsmAppendVoice(1, "001");
    BgsmAppendVoice(1, "002");
    BgsmCamera(4000);
    BgsmCheckMoney(work);
    if (!shenGetMinutesUntilNextFrameMinute())
        HLib::SetCurrentTaskTo(BgsmSleep);
    BgsmQueueVoices(work.self);
}
bool BgsmCheckMoney(BgsmWork &work) {
    if (static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= work.minimumBet) {
        HLib::SetCurrentTaskTo(BgsmInfoSelect);
        return false;
    }
    HLib::SetCurrentTaskTo(BgsmNoMoney);
    return true;
}
void BgsmNoMoney(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmCamera(4701);
    BgsmPosition(BGSM_RYO, BGSM_BASE, {0, 0, -.6f});
    BgsmAngle(BGSM_RYO, BGSM_BASE, 180);
    BgsmMotion(BGSM_RYO, 0xf002, 0x40000000U);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Normal, 16, 1);
    BgsmAppendVoice(0, "004");
    BgsmAppendVoice(1, "018");
    HLib::SetCurrentTaskTo(BgsmStop);
    BgsmQueueVoices(work.self);
}
void BgsmSleep(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    BgsmPosition(BGSM_RYO, BGSM_BASE, {0, 0, -.6f});
    BgsmAngle(BGSM_RYO, BGSM_BASE, 180);
    BgsmMotion(BGSM_RYO, 0xf002, 0x40000000U);
    BgsmCamera(4701);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Normal, 16, 1);
    BgsmAppendVoice(1, "063");
    BgsmAppendVoice(0, "012");
    BgsmAppendVoice(1, "064");
    HLib::SetCurrentTaskTo(BgsmStop);
    BgsmQueueVoices(work.self);
}
void BgsmInfoSelect(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (work.state) {
        if (work.state == 1) {
            const std::int16_t choice = shenGetCtrlSpeakIconFlag();
            HLib::SetCurrentTaskTo(choice == 36 ? BgsmPlay : (choice == 369 ? BgsmInfo : BgsmNoGame));
            work.state = 0;
        }
        return;
    }
    BgsmCamera(4100);
    BgsmPosition(BGSM_RYO, BGSM_BASE, {0, 0, -.6f});
    BgsmAngle(BGSM_RYO, BGSM_BASE, 180);
    BgsmPosition(work.opponent, BGSM_BASE, {0, 0, 1.05f});
    BgsmAngle(work.opponent, BGSM_BASE, 22.5f);
    BgsmMotion(BGSM_RYO, 0xf002, 0x40000000U);
    BgsmMotion(work.opponent, BGSM_IDLE_MOTIONS[bgsmOpponentPose]);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(SkChoiceWaitCallback, 6, 0x0b, sizeof(SkChoiceWaitWork), BGSM_TASK);
    SkChoiceWaitWork *choice = static_cast<SkChoiceWaitWork *>(HLib::GetTaskParameter(task));
    EV_SetAuthorActiveTo(0);
    choice->self = task;
    choice->parent = work.self;
    choice->choices[0] = -1;
    choice->choices[1] = 36;
    choice->timeout = 0x7fff;
    choice->reserved1a = 0;
    choice->phase = 0;
    choice->choices[2] = 369;
    choice->choices[3] = 34;
    choice->defaultChoice = 154;
    HLib::SetupTimedTask(work.self, 0);
    EVT_AT_SetPoint(BGSM_RYO, work.opponent, 5, 3, 0, 0, 0, 0x20000000U);
    EVT_AT_SetPoint(work.opponent, BGSM_RYO, 5, 3, 0, 0, 0, 0x20000000U);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), FacialExpressionId::Normal, 16, 1);
    ++work.state;
}
void BgsmPlay(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (!work.state) {
        BgsmCamera(4200);
        BgsmAppendVoice(0, "001");
        ++work.state;
    } else {
        BgsmCamera(4203);
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "003" : "004");
        BgsmAppendVoice(1, "075");
        HLib::SetCurrentTaskTo(BgsmBet);
        work.state = 0;
    }
    BgsmQueueVoices(work.self);
}
void BgsmNoGame(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (!work.state) {
        BgsmCamera(4700);
        BgsmAppendVoice(0, shenGetRandomInt32(2) ? "008" : "009");
        ++work.state;
    } else {
        BgsmCamera(4701);
        BgsmAppendVoice(1, shenGetRandomInt32(2) ? "061" : "062");
        HLib::SetCurrentTaskTo(BgsmStop);
        work.state = 0;
    }
    BgsmQueueVoices(work.self);
}
void BgsmInfo(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    switch (work.state) {
    case 0:
        BgsmCamera(4204);
        EVT_AT_SetPoint(BGSM_RYO, work.opponent, 5, 3, 0, 0, 0, 0x20000000U);
        EVT_AT_SetPoint(work.opponent, BGSM_RYO, 5, 3, 0, 0, 0, 0x20000000U);
        BgsmMotion(work.opponent, BGSM_TALK_MOTIONS[bgsmOpponentPose], 0x10000000U);
        BgsmAppendVoice(0, shenGetRandomInt32(2) ? "002" : "003");
        ++work.state;
        break;
    case 1:
        BgsmCamera(4201);
        BgsmAppendVoice(1, "005");
        BgsmAppendVoice(1, "006");
        ++work.state;
        break;
    case 2:
        BgsmCamera(4202);
        for (const char *voice : {"007", "008", "009", "010", "011", "012", "013", "014", "015", "016", "017"})
            BgsmAppendVoice(1, voice);
        BgsmCheckMoney(work);
        if (!shenGetMinutesUntilNextFrameMinute())
            HLib::SetCurrentTaskTo(BgsmSleep);
        work.state = 0;
        break;
    default:
        break;
    }
    BgsmQueueVoices(work.self);
}
void BgsmBet(void *data) {
    BgsmWork &work = *static_cast<BgsmWork *>(data);
    if (work.state) {
        if (work.state == 1) {
            work.wager = static_cast<std::int32_t>(GetNumBet());
            if (work.wager <= 0)
                HLib::SetCurrentTaskTo(BgsmNoGame);
            else {
                BgsmPlayVoice(1, "019");
                SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)) - work.wager);
                HLib::SetCurrentTaskTo(BgsmSelectSet);
            }
            work.state = 0;
        }
        return;
    }
    EVT_AT_SetPoint(BGSM_RYO, BGSM_BASE, -1, 3, 1, 0, 0, 0x20000000U);
    BgsmCamera(4300);
    EVT_AT_SetPoint(BGSM_RYO, work.opponent, 5, 3, 0, 0, 0, 0x20000000U);
    EVT_AT_SetPoint(work.opponent, BGSM_RYO, 5, 3, 0, 0, 0, 0x20000000U);
    QueueSkMoneyIconWait(work.self, static_cast<std::int16_t>(work.maximumBet), static_cast<std::int16_t>(work.minimumBet));
    ++work.state;
}
} // namespace shenmue::s2::areas
