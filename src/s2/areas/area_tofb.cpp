#include "s2/areas/area_tofb.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/game_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <algorithm>
#include <cmath>
namespace shenmue::s2 {
namespace {
std::int32_t tofbRetries;
float tofbLightPhase;
} // namespace
// @verified - p
void TofbInit() {
    tofbRetries = 0;
}
// @verified - p
void TofbDormantBoundary() {
}
// @verified - p
void TofbMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TofbStartCallback, 4, 0x0b, sizeof(TofbEventWork), TOFB_TASK);
    TofbEventWork *const work = static_cast<TofbEventWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    InitializeTofbResources(work);
    flddSetPointFlagOnGrid(TOFB_GRID, 0);
    tofbRetries = 0;
    SelectPostFxBank(1, 1);
    UploadGlowParams(GlowParameters{5, 0.3f, {0.3f, 0.3f, 0.3f, 0.2f}});
}
// @verified - p
void InitializeTofbResources(TofbEventWork *work) {
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    GetCurrentDateTime(eventTemporaryDateTime);
    work->characters = Queue_EVT_AT_ReadChr(work->task, TOFB_LOAD_CHARACTERS);
    work->motionHandle = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0047"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("TOFB"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
    EV_FreeAuthorPak(1, "0047");
    EV_FreeAuthorPak(1, "0048");
    EV_SetNewSceneIndex();
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    SetWeatherInstruction(0x0c, false);
    SetWeatherInstruction(0x780, false);
    EV_lightLoad(TOFB_LIGHT);
    PushGlobalLightingProgram();
    SetRenderClockAndCompassTo(1);
    TofbDormantBoundary();
}
// @verified - p
void TofbStartCallback(void *) {
    EV_EnableCharacterDisplay(TOFB_CHO);
    EV_EnableCharacterDisplay(TOFB_A412);
    EV_EnableCharacterDisplay(TOFB_FR00);
    EV_EnableCharacterDisplay(TOFB_A021);
    EV_DisableCharacterDisplay(TOFB_TABLE);
    EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    EV_DisableCharacterDisplay(TOFB_A418);
    EV_LoadCutsceneFromCurrentScene("0047A000");
    EV_LoadAudio("0047A000", 0);
    EV_spmBeginSampleExecute();
    if (tofbRetries > 0)
        EV_SetAuthorActiveTo(1);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOFB_AUTHOR.name));
    HLib::SetCurrentTaskTo(TofbOpeningAuthorCallback);
}
// @verified - p
void TofbOpeningAuthorCallback(void *) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 0)
        tofbLightPhase = 0;
    else if (frame == 925) {
        EV_EnableCharacterDisplay(TOFB_A418);
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, sizeof(EventPlayDynamics), TOFB_TASK);
        RegisterEventTask(task);
        EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
        dynamics->character = TOFB_A418;
        dynamics->resource = "MICS";
        dynamics->parts = TOFB_DYNAMICS_PARTS;
        dynamics->endFrame = 110;
        dynamics->trackCount = 84;
        dynamics->frameStep = 2;
        dynamics->currentFrame = 2;
        dynamics->delay = 0;
    } else {
        const std::uint16_t first = static_cast<std::uint16_t>(static_cast<std::int32_t>(tofbLightPhase * 182.04443f));
        const std::uint16_t third = static_cast<std::uint16_t>(static_cast<std::int32_t>((tofbLightPhase * 3.0f) * 182.04443f));
        const std::uint16_t foldedFirst = first <= 0x8000U ? first : static_cast<std::uint16_t>(-first);
        const std::uint16_t foldedThird = third <= 0x8000U ? third : static_cast<std::uint16_t>(-third);
        const float amplitude = sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedFirst)) * 0.05000000074505806f;
        SetLightSceneIntensity(20, amplitude * sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedThird)) + 2.0f);
        tofbLightPhase += 20.0f;
        if (tofbLightPhase >= 180.0f)
            tofbLightPhase = -180.0f;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(TofbBattleStartCallback);
        HLib::SetupTaskDelay(60);
    }
}
// @verified - p
void TofbBattleStartCallback(void *) {
    using enum CameraState;
    flddSetPointFlagOnGrid(TOFB_GRID, 1);
    flddSetPointFlagOnGrid(TOFB_DEFAULT_GRID, 0);
    EV_DisableCharacterDisplay(TOFB_CHO);
    EV_DisableCharacterDisplay(TOFB_A412);
    EV_DisableCharacterDisplay(TOFB_FR00);
    EV_DisableCharacterDisplay(TOFB_A021);
    EV_EnableCharacterDisplay(TOFB_TABLE);
    EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    shenStartFGST(10, 10, 0, 0);
    for (std::size_t index = 0; index < 5; ++index) {
        EVT_AT_SetPos(TOFB_BATTLE_CHARACTERS[index], TOFB_INVALID, TOFB_BATTLE_POSITIONS[index], 0);
        EVT_AT_SetAngY(TOFB_BATTLE_CHARACTERS[index], TOFB_INVALID, 180, 0);
    }
    for (const shen_name character : TOFB_BATTLE_CHARACTERS)
        EVT_AT_SetMot(character, 13, 0, 1, 0x40000000U, 0);
    camCtrlSetState(Event);
    camCtrlSetTargetPosition(TOFB_CAMERA_TARGET);
    camCtrlSetNextPosition(TOFB_CAMERA_POSITION);
    EVT_AT_SetPos(TOFB_TABLE, TOFB_A021, Vector3{}, 0);
    EVT_AT_SetAngY(TOFB_TABLE, TOFB_A021, 0, 0);
    EVT_AT_SetPos(TOFB_BROKEN_TABLE, TOFB_A021, Vector3{}, 0);
    EVT_AT_SetAngY(TOFB_BROKEN_TABLE, TOFB_A021, 0, 0);
    AllocateVOBJ(0, 1);
    vfStageDefinitionAddHitbox(0, 2, 1, 0, 0.30000001192092896f, 0, 0.6000000238418579f);
    vfStageDefinitionAddHitbox(0, 2, 0x8001, 0.8999999761581421f, 0.5f, 0, 0.5f);
    vfStageDefinitionSetMotion(0, TOFB_TABLE_MOTION.name, 17, nullptr, 2, 2, 299, 117);
    vfAllocateVOBJ(0, FindCharacterByScenarioName(TOFB_TABLE), 0);
    vfStageObjectSetBrokenModel(0, FindCharacterByScenarioName(TOFB_BROKEN_TABLE), 720);
    vfInitializeStageObjectHitboxes();
    shenInitFightData(FindCharacterByScenarioName(TOFB_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (tofbRetries + 2)), 0);
    shenInitFightData(FindCharacterByScenarioName(TOFB_CHR), nullptr, 1, 1, 80, 6);
    shenInitFightData(FindCharacterByScenarioName(TOFB_SBO), nullptr, 1, 1, 80, 3);
    shenInitFightData(FindCharacterByScenarioName(TOFB_SCX), nullptr, 1, 1, 100, 4);
    shenInitFightData(FindCharacterByScenarioName(TOFB_MRC), nullptr, 1, 1, 100, 5);
    vfCopyPosFromEntity(0, nullptr, 219, 212, 4);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    for (const shen_name character : TOFB_HIDDEN_CHARACTERS) {
        EV_DisableCharacterDisplay(character);
        ToggleScenarioCharacterFlag(character);
    }
    vfmQueueFightStart(5, 2, 1, 1, -1);
    EV_vfBattleUtil(FightBattleUtilOp::CombatCameraFlags, shen_name{2}, 1);
    EV_LoadAudio("0047A000", 1);
    EV_spmBeginSampleExecute();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    HLib::SetCurrentTaskTo(TofbBattleCallback);
}
// @verified - p
void TofbBattleCallback(void *) {
    if (IsFightActive()) {
        MaintainTofbBattleCamera(0);
        return;
    }
    for (const shen_name character : TOFB_HIDDEN_CHARACTERS)
        EnsureScenarioCharacterDraw(character);
    if (GetBtlStatus(FindCharacterByScenarioName(TOFB_PLAYER)) == 3) {
        EV_stepSetFlags(0, 0x55);
        HLib::SetCurrentTaskTo(TofbVictoryAuthorCallback);
    } else {
        SetCurrentDateTimeTo(eventTemporaryDateTime);
        shenSetTimeSimulationTo(1);
        if (tofbRetries < 15)
            ++tofbRetries;
        Queue_ST_LoadTitleFadeIn(TOFB_RETRY_TITLE);
        HLib::SetCurrentTaskTo(TofbRetryCallback);
    }
    spmFadeOut();
    SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTaskDelay(60);
}
// @verified - p
void TofbVictoryAuthorCallback(void *) {
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0047"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0048"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(TOFB_LIGHT);
    PushGlobalLightingProgram();
    EV_LoadCutsceneFromCurrentScene("0048A000");
    EV_LoadAudio("0048A000", 0);
    EV_spmBeginSampleExecute();
    EV_DisableCharacterDisplay(TOFB_TABLE);
    EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(TOFB_AUTHOR.name));
    HLib::SetCurrentTaskTo(TofbEndingAuthorCallback);
}
// @verified - p
void TofbRetryCallback(void *) {
    ST_LoadTitleFade();
    HLib::SetCurrentTaskTo(TofbStartCallback);
    HLib::SetupTaskDelay(30);
}
// @verified - p
void MaintainTofbBattleCamera(std::int32_t mode) {
    Vector3 camera;
    camCtrlGetNextCamPos(&camera);
    if (!FindCharacterByScenarioName(TOFB_BROKEN_TABLE))
        return;
    if (mode == 1) {
        EV_SetCharacterAlphaEnabled(TOFB_BROKEN_TABLE, false);
        return;
    }
    Vector3 position;
    EV_GetCharaPosition(TOFB_BROKEN_TABLE, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    const float x = camera.x - position.x, y = camera.y - position.y, z = camera.z - position.z;
    const float alpha = std::max(0.0f, (std::sqrt((x * x + y * y) + z * z) - 0.800000011920929f) / 1.7000000476837158f);
    if (alpha >= 1.0f)
        EV_SetCharacterAlphaEnabled(TOFB_BROKEN_TABLE, false);
    else {
        EV_SetCharacterAlphaEnabled(TOFB_BROKEN_TABLE, true);
        SetScenarioCharacterTransparency(TOFB_BROKEN_TABLE, alpha);
    }
}
// @verified - p
void TofbEndingAuthorCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        spmFadeOut();
        HLib::SetCurrentTaskTo(TofbEndCallback);
        HLib::SetupTaskDelay(60);
    }
}
// @verified - p
void CleanupTofbResources(TofbEventWork *work) {
    FinishLoading();
    SetRenderClockAndCompassTo(0);
    LoadCutsceneAndSoundProgramForArea();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0048");
    EV_FreeAuthorPak(0, "0047");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0048"), 0);
    DeleteMotion(work->motionHandle);
    ET_AT_KillWork(work->characters);
    EV_NPC_NewLoadSw(1);
}
// @verified - p
void TofbEndCallback(void *data) {
    TofbEventWork *const work = static_cast<TofbEventWork *>(data);
    flddSetPointFlagOnGrid(TOFB_DEFAULT_GRID, 1);
    flddSetPointFlagOnGrid(TOFB_GRID, 0);
    MaintainTofbBattleCamera(1);
    SelectPostFxBank(1, 0);
    CleanupTofbResources(work);
    EV_CleanupMapControlTask();
    HLib::EV_HLKillOtherHierarchyTask(work->task);
    if (g_currentMap.name == TOFB_AREA.name) {
        scenSetTaskFlags(8);
        EV_scenQueueLoad(g_actNumber, TOFB_NEXT_AREA, 0);
    }
}
const ScriptAreaDefinition tofbScriptDefinition{TOFB_AREA, 0, TofbInit, TofbMain};
} // namespace shenmue::s2
