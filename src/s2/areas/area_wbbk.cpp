#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_authored.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/areas/wbbk_qte.hpp"
#include "s2/areas/wbbk_scenario.hpp"
#include "s2/areas/wbbk_shadow.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/d3t_renderer.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/hand.hpp"
#include "s2/help.hpp"
#include "s2/load_title.hpp"
#include "s2/model_component.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
#include <cstring>
namespace shenmue::s2::areas {
namespace {
void BookScenarioInitializeCallback(void *) {
    EV_DisableCharacterDisplay(BOOK_BASE);
    EV_DisableCharacterDisplay(BOOK_DYNAMIC);
    bookDropTimePenalty = 15;
    for (std::uint8_t &available : bookAvailableStacks)
        available = 1;
    bookPlayerAnchor = CreateEventCharacter(CharacterCreationHighModel, nullptr, shen_name{});
    bookCameraAnchor = CreateEventCharacter(CharacterCreationHighModel, nullptr, shen_name{});
    bookStackAnchor = CreateEventCharacter(CharacterCreationHighModel, nullptr, shen_name{});
    EV_DisableCharacterDisplay(bookPlayerAnchor);
    EV_DisableCharacterDisplay(bookCameraAnchor);
    EV_DisableCharacterDisplay(bookStackAnchor);
    CreateBookStackModels();
    CreatePutBookModels();
    HLib::EnqueueTaskWithoutParameter(BookMotionSoundCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
    bookFaceControl = {};
    HLib::EnqueueTaskWithoutParameter(BookFaceLooksCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
    const shen_name actors[]{BOOK_SAI, SHEN_NAME(KUN_), SHEN_NAME(SYE_), SHEN_NAME(KNK_), shen_name{}};
    QueueBookScenarioActors("", actors, BookMainCallback);
    ChangeBookEventState(0);
    UpdateCurrentFramesRendered(1);
    shenSetForcedFilmbars(0);
}
void BookStartCallback(void *) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    EV_LoadCutsceneFromCurrentScene("0035A000");
    bookCarryAnchorBlend = 0;
    bookCarryAnchor = {.x = 0, .y = 0, .z = 0};
    std::uint8_t level = static_cast<std::uint8_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 10) & 15);
    if (level >= 5)
        level = 4;
    bookCarryMotion.difficulty = level;
    std::int32_t next = level + 1;
    if (next >= 5)
        next = 4;
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 10, next | (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 10) & 0xf0));
    SetLowQualityHands(FindCharacterByScenarioName(BOOK_RYO));
    SetHighModelXYZ(FindCharacterByScenarioName(BOOK_RYO), 1, 1, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(BOOK_RYO), 1, 0, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 1, reinterpret_cast<std::byte *>(bookHandCommand), 10);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 0, reinterpret_cast<std::byte *>(bookHandCommand), 10);
    EV_stepSetFlags(0, 0x29);
    EV_EventSetUpHelpIcon_0(0x4d, 0);
    CreateBookSmoke();
    srchSetTarget(2, bookSearchTargets);
    UpdateTempHourMinTimeScale(23, 30, 20);
    npcResetDateTimeIfNeeded();
    bookModelState.createdCount = 0;
    CheckIsArgEquZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(4, 0);
    SelectBookStoryVariant();
    EV_FreeAuthorPak(1, "0035");
    HLib::SetCurrentTaskTo(BookScenarioInitializeCallback);
}
void InitializeBookArea() {
    std::memset(bookModelState.created, 0, sizeof(bookModelState.created));
    std::memset(bookModelState.types, 0, sizeof(bookModelState.types));
    std::memset(bookModelState.placed, 0, sizeof(bookModelState.placed));
    std::memset(bookModelState.stacks, 0, sizeof(bookModelState.stacks));
    std::memset(bookAvailableStacks, 0, sizeof(bookAvailableStacks));
    std::memset(bookQteCommands, 0, 5);
    std::memset(bookQteCommands + 8, 0, 40);
    bookModelState.createdCount = 0;
    bookPlayerAnchor = {};
    bookCameraAnchor = {};
    bookStackAnchor = {};
    bookSmokeEffect = 0;
    bookSmoothMovement.fromPosition = {.x = 0, .y = 0, .z = 0};
    bookSmoothMovement.toPosition.y = 0;
    bookSmoothMovement.toPosition.z = 0;
    bookSmoothMovement.fromRotation = {.x = 0, .y = 0, .z = 0};
    bookSmoothMovement.toRotation = {.x = 0, .y = 0, .z = 0};
    bookSmoothMovement.character = {};
    bookSmoothMovement.frame = 0;
    bookSmoothMovement.duration = 0;
    bookSmoothMovement.flags = 0;
    bookCarryTrack = nullptr;
    bookModelState.currentBook = 0;
    bookModelState.currentStack = 0;
    bookCompletion = 0;
    bookEntrySelection = 0;
    bookEventState = 0;
    bookEventPhase = {};
    bookCarryMode = 0;
    bookCarryPhase = 0;
    bookModelState.displayMode = 0;
    bookCarryVariant = 0;
    bookDesiredCamera = 0;
    bookCurrentCamera = 0;
    bookCarryIdleFrames = 0;
    bookModelState.animationFrame = 0;
    bookCarryDistance = 0;
    std::memset(bookQteTimings, 0, sizeof(bookQteTimings));
    bookQteTimingIndex = 0;
    bookQteCommandIndex = 0;
    bookModelState.mirror = 0;
    bookCarrySegmentFrame = 0;
    bookDropTimePenalty = 0;
    bookCarryMotion = {};
    bookCarryAnchorBlend = 0;
    bookCarryAnchor = {.x = 0, .y = 0, .z = 0};
    bookPlayerAlpha = 0;
    bookFaceControl = {};
    bookCarryExitTicks = 0;
    bookIntroWait = 0;
    bookDropRecovery = 0;
    bookStoryVariant = 0;
    bookCompanionControl = {};
    bookCompanionTarget = {.x = 0, .y = 0, .z = 0};
    bookAuthoredScene = 0;
    std::memset(bookAuthoredActors, 0, 3 * sizeof(shen_name));
}
void EnterBookArea(std::uintptr_t argument) {
    if (argument == SHEN_NAME(burn)) {
        EV_stepSetFlags(0, 0x2b);
        SetPlayerControlEnabled(0);
        EV_stepSetFlags(0, 0x2b);
        QueueBookAuthoredScene(4);
        ChangeBookEventState(5);
    } else {
        SetBookActorPose(BOOK_RYO, {.x = 349.10001f, .y = 127.42f, .z = 232.59f}, 8192);
        shenUpdateFlagEvaluation(1);
        if (EV_GetShenmueFlag(0x3a))
            spmFadeOut();
        LoadBookShadowReader();
        SetPlayerControlEnabled(0);
        FinishLoading();
        EV_SetCanDoAreaJumpTo(0);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(BookStartCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
        HLib::SetupTimedTask(task, 32);
    }
}
void FinishBookSession() {
    CompleteBookWork();
    bookEventState = 1;
    bookEventPhase = {};
}
} // namespace
void BookMainCallback(void *) {
    using enum CameraState;
    ++bookEventPhase.frame;
    switch (bookEventState) {
    case 0:
        BookIntroduction();
        return;
    case 1:
        if (bookEventPhase.phase) {
            if (bookEventPhase.phase == 1) {
                CleanupBookActors();
                HLib::DestroyCurrentTask();
                ChangeBookEventState(2);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 966, 1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 2192, 1);
                scenSetTaskFlags(8);
                EV_scenQueueLoad(g_actNumber, SHEN_NAME(WB00), 2);
                ReleaseBookShadowReader();
            }
            return;
        }
        CheckHelpIcon(0, 0, CheckHelpIconOp::Set);
        for (std::int32_t index = 0; index < 20; ++index) {
            const shen_name actor{BOOK_FIRST.name + static_cast<std::uint32_t>(index)};
            EV_DisableCharacterDisplay(actor);
            EV_ToggleCharacterFlag(actor);
        }
        bookEntrySelection = -1;
        if (bookCompletion) {
            bookFaceControl.player = 3;
            bookFaceControl.companion = 3;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 11, ((EV_GetPlayerFlag(PlayerFlagReadMode::Stat, 11) & 15) + 1) | 0xe0);
            if (bookStoryVariant == 2 || bookStoryVariant == 3) {
                bookEntrySelection = bookStoryVariant == 2 ? 2 : 1;
                EV_stepSetFlags(0, bookStoryVariant == 2 ? 0xfd : 0xfc);
                EV_DisableCharacterDisplay(BOOK_SAI);
            } else if (((bookStoryVariant - 1) & ~4) == 0) {
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1354, 1);
                bookEntrySelection = 5;
                EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
            }
            if (bookEntrySelection != -1) {
                EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
                QueueBookAuthoredScene(bookEntrySelection);
            }
        }
        ++bookEventPhase.phase;
        return;
    case 3:
        if (!bookEventPhase.phase) {
            SetPlayerGameFlags(PlayerGameFlagMode::EnableAreaControl, nullptr);
            ++bookEventPhase.phase;
            return;
        }
        if (bookEventPhase.phase != 1 || !SetPlayerGameFlags(PlayerGameFlagMode::QueryAreaControl, nullptr))
            return;
        SetPlayerGameFlags(PlayerGameFlagMode::DisableAreaControl, nullptr);
        ChangeBookEventState(4);
        BookPickUpStack();
        return;
    case 4:
        BookPickUpStack();
        return;
    case 5:
        switch (bookEventPhase.phase) {
        case 0:
            if (!EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 897) || EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 188))
                EV_EnableCharacterDisplay(SHEN_NAME(a092));
            else
                EV_DisableCharacterDisplay(SHEN_NAME(a092));
            ResetBookMotionLayers();
            SetBookActorMotion(BOOK_RYO, 0xf002);
            SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 1, MotionRegion::LowerBody);
            EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
            EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
            camCtrlSetFovTransitionTo(true);
            camCtrlSetState(Init);
            ++bookEventPhase.phase;
            break;
        case 1:
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 1, reinterpret_cast<std::byte *>(bookHandCommand), 10);
            ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 0, reinterpret_cast<std::byte *>(bookHandCommand), 10);
            EV_EventSetUpHelpIcon_0(0x4d, 0);
            SetPlayerControlEnabled(1);
            bookCarryIdleFrames = 0;
            ++bookEventPhase.phase;
            break;
        case 2:
            UpdateBookCompanionPosition();
            BookIdleInput();
            break;
        default:
            break;
        }
        return;
    case 6:
        BookLeaveShelf();
        return;
    case 7:
        if (!bookEventPhase.phase) {
            spmFadeOut();
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(30);
            ++bookEventPhase.phase;
            return;
        }
        if (bookEventPhase.phase != 1)
            return;
        ResetBookMotionLayers();
        camCtrlToggleSmoothing(0);
        EV_AT_SetPos(BOOK_BASE, EventTransformFlags::AllAxes, {.x = 0, .y = -100, .z = 0});
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        QueueBookAuthoredScene(6);
        bookFaceControl.player = 7;
        bookFaceControl.companion = 2;
        RecordBookCount(bookModelState.currentBook, 0);
        FinishBookSession();
        return;
    case 8:
        switch (bookEventPhase.phase) {
        case 0:
            if (bookEventPhase.frame == 1) {
                ResetBookMotionLayers();
                SetBookActorMotion(BOOK_RYO, 6273);
                SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 1.3f, MotionRegion::LowerBody);
                SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 538, MotionRegion::LowerBody);
                QueueBookDialogue(6, bookModelState.currentBook + 1);
                Vector3 position;
                EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
                UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), &position, 1);
            }
            if (bookEventPhase.frame == 2) {
                camCtrlToggleSmoothing(0);
                CopyBookPlayerAnchor();
                EVS_LoopCamera(shenGetRandomInt32(2) + 1301, bookPlayerAnchor, bookPlayerAnchor);
            }
            UpdateBookCarryAnchor();
            if (bookEventPhase.frame == 69) {
                Loop3DSoundEffect(930);
                bookCompletion = 0;
                ++bookEventPhase.phase;
            }
            return;
        case 1:
            if (bookEventPhase.frame == 76) {
                SetScreenFilmFade(60, 0, 0, 0, 0, 255, 0, 0, 0);
                if (bookModelState.currentBook + 1 >= 14) {
                    bookCompletion = 1;
                    spmFadeOut();
                }
                if (EV_GetShenmueFlag(0xfe) && bookModelState.currentBook == 8) {
                    bookCompletion = 2;
                    spmFadeOut();
                }
            }
            if (bookEventPhase.frame != 146)
                return;
            UpdateMotionWorkWithPosition(scenFindMotionwork(BOOK_SAI), nullptr, 0);
            SetRenderClockAndCompassTo(0);
            EV_DisableCharacterDisplay(bookModelState.stacks[bookModelState.currentStack + 1]);
            EV_AT_SetAngY(BOOK_RYO, EventTransformFlags::AllAxes, {.x = 0, .y = 49152, .z = 0});
            ++bookEventPhase.phase;
            return;
        case 2:
            FinishBookRound();
            ++bookModelState.currentBook;
            if (bookCompletion == 1)
                FinishBookSession();
            else if (bookCompletion == 2) {
                EV_stepSetFlags(0, 0xfe);
                bookCompletion = 0;
                ChangeBookEventState(11);
            } else {
                SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
                ChangeBookEventState(5);
            }
            break;
        default:
            break;
        }
        return;
    case 11:
        spmFadeOut();
        QueueBookAuthoredScene(3);
        ChangeBookEventState(5);
        snapAssignFrmCntFlags(1195, 60);
        return;
    default:
        return;
    }
}
const ScriptAreaDefinition wbbkScriptDefinition{.areaId = SHEN_NAME(WBBK), .field04 = 0, .init = InitializeBookArea, .main = EnterBookArea};
} // namespace shenmue::s2::areas
