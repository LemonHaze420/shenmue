#include "s2/areas/area_0517.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/weather.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {

namespace {
HLib::Task *firefly0517Task;
std::uint16_t firefly0517Frame;
} // namespace

// @verified - p
void Initialize0517() {
    firefly0517Task = nullptr;
    firefly0517Frame = 0;
}

// @verified - p
void Start0517(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0517, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0517_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0517_CONVERSATION);
    WSCN_LoadWeatherData(Filepath_Gen(FilepathGenMode::SceneDirectory), AREA_0517_WEATHER);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0517_CHARACTERS, parent->self);
}

// @verified - p
void Update0517(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0517Author(parent);
    } else if (parent->state == 3) {
        Cleanup0517(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024) {
            parent->state = 1;
            Select0517Package(parent->selection);
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 4;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 4)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 4) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0517_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%x.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0517(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0517_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0517_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0517_RETURN, 0);
}

// @verified - p
std::int64_t Queue0517Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0517AuthorTask, 4, 0x0b, sizeof(Area0517LoopParameter), AUTHOR_VIEWER_TASK);
    Area0517LoopParameter *const child = static_cast<Area0517LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    if (parent->selection == 3)
        firefly0517Task = HLib::EnqueueTaskWithoutParameter(Update0517Fireflies, 4, 0x0b, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(1, 0, 0, parent->selection == 4 ? 1.0f : 0.0f);
    EV_UpdateWeather();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0517Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0517Author(Area0517LoopParameter *child) {
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0517Characters();
        child->parent->state = 0;
        if (child->parent->selection == 3)
            HLib::DestroyTaskInternal(firefly0517Task);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0517AuthorTask(void *taskData) {
    Update0517Author(static_cast<Area0517LoopParameter *>(taskData));
}

// @verified - p
void Prepare0517Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0517_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0517_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0517_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0517_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a988, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a990, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a992, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a994, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a840, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a842, false);
    EV_SetCharacterAlphaEnabled(AREA_0517_a844, false);
}

// @verified - p
void Restore0517Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0517_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0517_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0517_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0517_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a988, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a990, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a992, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a994, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a840, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a842, true);
    EV_SetCharacterAlphaEnabled(AREA_0517_a844, true);
}

// @verified - p
void Select0517Package(std::int32_t selection) {
    DateTime time;
    GetCurrentDateTime(time);
    time.minute = 0;
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE0), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE0), 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE1), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE1), 0);
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE2), 0))
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(AREA_0517_PACKAGE2), 0);
    const char *package;
    if (selection != 0) {
        time.hour = 18;
        package = static_cast<std::uint32_t>(selection) - 1U <= 1U ? AREA_0517_PACKAGE1 : AREA_0517_PACKAGE2;
    } else {
        time.hour = 7;
        time.minute = 40;
        package = AREA_0517_PACKAGE0;
    }
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    SetCurrentDateTimeTo(time);
    EV_SetModelDataDefaultVertexShader();
    switch (selection) {
    case 1:
        EV_SetObjectFlagByIndex(2, 0);
        return;
    case 3:
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(AREA_0517_LIGHT3);
        PushGlobalLightingProgram();
        return;
    case 4:
        EV_SetWeatherInstruction(12, 0);
        EV_lightLoad(AREA_0517_LIGHT4);
        PushGlobalLightingProgram();
        return;
    default:
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        return;
    }
}

// @verified - p
void Update0517Fireflies(void *) {
    if (firefly0517Frame == 180) {
        firefly0517Frame = 0;
        return;
    }
    const std::int32_t angle = static_cast<std::int32_t>(static_cast<float>(firefly0517Frame) * 8192.0f / 45.0f);
    const float alpha = sin_fixed16(static_cast<std::int16_t>(angle));
    SetScenarioCharacterTransparency(AREA_0517_a850, alpha);
    SetScenarioCharacterTransparency(AREA_0517_a852, alpha);
    SetScenarioCharacterTransparency(AREA_0517_a856, alpha);
    firefly0517Frame = static_cast<std::uint16_t>(firefly0517Frame + 4U);
}

const ScriptAreaDefinition area0517ScriptDefinition{.areaId = AREA_0517, .field04 = 0, .init = Initialize0517, .main = Start0517};

} // namespace shenmue::s2::areas
