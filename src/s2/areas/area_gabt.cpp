#include "s2/areas/area_gabt.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/door.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/fight.hpp"
#include "s2/hand.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/moves_scroll.hpp"
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
// @verified - p: native nullsub_0 remains a deliberate effect-free boundary.
void GabtSequenceSetupNullsub() {
}
void RootTransition(CompiledSequenceWork &work, std::uint16_t next) {
    work.previousMode = work.mode;
    work.mode = next;
    work.continueAfterWait = 1;
    work.continueLoop = 1;
}
void RootWait(CompiledSequenceWork &work, std::uint16_t wait, std::uint16_t next, std::int32_t delay, std::int32_t continueAfterWait = 1) {
    work.mode = wait;
    work.nextMode = next;
    work.delay = delay;
    work.continueAfterWait = continueAfterWait;
}
void QueueGabtAuthor(shen_name author) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GabtAuthorMainCallback, 4, 0x0b, 0x18, GABT_MOLE_TASK);
    GabtAuthorWork &work = *static_cast<GabtAuthorWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.state = 1;
    work.author = author;
    GabtPrepareAuthorLighting(&work);
}
void PrepareGabtHands() {
    for (shen_name character : {GABT_MOLE_RYO, GABT_MOLE_GAI}) {
        HasCharacterMotionWork(FindCharacterByScenarioName(character));
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(character), 1, 1, 0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(character), 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
    }
}
void ResetGabtBase() {
    EventSetPosition position{.cleanup = 0, .character = GABT_MOLE_BASE, .parent = GABT_MOLE_INVALID, .position = {.x = 0, .y = 0, .z = 0}, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = GABT_MOLE_BASE, .parent = GABT_MOLE_INVALID, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
}
void PrepareGabtResources(GabtEventWork &work) {
    if (IsFightActive())
        check_dead_status(1);
    work.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, GABT_MOLE_BASE);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, GABT_ATTACHMENT);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, GABT_PIVOT);
    EV_FreeAuthorPak(1, "0232");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0232A000");
    SetMapControlStatusFlag(0, 0);
    SetIsSkySetup(0);
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
void RetireGabtEvent(GabtEventWork &work) {
    HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_RYO));
    HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_GAI));
    hndlSetFlags(FindCharacterByScenarioName(GABT_MOLE_RYO));
    hndlSetFlags(FindCharacterByScenarioName(GABT_MOLE_GAI));
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    SetScenarioCharacterKillMode(GABT_MOLE_BASE, 1);
    SetScenarioCharacterKillMode(GABT_PIVOT, 1);
    SetScenarioCharacterKillMode(GABT_ATTACHMENT, 1);
    SetMotionRegionFlag(scenFindMotionwork(GABT_MOLE_RYO), 71, 0, MotionRegion::LowerBody);
    for (std::size_t index = 0; work.motions[index]; ++index)
        DeleteMotion(static_cast<std::int32_t>(work.motions[index]));
    EV_FreeAuthorPak(0, "0232");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("GABT"), 0);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    SetQeDoorSceneState(1);
    RemoveInventoryItem(39, 1);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_scenQueueLoad(g_actNumber, GABT_RETURN, 96);
}
} // namespace
// @verified - p: root compiled sequence106a10, including authored, battle, practice and both complete result branches.
void GabtRootSequenceCallback(void *data) {
    CompiledSequenceWork &work = *static_cast<CompiledSequenceWork *>(data);
    GabtEventWork &event = *static_cast<GabtEventWork *>(work.event);
    work.callbackFlag = 0;
    do {
        work.continueLoop = 0;
        switch (work.mode) {
        case 0:
            ResetGabtBase();
            EV_AT_SetPos(GABT_MOLE_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_PLAYER_POSITION);
            EV_AT_SetPos(GABT_MOLE_GAI, EventTransformFlags::World | EventTransformFlags::AllAxes, GABT_OPPONENT_POSITION);
            EV_SetCharaControllerFlags(GABT_MOLE_RYO, UpdateHeight, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_GAI, UpdateHeight, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_RYO, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_RYO, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_GAI, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_GAI, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_SCENERY, ApplyGravity, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(GABT_MOLE_SCENERY, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
            for (std::size_t index = 0; index < 3; ++index) {
                EV_DisableCharacterDisplay(GABT_AUTHOR_SCENERY[index]);
                EV_ToggleCharacterFlag(GABT_AUTHOR_SCENERY[index]);
            }
            mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_RYO), 0);
            mtwkSetRequestedRootMode(scenFindMotionwork(GABT_MOLE_GAI), 0);
            RootTransition(work, 100);
            break;
        case 100:
            if (event.entry > 0 && event.entry <= 2)
                RootWait(work, 2, 109, 10);
            else if (event.entry == 3)
                RootTransition(work, 103);
            else
                RootWait(work, 2, 101, 10);
            break;
        case 101:
            QueueGabtAuthor(GABT_AUTHOR_INTRO);
            RootWait(work, 13, 102, 5, 0);
            break;
        case 102:
            RootWait(work, 11, 103, 2);
            break;
        case 103: {
            EV_ReinitLights(0);
            EV_SetModelDataDefaultVertexShader();
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
            EV_lightLoad(GABT_AUTHOR_LIGHT_INTRO);
            PushGlobalLightingProgram();
            CompiledSequenceWork *child = QueueCompiledSequence(GabtBattleSequenceCallback, GABT_BATTLE, &event);
            if (!work.children[0])
                work.children[0] = child;
            child->parent = &work;
            child->childSlot = 0;
            child->reference = work.reference + 1;
            work.timer = 0;
            *reinterpret_cast<std::int32_t *>(work.parameters) = 0;
            work.waitChildIndex = 0;
            work.mode = 10;
            work.nextMode = 104;
            work.continueAfterWait = 1;
            break;
        }
        case 104:
            RootTransition(work, 105);
            break;
        case 105:
            QueueGabtAuthor(GABT_AUTHOR_PRACTICE);
            RootWait(work, 13, 106, 5, 0);
            break;
        case 106:
            RootWait(work, 2, 107, 32);
            break;
        case 107:
            QueueGabtMoleAttack(event.entry);
            RootWait(work, 2, 108, 1);
            break;
        case 108:
            if (gabtOutcome)
                RootTransition(work, 109);
            break;
        case 109:
            SetWeatherUpdateEnabledTo(0);
            EV_SetModelDataDefaultVertexShader();
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
            EV_lightLoad(GABT_AUTHOR_LIGHT_INTRO);
            PushGlobalLightingProgram();
            GabtSequenceSetupNullsub();
            GabtPrepareReflectors();
            ResetGabtBase();
            EV_SetCharacterAlphaEnabled(GABT_MOLE_RYO, false);
            EV_SetCharacterAlphaEnabled(GABT_MOLE_GAI, false);
            if (gabtOutcome == -1)
                RootWait(work, 2, 110, 10);
            else if (gabtOutcome == 1)
                RootWait(work, 2, 112, 10);
            break;
        case 110:
            GabtPrepareReflectors();
            event.authorFrames = QueueGabtFailureChoreography();
            RootWait(work, 2, 111, 1);
            break;
        case 111:
            if (authGetFinished()) {
                EV_CleanupMapControlTask();
                event.authorFrames = 0;
            }
            if (event.authorFrames)
                --event.authorFrames;
            else
                RootWait(work, 2, 116, 1);
            break;
        case 112:
            GabtPrepareReflectors();
            QueueExtraTelop("BBBB", 99);
            event.authorFrames = QueueGabtSuccessChoreography();
            RootWait(work, 2, 113, 1);
            break;
        case 113:
            if (event.authorFrames)
                --event.authorFrames;
            else
                RootWait(work, 2, event.entry == 1 ? 112 : 114, 1);
            break;
        case 114:
            SetMoveProficiency(0, 3, 21, 12);
            SetNewEwzoTypeForMoveLearned(0, 3, 21);
            event.moveTimer = 150;
            RootWait(work, 2, 115, 1);
            break;
        case 115:
            if (--event.moveTimer == 30)
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            if (GetCurrentControlInput(INPUT_PRESSED) == 0x200 || !event.moveTimer) {
                SetNewEwzoTypeForMoveLearned(1, 0, 0);
                RootTransition(work, 116);
            }
            break;
        case 116: {
            Vector3 position;
            Vector3i rotation;
            EV_GetCharaPosition(GABT_MOLE_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(GABT_MOLE_RYO, &rotation, EventTransformFlags::World);
            AT_SetLocalBaseChara(GABT_MOLE_RYO, GABT_MOLE_INVALID);
            EV_AT_SetPos(GABT_MOLE_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(GABT_MOLE_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            EV_GetCharaPosition(GABT_MOLE_GAI, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(GABT_MOLE_GAI, &rotation, EventTransformFlags::World);
            AT_SetLocalBaseChara(GABT_MOLE_GAI, GABT_MOLE_INVALID);
            EV_AT_SetPos(GABT_MOLE_GAI, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(GABT_MOLE_GAI, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            QueueGabtAuthor(GABT_AUTHOR_RESULT);
            RootWait(work, 13, 117, 5, 0);
            break;
        }
        case 117:
            EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
            RootTransition(work, 118);
            break;
        case 118:
            RootWait(work, 2, 119, 1);
            break;
        case 119:
            event.active = 0;
            RetireCompiledSequenceLinks(&work);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&work);
            break;
        }
    } while (work.continueLoop);
}
// @verified - p: BoutGaiMain_Callback106780.
void GabtRootCallback(void *data) {
    GabtEventWork &work = *static_cast<GabtEventWork *>(data);
    switch (work.state) {
    case 0:
        work.state = 3;
        break;
    case 3:
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("GABT"), 0);
        LoadBlurModelForCharacter(GABT_MOLE_RYO, "RYO_B.MT7", 1);
        HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_RYO));
        HasCharacterMotionWork(FindCharacterByScenarioName(GABT_MOLE_GAI));
        SetLowQualityHands(FindCharacterByScenarioName(GABT_MOLE_RYO));
        SetLowQualityHands(FindCharacterByScenarioName(GABT_MOLE_GAI));
        PrepareGabtHands();
        work.sequence = QueueCompiledSequence(GabtRootSequenceCallback, GABT_SEQUENCE, &work);
        work.active = 1;
        work.state = 4;
        break;
    case 4:
        if (!work.active)
            work.state = 5;
        break;
    case 5:
        work.state = 6;
        break;
    case 6:
        work.state = 7;
        break;
    case 7:
        RetireGabtEvent(work);
        break;
    default:
        break;
    }
}
// @verified - p: producer106460 preserves entry1/2 outcome overrides and all unwritten0x80 fields.
void QueueGabtEvent(std::int32_t entry) {
    if (entry == 1)
        gabtOutcome = 1;
    else if (entry == 2)
        gabtOutcome = -1;
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(GabtRootCallback, 4, 0x0b, 0x80, GABT_MOLE_TASK);
    GabtEventWork &work = *static_cast<GabtEventWork *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.entry = entry;
    work.state = 0;
    work.frames = 0;
    work.characters = Queue_EVT_AT_ReadChr(task, GABT_MOLE_CHARACTERS);
    PrepareGabtResources(work);
}
void InitializeGabtEvent() {
    InitializeGabtMoleGlobals();
}
// @verified - p: GABTmain1062f0 ignores its formal ABI parameter and uses EntryPoint.
void RunGabtEvent(std::uintptr_t) {
    if (g_entryPoint == 99) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>("GABT"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        QueueGabtMoleAttack(99);
    } else
        QueueGabtEvent(g_entryPoint);
}
const ScriptAreaDefinition gabtScriptDefinition{.areaId = GABT_AREA, .field04 = 0, .init = InitializeGabtEvent, .main = RunGabtEvent};
} // namespace shenmue::s2::areas
