#include "s2/areas/area_syw0.hpp"
#include "s2/areas/syw0_tables.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/extra_telop.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/particle_effects.hpp"
#include "s2/pause.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/step_event.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
namespace shenmue::s2::areas {
std::uint64_t syw0SmokeEffects[8];
std::uint32_t syw0FireEffect;
DateTime syw0SavedDateTime;
namespace {
std::byte syw0SavedDateTimePadding[3];
void Syw0SetupCharacters() {
    EV_EnableCharacterDisplay(SYW0_RYO);
    EV_EnableCharacterDisplay(SYW0_SYE);
    EV_DisableCharacterDisplay(SYW0_KNK);
    EventSetPosition position{.cleanup = 0, .character = SYW0_BASE, .parent = SYW0_INVALID, .position = SYW0_BASE_POSITION, .delay = 0};
    EVT_AT_SetPosRot(&position);
    EventSetAngle angle{.cleanup = 0, .character = SYW0_BASE, .parent = SYW0_INVALID, .degrees = 0, .delay = 0};
    EVT_AT_SetAngY_Callback(&angle);
    AT_SetLocalBaseChara(SYW0_RYO, SYW0_BASE);
    AT_SetLocalBaseChara(SYW0_SYE, SYW0_BASE);
    AT_SetLocalBaseChara(SYW0_KNK, SYW0_BASE);
    EventSetMotion motion{.cleanup = 0, .character = SYW0_RYO, .animation = 306, .frame = 0, .speed = 1, .flags = 0x40000000U, .delay = 0};
    EVT_AT_SetMot_Callback(&motion);
    GetMotionFrameCount(306);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SYW0"), 0);
    LoadBlurModelForCharacter(SYW0_RYO, "RYO_B.MT7", 1);
    LoadBlurModelForCharacter(SYW0_SYE, "SYE_B.MT7", 1);
    LoadBlurModelForCharacter(SYW0_KNK, "KNK_B.MT7", 1);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_RYO), 1);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_SYE), 1);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_KNK), 1);
    const Vector3i left{.x = 0, .y = 16384, .z = 0};
    constexpr shen_name leftProp{SHEN_NAME(dr1L)}, rightProp{SHEN_NAME(dr1R)};
    EV_AT_SetAngY(leftProp, EventTransformFlags::AllAxes, left);
    Vector3i right;
    right.x = 0;
    right.y = 49152;
    right.z = -1;
    EV_AT_SetAngY(rightProp, EventTransformFlags::AllAxes, right);
}
void Syw0LoadResources(Syw0Work &work) {
    if (IsFightActive())
        check_dead_status(1);
    work.motions[0] = 0;
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("SYW0"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("SYW0"), 0);
    CreateEventCharacter(CharacterCreationHighModel, nullptr, SYW0_BASE);
    EV_FreeAuthorPak(1, "0064");
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0064A000");
    flddSetPointFlagOnGrid(SYW0_GRID, 0);
    flddSetPointFlagOnGrid(SYW0_AREA, 1);
    SetMapControlStatusFlag(0, 1);
    SetWeatherUpdateEnabledTo(1);
    RefreshWeatherForCurrentTime(0);
    EV_UpdateWeather();
    Syw0CreateFire();
    if (syw0SavedDateTime.hour < 19) {
        CheckIsArgEquZero(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(4, 0);
    } else {
        EV_lightLoad(SYW0_LIGHT);
        PushGlobalLightingProgram();
        CheckIsArgEquZero(1);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(4, 1);
        ResumeParticleEffect(4, syw0FireEffect);
    }
    ApplyWeatherRainAndRippleClip(SYW0_RAIN_CLIP);
    ApplyWeatherParticleClip(SYW0_PARTICLE_CLIP);
    FinishLoading();
    SetRenderClockAndCompassTo(1);
}
void Syw0Cleanup(Syw0Work &work) {
    EV_stepSetFlags(0, 133);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_RYO), 0);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_SYE), 0);
    SetPluralControlFor(FindCharacterByScenarioName(SYW0_KNK), 0);
    hndlSetFlags(FindCharacterByScenarioName(SYW0_RYO));
    hndlSetFlags(FindCharacterByScenarioName(SYW0_SYE));
    hndlSetFlags(FindCharacterByScenarioName(SYW0_KNK));
    hndlSetFlags(FindCharacterByScenarioName(SYW0_KSY));
    hndlSetFlags(FindCharacterByScenarioName(SYW0_KSM));
    ET_AT_KillWork(work.characters);
    SetPlayerControlEnabled(0);
    SetCharaKill(SYW0_BASE, 1);
    SetMotionRegionFlag(scenFindMotionwork(SYW0_RYO), 71, 0, MotionRegion::LowerBody);
    if (syw0FireEffect) {
        FreeParticleEffect(4, syw0FireEffect);
        syw0FireEffect = 0;
    }
    flddSetPointFlagOnGrid(SYW0_AREA, 0);
    flddSetPointFlagOnGrid(SYW0_GRID, 1);
    for (std::uint32_t *motion = work.motions; *motion; ++motion)
        DeleteMotion(*motion);
    EV_FreeAuthorPak(0, "0064");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("SYW0"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("WB00"), 0);
    shenSetTimeSimulationTo(1);
    LoadCutsceneAndSoundProgramForArea();
    SetRenderClockAndCompassTo(0);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work.task);
    for (std::int32_t stat = 16; stat <= 31; ++stat)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, static_cast<std::int16_t>(stat), 85);
    if (EV_GetShenmueFlag(0x431))
        EV_scenQueueLoad(g_actNumber, SYW0_RETURN, 0);
    else {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour >= 8)
            ++time.day;
        time.hour = 8;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
        EV_stepSetFlags(0, 100);
        SetSceneInfos(3, SYW0_NEXT_SCENE, 0);
        StartShowScreenOfId(4);
    }
}
void Syw0RootCallback(void *data) {
    Syw0Work &work = *static_cast<Syw0Work *>(data);
    switch (work.state) {
    case 0:
        for (std::int32_t i = 0; i < 5; ++i)
            SetLowQualityHands(FindCharacterByScenarioName(SYW0_CHARACTERS[i]));
        QueueExtraTelop("", 99);
        Syw0SetupCharacters();
        work.sequence = QueueCompiledSequence(Syw0TrainingSequence, SYW0_SEQUENCE, &work);
        work.active = 1;
        work.state = 1;
        break;
    case 1:
        work.state = 2;
        break;
    case 2:
        if (!work.active)
            work.state = 3;
        break;
    case 3:
        work.state = 4;
        break;
    case 4:
        work.state = 5;
        break;
    case 5:
        Syw0Cleanup(work);
        break;
    default:
        break;
    }
}
void InitializeSyw0() {
    for (std::uint64_t &effect : syw0SmokeEffects)
        effect = 0;
    syw0FireEffect = 0;
    std::memset(&syw0SavedDateTime, 0, sizeof(syw0SavedDateTime));
    std::memset(syw0SavedDateTimePadding, 0, sizeof(syw0SavedDateTimePadding));
}
void QueueSyw0Event(std::int32_t entry) {
    SetPlayerControlEnabled(0);
    SetScreenFilmFade(1, 0, 0, 0, 0, 255, 0, 0, 0);
    GetCurrentDateTime(syw0SavedDateTime);
    shenSetTimeSimulationTo(0);
    HLib::Task *task = HLib::EnqueueTaskWithParameter(Syw0RootCallback, 4, 0x0b, sizeof(Syw0Work), SYW0_TASK);
    Syw0Work &work = *static_cast<Syw0Work *>(HLib::GetTaskParameter(task));
    work.task = task;
    work.selector = entry;
    work.active = 0;
    work.state = 0;
    syw0FireEffect = 0;
    Syw0Value(&work, 0x644, SYW0_KNK);
    work.characters = Queue_EVT_AT_ReadChr(task, SYW0_CHARACTERS);
    Syw0LoadResources(work);
}
void RunSyw0(std::uintptr_t) {
    QueueSyw0Event(g_entryPoint);
}
} // namespace
const ScriptAreaDefinition syw0ScriptDefinition{.areaId = SYW0_AREA, .field04 = 0, .init = InitializeSyw0, .main = RunSyw0};
} // namespace shenmue::s2::areas
