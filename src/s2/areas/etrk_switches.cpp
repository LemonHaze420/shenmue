#include "s2/areas/etrk_event.hpp"
#include "s2/areas/area_elevator_lamp.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/door.hpp"
#include "s2/elevator.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/math.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include "s2/map_control.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
// @verified - p
void EtrkPartCallback(void *data) {
    EtrkPartWork &work = *static_cast<EtrkPartWork *>(data);
    EV_scenApplyPosOrRotToPartOf(
        work.character, static_cast<std::uint16_t>(work.part), &work.position, &work.rotation, EventCharacterPartTransformMode::Absolute);
    HLib::DestroyCurrentTask();
}
void SetEtrkPart(shen_name character, std::int32_t part, Vector3 position, std::int32_t delay) {
    Vector3i rotation{};
    if (!delay) {
        EV_scenApplyPosOrRotToPartOf(character, static_cast<std::uint16_t>(part), &position, &rotation, EventCharacterPartTransformMode::Absolute);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EtrkPartCallback, 4, 0x0b, sizeof(EtrkPartWork), ETRK_TASK);
    HLib::SetupTimedTask(task, delay);
    EtrkPartWork *const work = static_cast<EtrkPartWork *>(HLib::GetTaskParameter(task));
    work->character = character;
    work->part = part;
    work->position = position;
    work->rotation = rotation;
}
// @verified - p
void EtrkLightCallback(void *data) {
    const EtrkLightWork &work = *static_cast<EtrkLightWork *>(data);
    if (!work.operation) {
        if (work.enabled)
            PushGlobalLightingProgram();
        else
            EV_SetModelDataDefaultVertexShader();
    } else if (work.operation == 1)
        ConfigureAreaElevatorLamp(work.enabled);
    else if (work.operation == 2)
        ExecuteCleanupFuncForAreaWithCachedScene();
    HLib::DestroyCurrentTask();
}
void QueueEtrkLight(std::int32_t operation, std::int32_t enabled, std::int32_t delay) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EtrkLightCallback, 4, 0x0b, sizeof(EtrkLightWork), ETRK_TASK);
    HLib::SetupTimedTask(task, delay);
    EtrkLightWork *const work = static_cast<EtrkLightWork *>(HLib::GetTaskParameter(task));
    work->operation = operation;
    work->enabled = enabled;
}
} // namespace
// @verified - p
void EtrkSetParts(EtrkWork &work, std::int32_t delay) {
    bool allOff = true;
    for (std::int32_t index = 0; index < 3; ++index) {
        const EtrkSwitch &row = work.switches[index];
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(row.flag))) {
            SetEtrkPart(work.board, row.onPart, row.pressedPosition, delay);
            SetEtrkPart(work.board, row.offPart, Vector3{}, delay);
            allOff = false;
        } else {
            SetEtrkPart(work.board, row.onPart, Vector3{}, delay);
            SetEtrkPart(work.board, row.offPart, row.position, delay);
        }
    }
    const EtrkSwitch &row = work.switches[3];
    SetEtrkPart(work.board, row.onPart, allOff ? row.pressedPosition : Vector3{}, delay);
    SetEtrkPart(work.board, row.offPart, allOff ? Vector3{} : row.pressedPosition, delay);
}
// @verified - p
void EtrkPrepareSwitches(EtrkWork &work) {
    EV_DisableCharacterDisplay(ETRK_RYO);
    EV_EnableCharacterDisplay(ETRK_HAND);
    if (work.selection == 4)
        work.selection = 0;
    work.disabledSelection = 3;
    for (std::int32_t index = 0; index < 3; ++index)
        if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, static_cast<std::int16_t>(work.switches[index].flag))) {
            work.disabledSelection = index;
            break;
        }
    AT_SetLocalBaseChara(ETRK_HAND, work.board);
    EV_AT_SetPos(ETRK_HAND, EventTransformFlags::AllAxes, work.switches[work.selection].position);
    EV_AT_SetAngY(ETRK_HAND, EventTransformFlags::AllAxes, work.switches[work.selection].rotation, nullptr);
    ApplyCharacterHandRotation(FindCharacterByScenarioName(ETRK_HAND), 0, 21, &etrkSelectorHandRotation);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_HAND), 0, reinterpret_cast<std::byte *>(etrkRelaxedHand), 0);
    work.moving = 0;
    work.remaining = 1800;
    EVT_AT_SetCam(1200, ETRK_HAND, ETRK_HAND, 0, 0, 0);
    camCtrlDisableFilmBars();
    EV_EventSetUpHelpIcon_0(0x57, 0);
}
// @verified - p
std::int32_t EtrkPollSwitches(EtrkWork &work) {
    const std::int32_t remaining = work.remaining--;
    if (remaining >= 1) {
        CheckHelpIcon(33, 0, CheckHelpIconOp::FrameOverlay);
        GetBattleDataConfig(0, ETRK_ACCEPT);
        GetBattleDataConfig(0, ETRK_CANCEL);
        if (work.moving >= 1) {
            Vector3 position;
            EV_GetCharaPosition(ETRK_HAND, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            position.x += (work.switches[work.selection].position.x - position.x) / static_cast<float>(work.moving);
            EV_AT_SetPos(ETRK_HAND, EventTransformFlags::AllAxes, position);
            --work.moving;
            return 0;
        }
        if (GetBattleDataConfig(2, ETRK_ACCEPT) && work.selection != work.disabledSelection)
            return 1;
        if (!GetBattleDataConfig(2, ETRK_CANCEL)) {
            if ((GetCurrentControlInput(2) & 0x8000U) && work.selection < 3) {
                work.moving = 10;
                ++work.selection;
            }
            if ((GetCurrentControlInput(2) & 0x4000U) && work.selection > 0) {
                work.moving = 10;
                --work.selection;
            }
            return 0;
        }
    }
    EVT_AT_SetSound(3063, 30);
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(60);
    return -1;
}
// @verified - p
void EtrkPressSwitch(EtrkWork &work) {
    EV_DisableCharacterDisplay(ETRK_RYO);
    EV_EnableCharacterDisplay(ETRK_HAND);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(ETRK_HAND), 0, reinterpret_cast<std::byte *>(etrkPressHand), 10);
    for (std::int32_t index = 0; index < 10; ++index) {
        EVT_AT_SetPos(ETRK_HAND, ETRK_HAND, Vector3{0, 0, -.003f}, index);
        EVT_AT_SetAngY(ETRK_HAND, ETRK_HAND, 0, index);
    }
    for (std::int32_t index = 0; index < 3; ++index)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(work.switches[index].flag), 0);
    if (work.selection < 3)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(work.switches[work.selection].flag), 1);
    EtrkSetParts(work, 10);
    EVT_AT_SetSound(2630, 10);
    EVT_AT_ObjectDisp_1(ETRK_RYO, 20, 0);
    EVT_AT_ObjectDisp_1(ETRK_HAND, 20, 1);
    const shen_name elevator = work.switches[work.selection].elevator;
    std::int32_t delay = 20;
    if (elevator.name) {
        Vector3 position;
        std::uint32_t yaw;
        GetElevatorTransform(elevator, position, yaw);
        if (g_currentMap.name == ETRK_QUG0.name)
            position.y = 96.400002f;
        EVT_AT_SetPos(ETRK_CAMERA, ETRK_INVALID, position, 20);
        EVT_AT_SetAngY(ETRK_CAMERA, ETRK_INVALID, static_cast<float>(yaw) / 182.04445f, 20);
        EVT_AT_SetCam(1420, ETRK_CAMERA, ETRK_CAMERA, 0, 0, 20);
        EVT_AT_SetSound(0xb40, 20);
        QueueEtrkLight(0, 0, 20);
        QueueEtrkLight(1, 1, 20);
        QueueEtrkLight(1, 0, 21);
        QueueEtrkLight(1, 1, 26);
        QueueEtrkLight(1, 0, 27);
        QueueEtrkLight(1, 1, 36);
        QueueEtrkLight(1, 0, 37);
        QueueEtrkLight(1, 1, 44);
        QueueEtrkLight(1, 0, 45);
        QueueEtrkLight(1, 1, 46);
        QueueEtrkLight(1, 0, 47);
        QueueEtrkLight(1, 1, 49);
        QueueEtrkLight(1, 0, 51);
        QueueEtrkLight(1, 1, 53);
        QueueEtrkLight(1, 0, 55);
        QueueEtrkLight(1, 1, 57);
        QueueEtrkLight(1, 0, 59);
        QueueEtrkLight(1, 1, 61);
        QueueEtrkLight(1, 0, 63);
        QueueEtrkLight(1, 1, 73);
        QueueEtrkLight(1, 0, 76);
        QueueEtrkLight(1, 1, 79);
        delay = 50;
    } else
        QueueEtrkLight(1, 1, 20);
    if (g_currentMap.name == ETRK_QF00.name || g_currentMap.name == ETRK_QUG0.name) {
        const bool qf = g_currentMap.name == ETRK_QF00.name;
        EVT_AT_SetCam(qf ? 1425 : 1426, ETRK_INVALID, ETRK_INVALID, 0, 0, delay);
        EVT_AT_SetVoice("0304A023", 1, delay); // @text EN: " " | JA: "んっ…"
        EVT_AT_SetPos(ETRK_RYO, work.board, Vector3{-.11f, -1.355f, .80500001f}, delay);
        EVT_AT_SetAngY(ETRK_RYO, work.board, qf ? 0.0f : 90.0f, delay);
        EVT_AT_SetMot(ETRK_RYO, ETRK_TURN_MOTION, 0, 1, ETRK_MOTION_FLAGS, delay);
    }
    delay += 30;
    QueueEtrkLight(2, 1, delay);
    if (elevator.name) {
        EVT_AT_SetPos(ETRK_CAMERA, ETRK_RYO, Vector3{}, delay);
        EVT_AT_SetAngY(ETRK_CAMERA, ETRK_RYO, 0, delay);
        EVT_AT_SetCam(1430, ETRK_CAMERA, ETRK_CAMERA, 0, 0, delay + 1);
        const std::int32_t choice = shenGetRandomInt32(4);
        EVT_AT_SetVoice(choice == 0 ? "0304A024" : choice == 1 ? "0304A036" : choice == 2 ? "0304A037" : "0304A038", 1, delay); // @text 0304A024 EN: "Good!\nIt's moving!" | JA: "よし！　動いた…" ; 0304A036 EN: "It's moving!" | JA: "動いたぞ。" ; 0304A037 EN: "Good=@" | JA: "よしっ…" ; 0304A038 EN: "It moved." | JA: "動いた。"
        delay += 60;
    }
    HLib::SetupTaskDelay(delay);
}
} // namespace shenmue::s2::areas
