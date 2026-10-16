#include "s2/areas/area_uede.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/map_control.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
void Camera(std::int32_t id) {
    EventSetCamera camera{.camera = id, .eye = UEDE_BASE, .target = UEDE_BASE};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
}
void Place(shen_name character, const Vector3 &position, float angle) {
    EventSetPositionAngle placement{.character = character, .parent = UEDE_BASE, .position = position, .degrees = angle};
    EVT_AT_SetPosAng_Callback(&placement);
}
void Motion(shen_name character, std::uint32_t animation) {
    EventSetMotion motion{.character = character, .animation = animation, .speed = 1, .flags = 0x40000000U};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(static_cast<std::uint16_t>(animation));
}
void Look(shen_name character, shen_name target) {
    EventSetPoint point{.character = character, .target = target, .targetBone = 5, .trackingMode = 3, .flags = 0x20000000U};
    EVT_AT_SetPoint_Callback(&point);
}
} // namespace
void UedeAppendVoice(std::int32_t speaker, const char *suffix) {
    std::strcpy(uedeVoices[uedeVoiceCount], uedeVoicePrefixes[speaker]);
    std::strcat(uedeVoices[uedeVoiceCount], suffix);
    ++uedeVoiceCount;
}
void UedePlayVoice(std::int32_t speaker, const char *suffix, std::int32_t delay) {
    char voice[64];
    std::strcpy(voice, uedeVoicePrefixes[speaker]);
    std::strcat(voice, suffix);
    EVT_AT_SetVoice(voice, delay);
}
void UedeQueueVoices(HLib::Task *parent, std::uint32_t flags, std::int32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(UedeWaitVoices, 6, 0x0b, sizeof(UedeVoiceWork), UEDE_TASK);
    UedeVoiceWork *work = static_cast<UedeVoiceWork *>(HLib::GetTaskParameter(task));
    work->parent = parent;
    work->delay = 8;
    work->index = 0;
    work->conversationFrames = 0;
    work->flags = flags;
    uedeVoices[uedeVoiceCount][0] = 0;
    uedeVoiceCount = 0;
    if (parent)
        HLib::SetupTimedTask(parent, delay);
}
void UedeWaitVoices(void *data) {
    UedeVoiceWork &work = *static_cast<UedeVoiceWork *>(data);
    if (cvsHasValidConverseData(0)) {
        const std::int32_t previous = work.conversationFrames++;
        if (previous > 600)
            scenCvsCtrlFree(0);
        if (((work.flags & 0x40000000U) && CheckMediatedDoublePress()) || HasPressedCancel())
            scenCvsCtrlFree(0);
    } else {
        const char *voice = uedeVoices[work.index];
        if (!voice[0]) {
            if (work.parent)
                HLib::ResolveDeferredCallback(work.parent);
            HLib::DestroyCurrentTask();
            return;
        }
        EVT_AT_SetVoice(voice, work.delay);
        if (work.delay > 0)
            HLib::SetupTaskDelay(work.delay);
        ++work.index;
        work.conversationFrames = 0;
        if (!uedeVoices[work.index][0] && (work.flags & 0x20000000U)) {
            if (work.parent)
                HLib::ResolveDeferredCallback(work.parent);
            HLib::DestroyCurrentTask();
        }
    }
}
bool UedeCheckMoney(UedeWork &work) {
    const bool insufficient = static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) < work.minimumBet;
    HLib::SetCurrentTaskTo(insufficient ? UedeNoMoney : UedeSelectInfo);
    return insufficient;
}
void UedeTalk(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    if (((g_currentMap.name - SHEN_NAME(AKT0)) & 0xfdffffffU) != 0) {
        EventFilmControl film{.frames = 30, .startAlpha = 255};
        EVT_AT_MenCtrl_Callback(&film);
    }
    work.repeat = 0;
    Place(UEDE_RYO, {0, 0, 1.9f}, 0);
    Place(work.opponent, {0, 0, -.9f}, 170);
    Motion(UEDE_RYO, 0xf002);
    Motion(work.opponent, uedeIdleMotions[uedeOpponentPose]);
    Look(work.opponent, UEDE_RYO);
    UedeAppendVoice(1, "001");
    UedeAppendVoice(1, "002");
    Camera(3000);
    HLib::SetCurrentTaskTo(static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) >= work.minimumBet ? UedeSelectInfo : UedeNoMoney);
    UedeQueueVoices(work.self);
}
void UedeNoMoney(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (work.state == 0) {
        Camera(3800);
        Place(UEDE_RYO, {0, 0, .6f}, 0);
        Motion(UEDE_RYO, 0xf002);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), 0, 16, 1);
        UedeAppendVoice(0, "001");
    } else {
        Camera(3802);
        UedeAppendVoice(1, "003");
        HLib::SetCurrentTaskTo(UedeStop);
    }
    ++work.state;
    UedeQueueVoices(work.self);
}
void UedeSelectInfo(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (work.state == 1) {
        const std::int16_t choice = shenGetCtrlSpeakIconFlag();
        HLib::SetCurrentTaskTo(choice == 36 ? UedePlay : choice == 369 ? UedeInfo : UedeNoGame);
        work.state = 0;
    } else if (work.state == 0) {
        Camera(3100);
        Place(UEDE_RYO, {0, 0, .6f}, 0);
        Place(work.opponent, {0, 0, -.9f}, 170);
        Motion(UEDE_RYO, 0xf002);
        Motion(work.opponent, uedeTalkMotions[uedeOpponentPose]);
        EVT_AT_SetPoint(UEDE_RYO, work.opponent, 5, 3, 0, 0, 0, 0x20000000U);
        EVT_AT_SetPoint(work.opponent, UEDE_RYO, 5, 3, 0, 0, 0, 0x20000000U);
        SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), 0, 16, 1);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(SkChoiceWaitCallback, 6, 0x0b, sizeof(SkChoiceWaitWork), UEDE_TASK);
        SkChoiceWaitWork *selection = static_cast<SkChoiceWaitWork *>(HLib::GetTaskParameter(task));
        EV_SetAuthorActiveTo(0);
        selection->self = task;
        selection->parent = work.self;
        selection->choices[0] = -1;
        selection->choices[1] = 36;
        selection->choices[2] = 369;
        selection->choices[3] = 34;
        selection->timeout = 32767;
        selection->reserved1a = 0;
        selection->defaultChoice = 154;
        selection->phase = 0;
        HLib::SetupTimedTask(work.self, 0);
        ++work.state;
    }
}
void UedePlay(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    Camera(uedeInfoPlayed ? 3300 : 3301);
    uedeInfoPlayed = 0;
    UedeAppendVoice(0, "004");
    UedeAppendVoice(1, shenGetRandomInt32(2) ? "006" : "007");
    HLib::SetCurrentTaskTo(UedeBet);
    UedeQueueVoices(work.self);
}
void UedeInfo(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    Motion(work.opponent, uedeInfoMotions[uedeOpponentPose]);
    EventSetPoint point{.character = work.opponent, .trackingMode = 3, .flags = 0x50000000U};
    EVT_AT_SetPoint_Callback(&point);
    Camera(3200);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), UEDE_TASK);
    RegisterEventTask(task);
    EventSetCamera *camera = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    camera->cleanup = 1;
    camera->camera = 3201;
    camera->eye = UEDE_BASE;
    camera->target = UEDE_BASE;
    camera->context = 0;
    camera->angle = 0;
    camera->delay = 60;
    UedePlayVoice(0, "005", 0);
    UedeAppendVoice(1, "008");
    UedeAppendVoice(1, "009");
    UedeAppendVoice(1, "010");
    uedeInfoPlayed = 1;
    if (shenGetMinutesUntilNextFrameMinute()) {
        UedeAppendVoice(1, "011");
        UedeCheckMoney(work);
    } else
        HLib::SetCurrentTaskTo(UedeSleep);
    UedeQueueVoices(work.self, 0, 60);
}
void UedeNoGame(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    Camera(3800);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), UEDE_TASK);
    RegisterEventTask(task);
    EventSetCamera *camera = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    camera->cleanup = 1;
    camera->camera = 3801;
    camera->eye = work.opponent;
    camera->target = work.opponent;
    camera->context = 0;
    camera->delay = 60;
    Place(UEDE_RYO, {0, 0, .6f}, 0);
    Motion(UEDE_RYO, 0xf002);
    UedeAppendVoice(0, shenGetRandomInt32(2) ? "011" : "012");
    UedeAppendVoice(1, "069");
    HLib::SetCurrentTaskTo(UedeStop);
    UedeQueueVoices(work.self);
}
void UedeBet(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    if (work.state == 1) {
        work.wager = static_cast<std::int32_t>(GetNumBet());
        if (work.wager > 0) {
            UedeAppendVoice(0, "006");
            SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(GetPlayerStat(PlayerStatsId::Cash)) - work.wager);
            HLib::SetCurrentTaskTo(UedeOpponentThrow);
            UedeQueueVoices(work.self);
        } else
            HLib::SetCurrentTaskTo(UedeNoGame);
        work.state = 0;
    } else if (work.state == 0) {
        Camera(3400);
        Place(work.opponent, {0, 0, -.9f}, 170);
        Motion(UEDE_RYO, 0xf002);
        Motion(work.opponent, uedeMiddleMotions[uedeOpponentPose]);
        EVT_AT_SetPoint(UEDE_RYO, work.opponent, 5, 3, 0, 0, 0, 0x20000000U);
        EVT_AT_SetPoint(work.opponent, UEDE_RYO, 5, 3, 0, 0, 0, 0x20000000U);
        work.repeat = 1;
        (void)GetPlayerStat(PlayerStatsId::Cash);
        QueueSkMoneyIconWait(work.self, static_cast<std::int16_t>(work.maximumBet), static_cast<std::int16_t>(work.minimumBet));
        ++work.state;
    }
}
void UedeSleep(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    SetHighModelAndFaceFlagsForChara(ctrlGetCharaByName(work.opponent), 0, 16, 1);
    Place(UEDE_RYO, {0, 0, .6f}, 0);
    Motion(UEDE_RYO, 0xf002);
    UedeAppendVoice(1, "004");
    UedeAppendVoice(0, "002");
    UedeAppendVoice(1, "005");
    HLib::SetCurrentTaskTo(UedeStop);
    UedeQueueVoices(work.self);
}
} // namespace shenmue::s2::areas
