#include "s2/areas/area_0404.hpp"
#include "s2/author.hpp"
#include "s2/areas/numeric_event.hpp"
#include "s2/character.hpp"
#include "s2/date_time.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/load_title.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {
namespace {
std::array<float, 64> fftInput0404 = [] {
    std::array<float, 64> value{};
    value[59] = -0.02160000056028366f;
    value[60] = -0.045099999755620956f;
    value[61] = -0.10199999809265137f;
    value[62] = -0.20200000703334808f;
    value[63] = -0.45100000500679016f;
    return value;
}();
} // namespace
void Initialize0404() {
}
bool Is0404ArgumentZero(std::int32_t argument) {
    return argument == 0;
}
void Start0404(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Load0404Characters, 4, 0x0b, sizeof(Area0404Parameter), AREA_0404_TASK);
    Area0404Parameter *const parent = static_cast<Area0404Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0404A.MOT"));
    parent->selection = 0;
    parent->state = 1;
    parent->loadState = 0;
    parent->characterIndex = 0;
    Setup0404Environment();
}
void Load0404Characters(void *parameter) {
    Area0404Parameter *const parent = static_cast<Area0404Parameter *>(parameter);
    const shen_name name = AREA_0404_CHARACTERS[parent->characterIndex];
    if (parent->loadState == 0) {
        EV_CheckWarmStartNpcRoute(name);
        EV_ForceLoadNpc(name);
        parent->loadState = 1;
    } else if (parent->loadState == 1 && static_cast<std::uint32_t>(EV_ForceLoadNpc(name)) == 1) {
        ++parent->characterIndex;
        parent->loadState = 0;
        if (AREA_0404_CHARACTERS[parent->characterIndex].name == 0xffffffffU) {
            parent->loadState = 0;
            parent->characterIndex = 0;
            EV_NPC_NewLoadSw(1);
            HLib::SetCurrentTaskTo(Update0404);
        }
    }
}
void Exit0404Characters(void *parameter) {
    Area0404Parameter *const parent = static_cast<Area0404Parameter *>(parameter);
    const shen_name name = AREA_0404_CHARACTERS[parent->characterIndex];
    if (parent->loadState == 0) {
        if (static_cast<std::uint32_t>(EV_npcLockUnlock(name)) == 0)
            EV_npcFree(name);
        parent->loadState = 1;
    } else if (parent->loadState == 1 && static_cast<std::uint32_t>(IsNpcLoadedOrVisible(name)) == 0) {
        ++parent->characterIndex;
        parent->loadState = 0;
        if (AREA_0404_CHARACTERS[parent->characterIndex].name == 0xffffffffU)
            HLib::SetCurrentTaskTo(Update0404);
    }
}
void Update0404(void *parameter) {
    Area0404Parameter *const parent = static_cast<Area0404Parameter *>(parameter);
    if (parent->state == 1)
        Queue0404Author(parent);
    else if (parent->state == 3) {
        HLib::SetCurrentTaskTo(Exit0404Characters);
        parent->state = 4;
    } else if (parent->state == 4)
        Cleanup0404(parent);
}
void Setup0404Environment() {
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 20;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    SetPlayerControlEnabled(0);
    SetMapModelCommandRoutingEnabled(0, 2);
    EV_SetWeatherInstruction(15, 0);
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    Is0404ArgumentZero(1);
    EV_FreeAuthorPak(1, "0404");
    std::array<std::byte, 0x100> distance;
    GenerateFFT(1.25f);
    FillFftDistance(FillFftKernel(184.0f, GenerateDistanceFalloffRowMajor(2.0f, distance)));
    ApplyFft(fftInput0404.data(), fftInput0404.data());
    FinalizeFft(0.01489999983459711f, 0.0f);
    SetWaveLighting(0.1f, Vector3{0.0f, 0.871999979019165f, -0.48899999260902405f});
    HLib::EnqueueTaskWithoutParameter(Update0404Waves, 3, 2, static_cast<std::int32_t>(AREA_0404_TASK.name));
}
void Update0404Waves(void *) {
    BuildWaveLightingTable(1, 1.0f, 1.0f, Vector3{1.0f, 1.0f, 1.0f}, Vector3{1.0f, 1.0f, 1.0f}, 10.0f, 1.0f);
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(1);
}
void Queue0404Author(Area0404Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0404Author, 4, 0x0b, sizeof(Area0404AuthorParameter), AREA_0404_TASK);
    Area0404AuthorParameter *const child = static_cast<Area0404AuthorParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    EV_LoadAudio("0404A000", 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
void Update0404Author(void *parameter) {
    Area0404AuthorParameter *const child = static_cast<Area0404AuthorParameter *>(parameter);
    child->frame = static_cast<std::int16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->frame == 0) {
        Prepare0404Character(AREA_0404_RYO);
        Prepare0404Character(AREA_0404_SIN);
        EV_EnableCharacterDisplay(AREA_0404_PROP);
        EV_SetCharaControllerFlags(AREA_0404_PROP, UpdateHeight, NpcCharacterControllerState::Disabled);
        EV_SetCharaControllerFlags(AREA_0404_PROP, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
        EV_SetCharacterAlphaEnabled(AREA_0404_PROP, false);
    }
    if (static_cast<std::uint32_t>(EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) == 0) {
        Restore0404Characters();
        child->parent->state = 3;
        HLib::SetupTimedTask(child->parent->self, 30);
        SetScreenFilmFade(30, 0, 0, 0, 0, 255, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}
void Prepare0404Character(shen_name name) {
    const Vector4 reflector{1.0f, 0.3f, 0.3f, 0.3f};
    EV_EnableCharacterDisplay(name);
    EV_SetCharaControllerFlags(name, UpdateHeight, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(name, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(name), 6, 2);
    EV_SetCharacterAlphaEnabled(name, false);
    SetScenarioReflectorBoardValues(name, &reflector);
    EV_SetRefBoardParamsFor(name, 1);
    hmdlSetFlagsFor(FindCharacterByScenarioName(name), 0, 8, 16);
    hmdlSetFlagsFor(FindCharacterByScenarioName(name), 1, 8, 16);
}
void Restore0404Characters() {
    EnableCharDisplayAndSetMotionworkData(AREA_0404_RYO);
    EnableCharDisplayAndSetMotionworkData(AREA_0404_SIN);
    EV_EnableCharacterDisplay(AREA_0404_PROP);
    EV_SetCharaControllerFlags(AREA_0404_PROP, UpdateHeight, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0404_PROP, ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharaControllerFlags(AREA_0404_PROP, UpdateHeight | ApplyGravity, NpcCharacterControllerState::Enabled);
    EV_SetCharacterAlphaEnabled(AREA_0404_PROP, true);
}
void Cleanup0404(Area0404Parameter *parent) {
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 9;
    time.minute = 0;
    SetCurrentDateTimeTo(time);
    EV_FreeAuthorPak(0, "0404");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    scenSetTaskFlags(9);
    EV_scenQueueLoad(g_actNumber, AREA_0404_RETURN_MAP, 1);
}
const ScriptAreaDefinition area0404ScriptDefinition{.areaId = AREA_0404, .field04 = 0, .init = Initialize0404, .main = Start0404};
} // namespace shenmue::s2::areas
