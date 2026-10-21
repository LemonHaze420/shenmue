#include "s2/areas/area_mfw0.hpp"
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
std::uint32_t mfw0SplashEffect;
std::uint16_t mfw0SplashFrames;
void Mfw0Nullsub() {
}
namespace {
void Mfw0LoadResources(Mfw0Work &work) {
    if (IsFightActive())
        check_dead_status(1);
    work.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("MFW0"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MFW0"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, MFW0_BASE);
    EV_FreeAuthorPak(1, "0050");
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
void Mfw0Cleanup(Mfw0Work &work) {
    using enum CameraState;
    EV_stepSetFlags(0, 153);
    hndlSetFlags(FindCharacterByScenarioName(MFW0_TKR));
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(MFW0_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(MFW0_RYO), 71, 0, MotionRegion::LowerBody);
    for (std::uint32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0050");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("MFW0"), 0);
    EV_LoadCutsceneFromCurrentScene("AK00");
    DateTime time;
    GetCurrentDateTime(time);
    EV_LoadAudio("AK00_000", time.hour >= 19);
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    EV_NPC_NewLoadSw(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
}
void Mfw0RootCallback(void *data) {
    Mfw0Work &work = *static_cast<Mfw0Work *>(data);
    switch (work.state) {
    case 0:
        work.state = 2;
        ++work.frames;
        break;
    case 2:
        SetLowQualityHands(FindCharacterByScenarioName(MFW0_TKR));
        EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("MFW0"), 0);
        LoadBlurModelForCharacter(MFW0_RYO, "RYO_B.MT7", 1);
        LoadBlurModelForCharacter(MFW0_TKR, "TKR_B.MT7", 1);
        Mfw0Nullsub();
        work.sequence = QueueCompiledSequence(Mfw0TrainingSequence, MFW0_SEQUENCE, &work);
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
            mtwkSetRequestedRootMode(scenFindMotionwork(MFW0_RYO), 2);
            check_dead_status(1);
        }
        FinishLoading();
        ++work.frames;
        work.state = 6;
        break;
    case 6:
        Mfw0Cleanup(work);
        ++work.frames;
        break;
    default:
        ++work.frames;
        break;
    }
}
void InitializeMfw0() {
    mfw0SplashEffect = 0;
    mfw0SplashFrames = 0;
}
void QueueMfw0Event(std::int32_t entry) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Mfw0RootCallback, 4, 0x0b, sizeof(Mfw0Work), MFW0_TASK);
    Mfw0Work &work = *static_cast<Mfw0Work *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.selector = entry;
    work.state = 0;
    work.frames = 0;
    work.fightSuspended = 0;
    work.active = 1;
    work.characters = Queue_EVT_AT_ReadChr(task, MFW0_CHARACTERS);
    Mfw0LoadResources(work);
}
// @verified - p
void RunMfw0(std::uintptr_t entry) {
    QueueMfw0Event(static_cast<std::int32_t>(entry));
}
} // namespace
const ScriptAreaDefinition mfw0ScriptDefinition{.areaId = MFW0_AREA, .field04 = 0, .init = InitializeMfw0, .main = RunMfw0};
} // namespace shenmue::s2::areas
