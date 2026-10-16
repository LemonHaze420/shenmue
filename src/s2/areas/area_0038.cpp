#include "s2/areas/area_0038.hpp"
#include "s2/character.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"

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
#include "s2/particle_effects.hpp"
#include "s2/event_characters.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <bit>

namespace shenmue::s2::areas {
namespace {

void E0038MainCallback(void *taskData);
void E0038LoopCallback(void *taskData);

// @verified - p
void E0038Init() {
}

// @verified - p
void E0038Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0038MainCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0038_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0038_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0038_CHARACTERS, parent->self);
}

// @verified - p
void SetupE0038Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0038_CHO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0038_CHO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0038_KEK_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0038_KEK_, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A058, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A059, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A060, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A061, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_AA62, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_AA65, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A476, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A057, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A037, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A420, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A421, false);
    EV_SetCharacterAlphaEnabled(AREA_0038_A127, false);
}

// @verified - p
void RestoreE0038Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0038_CHO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0038_CHO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0038_KEK_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0038_KEK_, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A058, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A059, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A060, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A061, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_AA62, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_AA65, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A476, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A057, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A037, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A420, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A421, true);
    EV_SetCharacterAlphaEnabled(AREA_0038_A127, true);
}

// @verified - p
void CleanupE0038(AreaAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0038_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0038_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
void LoadE0038ScenePackages(std::int32_t selection) {
    const char *const oldPackage = selection != 0 ? "WNS4" : "WKA0";
    const char *const newPackage = selection != 0 ? "WKA0" : "WNS4";
    if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(oldPackage), 0) != 0)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(oldPackage), 0);
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(newPackage));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    EV_ControlPackageResources(PakCategory::Task, PakTaskOp::LoadNamed, reinterpret_cast<std::uintptr_t>(newPackage), 0);
}

// @verified - p
void InitializeE0038Smoke(E0038LoopParameter *child) {
    const Vector3 position{0.0f, 0.0f, 0.0f}, volume{0.0f, 0.0f, 0.0f};
    const Vector3 velocity{0.0f, 0.009803921915590763f, 0.0f}, variance{0.0f, 0.0f, 0.0f};
    child->smoke = static_cast<std::uint32_t>(CreateSmokeEffect(position, volume, 50, 0.08f, shen_name{0}, 0));
    SetSmokeEffectFadeFactor(child->smoke, 8);
    SetSmokeEffectFalloff(child->smoke, 255);
    SetParticleEffectColour(0, child->smoke, 0, 193, 193, 191);
    SetParticleEffectInitialScale(0, child->smoke, 0.35686275362968445f);
    SetParticleEffectSize(0, child->smoke, 0.3921568691730499f, 0.4313725531101227f);
    SetParticleEffectBirthRate(0, child->smoke, 0.052941177040338516f);
    SetParticleEffectVelocity(0, child->smoke, velocity, variance);
    ResumeParticleEffect(0, child->smoke);
}

// @verified - p
void QueueE0038Loop(AreaAuthorTaskParameter *parent) {
    LoadE0038ScenePackages(parent->selection);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0038LoopCallback, 4, 0x0b, 0x10, AREA_AUTHOR_TASK);
    E0038LoopParameter *const child = static_cast<E0038LoopParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetMapModelDataDefaultVertexShader();
    EV_lightLoad(parent->selection == 0 ? AREA_0038_0000 : AREA_0038_0001);
    PushGlobalLightingProgram();
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    SetupE0038Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}

// @verified - p
void E0038MainCallback(void *taskData) {
    AreaAuthorTaskParameter *const parent = static_cast<AreaAuthorTaskParameter *>(taskData);
    if (parent->state == 1) {
        QueueE0038Loop(parent);
    } else if (parent->state == 3) {
        CleanupE0038(parent);
    } else if (parent->state == 0) {
        const std::int16_t input = static_cast<std::int16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024)
            parent->state = 1;
        else if (input == 64) {
            if (--parent->selection < 0)
                parent->selection = 3;
        } else if (input == 128 && ++parent->selection > 3)
            parent->selection = 0;
        if (parent->selection >= 0 && parent->selection <= 3) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0038_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", parent->selection);
    }
}

// @verified - p
void E0038LoopCallback(void *taskData) {
    E0038LoopParameter *const child = static_cast<E0038LoopParameter *>(taskData);
    Vector3 offset{0.22f, 0.066f, -0.004f};
    const Vector3 volume{0.0f, 0.0f, 0.0f};
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 0 && child->parent->selection == 3)
        InitializeE0038Smoke(child);
    if (child->parent->selection == 3) {
        Vector3 position;
        Vector3i rotation;
        const shen_name character = frame >= 311 ? AREA_0038_A059 : AREA_0038_A127;
        if (frame >= 311)
            offset = {0.0f, 0.1f, 0.0f};
        EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(character, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
        SetParticleEffectPosition(0, child->smoke, &offset, &volume);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) != 0)
        return;
    if (child->parent->selection == 3) {
        SuspendParticleEffect(0, child->smoke);
        FreeParticleEffect(0, child->smoke);
    }
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0038Characters();
    static_cast<E0038LoopParameter *>(taskData)->parent->state = 0;
    HLib::DestroyCurrentTask();
}

} // namespace

const ScriptAreaDefinition area0038ScriptDefinition{.areaId = AREA_0038_AREA, .field04 = 0, .init = E0038Init, .main = E0038Main};

} // namespace shenmue::s2::areas
