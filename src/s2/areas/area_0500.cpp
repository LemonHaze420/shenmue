#include "s2/areas/area_0500.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/author.hpp"
#include "s2/author_viewer_event.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/help.hpp"
#include "s2/hiki.hpp"
#include "s2/lighting.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/memo.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/renderer_initialization.hpp"
#include "s2/screen_film.hpp"
#include "s2/ship_spray.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
void Prepare0500Character(shen_name character, bool motion) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Disabled);
    if (motion)
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 2);
    EV_SetCharacterAlphaEnabled(character, false);
}
void Restore0500Character(shen_name character, bool motion) {
    EV_EnableCharacterDisplay(character);
    EV_SetCharaControllerFlags(character, 1, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, 2, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(character, 3, NpcCharacterControllerState::Enabled);
    if (motion)
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 0);
    EV_SetCharacterAlphaEnabled(character, true);
}
} // namespace
void Initialize0500() {
}
void Start0500(std::uintptr_t) {
    shenSetClockAndCompassRenderTo(1);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0500, 4, 11, sizeof(Area0500Parameter), AREA_0500_EVENT);
    Area0500Parameter *const parent = static_cast<Area0500Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    if (g_currentMap.name != AREA_0500_MAP.name)
        parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0500A.MOT"));
    parent->selection = 0;
    parent->state = 0;
    InitializeShipSpray();
    HIKI_Enqueue();
    Setup0500Environment();
}
void Setup0500Environment() {
    DateTime date{};
    GetCurrentDateTime(date);
    date.hour = 8;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    SetNewWindPhaseAndSpeed(4096, 0.2f);
    SetWeatherUpdateEnabledTo(0);
    SetIsSkySetup(0);
    EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1437, 1);
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    SetPlayerControlEnabled(0);
    EV_SetObjectFlagByIndex(5, 0);
    EV_SetObjectFlagByIndex(6, 0);
    EV_DisableCharacterDisplay(AREA_0500_BOAT);
    EV_DisableCharacterDisplay(AREA_0500_PROP);
    EV_DisableCharacterDisplay(AREA_0500_RYO);
    EV_ReinitLights(1);
    EV_SetLightFlagForIndex(0, 2);
    EV_SetIndexForLightEntry(0, 1);
    EV_FreeAuthorPak(1, "0500");
    SetMapModelCommandRoutingEnabled(0, 0);
    HLib::EnqueueTaskWithoutParameter(Update0500Waves, 3, 2, static_cast<std::int32_t>(AREA_0500_WAVE.name));
}
void Update0500(void *data) {
    Area0500Parameter *const parent = static_cast<Area0500Parameter *>(data);
    switch (parent->state) {
    case 0:
        WaitForNextRenderFrame(nullptr, 0);
        SetD3TRenderSettings(1);
        parent->state = 1;
        break;
    case 1:
        if (IsItemSelectable())
            parent->state = 2;
        else {
            SetScreenFilmFade(15, 255, 0, 0, 0, 0, 0, 0, 0);
            RefreshPadTask(2, 1);
            EV_EventSetUpHelpIcon_0(0, 0);
        }
        break;
    case 2:
        FinishLoading();
        if (!IsItemSelectable()) {
            parent->state = 3;
            Initialize0500();
            ProcessAicaCommand(0xaa0, 30);
            SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
            HLib::SetupTaskDelay(30);
        }
        break;
    case 3:
        parent->state = 4;
        SetAreaNpcLowQualityHands(false);
        Queue_EVT_UTL_LoadNPCChara_3(AREA_0500_CHARACTERS, parent->self);
        break;
    case 4:
        Queue0500Author(parent);
        break;
    case 6:
        Cleanup0500(parent);
        break;
    }
}
void Queue0500Author(Area0500Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0500Author, 4, 11, sizeof(Area0500AuthorParameter), AREA_0500_EVENT);
    Area0500AuthorParameter *const child = static_cast<Area0500AuthorParameter *>(HLib::GetTaskParameter(task));
    child->frame = 0;
    child->parent = parent;
    EV_LoadCutsceneFromCurrentScene("0500A000");
    EV_SetObjectFlagByIndex(5, 1);
    EV_SetObjectFlagByIndex(6, 1);
    SetWeatherUpdateEnabledTo(1);
    SetIsSkySetup(1);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_spmBeginSampleExecute();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 5;
}
void Update0500Author(void *data) {
    Area0500AuthorParameter *const child = static_cast<Area0500AuthorParameter *>(data);
    Vector3 position{};
    Vector3i rotation{};
    EV_GetCharaPosition(AREA_0500_BOAT, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::Local);
    GetScenarioCharacterEulerAngles(AREA_0500_BOAT, &rotation, EventTransformFlags::Local);
    rotation.y += 0x8000;
    AssignShipSprayLocalPosition(Vector3{0.0f, 0.0f, 13.5f});
    AssignHikiLocalPosition(Vector3{0.0f, 0.0f, 2.0f});
    position.y = 0.2f;
    AssignShipSprayPosition(position);
    AssignShipSprayYaw(rotation.y);
    AssignHikiPosition(position);
    AssignHikiRotation(rotation.y);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (child->frame) {
    case 0:
        Prepare0500Characters();
        camCtrlSetFarDistance(517.0f);
        camCtrlSetNearDistance(517.0f);
        break;
    case 1141:
        EV_lightLoad(AREA_0500_LIGHT);
        SetIsSkySetup(0);
        SetClothFlag(FindCharacterByScenarioName(AREA_0500_SGT), 4, 1);
        camCtrlSetFarDistance(700.0f);
        camCtrlSetNearDistance(700.0f);
        break;
    case 1650:
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
        SetIsSkySetup(1);
        SetClothFlag(FindCharacterByScenarioName(AREA_0500_SGT), 4, 0);
        break;
    }
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Restore0500Characters();
        child->parent->state = 6;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void Update0500Waves(void *) {
    SetTextureAnimationUV(0.0f, 0.008f);
    ApplyTextureAnimationResource(1);
}
void Prepare0500Characters() {
    Prepare0500Character(AREA_0500_RYO, true);
    Prepare0500Character(AREA_0500_BOAT, false);
    Prepare0500Character(AREA_0500_PROP, false);
    for (const shen_name character : AREA_0500_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Prepare0500Character(character, true);
    }
}
void Restore0500Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_0500_RYO);
    Restore0500Character(AREA_0500_BOAT, false);
    Restore0500Character(AREA_0500_PROP, false);
    for (const shen_name character : AREA_0500_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        Restore0500Character(character, true);
    }
}
void Cleanup0500(Area0500Parameter *parent) {
    for (const shen_name character : AREA_0500_CHARACTERS) {
        if (character.name == 0xffffffffU)
            break;
        if (DoesScenarioCharacterExist(character)) {
            EV_npcLockUnlock(character);
            EV_npcFree(character);
        }
    }
    EV_FreeAuthorPak(0, "0500");
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    DateTime date{};
    GetCurrentDateTime(date);
    date.hour = 8;
    date.minute = 30;
    SetCurrentDateTimeTo(date);
    EV_scenQueueLoad(
        g_actNumber, g_currentMap.name == AREA_0500_MAP.name ? AREA_0500_RETURN : g_currentMap, g_currentMap.name == AREA_0500_MAP.name ? 0 : g_entryPoint);
}
const ScriptAreaDefinition area0500ScriptDefinition{AREA_0500, 0, Initialize0500, Start0500};
} // namespace shenmue::s2::areas
