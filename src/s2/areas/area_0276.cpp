#if defined(SHENMUE_EVENT_OVERLAY)
#include "s2/areas/area_0276.hpp"

#define AREA_0276_CHARACTER(parent) area0276Characters[(parent)->characterIndex]
unsigned short firePalette0276[] = {
    0x311, 0x311, 0x411, 0x511, 0x612, 0x712, 0xa32, 0xb32, 0xb42, 0xc50, 0xd70, 0xe70, 0xe80, 0xfa0, 0xfc3, 0xfd6};
shen_name area0276Characters[] = {AREA_0276_SRY, AREA_0276_WON, AREA_0276_REN, AREA_0276_JOP, AREA_0276_CHO, AREA_0276_SGT, INVALID_CHARACTER};
shen_name area0276ReleaseCharacters[] = {AREA_0276_SRY, AREA_0276_WON, AREA_0276_REN, AREA_0276_JOP, AREA_0276_CHO, AREA_0276_SGT, INVALID_CHARACTER};
shen_name area0276RemainingCharacters[] = {AREA_0276_WON, AREA_0276_REN, AREA_0276_JOP, INVALID_CHARACTER};
float transparency0276 = 0.0f;
static unsigned int fire0276;

void Start0276(void)
{
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(Load0276Characters, 4, 11, sizeof(Area0276Parameter), TASK_EVNT);
    Area0276Parameter *parent = (Area0276Parameter *)FTBL.HLib_GetTaskParameter(task);
    parent->self = task;
    parent->state = 6;
    parent->selection = 0;
    parent->loadState = 0;
    parent->characterIndex = 0;
    Setup0276Environment();
}

void Update0276(void *data)
{
    Area0276Parameter *parent = (Area0276Parameter *)data;
    switch (parent->state) {
    case 6:
        if (FTBL.EV_ControlPackageResources(0, 5, "0276A", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "0276A", 0, 0);
        if (FTBL.EV_ControlPackageResources(0, 5, "0277A", 0, 0))
            FTBL.EV_ControlPackageResources(0, 6, "0277A", 0, 0);
        if (parent->selection <= 2)
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0276A", 0);
        else
            FTBL.EV_ControlPackageResources(0, 0, FTBL.Filepath_Gen(0), (int)"0277A", 0);
        FTBL.EV_ControlPackageResources(0, 7, 0, 0, 0);
        parent->state = 1;
        break;
    case 1:
        Queue0276Author(parent);
        break;
    case 3:
        Cleanup0276(parent);
        break;
    }
}

void Load0276Characters(void *data)
{
    Area0276Parameter *parent = (Area0276Parameter *)data;
    if (parent->loadState == 1) {
        FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", parent->characterIndex, AREA_0276_CHARACTER(parent) & 0xff,
            AREA_0276_CHARACTER(parent) >> 8 & 0xff, AREA_0276_CHARACTER(parent) >> 16 & 0xff, AREA_0276_CHARACTER(parent) >> 24);
        if (FTBL.ForceLoadNpc(AREA_0276_CHARACTER(parent)) == 1) {
            HideAuthorCharacter(AREA_0276_CHARACTER(parent));
            FTBL.SetLowQualityHands(FTBL.ctrlGetCharaByName(AREA_0276_CHARACTER(parent)));
            parent->characterIndex++;
            do {
                parent->loadState = 0;
                if (AREA_0276_CHARACTER(parent) == INVALID_CHARACTER) {
                    parent->characterIndex = 0;
                    FTBL.HLib_SetCurrentTaskTo(Update0276);
                }
            } while (0);
        }
    } else if (parent->loadState == 0) {
        FTBL.CheckWarmStartNpcRoute(AREA_0276_CHARACTER(parent));
        FTBL.ForceLoadNpc(AREA_0276_CHARACTER(parent));
        parent->loadState = 1;
    }
}

void Release0276Characters(void *data)
{
    Area0276Parameter *parent = (Area0276Parameter *)data;
    int visible;
    if (parent->loadState == 1) {
        FTBL.DebugLogToFileRemoved("%d ID : [%c%c%c%c]\n", parent->characterIndex, AREA_0276_CHARACTER(parent) & 0xff,
            AREA_0276_CHARACTER(parent) >> 8 & 0xff, AREA_0276_CHARACTER(parent) >> 16 & 0xff, AREA_0276_CHARACTER(parent) >> 24);
        visible = FTBL.IsNpcRouteLoadedOrVisible(AREA_0276_CHARACTER(parent));
        if (!visible) {
            ShowAuthorCharacter(AREA_0276_CHARACTER(parent));
            parent->characterIndex++;
            parent->loadState = 0;
            if (AREA_0276_CHARACTER(parent) == INVALID_CHARACTER)
                FTBL.HLib_SetCurrentTaskTo(Update0276);
        }
    } else if (parent->loadState == 0) {
        if (!FTBL.npcLockUnlock(AREA_0276_CHARACTER(parent)))
            FTBL.npcFree(AREA_0276_CHARACTER(parent));
        parent->loadState = 1;
    }
}

void Queue0276Author(Area0276Parameter *parent)
{
    HLibTaskCallback callbacks[2] = {Update0276FirstAuthor, Update0276SecondAuthor};
    int second = parent->selection > 2;
    HLibTask *task = FTBL.HLib_EnqueueTaskWithParameter(callbacks[second], 4, 11, sizeof(Area0276AuthorParameter), TASK_EVNT);
    Area0276AuthorParameter *child = (Area0276AuthorParameter *)FTBL.HLib_GetTaskParameter(task);
    child->parent = parent;
    child->frame = 0;
    if (parent->selection == 0) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0276A000");
        Prepare0276FirstScene();
        FTBL.OSAG_0(FTBL.ctrlGetCharaByName(AREA_0276_REN), 4);
    } else if (parent->selection == 3) {
        FTBL.EV_LoadCutsceneFromCurrentScene("0277A000");
        FTBL.SetIsSkySetup(0);
        FTBL.SetAllMapFieldDepthSlotsEnabled(1);
        FTBL.SetObjectFlagByIndex(0, 1);
        FTBL.SetObjectFlagByIndex(1, 0);
        FTBL.SetObjectFlagByIndex(2, 1);
        FTBL.SetObjectFlagByIndex(3, 1);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.ReinitializeLights(1);
        FTBL.SetLightFlagForIndex(0, 2);
        FTBL.SetLightEntryIndex(0, 1);
        Hide0276IndoorProps();
    } else if (parent->selection == 5) {
        FTBL.SetObjectFlagByIndex(0, 0);
        FTBL.SetObjectFlagByIndex(1, 1);
        FTBL.SetObjectFlagByIndex(2, 0);
        FTBL.SetObjectFlagByIndex(3, 0);
        FTBL.SetAllMapFieldDepthSlotsEnabled(0);
        FTBL.EV_SetWeatherInstruction(12, 0);
        FTBL.SetMapModelDataDefaultVertexShader();
        FTBL.EV_lightLoad(AREA_0276_LIGHT);
        FTBL.PushGlobalLightingProgram();
    }
    FTBL.EV_authSceneCtrl(parent->selection);
    parent->state = 2;
}

void Update0276FirstAuthor(void *data)
{
    Area0276AuthorParameter *child = (Area0276AuthorParameter *)data;
    Vector3 position;
    Vector3 offset = {0.0f, 0.14f, 0.0f};
    Vector3i rotation;
    child->frame = FTBL.EV_authSceneCtrl(-6);
    switch (child->parent->selection) {
    case 0:
        switch (child->frame) {
        case 1:
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 1156:
            FTBL.SetObjectFlagByIndex(1, 0);
            FTBL.SetIsSkySetup(0);
            break;
        case 1451:
            FTBL.SetObjectFlagByIndex(1, 1);
            FTBL.SetIsSkySetup(1);
            break;
        }
        break;
    case 1:
        if (child->frame == 2630)
            FTBL.ResumeParticleEffect(4, fire0276);
        else if (child->frame > 2630) {
            FTBL.GetCharaPosition(AREA_0276_FIRE_PROP, -1, &position, 0x40000000);
            FTBL.GetScenarioCharacterEulerAngles(AREA_0276_FIRE_PROP, &rotation, 0x40000000);
            FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
            FTBL.SetParticleEffectPosition(4, fire0276, &offset, &offset);
        }
        break;
    case 2:
        if (child->frame == 1) {
            FTBL.ResumeParticleEffect(4, fire0276);
            FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, 0.0f);
        } else if (child->frame >= 390 && child->frame < 450) {
            transparency0276 += 0.016f;
            FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (child->frame == 450) {
            transparency0276 = 1.0f;
            FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (child->frame >= 720 && child->frame < 780) {
            transparency0276 -= 0.016f;
            FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        } else if (child->frame == 780) {
            transparency0276 = 0.0f;
            FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, transparency0276);
        }
        FTBL.GetCharaPosition(AREA_0276_FIRE_PROP, -1, &position, 0x40000000);
        FTBL.GetScenarioCharacterEulerAngles(AREA_0276_FIRE_PROP, &rotation, 0x40000000);
        FTBL.ApplyScenarioTranslationAndRotation(&position, &rotation, &offset);
        FTBL.SetParticleEffectPosition(4, fire0276, &offset, &offset);
        break;
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        if (child->parent->selection == 2) {
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            Restore0276FirstScene();
            child->parent->selection = 3;
            child->parent->state = 6;
            FTBL.npcFree(AREA_0276_SRY);
            FTBL.npcFree(AREA_0276_SGT);
            FTBL.npcFree(AREA_0276_CHO);
            FTBL.SuspendParticleEffect(4, fire0276);
            FTBL.FreeParticleEffect(4, fire0276);
            FTBL.HLib_DestroyCurrentTask();
        } else {
            child->parent->selection++;
            FTBL.EV_authSceneCtrl(child->parent->selection);
        }
    }
}

void Update0276SecondAuthor(void *data)
{
    Area0276AuthorParameter *child = (Area0276AuthorParameter *)data;
    Vector3 position;
    short frame = FTBL.EV_authSceneCtrl(-6);
    if (!child->parent->selection) {
        FTBL.GetCharaPosition(RYO_CHARACTER, -1, &position, 0x40000000);
        position.y = 500.0f;
        position.y = FTBL.shenFindIntersectionWithScene(&position);
        FTBL.EV_AT_SetPos(RYO_CHARACTER, 0x38000000, &position);
        switch (frame) {
        case 1:
            FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
            break;
        case 2180:
            FTBL.SetNewWindPhaseAndSpeed(60334, 1.82f);
            break;
        case 2290:
            FTBL.SyncWindWithWDATWeatherState();
            break;
        }
    }
    if (!FTBL.EV_authSceneCtrl(-1)) {
        switch (child->parent->selection) {
        case 3:
            FTBL.SyncWindWithWDATWeatherState();
            child->parent->selection++;
            FTBL.EV_authSceneCtrl(child->parent->selection);
            break;
        case 4:
            child->parent->selection++;
            child->parent->state = 1;
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_DestroyCurrentTask();
            break;
        case 5:
            child->parent->state = 3;
            FTBL.SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
            FTBL.HLib_DestroyCurrentTask();
            Show0276IndoorProps();
            break;
        }
    }
}

void Setup0276Environment(void)
{
    Vector3 position = {106.46f, 90.1f, 152.23f};
    FTBL.FreeWAIR();
    FTBL.UpdateWeatherForMinute(0, 0, 0, 0.0f);
    FTBL.UpdateWeatherForMinute(1, 0, 0, 0.0f);
    FTBL.UpdateWeather();
    fire0276 = FTBL.CreateFireEffect(&position, 0.5f, 0.5f, 0, 0);
    FTBL.SetFirePalette(fire0276, firePalette0276);
    FTBL.SetFireFalloffScalar(fire0276, -0.05f);
    FTBL.SetFireFalloff(fire0276, 3.14f, 4.94f, 0.78f);
    FTBL.SetParticleEffectSize(4, fire0276, 0.025f, 0.05f);
    FTBL.SetPlayerControlEnabled(0);
    FTBL.EV_FreeAuthorPak(1, "0276");
    FTBL.EV_FreeAuthorPak(1, "0277");
    FTBL.SetObjectFlagByIndex(0, 0);
    FTBL.SetObjectFlagByIndex(1, 1);
    FTBL.SetObjectFlagByIndex(2, 0);
    FTBL.SetObjectFlagByIndex(3, 0);
    FTBL.SetAllMapFieldDepthSlotsEnabled(0);
    Prepare0276Characters();
}

void Cleanup0276(Area0276Parameter *parent)
{
    EventDateTime time;
    Restore0276Characters();
    FTBL.AddInventoryItem(11, 1, 0);
    FTBL.AddInventoryItem(12, 1, 0);
    FTBL.GetCurrentDateTime((DateTime *)&time);
    time.hour = 22;
    time.minute = 0;
    FTBL.Shenmue_SetCurrentDateTimeTo((DateTime *)&time);
    FTBL.EV_FreeAuthorPak(0, "0276");
    FTBL.EV_FreeAuthorPak(0, "0277");
    FTBL.HLib_DestroyTask(parent->self);
    if (FTBL.EV_GetShenmueFlag(1076))
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0276_RETURN_FLAGGED, 0);
    else
        FTBL.EV_scenQueueLoad(g_actNumber, AREA_0276_RETURN_CLEAR, 0);
}

void Prepare0276Characters(void)
{
    HideAuthorCharacter(RYO_CHARACTER);
    HideAuthorProp(AREA_0276_A209);
    HideAuthorProp(AREA_0276_A207);
    HideAuthorProp(AREA_0276_A208);
    HideAuthorProp(AREA_0276_A326);
    HideAuthorProp(AREA_0276_A270);
    HideAuthorProp(AREA_0276_A325);
}

void Restore0276Characters(void)
{
    ShowAuthorCharacter(RYO_CHARACTER);
    ShowAuthorProp(AREA_0276_A209);
    ShowAuthorProp(AREA_0276_A207);
    ShowAuthorProp(AREA_0276_A208);
    ShowAuthorProp(AREA_0276_A326);
    ShowAuthorProp(AREA_0276_A270);
    ShowAuthorProp(AREA_0276_A325);
}

void Prepare0276FirstScene(void)
{
    HideAuthorProp(AREA_0276_A300);
    HideAuthorProp(AREA_0276_A067);
    HideAuthorProp(AREA_0276_A393);
    HideAuthorProp(AREA_0276_A394);
    HideAuthorProp(AREA_0276_A395);
    HideAuthorProp(AREA_0276_FIRE_PROP);
    HideAuthorProp(AREA_0276_TRANSPARENT_PROP);
    FTBL.SetScenarioCharacterTransparency(AREA_0276_TRANSPARENT_PROP, 0.0f);
}

void Restore0276FirstScene(void)
{
    ShowAuthorProp(AREA_0276_A300);
    ShowAuthorProp(AREA_0276_A067);
    ShowAuthorProp(AREA_0276_A393);
    ShowAuthorProp(AREA_0276_A394);
    ShowAuthorProp(AREA_0276_A395);
    ShowAuthorProp(AREA_0276_FIRE_PROP);
    ShowAuthorProp(AREA_0276_TRANSPARENT_PROP);
}

void Hide0276IndoorProps(void)
{
    HideAuthorProp(AREA_0276_A342);
    HideAuthorProp(AREA_0276_A343);
    HideAuthorProp(AREA_0276_AB19);
    HideAuthorProp(AREA_0276_AB18);
    HideAuthorProp(AREA_0276_AA23);
}

void Show0276IndoorProps(void)
{
    ShowAuthorProp(AREA_0276_A342);
    ShowAuthorProp(AREA_0276_A343);
    ShowAuthorProp(AREA_0276_AB19);
    ShowAuthorProp(AREA_0276_AB18);
    ShowAuthorProp(AREA_0276_AA23);
}

void HideAuthorCharacter(shen_name character)
{
    FTBL.SetScenarioCharacterRadius(character, 40.0f);
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 2);
    FTBL.EV_SetCharacterAlphaEnabled(character, 0);
}

void ShowAuthorCharacter(shen_name character)
{
    FTBL.mtwkSetExtraFlag(FTBL.scenFindMotionwork(character), 6, 0);
    FTBL.EV_SetCharacterAlphaEnabled(character, 1);
}

void HideAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 0);
}

void ShowAuthorProp(shen_name prop)
{
    FTBL.EV_SetCharacterAlphaEnabled(prop, 1);
}
#else
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
    EV_GetCharaPosition(AREA_0276_FIRE_PROP, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0276_FIRE_PROP, &rotation, EventTransformFlags::World);
    scenApplyTransRot(&position, &rotation, &offset);
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
        mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(character)), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
        SetLowQualityHands(ctrlGetScenarioChara(character));
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
    mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(AREA_0276_RYO)), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0276_RYO, false);
    Set0276Props(AREA_0276_INITIAL_PROPS, std::size(AREA_0276_INITIAL_PROPS), false);
}
void Restore0276Characters() {
    mtwkSetExtraFlag(FindMotionWork(ctrlGetScenarioChara(AREA_0276_RYO)), 6, 0);
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
            if (EV_pakUtils(PakCategory::Package, PakPackageOp::IsLoaded, reinterpret_cast<std::uintptr_t>(package), 0))
                EV_pakUtils(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(package), 0);
        }
        EV_pakUtils(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(parent->selection > 2 ? "0277A" : "0276A"));
        EV_pakUtils(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
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
        SetCharacterOsageMode(ctrlGetScenarioChara(AREA_0276_REN), 4);
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
    shenGetCurrentDateTime(date);
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
#endif
