#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/motion_work.hpp"
#include "s2/help.hpp"
#include "s2/save.hpp"
#include "s2/training_seqp.hpp"
#include <cmath>
namespace shenmue::s2::areas {
namespace {
void Syw0GateLcd(Syw0Work &work) {
    Syw0Value(&work, 0x630, std::int32_t{600});
    Syw0Value(&work, 0x638, static_cast<const std::uint8_t *>(SYW0_FORCE_LCD));
    LcdmSelectMessage(1);
    LcdmInit(reinterpret_cast<std::uintptr_t>(SYW0_FORCE_LCD));
}
void Syw0GateResume(Syw0Work &work) {
    const std::int32_t suspended = Syw0Value<std::int32_t>(&work, 0x640);
    Syw0Value(&work, 0x640, std::int32_t{0});
    if (suspended) {
        ResumeFight(1);
        mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_RYO), 2);
    }
}
void Syw0GateStopCamera(Syw0Work &work) {
    CompiledSequenceWork *camera = Syw0Value<CompiledSequenceWork *>(&work, 0x5c0);
    if (camera) {
        Syw0KillSequence(camera);
        Syw0Value(&work, 0x5c0, static_cast<CompiledSequenceWork *>(nullptr));
    }
}
void Syw0GateCorrectionSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            Syw0BeginTeacherGesture(sequence);
            Syw0Wait(sequence, 2, 100, 30);
            break;
        case 100: {
            const std::int64_t correction = Syw0Value<std::int64_t>(&sequence, 0x138);
            EV_PlayDialogue(correction == 0 ? "0064B083" : correction == 1 ? "0064B084" : "0064B085");
            Syw0Wait(sequence, 4, 101, 60);
            break;
        }
        case 101:
            Syw0EndTeacherGesture();
            Syw0Complete(sequence);
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace
void Syw0GateSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Syw0Work &work = *static_cast<Syw0Work *>(sequence.event);
    const std::int32_t success = Syw0Value<std::int32_t>(&sequence, 0x138);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            if (Syw0Value<std::int32_t>(&work, 0x640)) {
                Syw0GateResume(work);
                check_dead_status(1);
            }
            Syw0GateLcd(work);
            Syw0Value(&sequence, 0x24c, std::int32_t{0});
            Syw0Value(&work, 0x5f4, std::int32_t{9013});
            Syw0Wait(sequence, 2, 100, 10);
            break;
        case 100: {
            EV_EventSetUpHelpIcon_0(14, 0);
            EV_AT_SetPos(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, SYW0_GATE_PLAYER_POSITION);
            const Vector3i playerRotation{.x = 0, .y = trainingGateTargetYaw, .z = 0};
            EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, playerRotation);
            EV_AT_SetPos(SYW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, SYW0_GATE_OPPONENT_POSITION);
            const Vector3i opponentRotation{.x = 0, .y = 32768, .z = 0};
            EV_AT_SetAngY(SYW0_KNK, EventTransformFlags::World | EventTransformFlags::AllAxes, opponentRotation);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 69, 2, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 69, 2, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_RYO), 306, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_KNK), 2325, MotionRegion::LowerBody);
            Syw0Advance(sequence, 101);
            break;
        }
        case 101: {
            Syw0Value(&sequence, 0x248, std::int32_t{30});
            if (Syw0Value<std::int32_t>(&work, 0x640)) {
                Syw0GateResume(work);
                const std::int32_t restart = Syw0Value<std::int32_t>(&work, 0x5b8);
                const std::uintptr_t motion = restart == 1 ? 409 : restart == 2 ? 408 : 0;
                Syw0Value(&work, 0x5b8, std::int32_t{0});
                if (vfmDispatchControl(FightControlOp::ReactionRank, SYW0_RYO, 0, -1) == 2)
                    vfmDispatchControl(FightControlOp::ReactionRank, SYW0_RYO, 1, 1);
                vfmDispatchControl(FightControlOp::QueuedAction, SYW0_RYO, motion, 1);
                vfmDispatchControl(FightControlOp::QueuedAction, SYW0_KNK, 0x915, 1);
                SetMotionRegionFlag(scenFindMotionwork(SYW0_KNK), 69, 2, MotionRegion::LowerBody);
            } else {
                shenStartFGST(10, 10, 0, 0);
                vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("SYW0"), 1);
                shenInitFightData(FindCharacterByScenarioName(SYW0_RYO), nullptr, 0, 0, 0, 16);
                vfUpdateFlagsFor(FindCharacterByScenarioName(SYW0_RYO), 0, 0x8000000, 1);
                shenInitFightData(FindCharacterByScenarioName(SYW0_KNK), nullptr, 1, 1, 100, 33);
                vfUpdateFlagsFor(FindCharacterByScenarioName(SYW0_RYO), 0, 0x10000, 0);
                vfUpdateFlagsFor(FindCharacterByScenarioName(SYW0_KNK), 0, 0x10000, 0);
                EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{4}, 1);
                qteBattleUtil(BattleQteMode::Load);
                qteBattleUtil(BattleQteMode::SetWait, 10, 7, 60, 0);
                qteBattleUtil(BattleQteMode::SetLastCommands, 64, 1280, 0, 0);
                qteBattleUtil(BattleQteMode::ResetStates);
                qteBattleUtil(BattleQteMode::Start);
                vfmQueueFightStart(2, 99, 1, 0, -1);
                EV_vfBattleUtil(FightBattleUtilOp::CollisionFlags, shen_name{1}, 1);
            }
            std::int32_t camera = Syw0Value<std::int32_t>(&work, 0x5f4);
            if (Syw0Value<std::int32_t>(&work, 0x5b0)) {
                camera = 9012 + (camera == 9012);
                Syw0Value(&work, 0x5b0, std::int32_t{0});
            }
            Syw0Value(&work, 0x5f4, camera);
            SetTrainingCameraSequence(&sequence, camera);
            Syw0Advance(sequence, 102);
            break;
        }
        case 102: {
            const std::uint32_t animation = GetMotionAnimationId(scenFindMotionwork(SYW0_RYO), MotionRegion::LowerBody);
            if (animation - 408U <= 1 || cvsHasValidConverseData(nullptr)) {
                if (cvsHasValidConverseData(nullptr)) {
                    GetConverseDataForChara(SYW0_RYO);
                    GetConverseDataForChara(SYW0_SYE);
                    GetConverseDataForChara(SYW0_KNK);
                }
            } else
                Syw0Advance(sequence, 103);
            break;
        }
        case 103: {
            const std::int32_t result = static_cast<std::int32_t>(qteBattleUtil(BattleQteMode::GetResult));
            if (result == -1) {
                Syw0Advance(sequence, 107);
                break;
            }
            if (result != 0) {
                if (result == 1) {
                    Syw0GateLcd(work);
                    Syw0Wait(sequence, 2, 104, 10);
                    vfmDispatchControl(FightControlOp::QueuedAction, SYW0_RYO, 0x8000048, 1);
                }
                break;
            }
            if ((EV_vfBattleUtil(FightBattleUtilOp::ActorField, SYW0_RYO, 14) & 0x2500) && (EV_vfBattleUtil(FightBattleUtilOp::ActorField, SYW0_KNK, 0) & 2)) {
                const double frame = GetMotionFrame(scenFindMotionwork(SYW0_KNK), MotionRegion::LowerBody);
                const double split = GetMotionSplitFrame(scenFindMotionwork(SYW0_KNK), MotionRegion::LowerBody);
                if (frame > split + 2.0) {
                    qteBattleUtil(BattleQteMode::Start);
                    break;
                }
            }
            Vector3 player, opponent;
            EV_GetCharaPosition(SYW0_RYO, CHARACTER_TRANSLATION_ROOT, &player, EventTransformFlags::World);
            EV_GetCharaPosition(SYW0_KNK, CHARACTER_TRANSLATION_ROOT, &opponent, EventTransformFlags::World);
            const float x = player.x - opponent.x, z = player.z - opponent.z;

            if (static_cast<double>(std::sqrt(x * x + z * z)) > 2.6)
                Syw0Advance(sequence, 107);
            const std::int32_t remaining = Syw0Value<std::int32_t>(&sequence, 0x248);
            if (remaining) {
                Syw0Value(&sequence, 0x248, remaining - 1);
                if (remaining - 1 == 10) {
                    vfmDispatchControl(FightControlOp::ReactionRank, SYW0_RYO, 2, 1);
                    const std::int32_t voice = static_cast<std::int32_t>(GetRandomNumberInRange(3));
                    if (voice >= 0 && voice <= 2)
                        EV_PlayDialogue(voice == 0 ? "0064E005" : voice == 1 ? "0064E006" : "0064E007");
                }
                if (!Syw0Value<std::int32_t>(&sequence, 0x248))
                    vfmDispatchControl(FightControlOp::QueuedAction, SYW0_KNK, 0x916, 1);
            }
            break;
        }
        case 104:
            Syw0GateStopCamera(work);
            Syw0GateResume(work);
            check_dead_status(1);
            Syw0Wait(sequence, 2, 105, 2);
            break;
        case 105:
            QueueExtraTelop("", 99);
            Syw0PrepareAuthorCharacters();
            EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
            Syw0QueueAuthor(SYW0_AUTHORS[5]);
            Syw0Child(sequence, Syw0AwardSequence, SHEN_NAME(TELP), 0);
            sequence.resultSlot = 0;
            Syw0Wait(sequence, 13, 106, 5, 0);
            break;
        case 106:
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 112, 2);
            break;
        case 107: {
            const std::int32_t countdown = Syw0Value<std::int32_t>(&sequence, 0x250);
            if (countdown)
                Syw0Value(&sequence, 0x250, countdown - 1);
            else {
                CompiledSequenceWork *child = Syw0Child(sequence, Syw0GateCorrectionSequence, SHEN_NAME(FS4S), 1);
                const std::int32_t correction = Syw0Value<std::int32_t>(&sequence, 0x24c);
                Syw0Value(child, 0x138, std::int64_t{correction});
                Syw0Value(&sequence, 0x24c, std::int32_t(correction == 0 ? 1 : correction == 1 ? 2 : 0));
                Syw0Value(&sequence, 0x250, std::int32_t{1});
            }
            sequence.waitCharacter = SYW0_RYO;
            Syw0Wait(sequence, 5, 108);
            break;
        }
        case 108:
            Syw0Wait(sequence, 2, 109, 30);
            return;
        case 109:
            Syw0Value(&work, 0x640, std::int32_t{1});
            SuspendFight();
            mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_RYO), 1);
            Syw0Wait(sequence, 2, 110, 5);
            break;
        case 110:
            QueueTrainingReposition(&sequence, 2, SYW0_RYO, 0, &SYW0_GATE_PLAYER_POSITION);
            QueueTrainingReposition(&sequence, 6, SYW0_KNK, 1, &SYW0_GATE_OPPONENT_POSITION);
            Syw0Wait(sequence, 2, 111, 1);
            break;
        case 111:
            if (Syw0Value<std::int32_t>(&work, 0x604) || Syw0Value<std::int32_t>(&work, 0x608))
                return;
            Syw0Value(&work, 0x5b8, std::int32_t{1});
            Syw0Advance(sequence, 101);
            break;
        case 112:
            Syw0GateStopCamera(work);
            Syw0GateResume(work);
            check_dead_status(1);
            Syw0Value(&sequence, 0x88, success);
            sequence.reserved1a0 = 1;
            if (sequence.parent) {
                Syw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
                Syw0Value(sequence.parent, 0xb8 + 4 * sequence.childSlot, success);
            }
            qteBattleUtil(BattleQteMode::RetireAndFree);
            for (CompiledSequenceWork *child : sequence.children)
                if (child) {
                    ClearTrainingSequenceChildren(child);
                    Syw0KillSequence(child);
                }
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            return;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
}
} // namespace shenmue::s2::areas
