#include "s2/areas/area_0510.hpp"
#include "s2/input.hpp"
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
#include "s2/model_component.hpp"
#include "s2/date_time.hpp"
#include "s2/weather.hpp"
#include "s2/scene_data.hpp"
#include "s2/pak.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::uint32_t fallingLeaves0510 = invalidEffect;
std::uint32_t breath0510[2]{};
} // namespace

// @verified - p
void Initialize0510() {
    fallingLeaves0510 = invalidEffect;
    breath0510[0] = 0;
    breath0510[1] = 0;
}

// @verified - p
void Start0510(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0510, 4, 0x0b, sizeof(AuthorViewerPairTaskParameter), AREA_0510_MAIN_TASK);
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0510_MOTION));
    parent->state = 0;
    parent->selection = 0;

    Queue_EVT_UTL_LoadNPCChara_3(AREA_0510_CHARACTERS, parent->self);
    Setup0510Environment();
}

// @verified - p
void Update0510(void *taskData) {
    AuthorViewerPairTaskParameter *const parent = static_cast<AuthorViewerPairTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0510Author(parent);
    } else if (parent->state == 3) {
        Cleanup0510(parent);
    } else if (parent->state == 0) {
        switch (EVT_GetControllerInput(0, 1)) {
        case 1024:
            parent->state = 1;
            break;
        case 64:
            --parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) < 0)
                parent->selection = 7;
            break;
        case 128:
            ++parent->selection;
            if (std::bit_cast<std::int32_t>(parent->selection) > 7)
                parent->selection = 0;
            break;
        default:
            break;
        }
        if (parent->selection <= 7) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0510_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", std::bit_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0510(AuthorViewerPairTaskParameter *parent) {
    for (const shen_name *character = AREA_0510_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, "0509");
    EV_FreeAuthorPak(0, "0510");
    EV_FreeAuthorPak(0, "0511");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0510Author(AuthorViewerPairTaskParameter *parent) {
    Select0510Package(std::bit_cast<std::int32_t>(parent->selection));
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0510AuthorTask, 4, 0x0b, sizeof(AuthorViewer517ELoopTaskParameter), AREA_0510_LOOP_TASK);
    AuthorViewer517ELoopTaskParameter *const child = static_cast<AuthorViewer517ELoopTaskParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->currentFrame = 0;
    if (parent->selection != 7)
        Create0510FallingLeaves(parent->selection == 1);
    switch (parent->selection) {
    case 0:
        EV_LoadCutsceneFromCurrentScene("0510A000");
        EV_LoadAudio("0510A000", 0);
        break;
    case 1:
        EV_LoadCutsceneFromCurrentScene("0510A000");
        EV_LoadAudio("0511A000", 1);
        break;
    case 2:
        EV_LoadCutsceneFromCurrentScene("0510A000");
        EV_LoadAudio("0510A000", 1);
        break;
    case 3:
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 0);
        break;
    case 4:
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 2);
        break;
    case 5:
        clmdUtil(FindCharacterByScenarioName(AREA_0510_SIN_), 14);
        [[fallthrough]];
    case 6:
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 3);
        break;
    case 7:
        EV_LoadCutsceneFromCurrentScene("0509A004");
        break;
    default:
        EV_LoadCutsceneFromCurrentScene("0511A000");
        EV_LoadAudio("0511A000", 3);
        break;
    }
    switch (parent->selection) {
    case 0:
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0510_LIGHT_FIRST);
        break;
    case 2:
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0510_LIGHT_SECOND);
        PushGlobalLightingProgram();
        break;
    case 3:
    case 5:
    case 6:
        EV_SetModelDataDefaultVertexShader();
        EV_SetWeatherInstruction(12, 0);
        EV_AssignLightParams(1, 0);
        EV_AssignLightParams(1, 1);
        EV_AssignLightParams(1, 5);
        EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
        SetupLightConfig();
        AssignLightProgram(3);
        break;
    case 7:
        Create0510BreathEffects();
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0510_LIGHT_SEVENTH);
        PushGlobalLightingProgram();
        break;
    default:
        EV_SetModelDataDefaultVertexShader();
        SetWeatherUpdateEnabledTo(1);
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        break;
    }
    Prepare0510Characters(parent->selection);
    const std::int64_t result = EV_authSceneCtrl(static_cast<AuthSceneCtrl>(std::bit_cast<std::int32_t>(parent->selection)));
    parent->state = 2;
    return result;
}

// @verified - p
std::int64_t Update0510Author(AuthorViewer517ELoopTaskParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->parent->selection == 7) {
        const Vector3 spawn{0.0f, 0.003f, 0.0f};
        Vector3 position{};
        EV_GetCharaPosition(AREA_0510_a904, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        SetParticleEffectPosition(0, breath0510[0], &position, &spawn);
        EV_GetCharaPosition(AREA_0510_a906, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        SetParticleEffectPosition(0, breath0510[1], &position, &spawn);
        if (frame == 0 || frame == 681 || frame == 1111) {
            ResumeParticleEffect(0, breath0510[0]);
            ResumeParticleEffect(0, breath0510[1]);
        } else if (frame == 311 || frame == 801 || frame == 1211) {
            SuspendParticleEffect(0, breath0510[0]);
            SuspendParticleEffect(0, breath0510[1]);
        }
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        if (fallingLeaves0510 != invalidEffect) {
            SuspendParticleEffect(3, fallingLeaves0510);
            FreeParticleEffect(3, fallingLeaves0510);
            fallingLeaves0510 = invalidEffect;
        }
        for (std::uint32_t &effect : breath0510) {
            if (effect != invalidEffect) {
                FreeParticleEffect(0, effect);
                effect = invalidEffect;
            }
        }
        Restore0510Characters(child->parent->selection);
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        child->parent->state = 0;
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0510AuthorTask(void *taskData) {
    Update0510Author(static_cast<AuthorViewer517ELoopTaskParameter *>(taskData));
}

void Setup0510Environment() {
    SetPlayerControlEnabled(0);
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 17;
    time.minute = 12;
    SetCurrentDateTimeTo(time);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_FreeAuthorPak(1, "0509");
    EV_FreeAuthorPak(1, "0510");
    EV_FreeAuthorPak(1, "0511");
    breath0510[0] = invalidEffect;
    breath0510[1] = invalidEffect;
}

void Select0510Package(std::int32_t selection) {
    for (const char *package : {"KSH1", "KSH3", "KSH5"}) {
        if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0) != 0)
            EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
    }
    const char *const directory = Filepath_Gen(FilepathGenMode::SceneDirectory);
    const char *const package = selection == 1 ? "KSH3" : selection == 7 ? "KSH5" : "KSH1";
    EV_ControlPackageResources(
        PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(directory), reinterpret_cast<std::uintptr_t>(package));
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
}

void Create0510FallingLeaves(bool alternate) {
    const Vector3 position = alternate ? Vector3{56.1f, 104.4f, 51.9f} : Vector3{164.66f, 110.4f, 192.4f};
    const Vector3 spawn{2.5f, 0.02f, 2.5f};
    const Vector3 velocity{0.01f, 0.015f, 0.01f};
    const Vector3 variance{0.01f, 0.005f, 0.01f};
    fallingLeaves0510 = static_cast<std::uint32_t>(
        CreateFallingParticleEffect(position, spawn, 300, alternate ? 0.2f : 0.4f, shen_name{}, "MISC", "SHENLEAF.PVR", alternate ? 99.3f : 96.6f));
    SetParticleEffectSize(3, fallingLeaves0510, 0.05f, 0.05f);
    SetParticleEffectVelocity(3, fallingLeaves0510, velocity, variance);
    ResumeParticleEffect(3, fallingLeaves0510);
}

void Create0510BreathEffects() {
    const Vector3 position{};
    const Vector3 spawn{0.0f, 0.003f, 0.0f};
    const Vector3 velocity{0.0f, 0.003f, 0.0f};
    const Vector3 variance{0.0f, 0.003f, 0.0f};
    for (std::uint32_t &effect : breath0510) {
        effect = static_cast<std::uint32_t>(CreateSmokeEffect(position, spawn, 40, 0.08f, shen_name{}, 0));
        SetSmokeEffectFadeFactor(effect, 10);
        SetSmokeEffectFalloff(effect, 144);
        SetParticleEffectColour(0, effect, 0, 255, 255, 255);
        SetParticleEffectInitialScale(0, effect, 0.5f);
        SetParticleEffectSize(0, effect, 0.3f, 0.3f);
        SetParticleEffectBirthRate(0, effect, 0.003f);
        SetParticleEffectVelocity(0, effect, velocity, variance);
        ResumeParticleEffect(0, effect);
    }
}

void Prepare0510Character(shen_name name) {
    EV_EnsureCharacterDraw(name);
    EV_EnableCharacterDisplay(name);
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
}

void Restore0510Character(shen_name name) {
    const Vector3 zero{};
    EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_ToggleCharacterFlag(name);
    EV_DisableCharacterDisplay(name);
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
    EV_SetCharacterAlphaEnabled(name, true);
}

void Restore0510Prop(shen_name name) {
    const Vector3 zero{};
    EV_AT_SetPos(name, EventTransformFlags::World | EventTransformFlags::AllAxes, zero);
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_ToggleCharacterFlag(name);
    EV_DisableCharacterDisplay(name);
    EV_SetCharacterAlphaEnabled(name, true);
}

void Prepare0510Characters(std::uint32_t selection) {
    switch (selection) {
    case 0:
        Prepare0510Character(AREA_0510_RYO_);
        Prepare0510Character(AREA_0510_SIN_);
        [[fallthrough]];
    case 2:
        Prepare0510Character(AREA_0510_RYO_);
        Prepare0510Character(AREA_0510_SIN_);
        for (const shen_name name : {AREA_0510_a246, AREA_0510_a349}) {
            EV_EnsureCharacterDraw(name);
            EV_EnableCharacterDisplay(name);
            EV_SetCharacterAlphaEnabled(name, false);
        }
        EV_DisableCharacterDisplay(AREA_0510_a246);
        break;
    case 1:
        Prepare0510Character(AREA_0510_SNP_);
        Prepare0510Character(AREA_0510_SNM_);
        break;
    case 3:
    case 5:
    case 6:
        Prepare0510Character(AREA_0510_RYO_);
        Prepare0510Character(AREA_0510_SIN_);
        for (const shen_name name : {AREA_0510_ab09, AREA_0510_a726, AREA_0510_a728}) {
            EV_EnsureCharacterDraw(name);
            EV_EnableCharacterDisplay(name);
            EV_SetCharacterAlphaEnabled(name, false);
        }
        break;
    case 4:
        Prepare0510Character(AREA_0510_SN5_);
        EV_EnsureCharacterDraw(AREA_0510_a345);
        EV_EnableCharacterDisplay(AREA_0510_a345);
        EV_SetCharacterAlphaEnabled(AREA_0510_a345, false);
        break;
    case 7:
        Prepare0510Character(AREA_0510_IWA_);
        Prepare0510Character(AREA_0510_LRY_);
        for (const shen_name name : {AREA_0510_a904, AREA_0510_ab21, AREA_0510_a932, AREA_0510_a906, AREA_0510_a908, AREA_0510_a918, AREA_0510_a920,
                 AREA_0510_a912, AREA_0510_a914, AREA_0510_a916, AREA_0510_a902, AREA_0510_a900, AREA_0510_a910, AREA_0510_a938, AREA_0510_a940}) {
            EV_EnsureCharacterDraw(name);
            EV_EnableCharacterDisplay(name);
            EV_SetCharacterAlphaEnabled(name, false);
        }
        break;
    default:
        break;
    }
}

void Restore0510Characters(std::uint32_t selection) {
    switch (selection) {
    case 0:
        Restore0510Character(AREA_0510_RYO_);
        Restore0510Character(AREA_0510_SIN_);
        [[fallthrough]];
    case 2:
        Restore0510Character(AREA_0510_RYO_);
        Restore0510Character(AREA_0510_SIN_);
        Restore0510Prop(AREA_0510_a246);
        Restore0510Prop(AREA_0510_a349);
        EV_DisableCharacterDisplay(AREA_0510_a246);
        break;
    case 1:
        Restore0510Character(AREA_0510_SNP_);
        Restore0510Character(AREA_0510_SNM_);
        break;
    case 3:
    case 5:
    case 6:
        Restore0510Character(AREA_0510_RYO_);
        Restore0510Character(AREA_0510_SIN_);
        for (const shen_name name : {AREA_0510_ab09, AREA_0510_a726, AREA_0510_a728})
            Restore0510Prop(name);
        break;
    case 4:
        Restore0510Character(AREA_0510_SN5_);
        Restore0510Prop(AREA_0510_a345);
        break;
    case 7:
        Restore0510Character(AREA_0510_IWA_);
        Restore0510Character(AREA_0510_LRY_);
        for (const shen_name name : {AREA_0510_a904, AREA_0510_ab21, AREA_0510_a932, AREA_0510_a906, AREA_0510_a908, AREA_0510_a918, AREA_0510_a920,
                 AREA_0510_a912, AREA_0510_a914, AREA_0510_a916, AREA_0510_a902, AREA_0510_a900, AREA_0510_a910, AREA_0510_a938, AREA_0510_a940})
            Restore0510Prop(name);
        break;
    default:
        break;
    }
}

const ScriptAreaDefinition area0510ScriptDefinition{.areaId = AREA_0510, .field04 = 0, .init = Initialize0510, .main = Start0510};

} // namespace shenmue::s2::areas
