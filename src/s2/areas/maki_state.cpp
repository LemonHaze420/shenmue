#include "s2/areas/maki_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/math.hpp"
#include "s2/motion_work.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"

namespace shenmue::s2::areas {
namespace {
void MakiEventNullsub() {
}
void MakiClearUpperBody() {
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::LeftArm);
    SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::RightArm);
}
void MakiSetStoryCamera(std::int32_t camera) {
    EVT_AT_SetCam(camera, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 0);
    camCtrlDisableFilmBars();
}
void MakiQueueFilm(std::int32_t frames, std::int32_t start, std::int32_t end, std::int32_t delay) {
    const EventFilmControl work{.cleanup = 0,
        .frames = frames,
        .startAlpha = start,
        .startRed = 0,
        .startGreen = 0,
        .startBlue = 0,
        .targetAlpha = end,
        .targetRed = 0,
        .targetGreen = 0,
        .targetBlue = 0,
        .delay = static_cast<float>(delay)};
    EVT_AT_MenCtrl(work);
}
void MakiPositionShenhua() {
    EVT_AT_SetPos(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, {.x = 154.9f, .y = 100, .z = 393.5f}, 0);
    EVT_AT_SetAngY(MAKI_SHENHUA, EVENT_CHARACTER_INVALID, 0, 0);
}
} // namespace

std::int32_t MakiCollectFirewood() {
    switch (makiRuntime.phase) {
    case 0: {
        Vector3 position;
        EV_GetCharaPosition(MAKI_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        EV_GetCharaPosition(MAKI_RYO, CHARACTER_TRANSLATION_ROOT, &makiSavedPosition, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(MAKI_RYO, &makiSavedRotation, EventTransformFlags::World);
        const shen_name selected = srchGetTargetName(0);
        std::int32_t index = 0;
        while (makiFirewoodTargets[index].name.name && makiFirewoodTargets[index].name.name != selected.name)
            ++index;
        const shen_name character = makiFirewoodTargets[index].name;
        srchSetTargetEnabled(character, 0);
        makiSelectedFirewood = index;
        srchReset();
        MakiLockPlayerForSequence();
        const MakiFirewoodPlacement &placement = makiFirewoodPlacements[index];
        if (placement.facing) {
            EVT_AT_SetAngY(MAKI_RYO, character, 0, 0);
            EVT_AT_SetAngY(MAKI_RYO, MAKI_RYO, -50, 0);
            EVT_AT_SetAngY(character, character, 180, 0);
        } else {
            const float degrees = static_cast<float>(InverseFixedAtan2(placement.position.x - position.x, placement.position.z - position.z)) / 182.04445f;
            EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, degrees, 0);
            EVT_AT_SetAngY(character, MAKI_RYO, 180, 0);
            EVT_AT_SetAngY(MAKI_RYO, MAKI_RYO, -50, 0);
        }
        std::int32_t delay = 0;
        if (makiRuntime.heldCount) {
            switch (placement.type) {
            case 0:
                delay = MakiPickOneLog(character);
                ++makiRuntime.heldCount;
                break;
            case 1:
                delay = MakiPickTwoLogs(character);
                makiRuntime.heldCount += 5;
                break;
            case 2:
                delay = MakiPickThreeLogs(character);
                makiRuntime.heldCount += 10;
                break;
            default:
                break;
            }
        } else {
            switch (placement.type) {
            case 0:
                delay = MakiPickSingleLog(character);
                ++makiRuntime.heldCount;
                break;
            case 1:
                delay = MakiPickTwoLogs(character);
                makiRuntime.heldCount += 5;
                break;
            case 2:
                delay = MakiPickThreeLogs(character);
                makiRuntime.heldCount += 10;
                break;
            default:
                break;
            }
        }
        std::uint32_t *command = makiRuntime.heldCount == 1 ? makiSingleLogHandCommand : makiCarryHandCommand;
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MAKI_RYO), 1, reinterpret_cast<std::byte *>(command), 0);
        EVT_AT_SetPoint(MAKI_RYO, character, 5, 3, 1, 0, 0, 0x28000000U);
        EVT_AT_ObjectDisp_1(character, delay, 1);
        EVT_AT_ObjectDisp_1(MAKI_SMALL_LOG, delay, 1);
        EVT_AT_ObjectDisp_1(MAKI_MIDDLE_LOG, delay, 1);
        EVT_AT_ObjectDisp_1(MAKI_LARGE_LOG, delay, 1);
        ++makiRuntime.phase;
        return delay;
    }
    case 1: {
        MakiRefreshHeldLogs();
        if (!EV_GetScreenFilmIsNotShowing())
            SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
        EVT_AT_SetPoint(MAKI_RYO, shen_name{}, 0, 3, 0, 0, 0, 0x50000000U);
        const char *voice = makiRuntime.heldCount < 2 ? "0518A010" : makiRuntime.heldCount < 4 ? "0518A012" : "0518A014"; // @text 0518A010 EN: "This won't be enough." | JA: "これだけじゃ足りないな…" ; 0518A012 EN: "I should look for more." | JA: "もうちょっと探してみるか…" ; 0518A014 EN: "There should be more." | JA: "まだありそうだな…"
        if (makiRuntime.heldCount + makiRuntime.depositedCount >= 20 || makiRuntime.heldCount >= 7)
            voice = "0518A016"; // @text EN: "I should go back." | JA: "いったん戻るか…"
        EVT_AT_SetVoice(voice, 0);
        ++makiRuntime.phase;
        return 60;
    }
    case 2:
        if (makiRuntime.heldCount + makiRuntime.depositedCount >= 20) {
            makiRuntime.state = 4;
            SetPlayerControlEnabled(0);
            makiRuntime.phase = 0;
            return 0;
        }
        makiRuntime.state = 1;
        SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 71, 0, MotionRegion::RightArm);
        mtwkRequestMotion(scenFindMotionwork(MAKI_RYO), 0x4004, MotionRegion::RightArm);
        SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::LeftArm);
        EVT_AT_SetMot(MAKI_RYO, 0xf003, 0, 1, 0x40000000U, 0);
        EV_AT_SetPos(MAKI_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, makiSavedPosition);
        EV_AT_SetAngY(MAKI_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, makiSavedRotation);
        MakiRestorePlayerAfterSequence();
        makiRuntime.phase = 0;
        break;
    default:
        break;
    }
    return 0;
}

std::int32_t MakiReturnToCampfire() {
    using enum CameraState;
    switch (makiRuntime.phase) {
    case 0:
        EVT_AT_SetMot(MAKI_RYO, 0xf003, 0, 1, 0, 0);
        SetPlayerControlEnabled(0);
        MakiQueueFilm(15, 0, 255, 0);
        ++makiRuntime.phase;
        return 15;
    case 1: {
        MakiLockPlayerForSequence();
        makiRuntime.depositedCount += makiRuntime.heldCount;
        MakiClearUpperBody();
        const std::int32_t delay = makiRuntime.heldCount ? MakiDepositLogs() : MakiDiscussEmptyHands();
        ++makiRuntime.phase;
        makiRuntime.heldCount = 0;
        MakiQueueFilm(30, 0, 255, delay);
        return delay + 30;
    }
    case 2:
        MakiRefreshHeldLogs();
        SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
        camCtrlSetState(Init);
        camCtrlSetFovTransitionTo(true);
        MakiRestorePlayerAfterSequence();
        EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 157.1f, .y = 100, .z = 389.4f}, 0);
        EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 315, 0);
        EVT_AT_SetMot(MAKI_RYO, 0xf002, 0, 1, 0x40000000U, 0);
        mtwkRequestMotion(scenFindMotionwork(MAKI_RYO), 0x4004, MotionRegion::RightArm);
        SetMotionSpeed(scenFindMotionwork(MAKI_RYO), .1f, MotionRegion::LeftArm);
        SetMotionRegionFlag(scenFindMotionwork(MAKI_RYO), 73, 0, MotionRegion::LeftArm);
        makiRuntime.state = 1;
        makiRuntime.phase = 0;
        break;
    default:
        break;
    }
    return 0;
}

std::int32_t MakiFinishCollecting() {
    switch (makiRuntime.phase) {
    case 0:
        MakiQueueFilm(30, 0, 255, 0);
        srchReset();
        SetPlayerControlEnabled(0);
        MakiEventNullsub();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        ++makiRuntime.phase;
        return 60;
    case 1:
        shenSetClockAndCompassRenderTo(1);
        MakiLockPlayerForSequence();
        SetScreenFilmFade(60, 255, 0, 0, 0, 0, 0, 0, 0);
        MakiSetStoryCamera(2000);
        EVT_AT_SetCam(2001, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 120);
        EVT_AT_SetCam(2002, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 280);
        MakiClearUpperBody();
        EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 156.8f, .y = 100, .z = 390.7f}, 0);
        EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 157.5f, 0);
        MakiPositionShenhua();
        EVT_AT_SetMot(MAKI_RYO, 0x4006, 0, .8f, 0x40000000U, 0);
        EVT_AT_SetMot(MAKI_RYO, 0xf002, 0, 1, 0x40000000U, 280);
        EVT_AT_SetMot(MAKI_SHENHUA, 0x4008, 0, 1, 0x40000000U, 0);
        EVT_AT_SetVoice("0518B032", 0); // @text EN: "We have enough firewood." | JA: "これだけあれば、もう大丈夫。"
        EVT_AT_SetVoice("0518A044", 80); // @text EN: "Good." | JA: "そうか。"
        EVT_AT_SetVoice("0518B033", 120); // @text EN: "Thank you." | JA: "ありがとう…"
        EVT_AT_SetVoice("0518B034", 150); // @text EN: "Come by the fire\nto get warm, Ryo." | JA: "さあ、リョウも火にあたって、\n体をあたためて。"
        EVT_AT_SetVoice("0518A045", 280); // @text EN: "Yeah." | JA: "ああ、そうだな。"
        ++makiRuntime.phase;
        EVT_AT_SetPoint(MAKI_RYO, MAKI_SHENHUA, 5, 3, 1, 280, 340, 0);
        EVT_AT_SetPoint(MAKI_SHENHUA, MAKI_RYO, 5, 3, 1, 20, 340, 0);
        return 310;
    case 2:
        MakiQueueFilm(30, 0, 255, 0);
        ++makiRuntime.phase;
        spmFadeOut();
        return 30;
    case 3:
        MakiClearUpperBody();
        makiRuntime.state = 6;
        makiRuntime.phase = 0;
        return 10;
    default:
        return 0;
    }
}

std::int32_t MakiNightfall() {
    switch (makiRuntime.phase) {
    case 0:
        srchReset();
        SetPlayerControlEnabled(0);
        EVT_AT_SetMot(MAKI_RYO, 0xf002, 0, 1, 0, 0);
        SetScreenFilmFade(90, 0, 0, 0, 0, 255, 0, 0, 0);
        MakiEventNullsub();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        ++makiRuntime.phase;
        return 150;
    case 1:
        shenSetClockAndCompassRenderTo(1);
        MakiLockPlayerForSequence();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        MakiSetStoryCamera(3000);
        EVT_AT_SetCam(3001, EVENT_CHARACTER_INVALID, EVENT_CHARACTER_INVALID, 0, 0, 140);
        MakiClearUpperBody();
        EVT_AT_SetPos(MAKI_RYO, EVENT_CHARACTER_INVALID, {.x = 156.9f, .y = 100, .z = 389}, 0);
        EVT_AT_SetAngY(MAKI_RYO, EVENT_CHARACTER_INVALID, 157.5f, 0);
        MakiPositionShenhua();
        EVT_AT_SetMot(MAKI_RYO, 0xf005, 0, 1, 0x40000000U, 0);
        EVT_AT_SetMot(MAKI_RYO, 0xf003, 0, 1, 0, 60);
        EVT_AT_SetMot(MAKI_SHENHUA, 0x4008, 0, 1, 0x40000000U, 0);
        if (makiRuntime.heldCount)
            mtwkRequestMotion(scenFindMotionwork(MAKI_RYO), 0x4004, MotionRegion::LeftArm);
        EVT_AT_SetVoice("0518B015", 60); // @text EN: "Did you find any?" | JA: "どう、集まった？"
        EVT_AT_SetVoice("0518A017", 140); // @text EN: "Well, not really=@" | JA: "いや、それが…"
        EVT_AT_SetPoint(MAKI_SHENHUA, MAKI_RYO, 5, 3, 1, 20, 210, 0);
        ++makiRuntime.phase;
        return 230;
    case 2:
        MakiSetStoryCamera(3002);
        EVT_AT_SetMot(MAKI_SHENHUA, 0x4009, 0, 1, 0x40000000U, 0);
        EVT_AT_SetAngY(MAKI_SHENHUA, MAKI_SHENHUA, 30, 0);
        EVT_AT_SetVoice("0518B016", 0); // @text EN: "I'll help you then." | JA: "じゃあ、私も一緒に手伝うわ。"
        ++makiRuntime.phase;
        return 70;
    case 3:
        spmFadeOut();
        MakiQueueFilm(30, 0, 255, 0);
        ++makiRuntime.phase;
        return 200;
    case 4:
        MakiClearUpperBody();
        makiRuntime.state = 6;
        makiRuntime.phase = 0;
        return 10;
    default:
        return 0;
    }
}
} // namespace shenmue::s2::areas
