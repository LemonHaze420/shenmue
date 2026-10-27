#include "s2/areas/area_ahqb.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/qte_hand_poses.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
namespace shenmue::s2::areas {
// @verified - p
void AHQBPlaceCombatants() {
    EVT_AT_SetPos(AHQB_PLAYER, AHQB_BASE, {-0.2f, 0, 0.5f}, 0);
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_BASE, 0, 0);
    EVT_AT_SetPos(AHQB_HTK, AHQB_BASE, {0.45f, 0, 0.28f}, 0);
    EVT_AT_SetAngY(AHQB_HTK, AHQB_BASE, 0, 0);
    EVT_AT_SetPos(AHQB_GOI, AHQB_BASE, {0.5f, 0, 1.3f}, 0);
    EVT_AT_SetAngY(AHQB_GOI, AHQB_BASE, 135, 0);
}
// @verified - p
std::int32_t AHQBBeginCombat(AhqbEventWork *work) {
    EV_LoadAudio("0010A000", 1);
    AHQBPlaceCombatants();
    if (g_currentMap.name == AREA_AHQB.name)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EVT_AT_SetCam(2000, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_SetAngY(AHQB_GOI, AHQB_BASE, 45, 0);
    EventSetPositionAngle position{0, AHQB_PLAYER, AHQB_BASE, {-0.3f, 0, 0.5f}, 30, 0};
    EVT_AT_SetPosAng_Callback(&position);
    position.character = AHQB_HTK;
    position.parent = AHQB_PLAYER;
    position.position = {1.1f, 0, -1.2f};
    position.degrees = 0;
    EVT_AT_SetPosAng_Callback(&position);

    EventSetMotion motion{0, AHQB_PLAYER, 4236, 1.1f, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4236);
    motion.character = AHQB_HTK;
    motion.animation = 4231;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4231);
    motion.character = AHQB_GOI;
    motion.animation = 4228;
    motion.flags = 0x60000000U;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4228);
    static constexpr std::uint16_t sequence[]{512, 256};
    AHQBQueueComboQte(reinterpret_cast<std::int16_t *>(&work->qteResult[2]), 24, sequence);
    return 0;
}
// @verified - p
std::int32_t AHQBCompleteCombat() {
    EventSetPositionAngle position{0, AHQB_HTK, AHQB_PLAYER, {1.1f, 0, -1.2f}, 0, 0};
    EVT_AT_SetPosAng_Callback(&position);
    EVT_AT_SetPosAng(AHQB_GOI, AHQB_PLAYER, 1.8f, 1.2f, 0, 25);
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_PLAYER, -10, 0);
    EVT_AT_SetAngY(AHQB_HTK, AHQB_HTK, 30, 0);
    EventSetMotion motion{0, AHQB_PLAYER, 4236, 30, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4236);
    EVT_AT_SetMot(AHQB_PLAYER, 4237, 10, 1, 0x44000000U, 25);
    motion.character = AHQB_HTK;
    motion.animation = 4231;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4231);
    EVT_AT_SetMot(AHQB_GOI, 4227, 10, 1, 0x44000000U, 25);
    EVT_AT_DispBlur(AHQB_PLAYER, 4, 15, 2, 20, 0);
    EVT_AT_DispBlur(AHQB_PLAYER, 6, 15, 2, 30, 25);
    EVT_AT_SetCam(2004, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_SetCam(2012, AHQB_BASE, AHQB_BASE, 0, 0, 25);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(AHQB_PLAYER), 0, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 0);
    Queue_EVT_AT_CtrlHandUp_1(AHQB_PLAYER, 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 10, 40);
    return 100;
}
// @verified - p
std::int32_t AHQBFirstMiss() {
    EVT_AT_SetAngY(AHQB_HTK, AHQB_HTK, 15, 0);
    EVT_AT_SetPosAng(AHQB_GOI, AHQB_PLAYER, 1.2f, 0.2f, 0, 20);
    EVT_AT_SetMot(AHQB_PLAYER, 4235, 35, 1, 0x44000000U, 20);
    EVT_AT_SetMot(AHQB_GOI, 4226, 35, 1, 0x44000000U, 20);
    EVT_AT_DispBlur(AHQB_PLAYER, 4, 8, 1, 20, 20);
    EVT_AT_SetCam(2004, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_SetCam(2005, AHQB_BASE, AHQB_BASE, 0, 0, 20);
    EVT_AT_SetFaceLooks(AHQB_PLAYER, FacialExpressionId::Suffer, 10, 1, 25);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(AHQB_PLAYER), 0, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 0);
    Queue_EVT_AT_CtrlHandUp_1(AHQB_PLAYER, 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 10, 40);
    return 60;
}
// @verified - p
std::int32_t AHQBFirstFailure() {
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_PLAYER, -15, 0);
    EventSetPositionAngle position{0, AHQB_HTK, AHQB_PLAYER, {0.35f, 0, -1.4f}, 0, 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{0, AHQB_PLAYER, 4234, 30, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4234);
    motion.character = AHQB_HTK;
    motion.animation = 4230;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4230);
    EVT_AT_SetCam(2001, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    return 20;
}
// @verified - p
std::int32_t AHQBFirstCounter() {
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_PLAYER, 180, 0);
    EventSetPositionAngle position{0, AHQB_HTK, AHQB_PLAYER, {-0.7f, 0, 0.6f}, 30, 0};
    EVT_AT_SetPosAng_Callback(&position);

    EventSetMotion motion{0, AHQB_PLAYER, 4232, -0.7f, 1, 0x44000000U, 30};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4232);
    motion.character = AHQB_HTK;
    motion.animation = 4229;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4229);
    EVT_AT_SetCam(2009, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_DispBlur(AHQB_PLAYER, 4, 8, 1, 20, 10);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(AHQB_PLAYER), 0, reinterpret_cast<std::byte *>(fortuneLeftReadingHandPose), 0);
    Queue_EVT_AT_CtrlHandUp_1(AHQB_PLAYER, 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 10, 40);
    return 20;
}
// @verified - p
std::int32_t AHQBFirstFailureCamera() {
    EVT_AT_SetCam(2002, AHQB_BASE, AHQB_BASE, 0, 0, -1);
    EVT_AT_SetCam(2003, AHQB_BASE, AHQB_BASE, 0, 0, 60);
    SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(AHQB_PLAYER), 6, 10, 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(AHQB_HTK), 0, reinterpret_cast<std::byte *>(opponentCounterHandPose), 0);
    return 90;
}
// @verified - p
std::int32_t AHQBSecondSuccess() {
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_PLAYER, 180, 0);
    EventSetPositionAngle position{0, AHQB_GOI, AHQB_PLAYER, {1.8f, 0, 1.2f}, 0, 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{0, AHQB_PLAYER, 4237, 15, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4237);
    motion.character = AHQB_GOI;
    motion.animation = 4227;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4227);
    EVT_AT_SetCam(2011, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_DispBlur(AHQB_PLAYER, 6, 15, 2, 30, 0);
    return 70;
}
// @verified - p
std::int32_t AHQBSecondFailure() {
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_BASE, 0, 0);
    EventSetPositionAngle position{0, AHQB_GOI, AHQB_PLAYER, {1.2f, 0, 0.2f}, 0, 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{0, AHQB_PLAYER, 4235, 40, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4235);
    motion.character = AHQB_GOI;
    motion.animation = 4226;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4226);
    EVT_AT_SetCam(2010, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_SetFaceLooks(AHQB_PLAYER, FacialExpressionId::Suffer, 10, 1, 25);
    return 35;
}
// @verified - p
std::int32_t AHQBFinalSuccess() {
    EVT_AT_SetAngY(AHQB_PLAYER, AHQB_BASE, 15, 0);
    EventSetPositionAngle position{0, AHQB_GOI, AHQB_PLAYER, {1.1f, 0, -0.3f}, 0, 0};
    EVT_AT_SetPosAng_Callback(&position);
    EventSetMotion motion{0, AHQB_PLAYER, 4233, 1.1f, 1, 0x44000000U, 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4233);
    motion.character = AHQB_GOI;
    motion.animation = 4225;
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(4225);
    EVT_AT_SetCam(2008, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_DispBlur(AHQB_PLAYER, 4, 15, 1, 30, 10);
    return 80;
}
// @verified - p
std::int32_t AHQBFinalFailure() {
    EVT_AT_SetCam(2007, AHQB_BASE, AHQB_BASE, 0, 0, 0);
    EVT_AT_SetFaceLooks(AHQB_PLAYER, FacialExpressionId::Suffer, 10, 1, 10);
    return 90;
}
// @verified - p
std::int32_t AHQBCombatPhase(AhqbEventWork *work) {
    switch (work->phase) {
    case 0: {
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        for (const shen_name character : {AHQB_PLAYER, AHQB_HTK, AHQB_GOI})
            SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(character), 1, 0, 1);
        for (const shen_name character : {AHQB_PLAYER, AHQB_HTK, AHQB_GOI}) {
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 0, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 1, reinterpret_cast<std::byte *>(handPoseReplacementCommand), 0);
        }
        const std::int32_t delay = AHQBBeginCombat(work);
        work->phase = 1;
        return delay;
    }
    case 1: {
        if (static_cast<std::int16_t>(work->qteResult[2]) == 2) {
            const std::int32_t delay = AHQBCompleteCombat();
            work->state = 3;
            work->phase = 0;
            return delay;
        }
        if (static_cast<std::int16_t>(work->qteResult[2]) == 1) {
            const std::int32_t delay = AHQBFirstMiss();
            work->phase = 6;
            return delay;
        }
        const std::int32_t delay = AHQBFirstFailure();
        work->phase = 2;
        return delay;
    }
    case 2:
        QueueSkQteWait(work->qteResult, 512, 24, 20, AHQBFirstCounter, AHQBFirstFailureCamera, 0);
        work->phase = 3;
        break;
    case 3:
        if (!static_cast<std::int16_t>(work->qteResult[2])) {
            work->state = 4;
            work->phase = 0;
        } else
            work->phase = 4;
        break;
    case 4:
        QueueSkQteWait(work->qteResult, 256, 20, 20, AHQBSecondSuccess, AHQBSecondFailure, 0);
        work->phase = 5;
        break;
    case 5:
        if (!static_cast<std::int16_t>(work->qteResult[2]))
            work->phase = 6;
        else {
            work->state = 3;
            work->phase = 0;
        }
        break;
    case 6:
        EVT_AT_SetPos(AHQB_PLAYER, AHQB_BASE, {-0.451f, 0, 0.308f}, 0);
        EVT_AT_SetAngY(AHQB_PLAYER, AHQB_BASE, 30, 0);
        EVT_AT_SetPos(AHQB_GOI, AHQB_BASE, {0.49f, 0, -0.313f}, 0);
        EVT_AT_SetAngY(AHQB_GOI, AHQB_BASE, 30, 0);
        EVT_AT_SetPos(AHQB_GOI, AHQB_BASE, {0.57f, 0, -0.483f}, 0);
        EVT_AT_SetAngY(AHQB_GOI, AHQB_BASE, 30, 0);
        EVT_AT_SetPos(AHQB_GOI, AHQB_BASE, {0.52f, 0, -0.39f}, 0);
        EVT_AT_SetAngY(AHQB_GOI, AHQB_BASE, 30, 0);
        QueueSkQteWait(work->qteResult, 512, 20, 20, AHQBFinalSuccess, AHQBFinalFailure, 0);
        EVT_AT_SetCam(2006, AHQB_BASE, AHQB_BASE, 0, 0, 0);
        EVT_AT_SetFaceLooks(AHQB_PLAYER, FacialExpressionId::Angry, 10, 1, 20);
        work->phase = 7;
        break;
    case 7:
        work->state = static_cast<std::int16_t>(work->qteResult[2]) ? 3 : 4;
        work->phase = 0;
        break;
    default:
        break;
    }
    return 0;
}
} // namespace shenmue::s2::areas
