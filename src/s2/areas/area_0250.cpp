#include "s2/areas/area_0250.hpp"
#include "s2/map_control.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/world_render.hpp"
namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0250{};
void E0250MainCallback(void *);
void E0250LoopCallback(void *);
// @verified - p
void E0250Init() {
    fire0250 = 0;
}
// @verified - p
void SetupE0250Environment(std::int16_t) {
    const Vector3 zero{};
    fire0250 = static_cast<std::uint32_t>(CreateFireEffect(zero, 0.025f, 0.05f, 0, 4));
    SetFirePalette(fire0250, AREA_0250_PALETTE);
    SetFireFalloffScalar(fire0250, -0.05f);
    SetFireFalloff(fire0250, 3.140000104904175f, 4.940000057220459f, 0.7799999713897705f);
    ResumeParticleEffect(4, fire0250);
    SetPlayerControlEnabled(0);
    SetRenderClockAndCompassTo(1);
    EV_FreeAuthorPak(1, "0250");
    EV_FreeAuthorPak(1, "0251");
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_SetWeatherInstruction(12, 0);
    const Vector3 offset{0.0f, 10.800000190734863f, 0.0f};
    for (std::int32_t index = 0; index < 6; ++index)
        if (index != 1)
            SetMapModelSlotTransform(index, &offset, 0);
    SetTempCommandStreamOffset(&offset);
    for (const shen_name character : AREA_0250_HIDDEN_PROPS)
        SetDisplayFlagForProp(character, 0);
}
// @verified - p
void E0250Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0250MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_entryPoint != 0;
    parent->state = 1;
    SetupE0250Environment(parent->selection);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0250_CHARACTERS, parent->self);
}
// @verified - p
void SetE0250SceneVisibility(std::int16_t selection) {
    for (std::int32_t index = 0; index < 6; ++index)
        EV_SetObjectFlagByIndex(index, selection != 0 || index == 1 ? 1 : 0);
}
void SetupE0250Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    for (const shen_name character : AREA_0250_PROPS) {
        EV_EnableCharacterDisplay(character);
        EV_SetCharacterAlphaEnabled(character, false);
    }
    if (selection != 0) {
        EV_DisableCharacterDisplay(AREA_0250_A297);
        EV_DisableCharacterDisplay(AREA_0250_A046);
    } else {
        EV_EnableCharacterDisplay(AREA_0250_AB07);
        EV_SetCharacterAlphaEnabled(AREA_0250_AB07, false);
        EV_EnableCharacterDisplay(AREA_0250_A297);
        EV_SetCharacterAlphaEnabled(AREA_0250_A297, false);
        EV_EnableCharacterDisplay(AREA_0250_A046);
        EV_SetCharacterAlphaEnabled(AREA_0250_A046, false);
    }
    for (const shen_name *character = AREA_0250_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0250Characters(std::int16_t selection) {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    for (const shen_name character : AREA_0250_PROPS)
        EV_SetCharacterAlphaEnabled(character, true);
    if (selection == 0) {
        EV_SetCharacterAlphaEnabled(AREA_0250_AB07, true);
        EV_SetCharacterAlphaEnabled(AREA_0250_A297, true);
        EV_SetCharacterAlphaEnabled(AREA_0250_A046, true);
    }
    for (const shen_name *character = AREA_0250_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}
// @verified - p
void QueueE0250Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0250LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *const loop = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
    loop->parent = parent;
    loop->state = 0;
    SetE0250SceneVisibility(parent->selection);
    if (parent->selection != 0) {
        EV_LoadCutsceneFromCurrentScene("0251A000");
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0250_LIGHT);
        PushGlobalLightingProgram();
    }
    SetupE0250Characters(parent->selection);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void E0250LoopCallback(void *parameter) {
    AreaImmediateAuthorFrameParameter *const loop = static_cast<AreaImmediateAuthorFrameParameter *>(parameter);
    AreaImmediateAuthorTaskParameter *const parent = loop->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 0) {
        Vector3 position{}, offset{0.0f, 0.41999999f, 0.0f};
        Vector3i rotation{};
        EV_GetCharaPosition(AREA_0250_AB07, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
        GetScenarioCharacterEulerAngles(AREA_0250_AB07, &rotation, EventTransformFlags::World);
        ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
        SetParticleEffectPosition(4, fire0250, &offset, &offset);
        if (frame == 2360) {
            position = {180.17999f, 111.77f, 152.56f};
            SetFirePosition(fire0250, 1, &position);
            position = {180.14999f, 111.77f, 153.34f};
            SetFirePosition(fire0250, 2, &position);
        }
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    if (parent->selection != 0)
        parent->state = 3;
    else {
        SetFirePosition(fire0250, 1, nullptr);
        SetFirePosition(fire0250, 2, nullptr);
        SuspendParticleEffect(4, fire0250);
        FreeParticleEffect(4, fire0250);
        parent->state = 1;
        ++parent->selection;
        HLib::SetupTimedTask(parent->self, 30);
    }
    RestoreE0250Characters(parent->selection);
    HLib::DestroyCurrentTask();
}
// @verified - p
void CleanupE0250(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0250_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_FreeAuthorPak(0, "0250");
    EV_FreeAuthorPak(0, "0251");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0250_NEXT, 0);
}
// @verified - p
void E0250MainCallback(void *parameter) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(parameter);
    if (parent->state == 1)
        QueueE0250Loop(parent);
    else if (parent->state == 3)
        CleanupE0250(parent);
}
} // namespace
const ScriptAreaDefinition area0250ScriptDefinition{AREA_0250_AREA, 0, E0250Init, E0250Main};
} // namespace shenmue::s2::areas
