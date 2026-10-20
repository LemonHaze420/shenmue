#include "s2/street_fight_event.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/camera.hpp"
#include "s2/event_call.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/motion_work.hpp"
#include "s2/character.hpp"
#include "s2/model_component.hpp"
#include "s2/pak.hpp"
#include "s2/hand.hpp"

namespace shenmue::s2 {

namespace {
// @verified - p
const StreetFightFighterDefinition *ReturnZeroStreetFightDebugFighter() {
    return nullptr;
}
}

StreetFightDebugFighterCallback streetFightDebugFighterCallback = ReturnZeroStreetFightDebugFighter;
std::uint32_t streetFightSpotlightPositioned{};

const StreetFightFighterDefinition *GetStreetFightDebugFighterDefinition() {
    return streetFightDebugFighterCallback();
}

// @verified - p
std::int64_t RecordStreetFightRound(StreetFightEventWork *work) {
    const std::int8_t round = work->round;
    work->roundResults[round] = work->battleStatus == 3 ? 1 : -1;
    work->wins = 0;
    work->losses = 0;
    for (std::int32_t i = 0; i <= round; ++i) {
        if (work->roundResults[i] <= 0)
            ++work->losses;
        else
            ++work->wins;
    }
    if (work->wins >= 2)
        return 1;
    if (work->losses >= 2)
        return 2;
    work->round = static_cast<std::int8_t>(round + 1);
    return 0;
}

// @verified - p
std::uint32_t CommitStreetFightStoryRank(std::int32_t stage) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014))
        return 0;
    std::int32_t flag = 0;
    std::int32_t rankDelta = 0;
    switch (stage) {
    case 1:
        flag = 0x127;
        break;
    case 2:
        flag = 0x25a;
        rankDelta = 1;
        break;
    case 3:
        flag = 0x25d;
        rankDelta = 5;
        break;
    case 5:
        flag = 0x25b;
        rankDelta = 6;
        break;
    case 7:
        flag = 0x25c;
        rankDelta = 4;
        break;
    case 8:
        flag = 0x128;
        break;
    case 10:
        flag = 0x25e;
        rankDelta = 3;
        break;
    case 11:
        flag = 0x259;
        rankDelta = 2;
        break;
    case 12:
        flag = 0x126;
        break;
    default:
        break;
    }
    switch (CanPerformForCurrentAreaFlags()) {
    case 1:
        AddStreetFightProgress(rankDelta);
        if (GetStreetFightProgress() < 10) {
            EV_stepSetFlags(1, flag);
            return 1;
        }
        EV_stepSetFlags(0, flag);
        return 2;
    case 2: {
        std::int32_t count = EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 530) != 0;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 529))
            ++count;
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 531))
            ++count;
        if (count <= 1) {
            EV_stepSetFlags(1, flag);
            return 0;
        }
        EV_stepSetFlags(0, flag);
        return 2;
    }
    default:
        return 0;
    }
}

// @verified - p
void ResetStreetFightStoryFlag(std::int32_t stage) {
    if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1014) || !CanPerformForCurrentAreaFlags())
        return;
    std::int32_t flag;
    switch (stage) {
    case 2:
        flag = 0x25a;
        break;
    case 3:
        flag = 0x25d;
        break;
    case 5:
        flag = 0x25b;
        break;
    case 7:
        flag = 0x25c;
        break;
    case 10:
        flag = 0x25e;
        break;
    case 11:
        flag = 0x259;
        break;
    default:
        return;
    }
    EV_stepSetFlags(2, flag);
}

// @verified - p
std::int64_t UnparentAndSetExistingPosRot(shen_name character) {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(character, STREET_FIGHT_INVALID);
    EV_AT_SetPos(character, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    return EV_AT_SetAngY(character, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}

void StreetFightReactionNullsub(void *) {
    nullsub();
}

void StreetFightRestoreChaseCamera(void *) {
    using enum CameraState;
    camCtrlSetState(ChaseGoal);
}

void QueueStreetFightPlayerReaction(shen_name fighter, bool playerWon) {
    EVT_AT_SetCam(playerWon ? 501 : 500, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 5);
    EVT_AT_SetCam(playerWon ? 511 : 510, STREET_FIGHT_RYO, STREET_FIGHT_RYO, 0, 0, 56);
    QueueEventCallFunction(StreetFightReactionNullsub, nullptr, 55);
    EVT_AT_SetFaceLooks(STREET_FIGHT_RYO, 1, 8, 35, 56);
    EVT_AT_SetFaceLooks(STREET_FIGHT_RYO, 3, 0, 5, 88);
    EVT_AT_SetCam(playerWon ? 521 : 520, fighter, fighter, 0, 0, 96);
    QueueEventCallFunction(StreetFightReactionNullsub, nullptr, 136);
    QueueEventCallFunction(StreetFightRestoreChaseCamera, nullptr, 136);
}

std::int64_t PlayStreetFightVictoryAppearance() {
    EV_EnableCharacterDisplay(STREET_FIGHT_CHN);
    EV_EnsureCharacterDraw(STREET_FIGHT_CHN);
    EV_SetCharacterAlphaEnabled(STREET_FIGHT_CHN, false);
    EVT_AT_SetPos(STREET_FIGHT_CHN, STREET_FIGHT_INVALID, STREET_FIGHT_VICTORY_POSITION, 0);
    EVT_AT_SetAngY(STREET_FIGHT_CHN, STREET_FIGHT_INVALID, 172, 0);
    EVT_AT_SetMot(STREET_FIGHT_CHN, 9666, 0, 1, 0x40000000U, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QSFC"), 0);
    EventSetCamera camera{
        .cleanup = 0, .camera = 9901, .eye = STREET_FIGHT_CHN, .target = STREET_FIGHT_CHN, .context = 0, .reserved11 = {}, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    return 60;
}

// @verified - p
std::uint64_t QueueStreetFightMotionSpeed(shen_name character, float speed, std::uint32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(StreetFightMotionSpeedCallback, 6, 11, sizeof(EventSetMotion), STREET_FIGHT_EVENT_TASK);
    HLib::SetupTimedTask(task, static_cast<std::int32_t>(delay));
    EventSetMotion *const work = static_cast<EventSetMotion *>(HLib::GetTaskParameter(task));
    work->speed = speed;
    work->cleanup = 1;
    work->character = character;
    return delay;
}

// @verified - p
void StreetFightMotionSpeedCallback(void *taskData) {
    EventSetMotion *const work = static_cast<EventSetMotion *>(taskData);
    SetMotionSpeed(scenFindMotionwork(work->character), work->speed, MotionRegion::LowerBody);
    if (work->cleanup)
        HLib::DestroyCurrentTask();
}

// @verified - p
void StreetFightHighModelCallback(void *taskData) {
    StreetFightHighModelWork *const work = static_cast<StreetFightHighModelWork *>(taskData);
    if (work->selector == 0) {
        SetHighModelXYZ(ctrlGetCharaByName(work->character), work->state, 0, 0);
        SetHighModelXYZ(ctrlGetCharaByName(work->character), work->state, 1, 0);
    } else if (work->selector == 1)
        SetFaceFlagsForChara_0(ctrlGetCharaByName(work->character), work->state);
    if (work->task)
        HLib::DestroyCurrentTask();
}

} // namespace shenmue::s2
