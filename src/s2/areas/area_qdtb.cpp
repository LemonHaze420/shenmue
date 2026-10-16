#include "s2/areas/area_qdtb.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_image.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstring>

namespace shenmue::s2 {
namespace {
std::uint8_t qdtbRetry{};
void BoutSequenceCallback(void *);
void FirstBoutCallback(void *);
void TogyuBoutCallback(void *);
void BoutAuthorCallback(void *);
void BoutAuthorLoopCallback(void *);
void QdtbAdmissionNullsub() {
}

void BeginBoutAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(QDTB_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(QDTB_RYO, false);
    for (shen_name character : QDTB_ALPHA_MODELS)
        EV_SetCharacterAlphaEnabled(character, false);
    EV_SetCharacterAlphaEnabled(QDTB_TENT, false);
    LoadBlurModelForCharacter(QDTB_RYO, "RYO_B.MT7", 1);
    LoadBlurModelForCharacter(QDTB_SYE, "SYE_B.MT7", 1);
    LoadBlurModelForCharacter(QDTB_TGY, "TGY_B.MT7", 1);
    for (std::size_t i = 0; QDTB_OPPONENTS[i].name != EVENT_CHARACTER_INVALID.name; ++i) {
        mtwkSetExtraFlag(scenFindMotionwork(QDTB_OPPONENTS[i]), 6, 2);
        EV_SetCharacterAlphaEnabled(QDTB_OPPONENTS[i], false);
    }
}

void EndBoutAuthorCharacters() {
    mtwkSetExtraFlag(scenFindMotionwork(QDTB_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(QDTB_RYO, true);
    for (shen_name character : QDTB_ALPHA_MODELS)
        EV_SetCharacterAlphaEnabled(character, true);
    for (std::size_t i = 0; QDTB_OPPONENTS[i].name != EVENT_CHARACTER_INVALID.name; ++i) {
        mtwkSetExtraFlag(scenFindMotionwork(QDTB_OPPONENTS[i]), 6, 0);
        EV_SetCharacterAlphaEnabled(QDTB_OPPONENTS[i], true);
    }
}

void QueueBoutAuthorLoop(QdtbAuthorWork *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(BoutAuthorLoopCallback, 4, 0x0b, sizeof(QdtbAuthorLoopWork), QDTB_TASK);
    QdtbAuthorLoopWork *const work = static_cast<QdtbAuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->frame = 0;
    work->parent = parent;
    EV_LoadAudio("0249A000", 0);
    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
    EVT_AT_MenCtrl_Callback(&film);
    EV_debug_log("QDTB\\eQDTB_et_0249.c", 159, "ID : [%c%c%c%c]\n", static_cast<int>(parent->author.name & 255U),
        static_cast<int>((parent->author.name >> 8) & 255U), static_cast<int>((parent->author.name >> 16) & 255U), static_cast<int>(parent->author.name >> 24));
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.name));
    parent->state = 2;
}

void BoutAuthorCallback(void *data) {
    QdtbAuthorWork *const work = static_cast<QdtbAuthorWork *>(data);
    if (work->state == 1)
        QueueBoutAuthorLoop(work);
    else if (work->state == 3)
        HLib::DestroyTask(work->task);
}

void BoutAuthorLoopCallback(void *data) {
    QdtbAuthorLoopWork *const work = static_cast<QdtbAuthorLoopWork *>(data);
    work->frame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    const shen_name author = work->parent->author;
    if (work->frame == 1) {
        BeginBoutAuthorCharacters();
        if (author.name == QDTB_AUTHORS[1].name) {
            EV_DisableCharacterDisplay(QDTB_YOH);
            EV_DisableCharacterDisplay(QDTB_RKN);
            EV_DisableCharacterDisplay(QDTB_III);
            EV_DisableCharacterDisplay(QDTB_SEE);
            EV_DisableCharacterDisplay(QDTB_SYE);
            EV_DisableCharacterDisplay(QDTB_A235);
        } else if (author.name == QDTB_AUTHORS[2].name) {
            EV_DisableCharacterDisplay(QDTB_YOH);
            EV_DisableCharacterDisplay(QDTB_RKN);
            EV_DisableCharacterDisplay(QDTB_III);
            EV_DisableCharacterDisplay(QDTB_SEE);
        }
        if (author.name == QDTB_AUTHORS[1].name) {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), QDTB_TASK);
            RegisterEventTask(task);
            EventPlayDynamics *const dynamic = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
            dynamic->resource = "TJMP";
            dynamic->parts = QDTB_TENT_PARTS;
            dynamic->character = QDTB_A401;
            dynamic->endFrame = 135;
            dynamic->trackCount = 42;
            dynamic->frameStep = 2;
            dynamic->currentFrame = 2;
            dynamic->delay = 175;
        } else if (author.name == QDTB_AUTHORS[2].name) {
            EV_DisableCharacterDisplay(QDTB_A352);
            EV_DisableCharacterDisplay(QDTB_TENT);
        }
    } else if (work->frame == 22 && author.name == QDTB_AUTHORS[0].name) {
        AssignImageModuleParameters(ctrlGetCharaByName(QDTB_TENT), 1, 1, 1, 0);
    } else if (work->frame == 171 && author.name == QDTB_AUTHORS[1].name) {
        EV_EnableCharacterDisplay(QDTB_A235);
        EV_DisableCharacterDisplay(QDTB_TENT);
        EV_DisableCharacterDisplay(QDTB_A352);
    } else if (work->frame == 470 && author.name == QDTB_AUTHORS[0].name) {
        EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 0, .targetAlpha = 255};
        EVT_AT_MenCtrl_Callback(&film);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        EndBoutAuthorCharacters();
        work->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

void InitializeBoutResources(QdtbBoutWork *work) {
    if (IsFightActive())
        check_dead_status(1);
    const char *const path = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(path), reinterpret_cast<std::uintptr_t>("QDTB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("QDTB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDTB"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, QDTB_BASE);
    EV_FreeAuthorPak(1, "0249");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0249A000");
    EV_LoadAudio("0249A000", 0);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    DateTime time;
    GetCurrentDateTime(time);
    if (static_cast<std::uint32_t>(time.hour - 19) > 5U) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_lightLoad(shen_name{});
        PushGlobalLightingProgram();
    }
    FinishLoading();
    SetRenderClockAndCompassTo(1);
    work->unused24 = 0;
}

void EndBout(QdtbBoutWork *work) {
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(QDTB_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(QDTB_RYO), 71, 0, MotionRegion::LowerBody);
    for (std::uint32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0249");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("QDTB"), 0);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    HLib::DestroyCurrentTask();
    DateTime time;
    GetCurrentDateTime(time);
    time.day = static_cast<std::uint8_t>(time.day + 3);
    time.hour = 8;
    time.minute = 0;
    time.second = 0;
    SetCurrentDateTimeTo(time);
    EV_scenQueueLoad(g_actNumber, QDTB_RETURN, 0);
}

void BoutMainCallback(void *data) {
    QdtbBoutWork *const work = static_cast<QdtbBoutWork *>(data);
    switch (work->state) {
    case 0:
        work->state = 3;
        break;
    case 3:
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("QDTB"), 0);
        LoadBlurModelForCharacter(QDTB_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(QDTB_SYE, "SYE_B.MT7", 1);
        LoadBlurModelForCharacter(QDTB_TGY, "TGY_B.MT7", 1);
        QdtbAdmissionNullsub();
        work->sequence = QueueCompiledSequence(BoutSequenceCallback, QDTB_SEQUENCE, work);
        work->sequenceActive = 1;
        work->state = 4;
        break;
    case 4:
        if (!work->sequenceActive)
            work->state = 5;
        break;
    case 5:
        work->state = 6;
        break;
    case 6:
        FinishLoading();
        work->state = 7;
        break;
    case 7:
        EV_stepSetFlags(0, 0x11e);
        EV_stepSetFlags(0, 0x11f);
        EndBout(work);
        break;
    default:
        break;
    }
}

void BoutSequenceCallback(void *data) {
    CompiledSequenceWork *const sequence = static_cast<CompiledSequenceWork *>(data);
    QdtbBoutWork *const work = static_cast<QdtbBoutWork *>(sequence->event);
    sequence->callbackFlag = 0;
    do {
        const std::uint16_t mode = sequence->mode;
        sequence->continueLoop = 0;
        switch (mode) {
        case 0:
            sequence->continueAfterWait = 1;
            sequence->previousMode = mode;
            sequence->mode = 100;
            sequence->continueLoop = 1;
            break;
        case 100:
        case 103:
        case 105: {
            HLib::Task *const task = HLib::EnqueueTaskWithParameter(BoutAuthorCallback, 4, 0x0b, sizeof(QdtbAuthorWork), QDTB_TASK);
            QdtbAuthorWork *const author = static_cast<QdtbAuthorWork *>(HLib::GetTaskParameter(task));
            author->task = task;
            author->state = 1;
            author->author = QDTB_AUTHORS[mode == 100 ? 0 : mode == 103 ? 1 : 2];
            SetPlayerControlEnabled(0);
            sequence->mode = 13;
            sequence->nextMode = mode == 100 ? 101 : mode == 103 ? 104 : 107;
            sequence->delay = 5;
            sequence->continueAfterWait = 0;
            break;
        }
        case 101:
        case 104: {
            if (mode == 104) {
                SetScenarioCharacterDrawControl(QDTB_TGY, 1, 1);
                EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
                EV_SetCharaControllerFlags(QDTB_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(QDTB_TGY, UpdateHeight, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(QDTB_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(QDTB_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(QDTB_TGY, ApplyGravity, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(QDTB_TGY, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
                for (std::size_t i = 0; i < 6; ++i)
                    EV_AT_SetPos(QDTB_POSE_CHARACTERS[i], EventTransformFlags::World | EventTransformFlags::AllAxes, QDTB_POSITIONS[i]);
                for (std::size_t i = 0; i < 6; ++i)
                    EV_AT_SetAngY(QDTB_POSE_CHARACTERS[i], EventTransformFlags::AllAxes, QDTB_ROTATIONS[i]);
                for (std::size_t i = 0; i < 6; ++i)
                    EVT_AT_SetMot(QDTB_STANCE_CHARACTERS[i], QDTB_STANCES[i], 0, 1, 0x40000000U, 0);
            }
            CompiledSequenceWork *const child = QueueCompiledSequence(mode == 101 ? FirstBoutCallback : TogyuBoutCallback, QDTB_BATTLE, work);
            if (!sequence->children[0])
                sequence->children[0] = child;
            child->parent = sequence;
            child->childSlot = 0;
            child->reference = sequence->reference + 1;
            sequence->timer = 0;
            std::uint32_t zero = 0;
            std::memcpy(sequence->parameters, &zero, sizeof(zero));
            sequence->mode = 10;
            sequence->nextMode = mode == 101 ? 102 : 105;
            sequence->waitChildIndex = 0;
            sequence->continueAfterWait = 1;
            break;
        }
        case 102:
            if (qdtbRetry) {
                ++work->retryCount;
                sequence->mode = 10;
                sequence->nextMode = 0;
            } else {
                sequence->mode = 10;
                sequence->nextMode = 103;
            }
            sequence->waitChildIndex = 0;
            sequence->continueAfterWait = 1;
            break;
        case 106:
            mtwkSetRequestedRootMode(scenFindMotionwork(QDTB_RYO), 0);
            mtwkSetRequestedRootMode(scenFindMotionwork(QDTB_TGY), 0);
            camCtrlSetFovTransitionTo(true);
            break;
        case 107:
            sequence->continueAfterWait = 1;
            sequence->previousMode = mode;
            sequence->mode = 110;
            sequence->continueLoop = 1;
            break;
        case 108:
            dbgPrintEventMessage("チャイの不意打ちオーサ\n");
            sequence->mode = 13;
            sequence->nextMode = 109;
            sequence->delay = 5;
            sequence->continueAfterWait = 0;
            break;
        case 109:
            sequence->mode = 2;
            sequence->nextMode = 110;
            sequence->delay = 122;
            sequence->continueAfterWait = 1;
            break;
        case 110:
            sequence->mode = 2;
            sequence->nextMode = 111;
            sequence->delay = 1;
            sequence->continueAfterWait = 1;
            break;
        case 111:
            work->sequenceActive = 0;
            RetireCompiledSequenceLinks(sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(sequence);
            break;
        }
    } while (sequence->continueLoop);
}

void FirstBoutCallback(void *data) {
    CompiledSequenceWork *const sequence = static_cast<CompiledSequenceWork *>(data);
    QdtbBoutWork *const work = static_cast<QdtbBoutWork *>(sequence->event);
    sequence->callbackFlag = 0;
    do {
        const std::uint16_t mode = sequence->mode;
        sequence->continueLoop = 0;
        switch (mode) {
        case 0: {
            EventSetCamera camera{.cleanup = 0, .camera = 100, .eye = QDTB_RYO, .target = QDTB_RYO, .context = 0, .angle = 0, .delay = 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
            EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
            EVT_AT_MenCtrl_Callback(&film);
            for (std::size_t i = 0; i < 6; ++i)
                EV_EnableCharacterDisplay(QDTB_CHARACTERS[i]);
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QDTB"), 1);
            shenInitFightData(ctrlGetCharaByName(QDTB_RYO), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (work->retryCount + 2)), 0);
            shenInitFightData(ctrlGetCharaByName(QDTB_YOH), nullptr, 1, 1, 100, 2);
            shenInitFightData(ctrlGetCharaByName(QDTB_III), nullptr, 1, 1, 100, 3);
            shenInitFightData(ctrlGetCharaByName(QDTB_SEE), nullptr, 1, 1, 80, 4);
            shenInitFightData(ctrlGetCharaByName(QDTB_RKN), nullptr, 1, 1, 80, 5);
            vfCopyPosFromEntity(0, nullptr, 175.60001f, 57.900002f, 5.0f);
            sequence->mode = 2;
            sequence->nextMode = 100;
            sequence->continueAfterWait = 1;
            sequence->delay = 30;
            SetScenarioCharacterDrawControl(QDTB_TGY, 0, 1);
            break;
        }
        case 12:
            if (!IsFightActive()) {
                sequence->previousMode = mode;
                sequence->mode = 104;
                sequence->continueAfterWait = 1;
                sequence->continueLoop = 1;
            }
            break;
        case 100:
            work->countdown = 30;
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
            vfmQueueFightStart(5, 2, 1, 1, -1);
            sequence->previousMode = mode;
            sequence->mode = 101;
            sequence->continueAfterWait = 1;
            sequence->continueLoop = 1;
            break;
        case 101:
            qdtbRetry = 1;
            if (!IsFightActive()) {
                sequence->previousMode = mode;
                sequence->mode = 12;
                sequence->continueAfterWait = 1;
                sequence->continueLoop = 1;
            } else if (vfmDispatchControl(FightControlOp::BattleCounter, shen_name{}, 0, -1) >= 3 && GetBtlStatus(ctrlGetCharaByName(QDTB_RYO)) == 1) {
                qdtbRetry = 0;
                EV_vfBattleUtil(FightBattleUtilOp::MoveFlag40000, QDTB_RYO, 1);
                if (!work->fadeStarted) {
                    work->fadeStarted = 1;
                    EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 0, .targetAlpha = 255};
                    EVT_AT_MenCtrl_Callback(&film);
                }
                if (work->countdown-- == 1 && work->fadeStarted == 1) {
                    check_dead_status(1);
                    sequence->previousMode = mode;
                    sequence->mode = 12;
                    sequence->continueAfterWait = 1;
                    sequence->continueLoop = 1;
                }
            }
            break;
        case 104:
            if (qdtbRetry == 1) {
                EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 0, .targetAlpha = 255};
                EVT_AT_MenCtrl_Callback(&film);
            }
            sequence->mode = 11;
            sequence->nextMode = 105;
            sequence->continueAfterWait = 1;
            sequence->delay = 2;
            break;
        case 105:
            figpAssignModeFor(QDTB_RYO, 0);
            figpAssignModeFor(QDTB_TGY, 0);
            RetireCompiledSequenceLinks(sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(sequence);
            break;
        }
    } while (sequence->continueLoop);
}

void TogyuBoutCallback(void *data) {
    CompiledSequenceWork *const sequence = static_cast<CompiledSequenceWork *>(data);
    QdtbBoutWork *const work = static_cast<QdtbBoutWork *>(sequence->event);
    sequence->callbackFlag = 0;
    do {
        const std::uint16_t mode = sequence->mode;
        sequence->continueLoop = 0;
        switch (mode) {
        case 0: {
            EventSetCamera camera{.cleanup = 0, .camera = 101, .eye = QDTB_RYO, .target = QDTB_RYO, .context = 0, .angle = 0, .delay = 0};
            EVT_AT_SetCam_Callback(&camera);
            camCtrlDisableFilmBars();
            EventFilmControl film{.cleanup = 0, .frames = 30, .startAlpha = 255, .targetAlpha = 0};
            EVT_AT_MenCtrl_Callback(&film);
            EV_EnableCharacterDisplay(QDTB_RYO);
            EV_EnableCharacterDisplay(QDTB_TGY);
            EV_EnableCharacterDisplay(QDTB_YOH);
            EV_EnableCharacterDisplay(QDTB_III);
            EV_EnableCharacterDisplay(QDTB_SEE);
            EV_DisableCharacterDisplay(QDTB_RKN);
            EV_ToggleCharacterFlag(QDTB_RKN);
            EV_DisableCharacterDisplay(QDTB_SYE);
            EV_ToggleCharacterFlag(QDTB_SYE);
            EV_EnableCharacterDisplay(QDTB_A235);
            EV_DisableCharacterDisplay(QDTB_A352);
            EV_DisableCharacterDisplay(QDTB_A401);
            EV_ToggleCharacterFlag(QDTB_A352);
            EV_ToggleCharacterFlag(QDTB_A401);
            SetScenarioCharacterRadius(QDTB_A235, 10);
            work->fadeStarted = 0;
            work->countdown = 360;
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("QDTB"), 1);
            shenInitFightData(ctrlGetCharaByName(QDTB_RYO), nullptr, 0, 0, 0, 0);
            shenInitFightData(ctrlGetCharaByName(QDTB_TGY), nullptr, 1, 1, 2000, 8);
            shenInitFightData(ctrlGetCharaByName(QDTB_YOH), nullptr, 1, 1, 100, 2);
            shenInitFightData(ctrlGetCharaByName(QDTB_III), nullptr, 1, 1, 100, 3);
            shenInitFightData(ctrlGetCharaByName(QDTB_SEE), nullptr, 1, 1, 80, 4);
            vfCopyPosFromEntity(0, nullptr, 175.60001f, 57.900002f, 5.0f);
            vfUpdateFlagsFor(ctrlGetCharaByName(QDTB_RYO), 0, 0x20000, 80);
            sequence->mode = 2;
            sequence->nextMode = 100;
            sequence->continueAfterWait = 1;
            sequence->delay = 30;
            break;
        }
        case 12:
            if (!IsFightActive()) {
                sequence->previousMode = mode;
                sequence->mode = 104;
                sequence->continueAfterWait = 1;
                sequence->continueLoop = 1;
            }
            break;
        case 100:
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(QDTB_RYO), FacialNodeOriginal);
            SetFaceFlagsForChara_0(ctrlGetCharaByName(QDTB_TGY), FacialNodeOriginal);
            vfmQueueFightStart(6, 2, 1, 1, -1);
            sequence->previousMode = mode;
            sequence->continueAfterWait = 1;
            sequence->mode = 101;
            sequence->continueLoop = 1;
            break;
        case 101:
            if (work->battleFrames++ <= 900)
                break;
            if (!work->fadeStarted) {
                vfmDispatchControl(FightControlOp::SpriteDepth, shen_name{}, 0, 1);
                EventFilmControl film{.cleanup = 0, .frames = 360, .startAlpha = 0, .targetAlpha = 185, .targetRed = 35, .targetGreen = 20};
                EVT_AT_MenCtrl_Callback(&film);
                QueueEventWeatherControl();
                work->fadeStarted = 1;
            }
            if (work->countdown-- == 1) {
                work->countdown = 210;
                sequence->previousMode = mode;
                sequence->continueAfterWait = 1;
                sequence->mode = 102;
                sequence->continueLoop = 1;
            }
            break;
        case 102:
            if (!work->closingFadeStarted) {
                EventFilmControl film{.cleanup = 0, .frames = 210, .startAlpha = 185, .startRed = 35, .startGreen = 20, .targetAlpha = 255};
                EVT_AT_MenCtrl_Callback(&film);
                work->closingFadeStarted = 1;
            }
            if (work->countdown-- == 1) {
                check_dead_status(1);
                EV_EnsureCharacterDraw(QDTB_SYE);
                SetWeatherUpdateEnabledTo(1);
                sequence->previousMode = mode;
                sequence->mode = 12;
                sequence->continueAfterWait = 1;
                sequence->continueLoop = 1;
            } else if (work->countdown == 30)
                spmFadeOut();
            break;
        case 104:
            if (!filmCheckFadeTrackColour()) {
                sequence->mode = 11;
                sequence->nextMode = 105;
                sequence->continueAfterWait = 1;
                sequence->delay = 2;
            }
            break;
        case 105:
            figpAssignModeFor(QDTB_RYO, 0);
            figpAssignModeFor(QDTB_TGY, 0);
            RetireCompiledSequenceLinks(sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(sequence);
            break;
        }
    } while (sequence->continueLoop);
}
} // namespace

void QdtbInit() {
    qdtbRetry = 0;
}
void QdtbMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    EventFilmControl film{.cleanup = 0, .frames = 1, .startAlpha = 0, .targetAlpha = 255};
    EVT_AT_MenCtrl_Callback(&film);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(BoutMainCallback, 4, 0x0b, sizeof(QdtbBoutWork), QDTB_TASK);
    QdtbBoutWork *const work = static_cast<QdtbBoutWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 0;
    work->battleFrames = 0;
    work->fadeStarted = work->closingFadeStarted = 0;
    work->retryCount = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, QDTB_CHARACTERS);
    InitializeBoutResources(work);
}
const ScriptAreaDefinition qdtbScriptDefinition{.areaId = AREA_QDTB, .field04 = 0, .init = QdtbInit, .main = QdtbMain};
} // namespace shenmue::s2
