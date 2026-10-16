#include "s2/areas/bkw0_sequence.hpp"
#include "s2/areas/bkw0_tables.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
void Bkw0BowSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Bkw0Work &work = *static_cast<Bkw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            AT_SetLocalBaseChara(BKW0_BASE, EVENT_CHARACTER_INVALID);
            AT_SetLocalBaseChara(BKW0_RYO, BKW0_BASE);
            AT_SetLocalBaseChara(BKW0_MYS, BKW0_BASE);
            EV_AT_SetPos(BKW0_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_BOW_BASE_POSITION);
            EV_AT_SetAngY(BKW0_BASE, EventTransformFlags::World | EventTransformFlags::AllAxes, BKW0_BOW_BASE_ROTATION);
            EV_EnableCharacterDisplay(BKW0_RYO);
            EV_EnableCharacterDisplay(BKW0_MYS);
            for (shen_name character : {BKW0_RYO, BKW0_MYS}) {
                EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
                EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
            }
            EV_SetCharaControllerFlags(BKW0_RYO, 2, NpcCharacterControllerState::Disabled);
            EV_SetCharaControllerFlags(BKW0_MYS, 2, NpcCharacterControllerState::Disabled);
            EV_SetCharacterAlphaEnabled(BKW0_RYO, false);
            EV_SetCharacterAlphaEnabled(BKW0_MYS, false);
            EV_mtwkSetEnabled(scenFindMotionwork(BKW0_RYO), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(BKW0_MYS), 1);
            SetMotionSpeed(scenFindMotionwork(BKW0_RYO), 1, MotionRegion::LowerBody);
            SetMotionSpeed(scenFindMotionwork(BKW0_MYS), 1, MotionRegion::LowerBody);
            Bkw0PlaceCrowd();
            Bkw0TrainCrowd(work.selector, 1);
            EV_SetRefBoardParamsFor(BKW0_RYO, 1);
            EV_SetRefBoardParamsFor(BKW0_MYS, 1);
            Bkw0Advance(sequence, 100);
            break;
        case 100:
            EV_AT_SetPos(BKW0_RYO, EventTransformFlags::AllAxes, bkw0BowRyoPosition);
            EV_AT_SetPos(BKW0_MYS, EventTransformFlags::AllAxes, BKW0_BOW_MYS_POSITION);
            EV_AT_SetAngY(BKW0_RYO, EventTransformFlags::AllAxes, bkw0BowRyoRotation);
            EV_AT_SetAngY(BKW0_MYS, EventTransformFlags::AllAxes, bkw0BowMysRotation);
            mtwkRequestMotion(scenFindMotionwork(BKW0_RYO), 7954, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(BKW0_MYS), 7955, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(BKW0_MYS), 71, 0, MotionRegion::LowerBody);
            work.camera = 9050;
            EVS_LoopCamera(9050, BKW0_BASE, BKW0_BASE);
            work.frames = 0;
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            Bkw0Advance(sequence, 103);
            break;
        case 101:
            if (!NpcWait() && work.frames <= 60)
                break;
            work.frames = 0;
            Bkw0Advance(sequence, 102);
            break;
        case 102:
            if (NpcWait() && CountNpcLiveStreams() < 5 && work.frames <= 1800)
                break;
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            work.frames = 0;
            Bkw0Advance(sequence, 103);
            break;
        case 103:
            switch (work.frames) {
            case 30:
                EV_PlayDialogue("0068D001");
                break;
            case 80:
                EV_PlayDialogue("0068A016");
                break;
            case 160:
                EV_PlayDialogue("0068D002");
                break;
            case 260:
                EV_PlayDialogue("0068D003");
                break;
            case 340:
                EV_PlayDialogue("0068A017");
                break;
            case 400:
                work.camera = 9051;
                EVS_LoopCamera(9051, BKW0_BASE, BKW0_BASE);
                EV_PlayDialogue("0068D004");
                break;
            case 510:
                hmdlSetFlagsFor(FindCharacterByScenarioName(BKW0_MYS), 0, 0, 16);
                hmdlSetFlagsFor(FindCharacterByScenarioName(BKW0_MYS), 1, 1, 16);
                break;
            case 570:
                hmdlSetFlagsFor(FindCharacterByScenarioName(BKW0_MYS), 0, 2, 16);
                hmdlSetFlagsFor(FindCharacterByScenarioName(BKW0_MYS), 1, 2, 16);
                break;
            case 590:
                EV_PlayDialogue("0068A018");
                break;
            case 610:
                SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
                break;
            case 640:
                Bkw0Advance(sequence, 104);
                break;
            default:
                break;
            }
            break;
        case 104:
            AT_SetLocalBaseChara(BKW0_RYO, EVENT_CHARACTER_INVALID);
            AT_SetLocalBaseChara(BKW0_MYS, EVENT_CHARACTER_INVALID);
            work.active = 0;
            Bkw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
