#include "s2/areas/area_0047.hpp"
#include "s2/character.hpp"
#include "s2/game_runtime.hpp"

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
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/event_queued.hpp"
#include "s2/particle_effects.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t e0047Fire;
void ConfigureE0047Fire();

void E0047MainCallback(void *taskData);
void E0047LoopCallback(void *taskData);

// @verified - p
void E0047Init() {
    e0047Fire = 0;
}

// @verified - p
void ConfigureE0047Fire() {
    e0047Fire = static_cast<std::uint32_t>(CreateFireEffect(Vector3{218.17f, 105.39f, 212.44f}, 0.5f, 0.5f, 0, 0));
    SetFirePalette(e0047Fire, AREA_0047_PALETTE);
    SetFireFalloffScalar(e0047Fire, -0.05f);
    SetFireFalloff(e0047Fire, 3.14f, 4.94f, 0.78f);
    SetParticleEffectSize(4, e0047Fire, 0.025f, 0.05f);
    ResumeParticleEffect(4, e0047Fire);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0047");
}

// @verified - p
void E0047Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0047MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0047_MOTION));
    parent->selection = 0;
    parent->state = 0;
    EV_stepSetFlags(0, 0x55);
    ConfigureE0047Fire();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0047_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0047Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_CHO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0047_CHO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_SBO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0047_SBO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_MRC_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0047_MRC_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_SCX_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0047_SCX_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_CHR_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0047_CHR_, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A020, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A022, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A021, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A418, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A420, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A421, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_A412, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_AA42, false);
    EV_SetCharacterAlphaEnabled(AREA_0047_AA43, false);
}

// @verified - p
void RestoreE0047Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_CHO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0047_CHO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_SBO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0047_SBO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_MRC_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0047_MRC_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_SCX_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0047_SCX_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0047_CHR_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0047_CHR_, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A020, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A022, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A021, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A418, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A420, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A421, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_A412, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_AA42, true);
    EV_SetCharacterAlphaEnabled(AREA_0047_AA43, true);
}

// @verified - p
void CleanupE0047(AreaAuthorTaskParameter *parent) {
    FreeParticleEffect(4, e0047Fire);
    EV_FreeAuthorPak(0, AREA_0047_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0047_TOFB, 0);
}

// @verified - p
void E0047MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0047LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
        AreaAuthorFrameTaskParameter *const child = static_cast<AreaAuthorFrameTaskParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->currentFrame = 0;
        SetupE0047Characters();
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        CleanupE0047(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 0;
        } else if (input == 128 && ++parent->selection > 0)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0047_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0047LoopCallback(void *taskData) {
    if (EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 925) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(EVT_AT_PlayDynamix_Callback, 6, 0x0b, 0x30, AREA_AUTHOR_TASK);
        RegisterEventTask(task);
        EventPlayDynamics *const dynamics = static_cast<EventPlayDynamics *>(HLib::GetTaskParameter(task));
        dynamics->resource = "MICS";
        dynamics->parts = AREA_0047_DYNAMICS_PARTS;
        dynamics->character = AREA_0047_DYNAMICS_CHARACTER;
        dynamics->endFrame = 108.0f;
        dynamics->trackCount = 84;
        dynamics->frameStep = 2.0f;
        dynamics->currentFrame = 2.0f;
        dynamics->delay = 0.0f;
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    RestoreE0047Characters();
    static_cast<AreaAuthorFrameTaskParameter *>(taskData)->parent->state = 0;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0047ScriptDefinition{.areaId = AREA_0047_AREA, .field04 = 0, .init = E0047Init, .main = E0047Main};

} // namespace shenmue::s2::areas
