#include "s2/areas/area_0278.hpp"
#include "s2/lighting.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/world_render.hpp"

#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/face.hpp"
#include "s2/hlib.hpp"
#include "s2/inventory.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/particle_effects.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s2::areas {
namespace {

// AreaMain's EventParameter constructor allocates 0x10, not the IDB's 0x0c type.
struct EventParameter {
    HLib::Task *task;
    std::int16_t author;
    std::int16_t mode;
    std::byte reserved0c[4];
};
struct AuthorLoopParameter {
    EventParameter *parent;
};
static_assert(sizeof(EventParameter) == 0x10);
static_assert(offsetof(EventParameter, author) == 8);
static_assert(offsetof(EventParameter, mode) == 10);
static_assert(sizeof(AuthorLoopParameter) == 8);

constexpr std::uint16_t candlePalette[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
constexpr Vector3 smokePositions[]{{330.64f, 128.05f, 268.0f}, {341.21f, 128.05f, 268.0f}};
Vector3 candlePositions[]{{159.66f, 100.33f, 133.7f}, {159.42f, 100.33f, 133.71f}};
std::uint32_t templeFire{};
std::array<std::uint32_t, 2> candleEffects{};
std::array<std::uint32_t, 2> smokeEffects{};

void SetAuthorPlayerFlag(bool enabled) {
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, enabled ? reinterpret_cast<void *>(std::uintptr_t{1}) : nullptr);
}

void CreateTempleFire() {
    constexpr Vector3 positions[]{{343.5f, 129.1f, 228.3f}, {343.5f, 129.1f, 235.5f}, {327.1f, 128.4f, 276.0f}, {343.7f, 128.4f, 267.9f},
        {343.7f, 128.4f, 276.0f}, {327.1f, 128.4f, 228.3f}, {327.1f, 128.4f, 236.4f}, {327.1f, 128.4f, 247.2f}, {327.1f, 128.4f, 257.1f},
        {327.1f, 128.4f, 267.9f}};
    templeFire = static_cast<std::uint32_t>(CreateFireEffect(positions[0], 0.3f, 0.4f, 0, 4));
    ResumeParticleEffect(4, templeFire);
    for (std::int32_t index = 1; index < 10; ++index)
        SetFirePosition(templeFire, index, &positions[index]);
}

void CreateTempleSmoke() {
    for (std::size_t index = 0; index < smokeEffects.size(); ++index) {
        const std::uint32_t effect = static_cast<std::uint32_t>(CreateSmokeEffect(smokePositions[index], Vector3{0.1f, 0.0f, 0.1f}, 40, 0.05f, shen_name{}, 0));
        smokeEffects[index] = effect;
        SetSmokeEffectFadeFactor(effect, 21);
        SetSmokeEffectFalloff(effect, 110);
        SetParticleEffectColour(0, effect, 0, 144, 144, 144);
        SetParticleEffectInitialScale(0, effect, 0.329f);
        SetParticleEffectSize(0, effect, 1.56f, 1.41f);
        SetParticleEffectBirthRate(0, effect, 0.0078f);
        SetParticleEffectVelocity(0, effect, Vector3{0.0f, 0.016f, 0.0f}, Vector3{});
        ResumeParticleEffect(0, effect);
    }
}

void CreateCandles() {
    for (std::size_t index = 0; index < candleEffects.size(); ++index) {
        const std::uint32_t effect = static_cast<std::uint32_t>(CreateFireEffect(candlePositions[index], 0.2f, 0.5f, 0, 0));
        candleEffects[index] = effect;
        SetFirePalette(effect, candlePalette);
        SetParticleEffectPosition(4, effect, &candlePositions[index], &candlePositions[index]);
        SetFireFalloff(effect, 3.176470518f, 6.784313679f, 0.7058823705f);
        SetFireFalloffScalar(effect, -0.2980392277f);
        SetParticleEffectSize(4, effect, 0.3529411852f, 0.4705882370f);
        ResumeParticleEffect(4, effect);
    }
}

void PrepareAuthorCharacter(shen_name name) {
    EV_EnsureCharacterDraw(name);
    EV_EnableCharacterDisplay(name);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(name)), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
}

void PrepareAuthorCharacters(std::int16_t author) {
    for (const shen_name name : {AREA_0278_RYO, AREA_0278_FANGMEI, AREA_0278_SYUEI, AREA_0278_HUNHUI, AREA_0278_ZHOUSHAN}) {
        EV_ToggleCharacterFlag(name);
        EV_DisableCharacterDisplay(name);
    }
    for (const shen_name name : AREA_0278_MODELS)
        EV_SetCharacterAlphaEnabled(name, false);
    switch (author) {
    case 0:
        PrepareAuthorCharacter(AREA_0278_RYO);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        LoadBlurModelForCharacter(AREA_0278_SYUEI, "SYE_B.MT7", 1.0f);
        break;
    case 1:
        PrepareAuthorCharacter(AREA_0278_RYO);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        PrepareAuthorCharacter(AREA_0278_FANGMEI);
        break;
    case 2:
        PrepareAuthorCharacter(AREA_0278_HUNHUI);
        PrepareAuthorCharacter(AREA_0278_ZHOUSHAN);
        break;
    case 3:
        PrepareAuthorCharacter(AREA_0278_RYO);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        break;
    }
    const float scale = author == 2 ? 0.7f : 1.0f;
    EV_SetCharacterScale(SHEN_NAME(a411), Vector3{scale, scale, scale});
    EV_SetCharacterScale(SHEN_NAME(a410), Vector3{scale, scale, scale});
}

void RestoreAuthorCharacters() {
    EV_EnableCharacterDisplay(AREA_0278_RYO);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(AREA_0278_RYO)), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0278_RYO, true);
    for (const shen_name name : AREA_0278_MODELS)
        EV_SetCharacterAlphaEnabled(name, true);
    for (const shen_name *name = AREA_0278_CHARACTERS; name->name != 0xffffffffU; ++name) {
        EV_EnableCharacterDisplay(*name);
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(*name)), 6, 0);
        EV_SetCharacterAlphaEnabled(*name, true);
    }
}

void AuthorLoop(void *taskData) {
    EventParameter *const parent = static_cast<AuthorLoopParameter *>(taskData)->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->author == 0 && frame == 1) {
        camCtrlResetTranslationScale();
        camCtrlResetClipDistances();
        StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 30, 255, 0);
    }
    if (parent->author == 1 && frame == 190) {
        camCtrlSetTranslationScale(0.032000002f);
        camCtrlSetClipDistances(0.01f, 2200.0f);
    }
    if (parent->author == 2 && frame == 1) {
        camCtrlResetClipDistances();
        camCtrlResetTranslationScale();
        SetAuthorPlayerFlag(false);
    }
    if (parent->author == 3 && frame == 1) {
        const AuthorController *const author = Author_GetController();
        SetHighModelAndFaceFlagsForChara(FindScenarioCharacter(author ? author->task : nullptr, AREA_0278_RYO), 0, 0.0f, 1.0f);
    }
    if (parent->author == 1 && frame == 545)
        SetAuthorPlayerFlag(false);
    if (parent->author == 3 && frame == 2080)
        SetAuthorPlayerFlag(true);
    if (parent->author == 2) {
        if (frame == 350) {
            candlePositions[0] = Vector3{159.45f, 100.34f, 133.56f};
            candlePositions[1] = Vector3{159.53999f, 100.32f, 133.45f};
            SetParticleEffectPosition(4, candleEffects[0], &candlePositions[0], &candlePositions[0]);
            SetParticleEffectPosition(4, candleEffects[1], &candlePositions[1], &candlePositions[1]);
        } else if (frame == 800) {
            SuspendParticleEffect(4, candleEffects[0]);
            SuspendParticleEffect(4, candleEffects[1]);
        }
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (parent->author == 3) {
        HLib::SetupTimedTask(parent->task, 60);
        StartScreenFilmFade(GetCurrentAreaScreenFilmTask(), 0, 0, 255);
        RestoreAuthorCharacters();
        parent->mode = 3;
        parent->author = 0;
        // AreaMain keeps this comparison after storing author=0.
        if (parent->author == 2) {
            for (const std::uint32_t effect : candleEffects) {
                SuspendParticleEffect(4, effect);
                FreeParticleEffect(4, effect);
            }
        }
    } else {
        ++parent->author;
        parent->mode = 1;
    }
    HLib::DestroyCurrentTask();
    camCtrlResetClipDistances();
    camCtrlResetTranslationScale();
    camCtrlRestoreCachedTransform();
}

void QueueAuthor(EventParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(AuthorLoop, 4, 0x0b, sizeof(AuthorLoopParameter), AREA_0278_TASK);
    static_cast<AuthorLoopParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    camCtrlCacheTransform();
    EV_SetModelDataDefaultVertexShader();
    switch (parent->author) {
    case 0:
        EV_lightLoad(AREA_0278_LIGHTS[0]);
        break;
    case 1:
        EV_lightLoad(AREA_0278_LIGHTS[1]);
        break;
    case 2:
        CreateCandles();
        SetClothFlag(FindCharacterByScenarioName(AREA_0278_HUNHUI), 2, true);
        break;
    case 3:
        EV_lightLoad(AREA_0278_LIGHTS[2]);
        break;
    }
    PushGlobalLightingProgram();
    PrepareAuthorCharacters(parent->author);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->author));
    parent->mode = 2;
}

void FinishEvent(EventParameter *parent) {
    AddInventoryItem(0xbd1, 1, 1);
    shenUpdateCurrentDateTime();
    EV_stepSetFlags(0, 0x12c);
    DateTime date{};
    GetCurrentDateTime(date);
    date.day += 2;
    date.hour = 8;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    AddInventoryItem(0x16, 1, 0);
    for (const shen_name *name = AREA_0278_CHARACTERS; name->name != 0xffffffffU; ++name) {
        if (!FindCharacterByScenarioName(*name))
            continue;
        EV_npcLockUnlock(*name);
        EV_npcFree(*name);
    }
    EV_FreeAuthorPak(0, "0278");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->task);
    camCtrlResetClipDistances();
    camCtrlResetTranslationScale();
    camCtrlRestoreCachedTransform();
    SetSceneInfos(4, AREA_0278_RETURN, 0);
    StartShowScreenOfId(4);
}

void EventMain(void *taskData) {
    EventParameter *const parent = static_cast<EventParameter *>(taskData);
    if (parent->mode == 1)
        QueueAuthor(parent);
    else if (parent->mode == 3)
        FinishEvent(parent);
}

void PrepareEnvironment() {
    const Vector3 planetOffset{0.0f, -2000.0f, 0.0f};
    SetPlanetLayerOffset(planetOffset, 0);
    SetPlanetLayerOffset(planetOffset, 1);
    DateTime date{};
    GetCurrentDateTime(date);
    date.hour = 20;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    SetPlayerControlEnabled(0);
    EV_SetWeatherInstruction(12, 0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 0x7b, 1);
    SetAuthorPlayerFlag(true);
    SetPlanetLayerParameter(0.0f, 0);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    EV_FreeAuthorPak(1, "0278");
    CreateTempleFire();
    CreateTempleSmoke();
}

void AreaInit() {
    templeFire = 0;
    candleEffects.fill(0);
    smokeEffects.fill(0);
}

void AreaMain(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(EventMain, 4, 0x0b, sizeof(EventParameter), AREA_0278_TASK);
    EventParameter *const parent = static_cast<EventParameter *>(HLib::GetTaskParameter(task));
    parent->task = task;
    parent->author = static_cast<std::int16_t>(g_entryPoint <= 3 ? g_entryPoint : 0);
    parent->mode = 1;
    PrepareEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0278_CHARACTERS, parent->task);
}

} // namespace


const ScriptAreaDefinition area0278ScriptDefinition{.areaId = AREA_0278, .field04 = 0, .init = AreaInit, .main = AreaMain};

} // namespace shenmue::s2::areas
