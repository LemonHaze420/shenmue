#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0278.hpp"
#include "s2/event_queued_dc.h"

#if defined(SHENMUE_EVENT_ENTRY)
void AreaMain(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(EventMain, 4, 11, sizeof(AreaImmediateAuthorTaskParameter), TASK_EVNT);
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 1;
    parent->selection = (unsigned int)g_entryPoint > 3 ? 0 : g_entryPoint;
    PrepareEnvironment();
    Queue_EVT_UTL_LoadNPCChara_3(area0278Characters, parent->self);
}
#else
Vector3 smokePositions[] = {{330.64f, 128.05f, 268.0f}, {341.21f, 128.05f, 268.0f}};
unsigned short candlePalette[] = {
    0x311, 0x311, 0x411, 0x511, 0x612, 0x712, 0xa32, 0xb32, 0xb42, 0xc50, 0xd70, 0xe70, 0xe80, 0xfa0, 0xfc3, 0xfd6};
Vector3 candlePositions[] = {{159.66f, 100.33f, 133.7f}, {159.42f, 100.33f, 133.71f}};
shen_name area0278Characters[] = {AREA_0278_HUNHUI, AREA_0278_ZHOUSHAN, AREA_0278_SYUEI, AREA_0278_FANGMEI, INVALID_CHARACTER};
static unsigned int smokeEffects[2];
static unsigned int candleEffects[2];
static unsigned int templeFire;

void EventMain(void *taskData)
{
    AreaImmediateAuthorTaskParameter *parent = (AreaImmediateAuthorTaskParameter *)taskData;
    switch (parent->state) {
    case 1:
        QueueAuthor(parent);
        break;
    case 3:
        FinishEvent(parent);
        break;
    }
}

void QueueAuthor(AreaImmediateAuthorTaskParameter *parent)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(AuthorLoop, 4, 11, sizeof(Area0278LoopParameter), TASK_EVNT);
    ((Area0278LoopParameter *)FTBL.HLib_GetTaskParameter(task))->parent = parent;
    FTBL.SetMapModelDataDefaultVertexShader();
    switch (parent->selection) {
    case 0:
        FTBL.EV_lightLoad(AREA_0278_L001);
        break;
    case 1:
        FTBL.EV_lightLoad(AREA_0278_L101);
        break;
    case 2:
        CreateCandles();
        FTBL.SetClothFlag(FTBL.ctrlGetCharaByName(AREA_0278_HUNHUI), 2, 1);
        break;
    case 3:
        FTBL.EV_lightLoad(AREA_0278_L301);
        break;
    }
    FTBL.PushGlobalLightingProgram();
    PrepareAuthorCharacters(parent->selection);
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void AuthorLoop(void *taskData)
{
    Area0278LoopParameter *child = (Area0278LoopParameter *)taskData;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (child->parent->selection == 0 && frame == 1)
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (child->parent->selection == 1 && frame == 545)
        FTBL.SetPlayerGameFlags(4, (void *)0);
    else if (child->parent->selection == 3 && frame == 2080)
        FTBL.SetPlayerGameFlags(4, (void *)1);
    if (child->parent->selection == 2 && frame == 350) {
        candlePositions[0].x = 159.45f;
        candlePositions[0].y = 100.34f;
        candlePositions[0].z = 133.56f;
        candlePositions[1].x = 159.54f;
        candlePositions[1].y = 100.32f;
        candlePositions[1].z = 133.45f;
        FTBL.SetParticleEffectPosition(4, candleEffects[0], &candlePositions[0], &candlePositions[0]);
        FTBL.SetParticleEffectPosition(4, candleEffects[1], &candlePositions[1], &candlePositions[1]);
    } else if (child->parent->selection == 2 && frame == 800) {
        FTBL.SuspendParticleEffect(4, candleEffects[0]);
        FTBL.SuspendParticleEffect(4, candleEffects[1]);
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        if (child->parent->selection == 3) {
            FTBL.HLib_SetupTimedTask(child->parent->self, 60);
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            RestoreAuthorCharacters();
            child->parent->state = 3;
            child->parent->selection = 0;
            if (child->parent->selection == 2)
                FreeCandles();
        } else {
            child->parent->selection++;
            child->parent->state = 1;
        }
        FTBL.HLib_DestroyCurrentTask();
    }
}

void CreateCandles(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        candleEffects[i] = FTBL.CreateFireEffect(&candlePositions[i], 0.2f, 0.5f, 0, 0);
        FTBL.SetFirePalette(candleEffects[i], candlePalette);
        FTBL.SetParticleEffectPosition(4, candleEffects[i], &candlePositions[i], &candlePositions[i]);
        FTBL.SetFireFalloff(candleEffects[i], 3.1764705f, 6.7843137f, 0.7058824f);
        FTBL.SetFireFalloffScalar(candleEffects[i], -0.29803923f);
        FTBL.SetParticleEffectSize(4, candleEffects[i], 0.3529412f, 0.47058824f);
        FTBL.ResumeParticleEffect(4, candleEffects[i]);
    }
}

void FreeCandles(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        FTBL.SuspendParticleEffect(4, candleEffects[i]);
        FTBL.FreeParticleEffect(4, candleEffects[i]);
    }
}

void PrepareEnvironment(void)
{
    EventDateTime time;
    Vector3 offset = {0.0f, -2000.0f, 0.0f};
    FTBL.SetPlanetLayerOffset(&offset, 0);
    FTBL.SetPlanetLayerOffset(&offset, 1);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 20;
    time.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_SetWeatherInstruction(12, 0);
    FTBL.EV_SetPlayerFlag(12, 123, 1);
    FTBL.SetPlayerGameFlags(4, (void *)1);
    FTBL.SetPlanetLayerParameter(0.0f, 0);
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    FTBL.EV_FreeAuthorPak(1, "0278");
    CreateTempleFire();
    CreateTempleSmoke();
}

void FinishEvent(AreaImmediateAuthorTaskParameter *parent)
{
    EventDateTime time;
    FTBL.AddInventoryItem(3025, 1, 1);
    AdvanceSleepToMorning();
    FTBL.EV_stepSetFlags(300, 0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.day += 2;
    time.hour = 8;
    time.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.AddInventoryItem(22, 1, 0);
    EVT_UTL_FreeNPCChara(area0278Characters);
    FTBL.EV_FreeAuthorPak(0, "0278");
    FTBL.HLib_DestroyTask(parent->self);
    FTBL.SetSceneInfos(4, AREA_0278_RETURN, 0);
    FTBL.StartShowScreenOfId(4);
}

void PrepareAuthorCharacters(short author)
{
    Vector3 scale;
    HideAuthorDisplay(RYO_CHARACTER);
    HideAuthorDisplay(AREA_0278_FANGMEI);
    HideAuthorDisplay(AREA_0278_SYUEI);
    HideAuthorDisplay(AREA_0278_HUNHUI);
    HideAuthorDisplay(AREA_0278_ZHOUSHAN);
    HideAuthorProp(AREA_0278_A015);
    HideAuthorProp(AREA_0278_A016);
    HideAuthorProp(AREA_0278_A236);
    HideAuthorProp(AREA_0278_A411);
    HideAuthorProp(AREA_0278_A410);
    HideAuthorProp(AREA_0278_A716);
    HideAuthorProp(AREA_0278_AA34);
    switch (author) {
    case 0:
        PrepareAuthorCharacter(RYO_CHARACTER);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        FTBL.LoadBlurModelForCharacter_0(AREA_0278_SYUEI, "SYE_B.MT7", 1.0f);
        break;
    case 3:
        PrepareAuthorCharacter(RYO_CHARACTER);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        break;
    case 1:
        PrepareAuthorCharacter(RYO_CHARACTER);
        PrepareAuthorCharacter(AREA_0278_SYUEI);
        PrepareAuthorCharacter(AREA_0278_FANGMEI);
        break;
    case 2:
        PrepareAuthorCharacter(AREA_0278_HUNHUI);
        PrepareAuthorCharacter(AREA_0278_ZHOUSHAN);
        break;
    }
    if (author == 2) {
        scale.x = scale.y = scale.z = 0.7f;
        FTBL.mtwkSetMotionworkScale3(AREA_0278_A411, &scale);
        FTBL.mtwkSetMotionworkScale3(AREA_0278_A410, &scale);
    } else {
        scale.x = scale.y = scale.z = 1.0f;
        FTBL.mtwkSetMotionworkScale3(AREA_0278_A411, &scale);
        FTBL.mtwkSetMotionworkScale3(AREA_0278_A410, &scale);
    }
}

void RestoreAuthorCharacters(void)
{
    int i;
    RestoreAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0278_A015);
    ShowAuthorProp(AREA_0278_A016);
    ShowAuthorProp(AREA_0278_A236);
    ShowAuthorProp(AREA_0278_A411);
    ShowAuthorProp(AREA_0278_A410);
    ShowAuthorProp(AREA_0278_A716);
    ShowAuthorProp(AREA_0278_AA34);
    for (i = 0; area0278Characters[i] != INVALID_CHARACTER; i++)
        RestoreAuthorCharacter(area0278Characters[i]);
}

void HideAuthorDisplay(shen_name name)
{
    FTBL.EV_ToggleCharacterFlag(name);
    FTBL.EV_DisableCharacterDisplay(name);
}

void PrepareAuthorCharacter(shen_name name)
{
    FTBL.EV_EnsureCharacterDraw(name);
    FTBL.EV_EnableCharacterDisplay(name);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(name, 0);
}

void RestoreAuthorCharacter(shen_name name)
{
    FTBL.EV_EnableCharacterDisplay(name);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(name), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(name, 1);
}

void HideAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 1);
}

void CreateTempleSmoke(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        Vector3 position = {333.1508f, 127.9809f, 247.705f};
        Vector3 spawnVolume = {0.1f, 0.0f, 0.1f};
        Vector3 variance = {0};
        Vector3 velocity = {0.0f, 0.016f, 0.0f};
        position.x = smokePositions[i].x;
        position.y = smokePositions[i].y;
        position.z = smokePositions[i].z;
        smokeEffects[i] = FTBL.CreateSmokeEffect(&position, &spawnVolume, 40, 0.05f, 0, 0);
        FTBL.SetSmokeEffectFadeFactor(smokeEffects[i], 21);
        FTBL.SetSmokeEffectFalloff(smokeEffects[i], 110);
        FTBL.SetParticleEffectColour(0, smokeEffects[i], 0, 144, 144, 144);
        FTBL.SetParticleEffectInitialScale(0, smokeEffects[i], 0.329f);
        FTBL.SetParticleEffectSize(0, smokeEffects[i], 1.56f, 1.41f);
        FTBL.SetParticleEffectBirthRate(0, smokeEffects[i], 0.0078f);
        FTBL.SetParticleEffectVelocity(0, smokeEffects[i], &velocity, &variance);
        FTBL.ResumeParticleEffect(0, smokeEffects[i]);
    }
}

void CreateTempleFire(void)
{
    int i;
    Vector3 positions[10];
    positions[0].x = 343.5f;
    positions[0].z = 228.3f;
    positions[1].x = 343.5f;
    positions[1].z = 235.5f;
    positions[2].x = 327.1f;
    positions[2].z = 276.0f;
    positions[3].x = 343.7f;
    positions[3].z = 267.9f;
    positions[4].x = 343.7f;
    positions[4].z = 276.0f;
    positions[5].x = 327.1f;
    positions[5].z = 228.3f;
    positions[6].x = 327.1f;
    positions[6].z = 236.4f;
    positions[7].x = 327.1f;
    positions[7].z = 247.2f;
    positions[8].x = 327.1f;
    positions[8].z = 257.1f;
    positions[9].x = 327.1f;
    positions[9].z = 267.9f;
    positions[0].y = positions[1].y = 129.1f;
    positions[2].y = positions[3].y = positions[4].y = positions[5].y = 128.4f;
    positions[6].y = positions[7].y = positions[8].y = positions[9].y = 128.4f;
    templeFire = FTBL.CreateFireEffect(&positions[0], 0.3f, 0.4f, 0, 4);
    FTBL.ResumeParticleEffect(4, templeFire);
    for (i = 1; i < 10; i++)
        FTBL.SetFirePosition(templeFire, i, &positions[i]);
}
#endif
#else
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
    mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(name)), 6, 2);
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
    mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(AREA_0278_RYO)), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0278_RYO, true);
    for (const shen_name name : AREA_0278_MODELS)
        EV_SetCharacterAlphaEnabled(name, true);
    for (const shen_name *name = AREA_0278_CHARACTERS; name->name != 0xffffffffU; ++name) {
        EV_EnableCharacterDisplay(*name);
        mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(*name)), 6, 0);
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
        SetHighModelAndFaceFlagsForChara(scenFindChara(author ? author->task : nullptr, AREA_0278_RYO), 0, 0.0f, 1.0f);
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
        SetClothFlag(ctrlGetScenarioChara(AREA_0278_HUNHUI), 2, true);
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
    shenGetCurrentDateTime(date);
    date.day += 2;
    date.hour = 8;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    AddInventoryItem(0x16, 1, 0);
    for (const shen_name *name = AREA_0278_CHARACTERS; name->name != 0xffffffffU; ++name) {
        if (!ctrlGetScenarioChara(*name))
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
    shenGetCurrentDateTime(date);
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
#endif
