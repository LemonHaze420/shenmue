#include "s2/areas/area_0512.hpp"
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
#include "s2/screen_film.hpp"

#include "s2/world_render.hpp"
#include <bit>

namespace shenmue::s2::areas {

// @verified - p
void Initialize0512() {
}

// @verified - p
void Start0512(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0512, 4, 0x0b, sizeof(AuthorViewerTaskParameter), AUTHOR_VIEWER_TASK);
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->motion = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, AREA_0512_MOTION));
    parent->selection = 0;
    parent->state = 0;
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, AREA_0512_CONVERSATION);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0512_CHARACTERS, parent->self);
}

// @verified - p
void Update0512(void *taskData) {
    AuthorViewerTaskParameter *const parent = static_cast<AuthorViewerTaskParameter *>(taskData);
    if (parent->state == 1) {
        Queue0512Author(parent);
    } else if (parent->state == 3) {
        Cleanup0512(parent);
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
            EV_debug_log_to_screen(AREA_0512_DEBUG_NAMES[parent->selection]);
        }
        EV_set_debug_text_pos(2, 5);
        EV_debug_log_to_screen("seqdata%d.avb\n", static_cast<std::int32_t>(parent->selection));
    }
}

// @verified - p
void Cleanup0512(AuthorViewerTaskParameter *parent) {
    for (const shen_name *character = AREA_0512_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    }
    EV_FreeAuthorPak(0, AREA_0512_CONVERSATION);
    DeleteMotion(std::bit_cast<std::int32_t>(parent->motion));
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
}

// @verified - p
std::int64_t Queue0512Author(AuthorViewerTaskParameter *parent) {
    constexpr HLib::TaskCallback callbacks[]{Update0512AuthorTask, Update0512SecondAuthorTask};
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(callbacks[parent->selection], 4, 0x0b, sizeof(Area0512AuthorParameter), AUTHOR_VIEWER_TASK);
    Area0512AuthorParameter *const child = static_cast<Area0512AuthorParameter *>(HLib::GetTaskParameter(task));
    child->sceneAlpha = 0.0f;
    child->parent = parent;
    child->skyAlpha = 1.0f;
    child->propAlpha = 0.0f;
    EV_SetWeatherInstruction(12, 0);
    EV_SetModelDataDefaultVertexShader();
    if (parent->selection == 0 || parent->selection == 1)
        EV_lightLoad(AREA_0512_LIGHTS[parent->selection]);
    PushGlobalLightingProgram();
    EV_LoadAudio("0512A000", 0);
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0512Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
    return 2;
}

// @verified - p
std::int64_t Update0512Author(Area0512AuthorParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 0) {
        SetScenarioCharacterTransparency(AREA_0512_a229, child->propAlpha);
        SetScenarioCharacterTransparency(AREA_0512_a067, child->skyAlpha);
        SetScenarioCharacterTransparency(AREA_0512_a460, child->sceneAlpha);
    } else if (frame == 721) {
        SetIsSkySetup(0);
    } else if (frame == 1240) {
        SetScenarioCharacterTransparency(AREA_0512_a067, 1.0f);
        SetIsSkySetup(1);
    }
    if (static_cast<std::uint32_t>(frame - 1121) <= 0x76U) {
        child->propAlpha += 0.008299999870359898f;
        SetScenarioCharacterTransparency(AREA_0512_a229, child->propAlpha);
    } else if (frame == 1240) {
        SetScenarioCharacterTransparency(AREA_0512_a229, 1.0f);
        goto AuthorStatus;
    }
    if (static_cast<std::uint32_t>(frame - 1121) <= 0x30U) {
        child->sceneAlpha += 0.019999999552965164f;
        SetScenarioCharacterTransparency(AREA_0512_a460, child->sceneAlpha);
    } else if (frame == 1170) {
        SetScenarioCharacterTransparency(AREA_0512_a460, 1.0f);
        goto AuthorStatus;
    }
    if (static_cast<std::uint32_t>(frame - 1045) <= 0x4aU) {
        child->skyAlpha -= 0.006599999964237213f;
        SetScenarioCharacterTransparency(AREA_0512_a067, child->skyAlpha);
    } else if (frame == 1120) {
        SetScenarioCharacterTransparency(AREA_0512_a067, 0.5f);
    }
AuthorStatus:
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetIsSkySetup(1);
        Restore0512Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}

void Update0512AuthorTask(void *taskData) {
    Update0512Author(static_cast<Area0512AuthorParameter *>(taskData));
}

// @verified - p
void Prepare0512Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0512_RYO_), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_0512_RYO_, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_a067, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_a460, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_a431, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_a214, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_aa29, false);
    EV_SetCharacterAlphaEnabled(AREA_0512_a229, false);
    for (const shen_name *character = AREA_0512_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}

// @verified - p
void Restore0512Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_0512_RYO_), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_0512_RYO_, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_a067, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_a460, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_a431, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_a214, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_aa29, true);
    EV_SetCharacterAlphaEnabled(AREA_0512_a229, true);
    for (const shen_name *character = AREA_0512_CHARACTERS; character->name != AUTHOR_VIEWER_INVALID_CHARACTER.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 0);
        EV_SetCharacterAlphaEnabled(*character, true);
    }
}

// @verified - p
std::int64_t Update0512SecondAuthor(Area0512AuthorParameter *child) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    if (frame == 0) {
        Prepare0512Characters();
        SetScenarioCharacterTransparency(AREA_0512_a229, child->propAlpha);
        SetScenarioCharacterTransparency(AREA_0512_a067, child->skyAlpha);
        SetScenarioCharacterTransparency(AREA_0512_a460, child->sceneAlpha);
    } else if (frame == 541) {
        SetIsSkySetup(0);
    } else if (frame == 1061) {
        SetScenarioCharacterTransparency(AREA_0512_a067, 1.0f);
        SetIsSkySetup(1);
    }
    if (static_cast<std::uint32_t>(frame - 941) <= 0x76U) {
        child->propAlpha += 0.008299999870359898f;
        SetScenarioCharacterTransparency(AREA_0512_a229, child->propAlpha);
    } else if (frame == 1060) {
        SetScenarioCharacterTransparency(AREA_0512_a229, 1.0f);
        goto AuthorStatus;
    }
    if (static_cast<std::uint32_t>(frame - 941) <= 0x30U) {
        child->sceneAlpha += 0.019999999552965164f;
        SetScenarioCharacterTransparency(AREA_0512_a460, child->sceneAlpha);
    } else if (frame == 990) {
        SetScenarioCharacterTransparency(AREA_0512_a460, 1.0f);
        goto AuthorStatus;
    }
    if (static_cast<std::uint32_t>(frame - 865) <= 0x4aU) {
        child->skyAlpha -= 0.006599999964237213f;
        SetScenarioCharacterTransparency(AREA_0512_a067, child->skyAlpha);
    } else if (frame == 940) {
        SetScenarioCharacterTransparency(AREA_0512_a067, 0.5f);
    }
AuthorStatus:
    const std::int64_t result = EV_authSceneCtrl(AuthSceneCtrl::IsLoaded);
    if (static_cast<std::uint32_t>(result) == 0) {
        SetIsSkySetup(1);
        Restore0512Characters();
        child->parent->state = 0;
        SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        return HLib::DestroyCurrentTask();
    }
    return result;
}
void Update0512SecondAuthorTask(void *taskData) {
    Update0512SecondAuthor(static_cast<Area0512AuthorParameter *>(taskData));
}

const ScriptAreaDefinition area0512ScriptDefinition{.areaId = AREA_0512, .field04 = 0, .init = Initialize0512, .main = Start0512};

} // namespace shenmue::s2::areas
