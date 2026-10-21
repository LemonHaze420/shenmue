#include "s2/areas/area_0518.hpp"
#include "s2/input.hpp"
#include "s2/author.hpp"
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
#include "s2/model_component.hpp"
#include "s2/date_time.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t stationaryFire0518{}, carriedFire0518{};
float sceneIntensity0518 = 0.3f;
constexpr std::uint16_t firePalette0518[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace

// @verified - p
void Initialize0518() {
    sceneIntensity0518 = 0.3f;
    stationaryFire0518 = 0;
    carriedFire0518 = 0;
}

// @verified - p
void Start0518(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0518, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0518_MOTION));
    parent->state = 0;
    parent->selection = 0;
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 22;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    const Vector3 position{154.78f, 100.14f, 392.16f};
    stationaryFire0518 = static_cast<std::uint32_t>(CreateFireEffect(position, 0.5f, 0.5f, 0, 0));
    SetFirePalette(stationaryFire0518, firePalette0518);
    SetFireFalloff(stationaryFire0518, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(stationaryFire0518, -0.2980392277240753f);
    SetParticleEffectSize(4, stationaryFire0518, 0.5098039507865906f, 1.0196079015731812f);
    SetParticleEffectPosition(4, stationaryFire0518, &position, &position);
    carriedFire0518 = static_cast<std::uint32_t>(CreateFireEffect(position, 0.5f, 0.5f, 0, 0));
    SetFirePalette(carriedFire0518, firePalette0518);
    SetFireFalloffScalar(carriedFire0518, -0.05000000074505806f);
    SetFireFalloff(carriedFire0518, 3.140000104904175f, 4.940000057220459f, 0.7799999713897705f);
    SetParticleEffectSize(4, carriedFire0518, 0.02500000037252903f, 0.05000000074505806f);
    EV_SetWeatherInstruction(12, 0);
    EV_lightLoad(AREA_0518_LIGHT);
    for (const std::int16_t light : {0, 6, 7, 8, 3, 5})
        AssignLightProgram(static_cast<std::uint32_t>(light));
    EV_FreeAuthorPak(1, AREA_0518_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0518_CHARACTERS, parent->self);
}

// @verified - p
void Update0518(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0518Author(parent);
    } else if (parent->state == 3) {
        Cleanup0518(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 0;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 0)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 0) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0518_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0518(AuthorViewerPairTaskParameter *parent) {
    SuspendParticleEffect(4, stationaryFire0518);
    SuspendParticleEffect(4, carriedFire0518);
    FreeParticleEffect(4, stationaryFire0518);
    FreeParticleEffect(4, carriedFire0518);
    for (const shen_name *character = AREA_0518_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0518_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_SetScenarioTaskFlags(9);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0518Author(AuthorViewerPairTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0518AuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0518Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0518Author(AuthorViewer517ELoopTaskParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 1003 || frame == 2679) {
        if (frame == 2679)
            EV_SetIndexForLightEntry(32, 1);
        ResumeParticleEffect(4, carriedFire0518);
    } else if (frame == 1150) {
        ResumeParticleEffect(4, stationaryFire0518);
        SuspendParticleEffect(4, carriedFire0518);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    } else if (frame == 1240) {
        SetFireFalloffScalar(stationaryFire0518, -0.2705882489681244f);
        SetParticleEffectSize(4, stationaryFire0518, 0.27450981736183167f, 0.5098039507865906f);
    }
    if (static_cast<std::uint32_t>(frame - 1240) <= 259U) {
        sceneIntensity0518 += 0.001538461539f;
        SetLightSceneIntensity(27, sceneIntensity0518);
    } else if (frame == 1500) {
        sceneIntensity0518 = 0.6f;
        SetLightSceneIntensity(27, 0.6f);
    } else if (frame == 0)
        SetLightSceneIntensity(27, sceneIntensity0518);
    Vector3 position{};
    Vector3i rotation{};
    Vector3 offset{0.0f, 0.03f, 0.0f};
    EV_GetCharaPosition(AREA_0518_a435, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0518_a435, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetParticleEffectPosition(4, carriedFire0518, &offset, &offset);
    if (static_cast<std::uint32_t>(frame - 1003) <= 66U || frame >= 2679 || frame == 1070) {
        if (frame == 1070)
            position = Vector3{154.5f, 101.69f, 392.75f};
        SetLightRuntimePosition(32, position);
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
        Restore0518Characters();
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0518AuthorTask(void *taskData) {
    Update0518Author(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0518Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0518_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0518_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0518_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0518_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0518_a435, false);
    EV_SetCharacterAlphaEnabled(AREA_0518_a016, false);
    EV_SetCharacterAlphaEnabled(AREA_0518_a466, false);
    EV_SetCharacterAlphaEnabled(AREA_0518_a468, false);
    EV_SetCharacterAlphaEnabled(AREA_0518_a470, false);
    clmdUtil(FindCharacterByScenarioName(AREA_0518_SIN_), 11);
}

// @verified - p
void Restore0518Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0518_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0518_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0518_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0518_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0518_a435, true);
    EV_SetCharacterAlphaEnabled(AREA_0518_a016, true);
    EV_SetCharacterAlphaEnabled(AREA_0518_a466, true);
    EV_SetCharacterAlphaEnabled(AREA_0518_a468, true);
    EV_SetCharacterAlphaEnabled(AREA_0518_a470, true);
    clmdUtil(FindCharacterByScenarioName(AREA_0518_SIN_), 0);
}

const ScriptAreaDefinition area0518ScriptDefinition{.areaId = AREA_0518, .field04 = 0, .init = Initialize0518, .main = Start0518};

} // namespace shenmue::s2::areas
