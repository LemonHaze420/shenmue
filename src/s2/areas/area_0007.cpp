#include "s2/debug.hpp"
#include "s2/areas/area_0007.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hand.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
std::array<std::byte, 0x100> distance0007{}, kernel0007First{}, kernel0007Second{};
void E0007MainCallback(void *);
void E0007LoopCallback(void *);
// @verified - p
void E0007Init() {
    distance0007.fill(std::byte{});
    kernel0007First.fill(std::byte{});
    kernel0007Second.fill(std::byte{});
}
// @verified - p
bool E0007CheckArgumentZero(std::int32_t value) {
    return value == 0;
}
// @verified - p
void E0007WaveCallback(void *) {
    const std::array<std::uint32_t, 2> colours = GetWeatherWaveColours();
    const std::uint32_t specular = colours[0], diffuse = colours[1];
    BuildWaveLightingTable(1, 1.0f, 1.0f,
        {static_cast<float>((diffuse >> 16) & 255U) / 255.0f * 0.36070001125335693f, static_cast<float>((diffuse >> 8) & 255U) / 255.0f * 0.3725000023841858f,
            static_cast<float>(diffuse & 255U) / 255.0f * 0.4352000057697296f},
        {static_cast<float>((specular >> 16) & 255U) / 255.0f * 0.2549000084400177f, static_cast<float>((specular >> 8) & 255U) / 255.0f * 0.3799999952316284f,
            static_cast<float>(specular & 255U) / 255.0f * 0.28619998693466187f},
        42.540000915527344f, 1.0f);
    ApplyWaveTextureMeshResource(17);
    ApplyWaveReflectionResource(18);
}
// @verified - p
void SetupE0007Environment() {
    SetPlayerControlEnabled(0);
    EV_SetNewSceneIndex();
    if (g_currentMap.name != AREA_0007_AR02.name) {
        DateTime time;
        GetCurrentDateTime(time);
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
        } else {
            EV_lightLoad(AREA_0007_LIGHT);
            PushGlobalLightingProgram();
            (void)E0007CheckArgumentZero(1);
        }
    }
    EV_FreeAuthorPak(1, "0007");
    EV_LoadCutsceneFromCurrentScene("0007A000");
    if (g_currentMap.name != AREA_0007_AR02.name) {
        GenerateFFT(2.0f);
        FillFftDistance(FillFftKernel(23.52899932861328f, GenerateDistanceFalloffRowMajor(2.0820000171661377f, distance0007)));
        void *const second = FillFftKernel(0.03500000014901161f, GenerateDistanceFalloffKernel8x8(0.9520000219345093f, kernel0007Second));
        void *const first = FillFftKernel(0.05640000104904175f, GenerateDistanceFalloffKernel8x8(0.9399999976158142f, kernel0007First));
        ApplyFft(first, second);
        FinalizeFft(0.0020000000949949026f, 0.0020000000949949026f);
        SetWaveLighting(0.1f, {0.059799999f, 0.97680002f, 0.2054f});
        HLib::EnqueueTaskWithoutParameter(E0007WaveCallback, 3, 2, static_cast<std::int32_t>(AREA_AUTHOR_TASK.name));
    }
}
// @verified - p
void E0007Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    SetNpcColdResetMode(0);
    SetPlayerControlEnabled(0);
    DateTime time;
    GetCurrentDateTime(time);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0007MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = EV_GetShenmueFlag(0xff) != 0;
    EV_stepSetFlags(0, 1);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 1);
    SetupE0007Environment();
    SetAreaNpcLowQualityHands(false);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0007_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0007Characters(std::int16_t selection) {
    SetLowQualityHands(FindCharacterByScenarioName(AREA_0007_JOP));
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_JOP), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0007_JOP, false);
    EV_SetCharacterAlphaEnabled(AREA_0007_AA39, false);
    if (selection == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_SHI), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_SHI, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_UNH), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_UNH, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_GOI), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_GOI, false);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_HTK), 6, 2);
        EV_SetCharacterAlphaEnabled(AREA_0007_HTK, false);
    }
}
// @verified - p
void RestoreE0007Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_JOP), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0007_JOP, true);
    EV_SetCharacterAlphaEnabled(AREA_0007_AA39, true);
    if (selection == 0) {
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_SHI), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_SHI, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_UNH), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_UNH, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_GOI), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_GOI, true);
        mtwkSetExtraFlag(scenFindMotionwork(AREA_0007_HTK), 6, 0);
        EV_SetCharacterAlphaEnabled(AREA_0007_HTK, true);
    }
}
// @verified - p
void CleanupE0007(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0007_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 0);
    EV_NPC_NewLoadSw(1);
    EV_FreeAuthorPak(0, "0007");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0007A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    SetNpcColdResetMode(1);
    if (g_currentMap.name == AREA_0007_AR02.name) {
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {224.8800048828125f, 100, 327.6000061035156f}, 0};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 25753, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        LoadCutsceneAndSoundProgramForArea();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueE0007Loop(AreaAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0007LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    shenPlayAudioWithFlags("0007C001", 1);
    SetupE0007Characters(parent->selection);
    EV_authSceneCtrl(AuthSceneCtrl::SetCharaHQFlags);
    if (parent->selection != 0)
        EV_debug_log("0007\\e0007_et_0007.c", 194, "NIGHT_AUTH\n");
    else
        EV_debug_log("0007\\e0007_et_0007.c", 197, "DEF_AUTH\n");
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0007_AUTHORS[parent->selection != 0].name));
    parent->state = 2;
}
// @verified - p
void E0007MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0007Loop(parent);
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(AREA_0007_RCKO), 0);
    } else if (parent->state == 3) {
        CleanupE0007(parent);
        charaSetDisplayEnabledFlag(FindCharacterByScenarioName(AREA_0007_RCKO), 1);
    }
}
// @verified - p
void E0007LoopCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorLoopTaskParameter *>(taskData)->parent;
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 0) {
        SetPluralControlFor(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 1);
        SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_JOP), 1);
    }
    if (parent->selection == 0 && frame == 341)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, AREA_0007_DUCK_GUTS_FLAG, 0);
    if ((parent->selection == 0 && frame == 370) || (parent->selection == 1 && frame == 0)) {
        if (parent->selection == 0) {
            SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_GOI), 1);
            SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_HTK), 1);
            SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_SHI), 1);
            SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_UNH), 1);
        }
        EV_ToggleCharacterFlag(AREA_0007_GOI);
        EV_ToggleCharacterFlag(AREA_0007_HTK);
        EV_ToggleCharacterFlag(AREA_0007_SHI);
        EV_ToggleCharacterFlag(AREA_0007_UNH);
        EV_DisableCharacterDisplay(AREA_0007_GOI);
        EV_DisableCharacterDisplay(AREA_0007_HTK);
        EV_DisableCharacterDisplay(AREA_0007_SHI);
        EV_DisableCharacterDisplay(AREA_0007_UNH);
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    EV_EnsureCharacterDraw(AREA_0007_GOI);
    EV_EnsureCharacterDraw(AREA_0007_HTK);
    EV_EnsureCharacterDraw(AREA_0007_SHI);
    EV_EnsureCharacterDraw(AREA_0007_UNH);
    EV_EnableCharacterDisplay(AREA_0007_GOI);
    EV_EnableCharacterDisplay(AREA_0007_HTK);
    EV_EnableCharacterDisplay(AREA_0007_SHI);
    EV_EnableCharacterDisplay(AREA_0007_UNH);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 0);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_JOP), 0);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_GOI), 0);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_HTK), 0);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_SHI), 0);
    SetPluralControlFor(FindCharacterByScenarioName(AREA_0007_UNH), 0);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0007Characters(parent->selection);
    parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0007ScriptDefinition{.areaId = AREA_0007_AREA, .field04 = 0, .init = E0007Init, .main = E0007Main};
} // namespace shenmue::s2::areas
