#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0009.hpp"

static const shen_name scenes0009[] = {SHEN_NAME4('A', 'U', '0', '1'), SHEN_NAME4('A', 'U', '0', '2')};
static shen_name characters0009[] = {SHEN_NAME4('C', 'L', 'J', '_'), SHEN_NAME4('L', 'L', 'Y', '_'), SHEN_NAME4('S', 'A', 'M', '_'),
    SHEN_NAME4('W', 'O', 'N', '_'), INVALID_CHARACTER};

void Main0009(void)
{
    HLibTask *task;
    AreaAuthorTaskParameter *parent;
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_NPC_NewLoadSw_0(0);
    FTBL.EV_ResetFreeTalkFlags();
    task = FTBL.HLib_EnqueueTaskWithParameter(MainCallback0009, 4, 11, sizeof(AreaAuthorTaskParameter), TASK_EVNT);
    parent = (AreaAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
    parent->selection = (unsigned int)g_entryPoint > 1 ? 0 : g_entryPoint;
    ConfigureEnvironment0009();
    Queue_EVT_UTL_LoadNPCChara_3(characters0009, parent->self);
}

void MainCallback0009(void *data)
{
    AreaAuthorTaskParameter *parent = (AreaAuthorTaskParameter *)data;
    switch (parent->state) {
    case 1:
        QueueLoop0009(parent);
        break;
    case 3:
        FTBL.mtwkRequestMotion(FTBL.scenFindMotionwork(AREA_0009_RYO_), 0xf003, 0);
        Cleanup0009(parent);
        break;
    }
}

void QueueLoop0009(AreaAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(LoopCallback0009, 4, 11, sizeof(AreaAuthorLoopTaskParameter), TASK_EVNT);
    ((AreaAuthorLoopTaskParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    switch (parent->selection) {
    case 0:
        FTBL.EV_LoadAudio("0009A010", 0);
    case 1:
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        break;
    }
    FTBL.EV_spmBeginSampleExecute(0);
    SetupCharacters0009();
    FTBL.EV_authSceneCtrl(scenes0009[parent->selection]);
    parent->state = 2;
}

void LoopCallback0009(void *data)
{
    AreaAuthorLoopTaskParameter *child = (AreaAuthorLoopTaskParameter *)data;
    int loaded = FTBL.EV_authSceneCtrl(-1);
    if (!loaded) {
        if (child->parent->selection == 1) {
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            RestoreCharacters0009();
            child->parent->state = 3;
            FTBL.HLib_DestroyCurrentEventTask();
        } else {
            child->parent->selection++;
            FTBL.EV_authSceneCtrl(scenes0009[1]);
        }
    }
}

void ConfigureEnvironment0009(void)
{
    EventDateTime dateTime;
    FTBL.EV_stepSetFlags(5, 0);
    FTBL.SetPlayerStat(2, 0);
    FTBL.SetPlayerGameFlags(4, 0);
    if (g_currentMap == AREA_0009_AREA) {
        FTBL.GetCurrentDateTime((DateTime *)&dateTime);
        if (dateTime.hour > 18) {
            FTBL.EV_lightLoad(AREA_0009_LIGHT);
            FTBL.PushGlobalLightingProgram();
            FTBL.CheckIsArgEquZero(1, 1);
            FTBL.SetObjectFlagByIndex(14, 1);
            FTBL.SetObjectFlagByIndex(15, 0);
            FTBL.EV_nlSetVertexColourType(1);
        } else {
            FTBL.ReinitializeLights(1);
            FTBL.SetLightFlagForIndex(0, 2);
            FTBL.SetLightEntryIndex(0, 1);
            FTBL.SetObjectFlagByIndex(14, 0);
            FTBL.SetObjectFlagByIndex(15, 1);
            FTBL.EV_nlSetVertexColourType(2);
        }
    }
    FTBL.EV_SetNewSceneIndex();
    FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0009A", 0);
    FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
    FTBL.EV_LoadCutsceneFromCurrentScene("0009A010");
    FTBL.EV_FreeAuthorPak(1, "0009");
}

void Cleanup0009(AreaAuthorTaskParameter *parent)
{
    EVT_UTL_FreeNPCChara(characters0009);
    FTBL.EV_FreeAuthorPak(0, "0009");
    FTBL.EV_ControlPackageResources(0, 6, "0009A", 0, 0);
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.SetScenarioFollowCharacter(0);
    if (g_currentMap == AREA_0009_AR02) {
        FTBL.EV_NPC_NewLoadSw_0(1);
        FTBL.EV_LoadCutsceneFromCurrentScene("AR02");
        FTBL.EV_LoadAudio("AR02_000", 2);
        EVT_AT_SetPos(AREA_0009_RYO_, EVENT_CHARACTER_INVALID, 250.2f, 100.0f, 434.8f, 0);
        EVT_AT_SetAng(AREA_0009_RYO_, EVENT_CHARACTER_INVALID, 0.0f, 230.0f, 0.0f, 0);
        FTBL.camCtrlSetFovTransitionTo(1);
        FTBL.camCtrlSetState(0);
        FTBL.SetPlayerControlEnabled(1);
        FTBL.EV_spmBeginSampleExecute(0);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    } else
        FTBL.EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

void SetupCharacters0009(void)
{
    int i;
    SetupAuthorCharacter0009(AREA_0009_RYO_);
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '4'));
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '5'));
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '6'));
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '7'));
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '8'));
    HideCharacter0009(SHEN_NAME4('a', '0', '1', '9'));
    for (i = 0; characters0009[i] != INVALID_CHARACTER; i++)
        SetupAuthorCharacter0009(characters0009[i]);
}

void RestoreCharacters0009(void)
{
    int i;
    RestoreAuthorCharacter0009(AREA_0009_RYO_);
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '4'));
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '5'));
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '6'));
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '7'));
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '8'));
    ShowCharacter0009(SHEN_NAME4('a', '0', '1', '9'));
    for (i = 0; characters0009[i] != INVALID_CHARACTER; i++)
        RestoreAuthorCharacter0009(characters0009[i]);
}

void SetupAuthorCharacter0009(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void RestoreAuthorCharacter0009(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideCharacter0009(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowCharacter0009(shen_name character)
{
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}
#else
#include "s2/areas/area_0009.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/game_runtime.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters[]{SHEN_NAME(CLJ_), SHEN_NAME(LLY_), SHEN_NAME(SAM_), SHEN_NAME(WON_), AREA_AUTHOR_INVALID};
constexpr std::array<shen_name, 6> alphaCharacters{
    SHEN_NAME(a014), SHEN_NAME(a005) | 0x10000U, SHEN_NAME(a016), SHEN_NAME(a016) | 0x1000000U, SHEN_NAME(a010) | 0x8000000U, SHEN_NAME(a009) | 0x10000U};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
void ConfigureEnvironment() {
    shenSetClockAndCompassRenderTo(1);
    EV_stepSetFlags(0, 5);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    DateTime time;
    shenGetCurrentDateTime(time);
    if (g_currentMap.name == SHEN_NAME(0009)) {
        if (time.hour < 19) {
            EV_ReinitLights(1);
            EV_SetLightFlagForIndex(0, 2);
            EV_SetIndexForLightEntry(0, 1);
            EV_SetObjectFlagByIndex(14, 0);
            EV_SetObjectFlagByIndex(15, 1);
            SetLegacyVertexColourType(2);
        } else {
            EV_lightLoad(SHEN_NAME(0000));
            PushGlobalLightingProgram();
            CheckIsArgEquZero(1);
            EV_SetObjectFlagByIndex(14, 1);
            EV_SetObjectFlagByIndex(15, 0);
            SetLegacyVertexColourType(1);
        }
    } else if (time.hour >= 23 || (time.hour == 22 && time.minute >= 30))
        shenSetTimeSimulationTo(0);
    EV_LoadCutsceneFromCurrentScene(AREA_0009_CUE);
    EV_FreeAuthorPak(1, "0009");
}
void SetCharacters(bool restore) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, restore ? 0 : 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, restore);
    for (shen_name name : alphaCharacters)
        EV_SetCharacterAlphaEnabled(name, restore);
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name) {
        mtwkSetExtraFlag(scenFindMotionwork(*name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(*name, restore);
    }
}
// @verified - p
void SetupCharacters() {
    SetCharacters(false);
}
// @verified - p
void RestoreCharacters() {
    SetCharacters(true);
}
void Cleanup(AreaAuthorTaskParameter *parent) {
    const std::uint64_t cash = GetPlayerStat(PlayerStatsId::Cash);
    if (static_cast<std::int64_t>(cash) < 50)
        IncrementSteamStat(12);
    SetPlayerStat(PlayerStatsId::Cash, 0);
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (ctrlGetScenarioChara(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    shenSetTimeSimulationTo(1);
    EV_FreeAuthorPak(0, "0009");
    EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0009A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == SHEN_NAME(AR02))
        SetPlayerControlEnabled(1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
void QueueLoop(AreaAuthorTaskParameter *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    if (parent->selection == 0)
        EV_LoadAudio(AREA_0009_CUE, 0);
    if (parent->selection == 0 || parent->selection == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    SetupCharacters();
    constexpr AuthSceneCtrl scenes[]{AuthSceneCtrl::AU01, AuthSceneCtrl::AU02};
    EV_authSceneCtrl(scenes[parent->selection]);
    parent->state = 3;
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    switch (parent->state) {
    case 0:
        EV_NPC_NewLoadSw(1);
        EV_LoadCutsceneFromCurrentScene("AR02");
        EV_LoadAudio("AR02_000", 2);
        EV_spmBeginSampleExecute();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        shenSetClockAndCompassRenderTo(0);
        parent->state = 1;
        break;
    case 1:
        if (!filmCheckFadeTrackColour())
            parent->state = 4;
        break;
    case 2:
        QueueLoop(parent);
        break;
    case 4:
        Cleanup(parent);
        break;
    default:
        break;
    }
}
// @verified - p
void LoopCallback(void *data) {
    using enum CameraState;
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    auto *parent = static_cast<AreaAuthorLoopTaskParameter *>(data)->parent;
    if (parent->selection == 1) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        RestoreCharacters();
        parent->state = 0;
        mtwkRequestMotion(scenFindMotionwork(AREA_AUTHOR_RYO), 0xf003, MotionRegion::LowerBody);
        EventSetPosition pos{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {250.1999969482422f, 100, 434.79998779296875f}, 0};
        EVT_AT_SetPosRot(&pos);
        EventSetRawAngle angle{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 41870, 0}, 0};
        EVT_AT_SetAng_Callback(&angle);
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(Init);
        HLib::DestroyCurrentTask();
    } else {
        ++parent->selection;
        SetupCharacters();
        EV_authSceneCtrl(AuthSceneCtrl::AU02);
    }
}
// @verified - p
void Main(std::uintptr_t) {
    ReleaseNpcScriptRouteOwner();
    SetScenarioFollowCharacter(shen_name{0});
    SetPlayerControlEnabled(0);
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = static_cast<std::int16_t>(g_entryPoint <= 1 ? g_entryPoint : 0);
    parent->state = 2;
    ConfigureEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
}
} // namespace
const ScriptAreaDefinition area0009ScriptDefinition{.areaId = SHEN_NAME(0009), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
#endif
