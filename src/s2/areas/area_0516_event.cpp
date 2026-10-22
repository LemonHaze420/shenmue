#include "s2/areas/area_0516_event.hpp"
#include "s2/input.hpp"
#include "s2/areas/area_0516.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
float fogBlue0516 = 0.25f;
std::uint32_t carriedFire0516{}, trackedFire0516{}, spreadingFire0516{}, growingFires0516[3]{};
std::int32_t flickerAngle0516{}, flickerCountdown0516{}, flickerStep0516{};
constexpr std::uint16_t firePalette0516[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
constexpr Vector3 firePositions0516[]{{136.17f, 106.64f, -10.8f}, {139.3f, 109.75f, -11.3f}, {134.07f, 107.19f, -13.497f}, {132.267f, 107.99f, -15.31f},
    {129.535f, 108.8f, -17.116f}, {125.854f, 108.26f, -17.91f}, {121.573f, 107.77f, -18.812f}, {117.73f, 110.28f, -19.855f}};
constexpr shen_name firstProps0516[]{AREA_0516_EVENT_a277, AREA_0516_EVENT_a704, AREA_0516_EVENT_aa33, AREA_0516_EVENT_aa75};
constexpr shen_name firstRestoreProps0516[]{AREA_0516_EVENT_a277, AREA_0516_EVENT_a704, AREA_0516_EVENT_aa33, AREA_0516_EVENT_aa41};
constexpr shen_name secondProps0516[]{AREA_0516_EVENT_ab38, AREA_0516_EVENT_a067, AREA_0516_EVENT_a292};
constexpr shen_name thirdProps0516[]{AREA_0516_EVENT_aa39, AREA_0516_EVENT_aa75, AREA_0516_EVENT_a067, AREA_0516_EVENT_a956, AREA_0516_EVENT_a922,
    AREA_0516_EVENT_a924, AREA_0516_EVENT_aa36, AREA_0516_EVENT_aa40, AREA_0516_EVENT_aa41, AREA_0516_EVENT_aa20};
constexpr shen_name fourthProps0516[]{
    AREA_0516_EVENT_aa39, AREA_0516_EVENT_a067, AREA_0516_EVENT_a956, AREA_0516_EVENT_a922, AREA_0516_EVENT_a924, AREA_0516_EVENT_aa36, AREA_0516_EVENT_aa20};
} // namespace

void Initialize0516() {
    fogBlue0516 = 0.25f;
    carriedFire0516 = trackedFire0516 = spreadingFire0516 = 0;
    for (std::uint32_t &effect : growingFires0516)
        effect = 0;
}
void Start0516(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0516, 4, 0x0b, sizeof(Area0516Parameter), AREA_0516_EVENT_TASK);
    Area0516Parameter *const parent = static_cast<Area0516Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0516A.MOT"));
    parent->selection = g_entryPoint <= 3 ? static_cast<std::int32_t>(g_entryPoint) : 0;
    parent->state = 1;
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_FreeAuthorPak(1, "0516");
    Setup0516Effects(parent->selection);
    for (std::int32_t object = 0; object != 4; ++object)
        EV_SetObjectFlagByIndex(object, 0);
    HLib::EnqueueTaskWithoutParameter(Update0516Waves, 3, 2, static_cast<std::int32_t>(AREA_0516_EVENT_TASK.name));
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0516_EVENT_CHARACTERS, parent->self);
}
void Update0516(void *parameter) {
    Area0516Parameter *const parent = static_cast<Area0516Parameter *>(parameter);
    if (parent->state == 1)
        Queue0516Author(parent);
    else if (parent->state == 3)
        Cleanup0516(parent);
    else if (parent->state == 0 && EVT_GetControllerInput(0, INPUT_PRESSED) == 1024)
        parent->state = 1;
}
void Queue0516Author(Area0516Parameter *parent) {
    const HLib::TaskCallback callbacks[]{Update0516FirstAuthor, Update0516SecondAuthor, Update0516ThirdAuthor, Update0516FourthAuthor};
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callbacks[parent->selection], 4, 0x0b, sizeof(Area0516AuthorParameter), AREA_0516_EVENT_TASK);
    static_cast<Area0516AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_SetModelDataDefaultVertexShader();
    EV_LoadCutsceneFromCurrentScene(parent->selection == 0 ? "0516A000" : parent->selection == 1 ? "0516A010" : "0516A001");
    EV_lightLoad(AREA_0516_EVENT_LIGHTS[parent->selection]);
    if (parent->selection != 3)
        PushGlobalLightingProgram();
    if (parent->selection == 1)
        SetDefaultVSByIndex(6);
    EV_SetIndexForLightEntry(13, 0);
    EV_SetIndexForLightEntry(14, 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    Prepare0516Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
void Cleanup0516(Area0516Parameter *parent) {
    for (const shen_name *name = AREA_0516_EVENT_CHARACTERS; name->name != 0xffffffffU; ++name) {
        if (FindCharacterByScenarioName(*name)) {
            EV_npcLockUnlock(*name);
            EV_npcFree(*name);
        }
    }
    EV_FreeAuthorPak(0, "0516");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    SuspendParticleEffect(4, carriedFire0516);
    FreeParticleEffect(4, carriedFire0516);
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (parent->selection != 0) {
        for (std::uint32_t effect : growingFires0516) {
            SuspendParticleEffect(4, effect);
            FreeParticleEffect(4, effect);
        }
        SuspendParticleEffect(4, spreadingFire0516);
        FreeParticleEffect(4, spreadingFire0516);
        SuspendParticleEffect(4, trackedFire0516);
        FreeParticleEffect(4, trackedFire0516);
    }
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}
void Setup0516Effects(std::int32_t selection) {
    const Vector3 zero{};
    if (selection == 1) {
        trackedFire0516 = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.5f, 0.5f, 0, 1));
        SetFirePalette(trackedFire0516, firePalette0516);
        SetFireFalloffScalar(trackedFire0516, -0.21568627655506134f);
        SetFireFalloff(trackedFire0516, 3.1764705181121826f, 6.313725471496582f, 0.8627451062202454f);
        SetParticleEffectSize(4, trackedFire0516, 0.11764705926179886f, 0.4313725531101227f);
        ResumeParticleEffect(4, trackedFire0516);
        for (std::uint32_t &effect : growingFires0516) {
            effect = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 0));
            SetFirePalette(effect, firePalette0516);
            SetParticleEffectPosition(4, effect, &zero, &zero);
            SetFireFalloff(effect, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
            SetFireFalloffScalar(effect, -0.2980392277240753f);
            SetParticleEffectSize(4, effect, 0.0f, 0.0f);
        }
        spreadingFire0516 = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 4));
        SetFirePalette(spreadingFire0516, firePalette0516);
        SetParticleEffectPosition(4, spreadingFire0516, &zero, &zero);
        SetFireFalloff(spreadingFire0516, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
        SetFireFalloffScalar(spreadingFire0516, -0.2980392277240753f);
        SetParticleEffectSize(4, spreadingFire0516, 0.9803921580314636f, 1.529411792755127f);
    }
    carriedFire0516 = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.2f, 0.5f, 0, 4));
    SetFirePalette(carriedFire0516, firePalette0516);
    SetFireFalloff(carriedFire0516, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(carriedFire0516, -0.23529411852359772f);
    SetParticleEffectSize(4, carriedFire0516, 0.3f, 0.6f);
    ResumeParticleEffect(4, carriedFire0516);
    AttachFireEffect(carriedFire0516, AREA_0516_EVENT_a287, 152, 0.05f);
    SetFirePosition(carriedFire0516, 9, &zero);
    AttachFireInstance(carriedFire0516, 9, AREA_0516_EVENT_a288, 152, std::bit_cast<std::int32_t>(0.1f));
}
void Queue0516FireStart(std::uint32_t effect, std::int32_t position) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0516FireStart, 4, 0x0b, sizeof(Area0516FireParameter), AREA_0516_EVENT_TASK);
    Area0516FireParameter *const parameter = static_cast<Area0516FireParameter *>(HLib::GetTaskParameter(task));
    parameter->frame = 0;
    parameter->height = 0.0f;
    parameter->radiusX = 0.0f;
    parameter->gain = 0.0f;
    parameter->effect = effect;
    SetFireFalloff(effect, 3.1764705181121826f, 6.784313678741455f, 0.7058823704719543f);
    SetFireFalloffScalar(effect, -0.2980392277240753f);
    SetParticleEffectSize(4, effect, 0.9803921580314636f, 1.529411792755127f);
    SetParticleEffectSize(4, effect, 0.0f, 0.0f);
    SetParticleEffectPosition(4, effect, &firePositions0516[position], &firePositions0516[position]);
}
void Update0516FireStart(void *data) {
    Area0516FireParameter *const parameter = static_cast<Area0516FireParameter *>(data);
    if (parameter->frame == 0) {
        parameter->width = parameter->height = 0.0f;
        parameter->radiusX = 2.3137255f;
        parameter->radiusY = 5.5294118f;
        parameter->gain = std::bit_cast<float>(1060418741U);
        parameter->scalar = -0.19607843f;
        SetFireFalloff(parameter->effect, parameter->radiusX, parameter->radiusY, parameter->gain);
        SetParticleEffectSize(4, parameter->effect, parameter->width, parameter->height);
        SetFireFalloffScalar(parameter->effect, parameter->scalar);
        ResumeParticleEffect(4, parameter->effect);
    } else if (parameter->frame == 15 || parameter->frame == 28) {
        if (parameter->frame == 15) {
            parameter->width = 2.0784314f;
            parameter->height = 2.1568627f;
            parameter->radiusX = 3.490196f;
            parameter->radiusY = 7.0980392f;
            parameter->scalar = -0.20980392f;
        } else {
            parameter->width = 0.98039216f;
            parameter->height = 1.5294118f;
            parameter->radiusX = 3.1764705f;
            parameter->radiusY = 6.7843137f;
            parameter->scalar = -0.29803923f;
        }
        SetFireFalloff(parameter->effect, parameter->radiusX, parameter->radiusY, parameter->gain);
        SetParticleEffectSize(4, parameter->effect, parameter->width, parameter->height);
        SetFireFalloffScalar(parameter->effect, parameter->scalar);
        if (parameter->frame == 28)
            HLib::DestroyCurrentTask();
    }
    if (parameter->frame <= 4) {
        parameter->width -= -0.086274512f;
        parameter->height -= -0.10196079f;
        SetParticleEffectSize(4, parameter->effect, parameter->width, parameter->height);
    } else if (parameter->frame - 5 <= 9) {
        parameter->width -= -0.16470589f;
        parameter->height -= -0.16470587f;
        parameter->radiusX -= -0.11764705f;
        parameter->radiusY -= -0.15686274f;
        parameter->scalar -= 0.001372549f;
        SetParticleEffectSize(4, parameter->effect, parameter->width, parameter->height);
        SetFireFalloff(parameter->effect, parameter->radiusX, parameter->radiusY, parameter->gain);
        SetFireFalloffScalar(parameter->effect, parameter->scalar);
    } else if (parameter->frame - 15 <= 12) {
        parameter->width -= 0.08446455f;
        parameter->height -= 0.048265457f;
        parameter->radiusX -= 0.024132729f;
        parameter->radiusY -= 0.030165892f;
        parameter->scalar -= 0.0067873308f;
        SetFireFalloff(parameter->effect, parameter->radiusX, parameter->radiusY, parameter->gain);
        SetParticleEffectSize(4, parameter->effect, parameter->width, parameter->height);
        SetFireFalloffScalar(parameter->effect, parameter->scalar);
    }
    ++parameter->frame;
}

void Prepare0516Characters(std::int32_t selection) {
    for (const shen_name name : {AREA_0516_EVENT_RYO, AREA_0516_EVENT_SIN}) {
        EV_EnsureCharacterDraw(name);
        EV_EnableCharacterDisplay(name);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    for (const shen_name name : {AREA_0516_EVENT_a287, AREA_0516_EVENT_a288}) {
        EV_EnsureCharacterDraw(name);
        EV_EnableCharacterDisplay(name);
        EV_SetCharacterAlphaEnabled(name, false);
    }
    const std::span<const shen_name> props = selection == 0 ? std::span<const shen_name>{firstProps0516} :
        selection == 1                                      ? std::span<const shen_name>{secondProps0516} :
        selection == 2                                      ? std::span<const shen_name>{thirdProps0516} :
        selection == 3                                      ? std::span<const shen_name>{fourthProps0516} :
                                                              std::span<const shen_name>{};
    for (const shen_name name : props) {
        EV_EnsureCharacterDraw(name);
        EV_EnableCharacterDisplay(name);
        EV_SetCharacterAlphaEnabled(name, false);
    }
}

void Restore0516Characters(std::int32_t selection) {
    for (const shen_name name : {AREA_0516_EVENT_RYO, AREA_0516_EVENT_SIN}) {
        EV_ToggleCharacterFlag(name);
        EV_DisableCharacterDisplay(name);
        mtwkSetExtraFlag(scenFindMotionwork(name), 6, 0);
        EV_SetCharacterAlphaEnabled(name, true);
    }
    for (const shen_name name : {AREA_0516_EVENT_a287, AREA_0516_EVENT_a288}) {
        EV_ToggleCharacterFlag(name);
        EV_DisableCharacterDisplay(name);
        EV_SetCharacterAlphaEnabled(name, true);
    }
    const std::span<const shen_name> props = selection == 0 ? std::span<const shen_name>{firstRestoreProps0516} :
        selection == 1                                      ? std::span<const shen_name>{secondProps0516} :
        selection == 2                                      ? std::span<const shen_name>{thirdProps0516} :
        selection == 3                                      ? std::span<const shen_name>{fourthProps0516} :
                                                              std::span<const shen_name>{};
    for (const shen_name name : props) {
        EV_ToggleCharacterFlag(name);
        EV_DisableCharacterDisplay(name);
        EV_SetCharacterAlphaEnabled(name, true);
    }
}

void Update0516FireLighting(std::int16_t firstLight) {
    const float intensity = sin_fixed16(static_cast<std::int16_t>(flickerAngle0516)) * 0.05f + 0.7f;
    flickerCountdown0516 = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(flickerCountdown0516) - 1U);
    if (flickerCountdown0516 <= 0) {
        flickerCountdown0516 = shenGetRandomInt32(60) + 30;
        flickerStep0516 = shenGetRandomInt32(2048) + 2048;
    }
    flickerAngle0516 = std::bit_cast<std::int32_t>(std::bit_cast<std::uint32_t>(flickerAngle0516) + static_cast<std::uint32_t>(flickerStep0516));
    Vector3 position{};
    Vector3i rotation{};
    Vector3 offset{-0.01f, 0.35f, -0.013f};
    if (firstLight != -1) {
        EV_GetCharaPosition(AREA_0516_EVENT_a287, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(AREA_0516_EVENT_a287, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
        SetLightRuntimePosition(static_cast<std::uint32_t>(firstLight), offset);
        PrioritizeLight(firstLight);
        SetLightSceneIntensity(static_cast<std::uint32_t>(firstLight), intensity);
    }
    offset = Vector3{-0.01f, 0.35f, -0.013f};
    EV_GetCharaPosition(AREA_0516_EVENT_a288, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0516_EVENT_a288, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetLightRuntimePosition(2, offset);
    PrioritizeLight(2);
    SetLightSceneIntensity(2, intensity);
}

void Update0516FirstAuthor(void *data) {
    Area0516AuthorParameter *const child = static_cast<Area0516AuthorParameter *>(data);
    Update0516FireLighting(1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0516Characters(child->parent->selection);
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

void Update0516ThirdAuthor(void *data) {
    Area0516AuthorParameter *const child = static_cast<Area0516AuthorParameter *>(data);
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 240)
        SetIsSkySetup(0);
    else if (frame == 435)
        SetIsSkySetup(1);
    Update0516FireLighting(-1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0516Characters(child->parent->selection);
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

void Update0516FourthAuthor(void *data) {
    Area0516AuthorParameter *const child = static_cast<Area0516AuthorParameter *>(data);
    EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    Update0516FireLighting(1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0516Characters(child->parent->selection);
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

void Update0516SecondAuthor(void *data) {
    Area0516AuthorParameter *const child = static_cast<Area0516AuthorParameter *>(data);
    const Vector3 zero{};
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 0:
        ResumeParticleEffect(4, spreadingFire0516);
        SetFirePosition(spreadingFire0516, 1, &firePositions0516[0]);
        SetFireInstanceSize(spreadingFire0516, 1, 0.9803921580314636f, 1.529411792755127f);
        break;
    case 179:
        Queue0516FireStart(growingFires0516[0], 1);
        break;
    case 384:
        Queue0516FireStart(growingFires0516[1], 2);
        break;
    case 420:
        SuspendParticleEffect(4, growingFires0516[0]);
        SuspendParticleEffect(4, growingFires0516[1]);
        for (std::int32_t instance = 2; instance != 6; ++instance) {
            SetFirePosition(spreadingFire0516, instance, &firePositions0516[instance - 1]);
            SetFireInstanceSize(spreadingFire0516, instance, 0.9803921580314636f, 1.529411792755127f);
        }
        break;
    case 684:
        Queue0516FireStart(growingFires0516[0], 6);
        break;
    case 820:
        SuspendParticleEffect(4, trackedFire0516);
        SuspendParticleEffect(4, growingFires0516[0]);
        SetFirePosition(spreadingFire0516, 6, &firePositions0516[5]);
        SetFireInstanceSize(spreadingFire0516, 6, 0.9803921580314636f, 1.529411792755127f);
        break;
    case 855:
        Queue0516FireStart(growingFires0516[0], 7);
        break;
    case 930:
        SuspendParticleEffect(4, growingFires0516[0]);
        for (std::int32_t instance = 7; instance != 9; ++instance) {
            SetFirePosition(spreadingFire0516, instance, &firePositions0516[instance - 1]);
            SetFireInstanceSize(spreadingFire0516, instance, 0.9803921580314636f, 1.529411792755127f);
        }
        break;
    case 2808:
        SuspendParticleEffect(4, carriedFire0516);
        break;
    case 3028:
        ResumeParticleEffect(4, carriedFire0516);
        break;
    case 3428:
        SetFirePosition(carriedFire0516, 9, nullptr);
        break;
    case 3793:
        SetFirePosition(carriedFire0516, 9, &zero);
        AttachFireInstance(carriedFire0516, 9, AREA_0516_EVENT_a288, 152, std::bit_cast<std::int32_t>(0.1f));
        break;
    case 4773:
        SuspendParticleEffect(4, carriedFire0516);
        SetFirePosition(carriedFire0516, 9, nullptr);
        EV_SetIndexForLightEntry(2, 0);
        break;
    case 5060:
        AttachFireEffect(carriedFire0516, AREA_0516_EVENT_a288, 152, 0.1f);
        ResumeParticleEffect(4, carriedFire0516);
        break;
    default:
        break;
    }
    if (static_cast<std::uint32_t>(frame - 4774) <= 631U) {
        fogBlue0516 += 0.0011867088f;
        SetFogBlue(fogBlue0516);
        ReinitFogTasks(1);
    } else if (frame == 5406) {
        fogBlue0516 = 1.0f;
        SetFogBlue(1.0f);
    }
    Vector3 position{};
    EV_GetCharaPosition(AREA_0516_EVENT_a292, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    SetParticleEffectPosition(4, trackedFire0516, &position, &position);
    Update0516FireLighting(1);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0516Characters(child->parent->selection);
        child->parent->state = 3;
        HLib::DestroyCurrentTask();
    }
}

const ScriptAreaDefinition area0516ScriptDefinition{.areaId = AREA_0516_EVENT, .field04 = 0, .init = Initialize0516, .main = Start0516};
} // namespace shenmue::s2::areas
