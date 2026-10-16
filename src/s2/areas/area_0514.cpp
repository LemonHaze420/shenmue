#include "s2/areas/area_0514.hpp"
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
#include "common/math.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t attached0514Fire, stationary0514Fire = 0xffffffffU, fire0514Angle;
std::int32_t fire0514Countdown, fire0514Step;
float fire0514Width, fire0514Height, fire0514Intensity;
constexpr std::uint16_t palette0514[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
} // namespace

// @verified - p
void Initialize0514() {
    stationary0514Fire = 0xffffffffU;
    attached0514Fire = 0;
    fire0514Countdown = fire0514Step = 0;
    fire0514Angle = 0;
}

// @verified - p
void Start0514(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0514, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0514_MOTION));
    parent->state = 0;
    parent->selection = 0;
    Setup0514Scene();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0514_CHARACTERS, parent->self);
}

// @verified - p
void Update0514(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0514Author(parent);
    } else if (parent->state == 3) {
        Cleanup0514(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 2;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 2)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 2) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0514_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0514(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0514_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    SuspendParticleEffect(4, attached0514Fire);
    FreeParticleEffect(4, attached0514Fire);
    if (stationary0514Fire != 0xffffffffU) {
        SuspendParticleEffect(4, stationary0514Fire);
        FreeParticleEffect(4, stationary0514Fire);
        stationary0514Fire = 0xffffffffU;
    }
    EV_FreeAuthorPak(0, AREA_0514_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0514Author(AuthorViewerPairTaskParameter *parent) {
    constexpr HLib::TaskCallback callbacks[]{Update0514AuthorTask, Update0514SecondAuthorTask, Update0514SecondAuthorTask};
    HLib::Task *const task =
        HLib::EnqueueTaskWithParameter(callbacks[parent->selection], 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->currentFrame = 0;
    child->parent = parent;
    EV_SetWeatherInstruction(12, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetModelDataDefaultVertexShader();
    if (parent->selection <= 2)
        EV_lightLoad(AREA_0514_LIGHTS[parent->selection]);
    EV_LoadAudio("0514A000", std::bit_cast<std::int32_t>(parent->selection));
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0514Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0514Author(AuthorViewer517ELoopTaskParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    Update0514FireLighting();
    if (frame == 1) {
        fire0514Height = fire0514Width = fire0514Intensity = 0.0f;
        SetLightSceneIntensity(3, 0.0f);
        SetParticleEffectSize(4, stationary0514Fire, fire0514Width, fire0514Height);
        goto AuthorStatus;
    }
    if (frame >= 100) {
        if (frame < 110) {
            fire0514Width += 0.03999999910593033f;
            SetParticleEffectSize(4, stationary0514Fire, fire0514Width, fire0514Height);
        } else if (frame < 130) {
            fire0514Intensity += 0.02500000037252903f;
            SetLightSceneIntensity(3, fire0514Intensity);
        }
    }
    if (static_cast<std::uint32_t>(frame - 100) <= 0x13U) {
        fire0514Height += 0.03500000014901161f;
        SetParticleEffectSize(4, stationary0514Fire, fire0514Width, fire0514Height);
    }
AuthorStatus:
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0514Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0514AuthorTask(void *taskData) {
    Update0514Author(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Prepare0514Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0514_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0514_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0514_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0514_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0514_aa85, false);
    EV_SetCharacterAlphaEnabled(AREA_0514_a287, false);
    EV_SetCharacterAlphaEnabled(AREA_0514_a288, false);
}

// @verified - p
void Restore0514Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0514_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0514_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0514_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0514_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0514_aa85, true);
    EV_SetCharacterAlphaEnabled(AREA_0514_a287, true);
    EV_SetCharacterAlphaEnabled(AREA_0514_a288, true);
}

// @verified - p
std::int64_t Update0514SecondAuthor(AuthorViewer517ELoopTaskParameter *child) {
    Update0514FireLighting();
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        Restore0514Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}
void Update0514SecondAuthorTask(void *taskData) {
    Update0514SecondAuthor(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

// @verified - p
void Setup0514Scene() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0514_CONVERSATION);
    Create0514FireEffects(0);
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 0);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 0);
    EV_SetObjectFlagByIndex(5, 0);
    EV_SetObjectFlagByIndex(6, 0);
    EV_SetObjectFlagByIndex(7, 0);
    EV_SetObjectFlagByIndex(14, 0);
}

// @verified - p
void Create0514FireEffects(std::int32_t selection) {
    const Vector3 zero{};
    attached0514Fire = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 4));
    SetFirePalette(attached0514Fire, palette0514);
    SetFireFalloff(attached0514Fire, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(attached0514Fire, -0.2980392277240753f);
    SetParticleEffectSize(4, attached0514Fire, 0.2f, 0.5f);
    ResumeParticleEffect(4, attached0514Fire);
    AttachFireEffect(attached0514Fire, AREA_0514_a287, 152, 0.1f);
    SetFirePosition(attached0514Fire, 1, &zero);
    AttachFireInstance(attached0514Fire, 1, AREA_0514_a288, 152, 0x3dcccccd);
    if (selection == 0) {
        const Vector3 position{53.97999954223633f, 96.19999694824219f, 117.30000305175781f};
        stationary0514Fire = static_cast<std::uint32_t>(CreateFireEffect(position, 0.2f, 0.5f, 0, 0));
        SetFirePalette(stationary0514Fire, palette0514);
        SetFireFalloff(stationary0514Fire, 3.140000104904175f, 4.940000057220459f, 0.7799999713897705f);
        SetFireFalloffScalar(stationary0514Fire, -0.15000000596046448f);
        SetParticleEffectSize(4, stationary0514Fire, 0.0f, 0.0f);
        SetParticleEffectPosition(4, stationary0514Fire, &position, &position);
        ResumeParticleEffect(4, stationary0514Fire);
    }
}

// @verified - p
void Update0514FireLighting() {
    const float intensity = sin_fixed16(static_cast<std::int16_t>(fire0514Angle)) * 0.05f + 0.699999988079071f;
    fire0514Countdown = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(fire0514Countdown) - 1U);
    if (fire0514Countdown <= 0) {
        fire0514Countdown = shenGetRandomInt32(60) + 30;
        fire0514Step = shenGetRandomInt32(2048) + 2048;
    }
    fire0514Angle += static_cast<std::uint32_t>(fire0514Step);
    Vector3 offset{-0.01f, 0.35f, -0.013f}, position;
    Vector3i rotation;
    EV_GetCharaPosition(AREA_0514_a287, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0514_a287, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetLightRuntimePosition(1, offset);
    PrioritizeLight(1);
    SetLightSceneIntensity(1, intensity);
    offset = {-0.01f, 0.35f, -0.013f};
    EV_GetCharaPosition(AREA_0514_a288, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0514_a288, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetLightRuntimePosition(2, offset);
    PrioritizeLight(2);
    SetLightSceneIntensity(2, intensity);
}

const ScriptAreaDefinition area0514ScriptDefinition{.areaId = AREA_0514, .field04 = 0, .init = Initialize0514, .main = Start0514};

} // namespace shenmue::s2::areas
