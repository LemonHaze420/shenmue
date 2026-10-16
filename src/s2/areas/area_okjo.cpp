#include "s2/areas/area_okjo.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/flag.hpp"
#include "s2/handcuffs.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/snapshot.hpp"
#include "s2/sound_program.hpp"
#include "s2/scene_data.hpp"
#include "s2/ui.hpp"
#include "s2/world_render.hpp"

namespace shenmue::s2::areas {
namespace {
struct OkjoAuthorScene {
    shen_name author, lighting;
    const char *audio;
    const char *authorPackage;
    const char *loadPackage;
    const char *releasePackage;
};
static_assert(sizeof(OkjoAuthorScene) == 40);
const OkjoAuthorScene authorScenes[5]{{OKJO_AUTHORS[0], OKJO_LIGHTS[0], "0214A000", "0214", nullptr, nullptr},
    {OKJO_AUTHORS[1], OKJO_LIGHTS[1], "0215A000", "0215", "OKJO1", "OKJO0"}, {OKJO_AUTHORS[2], OKJO_LIGHTS[2], "0216A000", "0216", "OKJO2", "OKJO1"},
    {OKJO_AUTHORS[3], OKJO_LIGHTS[3], nullptr, nullptr, nullptr, nullptr}, {OKJO_INVALID, OKJO_INVALID, nullptr, "0216", nullptr, "OKJO2"}};
const shen_name characters[2]{OKJO_REN, OKJO_INVALID};
const OkjoAuthorScene *currentScene;
void SetOkjoScreenFilm() {
    SetScreenFilmFade(0, 255, 0, 0, 0, 255, 0, 0, 0);
}
} // namespace
// @verified - p
void AreaOKJOInit() {
    currentScene = nullptr;
}
// @verified - p
void AreaOKJOMain(std::uintptr_t) {
    SetOkjoScreenFilm();
    SetPlayerControlEnabled(0);
    mtwkRequestMotion(scenFindMotionwork(OKJO_PLAYER), 0xf003, MotionRegion::LowerBody);
    SetIsSkySetup(1);
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 21;
    date.minute = 0;
    SetCurrentDateTimeTo(date);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    EV_SetWeatherInstruction(0x0c, 0);
    EV_FreeAuthorPak(1, "0214");
    EV_FreeAuthorPak(1, "0215");
    EV_FreeAuthorPak(1, "0216");
    currentScene = authorScenes;
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(OKJOPlayAuthorCallback, 6, 0x0b, static_cast<std::int32_t>(OKJO_TASK.name));
    AssignAreaNpcLoadContext(characters, task);
    g_usingCustomAuthorCharacterList = 0;
    EV_NPC_NewLoadSw(0);
    EV_ResetFreeTalkFlags();
    if (task)
        HLib::SetupTimedTask(task, 0);
    HLib::EnqueueTaskWithoutParameter(EVT_UTL_LoadNPCChara_Callback_2, 6, 0x0b, static_cast<std::int32_t>(OKJO_LOAD_TASK.name));
}
// @verified - p
void OKJOSetupCharacters() {
    const Vector4 reflector{1, .30000001f, .30000001f, .30000001f};
    EV_EnableCharacterDisplay(OKJO_PLAYER);
    EV_SetCharaControllerFlags(OKJO_PLAYER, ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(OKJO_PLAYER, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    EV_SetCharacterAlphaEnabled(OKJO_PLAYER, false);
    EV_SetRefBoardParamsFor(OKJO_PLAYER, 1);
    SetReflectorBoardValues(FindCharacterByScenarioName(OKJO_PLAYER), reflector);
    SetReflectorBoardValues(FindCharacterByScenarioName(OKJO_REN), reflector);
    charaSetDisplayEnabledFlag(FindCharacterByScenarioName(OKJO_PLAYER), 1);
    charaSetDisplayEnabledFlag(FindCharacterByScenarioName(OKJO_REN), 1);
    Handcuffs_Attach(OKJO_PLAYER, 1, OKJO_REN, 0, .30000001f);
    for (std::int32_t i = 0; i != 6; ++i)
        EV_SetObjectFlagByIndex(i, i & 1);
}
// @verified - p
void OKJOStartAuthor() {
    if (currentScene->releasePackage)
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(currentScene->releasePackage), 0);
    if (currentScene->loadPackage) {
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Load, reinterpret_cast<std::uintptr_t>(Filepath_Gen(FilepathGenMode::SceneDirectory)),
            reinterpret_cast<std::uintptr_t>(currentScene->loadPackage));
        EV_ControlPackageResources(PakCategory::Package, PakPackageOp::FinishLoading, 0, 0);
    }
    if (currentScene->audio)
        EV_LoadCutsceneFromCurrentScene(currentScene->audio);
    if (currentScene->lighting.name != 0) {
        EV_SetModelDataDefaultVertexShader();
        EV_lightLoad(currentScene->lighting);
        PushGlobalLightingProgram();
    }
    std::uint32_t enabled = 0;
    if (currentScene->author.name == OKJO_AUTHORS[3].name) {
        for (std::int32_t i = 0; i != 6; ++i)
            EV_SetObjectFlagByIndex(i, (i & 1) == 0);
        enabled = 1;
    }
    SetMapFieldDepthSlotEnabled(0, enabled);
    if (currentScene->author.name == OKJO_AUTHORS[1].name) {
        const Vector3 scale{50, 50, 50};
        SetMotionworkFlagForCharaById(OKJO_SCALED_PROP, &scale);
    }
    if (currentScene->author.name == OKJO_AUTHORS[2].name) {
        SetOkjoScreenFilm();
        DateTime date;
        GetCurrentDateTime(date);
        ++date.day;
        date.hour = 7;
        date.minute = 0;
        SetCurrentDateTimeTo(date);
        HLib::SetCurrentTaskTo(OKJOWaitDisplayCallback);
        Queue_ST_LoadTitleFadeIn(OKJO_NEXT_DAY);
        HLib::SetupTaskDelay(90);
    } else
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(currentScene->author.name));
}
// @verified - p
void OKJOPlayAuthorCallback(void *) {
    OKJOSetupCharacters();
    OKJOStartAuthor();
    HLib::SetCurrentTaskTo(OKJOWaitAuthorCallback);
}
// @verified - p
void OKJOWaitAuthorCallback(void *) {
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        SetOkjoScreenFilm();
        ++currentScene;
        if (currentScene->author.name == OKJO_INVALID.name)
            OKJOReleaseScene();
        else
            OKJOStartAuthor();
    }
    if (currentScene->author.name == OKJO_AUTHORS[2].name) {
        const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
        if (frame == 1) {
            snapSetDesc(1);
            SetScenarioCharacterType(OKJO_PLAYER, CharacterTypeDefault);
            SetScenarioCharacterType(OKJO_REN, CharacterTypeDefault);
        } else if (frame == 250)
            snapSetDesc(0);
    }
    if (currentScene->author.name == OKJO_AUTHORS[3].name && EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame) == 1) {
        SetScenarioCharacterType(OKJO_PLAYER, CharacterTypeStencilShadow);
        SetScenarioCharacterType(OKJO_REN, CharacterTypeStencilShadow);
    }
}
// @verified - p
void OKJOWaitDisplayCallback(void *) {
    ST_LoadTitleFade();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(currentScene->author.name));
    HLib::SetCurrentTaskTo(OKJOWaitAuthorCallback);
}
// @verified - p
void OKJOReleaseScene() {
    EV_cuffsSetFlags();
    for (const shen_name *character = characters; character->name != OKJO_INVALID.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_ControlPackageResources(PakCategory::Package, PakPackageOp::Free, reinterpret_cast<std::uintptr_t>(currentScene->releasePackage), 0);
    EV_FreeAuthorPak(0, "0214");
    EV_FreeAuthorPak(0, "0215");
    EV_FreeAuthorPak(0, "0216");
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x0c, 0);
    EV_SetObjectFlagByIndex(1, 1);
    EV_SetObjectFlagByIndex(2, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    AssignLightProgram(0);
    AssignLightProgram(2);
    AssignLightProgram(3);
    AssignLightProgram(4);
    AssignLightProgram(5);
    EV_AssignLightParams(1, 1);
    HLib::DestroyCurrentTask();
    EV_stepSetFlags(0, 0x13b);
}
const ScriptAreaDefinition areaOKJOScriptDefinition{.areaId = AREA_OKJO, .field04 = 0, .init = AreaOKJOInit, .main = AreaOKJOMain};
} // namespace shenmue::s2::areas
