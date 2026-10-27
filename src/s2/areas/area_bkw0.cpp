#include "s2/areas/bkw0_sequence.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/debug.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/step_event.hpp"
#include "s2/training_seqp.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
const shen_name bkw0TrainingCharacters[]{BKW0_RYO, BKW0_BKN, BKW0_LKN, BKW0_MYS, BKW0_CROWD[0], BKW0_CROWD[1], BKW0_CROWD[2], BKW0_CROWD[3], BKW0_CROWD[4],
    BKW0_CROWD[5], BKW0_CROWD[6], BKW0_CROWD[7], BKW0_CROWD[8], BKW0_CROWD[9], BKW0_CROWD[10], BKW0_CROWD[11], BKW0_CROWD[12], BKW0_CROWD[13], BKW0_CROWD[14],
    BKW0_CROWD[15], BKW0_CROWD[16], BKW0_CROWD[17], EVENT_CHARACTER_INVALID};
const shen_name bkw0BowCharacters[]{BKW0_RYO, BKW0_MYS, BKW0_CROWD[0], BKW0_CROWD[1], BKW0_CROWD[2], BKW0_CROWD[3], BKW0_CROWD[4], BKW0_CROWD[5], BKW0_CROWD[6],
    BKW0_CROWD[7], BKW0_CROWD[8], BKW0_CROWD[9], BKW0_CROWD[10], BKW0_CROWD[11], BKW0_CROWD[12], BKW0_CROWD[13], BKW0_CROWD[14], BKW0_CROWD[15], BKW0_CROWD[16],
    BKW0_CROWD[17], EVENT_CHARACTER_INVALID};
const shen_name *bkw0Characters;
std::int32_t bkw0CrowdOffset, bkw0AdviceIssued;
std::uint32_t bkw0Effects[16];
namespace {
void Bkw0SetupNullsub() {
}
bool Bkw0IsZero(std::int32_t value) {
    return value == 0;
}
void Bkw0RaiseHands(shen_name name) {
    SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 0, 0);
    SetHighModelXYZ(FindCharacterByScenarioName(name), 1, 1, 0);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(name), 0, reinterpret_cast<std::byte *>(openHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(FindCharacterByScenarioName(name), 1, reinterpret_cast<std::byte *>(openHandPoseCommand), 1);
}
void Bkw0Retire(Bkw0Work &work) {
    const std::int32_t selector = work.selector;
    if (!selector) {
        EV_stepSetFlags(0, 232);
        hndlSetFlags(FindCharacterByScenarioName(BKW0_RYO));
        hndlSetFlags(FindCharacterByScenarioName(BKW0_BKN));
        hndlSetFlags(FindCharacterByScenarioName(BKW0_LKN));
    } else if (selector == 1)
        hndlSetFlags(FindCharacterByScenarioName(BKW0_RYO));
    if (work.characters)
        ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(BKW0_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(BKW0_RYO), 71, 0, MotionRegion::LowerBody);
    for (std::uint32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0068");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("BKW0"), 0);
    if (!work.selector) {
        LoadCutsceneAndSoundProgramForArea();
        EV_spmBeginSampleExecute();
    }
    shenSetClockAndCompassRenderTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_scenQueueLoad(g_actNumber, BKW0_RETURN_AREA, selector ? 36 : 37);
}
void Bkw0RootCallback(void *data) {
    Bkw0Work &work = *static_cast<Bkw0Work *>(data);
    switch (work.state) {
    case 0:
        if (!work.selector) {
            SetLowQualityHands(FindCharacterByScenarioName(BKW0_RYO));
            SetLowQualityHands(FindCharacterByScenarioName(BKW0_BKN));
            SetLowQualityHands(FindCharacterByScenarioName(BKW0_LKN));
            Bkw0RaiseHands(BKW0_RYO);
            Bkw0RaiseHands(BKW0_BKN);
            Bkw0RaiseHands(BKW0_LKN);
        } else if (work.selector == 1) {
            SetLowQualityHands(FindCharacterByScenarioName(BKW0_RYO));
            Bkw0RaiseHands(BKW0_RYO);
        }
        work.state = 3;
        break;
    case 1:
        if (EV_npcStreamOrLoadHuman(bkw0Characters[work.characterIndex])) {
            ++work.characterIndex;
            work.state = 2;
        }
        break;
    case 2:
        if (bkw0Characters[work.characterIndex].name == EVENT_CHARACTER_INVALID.name) {
            work.state = 3;
            break;
        }
        while (DoesScenarioCharacterExist(bkw0Characters[work.characterIndex])) {
            ++work.characterIndex;
            work.characterLoading = 0;
            if (bkw0Characters[work.characterIndex].name == EVENT_CHARACTER_INVALID.name) {
                work.state = 3;
                break;
            }
        }
        if (work.state != 3) {
            work.characterLoading = 1;
            work.state = 1;
        }
        break;
    case 3:
        SetWeatherUpdateEnabledTo(1);
        RefreshWeatherForCurrentTime(0);
        EV_UpdateWeather();
        Bkw0CreateSmoke();
        Bkw0SetupNullsub();
        if (!work.selector) {
            EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BKW0"), 0);
            LoadBlurModelForCharacter(BKW0_RYO, "RYO_B.MT7", 1);
            LoadBlurModelForCharacter(BKW0_LKN, nullptr, 1);
            work.sequence = QueueCompiledSequence(Bkw0TrainingSequence, BKW0_SEQUENCE, &work);
        } else if (work.selector == 1)
            work.sequence = QueueCompiledSequence(Bkw0BowSequence, BKW0_SEQUENCE, &work);
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
        if (work.fightSuspended) {
            RetireTrainingFight(&work, BKW0_RYO);
            check_dead_status(1);
        }
        EV_FreeAuthorPak(0, "0068");
        for (std::uint32_t *motion = work.motions; *motion; ++motion)
            DeleteMotion(*motion);
        for (std::int32_t i = 0; i < 2; ++i)
            FreeParticleEffect(0, bkw0Effects[i]);
        FinishLoading();
        work.state = 7;
        break;
    case 7:
        Bkw0Retire(work);
        break;
    default:
        break;
    }
    ++work.frames;
}
void Bkw0LoadResources(Bkw0Work &work) {
    work.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("BKW0"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("BKW0"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, BKW0_BASE);
    EV_FreeAuthorPak(1, "0068");
    if (!work.selector) {
        EV_SetNewSceneIndex();
        EV_LoadCutsceneFromCurrentScene("0068A000");
        EV_spmBeginSampleExecute();
    }
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    Bkw0IsZero(1);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    FinishLoading();
    shenSetClockAndCompassRenderTo(1);
}
} // namespace
void InitializeBkw0Event() {
    for (std::uint32_t &effect : bkw0Effects)
        effect = 0;
    bkw0Characters = nullptr;
    bkw0CrowdOffset = 0;
    bkw0AdviceIssued = 0;
}
namespace {
void QueueBkw0Event(std::int32_t selector) {
    EV_debug_log("BKW0\\eBKW0_et_bukan.c", 290, "select %d\n", selector);
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Bkw0RootCallback, 4, 0x0b, sizeof(Bkw0Work), BKW0_TASK);
    Bkw0Work &work = *static_cast<Bkw0Work *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.state = 0;
    work.frames = 0;
    work.active = 1;
    work.fightSuspended = 0;
    work.opponent = BKW0_LKN;
    work.selector = selector;
    if (!selector) {
        bkw0Characters = bkw0TrainingCharacters;
        bkw0CrowdOffset = 4;
    } else if (selector == 1) {
        bkw0Characters = bkw0BowCharacters;
        bkw0CrowdOffset = 2;
    }
    work.characters = nullptr;
    work.characters = Queue_EVT_AT_ReadChr(task, bkw0Characters);
    Bkw0LoadResources(work);
}
} // namespace
void RunBkw0Event(std::uintptr_t) {
    QueueBkw0Event(g_entryPoint);
}
const ScriptAreaDefinition bkw0ScriptDefinition{.areaId = BKW0_AREA, .field04 = 0, .init = InitializeBkw0Event, .main = RunBkw0Event};
} // namespace shenmue::s2::areas
