#include "s2/areas/area_0010.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/event_runtime.hpp"
#include "s2/input.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include <bit>
namespace shenmue::s2::areas {
namespace {
constexpr shen_name characters[]{SHEN_NAME(GOI_), SHEN_NAME(HTK_), AREA_AUTHOR_INVALID};
constexpr shen_name props[3][7]{{SHEN_NAME(a880), SHEN_NAME(a882), SHEN_NAME(a884), SHEN_NAME(a886), SHEN_NAME(a878), SHEN_NAME(a193), SHEN_NAME(a194)},
    {SHEN_NAME(a712), SHEN_NAME(a708), SHEN_NAME(a710), SHEN_NAME(a706), SHEN_NAME(a894), SHEN_NAME(a003), SHEN_NAME(a022)},
    {SHEN_NAME(a058), SHEN_NAME(a059), SHEN_NAME(a060), SHEN_NAME(a061), SHEN_NAME(a127), SHEN_NAME(ab00), SHEN_NAME(ab01)}};
constexpr const char *debugNames[]{
    "0010A000.c\n", "0010A020.c\n", "0010A030.c\n", "0010A040.c\n", "0010A050.c\n", "0010A060.c\n", "0010A070.c\n", "0010A080.c\n", "0010A090.c\n"};
void MainCallback(void *);
void LoopCallback(void *);
// @verified - p
void InitNullsub() {
}
void SetCharacters(std::int16_t selection, bool restore) {
    for (shen_name name : {AREA_AUTHOR_RYO, characters[0], characters[1]}) {
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, restore ? 0 : 2);
        EV_SetCharacterAlphaEnabled(name, restore);
    }
    if (selection < 0 || selection > 8)
        return;
    for (shen_name name : props[selection / 3]) {
        if (restore) {
            EV_ToggleCharacterFlag(name);
            EV_DisableCharacterDisplay(name);
        } else {
            EV_EnsureCharacterDraw(name);
            EV_EnableCharacterDisplay(name);
        }
        EV_SetCharacterAlphaEnabled(name, restore);
    }
}
// @verified - p
void SetupCharacters(std::int16_t selection) {
    SetCharacters(selection, false);
}
// @verified - p
void RestoreCharacters(std::int16_t selection) {
    SetCharacters(selection, true);
}
// @verified - p
void SelectAuthor(std::int32_t selection) {
    for (const char *name : {"AR03", "WN00", "WS00"})
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
    if (selection < 0 || selection > 8)
        return;
    constexpr const char *names[]{"WS00", "AR03", "WN00"};
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>(names[selection / 3]));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}
// @verified - p
void Cleanup(AreaAuthorTaskParameter *parent) {
    for (const shen_name *name = characters; name->name != AREA_AUTHOR_INVALID.name; ++name)
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    for (const char *name : {"AR02", "AK00", "WS00"})
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(name), 0))
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(name), 0);
    EV_FreeAuthorPak(0, "0010");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
// @verified - p
void QueueLoop(AreaAuthorTaskParameter *parent) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    static_cast<AreaAuthorLoopTaskParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    SelectAuthor(parent->selection);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupCharacters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void LoopCallback(void *data) {
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    auto *parent = static_cast<AreaAuthorLoopTaskParameter *>(data)->parent;
    RestoreCharacters(parent->selection);
    parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}
// @verified - p
void MainCallback(void *data) {
    auto *parent = static_cast<AreaAuthorTaskParameter *>(data);
    if (parent->state == 1)
        QueueLoop(parent);
    else if (parent->state == 3)
        Cleanup(parent);
    else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 8;
        } else if (input == 128) {
            if (++parent->selection > 8)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 8) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(debugNames[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}
// @verified - p
void Main(std::uintptr_t) {
    HLib::Task *task = HLib::EnqueueTaskWithParameter(MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    auto *parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0010A.MOT"));
    parent->selection = 0;
    parent->state = 0;
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->self);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0010");
}
} // namespace
const ScriptAreaDefinition area0010ScriptDefinition{.areaId = SHEN_NAME(0010), .field04 = 0, .init = InitNullsub, .main = Main};
} // namespace shenmue::s2::areas
