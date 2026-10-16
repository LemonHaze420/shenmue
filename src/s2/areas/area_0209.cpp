#include "s2/areas/area_0209.hpp"
#include "s2/lighting.hpp"

#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/system_modules.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {

constexpr shen_name eventToken{SHEN_NAME(EVNT)};
constexpr shen_name ryo{SHEN_NAME(RYO_)};
constexpr shen_name ren{SHEN_NAME(REN_)};
constexpr shen_name characters[]{ren, SHEN_NAME(SOY_), shen_name{0xffffffffU}};
constexpr shen_name object{SHEN_NAME(a330)}; // Assembly immediate 0x30333361.


struct EventParameter {
    HLib::Task *task;
    std::uint32_t motion;
    std::int16_t sequence;
    std::int16_t state;
};
struct AuthorLoopParameter {
    EventParameter *parent;
};
static_assert(sizeof(EventParameter) == 0x10);
static_assert(offsetof(EventParameter, motion) == 8);
static_assert(offsetof(EventParameter, sequence) == 0x0c);
static_assert(offsetof(EventParameter, state) == 0x0e);
static_assert(sizeof(AuthorLoopParameter) == 8);

void SetAuthorCharacters(bool restore) {
    const std::uint32_t flag = restore ? 0 : 2;
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(ryo)), 6, flag);
    EV_SetCharacterAlphaEnabled(ryo, restore);
    EV_SetCharacterAlphaEnabled(object, restore);
    for (const shen_name *character = characters; character->name != 0xffffffffU; ++character) {
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(*character)), 6, flag);
        EV_SetCharacterAlphaEnabled(*character, restore);
    }
}



void AuthorLoop(void *taskData) {
    const auto *child = static_cast<const AuthorLoopParameter *>(taskData);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    EV_cuffsSetFlags();
    SetAuthorCharacters(true);
    child->parent->state = 0;
    StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 0, 0, 255);
    HLib::DestroyCurrentTask();
}

// 1402db860. CUFF's fifth argument is the DWORD at 1409cfc4c (0x3e99999a).
void QueueAuthorLoop(EventParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoop, 4, 0x0b, sizeof(AuthorLoopParameter), eventToken);
    static_cast<AuthorLoopParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_LoadAudio("0209A000", 0);
    StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 30, 255, 0);
    EV_cuffsAttach(ryo, 1, ren, 0, 0.30000001192092896f);
    EV_SetModelDataDefaultVertexShader();
    if (parent->sequence == 0)
        EV_lightLoad(SHEN_NAME(l402));
    else if (parent->sequence == 1)
        EV_lightLoad(SHEN_NAME(l601));
    PushGlobalLightingProgram();
    SetAuthorCharacters(false);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->sequence));
    parent->state = 2;
}

// 1402dba90: retained even though playback returns to the selector, not state 3.
void Cleanup(EventParameter *parent) {
    for (const shen_name *character = characters; character->name != 0xffffffffU; ++character) {
        if (!FindCharacterByScenarioName(*character))
            continue;
        EV_npcLockUnlock(*character);
        EV_npcFree(*character);
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>("MAP"), 0);
    EV_FreeAuthorPak(0, "0209");
    DeleteMotion(static_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->task);
    SetPlayerControlEnabled(0);
    const auto module =
        static_cast<SystemModule>(((g_actNumber >> 31) & static_cast<std::int32_t>(SystemModule::Game)) + static_cast<std::int32_t>(SystemModule::Game));
    shenQueueChangeModuleAndScene(module, g_actNumber, g_currentMap, g_entryPoint);
}

// 1402db760: exact equality tests, not bit tests. Keep the two-sequence debug UI.
void EventMain(void *taskData) {
    auto *parent = static_cast<EventParameter *>(taskData);
    if (parent->state == 1)
        QueueAuthorLoop(parent);
    else if (parent->state == 3)
        Cleanup(parent);
    else if (parent->state == 0) {
        const auto input = static_cast<std::int16_t>(g_inputBuffer[0].pressedPriority);
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->sequence < 0)
                parent->sequence = 1;
        } else if (input == 128 && ++parent->sequence > 1)
            parent->sequence = 0;
        if (parent->sequence == 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0209A040.c\n");
        } else if (parent->sequence == 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen("0209A060.c\n");
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->sequence);
    }
}

// 1402db9b0. off8371180 is the map-slot transform wrapper 140569ca0.
void SetupEnvironment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0209");
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
        reinterpret_cast<std::uintptr_t>("MAP"));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_SetWeatherInstruction(12, 0);
    Vector3 position{0.0f, 14.399999618530273f, 0.0f};
    for (std::int32_t slot = 0; slot < 9; ++slot)
        SetMapModelSlotTransform(slot, &position, 0);
    position.y = 10.800000190734863f;
    SetMapModelSlotTransform(2, &position, 0);
}

void AreaMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventMain, 4, 0x0b, sizeof(EventParameter), eventToken);
    auto *parent = static_cast<EventParameter *>(HLib::GetTaskParameter(task));
    parent->task = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0209A.MOT"));
    parent->sequence = 0;
    parent->state = 0;
    SetupEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(characters, parent->task);
}

void AreaInitNullsub() {
}

} // namespace

// g_area_definitions row 70, 140a8f340: genuine 140104200 init nullsub.
const ScriptAreaDefinition area0209ScriptDefinition{.areaId = SHEN_NAME(0209), .field04 = 0, .init = AreaInitNullsub, .main = AreaMain};

} // namespace shenmue::s2::areas
