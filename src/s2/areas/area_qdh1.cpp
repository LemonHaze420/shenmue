#include "s2/areas/area_qdh.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/door.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <bit>
#include <cstring>

namespace shenmue::s2 {
// @verified - p
bool WaitQdhAuthor(QdhEventWork *work) {
    if (work->entry != 1 && work->entry != 2)
        return false;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return false;
    if (work->entry == 2)
        EV_FreeAuthorPak(0, "0303");
    mtwkSetExtraFlag(scenFindMotionwork(QDH_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(QDH_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(QDH_REN), 6, 0);
    EV_SetCharacterAlphaEnabled(QDH_REN, true);
    mtwkSetExtraFlag(scenFindMotionwork(QDH_TGY), 6, 0);
    EV_SetCharacterAlphaEnabled(QDH_TGY, true);
    EV_SetCharacterAlphaEnabled(QDH_BACKGROUND, true);
    return true;
}
namespace {
std::int32_t qdh1VoiceFade;
HLib::Task *qdh1BridgeTask;
BridgeBendingDefinition qdh1BridgeDefinition{.sectionWidth = BRIDGE_SECTION_WIDTH,
    .character = BRIDGE_DUMMY,
    .model = BRIDGE_MODEL,
    .attachedCharacter = {},
    .partCount = 20,
    .reserved14 = 0,
    .parts = BRIDGE_SECTION_PARTS};
void Qdh1AdmissionNullsub() {
}
void Qdh1BridgeCallback(void *data) {
    BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(data);
    if (!work) {
        HLib::DestroyCurrentTask();
        return;
    }
    if (work->phaseStep)
        work->phase += static_cast<std::int32_t>(static_cast<float>(work->phaseStep) * work->tickFactor);
    BendQdh1Bridge(work);
    UpdateBridgeAttachedCharacter(work);
}
void QueueQdh1Bridge(std::int32_t delay) {
    if (qdh1BridgeTask)
        return;
    qdh1BridgeTask = HLib::EnqueueTaskWithParameter(Qdh1BridgeCallback, 6, 0x0b, sizeof(BridgeBendingWork), BRIDGE_BENDING_TASK);
    if (!qdh1BridgeTask)
        return;
    BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
    if (!work) {
        HLib::DestroyTaskInternal(qdh1BridgeTask);
        qdh1BridgeTask = nullptr;
        return;
    }
    std::memset(work, 0, sizeof(*work));
    work->phaseStep = 1820;
    work->definition = &qdh1BridgeDefinition;
    work->amplitude = 910;
    work->scale = .5f;
    work->pairCount = qdh1BridgeDefinition.partCount / 2;
    work->range = static_cast<float>(qdh1BridgeDefinition.partCount + 1) * qdh1BridgeDefinition.sectionWidth;
    work->tickFactor = 1;
    EV_GetCharaPosition(qdh1BridgeDefinition.character, CHARACTER_TRANSLATION_ROOT, &work->position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(work->definition->character, &work->rotation, EventTransformFlags::World);
    if (qdh1BridgeTask) {
        BridgeBendingWork *const attached = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (attached)
            attached->attachedCharacter = qdh1BridgeDefinition.attachedCharacter;
    }
    ResetBridgeSections(work);
    if (delay)
        HLib::SetupTimedTask(qdh1BridgeTask, delay);
}
void Qdh1AttachBridgeCallback(void *data) {
    const QdhBridgeAttachment *const request = static_cast<const QdhBridgeAttachment *>(data);
    if (qdh1BridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (work)
            work->attachedCharacter = request->character;
    }
    if (request->delay)
        HLib::DestroyCurrentTask();
}
void QueueQdh1BridgeAttachment(shen_name character, std::int32_t delay) {
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qdh1AttachBridgeCallback, 6, 0x0b, sizeof(QdhBridgeAttachment), BRIDGE_BENDING_TASK);
        if (task) {
            QdhBridgeAttachment *const request = static_cast<QdhBridgeAttachment *>(HLib::GetTaskParameter(task));
            if (request) {
                request->character = character;
                request->delay = delay;
                HLib::SetupTimedTask(task, delay);
            }
        }
    } else if (qdh1BridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (work)
            work->attachedCharacter = character;
    }
}
void Qdh1EndBridgeCallback(void *data) {
    const std::int32_t *const cleanup = static_cast<const std::int32_t *>(data);
    if (qdh1BridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (work) {
            ResetBridgeSections(work);
            EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
            EV_AT_SetAngY(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->rotation, nullptr);
            HLib::DestroyTaskInternal(qdh1BridgeTask);
            qdh1BridgeTask = nullptr;
        }
    }
    if (*cleanup)
        HLib::DestroyCurrentTask();
}
void QueueQdh1BridgeEnd(std::int32_t delay) {
    if (delay) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qdh1EndBridgeCallback, 6, 0x0b, sizeof(std::int32_t), BRIDGE_BENDING_TASK);
        if (task) {
            std::int32_t *const cleanup = static_cast<std::int32_t *>(HLib::GetTaskParameter(task));
            if (cleanup) {
                *cleanup = 1;
                HLib::SetupTimedTask(task, delay);
            }
        }
    } else if (qdh1BridgeTask) {
        BridgeBendingWork *const work = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (work) {
            ResetBridgeSections(work);
            EV_AT_SetPos(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->position);
            EV_AT_SetAngY(work->definition->character, EventTransformFlags::World | EventTransformFlags::AllAxes, work->rotation, nullptr);
            HLib::DestroyTaskInternal(qdh1BridgeTask);
            qdh1BridgeTask = nullptr;
        }
    }
}
void Qdh1VoiceFadeCallback(void *) {
    hsFadeVoice(nullptr, qdh1VoiceFade);
    HLib::DestroyCurrentTask();
}
void StartQdh1Author(QdhEventWork *work) {
    EVT_AT_ObjectDisp_1(QDH_RYO, 0, 0);
    EVT_AT_ObjectDisp_1(QDH_REN, 0, 0);
    EVT_AT_ObjectDisp_1(QDH_TGY, 0, 0);
    EVT_AT_ObjectDisp_1(work->definition->character, 0, 1);
    EVT_AT_ObjectDisp_1(QDH_BACKGROUND, 0, 0);
    mtwkSetExtraFlag(scenFindMotionwork(QDH_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(QDH_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(QDH_REN), 6, 2);
    EV_SetCharacterAlphaEnabled(QDH_REN, false);
    mtwkSetExtraFlag(scenFindMotionwork(QDH_TGY), 6, 2);
    EV_SetCharacterAlphaEnabled(QDH_TGY, false);
    EV_SetCharacterAlphaEnabled(QDH_BACKGROUND, false);
    if (work->entry == 1) {
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 0});
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        if (work->authorMode == 2)
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDH_AUTHOR_KYA2.name));
        else if (work->authorMode == 0 || work->authorMode == 1) {
            if (work->failureLevel > 0)
                EV_SetAuthorActiveTo(1);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->authorMode ? QDH_AUTHOR_KYA1.name : QDH_AUTHOR_KYA0.name));
        }
    } else if (work->entry == 2) {
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        EV_FreeAuthorPak(1, "0303");
        if (work->authorMode == 2)
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDH_AUTHOR_NOA1.name));
        else if (!work->authorMode) {
            if (work->failureLevel > 0)
                EV_SetAuthorActiveTo(1);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(QDH_AUTHOR_NOA0.name));
        }
    }
}
std::int32_t StartQdh1RenCrossing(QdhEventWork *work) {
    EVT_AT_ObjectDisp_1(QDH_RYO, 0, 0);
    EVT_AT_ObjectDisp_1(QDH_REN, 0, 0);
    EVT_AT_ObjectDisp_1(QDH_TGY, 0, 1);
    EVT_AT_ObjectDisp_1(work->definition->character, 0, 0);
    EVT_AT_ObjectDisp_1(QDH_BACKGROUND, 0, 1);
    AT_SetLocalBaseChara(QDH_CAMERA, QDH_REN);
    EVT_AT_SetPos(QDH_CAMERA, QDH_REN, {}, 0);
    EVT_AT_SetAngY(QDH_CAMERA, QDH_REN, 0, 0);
    if (work->entry == 1) {
        EVT_AT_SetPos(QDH_RYO, BRIDGE_DUMMY, {6.8f, 0, 1}, 0);
        EVT_AT_SetAngY(QDH_RYO, BRIDGE_DUMMY, 90, 0);
    } else {
        EVT_AT_SetPos(QDH_RYO, BRIDGE_DUMMY, {-6.3f, 0, -1}, 0);
        EVT_AT_SetAngY(QDH_RYO, BRIDGE_DUMMY, 270, 0);
    }
    EVT_AT_SetMot(QDH_RYO, QDH_MOTION_IDLE, 0, 1, 0x40000000U, 0);
    EVT_AT_SetPos(QDH_REN, BRIDGE_DUMMY, {1.5f, 0, 0}, 0);
    EVT_AT_SetAngY(QDH_REN, BRIDGE_DUMMY, 90, 0);
    EVT_AT_SetMot(QDH_REN, QDH_MOTION_WALK, 0, 1, 0x40000000U, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(QDH_REN), 3);
    QueueQdh1Bridge(1);
    QueueQdh1BridgeAttachment(QDH_REN, 1);
    if (work->entry == 1) {
        EVT_AT_SetCam(1600, BRIDGE_DUMMY, BRIDGE_DUMMY, 0, 0, 0);
        EVT_AT_SetCam(1610, BRIDGE_DUMMY, BRIDGE_DUMMY, 0, 0, 90);
        EVT_AT_SetCam(1620, BRIDGE_DUMMY, BRIDGE_DUMMY, 0, 0, 150);
    } else {
        EVT_AT_SetCam(1500, BRIDGE_DUMMY, BRIDGE_DUMMY, 0, 0, 0);
        EVT_AT_SetCam(1510, BRIDGE_DUMMY, BRIDGE_DUMMY, 0, 0, 105);
    }
    EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 0});
    std::int32_t time = 45;
    for (std::int32_t index = 0; index < 2; ++index) {
        EVT_AT_SetMot(QDH_REN, QDH_MOTION_WALK, 0, 1, 0, time);
        time += 45;
        if (work->entry == 1 && !index) {
            EVT_AT_SetMot(QDH_REN, QDH_MOTION_LEAN_RIGHT, 0, 1, 0, time);
            time += 32;
            EVT_AT_SetMot(QDH_REN, QDH_MOTION_RECOVER_RIGHT, 0, 1, 0, time);
            time += 70;
        }
    }
    EVT_AT_SetMot(QDH_REN, QDH_MOTION_WALK, 0, 1, 0, time);
    EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = static_cast<float>(time)});
    QueueQdh1BridgeEnd(time + 30);
    return time + 30;
}
void InitializeQdh1Event(HLib::Task *task, std::int32_t entry) {
    QdhEventWork *const work = static_cast<QdhEventWork *>(HLib::GetTaskParameter(task));
    std::memset(work, 0, sizeof(*work));
    work->task = task;
    work->actions = QDH_ACTIONS;
    work->entry = entry;
    work->state = 12;
    AT_SetLocalBaseChara(QDH_RYO, QDH_STAGES[entry][0].character);
    Vector3 position;
    EV_GetCharaPosition(QDH_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    AT_SetLocalBaseChara(QDH_RYO, EVENT_CHARACTER_INVALID);
    work->definition = &QDH_STAGES[entry][position.x <= 0 ? 1 : 0];
    work->qteTime = work->definition->qteTime + 4 * work->failureLevel;
    if (entry == 1) {
        EV_SetCharacterScale(work->definition->character, {1.3f, 1, 1.5f});
        work->speed = 1.25f;
        if (DoesScenarioCharacterExist(QDH_BRIDGE_1))
            EVT_AT_ObjectDisp_1(QDH_BRIDGE_1, 0, 1);
    } else {
        EV_SetCharacterScale(work->definition->character, {1, 1, 1.5f});
        work->speed = 1;
        if (DoesScenarioCharacterExist(QDH_BRIDGE_2))
            EVT_AT_ObjectDisp_1(QDH_BRIDGE_2, 0, 1);
    }
    EV_SetRefBoardParamsFor(work->definition->character, 1);
    EV_SetRefBoardParamsFor(QDH_BACKGROUND, 1);
    SetScenarioCharacterRadius(work->definition->character, 20);
    SetScenarioCharacterRadius(QDH_BACKGROUND, 20);
    EVT_AT_SetPos(BRIDGE_MODEL, work->definition->character, {}, 0);
    EVT_AT_SetAngY(BRIDGE_MODEL, work->definition->character, 0, 0);
    EVT_AT_SetPos(BRIDGE_DUMMY, work->definition->character, {}, 0);
    EVT_AT_SetAngY(BRIDGE_DUMMY, work->definition->character, 0, 0);
    if (work->definition->reversed)
        EVT_AT_SetAngY(BRIDGE_DUMMY, BRIDGE_DUMMY, 180, 0);
    AT_SetLocalBaseChara(QDH_CAMERA, QDH_RYO);
    EVT_AT_SetPos(QDH_CAMERA, QDH_RYO, {}, 0);
    EVT_AT_SetAngY(QDH_CAMERA, QDH_RYO, 0, 0);
    qdh1BridgeDefinition.character = work->definition->character;
    EV_SetCharaControllerFlags(QDH_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(QDH_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit2, nullptr);
    EV_SetRefBoardParamsFor(QDH_RYO, 1);
    EventSetMotion motion{.cleanup = 0, .character = QDH_RYO, .animation = QDH_MOTION_IDLE, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(QDH_MOTION_IDLE);
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0289A000.smp");
    EV_LoadAudio("0289A000", 0);
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(1, "0289");
    Queue_EVT_UTL_LoadNPCChara_3(QDH_CHARACTERS, work->task);
}
void Qdh1EventCallback(void *data) {
    using enum CameraState;
    QdhEventWork *const work = static_cast<QdhEventWork *>(data);
    switch (work->state) {
    case 0:
        Qdh1AdmissionNullsub();
        work->waitCount = StartQdh1RenCrossing(work);
        work->state = 14;
        break;
    case 1: {
        Qdh1AdmissionNullsub();
        work->qteTime = work->definition->qteTime + 4 * work->failureLevel;
        EVT_AT_ObjectDisp_1(QDH_RYO, 0, 0);
        EVT_AT_ObjectDisp_1(QDH_REN, 0, 0);
        EVT_AT_ObjectDisp_1(QDH_TGY, 0, 1);
        EVT_AT_ObjectDisp_1(work->definition->character, 0, 0);
        EVT_AT_ObjectDisp_1(QDH_BACKGROUND, 0, 1);
        AT_SetLocalBaseChara(QDH_CAMERA, QDH_RYO);
        EVT_AT_SetPos(QDH_CAMERA, QDH_RYO, {}, 0);
        EVT_AT_SetAngY(QDH_CAMERA, QDH_RYO, 0, 0);
        if (work->entry == 1) {
            EVT_AT_SetPos(QDH_RYO, BRIDGE_DUMMY, {5.5f, 0, 0}, 0);
            EVT_AT_SetAngY(QDH_RYO, BRIDGE_DUMMY, 90, 0);
            EVT_AT_SetPos(QDH_REN, BRIDGE_DUMMY, {-6, 0, -1.2f}, 0);
            EVT_AT_SetAngY(QDH_REN, BRIDGE_DUMMY, 270, 0);
            EVT_AT_SetMot(QDH_REN, QDH_MOTION_CALL, 0, 1, 0x01000000U, 0);
            mtwkSetRequestedRootMode(scenFindMotionwork(QDH_REN), 1);
        } else {
            EVT_AT_SetPos(QDH_RYO, BRIDGE_DUMMY, {4.4f, 0, 0}, 0);
            EVT_AT_SetAngY(QDH_RYO, BRIDGE_DUMMY, 90, 0);
            EVT_AT_SetPos(QDH_REN, BRIDGE_DUMMY, {5.5f, 0, 1}, 0);
            EVT_AT_SetAngY(QDH_REN, BRIDGE_DUMMY, 90, 0);
            EVT_AT_SetMot(QDH_REN, QDH_MOTION_IDLE, 0, 1, 0x40000000U, 0);
        }
        EVT_AT_SetMot(QDH_RYO, QDH_MOTION_START, 0, work->speed, 0x40000000U, 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(QDH_RYO), 3);
        QueueQdh1Bridge(0);
        QueueQdh1BridgeAttachment(QDH_RYO, 0);
        EVT_AT_SetCam(work->definition->camera, QDH_CAMERA, QDH_CAMERA, 0, 0, 0);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 255, .targetAlpha = 0, .delay = 0});
        work->action = static_cast<std::uint32_t>(work->definition->steps[2 * work->sequence + work->step]);
        std::int32_t delay = static_cast<std::int32_t>(40.0f / work->speed);
        if (work->action > 3) {
            work->state = 4;
            delay = static_cast<std::int32_t>(110.0f / work->speed);
        } else {
            EVT_AT_QTE(static_cast<std::uint16_t>(work->actions[work->action].qte), static_cast<std::int16_t>(work->qteTime), 0, delay);
            work->state = 2;
        }
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 2:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            work->state = work->actions[work->action].qte == static_cast<std::uint32_t>(EV_GetCurrentQteStatus() & 0xfff) ? 3 : 8;
        break;
    case 3:
        if (GetMotionFrame(scenFindMotionwork(QDH_RYO), MotionRegion::LowerBody) >= 110.0f / work->speed)
            work->state = 4;
        break;
    case 4:
        if (++work->step >= 2) {
            ++work->sequence;
            work->step = 0;
        }
        work->action = static_cast<std::uint32_t>(work->definition->steps[2 * work->sequence + work->step]);
        if (work->action == 5)
            work->state = 9;
        else {
            std::int32_t delay = 0;
            if (!work->step) {
                EVT_AT_SetMot(QDH_RYO, QDH_MOTION_WALK, 0, work->speed, 0, 0);
                delay = static_cast<std::int32_t>(45.0f / work->speed);
            }
            work->state = 5;
            if (delay)
                HLib::SetupTaskDelay(delay);
        }
        break;
    case 5:
        if (work->action > 3)
            work->state = 4;
        else {
            AT_SetLocalBaseChara(QDH_CAMERA, EVENT_CHARACTER_INVALID);
            EVT_AT_SetCam(work->actions[work->action].camera + shenGetRandomInt32(2), QDH_CAMERA, QDH_CAMERA, 0, 0, 0);
            EVT_AT_QTE(static_cast<std::uint16_t>(work->actions[work->action].qte), static_cast<std::int16_t>(work->qteTime), 0, 0);
            EV_GetCharaPosition(QDH_RYO, CHARACTER_TRANSLATION_ROOT, &work->previousPosition, EventTransformFlags::World);
            EVT_AT_SetMot(QDH_RYO, work->actions[work->action].motion, 0, work->speed, 0, 0);
            EVT_AT_SetVoice(QDH_LEAN_VOICES[shenGetRandomInt32(4)], 0);
            work->state = 6;
        }
        break;
    case 6:
        if (!GetShouldRenderIcons(RenderIconSlot::Qte))
            work->state = work->actions[work->action].qte == static_cast<std::uint32_t>(EV_GetCurrentQteStatus() & 0xfff) ? 7 : 8;
        break;
    case 7: {
        AT_SetLocalBaseChara(QDH_CAMERA, QDH_RYO);
        EVT_AT_SetPos(QDH_CAMERA, QDH_RYO, {}, 0);
        EVT_AT_SetAngY(QDH_CAMERA, QDH_RYO, 0, 0);
        EVT_AT_SetCam(work->definition->camera, QDH_CAMERA, QDH_CAMERA, 0, 0, 0);
        EVT_AT_SetVoice(QDH_RECOVER_VOICES[shenGetRandomInt32(4)], 0);
        EV_AT_SetPos(QDH_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, work->previousPosition);
        EVT_AT_SetMot(QDH_RYO, work->actions[work->action].recoverMotion, 0, work->speed, 0x40000000U, 0);
        work->state = 4;
        const std::int32_t delay = static_cast<std::int32_t>(70.0f / work->speed);
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 8: {
        if (!work->sequence && !work->step)
            work->action = 1;
        SetScenarioCharacterDrawControl(QDH_RYO, 0, 0);
        BridgeBendingWork *const bridge = static_cast<BridgeBendingWork *>(HLib::GetTaskParameter(qdh1BridgeTask));
        if (bridge)
            bridge->attachedHeight = .1f;
        EVT_AT_SetCam(work->actions[work->action].fallCamera, QDH_CAMERA, QDH_CAMERA, 0, 0, 0);
        EVT_AT_SetMot(QDH_RYO, work->actions[work->action].fallMotion, 0, 1, 0x40000000U, 0);
        EVT_AT_SetVoice("0289A016", 0);
        EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 40});
        HLib::Task *const fade = HLib::EnqueueTaskWithoutParameter(Qdh1VoiceFadeCallback, 4, 0x0b, BRIDGE_BENDING_TASK.name);
        qdh1VoiceFade = 60;
        HLib::SetupTimedTask(fade, 70);
        QueueQdh1BridgeEnd(130);
        work->state = 11;
        HLib::SetupTaskDelay(130);
        break;
    }
    case 9: {
        EVT_AT_SetMot(QDH_RYO, QDH_MOTION_EXIT, 0, work->speed, 0x20000000U, 0);
        std::int32_t delay;
        if (work->entry == 1) {
            delay = 30;
            EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 0});
            QueueQdh1BridgeEnd(30);
        } else {
            EVT_AT_MenCtrl({.cleanup = 1, .frames = 30, .startAlpha = 0, .targetAlpha = 255, .delay = 40});
            delay = 70;
            QueueQdh1BridgeEnd(70);
            EVT_AT_SetMot(QDH_RYO, QDH_MOTION_IDLE, 0, 1, 0, 70);
        }
        if (work->entry == 1) {
            work->authorMode = 2;
            work->state = 12;
        } else
            work->state = 0;
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 10:
        if (DoesScenarioCharacterExist(QDH_BRIDGE_1))
            EVT_AT_ObjectDisp_1(QDH_BRIDGE_1, 0, 0);
        if (DoesScenarioCharacterExist(QDH_BRIDGE_2))
            EVT_AT_ObjectDisp_1(QDH_BRIDGE_2, 0, 0);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
        ExecuteCleanupFuncForAreaWithCachedScene();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        if (work->entry == 1) {
            EVT_AT_SetPos(QDH_RYO, EVENT_CHARACTER_INVALID, {250.7700043f, 146.8000031f, 167.1600037f}, 0);
            EVT_AT_SetAngY(QDH_RYO, EVENT_CHARACTER_INVALID, 270, 0);
        } else {
            EVT_AT_SetPos(QDH_RYO, EVENT_CHARACTER_INVALID, {229.0200043f, 143.1999969f, 156.0299988f}, 0);
            EVT_AT_SetAngY(QDH_RYO, EVENT_CHARACTER_INVALID, 45, 0);
        }
        EV_SetCharaControllerFlags(QDH_RYO, ApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(QDH_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit2, reinterpret_cast<void *>(1));
        SetPlayerControlEnabled(1);
        FinishLoading();
        for (const shen_name *character = QDH_CHARACTERS; character->name != 0xffffffffU; ++character)
            if (DoesScenarioCharacterExist(*character)) {
                EV_npcLockUnlock(*character);
                EV_npcFree(*character);
            }
        EV_NPC_NewLoadSw(1);
        if (work->entry == 1) {
            EV_AT_SetPos(QDH_BRIDGE_1, EventTransformFlags::World | EventTransformFlags::AllAxes, {236.1000061f, 136, 148.2f});
            EV_AT_SetAngY(QDH_BRIDGE_1, EventTransformFlags::World | EventTransformFlags::AllAxes, {0, 7281, 0}, nullptr);
        }
        EV_FreeAuthorPak(0, "0289");
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDH1"), 0);
        scenFreeEvent("QDH1.EV3");
        EV_CleanupMapControlTask();
        HLib::MarkCurrentTaskForCleanup();
        break;
    case 11:
        SetScenarioCharacterDrawControl(QDH_RYO, 1, 0);
        if (work->failureLevel < 2)
            ++work->failureLevel;
        work->authorMode = 0;
        work->state = 12;
        work->sequence = 0;
        work->step = 0;
        break;
    case 12:
        StartQdh1Author(work);
        work->state = 13;
        break;
    case 13:
        if (!WaitQdhAuthor(work))
            break;
        if ((!GetAuthorFlags() || work->authorMode) && work->authorMode) {
            if (work->authorMode == 1)
                work->state = 1;
            else if (work->authorMode == 2)
                work->state = 10;
        } else
            work->state = work->entry != 1;
        break;
    case 14:
        if (work->waitCount)
            --work->waitCount;
        else {
            EV_CleanupMapControlTask();
            work->state = 12;
            work->authorMode = work->entry == 1 ? 1 : 2;
        }
        break;
    default:
        break;
    }
}
} // namespace
void Qdh1Init() {
    qdh1VoiceFade = 0;
    qdh1BridgeTask = nullptr;
}
void Qdh1Main(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    Qdh1AdmissionNullsub();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QDH1"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDH1"), 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Qdh1EventCallback, 4, 0x0b, sizeof(QdhEventWork), BRIDGE_BENDING_TASK);
    InitializeQdh1Event(task, std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(argument)));
}
const ScriptAreaDefinition qdh1ScriptDefinition{.areaId = AREA_QDH1, .field04 = 0, .init = Qdh1Init, .main = Qdh1Main};
} // namespace shenmue::s2
