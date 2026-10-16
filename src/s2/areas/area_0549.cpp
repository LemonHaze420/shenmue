#include "s2/areas/area_0549.hpp"
#include "s2/input.hpp"
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
#include "s2/npc.hpp"
#include "s2/pak.hpp"
#include "s2/scene_data.hpp"
#include "s2/sound_program.hpp"
#include "s2/screen_film.hpp"

#include "s2/date_time.hpp"
#include "s2/load_title.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {
namespace {
float range0549 = 0.5f, sceneIntensity0549 = 1.0f;
float falloff0549[2]{0.3f, 0.3f};
Vector4 emissive0549{1.0f, 1.0f, 1.0f, 1.0f};
Vector4 reflector0549{};
} // namespace

// @verified - p
void Initialize0549() {
    range0549 = 0.5f;
    sceneIntensity0549 = 1.0f;
    falloff0549[0] = falloff0549[1] = 0.3f;
    emissive0549 = Vector4{1.0f, 1.0f, 1.0f, 1.0f};
    reflector0549 = Vector4{};
}

// @verified - p
void Start0549(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0549, 4, 0x0b, sizeof(Area0549Parameter), AUTHOR_VIEWER_TASK);
    Area0549Parameter *const parent = static_cast<Area0549Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = g_entryPoint <= 1 ? static_cast<std::int16_t>(g_entryPoint) : 0;
    parent->state = g_entryPoint != 99 ? 1 : 0;

    Queue_EVT_UTL_LoadNPCChara_3(AREA_0549_CHARACTERS, parent->self);
    Setup0549Environment();
}

// @verified - p
void Update0549(void *taskData) {
    Area0549Parameter *const parent = static_cast<Area0549Parameter *>(taskData);
    if (parent->state == 1) {
        Queue0549Author(parent);
    } else if (parent->state == 3) {
        Queue_ST_LoadTitleFadeIn(AREA_0549_TITLE_AUTHOR);
        parent->state = 4;
        HLib::SetupTimedTask(parent->self, 160);
    } else if (parent->state == 4) {
        ST_LoadTitleFade();
        parent->state = 5;
        HLib::SetupTimedTask(parent->self, 100);
    } else if (parent->state == 5) {
        Cleanup0549(parent);
    } else if (parent->state == 0) {
        const std::uint16_t input = static_cast<std::uint16_t>(EVT_GetControllerInput(0, 1));
        if (input == 1024) {
            parent->state = 1;
        } else if (input == 64) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) - 1U));
            if (parent->selection < 0)
                parent->selection = 1;
        } else if (input == 128) {
            parent->selection = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(parent->selection) + 1U));
            if (parent->selection > 1)
                parent->selection = 0;
        }
        if (parent->selection >= 0 && parent->selection <= 1) {
            EV_set_debug_text_pos(2, 4);
            EV_debug_log_to_screen(AREA_0549_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0549(Area0549Parameter *parent) {
    for (const shen_name *character = AREA_0549_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0549_CONVERSATION);

    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    if (g_entryPoint == 99 || parent->selection == 0)
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
    else
        InitializeScene();
}

// @verified - p
std::int64_t Queue0549Author(Area0549Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0549AuthorTask, 4, 0x0b, sizeof(Area0549AuthorParameter), AUTHOR_VIEWER_TASK);
    static_cast<Area0549AuthorParameter *>(HLib::GetTaskParameter(task))->parent = parent;
    EV_lightLoad(AREA_0549_LIGHT);
    Prepare0549Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0549Author(Area0549AuthorParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (child->parent->selection == 1) {
        if (frame == 0) {
            SetLightSceneIntensity(23, sceneIntensity0549);
            SyncWindWithWDATWeatherState();
            SetIsSkySetup(0);
        } else if (frame == 810)
            SetIsSkySetup(1);
        else if (frame == 1020) {
            EV_FreeWAIR();
            EV_UpdateWeatherForMinute(0, 0, 60, 1.0f);
            EV_UpdateWeather();
        } else if (frame == 1180)
            SetNewWindPhaseAndSpeed(43326, 0.4f);
        if (static_cast<std::uint32_t>(frame - 220) <= 99U) {
            falloff0549[0] = falloff0549[1];
            SetLightFalloff(17, falloff0549);
            range0549 += 0.005f;
            SetLightRange(17, range0549);
            sceneIntensity0549 += 0.015f;
            SetLightSceneIntensity(17, sceneIntensity0549);
            emissive0549.z += -0.01f;
            emissive0549.w += -0.01f;
            SetLightRuntimeColour(17, emissive0549);
        } else if (frame == 330) {
            sceneIntensity0549 = 2.5f;
            falloff0549[0] = falloff0549[1] = 0.6f;
            range0549 = 1.0f;
            emissive0549.z = emissive0549.w = 0.0f;
        } else if (static_cast<std::uint32_t>(frame - 411) <= 88U) {
            emissive0549.y += -0.011111111f;
            SetLightRuntimeColour(17, emissive0549);
        } else if (frame == 500) {
            sceneIntensity0549 = 1.8f;
            emissive0549.y = 0.0f;
            SetLightRuntimeColour(17, emissive0549);
            emissive0549.y = 1.0f;
            SetScenarioReflectorBoardValues(AREA_0549_aa30, &reflector0549);
        } else if (static_cast<std::uint32_t>(frame - 501) <= 49U) {
            sceneIntensity0549 += -0.035999998f;
            SetLightSceneIntensity(21, sceneIntensity0549);
        } else if (frame == 1859)
            sceneIntensity0549 = 0.0f;
        else if (static_cast<std::uint32_t>(frame - 1860) <= 2U) {
            sceneIntensity0549 += 1.1999999f;
            SetLightSceneIntensity(23, sceneIntensity0549);
        } else if (frame == 1863) {
            sceneIntensity0549 = 3.6f;
            SetLightSceneIntensity(23, sceneIntensity0549);
        } else if (static_cast<std::uint32_t>(frame - 1865) <= 64U) {
            sceneIntensity0549 += -0.054545455f;
            SetLightSceneIntensity(23, sceneIntensity0549);
        }
    }
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        range0549 = 0.5f;
        sceneIntensity0549 = 1.0f;
        falloff0549[0] = falloff0549[1] = 0.3f;
        emissive0549 = Vector4{1.0f, 1.0f, 1.0f, 1.0f};
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        Restore0549Characters();
        child->parent->state = g_entryPoint == 99 ? 0 : child->parent->selection == 1 ? 3 : 5;
        HLib::SetupTimedTask(child->parent->self, 60);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0549AuthorTask(void *taskData) {
    Update0549Author(static_cast<Area0549AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0549Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0549_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0549_RYO_, false);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0549_SIN_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0549_SIN_, false);
    EV_SetCharacterAlphaEnabled(AREA_0549_a067, false);
    EV_SetCharacterAlphaEnabled(AREA_0549_aa30, false);
}

// @verified - p
void Restore0549Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0549_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0549_RYO_, true);
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0549_SIN_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0549_SIN_, true);
    EV_SetCharacterAlphaEnabled(AREA_0549_a067, true);
    EV_SetCharacterAlphaEnabled(AREA_0549_aa30, true);
}

void Setup0549Environment() {
    EV_SetWeatherInstruction(12, 0);
    DateTime time{};
    GetCurrentDateTime(time);
    time.hour = 18;
    time.minute = 50;
    SetCurrentDateTimeTo(time);
    EV_FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0.0f);
    EV_UpdateWeatherForMinute(1, 0, 0, 0.0f);
    EV_UpdateWeather();
    for (std::int32_t slot = 0; slot != 9; ++slot)
        EV_AssignLightParams(1, slot);
    SetPlayerControlEnabled(0);
    HLib::EnqueueTaskWithoutParameter(Update0549Waves, 3, 2, static_cast<std::int32_t>(AUTHOR_VIEWER_TASK.name));
    EV_FreeAuthorPak(1, "0549");
}
void Update0549Waves(void *) {
    SetTextureAnimationUV(0.0f, 0.015f);
    ApplyTextureAnimationResource(4);
}
const ScriptAreaDefinition area0549ScriptDefinition{.areaId = AREA_0549, .field04 = 0, .init = Initialize0549, .main = Start0549};

} // namespace shenmue::s2::areas
