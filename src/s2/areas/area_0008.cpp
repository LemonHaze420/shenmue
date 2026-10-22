#include "s2/areas/area_0008.hpp"

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
#include "s2/hand.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>
#include <array>

namespace shenmue::s2::areas {
namespace {

void E0008MainCallback(void *taskData);
void E0008LoopCallback(void *taskData);

// @verified - p
std::array<std::byte, 0x100> e0008FirstBuffer, e0008SecondBuffer, e0008ThirdBuffer;
void E0008Init() {
    e0008FirstBuffer.fill(std::byte{});
    e0008SecondBuffer.fill(std::byte{});
    e0008ThirdBuffer.fill(std::byte{});
}

// @verified - p
void ConfigureE0008Environment() {
    SetPlayerControlEnabled(0);
    DateTime time;
    GetCurrentDateTime(time);
    if (time.hour < 19) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        EV_SetObjectFlagByIndex(15, 0);
        EV_SetObjectFlagByIndex(16, 1);
    } else {
        EV_lightLoad(AREA_0008_0000);
        PushGlobalLightingProgram();
        EV_SetObjectFlagByIndex(15, 1);
        EV_SetObjectFlagByIndex(16, 0);
    }
    EV_FreeAuthorPak(1, AREA_0008_CONVERSATION);
}

// @verified - p
void E0008Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0008MainCallback, 4, 0x0b, 0x20, AREA_AUTHOR_TASK);
    E0008TaskParameter *const parent = static_cast<E0008TaskParameter *>(HLib::GetTaskParameter(task));
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0008_MOTION));
    parent->handMotion = LoadHandPoseBank(g_actNumber, g_currentMap, AREA_0008_HAND_MOTION);
    parent->self = task;
    parent->selection = 0;
    parent->state = 0;
    ConfigureE0008Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0008_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0008Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0008_A012, false);
    EV_SetCharacterAlphaEnabled(AREA_0008_A195, false);
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void RestoreE0008Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0008_A012, true);
    EV_SetCharacterAlphaEnabled(AREA_0008_A195, true);
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
void CleanupE0008(E0008TaskParameter *parent) {
    for (const shen_name *character = AREA_0008_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0008_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    ReleaseHandPoseBank(parent->handMotion);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void QueueE0008Loop(E0008TaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0008LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0008LoopParameter *const child = static_cast<E0008LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_LoadAudio("0008A000", parent->selection);
    SetupE0008Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0008MainCallback(void *taskData) {
    E0008TaskParameter *const parent = static_cast<E0008TaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0008Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0008(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, INPUT_PRESSED));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128 && ++parent->selection > 1)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0008_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0008LoopCallback(void *taskData) {
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame)) == 1) {
        HandParams *const firstHand = GetCharacterHand(FindCharacterByScenarioName(AREA_0008_HNG_), 0);
        if (firstHand)
            ConfigureHandPoseSequence(*firstHand, 1025, 0, 1, 0, 4, 1.0f, 0);
        HandParams *const secondHand = GetCharacterHand(FindCharacterByScenarioName(AREA_0008_SYO_), 0);
        if (secondHand)
            ConfigureHandPoseSequence(*secondHand, 1033, 0, 1, 0, 4, 1.0f, 0);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    static_cast<E0008LoopParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0008Characters();
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0008ScriptDefinition{.areaId = AREA_0008_AREA, .field04 = 0, .init = E0008Init, .main = E0008Main};

} // namespace shenmue::s2::areas
