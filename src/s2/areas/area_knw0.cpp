#include "s2/areas/area_knw0.hpp"
#include "s2/areas/knw0_authored.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/hand.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
std::int32_t knw0AdviceShown{};
namespace {
std::uint64_t knw0RetainedMotionTimes[8]{};
void Knw0Nullsub() {
}
void Knw0PrepareHands(shen_name character) {
    Character *actor = FindCharacterByScenarioName(character);
    SetHighModelXYZ(actor, 1, 1, 0);
    SetHighModelXYZ(actor, 1, 0, 0);
    ApplyCharacterHandPoseCommand(actor, 1, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
    ApplyCharacterHandPoseCommand(actor, 0, reinterpret_cast<std::byte *>(chestHandPoseCommand), 1);
}
void Knw0LoadResources(Knw0Work &work) {
    if (IsFightActive())
        check_dead_status(1);
    work.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("KNW0"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KNW0"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SHEN_NAME(BASE));
    EV_FreeAuthorPak(1, "0069");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0069A000");
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
void Knw0Cleanup(Knw0Work &work) {
    using enum CameraState;
    EV_stepSetFlags(0, 0xc4);
    hndlSetFlags(FindCharacterByScenarioName(KNW0_RYO));
    hndlSetFlags(FindCharacterByScenarioName(KNW0_KNK));
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(SHEN_NAME(BASE), 1);
    SetMotionRegionFlag(scenFindMotionwork(KNW0_RYO), 71, 0, MotionRegion::LowerBody);
    for (std::uint32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0069");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("KNW0"), 0);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
}
void Knw0RootCallback(void *data) {
    Knw0Work &work = *static_cast<Knw0Work *>(data);
    switch (work.state) {
    case 0:
        work.state = 2;
        ++work.frames;
        break;
    case 2:
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("KNW0"), 0);
        LoadBlurModelForCharacter(KNW0_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(KNW0_KNK, "KNK_B.MT7", 1);
        SetLowQualityHands(FindCharacterByScenarioName(KNW0_RYO));
        SetLowQualityHands(FindCharacterByScenarioName(KNW0_KNK));
        Knw0PrepareHands(KNW0_RYO);
        Knw0PrepareHands(KNW0_KNK);
        Knw0Nullsub();
        work.sequence = QueueCompiledSequence(Knw0RootSequence, SHEN_NAME(SEQP), &work);
        ++work.frames;
        work.state = 3;
        break;
    case 3:
        if (!work.active)
            work.state = 4;
        ++work.frames;
        break;
    case 4:
        work.state = 5;
        ++work.frames;
        break;
    case 5:
        if (work.fightSuspended) {
            work.fightSuspended = 0;
            ResumeFight(1);
            mtwkSetRequestedRootMode(scenFindMotionwork(KNW0_RYO), 2);
            check_dead_status(1);
        }
        FinishLoading();
        ++work.frames;
        work.state = 6;
        break;
    case 6:
        Knw0Cleanup(work);
        ++work.frames;
        break;
    default:
        ++work.frames;
        break;
    }
}
void InitializeKnw0() {
    for (std::uint64_t &time : knw0RetainedMotionTimes)
        time = 0;
    knw0AdviceShown = 0;
}
void RunKnw0(std::uintptr_t entry) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Knw0RootCallback, 4, 0x0b, sizeof(Knw0Work), KNW0_TASK);
    Knw0Work &work = *static_cast<Knw0Work *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.entryPoint = static_cast<std::int32_t>(entry);
    work.state = 0;
    work.frames = 0;
    work.fightSuspended = 0;
    work.active = 1;
    work.characters = Queue_EVT_AT_ReadChr(task, KNW0_CHARACTERS);
    Knw0LoadResources(work);
}
} // namespace
const ScriptAreaDefinition knw0ScriptDefinition{.areaId = KNW0_AREA, .field04 = 0, .init = InitializeKnw0, .main = RunKnw0};
} // namespace shenmue::s2::areas
