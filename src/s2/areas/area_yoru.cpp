#include "s2/areas/area_yoru.hpp"
#include "s2/areas/area_0061.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2 {
// @verified - p
void YoruInit() {
}
// @verified - p
void YoruMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(YoruAuthorCallback, 4, 0x0b, sizeof(YoruAuthorWork), YORU_TASK);
    YoruAuthorWork *const work = static_cast<YoruAuthorWork *>(HLib::GetTaskParameter(task));
    work->task = task;
    work->state = 1;
    if (g_currentMap.name == YORU_SPRING_MAP.name) {
        work->authorCommand = static_cast<std::int16_t>(static_cast<std::uint32_t>(g_entryPoint) > 2 ? 0 : g_entryPoint);
        LoadYoruMapForEntry(static_cast<std::int8_t>(work->authorCommand));
    } else if (g_currentMap.name == YORU_TOWER_MAP.name) {
        work->authorCommand = static_cast<std::int16_t>(static_cast<std::uint32_t>(g_entryPoint) > 1 ? 0 : g_entryPoint);
        const char *const package = work->authorCommand == 0 ? "Q300" : "QTB4";
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(package));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    } else
        work->authorCommand = 0;
    InitializeYoruNightScene();
}
// @verified - p
void LoadYoruMapForEntry(std::int8_t entry) {
    const char *package;
    if (entry == 0)
        package = "AR01";
    else if (entry == 1)
        package = "AR02";
    else if (entry == 2)
        package = "ARSF";
    else
        return;
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
// @verified - p
void InitializeYoruNightScene() {
    SetScreenFilmFade(0, 255, 0, 0, 0, 0, 0, 0, 0);
    SetPlayerControlEnabled(0);
    DateTime time;
    GetCurrentDateTime(time);
    time.hour = 19;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    if (g_currentMap.name == YORU_SPRING_MAP.name && g_entryPoint != 0) {
        if (g_entryPoint == 1) {
            EV_lightLoad(YORU_LIGHT_ONE);
            HLib::EnqueueTaskWithoutParameter(areas::E0061SpringCallback, 3, 2, static_cast<std::int32_t>(YORU_TASK.name));
        } else
            EV_lightLoad(YORU_LIGHT_TWO);
    } else
        EV_lightLoad(g_currentMap.name == YORU_TOWER_MAP.name && g_entryPoint != 0 ? YORU_LIGHT_ONE : YORU_LIGHT_ZERO);
    PushGlobalLightingProgram();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, YORU_AREA);
    EV_DisableCharacterDisplay(YORU_PLAYER);
}
// @verified - p
void YoruAuthorCallback(void *data) {
    YoruAuthorWork *const work = static_cast<YoruAuthorWork *>(data);
    if (work->state == 1) {
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(static_cast<std::int32_t>(work->authorCommand)));
        HLib::SetCurrentTaskTo(YoruAuthorLoopCallback);
        work->state = 2;
    } else if (work->state == 3) {
        HLib::DestroyTask(work->task);
        SetCharaKill(YORU_AREA, 1);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - p
void YoruAuthorLoopCallback(void *data) {
    YoruAuthorWork *const work = static_cast<YoruAuthorWork *>(data);
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        work->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::SetCurrentTaskTo(YoruAuthorCallback);
    }
}
const ScriptAreaDefinition yoruScriptDefinition{YORU_AREA, 0, YoruInit, YoruMain};
} // namespace shenmue::s2
