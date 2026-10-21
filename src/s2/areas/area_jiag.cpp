#include "s2/areas/area_jiag.hpp"
#include "s2/pak.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fight.hpp"
#include "s2/fight_stage.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include <array>
#include <cstring>

namespace shenmue::s2::areas {
namespace {
std::int32_t battleWon;
HLib::Task *eventTask;
std::int64_t savedSoundProgramIndex;
std::array<char, 16> savedBgm, savedSbt;
DateTime savedDate;
std::uint32_t battleWaitFrames;
struct BattlePlacement {
    shen_name character;
    float yaw;
    std::uint32_t motion;
    std::int32_t resetMotionRegion;
};
static_assert(sizeof(BattlePlacement) == 16);
constexpr BattlePlacement battlePlacements[5]{{JIAG_PLAYER, 261.3999938964844f, 306, 1}, {JIAG_CHARACTERS[1], 205.0f, 3636, 1},
    {JIAG_CHARACTERS[2], 258.79998779296875f, 3640, 1}, {JIAG_CHARACTERS[3], 358.6000061035156f, 3644, 1}, {JIAG_CHARACTERS[4], 172.10000610351562f, 3636, 0}};
constexpr shen_name battleOpponents[4]{JIAG_CHARACTERS[1], JIAG_CHARACTERS[2], JIAG_CHARACTERS[3], JIAG_CHARACTERS[4]};
constexpr std::uint8_t battleStyles[4]{3, 4, 5, 3};
constexpr shen_name inactiveCharacters[3]{JIAG_CHARACTERS[5], JIAG_CHARACTERS[6], JIAG_CHARACTERS[0]};
constexpr shen_name retiredOpponents[6]{JIAG_CHARACTERS[1], JIAG_CHARACTERS[2], JIAG_CHARACTERS[3], JIAG_CHARACTERS[4], JIAG_CHARACTERS[5], JIAG_CHARACTERS[6]};
void EnableNightPluralControl() {
    if (!EV_GetShenmueFlag(0xff))
        return;
    SetPluralControlFor(FindCharacterByScenarioName(JIAG_PLAYER), 1);
    for (const shen_name character : JIAG_CHARACTERS) {
        if (character.name == JIAG_INVALID.name)
            break;
        SetPluralControlFor(FindCharacterByScenarioName(character), 1);
    }
}
void DebugJiagEntry() {
}
} // namespace
// @verified - p
void AreaJIAGInit() {
    battleWon = 0;
    eventTask = nullptr;
    savedSoundProgramIndex = 0;
    savedBgm.fill(0);
    savedSbt.fill(0);
    battleWaitFrames = 0;
}
// @verified - p
void AreaJIAGMain(std::uintptr_t) {
    JIAGPrepareEvent();
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::SetCurrentNamed, reinterpret_cast<std::uintptr_t>("JIAGEYA"), 0);
    eventTask = HLib::EnqueueTaskWithoutParameter(JIAGPlayPreAuthorCallback, 4, 0x0b, static_cast<std::int32_t>(JIAG_TASK.name));
    GetCurrentDateTime(savedDate);
    AssignAreaNpcLoadContext(JIAG_CHARACTERS, eventTask);
    g_usingCustomAuthorCharacterList = 0;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (eventTask)
        HLib::SetupTimedTask(eventTask, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(JIAG_LOAD_TASK.name));
}
// @verified - p
void JIAGPrepareEvent() {
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(JIAG_PLAYER), 0xf003, MotionRegion::LowerBody);
    battleWon = 1;
    eventTask = nullptr;
    savedSoundProgramIndex = GetSelectedSoundSceneIndex();
    std::memcpy(savedBgm.data(), GetCurrentBgm(), savedBgm.size());
    std::memcpy(savedSbt.data(), shenGetCurrentSbt(), savedSbt.size());
    EV_LoadCutsceneFromCurrentScene("0030A000");
    EV_FreeAuthorPak(1, "0030");
    DebugJiagEntry();
    SetScenarioFollowCharacter({});
    ReleaseNpcScriptRouteOwner();
}
// @verified - p
void JIAGPlayPreAuthorCallback(void *) {
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(JIAG_START_AUTHOR.name));
    camCtrlSetFarDistance(50.0f);
    HLib::SetCurrentTaskTo(JIAGWaitPreAuthorCallback);
}
// @verified - p
void JIAGWaitPreAuthorCallback(void *) {
    EnableNightPluralControl();
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (EV_authSceneCtrl(AuthSceneCtrl::GetFramesRemaining) - frame == 30)
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
        JIAGPositionBattleCharacters();
        HLib::SetCurrentTaskTo(JIAGStartBattleCallback);
    }
}
// @verified - p
void JIAGPositionBattleCharacters() {
    for (const BattlePlacement &placement : battlePlacements) {
        mtwkRequestMotion(scenFindMotionwork(placement.character), placement.motion, MotionRegion::LowerBody);
        if (placement.resetMotionRegion)
            SetMotionRegionFlag(scenFindMotionwork(placement.character), 69, 0, MotionRegion::LowerBody);
        const std::uint16_t yaw = static_cast<std::uint16_t>(static_cast<std::int32_t>((placement.yaw * 8192.0f) / 45.0f));
        EV_AT_SetAngY(placement.character, EventTransformFlags::AllAxes, Vector3i{0, yaw, 0});
    }
}
// @verified - p
void JIAGStartBattleCallback(void *) {
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio("0030A000", 1);
    EVS_LoopCamera(0, JIAG_PLAYER, JIAG_PLAYER);
    shenStartFGST(10, 10, 0, 0);
    shenInitFightData(FindCharacterByScenarioName(JIAG_PLAYER), nullptr, 0, 0, 0, 0);
    for (std::size_t index = 0; index < 4; ++index) {
        shenInitFightData(FindCharacterByScenarioName(battleOpponents[index]), nullptr, 1, 1, 120, battleStyles[index]);
        vfUpdateFlagsFor(FindCharacterByScenarioName(battleOpponents[index]), 0, 0x400000, 128);
        vfUpdateFlagsFor(FindCharacterByScenarioName(battleOpponents[index]), 0, 0x200000, 2048);
    }
    for (const shen_name character : inactiveCharacters)
        EV_DisableCharacterDisplay(character);
    for (const shen_name character : inactiveCharacters)
        EV_ToggleCharacterFlag(character);
    SetPluralControlFor(FindCharacterByScenarioName(JIAG_PLAYER), 1);
    vfCopyPosFromEntity(0, nullptr, 78.30000305175781f, 296.20001220703125f, 8.0f);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x100}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x4000}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::FightModeFlags, shen_name{2}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x800}, 1);
    EV_vfBattleUtil(FightBattleUtilOp::StageBattleFlags, shen_name{0x80000}, 1);
    vfmQueueFightStart(6, 2, 1.0f, 1, -1);
    HLib::SetCurrentTaskTo(JIAGWaitBattleCallback);
    battleWaitFrames = 0;
}
// @verified - p
void JIAGWaitBattleCallback(void *) {
    ++battleWaitFrames;
    if (IsFightActive())
        return;
    EV_LoadAudio("0030A000", 2);
    for (const shen_name character : retiredOpponents) {
        EV_SetCharaControllerFlags(character, ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(character, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    }
    for (const shen_name character : retiredOpponents)
        EV_SetCharacterAlphaEnabled(character, false);
    if (GetBtlStatus(FindCharacterByScenarioName(JIAG_PLAYER)) == 3) {
        battleWon = 1;
        HLib::SetCurrentTaskTo(JIAGPlayWinAuthorCallback);
    } else {
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        spmFadeOut();
        battleWon = 0;
        HLib::SetCurrentTaskTo(JIAGRetryCallback);
    }
}
// @verified - p
void JIAGPlayWinAuthorCallback(void *) {
    for (const shen_name character : inactiveCharacters)
        EV_EnableCharacterDisplay(character);
    for (const shen_name character : inactiveCharacters)
        EV_EnsureCharacterDraw(character);
    SetPluralControlFor(FindCharacterByScenarioName(JIAG_PLAYER), 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(JIAG_WIN_AUTHOR.name));
    camCtrlSetFarDistance(40.0f);
    HLib::SetCurrentTaskTo(JIAGWaitWinAuthorCallback);
}
// @verified - p
void JIAGRetryCallback(void *) {
    if (!filmCheckFadeTrackColour()) {
        Queue_ST_LoadTitleFadeIn(JIAG_RETRY_TITLE);
        HLib::SetCurrentTaskTo(JIAGRetryReturnCallback);
        HLib::SetupTaskDelay(120);
    }
}
// @verified - p
void JIAGRetryReturnCallback(void *) {
    EV_EnableCharacterDisplay(JIAG_CHARACTERS[0]);
    EV_EnableCharacterDisplay(JIAG_CHARACTERS[5]);
    EV_EnableCharacterDisplay(JIAG_CHARACTERS[6]);
    EV_EnsureCharacterDraw(JIAG_CHARACTERS[0]);
    EV_EnsureCharacterDraw(JIAG_CHARACTERS[5]);
    EV_EnsureCharacterDraw(JIAG_CHARACTERS[6]);
    ST_LoadTitleFade();
    SetCurrentDateTimeTo(savedDate);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(-16));
    HLib::SetCurrentTaskTo(JIAGPlayPreAuthorCallback);
}
// @verified - p
void JIAGWaitWinAuthorCallback(void *) {
    EnableNightPluralControl();
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetPluralControlFor(FindCharacterByScenarioName(JIAG_PLAYER), 0);
        HLib::SetCurrentTaskTo(JIAGFinishCallback);
    }
}
// @verified - p
void JIAGFinishCallback(void *) {
    if (!filmCheckFadeTrackColour())
        JIAGReleaseEvent();
}
// @verified - p
void JIAGReleaseEvent() {
    using enum CameraState;
    FinishLoading();
    for (const shen_name character : JIAG_CHARACTERS) {
        if (character.name == JIAG_INVALID.name)
            break;
        if (FindCharacterByScenarioName(character)) {
            EV_npcLockUnlock(character);
            EV_npcFree(character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("JIAGEYA"), 0);
    EV_FreeAuthorPak(0, "0030");
    EV_stepSetFlags(battleWon == 0, 0x23);
    EV_NPC_NewLoadSw(1);
    EV_AT_SetPos(JIAG_PLAYER, EventTransformFlags::World | EventTransformFlags::AllAxes, Vector3{67.41999816894531f, 102.4800033569336f, 300.25f});
    EV_AT_SetAngY(JIAG_PLAYER, EventTransformFlags::AllAxes, Vector3i{0, 6342, 0});
    SetPlayerControlEnabled(1);
    camCtrlSetState(Init);
    camCtrlSetFovTransitionTo(true);
    EV_LoadCutsceneFromCurrentScene(savedBgm.data());
    EV_LoadAudio(savedSbt.data(), savedSoundProgramIndex);
    EV_spmBeginSampleExecute();
    CleanupMapControlCharacters();
    HLib::EV_HLKillOtherHierarchyTask(eventTask);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
}
const ScriptAreaDefinition areaJIAGScriptDefinition{.areaId = AREA_JIAG, .field04 = 0, .init = AreaJIAGInit, .main = AreaJIAGMain};
} // namespace shenmue::s2::areas
