#include "s2/areas/area_0048.hpp"
#include "s2/areas/area_0047.hpp"
#include "common/math.hpp"
#include "s2/author.hpp"
#include "s2/blur_figure_pose.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0048;
std::int32_t lightRow0048, lightDelay0048;
float lightPhase0048;
void E0048MainCallback(void *);
void E0048LoopCallback(void *);
// @verified - p
void E0048Init() {
    fire0048 = 0;
}
// @verified - p
void SetupE0048Environment() {
    fire0048 = static_cast<std::uint32_t>(CreateFireEffect({106.45999908447266f, 90.0999984741211f, 152.22999572753906f}, 0.5f, 0.5f, 0, 0));
    SetFirePalette(fire0048, AREA_0047_PALETTE);
    SetFireFalloffScalar(fire0048, -0.05000000074505806f);
    SetFireFalloff(fire0048, 3.140000104904175f, 4.940000057220459f, 0.7799999713897705f);
    SetParticleEffectSize(4, fire0048, 0.02500000037252903f, 0.05000000074505806f);
    if ((g_entryPoint & 0xfffffffdU) != 0) {
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 21;
        time.minute = 0;
        SetCurrentDateTimeTo(time);
    }
    EV_SetWeatherInstruction(12, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(AREA_0048_LIGHT);
    PushGlobalLightingProgram();
    EV_FreeAuthorPak(1, "0048");
}
// @verified - p
void E0048Main(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0048MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 1;
    parent->selection = static_cast<std::int16_t>(g_entryPoint > 5 ? 0 : g_entryPoint);
    SetupE0048Environment();
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0048_CHARACTERS[parent->selection], parent->self);
    lightRow0048 = 0;
    lightDelay0048 = 0;
}
// @verified - p
void SetupE0048Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    const Vector3 zero{};
    EV_AT_SetPos(AREA_0048_A047, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A047, false);
    EV_AT_SetPos(AREA_0048_A021_SHIFT, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A021_SHIFT, false);
    EV_AT_SetPos(AREA_0048_A020_SHIFT, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A020_SHIFT, false);
    EV_AT_SetPos(AREA_0048_A073, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A073, false);
    EV_AT_SetPos(AREA_0048_A020, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A020, false);
    EV_AT_SetPos(AREA_0048_A022, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A022, false);
    EV_AT_SetPos(AREA_0048_A127, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A127, false);
    EV_AT_SetPos(AREA_0048_A128, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A128, false);
    EV_AT_SetPos(AREA_0048_A009, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A009, false);
    EV_AT_SetPos(AREA_0048_A041, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A041, false);
    EV_AT_SetPos(AREA_0048_A021, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A021, false);
    EV_AT_SetPos(AREA_0048_A417, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A417, false);
    EV_AT_SetPos(AREA_0048_AA84, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_AA84, false);
    EV_AT_SetPos(AREA_0048_A412, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_A412, false);
    EV_AT_SetPos(AREA_0048_AA63, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
    EV_SetCharacterAlphaEnabled(AREA_0048_AA63, false);
    if (selection == 4)
        LoadBlurModelForCharacter(AREA_0048_SYE, "SYE_B.MT7", 1.0f);
    for (const shen_name *character = AREA_0048_CHARACTERS[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0048Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A047, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A021_SHIFT, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A020_SHIFT, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A073, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A020, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A022, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A127, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A128, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A009, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A041, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A021, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A417, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_AA84, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_A412, true);
    EV_SetCharacterAlphaEnabled(AREA_0048_AA63, true);
    for (const shen_name *character = AREA_0048_CHARACTERS[selection]; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
// @verified - p
void UpdateE0048Lighting() {
    if (lightDelay0048 < 10) {
        ++lightDelay0048;
        return;
    }
    const std::int32_t frame = static_cast<std::int32_t>(spliGetCurrentFrmCnt());
    const E0048TimedLights *row = &AREA_0048_TIMED_LIGHTS[lightRow0048];
    if (frame >= row->frame && row->frame != -1)
        row = &AREA_0048_TIMED_LIGHTS[++lightRow0048];
    for (std::int32_t index = 0; index < row->count; ++index) {
        const E0048Light &light = row->lights[index];
        const std::uint16_t angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(lightPhase0048 * 182.04443f));
        const std::uint16_t tripleAngle = static_cast<std::uint16_t>(static_cast<std::int32_t>(lightPhase0048 * 3.0f * 182.04443f));
        const std::int16_t folded = static_cast<std::int16_t>(angle <= 0x8000U ? angle : 0U - angle);
        const std::int16_t tripleFolded = static_cast<std::int16_t>(tripleAngle <= 0x8000U ? tripleAngle : 0U - tripleAngle);
        const float amplitude = sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)) * light.amplitude;
        SetLightSceneIntensity(light.index, amplitude * sin_fixed16(static_cast<std::int16_t>(0x4000 - tripleFolded)) + light.base);
    }
    lightPhase0048 += 20.0f;
    if (lightPhase0048 >= 180.0f)
        lightPhase0048 = -180.0f;
}
// @verified - p
void CleanupE0048(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0048_CHARACTERS[parent->selection]; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    FreeParticleEffect(4, fire0048);
    EV_FreeAuthorPak(0, "0048");
    EV_CleanupMapControlTask();
    HLib::Task *const task = parent->self;
    if (parent->selection == 0) {
        HLib::DestroyTask(task);
        EV_scenQueueLoad(g_actNumber, AREA_0048_YTQB, 0);
    } else if (parent->selection == 2) {
        HLib::DestroyTask(task);
        shenUpdateCurrentDateTime();
        DateTime time;
        GetCurrentDateTime(time);
        time.hour = 8;
        time.minute = 30;
        SetCurrentDateTimeTo(time);
        EV_scenQueueLoad(g_actNumber, AREA_0048_WB00, 33);
    } else {
        HLib::DestroyTask(task);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    }
}
// @verified - p
void QueueE0048Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0048LoopCallback, 4, 0x0b, 8, AREA_AUTHOR_TASK);
    *static_cast<AreaImmediateAuthorTaskParameter **>(HLib::GetTaskParameter(task)) = parent;
    if (parent->selection == 2) {
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0048_SELECTED_LIGHTS[2]);
        PushGlobalLightingProgram();
    } else if (parent->selection >= 0 && parent->selection <= 5)
        EV_lightLoad(AREA_0048_SELECTED_LIGHTS[parent->selection]);
    SetupE0048Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void E0048MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0048Loop(parent);
    else if (parent->state == 3)
        CleanupE0048(parent);
}
// @verified - p
void E0048LoopCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = *static_cast<AreaImmediateAuthorTaskParameter **>(taskData);
    Vector3 offset{0, 0.14f, 0};
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    if (frame == 1)
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    if (parent->selection == 2) {
        if (frame == 1)
            ResumeParticleEffect(4, fire0048);
        else if (frame > 1) {
            Vector3 position{};
            Vector3i rotation{};
            EV_GetCharaPosition(AREA_0048_A412, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
            GetScenarioCharacterEulerAngles(AREA_0048_A412, &rotation, EventTransformFlags::World);
            ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
            SetParticleEffectPosition(4, fire0048, &offset, &offset);
        }
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0) {
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        parent->state = 3;
        RestoreE0048Characters(parent->selection);
        HLib::DestroyCurrentTask();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        UpdateE0048Lighting();
}
} // namespace
const ScriptAreaDefinition area0048ScriptDefinition{.areaId = AREA_0048_AREA, .field04 = 0, .init = E0048Init, .main = E0048Main};
} // namespace shenmue::s2::areas
