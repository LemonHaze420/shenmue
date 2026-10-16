#include "s2/areas/knmn_practice.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/help.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/pause.hpp"
#include "s2/screen_film.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
void KnmnPracticeFinishNullsub() {
}
void KnmnSetPracticeGravity(std::int32_t enabled) {
    for (shen_name character : {KNMN_RYO, KNMN_DEKU}) {
        EV_SetCharaControllerFlags(character, ApplyGravity, static_cast<NpcCharacterControllerState>(enabled));
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, static_cast<NpcCharacterControllerState>(enabled));
    }
}
std::int32_t knmnSuccessSavedPostFx;
} // namespace
void KnmnPracticeSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t instruction = KnmnValue<std::int64_t>(data, 0x138), second = KnmnValue<std::int64_t>(data, 0x140);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            KnmnValue(&work, 0x650, std::uint8_t{0});
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
            KnmnSetPracticeGravity(1);
            EV_mtwkSetEnabled(scenFindMotionwork(KNMN_RYO), 1);
            EV_mtwkSetEnabled(scenFindMotionwork(KNMN_DEKU), 1);
            SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 71, 0, MotionRegion::LowerBody);
            EV_AT_SetPos(KNMN_DEKU, EventTransformFlags::AllAxes, KNMN_DUMMY_PRACTICE_POSITION);
            FinishLoading();
            CheckHelpIcon(1, 0, CheckHelpIconOp::Add);
            KnmnAdvance(sequence, 100);
            break;
        case 100:
            KnmnWait(sequence, 2, 101, 1);
            break;
        case 101: {
            vfSetMoveInstructionConfiguration(
                KNMN_RYO, KNMN_KEM, static_cast<std::int16_t>(instruction), static_cast<std::int16_t>(second), reinterpret_cast<std::byte *>(work.results));
            Vector3 position;
            EV_GetCharaPosition(KNMN_RYO, 5, &position, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), &position, 0);
            CompiledSequenceWork *child = KnmnChild(sequence, TrainingAttackSequence, SHEN_NAME(BATT), 0);
            KnmnValue(child, 0x138, KNMN_RYO);
            KnmnValue(child, 0x140, KNMN_DEKU);
            KnmnValue(child, 0x148, std::int32_t{1});
            KnmnValue(child, 0x150, std::int32_t{1});
            KnmnValue(child, 0x158, std::int64_t{0});
            KnmnWait(sequence, 2, 102, 10);
            break;
        }
        case 102:
            if (work.results[0].status != 2)
                break;
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 1);
            KnmnAdvance(sequence, 104);
            break;
        case 103:
            if (!IsFightActive())
                KnmnAdvance(sequence, 104);
            else {
                work.fightSuspended = 1;
                SuspendFight();
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 1);
            }
            break;
        case 104:
            sequence.waitChildIndex = 0;
            KnmnWait(sequence, 10, 105);
            break;
        case 105:
            KnmnPracticeFinishNullsub();
            KnmnSetPracticeGravity(0);
            KnmnComplete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnQteSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t continuation = KnmnValue<std::int64_t>(data, 0x138);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (work.fightSuspended) {
                work.fightSuspended = 0;
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
                check_dead_status(1);
            }
            knmnQteDialogue = nullptr;
            KnmnValue(data, 0x24c, std::uint64_t{0});
            if (work.changeCamera)
                work.changeCamera = 0;
            SetTrainingCameraSequence(&sequence, work.practiceCamera);
            KnmnWait(sequence, 2, 100, 10);
            break;
        case 100:
            EV_EventSetUpHelpIcon_0(HELP_TOPIC_MOVE_INSTRUCTION, 0);
            shenSetTimeSimulationTo(0);
            EV_AT_SetPos(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_PLAYER_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 16384, .z = 0});
            EV_AT_SetPos(KNMN_DEKU, EventTransformFlags::World | EventTransformFlags::AllAxes, KNMN_DUMMY_PRACTICE_POSITION);
            EV_AT_SetAngY(KNMN_DEKU, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3i{.x = 0, .y = 49152, .z = 0});
            SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 2, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(KNMN_RYO), 306, MotionRegion::LowerBody);
            KnmnAdvance(sequence, 101);
            break;
        case 101:
            KnmnValue(data, 0x248, std::int32_t{60});
            if (work.fightSuspended) {
                work.fightSuspended = 0;
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
                const std::int32_t restart = work.restartMotion;
                work.restartMotion = 0;
                vfmDispatchControl(FightControlOp::QueuedAction, KNMN_RYO, restart == 1 ? 409 : restart == 2 ? 408 : 0, 1);
            } else {
                shenStartFGST(10, 10, 0, 0);
                shenInitFightData(FindCharacterByScenarioName(KNMN_RYO), nullptr, 0, 0, 0, 16);
                vfUpdateFlagsFor(FindCharacterByScenarioName(KNMN_RYO), 0, 0x08000000U, 1);
                shenInitFightData(FindCharacterByScenarioName(KNMN_DEKU), nullptr, 1, 1, 100, 1);
                vfUpdateFlagsFor(FindCharacterByScenarioName(KNMN_RYO), 0, 0x10000U, 0);
                vfUpdateFlagsFor(FindCharacterByScenarioName(KNMN_DEKU), 0, 0x10000U, 0);
                EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
                qteBattleUtil(BattleQteMode::Load);
                qteBattleUtil(BattleQteMode::SetWait, 0, 12, 24);
                qteBattleUtil(BattleQteMode::SetFirstCommands, 1280);
                qteBattleUtil(BattleQteMode::SetLastCommands);
                qteBattleUtil(BattleQteMode::ResetStates);
                qteBattleUtil(BattleQteMode::Queue);
                EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
                vfmQueueFightStart(2, 99, 1, 0, -1);
                EV_vfBattleUtil(FightBattleUtilOp::CollisionFlags, shen_name{1}, 1);
            }
            if (work.changeCamera) {
                work.practiceCamera = 9010 + (work.practiceCamera == 9010);
                work.changeCamera = 0;
            }
            SetTrainingCameraSequence(&sequence, work.practiceCamera);
            KnmnAdvance(sequence, 102);
            break;
        case 102:
            if (GetMotionAnimationId(scenFindMotionwork(KNMN_RYO), MotionRegion::LowerBody) == 306 && !cvsHasValidConverseData()) {
                qteBattleUtil(BattleQteMode::Start);
                KnmnAdvance(sequence, 103);
            }
            break;
        case 103: {
            const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
            if (result == -1)
                KnmnAdvance(sequence, 109);
            else if (result == 1) {
                KnmnShowPracticeLcd(work);
                KnmnAdvance(sequence, 104);
            } else if (!result) {
                const std::int32_t frames = KnmnValue<std::int32_t>(data, 0x248);
                if (frames)
                    KnmnValue(data, 0x248, frames - 1);
                else if (GetMotionAnimationId(scenFindMotionwork(KNMN_RYO), MotionRegion::LowerBody) == 306 && !cvsHasValidConverseData()) {
                    qteBattleUtil(BattleQteMode::Start);
                    KnmnValue(data, 0x248, std::int32_t{60});
                }
            }
            break;
        }
        case 104:
            if (work.cameraSequence) {
                KnmnKillSequence(work.cameraSequence);
                work.cameraSequence = nullptr;
            }
            if (work.fightSuspended) {
                work.fightSuspended = 0;
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
            }
            check_dead_status(1);
            KnmnWait(sequence, 2, 105, 2);
            break;
        case 105:
            mtwkRequestMotion(scenFindMotionwork(KNMN_RYO), 1, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(KNMN_RYO), 69, 0, MotionRegion::LowerBody);
            KnmnWait(sequence, 2, 106, 12);
            break;
        case 106:
            if (knmnQteDialogue) {
                RetireTrainingSequence(knmnQteDialogue);
                knmnQteDialogue = nullptr;
            }
            EV_EnableCharacterDisplay(KNMN_KEM);
            ResetTrainingAuthorCharacter(KNMN_RYO);
            ResetTrainingAuthorCharacter(KNMN_KEM);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(WAZA)));
            KnmnWait(sequence, 2, 107, 2);
            break;
        case 107:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                KnmnWazaFrameEvents();
            else
                KnmnAdvance(sequence, 108);
            break;
        case 108:
            KnmnRestorePracticeCharacters();
            KnmnWait(sequence, 2, 113, 2);
            break;
        case 109: {
            const std::int32_t wait = KnmnValue<std::int32_t>(data, 0x250);
            if (wait)
                KnmnValue(data, 0x250, wait - 1);
            else {
                CompiledSequenceWork *child = KnmnChild(sequence, KnmnQteFailureSequence, SHEN_NAME(FS2S), 1);
                const std::int32_t previous = KnmnValue<std::int32_t>(data, 0x24c);
                KnmnValue(child, 0x138, previous);
                knmnQteDialogue = child;
                KnmnValue(data, 0x24c, previous == 0 ? 1 : previous == 1 ? 2 : 0);
                KnmnValue(data, 0x250, std::int32_t{1});
            }
            sequence.waitCharacter = KNMN_RYO;
            KnmnWait(sequence, 5, 110);
            break;
        }
        case 110:
            KnmnWait(sequence, 2, 111, 30);
            break;
        case 111:
            work.fightSuspended = 1;
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 1);
            QueueTrainingReposition(&sequence, 2, KNMN_RYO, 0, &KNMN_PLAYER_PRACTICE_POSITION);
            KnmnAdvance(sequence, 112);
            break;
        case 112:
            if (!work.repositionFrames[0]) {
                work.restartMotion = 1;
                KnmnAdvance(sequence, 101);
            }
            break;
        case 113:
            if (work.cameraSequence) {
                KnmnKillSequence(work.cameraSequence);
                work.cameraSequence = nullptr;
            }
            shenSetTimeSimulationTo(1);
            if (work.fightSuspended) {
                work.fightSuspended = 0;
                ResumeFight(1);
                mtwkSetRequestedRootMode(scenFindMotionwork(KNMN_RYO), 2);
            }
            check_dead_status(1);
            KnmnValue(data, 0x88, static_cast<std::int32_t>(continuation));
            sequence.reserved1a0 = 1;
            if (sequence.parent) {
                KnmnValue(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
                KnmnValue(sequence.parent, 0xb8 + 4 * sequence.childSlot, KnmnValue<std::int32_t>(data, 0x88));
            }
            qteBattleUtil(BattleQteMode::RetireAndFree);
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
void KnmnSuccessSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    KnmnPracticeWork &work = *static_cast<KnmnPracticeWork *>(sequence.event);
    const std::int64_t continuation = KnmnValue<std::int64_t>(data, 0x148);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
        case 102:
            KnmnAdvance(sequence, 103);
            break;
        case 100:
            KnmnAdvance(sequence, 101);
            break;
        case 101:
            work.camera = 9001;
            EVS_LoopCamera(9001, shen_name{}, shen_name{});
            KnmnChild(sequence, KnmnRepeatFeedbackSequence, SHEN_NAME(REPL), 0);
            sequence.waitChildIndex = 0;
            KnmnWait(sequence, 10, 102);
            sequence.children[1] = nullptr;
            break;
        case 103:
            KnmnWait(sequence, 13, 133, 5);
            sequence.continueAfterWait = 0;
            break;
        case 133: {
            KnmnValue(data, 0x88, static_cast<std::int32_t>(continuation));
            sequence.reserved1a0 = 1;
            EV_EnableCharacterDisplay(KNMN_KEM);
            ResetTrainingAuthorCharacter(KNMN_RYO);
            ResetTrainingAuthorCharacter(KNMN_KEM);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), nullptr, 3);
            Vector3 target;
            EV_GetCharaPosition(KNMN_RYO, 5, &target, EventTransformFlags::World);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_KEM), &target, 0);
            knmnSuccessSavedPostFx = static_cast<std::int32_t>(GetPostFxBankValue(3));
            SelectPostFxBank(3, 0);
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(SHEN_NAME(WAZA)));
            KnmnAdvance(sequence, 144);
            break;
        }
        case 134:
            if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
                KnmnWazaFrameEvents();
            else {
                SelectPostFxBank(3, knmnSuccessSavedPostFx);
                KnmnAdvance(sequence, 135);
            }
            break;
        case 135:
            KnmnRestorePracticeCharacters();
            KnmnWait(sequence, 2, 143, 2);
            break;
        case 143:
            KnmnComplete(sequence, true);
            return;
        case 144:
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 3);
            UpdateMotionWorkWithPosition(scenFindMotionwork(KNMN_RYO), nullptr, 0);
            KnmnWait(sequence, 2, 134, 1);
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
