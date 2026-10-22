#include "s2/areas/area_qabt.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/scene_data.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
namespace {
void QabtReadyInit(void *);
void QabtReadyInitAuto(void *);
void QabtCarryInit(void *);
void QabtCarryAutoWait(void *);
void QabtCarryLift(void *);
void QabtCarryRoute(void *);
void QabtCarryFinishTurns(void *);
void QabtCarryQte(void *);
void QabtCarryForwardQte(void *);
void QabtCarryPutQte(void *);
void QabtCarryFalse(void *);
void QabtCarryPut(void *);
void QabtCarryEnd(void *);
void QabtCarryAddTime(void *);
void QabtCarryTimeOver(void *);
void QabtCarryTimeOverAuto(void *);
void QabtPayInit(void *);
void QabtPayAuto(void *);
void QabtPayAutoWait(void *);
void QabtPayEnd(void *);
void QabtHelpNullsub() {
}
void HideQabtDockProps() {
    for (shen_name name : ARBT_HIDDEN_PROPS)
        SetDisplayFlagForProp(name, 0);
}
void QabtCleanup(QabtWork &work) {
    SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 0);
    SetPluralControlFor(ctrlGetCharaByName(QABT_AW2), 0);
    hndlSetFlags(ctrlGetCharaByName(ARBT_RYO));
    hndlSetFlags(ctrlGetCharaByName(QABT_AW2));
    for (shen_name name : work.crates.pallets)
        SetCharaKill(name, 1);
    EV_EventSetUpHelpIcon_0(0, 0);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    QabtHelpNullsub();
    SetRenderClockAndCompassTo(0);
    EV_FreeAuthorPak(0, work.descriptor->mode == 1 ? "0360" : "0018");
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    ET_AT_KillWork(work.characters);
    EV_NPC_NewLoadSw(1);
    SetPlayerControlEnabled(0);
}
void QabtLeave(QabtWork &work) {
    QabtCleanup(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.self);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 966, 1);
    if (EV_GetShenmueFlag(0x435)) {
        EV_stepSetFlags(0, 0x435);
        EV_scenQueueLoad(g_actNumber, ARBT_AK00, 0);
    } else if (work.descriptor->mode == 1) {
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x74f, 1);
        EV_scenQueueLoad(g_actNumber, ARBT_Q100, 97);
    } else
        EV_scenQueueLoad(g_actNumber, ARBT_AK00, 92);
}
void QabtReadyInit(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
    FinishLoading();
    for (shen_name name : {ARBT_RYO, QABT_AW2}) {
        SetLowQualityHands(ctrlGetCharaByName(name));
        SetHighModelXYZ(ctrlGetCharaByName(name), 1, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(name), 1, 1, 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 0, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(name), 1, reinterpret_cast<std::byte *>(crateGripHandPose), 0);
    }
    mtwkSetRequestedRootMode(scenFindMotionwork(ARBT_RYO), 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(QABT_AW2), 1);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 10, 2);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(ARBT_RYO), 17, 1);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(QABT_AW2), 10, 2);
    SetMotionBoneTargetStepLimit(scenFindMotionwork(QABT_AW2), 17, 2);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_RYO), 55, 0);
    mtwkSetExtraFlag(scenFindMotionwork(QABT_AW2), 55, 0);
    AT_SetLocalBaseChara(ARBT_RYO, ARBT_BASE);
    AT_SetLocalBaseChara(QABT_AW2, ARBT_BASE);
    AT_SetLocalBaseChara(QABT_RNP, ARBT_BASE);
    AT_SetLocalBaseChara(ARBT_DUMA, ARBT_RYO);
    AT_SetLocalBaseChara(ARBT_DUMT, QABT_AW2);
    AT_SetLocalBaseChara(ARBT_DUMS, QABT_RNP);
    for (shen_name name : {ARBT_DUMA, ARBT_DUMT, ARBT_DUMS})
        EV_AT_SetPos(name, EventTransformFlags::AllAxes, qabtCameraOffset);
    for (shen_name name : {ARBT_DUMA, ARBT_DUMT, ARBT_DUMS})
        EV_AT_SetAngY(name, EventTransformFlags::AllAxes, qabtCameraRotation);
    EVT_AT_SetPos(QABT_AW2, ARBT_BASE, {-1.5f, 0, -1.5f}, 0);
    EventSetRawAngle tkr{.cleanup = 0, .character = QABT_AW2, .parent = ARBT_BASE, .rotation = {0, 0, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&tkr);
    EVT_AT_SetPos(QABT_RNP, ARBT_BASE, {-1, 0, -6.5f}, 0);
    EventSetRawAngle qln{.cleanup = 0, .character = QABT_RNP, .parent = ARBT_BASE, .rotation = {0, 29127, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&qln);
    EVT_AT_SetPos(ARBT_RYO, QABT_RNP, {0, 0, -1.5f}, 0);
    EventSetRawAngle ryo{.cleanup = 0, .character = ARBT_RYO, .parent = QABT_RNP, .rotation = {0, 32768, 0}, .delay = 0};
    EVT_AT_SetAng_Callback(&ryo);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_DisableCharacterDisplay(name);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_ToggleCharacterFlag(name);
    SetFixoDataForMotionWork(ctrlGetCharaByName(ARBT_PAY), ctrlGetCharaByName(QABT_RNP), 14, {0.197f, 0.075f, -0.018f}, {48423, 42780, 52792});
    EV_DisableCharacterDisplay(ARBT_PAY);
    BeginPrimarySampleExecution(0);
    EV_LoadAudio("0018E000", 0);
    EV_spmBeginSampleExecute();
    if (work.firstJob)
        HLib::SetCurrentTaskTo(QabtCarryInit);
    else {
        work.authorFrames = QabtIntroduceJob();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(QabtReadyInitAuto);
    }
}
void QabtReadyInitAuto(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    if (authGetFinished() || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0) {
        HLib::SetCurrentTaskTo(QabtCarryInit);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
    }
}
void QabtCarryInit(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_DisableCharacterDisplay(QABT_RNP);
    SetInitialQabtCarrySettings(work.carry, work.delivered);
    work.authorFrames = work.delivered ? QabtIntroduceNextCrate() : work.firstJob ? QabtExplainFirstCarry() : QabtExplainCarry();
    HLib::SetCurrentTaskTo(QabtCarryAutoWait);
}
void QabtCarryAutoWait(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    if (authGetFinished() || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0)
        HLib::SetCurrentTaskTo(QabtCarryLift);
}
void QabtCarryLift(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    SetRenderClockAndCompassTo(0);
    Vector3 position;
    EV_GetCharaPosition(work.crates.crates[work.crates.index], CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    work.carry.frames = position.y <= 0.5f ? QabtLiftLowCrate(work.crates) : QabtLiftHighCrate(work.crates);
    HLib::SetCurrentTaskTo(work.carry.phase);
}
void QabtCarryFinishTurns(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    const std::int32_t turns = carry.turns--;
    if (turns > 0) {
        CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
        const std::int32_t delay = carry.frames > 40 ? carry.frames - 40 : 0;
        carry.success = 0;
        carry.frames = 40;
        carry.direction = 0x8000;
        carry.presses = 1;
        carry.intensity = 0;
        QabtQueueDirection(carry.direction, 0, delay);
        HLib::SetCurrentTaskTo(QabtCarryQte);
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
    } else if (turns - 1 >= -1) {
        EVT_AT_SetVoice("0360C090", 1, carry.frames); // @text EN: "Put it down, okay?" | JA: "フーッ、ダウン、ＯＫ。"
        HLib::SetupTaskDelay(carry.frames + 100);
    } else {
        GetCurrentDateTime(work.time);
        if (work.time.hour < work.endTime.hour) {
            CheckHelpIcon(0xd020, 0xd020, CheckHelpIconOp::Remove);
            CheckHelpIcon(0x2020, 0, CheckHelpIconOp::Add);
            carry.frames = 50;
            carry.success = 0;
            carry.direction = 0x2000;
            carry.intensity = 7;
            carry.presses = 1;
            QabtQueueDirection(carry.direction, 7, 0);
            HLib::SetCurrentTaskTo(QabtCarryPutQte);
        } else {
            SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetCurrentTaskTo(QabtCarryAddTime);
        }
    }
}
void QabtCarryRoute(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    const ArbtRouteStep *step = carry.route;
    CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
    if (carry.forwardSteps >= step->forwardSteps) {
        ++carry.route;
        carry.forwardSteps = 0;
        if (step->intensity == 8) {
            carry.turns = 2;
            carry.phase = QabtCarryFinishTurns;
            HLib::SetCurrentTaskTo(carry.phase);
        } else {
            std::int32_t delay = static_cast<std::int32_t>(static_cast<float>(carry.frames) - step->frames);
            if (delay < 0)
                delay = 0;
            carry.success = 0;
            carry.direction = step->direction;
            carry.intensity = step->intensity;
            carry.frames = static_cast<std::int32_t>(step->frames);
            carry.presses = ARBT_PRESS_COUNTS[step->intensity];
            QabtQueueDirection(carry.direction, carry.intensity, delay);
            HLib::SetCurrentTaskTo(QabtCarryQte);
            if (delay > 0)
                HLib::SetupTaskDelay(delay);
        }
    } else {
        GetCurrentDateTime(work.time);
        if (work.time.hour < work.endTime.hour) {
            const std::int32_t delay = carry.frames > 30 ? carry.frames - 30 : 0;
            carry.success = 0;
            carry.frames = 30;
            carry.direction = 0x1000;
            carry.intensity = 6;
            carry.presses = 1;
            QabtQueueDirection(carry.direction, 6, delay);
            HLib::SetCurrentTaskTo(QabtCarryForwardQte);
            if (delay > 0)
                HLib::SetupTaskDelay(delay);
        } else {
            EVT_AT_MenCtrl({.cleanup = 0,
                .frames = 60,
                .startAlpha = 0,
                .startRed = 0,
                .startGreen = 0,
                .startBlue = 0,
                .targetAlpha = 255,
                .targetRed = 0,
                .targetGreen = 0,
                .targetBlue = 0,
                .delay = static_cast<float>(carry.frames)});
            HLib::SetCurrentTaskTo(QabtCarryAddTime);
            if (carry.frames > 0)
                HLib::SetupTaskDelay(carry.frames);
        }
    }
}
void QabtCarryQte(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    const std::uint32_t wrong = carry.direction ^ 0xd000;
    if (--carry.frames <= 0) {
        if (carry.success) {
            carry.consecutiveFailures = 0;
            carry.frames = QabtWalkDirection(carry.speed, carry.intensity, carry.direction);
            HLib::SetCurrentTaskTo(carry.phase);
        } else {
            ++carry.failures;
            ++carry.consecutiveFailures;
            const std::int32_t frames = QabtFailCarry(work.crates, work.crates.crates[work.crates.index], carry.intensity, carry.direction);
            HLib::SetCurrentTaskTo(QabtCarryFalse);
            HLib::SetupTaskDelay(frames);
        }
        return;
    }
    const std::uint32_t pressed = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_PRESSED));
    if (pressed == carry.direction || ((pressed & carry.direction) && carry.direction == 0x1000)) {
        if (carry.presses-- == 1) {
            Loop3DSoundEffect(0x900ab);
            carry.success = 1;
        } else if (carry.presses <= 0) {
            Loop3DSoundEffect(0xa00ab);
            HLib::SetupTaskDelay(carry.frames);
            carry.success = 0;
            carry.frames = 0;
        } else
            Loop3DSoundEffect(0x900ab);
    } else if ((pressed & wrong) && carry.frames < 20 && !carry.success) {
        Loop3DSoundEffect(0xa00ab);
        HLib::SetupTaskDelay(carry.frames);
        carry.frames = 0;
    }
}
void QabtCarryForwardQte(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    const std::uint32_t wrong = carry.direction ^ 0xd000;
    if (--carry.frames <= 0) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (++carry.idleCount > 3) {
            carry.idleCount = 0;
            HLib::SetupTaskDelay(QabtWarnIdleForward(++work.idleWarnings));
        }
        return;
    }
    const std::uint32_t held = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD)),
                        pressed = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_PRESSED));
    if (held & carry.direction) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (carry.idleCount || carry.frames <= 0)
            carry.frames = 0;
        else
            HLib::SetupTaskDelay(carry.frames);
        ++carry.forwardSteps;
        carry.idleCount = 0;
        carry.frames = QabtWalkForward(carry.speed, carry.frames);
    } else if (pressed & wrong) {
        HLib::SetCurrentTaskTo(QabtCarryQte);
        if (!carry.idleCount && carry.frames > 0)
            HLib::SetupTaskDelay(carry.frames);
        carry.frames = 0;
        carry.idleCount = 0;
        carry.direction = 0x2000;
        EVT_AT_SetVoice((pressed & 0x8000) ? "0360A003" : (pressed & 0x4000) ? "0360A005" : "0360A006", 1, 0); // @text 0360A003 EN: "Right=@" | JA: "右…" ; 0360A005 EN: "Left=@" | JA: "左…" ; 0360A006 EN: " " | JA: "後ろ…"
    }
}
void QabtCarryPutQte(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    if (--carry.frames <= 0) {
        HLib::SetCurrentTaskTo(carry.phase);
        if (++carry.idleCount > 3) {
            carry.idleCount = 0;
            HLib::SetupTaskDelay(QabtWarnIdlePut(++work.idleWarnings));
        }
    } else {
        const std::uint32_t held = static_cast<std::uint32_t>(GetCurrentControlInput(INPUT_HELD));
        if (held & carry.direction) {
            Loop3DSoundEffect(0x900ab);
            HLib::SetCurrentTaskTo(QabtCarryPut);
            carry.idleCount = 0;
            carry.frames = 0;
        }
    }
}
void QabtCarryFalse(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    const QabtCarryWork &carry = work.carry;
    std::int32_t frames;
    if (carry.intensity == 3)
        frames = QabtWarnWrongDouble(carry.direction);
    else if (carry.intensity > 3 && carry.intensity <= 5)
        frames = QabtWarnWrongMultiple();
    else if (carry.intensity == 6)
        frames = QabtWarnWrongForward();
    else
        switch (carry.failures >> 1) {
        case 0:
            frames = QabtWarnCarryOnce();
            break;
        case 1:
            frames = QabtWarnCarryTwice();
            break;
        case 2:
            frames = QabtWarnCarryThird();
            break;
        case 3:
            frames = QabtWarnCarryFourth();
            break;
        default:
            frames = QabtWarnCarryRepeatedly();
            break;
        }
    HLib::SetCurrentTaskTo(QabtCarryLift);
    HLib::SetupTaskDelay(frames);
}
void QabtCarryPut(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    const std::int32_t index = work.crates.index;
    const shen_name crate = work.crates.crates[index], destination = work.crates.destinations[index];
    CheckHelpIcon(0x2020, 0x2020, CheckHelpIconOp::Remove);
    if (work.carry.failures)
        work.perfectStreak = 0;
    else
        ++work.perfectStreak;
    EventSetPositionAngle request{.cleanup = 0,
        .character = crate,
        .parent = destination,
        .position = work.crates.stacked[index] ? Vector3{-0.05f, 0, 0.1f} : Vector3{0, 0, 0.2f},
        .degrees = 0,
        .delay = 0};
    EVT_AT_SetPosAng_Callback(&request);
    const std::int32_t frames = work.crates.stacked[index] ? QabtPutHighCrate(work.crates, work.perfectStreak, destination) :
                                                             QabtPutLowCrate(work.crates, work.perfectStreak, destination);
    HLib::SetCurrentTaskTo(QabtCarryEnd);
    HLib::SetupTaskDelay(frames);
}
void QabtCarryEnd(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    std::int32_t frames = work.crates.limit - (work.carry.failures >> 2);
    if (frames < work.delivered)
        frames = work.delivered;
    work.crates.limit = frames;
    work.failures += work.carry.failures;
    ++work.delivered;
    if (SelectNextQabtCrate(work.crates)) {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(QabtCarryInit);
        HLib::SetupTaskDelay(30);
    } else {
        SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(QabtCarryAddTime);
    }
}
void QabtCarryAddTime(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
    SetRenderClockAndCompassTo(1);
    GetCurrentDateTime(work.time);
    if (work.time.hour >= work.endTime.hour + 1) {
        if (!filmCheckFadeTrackColour())
            HLib::SetCurrentTaskTo(QabtCarryTimeOver);
    } else {
        if (++work.time.minute >= 60) {
            work.time.minute = 0;
            ++work.time.hour;
        }
        SetCurrentDateTimeTo(work.time);
    }
}
void QabtCarryTimeOver(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    EV_EnableCharacterDisplay(QABT_RNP);
    const std::int32_t frames = QabtAnnounceTimeOver();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_MenCtrl({.cleanup = 0,
        .frames = 30,
        .startAlpha = 0,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = 255,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(frames)});
    work.authorFrames = frames + 30;
    HLib::SetCurrentTaskTo(QabtCarryTimeOverAuto);
}
void QabtCarryTimeOverAuto(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    if (authGetFinished() || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
        work.authorFrames = 0;
    }
    if (work.authorFrames-- == 0)
        HLib::SetCurrentTaskTo(QabtPayInit);
}
void QabtPayInit(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, work.descriptor->mode == 1 ? Vector3{142.9f, 100, 138.9f} : Vector3{225.2f, 100, 247.5f}, 0);
    EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
    if (work.descriptor->mode != 1) {
        EV_SetModelDataDefaultVertexShader();
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_AKB0);
        PushGlobalLightingProgram();
        HideQabtDockProps();
    }
    SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 1);
    SetPluralControlFor(ctrlGetCharaByName(QABT_AW2), 1);
    for (shen_name name : ARBT_PAY_CROWD)
        SetPluralControlFor(ctrlGetCharaByName(name), 1);
    mtwkSetExtraFlag(scenFindMotionwork(ARBT_RYO), 55, 2);
    mtwkSetExtraFlag(scenFindMotionwork(QABT_AW2), 55, 2);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_EnableCharacterDisplay(name);
    for (shen_name name : ARBT_PAY_CROWD)
        EV_EnsureCharacterDraw(name);
    if (work.crates.moveTask) {
        work.crates.moveFrames = 0;
        HLib::DestroyTaskInternal(work.crates.moveTask);
        work.crates.moveTask = nullptr;
    }
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (work.delivered < 6)
        ++work.delivered;
    HLib::SetCurrentTaskTo(QabtPayAuto);
}
void QabtPayAuto(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    work.cash = 10 * work.delivered;
    work.payEndFrame = work.firstJob ? QabtPayFirstJob(work.cash, work.delivered) :
        work.delivered >= 5          ? QabtPayGoodJob(work.cash, work.delivered) :
        work.delivered >= 3          ? QabtPayAverageJob(work.cash, work.delivered) :
                                       QabtPayPoorJob(work.cash, work.delivered);
    work.payFrame = 0;
    HLib::SetCurrentTaskTo(QabtPayAutoWait);
}
void QabtPayAutoWait(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    if (work.payFrame >= 220) {
        if (work.payFrame == work.payEndFrame - 60)
            ProcessAicaCommand(0xaa0, 0);
        if (work.payFrame == work.payEndFrame - 30)
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        if (work.payFrame == work.payEndFrame)
            HLib::SetCurrentTaskTo(QabtPayEnd);
    } else if (authGetFinished() || (!work.firstJob && HasPressedCancel())) {
        EV_CleanupMapControlTask();
        AddThisToCashOrSegaCoins(work.cash, static_cast<ECashOrCoins>(2));
        ProcessAicaCommand(0xaa0, 30);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(QabtPayEnd);
        HLib::SetupTaskDelay(80);
        return;
    }
    ++work.payFrame;
}
void QabtPayEnd(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    ResetCharaFixo(ctrlGetCharaByName(ARBT_PAY));
    if (work.descriptor->mode == 2)
        HLib::SetCurrentTaskTo(QabtReadyInit);
    else
        QabtLeave(work);
}
void QabtMain(QabtWork &work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    work.characters = Queue_EVT_AT_ReadChr(work.self, QABT_CHARACTERS);
    work.delivered = 0;
    work.failures = 0;
    work.idleWarnings = 0;
    work.perfectStreak = 0;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ARBT"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("ARBT"), 0);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0018E000");
    EV_FreeAuthorPak(1, work.descriptor->mode == 1 ? "0360" : "0018");
    GetCurrentDateTime(work.time);
    GetCurrentDateTime(work.endTime);
    if (EV_GetShenmueFlag(0x12)) {
        EV_stepSetFlags(0, 0x12);
        work.firstJob = 1;
        work.time.hour = 10;
    } else {
        work.firstJob = 0;
        work.time.hour = work.descriptor->shift == 0 ? 10 : work.descriptor->shift == 1 ? 13 : work.descriptor->shift == 2 ? 16 : 20;
    }
    work.time.minute = 0;
    work.time.second = 0;
    work.endTime.hour = work.time.hour + 1;
    work.endTime.minute = 0;
    work.endTime.second = 0;
    SetCurrentDateTimeTo(work.time);
    shenSetTimeSimulationTo(1);
    EV_stepSetFlags(0, 65);
    if (work.descriptor->mode == 1) {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, {175.7f, 100, 177}, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_QCAE);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 2);
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        for (std::int32_t index = 0; index < 10; ++index)
            EV_SetObjectFlagByIndex(index, 0);
    } else if (work.descriptor->shift == 3) {
        EventSetPositionAngle position{
            .cleanup = 0, .character = ARBT_BASE, .parent = EVENT_CHARACTER_INVALID, .position = {266.8f, 0, 260.5f}, .degrees = 0, .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_AK00);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        for (std::int32_t index : {5, 6, 8, 10})
            EV_SetObjectFlagByIndex(index, 0);
        SetPluralControlFor(ctrlGetCharaByName(ARBT_RYO), 1);
        SetPluralControlFor(ctrlGetCharaByName(QABT_AW2), 1);
    } else if (work.descriptor->shift == 1) {
        EventSetPositionAngle position{
            .cleanup = 0, .character = ARBT_BASE, .parent = EVENT_CHARACTER_INVALID, .position = {270, 0, 238.7f}, .degrees = 270, .delay = 0};
        EVT_AT_SetPosAng_Callback(&position);
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 0, 0);
        EV_UpdateWeatherForMinute(1, 0, 0, 0);
        EV_UpdateWeather();
        EV_SetWeatherInstruction(12, 0);
        EV_SetWeatherInstruction(0x780, 0);
        EV_lightLoad(ARBT_LIGHT_AKA3);
        PushGlobalLightingProgram();
        EV_AssignLightParams(0, 2);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        EV_SetObjectFlagByIndex(10, 0);
        HideQabtDockProps();
        for (std::int32_t index = 0; index < 8; ++index)
            EV_SetObjectFlagByIndex(index, 0);
    } else {
        EVT_AT_SetPos(ARBT_BASE, EVENT_CHARACTER_INVALID, {225.2f, 100, 247.5f}, 0);
        EVT_AT_SetAngY(ARBT_BASE, EVENT_CHARACTER_INVALID, 270, 0);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_AssignLightParams(0, 8);
        EV_AssignLightParams(0, 9);
        EV_AssignLightParams(0, 10);
        for (std::int32_t index : {5, 7, 8, 10})
            EV_SetObjectFlagByIndex(index, 0);
    }
    SetRenderClockAndCompassTo(1);
    QabtHelpNullsub();
    EV_EventSetUpHelpIcon_0(70, 0);
    SetInitialQabtCarrySettings(work.carry, 0);
    CreateQabtCrates(work.crates);
}
} // namespace
void QabtCarryTurnInit(void *data) {
    QabtWork &work = *static_cast<QabtWork *>(data);
    QabtCarryWork &carry = work.carry;
    CheckHelpIcon(0xd020, 0, CheckHelpIconOp::Add);
    if (carry.turns-- != 0) {
        const std::int32_t delay = carry.frames > 40 ? carry.frames - 40 : 0;
        carry.success = 0;
        carry.frames = 40;
        carry.presses = 1;
        carry.intensity = 0;
        carry.direction = work.delivered < 2 ? 0x4000 : 0x8000;
        QabtQueueDirection(carry.direction, 0, delay);
        HLib::SetCurrentTaskTo(QabtCarryQte);
        if (delay > 0)
            HLib::SetupTaskDelay(delay);
    } else {
        carry.phase = QabtCarryRoute;
        HLib::SetCurrentTaskTo(carry.phase);
        if (!work.delivered) {
            const std::int32_t frames = QabtStartWork(carry.frames);
            carry.frames = 0;
            HLib::SetupTaskDelay(frames);
        }
    }
}
void InitializeQabtEvent() {
}
void QueueQabtReadyInit(std::uintptr_t argument) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(QabtReadyInit, 4, 0x0b, sizeof(QabtWork), AREA_EVENT_TASK);
    QabtWork &work = *static_cast<QabtWork *>(HLib::GetTaskParameter(task));
    work.reserved170 = 0;
    work.self = task;
    if (argument)
        work.descriptor = reinterpret_cast<const QabtDescriptor *>(argument);
    else {
        work.descriptor = QABT_DESCRIPTORS;
        if (g_entryPoint - 90U <= 8U)
            work.descriptor = &QABT_DESCRIPTORS[g_entryPoint - 90U];
    }
    QabtMain(work);
}
void RunQabtEvent(std::uintptr_t argument) {
    QueueQabtReadyInit(argument);
    SetD3TRenderSettings(1);
}
const ScriptAreaDefinition qabtScriptDefinition{.areaId = QABT_AREA, .field04 = 0, .init = InitializeQabtEvent, .main = RunQabtEvent};
} // namespace shenmue::s2::areas
