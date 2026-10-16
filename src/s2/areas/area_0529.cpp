#include "s2/areas/area_0529.hpp"
#include "s2/author.hpp"
#include "s2/sound_program.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/particle_effects.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t attached0529Fire, stationary0529Fire;
constexpr std::uint16_t palette0529[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace
// @verified - p
void Setup0529FireEffects() {
    const Vector3 zero{};
    attached0529Fire = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 4));
    SetFirePalette(attached0529Fire, palette0529);
    SetFireFalloff(attached0529Fire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(attached0529Fire, -0.23529411852359772f);
    SetParticleEffectSize(4, attached0529Fire, 0.3f, 0.6f);
    AttachFireEffect(attached0529Fire, AREA_0529_a287, 152, 0.05f);
    SetFirePosition(attached0529Fire, 1, &zero);
    const Vector3 position{66.2300033569336f, 101.5f, 139.13999938964844f};
    stationary0529Fire = static_cast<std::uint32_t>(CreateFireEffect(position, 0.2f, 0.5f, 0, 0));
    SetFirePalette(stationary0529Fire, palette0529);
    SetFireFalloff(stationary0529Fire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(stationary0529Fire, -0.23529411852359772f);
    SetParticleEffectSize(4, stationary0529Fire, 0.3f, 0.5f);
    SetParticleEffectPosition(4, stationary0529Fire, &position, &position);
    ResumeParticleEffect(4, stationary0529Fire);
}

// @verified - p
void Initialize0529() {
    attached0529Fire = stationary0529Fire = 0;
}

// @verified - p
void Start0529(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0529, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0529_MOTION));
    parent->selection = 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0529_CONVERSATION);
    EV_SetWeatherInstruction(12, 0);
    EV_SetWeatherInstruction(0x780, 0);
    Setup0529FireEffects();
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0529_CHARACTERS, parent->self);
}

// @verified - p
void Update0529(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0529Author(parent);
    } else if (parent->state == 3) {
        Cleanup0529(parent);
    }
}

// @verified - p
void Cleanup0529(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0529_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    SuspendParticleEffect(4, attached0529Fire);
    SuspendParticleEffect(4, stationary0529Fire);
    FreeParticleEffect(4, attached0529Fire);
    FreeParticleEffect(4, stationary0529Fire);
    EV_stepSetFlags(0, 0x1cd);
    EV_FreeAuthorPak(0, AREA_0529_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0529Author(AuthorViewerTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0529AuthorTask, 4, 0x0b, sizeof(AuthorViewerFrameTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerFrameTaskParameter *const child = static_cast<AuthorViewerFrameTaskParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_LoadAudio("0529A000", 0);
    EV_spmBeginSampleExecute();
    Prepare0529Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0529Author(AuthorViewerFrameTaskParameter *child) {
    child->currentFrame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 540)
        ResumeParticleEffect(4, attached0529Fire);
    else if (child->currentFrame == 830)
        AttachFireInstance(attached0529Fire, 1, AREA_0529_a288, 152, 0x3d4ccccd);
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0529Characters();
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0529AuthorTask(void *taskData) {
    Update0529Author(static_cast<AuthorViewerFrameTaskParameter *>(taskData));
}

// @verified - p
void Prepare0529Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0529_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0529_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0529_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0529_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0529_a287, false);
    EV_SetCharacterAlphaEnabled(AREA_0529_a288, false);
    EV_SetCharacterAlphaEnabled(AREA_0529_a636, false);
}

// @verified - p
void Restore0529Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0529_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0529_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0529_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0529_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0529_a287, true);
    EV_SetCharacterAlphaEnabled(AREA_0529_a288, true);
    EV_SetCharacterAlphaEnabled(AREA_0529_a636, true);
}

const ScriptAreaDefinition area0529ScriptDefinition{.areaId = AREA_0529, .field04 = 0, .init = Initialize0529, .main = Start0529};

} // namespace shenmue::s2::areas
