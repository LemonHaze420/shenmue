#include "s2/areas/area_0276.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hand.hpp"
#include "s2/inventory.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/pak.hpp"
#include "s2/particle_effects.hpp"
#include "s2/player.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {
std::uint32_t fire0276;
float transparency0276;
constexpr std::uint16_t firePalette0276[]{
    0x0311, 0x0311, 0x0411, 0x0511, 0x0612, 0x0712, 0x0a32, 0x0b32, 0x0b42, 0x0c50, 0x0d70, 0x0e70, 0x0e80, 0x0fa0, 0x0fc3, 0x0fd6};
void Set0276Props(const shen_name *props, std::size_t count, bool enabled) {
    for (std::size_t index = 0; index < count; ++index)
        EV_SetCharacterAlphaEnabled(props[index], enabled);
}
void Follow0276Fire() {
    Vector3 position{};
    Vector3i rotation{};
    Vector3 offset{0.0f, 0.14f, 0.0f};
    EV_GetCharaPosition(AREA_0276_FIRE_PROP, 0, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0276_FIRE_PROP, &rotation, EventTransformFlags::World);
    ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
    SetParticleEffectPosition(4, fire0276, &offset, &offset);
}
void Fade0276Out() {
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
}
} // namespace
void Initialize0276() {
    fire0276 = 0;
    transparency0276 = 0.0f;
}
void Start0276(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Load0276Characters, 4, 11, sizeof(Area0276Parameter), AREA_0276_EVENT);
    Area0276Parameter *const parent = static_cast<Area0276Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 6;
    parent->loadState = 0;
    parent->characterIndex = 0;
    Setup0276Environment();
}
void Load0276Characters(void *data) {
    Area0276Parameter *const parent = static_cast<Area0276Parameter *>(data);
    const shen_name character = AREA_0276_CHARACTERS[parent->characterIndex];
    if (parent->loadState == 0) {
        EV_CheckWarmStartNpcRoute(character);
        EV_ForceLoadNpc(character);
        parent->loadState = 1;
    } else if (parent->loadState == 1 && EV_ForceLoadNpc(character) == 1) {
        SetScenarioCharacterRadius(character, 40.0f);
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
        SetLowQualityHands(FindCharacterByScenarioName(character));
        ++parent->characterIndex;
        parent->loadState = 0;
        if (AREA_0276_CHARACTERS[parent->characterIndex].name == 0xffffffffU) {
            parent->characterIndex = 0;
            HLib::SetCurrentTaskTo(Update0276);
        }
    }
}
void Setup0276Environment() {
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    fire0276 = static_cast<std::uint32_t>(CreateFireEffect(Vector3{106.46f, 90.1f, 152.23f}, 0.5f, 0.5f, 0, 0));
    SetFirePalette(fire0276, firePalette0276);
    SetFireFalloffScalar(fire0276, -0.05f);
    SetFireFalloff(fire0276, 3.14f, 4.94f, 0.78f);
    SetParticleEffectSize(4, fire0276, 0.025f, 0.05f);
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0276");
    EV_FreeAuthorPak(1, "0277");
    EV_SetObjectFlagByIndex(0, 0);
    EV_SetObjectFlagByIndex(1, 1);
    EV_SetObjectFlagByIndex(2, 0);
    EV_SetObjectFlagByIndex(3, 0);
    SetAllMapFieldDepthSlotsEnabled(0);
    Prepare0276Characters();
}
void Prepare0276Characters() {
    SetScenarioCharacterRadius(AREA_0276_RYO, 40.0f);
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(AREA_0276_RYO)), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0276_RYO, false);
    Set0276Props(AREA_0276_INITIAL_PROPS, std::size(AREA_0276_INITIAL_PROPS), false);
}
void Restore0276Characters() {
    mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(AREA_0276_RYO)), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0276_RYO, true);
    Set0276Props(AREA_0276_INITIAL_PROPS, std::size(AREA_0276_INITIAL_PROPS), true);
}
void Prepare0276FirstScene() {
    Set0276Props(AREA_0276_FIRST_PROPS, std::size(AREA_0276_FIRST_PROPS), false);
    SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, 0.0f);
}
void Restore0276FirstScene() {
    Set0276Props(AREA_0276_FIRST_PROPS, std::size(AREA_0276_FIRST_PROPS), true);
}
void Update0276(void *data) {
    Area0276Parameter *const parent = static_cast<Area0276Parameter *>(data);
    switch (parent->state) {
    case 1:
        Queue0276Author(parent);
        break;
    case 3:
        Cleanup0276(parent);
        break;
    case 6:
        for (const char *package : {"0276A", "0277A"}) {
            if (EV_ControlPackageResources(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
                EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
        }
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(parent->selection > 2 ? "0277A" : "0276A"));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
        parent->state = 1;
        break;
    }
}
void Queue0276Author(Area0276Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(
        parent->selection > 2 ? Update0276SecondAuthor : Update0276FirstAuthor, 4, 11, sizeof(Area0276AuthorParameter), AREA_0276_EVENT);
    Area0276AuthorParameter *const child = static_cast<Area0276AuthorParameter *>(HLib::GetTaskParameter(task));
    child->frame = 0;
    child->parent = parent;
    if (parent->selection == 0) {
        EV_LoadCutsceneFromCurrentScene("0276A000");
        Prepare0276FirstScene();
        SetCharacterOsageMode(FindCharacterByScenarioName(AREA_0276_REN), 4);
    } else if (parent->selection == 3) {
        EV_LoadCutsceneFromCurrentScene("0277A000");
        SetIsSkySetup(0);
        SetAllMapFieldDepthSlotsEnabled(1);
        EV_SetObjectFlagByIndex(0, 1);
        EV_SetObjectFlagByIndex(1, 0);
        EV_SetObjectFlagByIndex(2, 1);
        EV_SetObjectFlagByIndex(3, 1);
        EV_SetModelDataDefaultVertexShader();
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        Set0276Props(AREA_0276_INDOOR_PROPS, std::size(AREA_0276_INDOOR_PROPS), false);
    } else if (parent->selection == 5) {
        EV_SetObjectFlagByIndex(0, 0);
        EV_SetObjectFlagByIndex(1, 1);
        EV_SetObjectFlagByIndex(2, 0);
        EV_SetObjectFlagByIndex(3, 0);
        SetAllMapFieldDepthSlotsEnabled(0);
        EV_SetWeatherInstruction(12, 0);
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(AREA_0276_LIGHT);
        PushGlobalLightingProgram();
    }
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
void Update0276FirstAuthor(void *data) {
    Area0276AuthorParameter *const child = static_cast<Area0276AuthorParameter *>(data);
    Area0276Parameter *const parent = child->parent;
    const std::int16_t frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    child->frame = frame;
    if (parent->selection == 0) {
        if (frame == 1)
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        else if (frame == 876 || frame == 1621)
            snapSetDesc(1);
        else if (frame == 964 || frame == 1689)
            snapSetDesc(0);
        else if (frame == 1156 || frame == 1451) {
            const std::int32_t enabled = frame == 1451;
            EV_SetObjectFlagByIndex(1, enabled);
            SetIsSkySetup(enabled);
        }
    } else if (parent->selection == 1) {
        if (frame == 2630)
            ResumeParticleEffect(4, fire0276);
        else if (frame > 2630)
            Follow0276Fire();
    } else if (parent->selection == 2) {
        if (frame == 1) {
            ResumeParticleEffect(4, fire0276);
            SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, 0.0f);
        } else if (frame >= 390 && frame < 450) {
            transparency0276 += 0.016000001f;
            SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (frame == 450) {
            transparency0276 = 1.0f;
            SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (frame >= 720 && frame < 780) {
            transparency0276 -= 0.016000001f;
            SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (frame == 780) {
            transparency0276 = 0.0f;
            SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        }
        Follow0276Fire();
    }
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded))
        return;
    if (parent->selection == 2) {
        Fade0276Out();
        Restore0276FirstScene();
        parent->selection = 3;
        parent->state = 6;
        EV_npcFree(AREA_0276_SRY);
        EV_npcFree(AREA_0276_SGT);
        EV_npcFree(AREA_0276_CHO);
        SuspendParticleEffect(4, fire0276);
        FreeParticleEffect(4, fire0276);
        HLib::DestroyCurrentTask();
    } else {
        ++parent->selection;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    }
}
void Update0276SecondAuthor(void *data) {
    Area0276AuthorParameter *const child = static_cast<Area0276AuthorParameter *>(data);
    Area0276Parameter *const parent = child->parent;
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (parent->selection == 0) {
        Vector3 position{};
        EV_GetCharaPosition(AREA_0276_RYO, 0, &position, EventTransformFlags::World);
        position.y = 500.0f;
        position.y = shenFindIntersectionWithScene(position, 500.0f);
        EV_AT_SetPos(AREA_0276_RYO, EventTransformFlags::AllAxes, position);
        if (frame == 1)
            SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        else if (frame == 2180)
            SetNewWindPhaseAndSpeed(60334, 1.8200001f);
        else if (frame == 2290)
            SyncWindWithWDATWeatherState();
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        if (parent->selection == 3) {
            SyncWindWithWDATWeatherState();
            ++parent->selection;
            EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        } else if (parent->selection == 4) {
            ++parent->selection;
            parent->state = 1;
            Fade0276Out();
            HLib::DestroyCurrentTask();
        } else if (parent->selection == 5) {
            parent->state = 3;
            Fade0276Out();
            HLib::DestroyCurrentTask();
            Set0276Props(AREA_0276_INDOOR_PROPS, std::size(AREA_0276_INDOOR_PROPS), true);
        }
    }
    if (parent->selection == 3) {
        if (frame == 1)
            snapSetDesc(1);
        else if (frame == 535)
            snapSetDesc(0);
    }
}
void Cleanup0276(Area0276Parameter *parent) {
    Restore0276Characters();
    AddInventoryItem(11, 1, 0);
    AddInventoryItem(12, 1, 0);
    DateTime date{};
    GetCurrentDateTime(date);
    date.hour = 22;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    EV_FreeAuthorPak(0, "0276");
    EV_FreeAuthorPak(0, "0277");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, EV_GetShenmueFlag(0x434) ? AREA_0276_RETURN_FLAGGED : AREA_0276_RETURN_CLEAR, 0);
}
const ScriptAreaDefinition area0276ScriptDefinition{AREA_0276, 0, Initialize0276, Start0276};
} // namespace shenmue::s2::areas
