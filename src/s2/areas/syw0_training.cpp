#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/advice.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/pak.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/motion_work.hpp"
#include "s2/particle_effects.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/training_seqp.hpp"
#include "s2/ui.hpp"
namespace shenmue::s2::areas {
CompiledSequenceWork *Syw0Child(CompiledSequenceWork &sequence, HLib::TaskCallback callback, shen_name token, std::int32_t slot) {
    CompiledSequenceWork *child = QueueCompiledSequence(callback, token, sequence.event);
    if (!sequence.children[slot])
        sequence.children[slot] = child;
    child->parent = &sequence;
    child->childSlot = slot;
    child->reference = sequence.reference + 1;
    Syw0Value(&sequence, 0x19c, slot);
    Syw0Value(&sequence, 0x90 + 4 * slot, std::int32_t{0});
    return child;
}
void Syw0Complete(CompiledSequenceWork &sequence, bool result) {
    if (sequence.parent) {
        Syw0Value(sequence.parent, 0x90 + 4 * sequence.childSlot, std::int32_t{1});
        if (result)
            Syw0Value(sequence.parent, 0xb8 + 4 * sequence.childSlot, Syw0Value<std::int32_t>(&sequence, 0x88));
    }
    RetireCompiledSequenceLinks(&sequence);
    HLib::DestroyCurrentTask();
}
void Syw0KillSequence(CompiledSequenceWork *sequence) {
    if (sequence && sequence->task) {
        HLib::Task *task = sequence->task;
        RetireCompiledSequenceLinks(sequence);
        HLib::DestroyTask(task);
    }
}
namespace {
inline constexpr shen_name SYW0_LCD{SHEN_NAME(DLCD)}, SYW0_SPAR{SHEN_NAME(BSBT)}, SYW0_PRACTICE{SHEN_NAME(WDMA)};
inline constexpr shen_name SYW0_GATE{SHEN_NAME(WQTE)}, SYW0_AWARD{SHEN_NAME(TELP)};
void Syw0DetachActor(shen_name name) {
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::World);
    AT_SetLocalBaseChara(name, SYW0_INVALID);
    EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void Syw0LoadStagePackage(const char *name) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(name));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(name), 0);
}
void Syw0StopLcd(CompiledSequenceWork &sequence, Syw0Work &work) {
    if (Syw0Value<CompiledSequenceWork *>(&work, 0x5e0)) {
        Syw0KillSequence(sequence.children[1]);
        Syw0Value(&work, 0x5e0, static_cast<CompiledSequenceWork *>(nullptr));
        LcdmInit(0);
        CheckHelpIcon(2048, 2048, CheckHelpIconOp::Remove);
    }
}
void Syw0RestoreTeacher(const Vector3 &position, std::int32_t yaw) {
    EV_AT_SetPos(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    const Vector3i rotation{.x = 0, .y = yaw, .z = 0};
    EV_AT_SetAngY(SYW0_SYE, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    mtwkRequestMotion(scenFindMotionwork(SYW0_SYE), 8841, MotionRegion::LowerBody);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_SYE), 71, 0, MotionRegion::LowerBody);
}
} // namespace
void Syw0TrainingSequence(void *data) {
    CompiledSequenceWork &sequence = *static_cast<CompiledSequenceWork *>(data);
    Syw0Work &work = *static_cast<Syw0Work *>(sequence.event);
    sequence.callbackFlag = 0;
    do {
        sequence.continueLoop = 0;
        switch (sequence.mode) {
        case 0: {
            Syw0Value(&work, 0x640, std::int32_t{0});
            Syw0Value(&work, 0x5e0, static_cast<CompiledSequenceWork *>(nullptr));
            Syw0Value(&work, 0x558, std::int32_t{0});
            Syw0Value(&work, 0x560, std::int32_t{0});
            Syw0Value(&work, 0x65c, std::int32_t{0});
            Syw0Value(&work, 0x74, std::int8_t{0});
            Syw0Value(&work, 0x680, std::int8_t{0});
            Syw0Value(&work, 0x5a8, std::int32_t{0});
            Syw0Value(&work, 0x5f8, std::int32_t{0});
            Syw0Value(&work, 0x5b4, std::int32_t{0});
            Syw0Value(&work, 0x5bc, std::int16_t{0});
            Syw0Value(&work, 0x651, std::uint8_t{0});
            for (std::int32_t i = 0; i < 10; ++i) {
                Syw0Value(&work, 0x558 + 4 * i, std::int32_t{0});
                Syw0Value(&work, 0x580 + 4 * i, std::int32_t{0});
            }
            Syw0Value(&work, 0x5e0, Syw0Child(sequence, CompiledLcdCountdownCallback, SYW0_LCD, 1));
            Syw0CreateSmoke();
            Syw0LoadStagePackage("WB00");
            EV_SetObjectFlagByIndex(2, 0);
            EV_SetObjectFlagByIndex(5, 0);
            PushGlobalLightingProgram();
            EV_LoadAudio("0064A000", 0);
            if (work.selector == 98 || work.selector == 99)
                SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            Syw0Advance(sequence, work.selector == 98 ? 108 : work.selector == 99 ? 112 : 100);
            break;
        }
        case 100:
            Syw0Advance(sequence, 102);
            break;
        case 102:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[0]);
            Syw0Wait(sequence, 13, 103, 5, 0);
            break;
        case 103:
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WB00"), 0);
            Syw0RestoreTrainingCharacters();
            EV_SetCinemaBarVisibility(FilmControlFlag::Show, 1);
            Syw0Wait(sequence, 2, 104, 32);
            break;
        case 104:
            UpdateWeatherForMinute(1, 0, 0, 0);
            Syw0LoadStagePackage("WBK1");
            PushGlobalLightingProgram();
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[6]);
            Syw0Wait(sequence, 13, 105, 5, 0);
            break;
        case 105:
            UpdateWeatherForMinute(1, 1, 0, 0);
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WBK1"), 0);
            for (std::int32_t i = 0; i < 5; ++i)
                hndlSetFlags(FindCharacterByScenarioName(SYW0_CHARACTERS[i]));
            EV_SetCinemaBarVisibility(FilmControlFlag::Show, 1);
            Syw0LoadStagePackage("WB00");
            EV_SetObjectFlagByIndex(2, 0);
            EV_SetObjectFlagByIndex(5, 0);
            PushGlobalLightingProgram();
            for (std::int32_t i = 0; i < 5; ++i)
                SetLowQualityHands(FindCharacterByScenarioName(SYW0_CHARACTERS[i]));
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 106, 32);
            break;
        case 106:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[1]);
            Syw0Wait(sequence, 13, 107, 5, 0);
            break;
        case 107:
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 108, 1);
            break;
        case 108:
            Syw0Child(sequence, Syw0SparringSequence, SYW0_SPAR, 0);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 109);
            break;
        case 109:
            Syw0Advance(sequence, 110);
            break;
        case 110:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[2]);
            Syw0Wait(sequence, 13, 111, 5, 0);
            break;
        case 111:
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 112, 1);
            break;
        case 112:
            Syw0DetachActor(SYW0_RYO);
            Syw0DetachActor(SYW0_SYE);
            Syw0DetachActor(SYW0_KNK);
            EV_EnableCharacterDisplay(SYW0_RYO);
            EV_EnableCharacterDisplay(SYW0_SYE);
            EV_DisableCharacterDisplay(SYW0_KNK);
            Syw0RestoreTeacher(SYW0_TEACHER_WEIGHT_POSITION, 32768);
            Syw0Advance(sequence, 114);
            break;
        case 113:
            Syw0Advance(sequence, 120);
            break;
        case 114:
        case 115:
        case 116: {
            const std::int32_t mode = sequence.mode;
            CompiledSequenceWork *child = Syw0Child(sequence, Syw0PracticeDecisionSequence, SYW0_PRACTICE, 0);
            Syw0Value(child, 0x138, std::int64_t(mode - 114));
            Syw0Value(child, 0x140, std::int64_t(mode - 109));
            Syw0Value(child, 0x148, std::int64_t{-1});
            Syw0Value(child, 0x150, std::int64_t(mode == 114 ? 115 : mode == 115 ? 116 : 118));
            Syw0Value(child, 0x158, std::int64_t(mode == 114 ? 116 : mode == 115 ? 116 : 118));
            Syw0Value(child, 0x160, std::int64_t(mode));
            Syw0Value(child, 0x168, std::int64_t{113});
            Syw0Value(child, 0x170, std::int64_t(mode == 116 ? 117 : 126));
            Syw0RestoreTeacher(mode == 116 ? SYW0_TEACHER_FORCE_POSITION : SYW0_TEACHER_WEIGHT_POSITION, mode == 116 ? 49152 : 32768);
            sequence.mode = 9;
            sequence.nextMode = 0;
            sequence.resultSlot = 0;
            sequence.continueAfterWait = 1;
            break;
        }
        case 117: {
            QueueFightAdviceControl(18);
            CompiledSequenceWork *child = Syw0Child(sequence, Syw0GateSequence, SYW0_GATE, 0);
            Syw0Value(child, 0x138, std::int64_t{118});
            sequence.mode = 9;
            sequence.nextMode = 0;
            sequence.resultSlot = 0;
            sequence.continueAfterWait = 1;
            break;
        }
        case 118:
            Syw0Advance(sequence, 119);
            break;
        case 119:
            if (Syw0Value<std::int32_t>(&work, 0x640)) {
                RetireTrainingFight(&work, SYW0_RYO);
                check_dead_status(1);
            }
            AT_SetLocalBaseChara(SYW0_RYO, SYW0_BASE);
            AT_SetLocalBaseChara(SYW0_KNK, SYW0_BASE);
            AT_SetLocalBaseChara(SYW0_SYE, SYW0_BASE);
            ProcessAicaCommand(0xaa0, 120);
            Syw0Advance(sequence, 127);
            break;
        case 120:
            Syw0PrepareAuthorCharacters();
            Syw0QueueAuthor(SYW0_AUTHORS[3]);
            Syw0Wait(sequence, 13, 121, 5, 0);
            break;
        case 121:
            Syw0RestoreTrainingCharacters();
            Syw0Wait(sequence, 2, 114, 32);
            break;
        case 126:
            Syw0Child(sequence, Syw0AwardSequence, SYW0_AWARD, 0);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 127);
            break;
        case 127:
            if (static_cast<std::uint32_t>(work.selector) - 98U <= 1) {
                Syw0StopLcd(sequence, work);
                Syw0Advance(sequence, 0);
            } else if (syw0SavedDateTime.hour < 19 && EV_GetShenmueFlag(0x431))
                Syw0Advance(sequence, 128);
            else {
                DateTime time;
                GetCurrentDateTime(time);
                ++time.hour;
                SetCurrentDateTimeTo(time);
                Syw0Advance(sequence, 129);
            }
            break;
        case 128:
            Syw0Child(sequence, Syw0NightSequence, SYW0_NIGHT, 0);
            sequence.waitChildIndex = 0;
            Syw0Wait(sequence, 10, 129);
            break;
        case 129:
            for (std::int32_t i = 0; i < 2; ++i)
                FreeParticleEffect(0, static_cast<std::uint32_t>(syw0SmokeEffects[0] >> (32 * i)));
            work.active = 0;
            Syw0StopLcd(sequence, work);
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
