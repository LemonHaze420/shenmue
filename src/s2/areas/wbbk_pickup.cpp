#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/areas/wbbk_pickup_hands.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
namespace shenmue::s2::areas {
void BookPickUpStack() {
    const shen_name stack = bookModelState.stacks[bookModelState.currentStack + 1];
    switch (bookEventPhase.phase) {
    case 0: {
        SetPlayerControlEnabled(0);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 1, reinterpret_cast<std::byte *>(bookPickupLeftHand), 10);
        ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(BOOK_RYO), 0, reinterpret_cast<std::byte *>(bookPickupRightHand), 10);
        EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(BOOK_RYO, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(BOOK_BASE, CharacterControlUpdateHeight, NpcCharacterControllerState::Disabled);
        SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 1, MotionRegion::LowerBody);
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf002, MotionRegion::LowerBody);
        const BookStackPlacement &placement = bookStackPlacements[bookModelState.currentStack];
        Vector3 camera = placement.position;
        camera.y -= .5f;
        EV_AT_SetPos(bookCameraAnchor, EventTransformFlags::AllAxes, camera);
        EV_AT_SetAngY(bookCameraAnchor, EventTransformFlags::AllAxes, {.x = 0, .y = placement.yaw, .z = 0});
        bookDesiredCamera = 2006;
        bookCurrentCamera = 2006;
        EVS_LoopCamera(2006, bookCameraAnchor, bookCameraAnchor);
        HLib::Task *task = HLib::EnqueueTaskWithoutParameter(BookPickupCameraCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
        HLib::SetupTimedTask(task, 60);
        Vector3 offset{.x = 0, .y = -.45f, .z = -.69999999f};
        Vector3 origin = placement.position;
        const Vector3i rotation{.x = 0, .y = placement.yaw + 32768, .z = 0};
        ApplyScenarioTranslationAndRotation(&origin, &rotation, &offset);
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 1);
        SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 15, MotionRegion::LowerBody);
        CopyMotionTransition(scenFindMotionwork(BOOK_RYO), &offset, placement.yaw, 0, 0);
        const std::uint16_t animation = placement.yaw == 32768 ? 6291 : 6276;
        SetMotionTimeRelated(scenFindMotionwork(BOOK_RYO), 1, 5, animation);
        mtwkSetExtraFlag(scenFindMotionwork(BOOK_RYO), 15, 0);
        SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), 29, MotionRegion::LowerBody);
        ++bookEventPhase.phase;
        break;
    }
    case 1:
        if (!mtwkSetExtraFlag(scenFindMotionwork(BOOK_RYO), 58, 1)) {
            ResetBookRound();
            EV_EnsureCharacterDraw(BOOK_BASE);
            AT_SetLocalBaseChara(BOOK_BASE, shen_name{0xffffffffU});
            bookModelState.displayMode = 0;
            UpdateBookStackDisplay();
            ++bookEventPhase.phase;
        }
        break;
    case 2:
        bookEventPhase = {.phase = 3, .frame = 10};
        break;
    case 3:
        if (bookEventPhase.frame == 28)
            Loop3DSoundEffect(930);
        if (GetMotionFrame(scenFindMotionwork(BOOK_RYO), MotionRegion::LowerBody) >= 76) {
            EV_GetCharaPosition(stack, CHARACTER_TRANSLATION_ROOT, &bookCarryAnchor, EventTransformFlags::Local);
            bookCarryAnchorBlend = 0;
            AT_SetLocalBaseChara(stack, bookStackAnchor);
            EV_AT_SetPos(stack, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
            EV_AT_SetAngY(stack, EventTransformFlags::AllAxes, {.x = 0, .y = 0, .z = 0});
            AT_SetLocalBaseChara(stack, bookStackAnchor);
            UpdateBookCarryAnchor();
            ++bookEventPhase.phase;
        }
        break;
    case 4:
        UpdateBookCarryAnchor();
        if (bookCarryAnchorBlend >= 1)
            bookCarryAnchorBlend = 1;
        else
            bookCarryAnchorBlend += .1f;
        if (bookEventPhase.frame == 78)
            Loop3DSoundEffect(930);
        if (bookEventPhase.frame == 142) {
            ++bookEventPhase.phase;
            bookEventPhase.frame = 0;
        }
        break;
    case 5: {
        InitializeBookCarryState();
        bookCarryIdleFrames = 0;
        bookCarryDistance = 0;
        Vector3 position;
        Vector3i oldRotation;
        EV_GetCharaPosition(BOOK_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
        GetScenarioCharacterEulerAngles(BOOK_RYO, &oldRotation, EventTransformFlags::Local);
        const std::uint32_t yaw = SampleBookCarryTrack(bookInitialCarryTracks[bookModelState.currentStack], position, 0, 0);
        EV_AT_SetPos(BOOK_RYO, EventTransformFlags::AllAxes, position);
        EV_AT_SetAngY(BOOK_RYO, EventTransformFlags::AllAxes, {.x = 0, .y = static_cast<std::int32_t>(yaw), .z = 0});
        const BookMotionPreset &preset = bookMotionPresets[bookCarryMotion.current];
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), preset.motion, MotionRegion::LowerBody);
        SetMotionRegionEvalTime(scenFindMotionwork(BOOK_RYO), preset.firstFrame, MotionRegion::LowerBody);
        mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_RYO), 1);
        SetMotionFrameTime(scenFindMotionwork(BOOK_RYO), 10, MotionRegion::LowerBody);
        ++bookEventPhase.phase;
        bookEventPhase.frame = 0;
        break;
    }
    case 6:
        if (bookModelState.currentStack == 13 && bookModelState.currentBook >= 2) {
            const BookCompanionPlacement &placement = bookCompanionPlacements[bookModelState.currentBook - 1];
            SetBookCompanionPose(placement.position, placement.yaw);
            mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1899, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BOOK_SAI), 66, 0, MotionRegion::LowerBody);
            mtwkSetRequestedRootMode(scenFindMotionwork(BOOK_SAI), 1);
            SetMotionFrameTime(scenFindMotionwork(BOOK_SAI), 20, MotionRegion::LowerBody);
            bookCompanionTarget = placement.position;
        }
        UpdateBookCarryAnchor();
        if (bookEventPhase.frame >= 10) {
            bookDesiredCamera = 0;
            bookCurrentCamera = 0;
            AttachBookStackToPlayer();
            bookEventPhase.phase = 100;
            SetMotionSpeed(scenFindMotionwork(BOOK_RYO), 1, MotionRegion::LowerBody);
            SelectBookStoryVariant();
            if (bookStoryVariant == 4 && bookModelState.currentBook == 13) {
                bookCarryExitTicks = 30;
                SetScreenFilmFade(28, 0, 0, 0, 0, 255, 0, 0, 0);
            }
        }
        break;
    case 100: {
        UpdateBookCarryTask();
        const std::int16_t countdown = bookCarryExitTicks;
        if (countdown) {
            --bookCarryExitTicks;
            if (countdown == 1) {
                CleanupBookActors();
                HLib::DestroyCurrentTask();
                RecordBookCount(bookModelState.currentBook, 1);
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1873, 1);
                scenSetTaskFlags(8);
                EV_scenQueueLoad(g_actNumber, SHEN_NAME(WB00), 0);
            }
        }
        UpdateBookCompanionPosition();
        std::int32_t idle = 0;
        if (!bookCarryPhase && bookCarryMotion.requested == 9 && !bookModelState.displayMode)
            idle = bookCarryIdleFrames;
        bookCarryIdleFrames = idle + 1;
        if (idle > 300) {
            QueueBookDialogue(9, 0);
            bookCarryIdleFrames = 0;
        }
        if (bookCarryPhase == 1)
            ChangeBookEventState(8);
        break;
    }
    default:
        return;
    }
}
} // namespace shenmue::s2::areas
