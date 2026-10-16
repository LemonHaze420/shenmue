#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/osage.hpp"
#include "s2/screen_film.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name SYW0_DEMONSTRATION{SHEN_NAME(TELP)};
void Syw0AlphaOff(void *) {
    EV_SetCharacterAlphaEnabled(SYW0_SYE, false);
    HLib::DestroyCurrentTask();
}
void Syw0QueueAlphaOff() {
    HLib::EnqueueTaskWithoutParameter(Syw0AlphaOff, 5, 0x0b, static_cast<std::int32_t>(SYW0_TASK.name));
}
std::int64_t Syw0SparringAdviceCycle(void *data) {
    constexpr std::uint16_t lines[]{181, 182, 183};
    std::uint16_t &index = *static_cast<std::uint16_t *>(data);
    const std::uint16_t result = lines[index];
    ++index;
    if (index >= 3)
        index = 0;
    return result;
}
void Syw0QueueDemonstrationVoice(const char *name, float delay) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(EVT_AT_SetVoice_Callback, 6, 0x0b, sizeof(EventSetVoice), SYW0_TASK);
    RegisterEventTask(task);
    EventSetVoice &voice = *static_cast<EventSetVoice *>(HLib::GetTaskParameter(task));
    voice.cleanup = 1;
    voice.mode = 1;
    std::strcpy(voice.voice, name);
    voice.delay = delay;
}
void Syw0DemonstrationEyes(std::int16_t frames) {
    Vector3 target;
    EV_GetCharaPosition(SYW0_SYE, 5, &target, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(SYW0_RYO), &target, frames, 2);
    EV_GetCharaPosition(SYW0_RYO, 5, &target, EventTransformFlags::World);
    SetEyeToChara(FindCharacterByScenarioName(SYW0_SYE), &target, frames, 2);
}
void Syw0StartDemonstration(bool second, CompiledSequenceWork &sequence) {
    for (shen_name name : {SYW0_RYO, SYW0_SYE}) {
        EV_SetCharaControllerFlags(name, 1, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(name, 3, NpcCharacterControllerState::Disabled);
    }
    SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 69, 2, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 69, 2, MotionRegion::LowerBody);
    figpAssignModeFor(SYW0_RYO, 0);
    figpAssignModeFor(SYW0_SYE, 0);
    const Vector3 &position = second ? SYW0_DEMONSTRATION_FORCE_POSITION : SYW0_DEMONSTRATION_WEIGHT_POSITION;
    EV_AT_SetPos(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    const Vector3i rotation{.x = 0, .y = 61895, .z = 0};
    EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_RYO), nullptr, 3);
    UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), nullptr, 3);
    UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_RYO), nullptr, 0);
    UpdateMotionWorkWithPosition(scenFindMotionwork(SYW0_SYE), nullptr, 0);
    mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 55, 2);
    mtwkSetExtraFlag(scenFindMotionwork(SYW0_SYE), 55, 2);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(SYW0_RYO), second ? 2322 : 2321, MotionRegion::LowerBody);
    mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), second ? 2324 : 2323, MotionRegion::LowerBody);
    for (shen_name name : {SYW0_RYO, SYW0_SYE}) {
        SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 1, 0);
    }
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(SYW0_RYO), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(SYW0_RYO), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 1);
    static std::uint32_t teacherClosedHand[2]{0, 0x1110};
    static std::uint16_t teacherStrikeHand[]{1999, 0};
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(SYW0_SYE), 0,
        reinterpret_cast<std::byte *>(second ? static_cast<void *>(teacherClosedHand) : static_cast<void *>(teacherStrikeHand)), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(SYW0_SYE), 1,
        reinterpret_cast<std::byte *>(second ? static_cast<void *>(teacherStrikeHand) : static_cast<void *>(teacherClosedHand)), 1);
    toggleOsagFlags(FindCharacterByScenarioName(SYW0_RYO));
    toggleOsagFlags(FindCharacterByScenarioName(SYW0_SYE));
    SetMotionEnvironmentEnabled(1);
    Syw0QueueDemonstrationVoice(second ? "0064B008" : "0064B007", second ? 140.0f : 120.0f);
    Syw0QueueDemonstrationVoice(second ? "0064A007" : "0064A006", second ? 170.0f : 150.0f);
    EventSetCamera camera;
    camera.cleanup = 0;
    camera.camera = second ? 9060 : 9053;
    camera.eye = SYW0_RYO;
    camera.target = SYW0_RYO;
    camera.context = 0;
    camera.reserved11[0] = std::byte{};
    camera.reserved11[1] = std::byte{};
    camera.reserved11[2] = std::byte{};
    camera.angle = 0;
    camera.delay = 0;
    EVT_AT_SetCam_Callback(&camera);
    camCtrlDisableFilmBars();
    EV_SetCinemaBarVisibility(FilmControlFlag::Show, 5);
    Syw0Value(&sequence, 0x1d0, std::int32_t{0});
    sequence.previousMode = sequence.mode;
    sequence.mode = 102;
    sequence.continueLoop = 0;
}
void Syw0FinishDemonstration(bool second) {
    EV_SetCinemaBarVisibility(FilmControlFlag::Hide, 5);
    Vector3i rotation;
    GetScenarioCharacterEulerAngles(SYW0_RYO, &rotation, EventTransformFlags::World);
    rotation.y = second ? 0 : 49152;
    EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    GetScenarioCharacterEulerAngles(SYW0_SYE, &rotation, EventTransformFlags::World);
    rotation.y = second ? 32768 : 16384;
    EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    for (shen_name name : {SYW0_RYO, SYW0_SYE}) {
        SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 0, 0);
        SetHighModelXYZ(FindCharacterByScenarioName(name), 0, 1, 0);
    }
    ResumeFight(1);
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 0);
    mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_RYO), 2);
    mtwkSetRequestedRootMode(scenFindMotionwork(SYW0_SYE), 2);
    camCtrlSetState(CameraState::ChaseGoal);
    Syw0QueueAlphaOff();
}
} // namespace
void Syw0FirstDemonstrationSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            SuspendFight();
            Syw0Wait(sequence, 2, 100, 2);
            break;
        case 100:
            Syw0StartDemonstration(false, sequence);
            break;
        case 102: {
            const std::int32_t frame = Syw0Value<std::int32_t>(&sequence, 0x1d0);
            if (frame == 1) {
                mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 55, 2);
                mtwkSetExtraFlag(scenFindMotionwork(SYW0_SYE), 55, 2);
                Syw0DemonstrationEyes(1);
                SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SYW0_RYO), 1, 5, 1);
            } else if (frame == 82)
                SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SYW0_RYO), 11, 5, 1);
            else if (frame == 110 || frame == 130 || frame == 150)
                Syw0DemonstrationEyes(5);
            else if (frame == 185)
                SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SYW0_RYO), 0, 5, 1);
            if (SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody))
                Syw0Advance(sequence, 103);
            break;
        }
        case 103:
            Syw0FinishDemonstration(false);
            Syw0Advance(sequence, 104);
            break;
        case 104:
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
    Syw0Value(&sequence, 0x1d0, Syw0Value<std::int32_t>(&sequence, 0x1d0) + 1);
}
void Syw0SecondDemonstrationSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0:
            SuspendFight();
            Syw0Wait(sequence, 2, 100, 2);
            break;
        case 100:
            Syw0StartDemonstration(true, sequence);
            break;
        case 102: {
            const std::int32_t frame = Syw0Value<std::int32_t>(&sequence, 0x1d0);
            if (frame == 1) {
                mtwkSetExtraFlag(scenFindMotionwork(SYW0_RYO), 55, 2);
                mtwkSetExtraFlag(scenFindMotionwork(SYW0_SYE), 55, 2);
                Syw0DemonstrationEyes(1);
            } else if (frame == 92) {
                SetEyeToChara(FindCharacterByScenarioName(SYW0_RYO), nullptr, 5, 1);
                SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SYW0_RYO), 11, 5, 1);
            } else if (frame == 150)
                Syw0DemonstrationEyes(5);
            else if (frame == 185)
                SetHighModelAndFaceFlagsForChara(FindCharacterByScenarioName(SYW0_RYO), 0, 5, 1);
            if (SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody))
                Syw0Advance(sequence, 103);
            break;
        }
        case 103:
            Syw0FinishDemonstration(true);
            Syw0Advance(sequence, 104);
            break;
        case 104:
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
    Syw0Value(&sequence, 0x1d0, Syw0Value<std::int32_t>(&sequence, 0x1d0) + 1);
}
void Syw0SparringSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            const Vector3 eye{.x = 335.39011f, .y = 128.05009f, .z = 235.29111f}, target{.x = 331.73181f, .y = 127.8009f, .z = 233.3663f};
            camCtrlSetState(CameraState::Event);
            camCtrlSetNextPosition(eye);
            camCtrlSetTargetPosition(target);
            EV_EnableCharacterDisplay(SYW0_RYO);
            EV_EnableCharacterDisplay(SYW0_SYE);
            EV_DisableCharacterDisplay(SYW0_KNK);
            Syw0Advance(sequence, 100);
            break;
        }
        case 100: {
            Vector3i rotation;
            Vector3 position;
            GetScenarioCharacterEulerAngles(SYW0_RYO, &rotation, EventTransformFlags::MotionYaw);
            EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::AllAxes, rotation);
            GetScenarioCharacterEulerAngles(SYW0_SYE, &rotation, EventTransformFlags::MotionYaw);
            EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::AllAxes, rotation);
            EV_GetCharaPosition(SYW0_RYO, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(SYW0_RYO, &rotation, EventTransformFlags::World);
            AT_SetLocalBaseChara(SYW0_RYO, SYW0_INVALID);
            EV_AT_SetPos(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            GetScenarioCharacterEulerAngles(SYW0_SYE, &rotation, EventTransformFlags::World);
            EV_GetCharaPosition(SYW0_SYE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            AT_SetLocalBaseChara(SYW0_SYE, SYW0_INVALID);
            EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
            EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
            const Vector3i teacher{.x = 0, .y = 32768, .z = 0};
            EV_AT_SetAngY(SYW0_RYO, EventTransformFlags::AllAxes, trainingSparRotation);
            EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::AllAxes, teacher);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
            SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_RYO), 306, MotionRegion::LowerBody);
            mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
            Syw0Advance(sequence, 101);
            break;
        }
        case 101: {
            Syw0Value(&sequence, 0x1d0, std::int32_t{0});
            Syw0Value(&sequence, 0x1d8, std::int32_t{0});
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            shenStartFGST(10, 10, 0, 0);
            vfmDispatchControl(FightControlOp::Package, shen_name{}, reinterpret_cast<std::uintptr_t>("SYW0"), 1);
            shenInitFightData(FindCharacterByScenarioName(SYW0_RYO), nullptr, 0, 0, 0, 0);
            shenInitFightData(FindCharacterByScenarioName(SYW0_SYE), nullptr, 1, 1, 20000, 15);
            vfCopyPosFromEntity(0, nullptr, 331.9f, 235.3f, 3.3f);
            vfmQueueFightStart(2, 99, 1, 0, -1);
            AssignBattleDialogueCallback(AllocateBattleDialogueCondition13(12, -1, SYW0_RYO, shen_name{}, 150, 100, 1), Syw0SparringAdviceCycle);
            Syw0QueueAlphaOff();
            Syw0Advance(sequence, 103);
            break;
        }
        case 102: {
            if (!IsFightActive()) {
                Syw0Advance(sequence, 114);
                break;
            }
            const std::int32_t phase = Syw0Value<std::int32_t>(&sequence, 0x1d8);
            if (phase == 0 || phase == 2)
                Syw0Value(&sequence, 0x1d8, phase + 1);
            else if (phase == 1 || phase == 3) {
                if (GetMotionAnimationId(scenFindMotionwork(SYW0_SYE), MotionRegion::LowerBody) == (phase == 1 ? 2323 : 2324) &&
                    SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 65, 1, MotionRegion::LowerBody))
                    Syw0Value(&sequence, 0x1d8, phase + 1);
            } else if (phase == 4 && Syw0Value<std::int32_t>(&sequence, 0x1d0) > 1500) {
                check_dead_status(1);
                Syw0Value(&sequence, 0x1d8, phase + 1);
                Syw0Advance(sequence, 113);
            }
            break;
        }
        case 103:
            Syw0Value(&sequence, 0x1d0, std::int32_t{0});
            Syw0Wait(sequence, 2, 104, 600);
            break;
        case 104:
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
            Syw0Advance(sequence, 105);
            break;
        case 105:
            Syw0Wait(sequence, 2, 106, 1);
            break;
        case 106:
            Syw0Child(sequence, Syw0FirstDemonstrationSequence, SYW0_DEMONSTRATION, 0);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 107);
            SetQteCameraAtTop(1);
            break;
        case 107:
            Syw0Value(&sequence, 0x1d0, std::int32_t{0});
            Syw0Wait(sequence, 2, 108, 210);
            SetQteCameraAtTop(0);
            break;
        case 108:
            EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{1}, 1);
            Syw0Advance(sequence, 109);
            break;
        case 109:
            Syw0Wait(sequence, 2, 110, 1);
            break;
        case 110:
            Syw0Child(sequence, Syw0SecondDemonstrationSequence, SYW0_DEMONSTRATION, 0);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 111);
            SetQteCameraAtTop(1);
            break;
        case 111:
            Syw0Wait(sequence, 2, 112, 150);
            SetQteCameraAtTop(0);
            break;
        case 112:
            check_dead_status(1);
            Syw0Advance(sequence, 113);
            break;
        case 113:
            if (!IsFightActive())
                Syw0Advance(sequence, 114);
            break;
        case 114:
            SetScreenFilmFade(5, 0, 0, 0, 0, 255, 0, 0, 0);
            Syw0Wait(sequence, 11, 115, 2);
            break;
        case 115:
            AT_SetLocalBaseChara(SYW0_RYO, SYW0_BASE);
            AT_SetLocalBaseChara(SYW0_SYE, SYW0_BASE);
            figpAssignModeFor(SYW0_RYO, 0);
            figpAssignModeFor(SYW0_SYE, 0);
            RetireCompiledSequenceLinks(&sequence);
            HLib::DestroyCurrentTask();
            break;
        default:
            UpdateCompiledSequenceWait(&sequence);
            break;
        }
    } while (sequence.continueLoop);
    Syw0Value(&sequence, 0x1d0, Syw0Value<std::int32_t>(&sequence, 0x1d0) + 1);
    Syw0Value(&sequence, 0x1d4, Syw0Value<std::int32_t>(&sequence, 0x1d4) + 1);
}
} // namespace shenmue::s2::areas
