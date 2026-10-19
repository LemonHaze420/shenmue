#include "s2/areas/area_istb.hpp"
#include "s2/character.hpp"
#include "s2/author.hpp"
#include "s2/battle_qte.hpp"
#include "s2/camera.hpp"
#include "s2/date_time.hpp"
#include "s2/event_call.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sk_event.hpp"
#include "s2/sound_program.hpp"
#include <bit>
#include "s2/area_names.hpp"

namespace shenmue::s2::areas {
namespace {
constexpr shen_name ryo{SHEN_NAME(RYO_)}, sin{SHEN_NAME(SIN_)}, event{SHEN_NAME(EVNT)};
constexpr shen_name names[]{sin, shen_name{0xffffffffU}};
std::int32_t currentSelection{};
std::uint32_t splash{};
std::uint16_t splashAge{};
struct StoneWork {
    HLib::Task *self;
    std::int32_t selection, reserved0c;
    std::uint16_t phase, retries;
    std::int16_t qteAge, retryAge;
    std::int32_t succeeded;
    std::uint32_t reserved1c;
};
static_assert(sizeof(StoneWork) == 0x20 && offsetof(StoneWork, phase) == 0x10);
void Main_Callback(void *);
void Init2_Callback(void *);
void Auth1_Callback(void *);
void Auth2_Callback(void *);
void Qte_Callback(void *);
void Exit_Callback(void *);
void Gravity(shen_name name, bool enabled) {
    const auto state = enabled ? NpcCharacterControllerState::Enabled : NpcCharacterControllerState::Disabled;
    EV_SetCharaControllerFlags(name, CharacterControlApplyGravity, state);
    EV_SetCharaControllerFlags(name, CharacterControlApplyGravity | CharacterControlUpdateHeight, state);
}
void Splash_Callback(void *) {
    if (splashAge == 20) {
        SetParticleEffectBirthRate(1, splash, 0);
        ++splashAge;
    } else if (splashAge == 45) {
        splashAge = 0;
        SuspendParticleEffect(1, splash);
        FreeParticleEffect(1, splash);
        HLib::DestroyCurrentTask();
    } else
        ++splashAge;
}
void SpawnSplash_Callback(void *) {
    const Vector3 position = currentSelection ? Vector3{47.73f, 100.6f, 61.53f} : Vector3{33.34f, 100.1f, 39.830002f};
    const Vector3 volume{0.2f, 0, 0.2f}, velocity{0.0074509755f, 0.059999995f, 0.016078435f}, variance{0.015294112f, 0.041176476f, 0.039607845f};
    HLib::EnqueueTaskWithoutParameter(Splash_Callback, 4, 0x0b, event.name);
    splash = static_cast<std::uint32_t>(CreateSplashEffect(position, volume, 3000, 0.02f, shen_name{}, std::bit_cast<std::int32_t>(99.8f), nullptr, nullptr));
    SetParticleEffectColour(1, splash, 246, 175, 175, 255);
    SetParticleEffectPosition(1, splash, &position, &volume);
    SetParticleEffectVelocity(1, splash, velocity, variance);
    SetParticleEffectBirthRate(1, splash, 0.1f);
    SetParticleEffectSize(1, splash, 0.012549019418656826f, 0.012549019418656826f);
    ResumeParticleEffect(1, splash);
}
void RetryTitle_Callback(void *) {
    Queue_ST_LoadTitleFadeIn(SHEN_NAME(RTR2));
}

void Init2_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    work->phase = 0;
    Gravity(ryo, false);
    Gravity(sin, false);
    mtwkSetRequestedRootMode(scenFindMotionwork(ryo), 0);
    if (work->selection) {
        qteBattleUtil(BattleQteMode::SetWait, 4, work->retries + 5, 8 * work->retries + 36);
        qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 4, 256, 4);
        qteBattleUtil(BattleQteMode::SetLastCommands, 64);
    } else {
        qteBattleUtil(BattleQteMode::SetWait, 8, 6, 6 * (work->retries + 4));
        qteBattleUtil(BattleQteMode::SetFirstCommands, 64, 4, 256);
        qteBattleUtil(BattleQteMode::SetLastCommands);
    }
    qteBattleUtil(BattleQteMode::ResetStates);
    const Vector4 board{1, 1, 1, 1};
    for (shen_name name : {ryo, sin}) {
        SetScenarioReflectorBoardValues(name, &board);
        EV_SetRefBoardParamsFor(name, 1);
        EV_SetRefBoardParamsFor(name, 1);
    }
    if (work->retries) {
        ST_LoadTitleFade();
        work->phase = 1;
    }
    HLib::SetCurrentTaskTo(Main_Callback);
}
void Auth1_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
    HLib::SetCurrentTaskTo(Main_Callback);
    QueueSkAuthorStart(work->self, work->selection ? SHEN_NAME(ISI5) : SHEN_NAME(ISI0), nullptr, nullptr, 0);
    SetScreenFilmFade(50, 255, 0, 0, 0, 0, 0, 0, 0);
}
int BeginQte(StoneWork *work) {
    EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
    EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->selection ? SHEN_NAME(ISI6) : SHEN_NAME(ISI2)));
    const int frames = static_cast<int>(EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining));
    if (work->retries) {
        SetScreenFilmFade(50, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_LoadCutsceneFromCurrentScene("0519A000.smp");
        EV_LoadAudio("0519A000", work->selection ? 4 : 1);
        EV_spmBeginSampleExecute();
    } else if (work->selection)
        EVT_AT_MenCtrl(EventFilmControl{1, 1, 255, 0, 0, 0, 0, 0, 0, 0, 1});
    return frames - 2;
}
void Qte_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    if (!work->qteAge) {
        EV_authSceneCtrl(AuthSceneCtrl::HideFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(work->selection ? SHEN_NAME(ISI7) : SHEN_NAME(ISI3)));
    }
    if (work->qteAge == 10) {
        qteBattleUtil(BattleQteMode::Start);
        work->phase = 3;
        HLib::SetCurrentTaskTo(Main_Callback);
    }
    ++work->qteAge;
}
void Auth2_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    if (!work->retryAge) {
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::DisallowSkipAuthor);
        if (work->retries <= 5)
            ++work->retries;
    }
    if (work->retryAge == (work->selection ? 85 : 30)) {
        work->phase = 4;
        QueueEventRepeatedFunction(RetryTitle_Callback, nullptr, 370, 1);
        QueueEventRepeatedFunction(SpawnSplash_Callback, nullptr, 40, 1);
        QueueSkAuthorStart(work->self, work->selection ? SHEN_NAME(ISI8) : SHEN_NAME(ISI1), nullptr, nullptr, 0);
        HLib::SetCurrentTaskTo(Main_Callback);
    }
    ++work->retryAge;
}
void Main_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    switch (work->phase) {
    case 0:
        HLib::SetCurrentTaskTo(Auth1_Callback);
        work->phase = 1;
        break;
    case 1: {
        const int delay = BeginQte(work);
        work->phase = 2;
        if (delay)
            HLib::SetupTaskDelay(delay);
        break;
    }
    case 2:
        work->qteAge = 0;
        work->retryAge = 0;
        HLib::SetCurrentTaskTo(Qte_Callback);
        work->phase = 3;
        break;
    case 3:
        if (qteBattleUtil(BattleQteMode::GetResult)) {
            if (qteBattleUtil(BattleQteMode::GetCompletedCount) == (work->selection ? 5 : 3)) {
                work->phase = 4;
                work->succeeded = 1;
            } else
                HLib::SetCurrentTaskTo(Auth2_Callback);
        }
        break;
    case 4:
        if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
            if (work->succeeded)
                work->phase = 5;
            else {
                HLib::SetCurrentTaskTo(Init2_Callback);
                HLib::SetupTaskDelay(10);
            }
        }
        break;
    case 5:
        EV_authSceneCtrl(AuthSceneCtrl::ShowFilmBars);
        EV_authSceneCtrl(AuthSceneCtrl::AllowSkipAuthor);
        HLib::SetCurrentTaskTo(Exit_Callback);
        break;
    default:
        break;
    }
}
void Exit_Callback(void *data) {
    auto *work = static_cast<StoneWork *>(data);
    DateTime time{};
    GetCurrentDateTime(time);
    time.minute = static_cast<std::int8_t>(time.minute + 30);
    if (time.minute >= 60) {
        ++time.hour;
        time.minute -= 60;
    }
    SetCurrentDateTimeTo(time);
    shenSetTimeSimulationTo(1);
    qteBattleUtil(BattleQteMode::RetireAndFree);
    Gravity(ryo, true);
    Gravity(sin, true);
    EV_npcFree(sin);
    EV_npcLockUnlock(sin);
    EV_NPC_NewLoadSw(1);
    EVS_LoopCamera(0, shen_name{}, shen_name{});
    camCtrlSetFovTransitionTo(true);
    camCtrlSetState(CameraState::Init);
    SetPlayerControlEnabled(1);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("ISTB"), 0);
    const int selection = work->selection;
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->self);
    EV_scenQueueLoad(g_actNumber, shen_name{selection ? SHEN_NAME(KMZ3) : SHEN_NAME(KMZ2)}, 0);
}
} // namespace
void AreaISTBInit() {
    currentSelection = 0;
}
void AreaISTBMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    auto *task = HLib::EnqueueTaskWithParameter(Init2_Callback, 5, 0x0b, sizeof(StoneWork), event);
    auto *work = static_cast<StoneWork *>(HLib::GetTaskParameter(task));
    work->self = task;
    work->selection = 99;
    work->retries = 0;
    work->succeeded = 0;
    if (g_currentMap == AREA_KRK2)
        work->selection = 0;
    if (g_currentMap == AREA_KRK4)
        work->selection = 1;
    currentSelection = work->selection;
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    Queue_EVT_AT_ReadChr(work->self, names);
    shenSetTimeSimulationTo(0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("ISTB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    qteBattleUtil(BattleQteMode::Load);
    qteBattleUtil(BattleQteMode::Queue);
    EV_LoadCutsceneFromCurrentScene("0519A000.smp");
    EV_FreeAuthorPak(1, "0519");
}
const ScriptAreaDefinition areaISTBScriptDefinition{.areaId = SHEN_NAME(ISTB), .field04 = 0, .init = AreaISTBInit, .main = AreaISTBMain};
} // namespace shenmue::s2::areas
