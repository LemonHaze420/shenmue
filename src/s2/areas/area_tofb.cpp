#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_tofb.hpp"

#if defined(SHENMUE_EVENT_ENTRY)
void TofbMain(void *argument)
{
    QueueTofbEvent();
}
#else
static const int tofbDynamicsParts[] = {152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
    175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
    205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 228, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234,
    235};
const shen_name tofbInvalidCharacter = EVENT_CHARACTER_INVALID;
static const shen_name tofbLoadCharacters[] = {TOFB_PLAYER, TOFB_CHO, TOFB_SBO, TOFB_MRC, TOFB_SCX, TOFB_CHR, TOFB_RKN, TOFB_SBK, TOFB_SKH,
    EVENT_CHARACTER_INVALID};
static Vector3 tofbCameraTarget = {215.9774932861328f, 105.64350128173828f, 212.9739990234375f};
static Vector3 tofbCameraPosition = {213.4884033203125f, 105.7667007446289f, 213.3881072998047f};
static shen_name tofbBrokenTableNames[] = {TOFB_TABLE, TOFB_BROKEN_TABLE, 0};
static int tofbRetries;

void QueueTofbEvent(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(TofbStartCallback, 4, 11, sizeof(TofbEventWork), TASK_EVNT);
    TofbEventWork *work = (TofbEventWork *)FTBL.HLib_GetTaskParameter(task);
    work->task = task;
    InitializeTofbResources(work);
    FTBL.flddSetPointFlagOnGrid(TOFB_GRID, 0);
    tofbRetries = 0;
    EventDebugRegister(TOFB_DEBUG_ID, TofbDebugExitCallback, work, 2000000000);
}

void TofbDebugExitCallback(void *data)
{
    FTBL.EV_stepSetFlags(85, 0);
    FinishTofbEvent((TofbEventWork *)data);
}

void FinishTofbEvent(TofbEventWork *work)
{
    CleanupTofbResources(work);
    FTBL.EV_HLKillOtherHierarchyTask(work->task);
    EventDebugUnregister(TOFB_DEBUG_ID);
    if (g_currentMap == TOFB_AREA) {
        FTBL.scenSetTaskFlags(8);
        FTBL.EV_scenQueueLoad(g_actNumber, TOFB_NEXT_AREA, 0);
    }
}

void TofbStartCallback(void *data)
{
    EventDebugCheck(TOFB_DEBUG_ID, 4);
    FTBL.EV_EnableCharacterDisplay(TOFB_CHO);
    FTBL.EV_EnableCharacterDisplay(TOFB_A412);
    FTBL.EV_EnableCharacterDisplay(TOFB_FR00);
    FTBL.EV_EnableCharacterDisplay(TOFB_A021);
    FTBL.EV_DisableCharacterDisplay(TOFB_TABLE);
    FTBL.EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    FTBL.EV_LoadCutsceneFromCurrentScene("0047A000");
    FTBL.EV_LoadAudio("0047A000", 0);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_authSceneCtrl(TOFB_AUTHOR);
    FTBL.HLib_SetCurrentTaskTo(TofbOpeningAuthorCallback);
}

void TofbOpeningAuthorCallback(void *data)
{
    if (FTBL.EV_authSceneCtrl(-6) == 925)
        Queue_EVT_AT_PlayDynamix(TOFB_A418, "MICS", 2.0f, 110.0f, 84, tofbDynamicsParts, 2.0f, 0);
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(TofbBattleStartCallback);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void TofbBattleStartCallback(void *data)
{
    FTBL.flddSetPointFlagOnGrid(TOFB_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(TOFB_DEFAULT_GRID, 0);
    FTBL.EV_DisableCharacterDisplay(TOFB_CHO);
    FTBL.EV_DisableCharacterDisplay(TOFB_A412);
    FTBL.EV_DisableCharacterDisplay(TOFB_FR00);
    FTBL.EV_DisableCharacterDisplay(TOFB_A021);
    FTBL.EV_EnableCharacterDisplay(TOFB_TABLE);
    FTBL.EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    FTBL.shenStartFGST(10, 10, 0, 0);
    EVT_AT_SetPos(TOFB_PLAYER, EVENT_CHARACTER_INVALID, 217.0f, 104.6f, 212.5f, 0);
    EVT_AT_SetAngY(TOFB_PLAYER, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetPos(TOFB_SBO, EVENT_CHARACTER_INVALID, 219.6f, 104.6f, 212.5f, 0);
    EVT_AT_SetAngY(TOFB_SBO, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetPos(TOFB_MRC, EVENT_CHARACTER_INVALID, 216.6f, 104.6f, 210.5f, 0);
    EVT_AT_SetAngY(TOFB_MRC, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetPos(TOFB_SCX, EVENT_CHARACTER_INVALID, 219.7f, 104.6f, 212.3f, 0);
    EVT_AT_SetAngY(TOFB_SCX, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetPos(TOFB_CHR, EVENT_CHARACTER_INVALID, 215.7f, 104.6f, 214.3f, 0);
    EVT_AT_SetAngY(TOFB_CHR, EVENT_CHARACTER_INVALID, 180.0f, 0);
    EVT_AT_SetMot(TOFB_PLAYER, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(TOFB_SBO, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(TOFB_MRC, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(TOFB_SCX, 13, 0.0f, 1.0f, 0x40000000U, 0);
    EVT_AT_SetMot(TOFB_CHR, 13, 0.0f, 1.0f, 0x40000000U, 0);
    FTBL.camCtrlSetState(9);
    FTBL.camCtrlSetTargetPosition(&tofbCameraTarget);
    FTBL.camCtrlSetNextPosition(&tofbCameraPosition);
    EVT_AT_SetPos(TOFB_TABLE, TOFB_A021, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(TOFB_TABLE, TOFB_A021, 0.0f, 0);
    EVT_AT_SetPos(TOFB_BROKEN_TABLE, TOFB_A021, 0.0f, 0.0f, 0.0f, 0);
    EVT_AT_SetAngY(TOFB_BROKEN_TABLE, TOFB_A021, 0.0f, 0);
    FTBL.AllocateVOBJ(0, 1);
    FTBL.vfStageDefinitionAddHitbox(0, 2, 1, 0.0f, 0.3f, 0.0f, 0.6f, 0.0f, 0.0f);
    FTBL.vfStageDefinitionAddHitbox(0, 2, 0x8001, 0.9f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f);
    FTBL.vfStageDefinitionSetMotion(0, TOFB_TABLE_MOTION, 17, 0, 2, 2, 299, 117);
    FTBL.vfAllocateVOBJ(0, FTBL.ctrlGetCharaByName(TOFB_TABLE), 0);
    FTBL.vfStageObjectSetBrokenModel(0, FTBL.ctrlGetCharaByName(TOFB_BROKEN_TABLE), 720);
    FTBL.vfInitializeStageObjectHitboxes();
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TOFB_PLAYER), 0, 0, 0, tofbRetries * 100 + 200, 0);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TOFB_CHR), 0, 1, 1, 80, 6);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TOFB_SBO), 0, 1, 1, 80, 3);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TOFB_SCX), 0, 1, 1, 100, 4);
    FTBL.shenInitFightData(FTBL.ctrlGetCharaByName(TOFB_MRC), 0, 1, 1, 100, 5);
    FTBL.vfCopyPosFromEntity(0, 0, 219.0f, 212.0f, 4.0f);
    FTBL.EV_vfBattleUtil(3, 0x800, 1);
    FTBL.EV_DisableCharacterDisplay(TOFB_CHO);
    FTBL.EV_ToggleCharacterFlag(TOFB_CHO);
    FTBL.EV_DisableCharacterDisplay(TOFB_RKN);
    FTBL.EV_ToggleCharacterFlag(TOFB_RKN);
    FTBL.EV_DisableCharacterDisplay(TOFB_SBK);
    FTBL.EV_ToggleCharacterFlag(TOFB_SBK);
    FTBL.EV_DisableCharacterDisplay(TOFB_SKH);
    FTBL.EV_ToggleCharacterFlag(TOFB_SKH);
    FTBL.vfmQueueFightStart(5, 2, 1.0f, 1, -1);
    FTBL.EV_vfBattleUtil(1, 2, 1);
    FTBL.EV_LoadAudio("0047A000", 1);
    EVT_AT_SetSample(0, 0, 0);
    EVT_AT_MenCtrl(30, 255, 0, 0, 0, 0, 0, 0, 0, 0);
    FTBL.HLib_SetCurrentTaskTo(TofbBattleCallback);
}

void TofbBattleCallback(void *data)
{
    if (!FTBL.IsFightActive()) {
        FTBL.EV_EnsureCharacterDraw(TOFB_CHO);
        FTBL.EV_EnsureCharacterDraw(TOFB_RKN);
        FTBL.EV_EnsureCharacterDraw(TOFB_SBK);
        FTBL.EV_EnsureCharacterDraw(TOFB_SKH);
        if (FTBL.GetBtlStatus(FTBL.ctrlGetCharaByName(TOFB_PLAYER)) == 3) {
            FTBL.EV_stepSetFlags(85, 0);
            FTBL.HLib_SetCurrentTaskTo(TofbVictoryAuthorCallback);
        } else {
            RestoreEventDateTime();
            if (tofbRetries < 15)
                tofbRetries++;
            FTBL.Queue_ST_LoadTitleFadeIn(TOFB_RETRY_TITLE);
            FTBL.HLib_SetCurrentTaskTo(TofbRetryCallback);
        }
        EVT_AT_MenCtrl(30, 0, 0, 0, 0, 255, 0, 0, 0, 0);
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetupTaskDelay(60);
    } else
        MaintainTofbBattleCamera(0);
}

void TofbRetryCallback(void *data)
{
    FTBL.ST_LoadTitleFade();
    FTBL.HLib_SetCurrentTaskTo(TofbStartCallback);
    FTBL.HLib_SetupTaskDelay(30);
}

void TofbVictoryAuthorCallback(void *data)
{
    FTBL.EV_ControlPackageResources(0, 6, "0047", 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0048", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.SetMapModelDataDefaultVertexShader();
    FTBL.EV_lightLoad(TOFB_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.EV_LoadCutsceneFromCurrentScene("0048A000");
    FTBL.EV_LoadAudio("0048A000", 0);
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_DisableCharacterDisplay(TOFB_TABLE);
    FTBL.EV_DisableCharacterDisplay(TOFB_BROKEN_TABLE);
    FTBL.EV_authSceneCtrl(TOFB_AUTHOR);
    FTBL.HLib_SetCurrentTaskTo(TofbEndingAuthorCallback);
}

void TofbEndingAuthorCallback(void *data)
{
    if (!FTBL.EV_authSceneCtrl(-1)) {
        EVT_AT_SetSample(0, 1, 0);
        FTBL.HLib_SetCurrentTaskTo(TofbEndCallback);
        FTBL.HLib_SetupTaskDelay(60);
    }
}

void TofbEndCallback(void *data)
{
    FTBL.flddSetPointFlagOnGrid(TOFB_DEFAULT_GRID, 1);
    FTBL.flddSetPointFlagOnGrid(TOFB_GRID, 0);
    MaintainTofbBattleCamera(1);
    FinishTofbEvent((TofbEventWork *)data);
}

void InitializeTofbResources(TofbEventWork *work)
{
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    SaveEventDateTime();
    work->characters = Queue_EVT_AT_ReadChr(work->task, tofbLoadCharacters);
    work->motion = FTBL.ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0047", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"TOFB", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_ControlPackageResources(2, 4, "TOFB", 0, 0);
    FTBL.EV_ControlPackageResources(2, 2, "TOFB", 0, 0);
    FTBL.EV_FreeAuthorPak(1, "0047");
    FTBL.EV_FreeAuthorPak(1, "0048");
    FTBL.EV_SetNewSceneIndex();
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    FTBL.EV_SetWeatherInstruction(0x0c, 0);
    FTBL.EV_SetWeatherInstruction(0x780, 0);
    FTBL.EV_lightLoad(TOFB_LIGHT);
    FTBL.PushGlobalLightingProgram();
    FTBL.SetRenderClockAndCompassTo(1);
    FTBL.SetLoadingBusy();
}

void CleanupTofbResources(TofbEventWork *work)
{
    FTBL.FinishLoading();
    FTBL.SetRenderClockAndCompassTo(0);
    FTBL.LoadCutsceneAndSoundProgramForArea();
    EVT_AT_SetSample(0, 0, 0);
    FTBL.EV_FreeAuthorPak(0, "0048");
    FTBL.EV_FreeAuthorPak(0, "0047");
    FTBL.EV_ControlPackageResources(0, 6, "TOFB", 0, 0);
    FTBL.EV_ControlPackageResources(0, 6, "0048", 0, 0);
    FTBL.DeleteMotion(work->motion);
    ET_AT_KillWork(work->characters);
    FTBL.EV_NPC_NewLoadSw_0(1);
}

void MaintainTofbBattleCamera(int mode)
{
    Vector3 camera, position;
    float distance, alpha;
    int index;
    FTBL.camCtrlGetNextCamPos(&camera);
    for (index = 0; tofbBrokenTableNames[index]; index++) {
        if (FTBL.ctrlGetCharaByName(tofbBrokenTableNames[index])) {
            if (mode == 1)
                alpha = 1.0f;
            else {
                FTBL.GetCharaPosition(tofbBrokenTableNames[index], -1, &position, 0x40000000);
                distance = (camera.x - position.x) * (camera.x - position.x) + (camera.y - position.y) * (camera.y - position.y)
                    + (camera.z - position.z) * (camera.z - position.z);
                TOFB_FSQRT(distance);
                alpha = (distance - 0.8f) / 1.7f;
                if (alpha < 0.0f)
                    alpha = 0.0f;
            }
            if (alpha >= 1.0f)
                FTBL.EV_SetCharacterAlphaEnabled(tofbBrokenTableNames[index], 0);
            else {
                FTBL.EV_SetCharacterAlphaEnabled(tofbBrokenTableNames[index], 1);
                FTBL.SetScenarioCharacterTransparency(tofbBrokenTableNames[index], alpha);
            }
        }
    }
}
#endif
#else
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
    shenGetCurrentDateTime(eventTemporaryDateTime);
    work->characters = Queue_EVT_AT_ReadChr(work->task, TOFB_LOAD_CHARACTERS);
    work->motionHandle = ReadMotiFromDirectory(g_actNumber, g_currentMap, "ZAKO.MOT");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0047"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("TOFB"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
    EV_pakUtils(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
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
    shenSetClockAndCompassRenderTo(1);
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
    vfAllocateVOBJ(0, ctrlGetScenarioChara(TOFB_TABLE), 0);
    vfStageObjectSetBrokenModel(0, ctrlGetScenarioChara(TOFB_BROKEN_TABLE), 720);
    vfInitializeStageObjectHitboxes();
    shenInitFightData(ctrlGetScenarioChara(TOFB_PLAYER), nullptr, 0, 0, static_cast<std::uint16_t>(100 * (tofbRetries + 2)), 0);
    shenInitFightData(ctrlGetScenarioChara(TOFB_CHR), nullptr, 1, 1, 80, 6);
    shenInitFightData(ctrlGetScenarioChara(TOFB_SBO), nullptr, 1, 1, 80, 3);
    shenInitFightData(ctrlGetScenarioChara(TOFB_SCX), nullptr, 1, 1, 100, 4);
    shenInitFightData(ctrlGetScenarioChara(TOFB_MRC), nullptr, 1, 1, 100, 5);
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
    if (GetBtlStatus(ctrlGetScenarioChara(TOFB_PLAYER)) == 3) {
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
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0047"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("0048"));
    EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
    if (!ctrlGetScenarioChara(TOFB_BROKEN_TABLE))
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
    shenSetClockAndCompassRenderTo(0);
    sbtLoadProgram();
    EV_spmBeginSampleExecute();
    EV_FreeAuthorPak(0, "0048");
    EV_FreeAuthorPak(0, "0047");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("TOFB"), 0);
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0048"), 0);
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
#endif
