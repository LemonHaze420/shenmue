#include "s2/areas/area_kew0.hpp"
#include "s2/advice.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/fight.hpp"
#include "s2/field_collision.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/fight_stage.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include <cstring>
namespace shenmue::s2 {
namespace {
void KeikaBattleCallback(void *);
void KeikaSequenceCallback(void *);
void KeikaAuthorCallback(void *);
void KeikaAuthorLoopCallback(void *);
void KeikaAdmissionNullsub() {
}
void AdvanceKeikaSequence(CompiledSequenceWork *work, std::uint16_t next) {
    work->previousMode = work->mode;
    work->mode = next;
    work->continueAfterWait = 1;
    work->continueLoop = 1;
}
void WaitKeikaSequence(CompiledSequenceWork *work, std::uint16_t mode, std::uint16_t next, std::int32_t delay, std::int32_t continueAfter) {
    work->mode = mode;
    work->nextMode = next;
    work->delay = delay;
    work->continueAfterWait = continueAfter;
}
void BeginKeikaAuthorCharacters() {
    for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_IKO, KEW0_MOK}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    EV_SetCharacterAlphaEnabled(KEW0_PROP, false);
}
void EndKeikaAuthorCharacters() {
    for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_IKO, KEW0_MOK})
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
}
void QueueKeikaAuthor(shen_name author) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KeikaAuthorCallback, 4, 0x0b, sizeof(Kew0AuthorWork), KEW0_TASK);
    Kew0AuthorWork *const work = static_cast<Kew0AuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    work->author = author;
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour < 19) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    } else {
        EV_lightLoad(KEW0_LIGHT);
        PushGlobalLightingProgram();
    }
}
void QueueKeikaAuthorLoop(Kew0AuthorWork *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KeikaAuthorLoopCallback, 4, 0x0b, sizeof(Kew0AuthorLoopWork), KEW0_TASK);
    Kew0AuthorLoopWork *const work = static_cast<Kew0AuthorLoopWork *>(HLib::GetTaskParameter(task));
    work->parent = parent;
    work->unused08 = 0;
    EV_LoadAudio("0067A000", 0);
    SetScreenFilmFade(parent->author.name == KEW0_INTRO.name ? 30 : 10, 255, 0, 0, 0, 0, 0, 0, 0);
    parent->savedPostFx = static_cast<std::int32_t>(GetPostFxBankValue(3));
    SelectPostFxBank(3, 0);
    BeginKeikaAuthorCharacters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author.name));
    parent->state = 2;
}
void KeikaAuthorCallback(void *data) {
    Kew0AuthorWork *const work = static_cast<Kew0AuthorWork *>(data);
    if (work->state == 1)
        QueueKeikaAuthorLoop(work);
    else if (work->state == 3) {
        DeleteMotion(static_cast<std::int32_t>(work->motion));
        HLib::DestroyTask(work->task);
    }
}
void KeikaAuthorLoopCallback(void *data) {
    Kew0AuthorLoopWork *const work = static_cast<Kew0AuthorLoopWork *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (!frame)
        BeginKeikaAuthorCharacters();
    if (work->parent->author.name == KEW0_LEARN.name) {
        if (frame == 103)
            ExecuteAicaCommand(0x1200a0, 0, 0);
        else if (frame == 105)
            QueueExtraTelop("EEEE", 0);
        else if (frame == 110)
            EV_LoadAudio("0067A000", 0);
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SelectPostFxBank(3, work->parent->savedPostFx);
        EndKeikaAuthorCharacters();
        work->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}
void FreezeKeikaCharactersForAuthor() {
    for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_MOK, KEW0_IKO}) {
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        UpdateMotionWorkWithPosition(scenFindMotionwork(name), nullptr, 0);
        mtwkSetRequestedRootMode(scenFindMotionwork(name), 0);
    }
}
void PrepareKeikaCharactersForFight() {
    for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_MOK, KEW0_IKO}) {
        EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(name, false);
        SetScenarioReflectorBoardValues(name, &KEW0_REFLECTOR);
        EV_SetRefBoardParamsFor(name, 1);
    }
}
void PrepareKeikaCharacter(shen_name name) {
    SetScenarioReflectorBoardValues(name, &KEW0_REFLECTOR);
    EV_SetRefBoardParamsFor(name, 1);
    mtwkSetRequestedRootMode(scenFindMotionwork(name), 1);
    SetLowQualityHands(ctrlGetCharaByName(name));
}
void KeikaSequenceCallback(void *data) {
    CompiledSequenceWork *const work = static_cast<CompiledSequenceWork *>(data);
    Kew0EventWork *const event = static_cast<Kew0EventWork *>(work->event);
    work->callbackFlag = 0;
    do {
        work->continueLoop = 0;
        switch (work->mode) {
        case 0:
            event->learnedResult = 0;
            if (!event->lcdSequence) {
                CompiledSequenceWork *const child = QueueCompiledSequence(CompiledLcdCountdownCallback, KEW0_LCD_SEQUENCE, event);
                if (!work->children[1])
                    work->children[1] = child;
                child->parent = work;
                child->childSlot = 1;
                child->reference = work->reference + 1;
                work->timer = 1;
                const std::uint32_t zero = 0;
                std::memcpy(work->parameters + 4, &zero, sizeof(zero));
                event->lcdSequence = child;
            }
            AdvanceKeikaSequence(work, event->entry == 99 ? 104 : 100);
            break;
        case 100:
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            FreezeKeikaCharactersForAuthor();
            QueueKeikaAuthor(KEW0_INTRO);
            WaitKeikaSequence(work, 13, 101, 5, 0);
            break;
        case 101:
            PrepareKeikaCharactersForFight();
            WaitKeikaSequence(work, 2, 104, 32, 1);
            break;
        case 104: {
            CompiledSequenceWork *const child = QueueCompiledSequence(KeikaBattleCallback, KEW0_BATTLE_SEQUENCE, event);
            if (!work->children[0])
                work->children[0] = child;
            child->parent = work;
            child->childSlot = 0;
            child->reference = work->reference + 1;
            work->timer = 0;
            const std::uint32_t zero = 0;
            std::memcpy(work->parameters, &zero, sizeof(zero));
            work->waitChildIndex = 0;
            work->mode = 10;
            work->nextMode = 105;
            work->continueAfterWait = 1;
            break;
        }
        case 105:
            AdvanceKeikaSequence(work, 106);
            break;
        case 106:
            if (event->fightResult == 1 || event->fightResult == 2) {
                FreezeKeikaCharactersForAuthor();
                QueueKeikaAuthor(event->fightResult == 1 ? KEW0_WIN : KEW0_LOSE);
                EV_stepSetFlags(event->fightResult, 178);
                event->learnedResult = 0;
            } else if (!event->fightResult) {
                FreezeKeikaCharactersForAuthor();
                EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
                QueueKeikaAuthor(KEW0_LEARN);
                EV_stepSetFlags(0, 178);
                event->learnedResult = 3;
            }
            WaitKeikaSequence(work, 13, 107, 5, 0);
            break;
        case 107:
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            PrepareKeikaCharactersForFight();
            ExecuteAicaCommand(0xaa0, 30, 0);
            WaitKeikaSequence(work, 2, 110, 2, 1);
            break;
        case 110:
            AdvanceKeikaSequence(work, 111);
            break;
        case 111:
            SetMoveProficiency(1, 3, 14, 12);
            if (event->learnedResult == -1) {
                AdvanceKeikaSequence(work, 113);
                break;
            }
            if (event->learnedResult == 3) {
                SetMoveProficiency(0, 3, 14, 12);
                SetNewEwzoTypeForMoveLearned(0, 3, 14);
            } else if (!event->learnedResult) {
                SetMoveProficiency(0, 3, 14, 1);
                SetNewEwzoTypeForMoveLearned(2, 3, 14);
            }
            event->learnedFrames = 150;
            AdvanceKeikaSequence(work, 112);
            break;
        case 112:
            event->learnedFrames = static_cast<std::int16_t>(static_cast<std::uint16_t>(event->learnedFrames) - 1U);
            if (static_cast<std::int16_t>(GetCurrentControlInput(INPUT_PRESSED)) != 512 && event->learnedFrames)
                break;
            SetNewEwzoTypeForMoveLearned(1, 0, 0);
            AdvanceKeikaSequence(work, 113);
            break;
        case 113:
            AdvanceKeikaSequence(work, 114);
            break;
        case 114:
            if (event->lcdSequence) {
                CompiledSequenceWork *const child = work->children[1];
                if (child && child->task) {
                    RetireCompiledSequenceLinks(child);
                    HLib::DestroyTaskInternal(child->task);
                }
                event->lcdSequence = nullptr;
                LcdmInit(0);
                CheckHelpIcon(2048, 2048, CheckHelpIconOp::Remove);
            }
            event->sequenceActive = 0;
            RetireCompiledSequenceLinks(work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(work);
            break;
        }
    } while (work->continueLoop);
}
void KeikaBattleCallback(void *data) {
    using enum CameraState;
    CompiledSequenceWork *const work = static_cast<CompiledSequenceWork *>(data);
    Kew0EventWork *const event = static_cast<Kew0EventWork *>(work->event);
    work->callbackFlag = 0;
    do {
        work->continueLoop = 0;
        switch (work->mode) {
        case 0:
            camCtrlSetState(Event);
            camCtrlSetNextPosition(KEW0_CAMERA_POSITION);
            camCtrlSetTargetPosition(KEW0_CAMERA_TARGET);
            EV_LoadAudio("0067A000", 1);
            flddSetPointFlagOnGrid(KEW0_LIGHT, 0);
            flddSetPointFlagOnGrid(AREA_KEW0, 1);
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_IKO, KEW0_MOK})
                EV_EnableCharacterDisplay(name);
            AdvanceKeikaSequence(work, 100);
            break;
        case 100: {
            const std::uint32_t zero = 0;
            std::memcpy(reinterpret_cast<std::byte *>(work) + 0x1d0, &zero, sizeof(zero));
            SetMoveProficiency(0, 3, 14, 12);
            EV_SetCharaControllerFlags(KEW0_RYO, 1, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(KEW0_MOK, 1, NpcCharacterControllerState::Enabled);
            for (shen_name name : {KEW0_RYO, KEW0_MOK, KEW0_KEK, KEW0_IKO}) {
                EV_SetCharaControllerFlags(name, 2, NpcCharacterControllerState::Enabled);
                EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Enabled);
            }
            EV_AT_SetPos(KEW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KEW0_RYO_POSITION);
            EV_AT_SetPos(KEW0_MOK, EventTransformFlags::World | EventTransformFlags::AllAxes, KEW0_MOK_POSITION);
            EV_AT_SetPos(KEW0_KEK, EventTransformFlags::World | EventTransformFlags::AllAxes, KEW0_KEK_POSITION);
            EV_AT_SetPos(KEW0_IKO, EventTransformFlags::World | EventTransformFlags::AllAxes, KEW0_IKO_POSITION);
            EV_AT_SetAngY(KEW0_RYO, EventTransformFlags::AllAxes, {0, 32768, 0});
            EV_AT_SetAngY(KEW0_MOK, EventTransformFlags::AllAxes, {0, kew0MokYaw, 0});
            EV_AT_SetAngY(KEW0_KEK, EventTransformFlags::AllAxes, {0, 32768, 0});
            EV_AT_SetAngY(KEW0_IKO, EventTransformFlags::AllAxes, {0, 58254, 0});
            mtwkRequestMotion(scenFindMotionwork(KEW0_RYO), KEW0_IDLE, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KEW0_MOK), KEW0_IDLE, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KEW0_KEK), KEW0_ANGRY_B, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KEW0_IKO), KEW0_IKO_MOTION, MotionRegion::LowerBody);
            SetFixoDataForMotionWork(ctrlGetCharaByName(KEW0_PROP), ctrlGetCharaByName(KEW0_IKO), 14, kew0PropAttachmentPosition,
                {KEW0_PROP_ROTATION.x, KEW0_PROP_ROTATION.y, KEW0_PROP_ROTATION.z});
            SetMotionRegionFlag(scenFindMotionwork(KEW0_IKO), 66, 0, MotionRegion::LowerBody);
            mtwkSetExtraFlag(scenFindMotionwork(KEW0_KEK), 6, 2);
            WaitKeikaSequence(work, 2, 101, 2, 1);
            break;
        }
        case 101: {
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("KEW0"), 1);
            shenInitFightData(ctrlGetCharaByName(KEW0_RYO), nullptr, 0, 0, 0, 0);
            shenInitFightData(ctrlGetCharaByName(KEW0_MOK), nullptr, 1, 1, 800, 22);
            vfUpdateFlagsFor(ctrlGetCharaByName(KEW0_RYO), 0, 1024, 78);
            vfCopyPosFromEntity(0, nullptr, 74.400002f, 294.32999f, 3.8f);
            EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{256}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{1}, 1);
            EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
            vfmQueueFightStart(2, 99, 1, 0, -1);
            const std::uint64_t timers = 90;
            std::memcpy(&event->dialogueFrames, &timers, sizeof(timers));
            event->dialogueIndex = 0;
            WaitKeikaSequence(work, 2, 102, 5, 1);
            break;
        }
        case 102:
            if (GetFightActorFlags(ctrlGetCharaByName(KEW0_RYO)) & 0x400U) {
                event->fightResult = 0;
                event->lcdBitmap = reinterpret_cast<const std::byte *>(KEW0_LCD_BITMAP);
                event->lcdFrames = 600;
                LcdmSelectMessage(1);
                LcdmInit(reinterpret_cast<std::uintptr_t>(event->lcdBitmap));
                if (event->entry) {
                    check_dead_status(1);
                    AdvanceKeikaSequence(work, 107);
                } else {
                    event->battleFrames = 35;
                    event->battlePhase = 0;
                    AdvanceKeikaSequence(work, 105);
                }
                break;
            }
            if (GetBtlStatus(ctrlGetCharaByName(KEW0_RYO)) == 3) {
                event->fightResult = 1;
                check_dead_status(1);
                AdvanceKeikaSequence(work, 107);
                break;
            }
            if (GetBtlStatus(ctrlGetCharaByName(KEW0_RYO)) == 2) {
                event->fightResult = 2;
                check_dead_status(1);
                AdvanceKeikaSequence(work, 107);
                break;
            }
            if (event->dialogueFrames-- == 1) {
                const char *voice = nullptr;
                std::uint16_t motion = 0;
                switch (event->dialogueIndex) {
                case 0:
                    voice = "0067B020";
                    motion = KEW0_ANGRY_C;
                    break;
                case 1:
                    voice = "0067B021";
                    motion = KEW0_ANGRY_A;
                    break;
                case 2:
                    voice = "0067B022";
                    motion = KEW0_ANGRY_B;
                    break;
                case 3:
                    voice = "0067B023";
                    motion = KEW0_ANGRY_C;
                    break;
                case 4:
                    voice = "0067B024";
                    motion = KEW0_ANGRY_A;
                    break;
                case 5:
                    voice = "0067B025";
                    motion = KEW0_ANGRY_B;
                    break;
                case 6:
                    voice = "0067B026";
                    motion = KEW0_ANGRY_C;
                    break;
                default:
                    break;
                }
                if (voice) {
                    EV_PlayDialogue(voice);
                    if (motion == KEW0_ANGRY_A)
                        UpdateMotionWorkWithPosition(scenFindMotionwork(KEW0_KEK), nullptr, 0);
                    mtwkRequestMotion(scenFindMotionwork(KEW0_KEK), motion, MotionRegion::LowerBody);
                }
                if (++event->dialogueIndex == 6 && !event->adviceShown) {
                    event->adviceShown = 1;
                    QueueFightAdviceControl(14);
                }
                if (event->dialogueIndex > 6)
                    event->dialogueIndex = 0;
                event->dialogueFrames = shenGetRandomInt32(150) + 120;
            }
            if (GetMotionAnimationId(scenFindMotionwork(KEW0_KEK), MotionRegion::LowerBody) == KEW0_ANGRY_A) {
                if (SetMotionRegionFlag(scenFindMotionwork(KEW0_KEK), 65, 1, MotionRegion::LowerBody)) {
                    mtwkRequestMotion(scenFindMotionwork(KEW0_KEK), KEW0_ANGRY_C, MotionRegion::LowerBody);
                    SetMotionLayerFrameRange(scenFindMotionwork(KEW0_KEK), 60, 60, -1, MotionRegion::LowerBody);
                    event->targetFrames = 21;
                } else
                    event->targetFrames = 0;
            }
            if (event->targetFrames > 20) {
                Vector3 position;
                if (GetMotionAnimationId(scenFindMotionwork(KEW0_KEK), MotionRegion::LowerBody) != KEW0_ANGRY_A) {
                    EV_GetCharaPosition(KEW0_RYO, 5, &position, EventTransformFlags::World);
                    UpdateMotionWorkWithPosition(scenFindMotionwork(KEW0_KEK), &position, 0);
                }
                EV_GetCharaPosition(KEW0_MOK, 5, &position, EventTransformFlags::World);
                UpdateMotionWorkWithPosition(scenFindMotionwork(KEW0_IKO), &position, 0);
                event->targetFrames = 0;
            }
            ++event->targetFrames;
            break;
        case 105:
            if (!event->battlePhase && event->battleFrames-- == 1) {
                EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
                vfmDispatchControl(FightControlOp::QueuedAction, KEW0_RYO, 0x08000e6fU, 1);
                SetMotionSpeed(scenFindMotionwork(KEW0_KEK), .25f, MotionRegion::LowerBody);
                SetMotionSpeed(scenFindMotionwork(KEW0_IKO), .25f, MotionRegion::LowerBody);
                ++event->battlePhase;
                event->battleFrames = 90;
            }
            if (event->battleFrames-- == 1) {
                check_dead_status(1);
                AdvanceKeikaSequence(work, 107);
            } else if (event->battleFrames == 10)
                SetScreenFilmFade(10, 0, 0, 0, 0, 255, 0, 0, 0);
            break;
        case 106:
            if (event->battleFrames-- == 1) {
                check_dead_status(1);
                AdvanceKeikaSequence(work, 107);
            }
            break;
        case 107:
            if (!IsFightActive()) {
                EV_EventSetUpHelpIcon_0(0, 0);
                AdvanceKeikaSequence(work, 108);
            }
            break;
        case 108:
            ResetCharaFixo(ctrlGetCharaByName(KEW0_PROP));
            AdvanceKeikaSequence(work, 109);
            break;
        case 109:
            scenCvsCtrlFree(0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KEW0_KEK), nullptr, 0);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KEW0_IKO), nullptr, 0);
            figpAssignModeFor(KEW0_RYO, 0);
            figpAssignModeFor(KEW0_MOK, 0);
            RetireCompiledSequenceLinks(work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(work);
            break;
        }
    } while (work->continueLoop);
}
void CleanupKeikaEvent(Kew0EventWork *work) {
    using enum CameraState;
    flddSetPointFlagOnGrid(KEW0_LIGHT, 1);
    flddSetPointFlagOnGrid(AREA_KEW0, 0);
    ET_AT_KillWork(work->characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(KEW0_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(KEW0_RYO), 71, 0, MotionRegion::LowerBody);
    mtwkSetRequestedRootMode(scenFindMotionwork(KEW0_RYO), 1);
    for (const std::uint32_t *motion = work->motions; *motion; ++motion)
        DeleteMotion(static_cast<std::int32_t>(*motion));
    EV_FreeAuthorPak(0, "0067");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KEW0"), 0);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
}
void KeikaMainCallback(void *data) {
    Kew0EventWork *const work = static_cast<Kew0EventWork *>(data);
    switch (work->state) {
    case 0:
        SetPlayerControlEnabled(0);
        work->state = 1;
        break;
    case 1:
        for (shen_name name : {KEW0_RYO, KEW0_KEK, KEW0_MOK, KEW0_IKO})
            PrepareKeikaCharacter(name);
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KEW0"), 0);
        LoadBlurModelForCharacter(KEW0_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(KEW0_KEK, "KEK_B.MT7", 1);
        LoadBlurModelForCharacter(KEW0_MOK, nullptr, 1);
        KeikaAdmissionNullsub();
        EV_EventSetUpHelpIcon_0(20, 0);
        work->sequence = QueueCompiledSequence(KeikaSequenceCallback, KEW0_SEQUENCE, work);
        work->sequenceActive = 1;
        work->state = 2;
        break;
    case 2:
        if (!work->sequenceActive)
            work->state = 3;
        break;
    case 3:
        work->state = 4;
        break;
    case 4:
        FinishLoading();
        work->state = 5;
        break;
    case 5:
        CleanupKeikaEvent(work);
        break;
    default:
        break;
    }
}
void InitializeKeikaEvent(Kew0EventWork *work) {
    if (IsFightActive())
        check_dead_status(1);
    work->motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KEW0"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KEW0"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, KEW0_BASE);
    EV_FreeAuthorPak(1, "0067");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0067A000");
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    UpdateWeather();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
} // namespace
void Kew0Init() {
}
void Kew0Main(std::uintptr_t argument) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(KeikaMainCallback, 4, 0x0b, sizeof(Kew0EventWork), KEW0_TASK);
    Kew0EventWork *const work = static_cast<Kew0EventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->entry = static_cast<std::int32_t>(argument);
    work->state = 0;
    work->reserved18 = 0;
    work->lcdSequence = nullptr;
    work->adviceShown = 0;
    work->characters = Queue_EVT_AT_ReadChr(task, KEW0_CHARACTERS);
    InitializeKeikaEvent(work);
}
const ScriptAreaDefinition kew0ScriptDefinition{.areaId = AREA_KEW0, .field04 = 0, .init = Kew0Init, .main = Kew0Main};
} // namespace shenmue::s2
