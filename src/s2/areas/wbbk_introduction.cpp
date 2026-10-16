#include "s2/areas/area_wbbk.hpp"
#include "s2/areas/wbbk_authored.hpp"
#include "s2/areas/wbbk_dialogue.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/help.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void WaitBookOpening() {
    if (EV_GetShenmueFlag(0x3a)) {
        if (authGetFinished()) {
            EV_stepSetFlags(0, 0x3a);
            bookEventPhase.phase = 8;
            return;
        }
    } else if (HasPressedCancel()) {
        bookEventPhase.phase = 8;
        return;
    }
    const std::int32_t wait = bookIntroWait--;
    if (!wait)
        bookEventPhase.phase = 5;
}
} // namespace
void BookIntroduction() {
    switch (bookEventPhase.phase) {
    case 0:
        bookEventPhase.phase = 1;
        return;
    case 1: {
        const shen_name actors[]{SHEN_NAME(SYE_), SHEN_NAME(KUN_), SHEN_NAME(KNK_)};
        for (const shen_name actor : actors)
            if (FindCharacterByScenarioName(actor)) {
                EV_DisableCharacterDisplay(actor);
                EV_ToggleCharacterFlag(actor);
            }
        EV_SetCharaControllerFlags(BOOK_SAI, CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
        EV_SetCharaControllerFlags(BOOK_SAI, CharacterControlUpdateHeight | CharacterControlApplyGravity, NpcCharacterControllerState::Enabled);
        EV_npcUnknownFunc(BOOK_SAI);
        ++bookEventPhase.phase;
        return;
    }
    case 2:
        EV_SetCharaControllerFlags(BOOK_SAI, CharacterControlApplyGravity, NpcCharacterControllerState::Disabled);
        bookEntrySelection = 0;
        if (EV_GetShenmueFlag(0x3a)) {
            PrepareBookAuthoredActor(BOOK_RYO);
            PrepareBookAuthoredActor(SHEN_NAME(SYE_));
            PrepareBookAuthoredActor(SHEN_NAME(KNK_));
            QueueBookAuthoredScene(0);
        }
        ++bookEventPhase.phase;
        return;
    case 3: {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 9;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        InitializeBookActors();
        ++bookEventPhase.phase;
        return;
    }
    case 4:
        EV_LoadAudio("0035A000", 1);
        EV_spmBeginSampleExecute();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        EVT_AT_SetCam(2008, shen_name{0xffffffffU}, shen_name{0xffffffffU}, 0, 6, 0);
        camCtrlDisableFilmBars();
        SetBookActorPose(BOOK_RYO, {.x = 349.10001f, .y = 127.42f, .z = 232.59f}, 8192);
        SetBookActorPose(BOOK_SAI, {.x = 343.67001f, .y = 127.42f, .z = 231.69f}, 48241);
        SetBookActorMotion(BOOK_RYO, 6294);
        SetBookActorMotion(BOOK_SAI, 61445);
        bookIntroWait = 90;
        bookFaceControl.player = 7;
        bookFaceControl.companion = 3;
        bookEventPhase.phase = 140;
        WaitBookOpening();
        return;
    case 5:
        SetScreenFilmFade(15, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(15);
        ++bookEventPhase.phase;
        return;
    case 6:
        SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
        SetBookActorPose(BOOK_RYO, {.x = 348.10001f, .y = 127.42f, .z = 232.59f}, 6912);
        SetBookActorPose(BOOK_SAI, {.x = 347.04001f, .y = 127.42f, .z = 231.14f}, 40368);
        SetBookActorMotion(BOOK_RYO, 6294);
        SetBookActorMotion(BOOK_SAI, 6295);
        if (EV_GetShenmueFlag(0x3a)) {
            EV_stepSetFlags(0, 0x3a);
            bookIntroWait = QueueBookDialogue(0, 0) - 47;
            bookEventPhase.phase = 160;
        } else {
            bookIntroWait = QueueBookDialogue(1, 0) - 40;
            bookEventPhase.phase = 161;
        }
        return;
    case 7:
        SetBookActorPose(BOOK_SAI, {.x = 346.20999f, .y = 127.42f, .z = 232.03999f}, 16384);
        mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0xf005, MotionRegion::LowerBody);
        HLib::SetupTaskDelay(40);
        ++bookEventPhase.phase;
        return;
    case 8:
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetupTaskDelay(30);
        ++bookEventPhase.phase;
        return;
    case 9:
        SetBookActorPose(BOOK_RYO, {.x = 350.10999f, .y = 127.42f, .z = 231.25f}, 0);
        SetBookCompanionPose(bookCompanionPlacements[0].position, bookCompanionPlacements[0].yaw);
        mtwkRequestMotion(scenFindMotionwork(BOOK_SAI), 0x1897, MotionRegion::LowerBody);
        HLib::SetupTaskDelay(30);
        ++bookEventPhase.phase;
        return;
    case 10:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        mtwkRequestMotion(scenFindMotionwork(BOOK_RYO), 0xf002, MotionRegion::LowerBody);
        bookCompanionControl.state = 0;
        HLib::EnqueueTaskWithoutParameter(BookCompanionControlCallback, 6, 0x0d, static_cast<std::int32_t>(BOOK_TASK.name));
        CheckHelpIcon(1, 0, CheckHelpIconOp::Set);
        ChangeBookEventState(5);
        return;
    case 140:
        WaitBookOpening();
        return;
    case 160:
    case 161:
        if (bookEventPhase.phase == 160 ? authGetFinished() != 0 : HasPressedCancel()) {
            EV_CleanupMapControlTask();
            bookEventPhase.phase = 8;
        } else {
            const std::int32_t wait = bookIntroWait--;
            if (!wait)
                bookEventPhase.phase = 7;
        }
        break;
    default:
        return;
    }
}
} // namespace shenmue::s2::areas
