#include "s2/areas/area_0262.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/inventory.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/sound_program.hpp"
#include "s2/camera.hpp"
#include "s2/event_queued.hpp"
#include "s2/fog.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0262MainCallback(void *taskData);
void E0262LoopCallback(void *taskData);

// @verified - p
void E0262Init() {
}

// @verified - p
void ConfigureE0262Environment() {
    EV_SetWeatherInstruction(12, 0);
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0262_L001);
    PushGlobalLightingProgram();
    EV_SetNewSceneIndex();
    EV_LoadCutsceneFromCurrentScene("0262A000");
    EV_FreeAuthorPak(1, AREA_0262_CONVERSATION);
}

// @verified - p
void E0262Main(std::uintptr_t argument) {
    DateTime time;
    GetCurrentDateTime(time);
    SetPlayerControlEnabled(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 32, time.hour);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 33, time.minute);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 34, static_cast<std::int8_t>(time.day));
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 48, time.year);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, 49, static_cast<std::int8_t>(time.month));
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0262MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_currentMap.name == AREA_0262_AREA.name ? static_cast<std::int16_t>(g_entryPoint > 1 ? 0 : g_entryPoint) :
                                                                   *reinterpret_cast<const std::int16_t *>(argument);
    parent->state = 1;
    ConfigureE0262Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0262_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0262Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0262_A276, false);
    EV_SetCharacterAlphaEnabled(AREA_0262_AA80, false);
    EV_SetCharacterAlphaEnabled(AREA_0262_AA77, false);
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0262Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0262_A276, true);
    EV_SetCharacterAlphaEnabled(AREA_0262_AA80, true);
    EV_SetCharacterAlphaEnabled(AREA_0262_AA77, true);
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0262(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0262_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0262_CONVERSATION);
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("0262A"), 0);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_NPC_NewLoadSw(1);
    if (g_currentMap.name == AREA_0262_AREA.name)
        EV_scenQueueLoad(g_actNumber, AREA_0262_AREA, g_entryPoint);
    else {
        LoadCutsceneAndSoundProgramForArea();
        EventSetPosition position{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {215.3f, 92.8f, 129.8f}, 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetRawAngle rotation{0, AREA_AUTHOR_RYO, AREA_AUTHOR_INVALID, {0, 49152, 0}, 0.0f};
        EVT_AT_SetAng_Callback(&rotation);
        SetMapModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0262_0000);
        PushGlobalLightingProgram();
        SetAreaFogParams();
        camCtrlSetFovTransitionTo(true);
        camCtrlSetState(CameraState::Init);
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        BeginSecondarySampleExecution(0);
    }
}

// @verified - p
void E0262MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0262LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
        child->state = 0;
        child->parent = parent;
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        BeginSecondarySampleExecution(0);
        SetupE0262Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(AREA_0262_AUTHORS[parent->selection].name));
        parent->state = 2;
    } else if (parent->state == 3)
        CleanupE0262(parent);
}

// @verified - p
void E0262LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::SetupTimedTask(static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->self, 130);
    AddInventoryItem(0x1787, 1, 1);
    RestoreE0262Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0262ScriptDefinition{.areaId = AREA_0262_AREA, .field04 = 0, .init = E0262Init, .main = E0262Main};

} // namespace shenmue::s2::areas
