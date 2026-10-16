#include "s2/areas/area_ytqb.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/face.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"

namespace shenmue::s2::areas {
namespace {
void Failure(YtqbWork &work, float angle, std::int32_t delay) {
    YtqbKnockdown(angle, delay);
    YtqbTransition(work, YtqbRetry, 0, YtqbWaitMotion);
}
void SetHands(std::byte *pose, std::uint32_t frames) {
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(YTQB_RYO_), 0, pose, frames);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(YTQB_RYO_), 1, pose, frames);
}
void QueueCamera(std::int32_t camera, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 11, sizeof(EventSetCamera), YTQB_EVNT);
    RegisterEventTask(task);
    EventSetCamera *request = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
    request->cleanup = 1;
    request->camera = camera;
    request->eye = YTQB_DUMR;
    request->target = YTQB_DUMR;
    request->context = 0;
    request->reserved11[0] = std::byte{0};
    request->reserved11[1] = std::byte{0};
    request->reserved11[2] = std::byte{0};
    request->angle = 0;
    request->delay = delay;
}
void QueueFinalFade() {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_FadeBGM_Callback, 6, 11, sizeof(EventFadeBgmTaskData), YTQB_EVNT);
    RegisterEventTask(task);
    EventFadeBgmTaskData *bgm = static_cast<EventFadeBgmTaskData *>(HLib::GetTaskParameter(task));
    bgm->cleanupTask = 1;
    bgm->frames = 0;
    bgm->mode = 1;
    bgm->time = 90;
    task = HLib::EnqueueTaskWithParameter(EVT_AT_MenCtrl_Callback, 6, 11, sizeof(EventFilmControl), YTQB_EVNT);
    RegisterEventTask(task);
    EventFilmControl *film = static_cast<EventFilmControl *>(HLib::GetTaskParameter(task));
    film->cleanup = 1;
    film->frames = 30;
    film->startBlue = 0;
    film->startGreen = 0;
    film->startAlpha = 0;
    film->startRed = 0;
    film->targetAlpha = 255;
    film->targetRed = 0;
    film->targetGreen = 0;
    film->targetBlue = 0;
    film->delay = 90;
}
} // namespace
void YtqbSboAttack(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        RequestEventNpcCharacters(w.npcs, YTQB_SecondNames);
        w.parameters.firstOpponent = YTQB_SBO_;
        w.parameters.motionFlags = 0x04000000U;
        SelectPostFxBank(3, 1);
        UploadBlurStrength(.5f);
        EV_lightLoad(YTQB_0000);
        SetHighModelXYZ(FindCharacterByScenarioName(YTQB_CHO_), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(YTQB_CHO_), 0, 1, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(YTQB_RYO_), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(YTQB_RYO_), 1, 1, 0);
        SetHands(reinterpret_cast<std::byte *>(ryoQteHandPose), 0);
        SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(YTQB_RYO_), static_cast<std::int32_t>(FacialExpressionId::Angry), 1, 1);
        YtqbPosition(YTQB_RYO_, YTQB_BASE, {});
        YtqbAngle(YTQB_RYO_, YTQB_BASE, 0);
        YtqbPosition(YTQB_YOH_, YTQB_BASE, YTQB_YohWaiting);
        YtqbAngle(YTQB_YOH_, YTQB_BASE, 180);
        YtqbPosition(YTQB_SCX_, YTQB_BASE, YTQB_ScxWaiting);
        YtqbAngle(YTQB_SCX_, YTQB_BASE, 180);
        for (shen_name c : {YTQB_RYO_, YTQB_SBO_, YTQB_YOH_, YTQB_SCX_})
            EVT_AT_SetMot(c, 13, 2, 1, 0x40000000U, 0);
        YtqbCamera(7905);
        YtqbPrompt(w, YTQB_Kick_Prompt, YTQB_Kick_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7932 : 7908);
        YtqbTransition(w, YtqbYohAttack, 40);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7906);
        YtqbPrompt(w, YTQB_KickCounter_Prompt, YTQB_KickCounter_Authored);
    } else {
        Failure(w, -70, 0);
        w.phase = 0;
    }
}
void YtqbYohAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_YOH_;
        YtqbCamera(7907);
        YtqbPosition(w.parameters.firstOpponent, w.parameters.player, YTQB_GrabOpponentB);
        YtqbAngle(w.parameters.firstOpponent, w.parameters.player, 180);
        YtqbPrompt(w, YTQB_GrabEvadeB_Prompt, YTQB_GrabEvadeB_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7909 : 7908);
        YtqbTransition(w, YtqbScxAttack, phase == 1 ? 30 : 70);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7905);
        YtqbPrompt(w, YTQB_ThrowB_Prompt, YTQB_ThrowB_Authored);
    } else {
        Failure(w, -70, 0);
        w.phase = 0;
    }
}
void YtqbScxAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_SCX_;
        YtqbCamera(7900);
        YtqbPrompt(w, YTQB_Backfist_Prompt, YTQB_Backfist_Authored);
        return;
    }
    if (phase == 1 && w.result) {
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_ScxPlayer);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, 0);
        YtqbApplyResult(w);
        YtqbCamera(7903);
        QueueCamera(7930, 30);
        QueueCameraShake(.2f, .4f, .7f, 37);
        YtqbBreakBirdStall();
        YtqbTransition(w, YtqbSbkAttack, 77, YtqbNextQte);
        w.phase = 0;
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(7905);
        YtqbTransition(w, YtqbSbkAttack, 98, YtqbNextQte);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7906);
        YtqbPrompt(w, YTQB_KneeCounter_Prompt, YTQB_KneeCounter_Authored);
    } else {
        Failure(w, -80, 15);
        w.phase = 0;
    }
}
void YtqbSbkAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        RequestEventNpcCharacters(w.npcs, YTQB_WaterNames);
        w.parameters.firstOpponent = YTQB_SBK_;
        w.parameters.motionFlags = 0x24000000U;
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_SbkPlayer);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
        YtqbPosition(YTQB_VUN_, YTQB_BASE, YTQB_VunWaiting);
        YtqbAngle(YTQB_VUN_, YTQB_BASE, 135);
        YtqbPosition(YTQB_IKO_, YTQB_BASE, YTQB_IkoWaiting);
        YtqbAngle(YTQB_IKO_, YTQB_BASE, 135);
        for (shen_name c : {YTQB_RYO_, YTQB_SBK_, YTQB_VUN_, YTQB_IKO_})
            YtqbStand(c);
        EV_EnableCharacterDisplay(YTQB_SBK_);
        EV_EnableCharacterDisplay(YTQB_VUN_);
        YtqbCamera(7911);
        YtqbPosition(w.parameters.firstOpponent, w.parameters.player, YTQB_GrabOpponentA);
        YtqbAngle(w.parameters.firstOpponent, w.parameters.player, 180);
        YtqbPrompt(w, YTQB_GrabEvadeB_Prompt, YTQB_GrabEvadeB_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7939 : 7907);
        YtqbTransition(w, YtqbVunAttack, phase == 1 ? 30 : 70);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7945);
        YtqbPrompt(w, YTQB_ThrowB_Prompt, YTQB_ThrowB_Authored);
    } else {
        Failure(w, 70, 0);
        YtqbCamera(7946, 0, -0.0f);
        w.phase = 0;
    }
}
void YtqbVunAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_VUN_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbCamera(7912);
        YtqbPrompt(w, YTQB_Kick_Prompt, YTQB_Kick_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7940 : 7908);
        YtqbTransition(w, YtqbIkoAttack, 40);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7906);
        YtqbPrompt(w, YTQB_KickCounter_Prompt, YTQB_KickCounter_Authored);
    } else {
        EV_DisableCharacterDisplay(YTQB_SBK_);
        Failure(w, -60, 0);
        YtqbCamera(7905);
        w.phase = 0;
    }
}
void YtqbIkoAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_IKO_;
        w.parameters.motionFlags = 0x24000000U;
        YtqbCamera(7900);
        YtqbPrompt(w, YTQB_FinishingBackfist_Prompt, YTQB_FinishingBackfist_Authored);
        return;
    }
    if (phase == 1 && w.result) {
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_VunPlayer);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
        YtqbApplyResult(w);
        YtqbCamera(7914);
        YtqbBreakNearStall();
        YtqbTransition(w, YtqbStartSecondGroup, 43, YtqbStartWaterAuthor);
        w.phase = 0;
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(7905);
        YtqbTransition(w, YtqbStartSecondGroup, 73, YtqbStartWaterAuthor);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7908);
        YtqbPrompt(w, YTQB_FinishingBackfistCounter_Prompt, YTQB_FinishingBackfistCounter_Authored);
    } else {
        EV_DisableCharacterDisplay(YTQB_VUN_);
        Failure(w, -65, 15);
        w.phase = 0;
    }
}
void YtqbStartSecondGroup(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    w.phase = 0;
    w.continuation = YtqbRikThgAttack;
    HLib::SetCurrentTaskTo(w.continuation);
}
void YtqbRikThgAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        RequestEventNpcCharacters(w.npcs, YTQB_ThirdNames);
        SetHands(reinterpret_cast<std::byte *>(crateGripHandPose), 30);
        w.parameters.firstOpponent = YTQB_RIK_;
        w.parameters.secondOpponent = YTQB_THG_;
        w.parameters.motionFlags = 0x24000000U;
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_WaterPlayer);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
        YtqbPosition(YTQB_KIC_, w.parameters.player, YTQB_WaterOpponent);
        YtqbAngle(YTQB_KIC_, w.parameters.player, 180);
        YtqbPosition(YTQB_SOY_, w.parameters.player, {.x = 0, .y = 0, .z = 2});
        YtqbAngle(YTQB_SOY_, w.parameters.player, 0);
        for (shen_name c : {YTQB_RYO_, YTQB_KIC_, YTQB_RIK_, YTQB_THG_, YTQB_SOY_})
            YtqbStand(c);
        YtqbCamera(7900);
        YtqbPrompt(w, YTQB_Pin_Prompt, YTQB_Pin_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (phase == 1) {
        if (w.result) {
            YtqbCamera(7916);
            YtqbPrompt(w, YTQB_PinKick_Prompt, YTQB_PinKick_Authored);
        } else {
            YtqbBackKnockdown(-70);
            YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
        }
    } else {
        if (w.result) {
            YtqbCamera(7917);
            YtqbTransition(w, YtqbSoyAttack, 50);
        } else {
            YtqbSideKnockdown(0);
            YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
        }
        w.phase = 0;
    }
}
void YtqbSoyAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_SOY_;
        w.parameters.motionFlags = 0x04000000U;
        EventSetPositionAngle position{.cleanup = 0,
            .character = YTQB_RIK_,
            .parent = YTQB_INVALID,
            .position = {.x = 189.5f, .y = 0, .z = 182.6999969482422f},
            .degrees = 45,
            .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        YtqbCamera(7918);
        YtqbPrompt(w, YTQB_RearKick_Prompt, YTQB_RearKick_Authored);
        return;
    }
    if (phase != 1) {
        YtqbFinalKnockdown(0);
        YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
        w.phase = 0;
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(7919);
        YtqbTransition(w, YtqbKicAttack, 10);
        w.phase = 0;
    } else {
        YtqbCamera(7920);
        HLib::SetupTaskDelay(10);
    }
}
void YtqbKicAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_KIC_;
        w.parameters.motionFlags = 0x24000000U;
        YtqbCamera(7901);
        YtqbAngle(YTQB_RYO_, YTQB_RYO_, 20);
        YtqbPrompt(w, YTQB_WristTwist_Prompt, YTQB_WristTwist_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (phase == 1 && w.result) {
        EVT_AT_SetSound(649, 98);
        EVT_AT_SetVoice("S2BVB023", 1, 107);
        YtqbCamera(7904);
        QueueCamera(7941, 46);
        QueueCameraShake(.2f, .4f, .7f, 37);
        YtqbBreakFrontStall();
        YtqbTransition(w, YtqbKt2Attack, 160, YtqbNextQte);
        w.phase = 0;
    } else if (w.result) {
        YtqbCamera(7922);
        YtqbTransition(w, YtqbKt2Attack, 75, YtqbNextQte);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7921);
        YtqbPrompt(w, YTQB_WristCounter_Prompt, YTQB_WristCounter_Authored);
    } else {
        Failure(w, -70, 15);
        YtqbCamera(7909);
        w.phase = 0;
    }
}
void YtqbKt2Attack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        RequestEventNpcCharacters(w.npcs, YTQB_LastNames);
        SetHands(reinterpret_cast<std::byte *>(ryoQteHandPose), 10);
        w.parameters.firstOpponent = YTQB_KT2_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_Kt2Player);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
        YtqbPosition(YTQB_RNT_, YTQB_BASE, YTQB_RntWaiting);
        YtqbAngle(YTQB_RNT_, YTQB_BASE, 135);
        YtqbPosition(YTQB_KT3_, YTQB_BASE, YTQB_Kt3Waiting);
        YtqbAngle(YTQB_KT3_, YTQB_BASE, 135);
        for (shen_name c : {YTQB_RYO_, YTQB_RNT_, YTQB_KT3_})
            YtqbStand(c);
        YtqbCamera(7923);
        YtqbPrompt(w, YTQB_WristTwist_Prompt, YTQB_WristTwist_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7942 : 7909);
        if (phase == 1)
            YtqbBreakLantern();
        YtqbTransition(w, YtqbRntAttack, 20);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7906);
        YtqbPrompt(w, YTQB_WristCounter_Prompt, YTQB_WristCounter_Authored);
    } else {
        Failure(w, -60, 0);
        w.phase = 0;
    }
}
void YtqbRntAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_RNT_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbCamera(7924);
        YtqbPosition(w.parameters.firstOpponent, w.parameters.player, YTQB_GrabOpponentB);
        YtqbAngle(w.parameters.firstOpponent, w.parameters.player, 180);
        YtqbPrompt(w, YTQB_GrabEvadeB_Prompt, YTQB_GrabEvadeB_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7944 : 7907);
        YtqbTransition(w, YtqbKt3Attack, phase == 1 ? 20 : 40);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7944);
        YtqbPrompt(w, YTQB_ThrowB_Prompt, YTQB_ThrowB_Authored);
    } else {
        Failure(w, -70, 0);
        w.phase = 0;
    }
}
void YtqbKt3Attack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_KT3_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbCamera(7924);
        YtqbPrompt(w, YTQB_Kick_Prompt, YTQB_Kick_Authored);
        return;
    }
    if (phase == 1) {
        YtqbPosition(YTQB_RYO_, YTQB_BASE, {.x = 13.5f, .y = 0, .z = -23.899999618530273f});
        YtqbAngle(YTQB_RYO_, YTQB_BASE, -45);
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(7925);
        YtqbTransition(w, YtqbAncAttack, phase == 1 ? 84 : 73, YtqbNextQte);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7908);
        YtqbPrompt(w, YTQB_KickCounter_Prompt, YTQB_KickCounter_Authored);
    } else {
        Failure(w, -70, 15);
        YtqbCamera(7931);
        w.phase = 0;
    }
}
void YtqbAncAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_ANC_;
        w.parameters.motionFlags = 0x24000000U;
        YtqbPosition(YTQB_RYO_, YTQB_BASE, YTQB_AncPlayer);
        YtqbAngle(YTQB_RYO_, YTQB_BASE, 0);
        YtqbPosition(YTQB_EHY_, YTQB_BASE, YTQB_EhyWaiting);
        YtqbAngle(YTQB_EHY_, YTQB_BASE, 180);
        YtqbPosition(YTQB_KT5_, YTQB_BASE, {.x = 12.199999809265137f, .y = 0, .z = -31.799999237060547f});
        YtqbAngle(YTQB_KT5_, YTQB_BASE, -135);
        YtqbPosition(YTQB_CJU_, YTQB_BASE, YTQB_CjuWaiting);
        YtqbAngle(YTQB_CJU_, YTQB_BASE, 0);
        YtqbPosition(YTQB_KT7_, YTQB_BASE, {.x = 14, .y = 0, .z = -34.29999923706055f});
        YtqbAngle(YTQB_KT7_, YTQB_BASE, 160);
        for (shen_name c : {YTQB_RYO_, YTQB_EHY_, YTQB_KT5_, YTQB_CJU_, YTQB_KT7_})
            YtqbStand(c);
        EV_EnableCharacterDisplay(YTQB_ANC_);
        EV_EnableCharacterDisplay(YTQB_CJU_);
        EV_EnableCharacterDisplay(YTQB_KT7_);
        YtqbPosition(w.parameters.firstOpponent, w.parameters.player, YTQB_GrabOpponentEnd);
        YtqbAngle(w.parameters.firstOpponent, w.parameters.player, 180);
        YtqbCamera(7909);
        YtqbPrompt(w, YTQB_GrabEvadeA_Prompt, YTQB_GrabEvadeA_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7907 : 7908);
        YtqbTransition(w, YtqbKt5Attack, phase == 1 ? 10 : 20);
        w.phase = 0;
    } else if (phase == 1) {
        SetHands(reinterpret_cast<std::byte *>(crateGripHandPose), 10);
        YtqbCamera(7907);
        YtqbPrompt(w, YTQB_ThrowA_Prompt, YTQB_ThrowA_Authored);
    } else {
        Failure(w, 70, 0);
        w.phase = 0;
    }
}
void YtqbKt5Attack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_KT5_;
        w.parameters.motionFlags = 0x24000000U;
        SetHands(reinterpret_cast<std::byte *>(ryoQteHandPose), 10);
        YtqbCamera(7900);
        YtqbPrompt(w, YTQB_FlyingKick_Prompt, YTQB_FlyingKick_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7947 : 7926);
        if (phase == 1)
            YtqbBreakBucket();
        YtqbTransition(w, YtqbCjuAttack, 20);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7908);
        YtqbPrompt(w, YTQB_FlyingKickCounter_Prompt, YTQB_FlyingKickCounter_Authored);
    } else {
        EV_DisableCharacterDisplay(YTQB_ANC_);
        Failure(w, -65, 0);
        w.phase = 0;
    }
}
void YtqbCjuAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_CJU_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbPrompt(w, YTQB_RearKickAssist_Prompt, YTQB_RearKickAssist_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        EV_DisableCharacterDisplay(YTQB_ANC_);
        YtqbCamera(7927);
        YtqbTransition(w, YtqbKt7Attack, 10);
    } else {
        EV_DisableCharacterDisplay(YTQB_KT7_);
        YtqbFinalKnockdown(0);
        YtqbCamera(7920);
        YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
    }
    w.phase = 0;
}
void YtqbKt7Attack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_KT7_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbPrompt(w, YTQB_BackfistAssist_Prompt, YTQB_BackfistAssist_Authored);
        return;
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(7928);
        YtqbTransition(w, YtqbFinalEhyAttack, 10);
    } else {
        EV_DisableCharacterDisplay(YTQB_CJU_);
        YtqbFinalKnockdown(0);
        YtqbCamera(7920);
        YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
    }
    w.phase = 0;
}
void YtqbFinalEhyAttack(void *data) {
    YtqbWork &w = *static_cast<YtqbWork *>(data);
    const std::int32_t phase = w.phase++;
    if (!phase) {
        w.parameters.firstOpponent = YTQB_EHY_;
        w.parameters.motionFlags = 0x04000000U;
        YtqbPrompt(w, YTQB_KickDodge_Prompt, YTQB_KickDodge_Authored);
        return;
    }
    if (phase == 1) {
        YtqbPosition(YTQB_RYO_, YTQB_BASE, {.x = 12.899999618530273f, .y = 0, .z = -32.20000076293945f});
        YtqbAngle(YTQB_RYO_, YTQB_BASE, 0);
    }
    YtqbApplyResult(w);
    if (w.result) {
        YtqbCamera(phase == 1 ? 7929 : 7905);
        if (phase == 1) {
            EVT_AT_FadeBGM(0, 1, 90);
            EventFilmControl film{.cleanup = 0,
                .frames = 30,
                .startAlpha = 0,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 255,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = 90};
            EVT_AT_MenCtrl(film);
        } else
            QueueFinalFade();
        EVT_SetQteFailureCount(0);
        EVT_SetQteRetryFrames(36);
        YtqbTransition(w, YtqbLoadFinalCharacters, 120);
        w.phase = 0;
    } else if (phase == 1) {
        YtqbCamera(7901);
        YtqbPrompt(w, YTQB_KickDodgeCounter_Prompt, YTQB_KickDodgeCounter_Authored);
    } else {
        YtqbBackKnockdown(0);
        YtqbCamera(7929);
        YtqbTransition(w, YtqbRetry, 0, YtqbWaitMotion);
        w.phase = 0;
    }
}
} // namespace shenmue::s2::areas
