#include "s2/areas/area_uede.hpp"
#include "s2/areas/uede_dice_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void Camera(std::int32_t id) {
    EventSetCamera camera{.camera = id, .eye = UEDE_BASE, .target = UEDE_BASE};
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
}
void Sound(std::uint32_t id, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetSound_Callback, 6, 0x0b, sizeof(EventSetSound), UEDE_TASK);
    RegisterEventTask(task);
    EventSetSound *sound = static_cast<EventSetSound *>(HLib::GetTaskParameter(task));
    sound->cleanup = 1;
    sound->sound = id;
    sound->delay = delay;
}
void QueueHand(shen_name character, std::int32_t side, std::int32_t (*pose)[3], std::uint32_t ticks, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_CtrlHandUp_Callback, 6, 0x0b, sizeof(EventControlHand), UEDE_TASK);
    RegisterEventTask(task);
    EventControlHand *hand = static_cast<EventControlHand *>(HLib::GetTaskParameter(task));
    hand->cleanup = 1;
    hand->character = character;
    hand->side = static_cast<std::uint32_t>(side);
    hand->command = reinterpret_cast<std::byte *>(pose);
    hand->ticks = ticks;
    hand->delay = delay;
}
void QueueDisplay(shen_name character) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_ObjectDisp_Callback, 6, 0x0b, sizeof(StepObjectDisplayWork), UEDE_TASK);
    RegisterEventTask(task);
    StepObjectDisplayWork *display = static_cast<StepObjectDisplayWork *>(HLib::GetTaskParameter(task));
    display->cleanup = 1;
    display->character = character;
    display->mode = 1;
    display->delay = -1;
}
} // namespace
void UedeDiceNullsub() {
}
void UedeSetMob(void *data) {
    if (static_cast<const std::int32_t *>(data)[1])
        HLib::DestroyCurrentTask();
}
void UedeQueueDice(std::int16_t first, std::int16_t second) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(UedeRollDice, 4, 0x0b, sizeof(UedeDiceWork), UEDE_TASK);
    UedeDiceWork *work = static_cast<UedeDiceWork *>(HLib::GetTaskParameter(task));
    work->frame = 2;
    work->faces[0] = first;
    work->faces[1] = second;
    work->trajectory = shenGetRandomInt32(4);
    EV_EnableCharacterDisplay(UEDE_DICE[0]);
    EV_EnableCharacterDisplay(UEDE_DICE[1]);
    Sound(274, 10);
}
void UedeRollDice(void *data) {
    UedeDiceWork &work = *static_cast<UedeDiceWork *>(data);
    static constexpr const std::uint8_t trajectories[4][4]{{'D', 'A', 'T', '1'}, {'D', 'A', 'T', '2'}, {'D', 'A', 'T', '3'}, {'D', 'A', 'T', '4'}};
    if (work.frame == 2) {
        UedeDiceNullsub();
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        EV_AT_SetPos(UEDE_DICE[0], EventTransformFlags::AllAxes, uedeDiceOrigins[2 * work.trajectory]);
        EV_AT_SetPos(UEDE_DICE[1], EventTransformFlags::AllAxes, uedeDiceOrigins[2 * work.trajectory + 1]);
    }
    Vector3i correction[2];
    for (std::int32_t die = 0; die < 2; ++die) {
        const Vector3 &angles = uedeDiceFaceAngles[6 * die + work.faces[die]];
        correction[die] = {static_cast<std::uint16_t>(static_cast<std::int32_t>((angles.x * 8192) / 45)),
            static_cast<std::uint16_t>(static_cast<std::int32_t>((angles.y * 8192) / 45)),
            static_cast<std::uint16_t>(static_cast<std::int32_t>((angles.z * 8192) / 45))};
        Vector3 position;
        Vector3i rotation;
        dynaReadEvaluate(die, 120, &position.x, &rotation, trajectories[work.trajectory]);
        correction[die].x -= rotation.x;
        correction[die].y -= rotation.y;
        correction[die].z -= rotation.z;
    }
    for (std::int32_t die = 0; die < 2; ++die) {
        Vector3 position;
        Vector3i rotation;
        dynaReadEvaluate(die, static_cast<std::int32_t>(work.frame), &position.x, &rotation, trajectories[work.trajectory]);
        const Vector3 &origin = uedeDiceOrigins[2 * work.trajectory + die];
        position.x += origin.x;
        position.y += origin.y;
        position.z += origin.z;
        rotation.x += correction[die].x;
        rotation.y += correction[die].y;
        rotation.z += correction[die].z;
        EV_AT_SetPos(UEDE_DICE[die], EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(UEDE_DICE[die], EventTransformFlags::AllAxes, rotation);
    }
    work.frame += 2;
    if (work.frame > 120) {
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        FinishLoading();
        HLib::DestroyCurrentTask();
    }
}
void UedeSayTotal(std::int16_t total) {
    UedeAppendVoice(1, shenGetRandomInt32(2) ? "014" : "015");
    if (total >= 9) {
        Sound(988, 60);
        Sound(990, 50);
    } else if (total > 4)
        Sound(951, 30);
    else {
        Sound(988, 30);
        Sound(989, 50);
    }
    HLib::Task *task = HLib::EnqueueTaskWithParameter(UedeSetMob, 3, 0x0b, 8, UEDE_TASK);
    std::int32_t *mob = static_cast<std::int32_t *>(HLib::GetTaskParameter(task));
    mob[0] = 0;
    mob[1] = 1;
    HLib::SetupTimedTask(task, 40);
    static constexpr const char *voices[]{"016", "017", "018", "019", "020", "021", "022", "023", "024", "025", "026"};
    if (total >= 2 && total <= 12)
        UedeAppendVoice(1, voices[total - 2]);
}
void UedeOpponentThrow(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    switch (work.state) {
    case 0: {
        uedeDiceSpeaker = static_cast<std::int32_t>(work.opponent.name);
        if (g_currentMap.name != UEDE_AREA.name)
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(uedePlayedFlags[uedeOpponentIndex]), 1);
        EVT_AT_SetMot(work.opponent, uedeThrowMotions[uedeOpponentPose], 0, 1, 0x40000000U, 90);
        UedePlayVoice(1, "012", 0);
        UedePlayVoice(1, "013", 130);
        HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetCam_Callback, 6, 0x0b, sizeof(EventSetCamera), UEDE_TASK);
        RegisterEventTask(task);
        EventSetCamera *camera = static_cast<EventSetCamera *>(HLib::GetTaskParameter(task));
        camera->cleanup = 1;
        camera->camera = 3500;
        camera->eye = UEDE_BASE;
        camera->target = UEDE_BASE;
        camera->context = 0;
        camera->angle = 0;
        camera->delay = 90;
        EVT_AT_SetPosAng(work.opponent, UEDE_BASE, 0, -.8f, 180, 90);
        QueueHand(work.opponent, 0, uedeClosedHandPose, 0, 90);
        EVT_AT_SetPoint(work.opponent, UEDE_RYO, -1, 3, 0, 0, 0, 0x20000000U);
        ++work.state;
        HLib::SetupTaskDelay(156);
        break;
    }
    case 1:
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.opponent), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 8);
        Camera(3501);
        work.opponentDice[0] = static_cast<std::int16_t>(shenGetRandomInt32(6));
        work.opponentDice[1] = static_cast<std::int16_t>(shenGetRandomInt32(6));
        UedeQueueDice(work.opponentDice[0], work.opponentDice[1]);
        ++work.state;
        HLib::SetupTaskDelay(80);
        break;
    case 2:
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.opponent), 0, reinterpret_cast<std::byte *>(uedeCupHandPose), 8);
        ++work.state;
        UedeSayTotal(static_cast<std::int16_t>(work.opponentDice[0] + work.opponentDice[1] + 2));
        UedeQueueVoices(work.self);
        break;
    case 3: {
        EVT_AT_SetPoint(work.opponent, {}, 0, 3, 0, 0, 0, 0x50000000U);
        const std::int32_t sum = work.opponentDice[0] + work.opponentDice[1];
        if (sum == 10) {
            Camera(3700);
            UedeAppendVoice(1, "038");
            UedeAppendVoice(0, "007");
            UedeAppendVoice(1, "039");
            uedeWinStreak = 0;
            work.state = 0;
            if (shenGetMinutesUntilNextFrameMinute()) {
                static constexpr const char *voices[]{"055", "056", "057"};
                UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
                static constexpr const char *below[]{"062", "063"};
                static constexpr const char *above[]{"064", "065"};
                UedeAppendVoice(1, (uedeStartingCash <= static_cast<std::int32_t>(GetPlayerStat(PlayerStatsId::Cash)) ? above : below)[shenGetRandomInt32(2)]);
                UedeCheckMoney(work);
            } else
                HLib::SetCurrentTaskTo(UedeSleep);
        } else {
            if (sum + 2 < 6) {
                static constexpr const char *voices[]{"027", "028", "029"};
                UedeAppendVoice(1, voices[shenGetRandomInt32(3)]);
                EVT_AT_SetMot(work.opponent, uedeSmallMotions[uedeOpponentPose], 0, 1, 0x50000000U, 0);
                ApplyCharacterHandPoseCommand(ctrlGetCharaByName(work.opponent), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 8);
            } else if (sum + 2 < 10) {
                static constexpr const char *voices[]{"030", "031", "032", "033"};
                UedeAppendVoice(1, voices[shenGetRandomInt32(4)]);
                EVT_AT_SetMot(work.opponent, uedeMiddleMotions[uedeOpponentPose], 0, 1, 0x50000000U, 0);
            } else {
                static constexpr const char *voices[]{"034", "035", "036", "037"};
                UedeAppendVoice(1, voices[shenGetRandomInt32(4)]);
                EVT_AT_SetMot(work.opponent, uedeBigMotions[uedeOpponentPose], 0, 1, 0x50000000U, 0);
            }
            work.state = 0;
            HLib::SetCurrentTaskTo(UedePlayerThrow);
        }
        UedeQueueVoices(work.self);
        break;
    }
    default:
        break;
    }
}
void UedePlayerThrow(void *data) {
    UedeWork &work = *static_cast<UedeWork *>(data);
    switch (work.state) {
    case 0: {
        uedeDiceSpeaker = static_cast<std::int32_t>(UEDE_RYO.name);
        QueueDisplay(UEDE_DICE[0]);
        QueueDisplay(UEDE_DICE[1]);
        EventSetPositionAngle placement{.character = UEDE_RYO, .parent = UEDE_BASE, .position = {.1f, 0, .6f}};
        EVT_AT_SetPosAng_Callback(&placement);
        EVT_AT_SetMot(UEDE_RYO, 4425, 0, 1.3f, 0x40000000U, 0);
        EventSetCamera camera{.camera = 1008, .eye = UEDE_CAMERA, .target = UEDE_CAMERA};
        EVT_AT_SetCam_Callback(&camera);
        camCtrlDisableFilmBars();
        EVT_AT_SetPoint(UEDE_RYO, {}, 0, 3, 0, 0, 0, 0x50000000U);
        ApplyCharacterHandPoseCommand(ctrlGetCharaByName(UEDE_RYO), 0, reinterpret_cast<std::byte *>(uedeClosedHandPose), 0);
        work.elapsed = 0;
        ++work.state;
        break;
    }
    case 1: {
        const std::int32_t previous = work.elapsed++;
        if (previous >= 1800) {
            ++work.state;
            break;
        }
        GetBattleDataConfig(0, UEDE_BUTTON);
        if (work.elapsed == 1500)
            UedePlayVoice(1, shenGetRandomInt32(2) ? "041" : "040", 0);
        if (GetBattleDataConfig(1, UEDE_BUTTON) && GetBattleDataConfig(2, UEDE_BUTTON))
            ++work.state;
        break;
    }
    case 2:
        Loop3DSoundEffect(0xc40);
        work.playerDice[0] = static_cast<std::int16_t>(shenGetRandomInt32(6));
        work.playerDice[1] = static_cast<std::int16_t>(shenGetRandomInt32(6));
        if (GetMotionFrame(scenFindMotionwork(UEDE_RYO), MotionRegion::LowerBody) > 33 &&
            GetMotionFrame(scenFindMotionwork(UEDE_RYO), MotionRegion::LowerBody) < 34)
            work.playerDice[1] = 5;
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        Camera(3600);
        UedePlayVoice(0, shenGetRandomInt32(2) ? "009" : "024", 0);
        EVT_AT_SetMot(UEDE_RYO, 4426, 10, 1.5f, 0x40000000U, -1);
        QueueHand(UEDE_RYO, 0, openHandPoseCommand, 8, 30);
        ++work.state;
        HLib::SetupTaskDelay(30);
        break;
    case 3:
        Camera(3601);
        UedeQueueDice(work.playerDice[0], work.playerDice[1]);
        ++work.state;
        HLib::SetupTaskDelay(80);
        break;
    case 4:
        work.state = 0;
        HLib::SetCurrentTaskTo(UedeJudge);
        UedeSayTotal(static_cast<std::int16_t>(work.playerDice[0] + work.playerDice[1] + 2));
        UedeQueueVoices(work.self);
        break;
    default:
        break;
    }
}
} // namespace shenmue::s2::areas
