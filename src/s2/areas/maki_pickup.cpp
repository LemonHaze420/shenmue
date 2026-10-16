#include "s2/areas/maki_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_gameplay_queued.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/step_event.hpp"
#include "s2/qte_hand_poses.hpp"

namespace shenmue::s2::areas {
namespace {
template <class Work> Work &QueueMakiAction(HLib::TaskCallback callback) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(callback, 6, 0x0b, sizeof(Work), EVENT_CHARACTER_TASK);
    RegisterEventTask(task);
    return *static_cast<Work *>(HLib::GetTaskParameter(task));
}
void MakiDetachLog(shen_name character, float delay) {
    EventCling &work = QueueMakiAction<EventCling>(EVT_AT_Cling);
    work.cleanup = 1;
    work.target = character;
    work.character = shen_name{};
    work.mode = 1;
    work.delay = delay;
}
void MakiAttachPickupLog(shen_name character, float x, float z, float delay) {
    EventCling &work = QueueMakiAction<EventCling>(EVT_AT_Cling);
    work.cleanup = 1;
    work.target = character;
    work.character = MAKI_RYO;
    work.bone = 14;
    work.position.x = x;
    work.position.y = 0;
    work.position.z = z;
    work.rotation.x = 45056;
    work.rotation.y = 57344;
    work.rotation.z = 0;
    work.mode = 0;
    work.delay = delay;
}
void MakiShowPickupLog(shen_name character, float delay) {
    StepObjectDisplayWork &work = QueueMakiAction<StepObjectDisplayWork>(EVT_AT_ObjectDisp_Callback);
    work.cleanup = 1;
    work.character = character;
    work.mode = 0;
    work.delay = delay;
}
void MakiScheduleLocalBase(shen_name character, std::uint32_t delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MakiSetLocalBaseCallback, 6, 0x0b, sizeof(MakiLocalBaseWork), EVENT_CHARACTER_TASK);
    HLib::SetupTimedTask(task, delay);
    MakiLocalBaseWork &work = *static_cast<MakiLocalBaseWork *>(HLib::GetTaskParameter(task));
    work.cleanup = 1;
    work.character = character;
    work.base = MAKI_CLING;
}
void MakiSetPickupCamera(std::int32_t camera, shen_name character, const Vector3 &position) {
    EventSetCamera cameraWork{.cleanup = 0, .camera = camera, .eye = character, .target = character, .context = 0, .angle = 0, .delay = 0};
    EVT_AT_SetCam_Callback(&cameraWork);
    camCtrlDisableFilmBars();
    EventSetPosition positionWork{.cleanup = 0, .character = MAKI_RYO, .parent = character, .position = position, .delay = 0};
    EVT_AT_SetPosRot(&positionWork);
}
void MakiQueuePickupFade(float delay) {
    EventFilmControl &work = QueueMakiAction<EventFilmControl>(EVT_AT_MenCtrl_Callback);
    work.cleanup = 1;
    work.frames = 30;
    work.startAlpha = 0;
    work.startRed = 0;
    work.startGreen = 0;
    work.startBlue = 0;
    work.targetAlpha = 255;
    work.targetRed = 0;
    work.targetGreen = 0;
    work.targetBlue = 0;
    work.delay = delay;
}
} // namespace

void MakiAttachLighter(std::int32_t delay) {
    const Vector3 position{.x = .12800001f, .y = .046f, .z = -.024f};
    if (delay) {
        EventCling &work = QueueMakiAction<EventCling>(EVT_AT_Cling);
        work.cleanup = 1;
        work.target = MAKI_LIGHTER;
        work.character = MAKI_RYO;
        work.bone = 14;
        work.position = position;
        work.rotation.x = 61440;
        work.rotation.y = 14927;
        work.rotation.z = 57344;
        work.mode = 0;
        work.delay = static_cast<float>(delay);
    } else {
        EventCling work{.cleanup = 0,
            .target = MAKI_LIGHTER,
            .character = MAKI_RYO,
            .bone = 14,
            .position = position,
            .rotation = {.x = 61440, .y = 14927, .z = 57344},
            .mode = 0,
            .delay = 0};
        EVT_AT_Cling(&work);
    }
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MAKI_RYO), 0, reinterpret_cast<std::byte *>(makiLighterHandCommand), 0);
}

std::int32_t MakiPickSingleLog(shen_name character) {
    MakiSetPickupCamera(1000, character, {.x = 0, .y = 0, .z = -.7f});
    EVT_AT_SetMot(MAKI_RYO, 0x4002, 40, 1, 0x44000000U, 0);
    return 35;
}

std::int32_t MakiPickOneLog(shen_name character) {
    MakiQueuePickupFade(80);
    MakiSetPickupCamera(1001, character, {.x = 0, .y = 0, .z = -1.2f});
    const std::uint32_t motion = makiRuntime.heldCount - 1 <= 2 ? 0x400c : 0x4007;
    EVT_AT_SetMot(MAKI_RYO, motion, 40, 1, 0x44000000U, 0);
    EVT_AT_SetMot(MAKI_RYO, motion, 320, 1, 0x40000000U, 110);
    EVT_AT_SetSound(1426, 80);
    MakiDetachLog(MAKI_LIGHTER, 40);
    MakiAttachLighter(90);
    AT_SetLocalBaseChara(MAKI_SMALL_LOG, EVENT_CHARACTER_INVALID);
    MakiScheduleLocalBase(MAKI_SMALL_LOG, 110);
    MakiAttachPickupLog(MAKI_SMALL_LOG, .1f, 0, 85);
    MakiShowPickupLog(MAKI_SMALL_LOG, 85);
    MakiDetachLog(MAKI_SMALL_LOG, 110);
    return 110;
}

std::int32_t MakiPickTwoLogs(shen_name character) {
    MakiQueuePickupFade(150);
    MakiSetPickupCamera(1002, character, {.x = 0, .y = 0, .z = -1.2f});
    const std::uint32_t motion = makiRuntime.heldCount - 5 <= 2 ? 0x400c : 0x4007;
    EVT_AT_SetMot(MAKI_RYO, motion, 40, 1, 0x44000000U, 0);
    EVT_AT_SetMot(MAKI_RYO, motion, 320, 1, 0x40000000U, 210);
    MakiDetachLog(MAKI_LIGHTER, 40);
    MakiAttachLighter(190);
    EVT_AT_SetSound(1426, 80);
    EVT_AT_SetSound(1426, 160);
    MakiAttachPickupLog(MAKI_SMALL_LOG, .07f, -.02f, 85);
    MakiAttachPickupLog(MAKI_MIDDLE_LOG, .07f, -.02f, 155);
    MakiShowPickupLog(MAKI_SMALL_LOG, 85);
    MakiShowPickupLog(MAKI_MIDDLE_LOG, 155);
    AT_SetLocalBaseChara(MAKI_SMALL_LOG, EVENT_CHARACTER_INVALID);
    AT_SetLocalBaseChara(MAKI_MIDDLE_LOG, EVENT_CHARACTER_INVALID);
    MakiScheduleLocalBase(MAKI_SMALL_LOG, 125);
    MakiScheduleLocalBase(MAKI_MIDDLE_LOG, 195);
    MakiDetachLog(MAKI_SMALL_LOG, 125);
    MakiDetachLog(MAKI_MIDDLE_LOG, 195);
    return 210;
}

std::int32_t MakiPickThreeLogs(shen_name character) {
    MakiQueuePickupFade(270);
    MakiSetPickupCamera(1003, character, {.x = 0, .y = 0, .z = -1.2f});
    const std::uint32_t motion = makiRuntime.heldCount - 10 <= 2 ? 0x400c : 0x4007;
    EVT_AT_SetMot(MAKI_RYO, motion, 0, 1, 0x40000000U, 0);
    EVT_AT_SetMot(MAKI_RYO, motion, 320, 1, 0x40000000U, 330);
    MakiDetachLog(MAKI_LIGHTER, 80);
    MakiAttachLighter(330);
    EVT_AT_SetSound(1426, 130);
    EVT_AT_SetSound(1426, 190);
    EVT_AT_SetSound(1426, 270);
    MakiAttachPickupLog(MAKI_SMALL_LOG, .07f, -.02f, 125);
    MakiAttachPickupLog(MAKI_MIDDLE_LOG, .07f, -.02f, 195);
    MakiAttachPickupLog(MAKI_LARGE_LOG, .07f, -.02f, 275);
    MakiShowPickupLog(MAKI_SMALL_LOG, 125);
    MakiShowPickupLog(MAKI_MIDDLE_LOG, 195);
    MakiShowPickupLog(MAKI_LARGE_LOG, 275);
    AT_SetLocalBaseChara(MAKI_SMALL_LOG, EVENT_CHARACTER_INVALID);
    AT_SetLocalBaseChara(MAKI_MIDDLE_LOG, EVENT_CHARACTER_INVALID);
    AT_SetLocalBaseChara(MAKI_LARGE_LOG, EVENT_CHARACTER_INVALID);
    MakiScheduleLocalBase(MAKI_SMALL_LOG, 165);
    MakiScheduleLocalBase(MAKI_MIDDLE_LOG, 235);
    MakiScheduleLocalBase(MAKI_LARGE_LOG, 300);
    MakiDetachLog(MAKI_SMALL_LOG, 165);
    MakiDetachLog(MAKI_MIDDLE_LOG, 235);
    MakiDetachLog(MAKI_LARGE_LOG, 300);
    return 330;
}

void MakiRefreshHeldLogs() {
    for (shen_name *character = makiRuntime.heldCharacters; character < makiRuntime.heldCharacters + 16; ++character)
        EV_DisableCharacterDisplay(*character);
    if (makiRuntime.heldCount <= 1) {
        std::byte *command;
        if (makiRuntime.heldCount == 1) {
            EV_EnableCharacterDisplay(makiRuntime.heldCharacters[0]);
            EventCling work{.cleanup = 0,
                .target = makiRuntime.heldCharacters[0],
                .character = MAKI_RYO,
                .bone = 21,
                .position = {.x = .1f, .y = 0, .z = .02f},
                .rotation = {.x = 15746, .y = 40960, .z = 29400},
                .mode = 0,
                .delay = 0};
            EVT_AT_Cling(&work);
            command = reinterpret_cast<std::byte *>(makiSingleLogHandCommand);
        } else
            command = reinterpret_cast<std::byte *>(crateGripHandPose);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MAKI_RYO), 1, command, 0);
    } else {
        mtwkRequestMotion(scenFindMotionwork(MAKI_RYO), 0x4004, MotionRegion::LeftArm);
        for (std::int32_t i = 0; i < makiRuntime.heldCount; ++i) {
            const Vector3 &angles = makiHeldLogAngles[i];
            EV_EnableCharacterDisplay(makiRuntime.heldCharacters[i]);
            EventCling work{.cleanup = 0,
                .target = makiRuntime.heldCharacters[i],
                .character = MAKI_RYO,
                .bone = 21,
                .position = makiHeldLogPositions[i],
                .rotation = {.x = static_cast<std::int32_t>(angles.x * 8192.0f / 45.0f),
                    .y = static_cast<std::int32_t>(angles.y * 8192.0f / 45.0f),
                    .z = static_cast<std::int32_t>(angles.z * 8192.0f / 45.0f)},
                .mode = 0,
                .delay = 0};
            EVT_AT_Cling(&work);
        }
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(MAKI_RYO), 1, reinterpret_cast<std::byte *>(makiCarryHandCommand), 0);
    }
}
} // namespace shenmue::s2::areas
