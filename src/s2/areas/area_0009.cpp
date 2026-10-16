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
// @verified - p
void ConfigureEnvironment() {
    SetRenderClockAndCompassTo(1);
    EV_stepSetFlags(0, 5);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    DateTime time;
    GetCurrentDateTime(time);
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
    EV_LoadCutsceneFromCurrentScene("0009");
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
// @verified - p
void Cleanup(AreaAuthorTaskParameter *parent) {
    const std::uint64_t cash = GetPlayerStat(PlayerStatsId::Cash);
    if (static_cast<std::int64_t>(cash) < 50)
        IncrementSteamStat(12);
    SetPlayerStat(PlayerStatsId::Cash, static_cast<std::uint32_t>(cash));
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    shenSetTimeSimulationTo(1);
    EV_FreeAuthorPak(0, "0009");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0009A"), 0);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_currentMap.name == SHEN_NAME(AR02))
        SetPlayerControlEnabled(1);
    else
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueLoop(AreaAuthorTaskParameter *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    if (parent->selection == 0)
        EV_LoadAudio("0009", 0);
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
        SetRenderClockAndCompassTo(0);
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
        camCtrlSetState(CameraState::Init);
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
