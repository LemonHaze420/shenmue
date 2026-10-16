#include "s2/ren_qte_choreography.hpp"
#include "s2/character.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"

namespace shenmue::s2 {
namespace {
void QueueDoorAngle(shen_name character, shen_name base, float delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_SetAngY_Callback, 6, 0x0b, sizeof(EventSetAngle), REN_QTE_EVENT_TASK);
    RegisterEventTask(task);
    EventSetAngle *const angle = static_cast<EventSetAngle *>(HLib::GetTaskParameter(task));
    angle->cleanup = 1;
    angle->character = character;
    angle->parent = base;
    angle->degrees = 0;
    angle->delay = delay;
}
} // namespace
// @verified - p
void TurnRenQteLeftSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_RUN_PLAYER_POSITION);
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_LEFT);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_RUN_REN_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_LEFT);
    work->timer = work->entry->successFrames ? work->entry->successFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 90, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 10, 1, 0, frames);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, {0.7F, 0, 0}, frames);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 90, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 0, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void TurnRenQteLeftFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_LEFT_FAILURE_PLAYER_POSITION);
    const std::int32_t frames = static_cast<std::int32_t>(static_cast<float>(PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_LEFT_MISS, 0, 0.85F)) / 0.85F);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_LEFT_FAILURE_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_LEFT_MISS, 0, 0.85F);
    work->timer = work->entry->failureFrames ? work->entry->failureFrames : frames;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 90, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 1, 1, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, work->base, 90, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 9, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void TurnRenQteRightSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_RUN_PLAYER_POSITION);
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_RIGHT);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_RUN_REN_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_RIGHT);
    work->timer = work->entry->successFrames ? work->entry->successFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 270, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 1, 1, 0x40000000U, frames);
        EVT_AT_SetAngY(REN_QTE_REN, work->base, 270, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0x40000000U, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void TurnRenQteRightFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_RIGHT_FAILURE_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_TRIP);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_RIGHT_FAILURE_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_TRIP);
    work->timer = work->entry->failureFrames ? work->entry->failureFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 270, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 9, 1, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, work->base, 270, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void ContinueRenQteRunSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_RUN_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_RUN_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_RUN);
    work->state = REN_QTE_FINISH;
    work->timer = frames + 30;
}
// @verified - p
void ContinueRenQteRunFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_TRIP_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_TRIP);
    EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 0, 1, 0, frames);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_TRIP_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_TRIP);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 0, 1, 0, frames);
    work->state = REN_QTE_FINISH;
    work->timer = frames + 30;
}
// @verified - p
void JumpRenQtePitSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_RUN_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_JUMP, 0, 1, 0x44000000U);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_RUN_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_JUMP, 0, 1, 0x44000000U);
    work->timer = work->entry->successFrames ? work->entry->successFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 3, 1, 0, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 3, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void JumpRenQtePitFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_PIT_PLAYER_POSITION);
    const std::int32_t fallFrames = static_cast<std::int32_t>(static_cast<float>(PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_PIT_FALL, 0, 1.3F)) / 1.3F);
    EVT_AT_SetPos(REN_QTE_PLAYER, work->base, {0.4F, -0.8F, -1.3F}, fallFrames);
    EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 0, fallFrames);
    const std::int32_t climbFrames = EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_PIT_CLIMB, 0, 1, 0x40000000U, fallFrames);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_PIT_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_PIT_FALL, 0, 1.3F);
    EVT_AT_SetPos(REN_QTE_REN, work->base, {-0.4F, -0.8F, -1.3F}, fallFrames);
    EVT_AT_SetAngY(REN_QTE_REN, work->base, 0, fallFrames);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_PIT_CLIMB, 0, 1, 0x40000000U, fallFrames);
    work->timer = work->entry->failureFrames ? work->entry->failureFrames : climbFrames;
    if (work->timer > climbFrames) {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, {0, 0.8F, 0}, climbFrames);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_PLAYER, 0, climbFrames);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, {0, 0.8F, 0}, climbFrames);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 0, climbFrames);
    } else if (work->lives > 0) {
        EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, {0, 0.8F, 0}, work->timer);
        EVT_AT_SetAngY(REN_QTE_PLAYER, REN_QTE_PLAYER, 0, work->timer);
        EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, {0, 0.8F, 0}, work->timer);
        EVT_AT_SetAngY(REN_QTE_REN, REN_QTE_REN, 0, work->timer);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 9, 1, 0x40000000U, work->timer);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0x40000000U, work->timer);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void DodgeRenQtePoleSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_DODGE_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_DODGE);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_DODGE_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_DODGE);
    work->timer = work->entry->successFrames ? work->entry->successFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetAngY(REN_QTE_PLAYER, work->base, 0, frames);
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 1, 1, 0, frames);
        EVT_AT_SetAngY(REN_QTE_REN, work->base, 0, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void DodgeRenQtePoleFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_POLE_PLAYER_POSITION);
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_POLE_HIT);
    EVT_AT_SetPos(REN_QTE_PLAYER, REN_QTE_PLAYER, {-1, 0, 0.8F}, 71);
    QueueDoorAngle(REN_QTE_PLAYER, REN_QTE_PLAYER, 71);
    const std::int32_t frames = EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 5, 1, 0, 160);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_POLE_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_POLE_HIT);
    EVT_AT_SetPos(REN_QTE_REN, REN_QTE_REN, {-1.1F, 0, 0.3F}, 71);
    QueueDoorAngle(REN_QTE_REN, REN_QTE_REN, 71);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0, 160);
    work->state = REN_QTE_FINISH;
    work->timer = work->entry->failureFrames ? work->entry->failureFrames : frames + 30;
}
// @verified - p
void SwitchRenQteLeaderSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_SWITCH_SUCCESS_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_SWITCH_SUCCESS);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_SWITCH_SUCCESS_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_SWITCH_SUCCESS);
    work->state = REN_QTE_FINISH;
    work->timer = work->entry->successFrames ? work->entry->successFrames : frames + 30;
}
// @verified - p
void SwitchRenQteLeaderFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_SWITCH_FAILURE_PLAYER_POSITION);
    const std::int32_t frames = PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_SWITCH_FAILURE);
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_SWITCH_FAILURE_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_SWITCH_FAILURE);
    work->timer = work->entry->failureFrames ? work->entry->failureFrames : frames + 30;
    if (work->timer > frames) {
        EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_RUN, 9, 1, 0, frames);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_RUN, 1, 1, 0, frames);
    }
    work->state = REN_QTE_FINISH;
}
// @verified - p
void BreakRenQteDoorSuccess(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_BREAK_PLAYER_POSITION);
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_BREAK, 20);
    EVT_AT_SetPos(REN_QTE_PLAYER, work->base, REN_QTE_BREAK_PLAYER_POSITION, 16);
    QueueDoorAngle(REN_QTE_PLAYER, work->base, 16);
    EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_BREAK, 20, 1, 0x40000000U, 16);
    EVT_AT_SetPos(REN_QTE_PLAYER, work->base, REN_QTE_BREAK_PLAYER_POSITION, 31);
    QueueDoorAngle(REN_QTE_PLAYER, work->base, 31);
    EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_BREAK, 20, 1, 0x40000000U, 31);
    const std::int32_t frames = EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_BREAK, 85, 1, 0x40000000U, 51);
    SetupHandPoseScript(ctrlGetCharaByName(REN_QTE_PLAYER));
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_BREAK_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_BREAK, 20);
    EVT_AT_SetPos(REN_QTE_REN, work->base, REN_QTE_BREAK_REN_POSITION, 16);
    QueueDoorAngle(REN_QTE_REN, work->base, 16);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_BREAK, 20, 1, 0x40000000U, 16);
    EVT_AT_SetPos(REN_QTE_REN, work->base, REN_QTE_BREAK_REN_POSITION, 31);
    QueueDoorAngle(REN_QTE_REN, work->base, 31);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_BREAK, 20, 1, 0x40000000U, 31);
    EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_BREAK, 85, 1, 0x40000000U, 51);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(RenQteCrushDoorCallback, 5, 0x0b, sizeof(RenQteCrushDoorWork), REN_QTE_DOOR_TASK);
    RenQteCrushDoorWork *const door = static_cast<RenQteCrushDoorWork *>(HLib::GetTaskParameter(task));
    door->wait = 13;
    door->frame = 1;
    door->lastFrame = 100;
    door->restoreIntactDoor = 0;
    HLib::SetupTimedTask(task, 30);
    work->state = REN_QTE_FINISH;
    work->timer = frames - 75;
    EVT_AT_MenCtrl({0, 20, 0, 0, 0, 0, 255, 0, 0, 0, static_cast<float>(frames - 95)});
    renQteDoorMotionPending = 0;
}
// @verified - p
void BreakRenQteDoorFailure(RenQteWork *work) {
    SetRenQtePose(REN_QTE_PLAYER, work->base, REN_QTE_KICK_PLAYER_POSITION);
    PlayRenQteMotion(REN_QTE_PLAYER, REN_QTE_PLAYER_KICK);
    SetupHandPoseScript(ctrlGetCharaByName(REN_QTE_PLAYER));
    SetRenQtePose(REN_QTE_REN, work->base, REN_QTE_KICK_REN_POSITION);
    PlayRenQteMotion(REN_QTE_REN, REN_QTE_REN_KICK);
    if (work->lives <= 0)
        work->timer = 140;
    else {
        const std::int32_t frames = EVT_AT_SetMot(REN_QTE_PLAYER, REN_QTE_PLAYER_KICK, 200, 1, 0x40000000U, 140);
        EVT_AT_SetMot(REN_QTE_REN, REN_QTE_REN_KICK, 200, 1, 0x40000000U, 140);
        work->state = REN_QTE_START;
        work->timer = frames;
    }
    renQteDoorMotionPending = 0;
}
// @verified - p
void RenQteCrushDoorCallback(void *parameter) {
    RenQteCrushDoorWork *const work = static_cast<RenQteCrushDoorWork *>(parameter);
    if (work->wait) {
        --work->wait;
        return;
    }
    EV_DisableCharacterDisplay(REN_QTE_DOOR);
    EV_EnableCharacterDisplay(REN_QTE_BROKEN_DOOR);
    const std::uint32_t frame = static_cast<std::uint32_t>(work->frame) * 2U;
    for (std::int32_t track = 0; track < 20; ++track) {
        Vector3 position;
        Vector3i rotation;
        dynaReadEvaluate(track, static_cast<std::int32_t>(frame), reinterpret_cast<float *>(&position), &rotation,
            reinterpret_cast<const std::uint8_t *>(&REN_QTE_DOOR_DYNAMICS.name));
        position.x *= 10;
        position.y *= 10;
        position.z *= 10;
        EV_scenApplyPosOrRotToPartOf(
            REN_QTE_BROKEN_DOOR, static_cast<std::uint16_t>(REN_QTE_DOOR_PARTS[track]), &position, &rotation, EventCharacterPartTransformMode::Absolute);
    }
    if (++work->frame > work->lastFrame) {
        if (work->restoreIntactDoor) {
            EV_EnableCharacterDisplay(REN_QTE_DOOR);
            EV_DisableCharacterDisplay(REN_QTE_BROKEN_DOOR);
        }
        HLib::DestroyCurrentTask();
    }
}
} // namespace shenmue::s2
