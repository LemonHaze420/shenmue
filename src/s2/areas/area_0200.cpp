#include "s2/areas/area_0200.hpp"
#include "s2/lighting.hpp"
#include "common/math.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/model_component.hpp"
#include "s2/model_image.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/weather.hpp"
#include <algorithm>
#include <bit>
namespace shenmue::s2::areas {
namespace {
std::uint32_t bobSpeed0200 = 10, bobPhase0200 = 0;
Vector3 bobPosition0200{};
Vector3i fingerRotation0200{}, handRotation0200{56434, 0, 0};
void E0200MainCallback(void *);
void E0200LoopCallback(void *);
// @verified - p
void E0200Init() {
    bobSpeed0200 = 10;
    bobPhase0200 = 0;
    bobPosition0200 = {};
    fingerRotation0200 = {};
    handRotation0200 = {56434, 0, 0};
}
// @verified - p
void SetupE0200Environment() {
    SetPlayerControlEnabled(0);
    SetRenderClockAndCompassTo(1);
    EV_FreeAuthorPak(1, "0200");
    FreeWAIR();
    EV_UpdateWeatherForMinute(0, 0, 0, 0);
    EV_UpdateWeatherForMinute(1, 0, 0, 0);
    EV_UpdateWeather();
    CreateEventCharacter(CharacterCreationHighModel, nullptr, AREA_0200_BASE);
    AT_SetLocalBaseChara(AREA_0200_A237, AREA_0200_BASE);
    AT_SetLocalBaseChara(AREA_0200_A238, AREA_0200_BASE);
    for (std::int32_t index = 0; index < 9; ++index)
        SetMapLightingPlacementEnabled(0, index);
}
// @verified - p
void E0200Main(std::uintptr_t) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0200MainCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->selection = 0;
    parent->state = 1;
    SetupE0200Environment();
    EV_stepSetFlags(0, 0x85);
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0200_CHARACTERS, parent->self);
}
// @verified - p
void SetupE0200Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 2);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A088, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A237, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A238, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A142, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A143, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A219, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_AB20, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A814, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A816, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A818, false);
    EV_SetCharacterAlphaEnabled(AREA_0200_A820, false);
    AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A814), 1, -1, 1.0f, 1);
    AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A816), 1, -1, 1.0f, 1);
    AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A818), 1, -1, 1.0f, 1);
    AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A820), 1, -1, 1.0f, 1);
    for (const shen_name *character = AREA_0200_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void RestoreE0200Characters() {
    mtwkSetExtraFlag(scenFindMotionwork(AREA_AUTHOR_RYO), 6, 0);
    EV_SetCharacterAlphaEnabled(AREA_AUTHOR_RYO, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A088, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A237, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A238, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A142, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A143, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A219, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_AB20, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A814, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A816, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A818, true);
    EV_SetCharacterAlphaEnabled(AREA_0200_A820, true);
    for (const shen_name *character = AREA_0200_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character) {
        mtwkSetExtraFlag(scenFindMotionwork(*character), 6, 2);
        EV_SetCharacterAlphaEnabled(*character, false);
    }
}
// @verified - p
void ApplyE0200HandRotation() {
    EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0xa3, nullptr, &handRotation0200, EventCharacterPartTransformMode::Relative);
    EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0xa4, nullptr, &handRotation0200, EventCharacterPartTransformMode::Relative);
    EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0xa5, nullptr, &handRotation0200, EventCharacterPartTransformMode::Relative);
}
// @verified - p
void AnimateE0200(std::int32_t frame) {
    Vector3 zero{};
    if (bobPhase0200 > 180) {
        bobSpeed0200 = std::max(static_cast<std::uint32_t>(shenGetRandomInt32(20)), 8U);
        bobPhase0200 = 0;
    } else {
        bobPosition0200.y = sin_fixed16(static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(bobPhase0200) * 8192.0f / 45.0f))) * 0.02f;
        bobPhase0200 += bobSpeed0200;
    }
    if (frame == 3675) {
        fingerRotation0200.x = 0;
        EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, nullptr, &fingerRotation0200, EventCharacterPartTransformMode::Absolute);
    } else if (frame < 3675) {
        if (frame >= 3655) {
            if (frame == 3655)
                fingerRotation0200.x = 65527;
            else if (frame == 3665)
                fingerRotation0200.x = 9;
            EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, nullptr, &fingerRotation0200, EventCharacterPartTransformMode::Relative);
        } else {
            EV_AT_SetPos(AREA_0200_BASE, AREA_AUTHOR_ROOT_POSITION_FLAGS, zero);
            EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, &zero, nullptr, EventCharacterPartTransformMode::Absolute);
        }
    }
    if (frame != 3675) {
        if (static_cast<std::uint32_t>(frame - 3630) <= 24U) {
            handRotation0200.x = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(handRotation0200.x) + 182U);
        } else if (frame == 3500) {
            handRotation0200.x = 60985;
            ApplyE0200HandRotation();
            EV_AT_SetPos(AREA_0200_BASE, AREA_AUTHOR_ROOT_POSITION_FLAGS, bobPosition0200);
            EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, &bobPosition0200, nullptr, EventCharacterPartTransformMode::Absolute);
            ApplyE0200HandRotation();
            return;
        }
    }
    ApplyE0200HandRotation();
    if (static_cast<std::uint32_t>(frame - 844) > 283U && static_cast<std::uint32_t>(frame - 2711) > 107U && frame < 3501) {
        EV_AT_SetPos(AREA_0200_BASE, AREA_AUTHOR_ROOT_POSITION_FLAGS, bobPosition0200);
        EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, &bobPosition0200, nullptr, EventCharacterPartTransformMode::Absolute);
        ApplyE0200HandRotation();
    } else {
        bobPosition0200.y = 0;
        EV_AT_SetPos(AREA_0200_BASE, AREA_AUTHOR_ROOT_POSITION_FLAGS, bobPosition0200);
        EV_scenApplyPosOrRotToPartOf(AREA_0200_A219, 0x99, &bobPosition0200, nullptr, EventCharacterPartTransformMode::Absolute);
    }
}
// @verified - p
void QueueE0200Loop(AreaImmediateAuthorTaskParameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(E0200LoopCallback, 4, 0x0b, 16, AREA_AUTHOR_TASK);
    AreaImmediateAuthorFrameParameter *const child = static_cast<AreaImmediateAuthorFrameParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->state = 0;
    camCtrlSetFarDistance(600.0f);
    camCtrlSetNearDistance(300.0f);
    EV_LoadAudio("0200A000", 0);
    SetupE0200Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(parent->selection));
    parent->state = 2;
}
// @verified - p
void CleanupE0200(AreaImmediateAuthorTaskParameter *parent) {
    for (const shen_name *character = AREA_0200_CHARACTERS; character->name != AREA_AUTHOR_INVALID.name; ++character)
        if (FindCharacterByScenarioName(*character)) {
            EV_npcLockUnlock(*character);
            EV_npcFree(*character);
        }
    EV_FreeAuthorPak(0, "0200");
    EV_CleanupMapControlTask();
    HLib::DestroyTask(parent->self);
    EV_scenQueueLoad(g_actNumber, AREA_0200_QR00, 1);
}
// @verified - p
void E0200MainCallback(void *taskData) {
    AreaImmediateAuthorTaskParameter *const parent = static_cast<AreaImmediateAuthorTaskParameter *>(taskData);
    if (parent->state == 1)
        QueueE0200Loop(parent);
    else if (parent->state == 3)
        CleanupE0200(parent);
}
// @verified - p
void E0200LoopCallback(void *taskData) {
    const std::int32_t frame = EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame);
    switch (frame) {
    case 201:
        camCtrlSetNearDistance(200.0f);
        break;
    case 530:
        AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A814), 1, -1, 0.5f, 0);
        AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A818), 1, -1, 0.5f, 0);
        AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A820), 1, -1, 0.5f, 0);
        break;
    case 535:
        AssignImageModuleParameters(FindCharacterByScenarioName(AREA_0200_A816), 1, -1, 0.5f, 0);
        break;
    case 1940:
        camCtrlSetFarDistance(1200.0f);
        camCtrlSetNearDistance(700.0f);
        break;
    case 2710:
        SetClothFlag(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 4, 1);
        break;
    case 2872:
        SetAllMapFieldDepthSlotsEnabled(0);
        break;
    case 2880:
        SetClothFlag(FindCharacterByScenarioName(AREA_AUTHOR_RYO), 4, 0);
        break;
    case 3128:
        SetAllMapFieldDepthSlotsEnabled(1);
        break;
    case 3220:
        EV_EnableCharacterDisplay(AREA_0200_A088);
        break;
    default:
        break;
    }
    AnimateE0200(frame);
    if (EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) != 0)
        return;
    SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
    RestoreE0200Characters();
    static_cast<AreaImmediateAuthorFrameParameter *>(taskData)->parent->state = 3;
    HLib::DestroyCurrentTask();
}
} // namespace
const ScriptAreaDefinition area0200ScriptDefinition{.areaId = AREA_0200_AREA, .field04 = 0, .init = E0200Init, .main = E0200Main};
} // namespace shenmue::s2::areas
