#include "s2/areas/area_0505.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"

#include "s2/areas/numeric_event.hpp"
#include "s2/date_time.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <array>
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0505() {
}

// @verified - p
void Start0505(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0505, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0505_MOTION));
    parent->selection = 0;
    parent->state = 1;
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0505_CHARACTERS, parent->self);
    Setup0505Waves();
}

// @verified - p
void Update0505(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0505AuthorTask, 4, 0x0b, sizeof(Area0505AuthorParameter), AUTHOR_VIEWER_TASK);
        Area0505AuthorParameter *const child = static_cast<Area0505AuthorParameter *>(HLib::GetTaskParameter(task));
        child->parent = parent;
        child->currentFrame = 0;
        EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
        parent->state = 2;
    } else if (parent->state == 3) {
        Cleanup0505(parent);
    }
}

// @verified - p
void Cleanup0505(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0505_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0505_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0505_RETURN, 0);
}

// @verified - p
std::int64_t Update0505Author(Area0505AuthorParameter *child) {
    child->currentFrame = static_cast<std::uint16_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->currentFrame == 0) {
        EV_FreeWAIR();
        EV_UpdateWeatherForMinute(0, 0, 160, 0.0f);
        EV_UpdateWeather();
        Prepare0505Characters();
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        EnableCharDisplayAndSetMotionworkData(AREA_0505_RYO_);
        EnableCharDisplayAndSetMotionworkData(AREA_0505_SIN_);
        EnableCharDisplayAndSetMotionworkData(AREA_0505_BN0_);
        child->parent->state = 3;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0505AuthorTask(void *taskData) {
    Update0505Author(static_cast<Area0505AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0505Characters() {
    EV_EnableCharacterDisplay(AREA_0505_RYO_);
    EV_SetCharaControllerFlags(AREA_0505_RYO_, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0505_RYO_, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0505_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0505_RYO_, false);
    EV_EnableCharacterDisplay(AREA_0505_SIN_);
    EV_SetCharaControllerFlags(AREA_0505_SIN_, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0505_SIN_, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0505_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0505_SIN_, false);
    EV_EnableCharacterDisplay(AREA_0505_BN0_);
    EV_SetCharaControllerFlags(AREA_0505_BN0_, 1, NpcCharacterControllerState::Disabled);
    EV_SetCharaControllerFlags(AREA_0505_BN0_, 3, NpcCharacterControllerState::Disabled);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0505_BN0_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0505_BN0_, false);
}

// @verified - p
void Setup0505Waves() {
    DateTime date;
    GetCurrentDateTime(date);
    date.hour = 14;
    date.minute = 30;
    SetCurrentDateTimeTo(date);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(0, 0, 0, 0.5f);
    EV_UpdateWeather();
    SetPlayerControlEnabled(0);
    SetPlayerGameFlags(PlayerGameFlagMode::Bit5, nullptr);
    EV_FreeAuthorPak(1, AREA_0505_CONVERSATION);
    std::array<std::byte, 0x100> distance, first, second;
    GenerateFFT(0.5f);
    FillFftDistance(FillFftKernel(3.9200000762939453f, GenerateDistanceFalloffRowMajor(1.2899999618530273f, distance)));
    void *const firstKernel = FillFftKernel(0.0729999989271164f, GenerateDistanceFalloffKernel8x8(0.5640000104904175f, first));
    void *const secondKernel = FillFftKernel(0.05999999865889549f, GenerateDistanceFalloffKernel8x8(1.034999966621399f, second));
    ApplyFft(secondKernel, firstKernel);
    FinalizeFft(0.0f, 0.004699999932199717f);
    SetWaveLighting(0.10000000149011612f, {0.30000001192092896f, 0.800000011920929f, 0.5f});
    HLib::EnqueueTaskWithoutParameter(Update0505Waves, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
}

// @verified - p
void Update0505Waves(void *) {
    BuildWaveLightingTable(0, 1.0f, 1.0f, {0.800000011920929f, 0.800000011920929f, 0.949999988079071f}, {0.5f, 0.5f, 0.5f}, 50.0f, 1.0f);
    ApplyWaveTextureResource(1);
    SetTextureAnimationUV(0.0f, 0.004699999932199717f);
    ApplyTextureAnimationResource(0);
}

const ScriptAreaDefinition area0505ScriptDefinition{.areaId = AREA_0505, .field04 = 0, .init = Initialize0505, .main = Start0505};

} // namespace shenmue::s2::areas
