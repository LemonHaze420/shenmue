#include "s2/areas/area_0547.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc_package.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
#include "s2/sound_program.hpp"
#include "s2/ui.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include <bit>
#include <cmath>
#include <cstring>

namespace shenmue::s2::areas {
namespace {
std::int32_t butterflyLimit0547, butterflyEscape0547, butterflyPaused0547;
HLib::Task *butterflyTask0547;
constexpr std::int32_t butterflySteps0547[]{546, 364, 273, 182};
constexpr float butterflySpeeds0547[]{0.036f, 0.0322f, 0.0288f};
constexpr float butterflyVerticalSpeeds0547[]{0.0125f, 0.01f, 0.005f};
constexpr float butterflyAccelerations0547[]{0.00025f, 0.0002f, 0.00015f};
float DistanceSquared0547(const Vector3 &first, const Vector3 &second) {
    return ((first.y - second.y) * (first.y - second.y) + (first.x - second.x) * (first.x - second.x)) + (first.z - second.z) * (first.z - second.z);
}
void Reset0547Camera() {
    using enum CameraState;
    camCtrlSetFovTransitionTo(true);
    camCtrlUtil(CameraUtilityMode::ResetPlayerDistance, 0);
    camCtrlSetState(Init);
}
void Restore0547Motion(shen_name character, std::uint16_t motionId, const Vector3 &position, const Vector3i &rotation) {
    MotionWork *const motion = FindMotionWork(FindCharacterByScenarioName(character));
    mtwkRequestMotion(motion, motionId, MotionRegion::LowerBody);
    EV_AT_SetPos(character, EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(character, EventTransformFlags::AllAxes, rotation, nullptr);
    mtwkSetExtraFlag(motion, 17, 0);
    SetMotionRegionFlag(motion, 71, 0, MotionRegion::LowerBody);
    mtwkSetExtraFlag(motion, 13, 0);
}
} // namespace
void Initialize0547() {
    butterflyLimit0547 = 0;
    butterflyTask0547 = nullptr;
    butterflyEscape0547 = 0;
    butterflyPaused0547 = 0;
}
void Start0547(std::uintptr_t argument) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0547, 4, 11, sizeof(Area0547Parameter), AREA_0547_EVENT);
    Area0547Parameter *const parent = static_cast<Area0547Parameter *>(HLib::GetTaskParameter(task));
    parent->self = task;
    parent->state = 2;
    EV_PTYCUtils(AREA_0547_SIN, 3, 1);
    EV_PTYCUtils(AREA_0547_SIN, 8, 0);
    if (g_currentMap.name == AREA_0547_MAP.name) {
        if (argument == 0) {
            butterflyTask0547 = HLib::EnqueueTaskWithParameter(Initialize0547Butterflies, 4, 11, sizeof(Area0547ButterflyParameter), AREA_0547_BUTTERFLY);
            butterflyLimit0547 = 300;
            butterflyEscape0547 = 0;
            butterflyPaused0547 = 0;
        }
        parent->selection = static_cast<std::int32_t>(argument);
    } else {
        parent->motions[0] = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0539A.MOT"));
        parent->motions[1] = static_cast<std::uint32_t>(ReadMotiFromDirectory(g_actNumber, g_currentMap, "0547A.MOT"));
        parent->selection = g_entryPoint != 0;
    }
    Queue_EVT_UTL_LoadNPCChara_3(AREA_0547_CHARACTERS, parent->self);
    Setup0547Environment();
}
void Setup0547Environment() {
    SetPlayerControlEnabled(0);
    EV_FreeAuthorPak(1, "0539");
    EV_FreeAuthorPak(1, "0547");
    EV_SetWeatherInstruction(0x780, 0);
    if (g_currentMap.name != AREA_0547_MAP.name) {
        EV_ReinitLights(1);
        EV_SetLightFlagForIndex(0, 2);
        EV_SetIndexForLightEntry(0, 1);
    }
}
void Update0547(void *data) {
    Area0547Parameter *const parent = static_cast<Area0547Parameter *>(data);
    if (parent->state == 2)
        Queue0547Author(parent);
    else if (parent->state == 4)
        Cleanup0547(parent);
}
void Queue0547Author(Area0547Parameter *parent) {
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(Update0547Author, 4, 11, sizeof(Area0547AuthorParameter), AREA_0547_EVENT);
    Area0547AuthorParameter *const child = static_cast<Area0547AuthorParameter *>(HLib::GetTaskParameter(task));
    child->parent = parent;
    child->frame = 0;
    SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    Prepare0547Characters();
    EV_authSceneCtrl(static_cast<AuthSceneCtrl>(g_currentMap.name == AREA_0547_MAP.name ? AREA_0547_AUTHORS[parent->selection].name : parent->selection));
    parent->state = 3;
}
void Prepare0547Characters() {
    for (const shen_name character : {AREA_0547_RYO, AREA_0547_SIN}) {
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 2);
        EV_SetCharacterAlphaEnabled(character, false);
    }
    for (const shen_name prop : AREA_0547_PROPS) {
        EV_EnableCharacterDisplay(prop);
        EV_SetCharacterAlphaEnabled(prop, false);
    }
}
void Restore0547Characters() {
    for (const shen_name character : {AREA_0547_RYO, AREA_0547_SIN}) {
        mtwkSetExtraFlag(FindMotionWork(FindCharacterByScenarioName(character)), 6, 0);
        EV_SetCharacterAlphaEnabled(character, true);
    }
    for (const shen_name prop : AREA_0547_PROPS)
        EV_SetCharacterAlphaEnabled(prop, true);
}
void Finish0547AuthorNoop() {
}
void Update0547Author(void *data) {
    Area0547AuthorParameter *const child = static_cast<Area0547AuthorParameter *>(data);
    child->frame = std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(static_cast<std::uint16_t>(child->frame) + 1U));
    if (g_currentMap.name == AREA_0547_MAP.name && child->parent->selection == 0)
        butterflyEscape0547 = child->frame >= 270;
    if (!EV_authSceneCtrl(AuthSceneCtrl::IsLoaded)) {
        Restore0547Characters();
        child->parent->state = 4;
        if (g_currentMap.name != AREA_0547_MAP.name)
            SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        else {
            Finish0547AuthorNoop();
            butterflyEscape0547 = 0;
            if (child->parent->selection == 1)
                SetScreenFilmFade(0, 0, 0, 0, 0, 255, 0, 0, 0);
        }
        HLib::DestroyCurrentTask();
    }
}
void Update0547Wake(void *) {
    SetPlayerControlEnabled(1);
    HLib::DestroyCurrentTask();
}
void Finish0547Loading(void *data) {
    Area0547Parameter *const parent = static_cast<Area0547Parameter *>(data);
    if (IsScenarioBusy()) {
        shenToggleAreaFlags(0);
        FinishLoading();
        HLib::DestroyTask(parent->self);
    }
}
void Update0547ButterflyControl(void *) {
    if (GetShouldRenderIcons(RenderIconSlot::ItemSelect)) {
        if (!butterflyPaused0547)
            HLib::SetupTimedTask(butterflyTask0547, 0);
        butterflyPaused0547 = 1;
    } else {
        if (butterflyPaused0547)
            HLib::ResolveDeferredCallback(butterflyTask0547);
        butterflyPaused0547 = 0;
    }
}
void Cleanup0547(Area0547Parameter *parent) {
    EV_SetWeatherInstruction(0x780, 1);
    EV_FreeAuthorPak(0, "0539");
    EV_FreeAuthorPak(0, "0547");
    EV_PTYCUtils(AREA_0547_SIN, 3, 0);
    EV_CleanupMapControlTask();
    if (g_currentMap.name != AREA_0547_MAP.name) {
        for (const shen_name character : AREA_0547_CHARACTERS) {
            if (character.name == 0xffffffffU)
                break;
            if (DoesScenarioCharacterExist(character)) {
                EV_npcLockUnlock(character);
                EV_npcFree(character);
            }
        }
        DeleteMotion(std::bit_cast<std::int32_t>(parent->motions[0]));
        DeleteMotion(std::bit_cast<std::int32_t>(parent->motions[1]));
        HLib::DestroyTask(parent->self);
        EV_scenQueueLoad(g_actNumber, g_currentMap, g_entryPoint);
        return;
    }
    if (parent->selection != 0) {
        EV_LoadCutsceneFromCurrentScene("KWW1");
        EV_LoadAudio("KWW1_000", 0);
        EV_spmBeginSampleExecute();
        Vector3 ryo{88.900002f, 200.0f, 213.39999f};
        ryo.y = shenFindIntersectionWithScene(ryo, 90.0f);
        Restore0547Motion(AREA_0547_RYO, 0xf003, ryo, Vector3i{0, 64170, 0});
        Vector3 shenhua{90.0f, 200.0f, 210.5f};
        shenhua.y = shenFindIntersectionWithScene(shenhua, ryo.y);
        Restore0547Motion(AREA_0547_SIN, 0x9502, shenhua, Vector3i{});
        EV_NPC_NewLoadSw(1);
        InitiateCleanupForSceneChara(AREA_0547_SIN);
        ActivatePartyCompanion(AREA_0547_SIN);
        EV_PTYCUtils(AREA_0547_SIN, 3, 0);
        shenToggleAreaFlags(3);
        SetPlayerGameFlags(PlayerGameFlagMode::RetainMovementMode, reinterpret_cast<void *>(std::uintptr_t{1}));
        HLib::Task *const wake = HLib::EnqueueTaskWithoutParameter(Update0547Wake, 3, 2, static_cast<std::int32_t>(AREA_0547_WAKE.name));
        HLib::SetupTimedTask(wake, 30);
        Reset0547Camera();
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::SetCurrentTaskTo(Finish0547Loading);
        HLib::SetupTaskDelay(40);
    } else {
        EV_NPC_NewLoadSw(1);
        InitiateCleanupForSceneChara(AREA_0547_SIN);
        Reset0547Camera();
        HLib::EnqueueTaskWithoutParameter(Update0547ButterflyControl, 4, 11, static_cast<std::int32_t>(AREA_0547_CONTROL.name));
        HLib::DestroyTask(parent->self);
        SetPlayerControlEnabled(1);
        FinishLoading();
        EV_PTYCUtils(AREA_0547_SIN, 3, 0);
    }
}
void Initialize0547Butterflies(void *data) {
    Area0547ButterflyParameter *const parameter = static_cast<Area0547ButterflyParameter *>(data);
    for (Area0547Butterfly &butterfly : parameter->butterflies) {
        const std::int32_t delay = shenGetRandomInt32(90);
        butterfly.delay = delay;
        butterfly.initialDelay = delay;
        butterfly.state = 0;
        butterfly.animation = 0;
        butterfly.animationDelay = 0;
    }
    HLib::SetCurrentTaskTo(Update0547Butterflies);
}
void Update0547Butterflies(void *data) {
    Area0547ButterflyParameter *const parameter = static_cast<Area0547ButterflyParameter *>(data);
    Vector3 target{}, eye{}, ground{}, ryo{};
    Vector3i ryoRotation{};
    camCtrlGetTargetPosition(&target);
    camCtrlGetNextCamPos(&eye);
    camCtrlGetTargetPosition(&ground);
    ground.y = 500.0f;
    ground.y = shenFindIntersectionWithScene(ground, 500.0f);
    EV_GetCharaPosition(AREA_0547_RYO, 0, &ryo, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(AREA_0547_RYO, &ryoRotation, EventTransformFlags::World);
    const Vector3 scale{1.0f, 1.0f, 1.0f};
    std::int32_t drawn = 0;
    for (std::int32_t index = 0; index < 500; ++index) {
        Area0547Butterfly &butterfly = parameter->butterflies[index];
        if (butterfly.state == 0) {
            if (butterfly.delay > 0) {
                --butterfly.delay;
                continue;
            }
            butterfly.state = 1;
            const std::int32_t spawn = shenGetRandomInt32(7);
            const std::uint16_t yaw = static_cast<std::uint16_t>(shenGetRandomInt32(65536));
            if (spawn == 5 || spawn == 6) {
                const float radius = GetRandomNumberInRange(20.0f);
                butterfly.position.x = sin_fixed16(static_cast<std::int16_t>(yaw)) * radius + eye.x + 1.0f;
                butterfly.height = GetRandomNumberInRange(1.0f) + (ground.y + 0.60000002f);
                butterfly.position.y = butterfly.height;
                butterfly.position.z = cos_fixed16(yaw) * radius + eye.z + 2.0f;
            } else {
                const float radius = GetRandomNumberInRange(3.0f);
                butterfly.position.x = sin_fixed16(static_cast<std::int16_t>(yaw)) * radius + AREA_0547_SPAWNS[spawn].x + 1.0f;
                butterfly.height = GetRandomNumberInRange(1.0f) + (AREA_0547_SPAWNS[spawn].y - 0.30000001f);
                butterfly.position.y = butterfly.height;
                butterfly.position.z = cos_fixed16(yaw) * radius + AREA_0547_SPAWNS[spawn].z + 2.0f;
            }
            butterfly.velocity = {};
            std::memset(butterfly.reserved24, 0, sizeof(butterfly.reserved24));
            butterfly.acceleration = {};
            butterfly.model = AREA_0547_BUTTERFLY_MODELS[index & 15];
            butterfly.rotation = {};
            butterfly.delay = 0;
            butterfly.initialDelay = 0;
            butterfly.animation = 0;
            butterfly.animationDelay = 1;
            butterfly.flightPhase = 0;
            butterfly.flightReserved = 0;
            butterfly.flutterPhase = 0;
            butterfly.flutterReserved = 0;
            butterfly.flightSpeedIndex = shenGetRandomInt32(4);
            butterfly.flutterSpeedIndex = shenGetRandomInt32(4);
            continue;
        }
        if (butterfly.state < 0 || butterfly.state > 3)
            continue;
        float distance;
        Vector2 screen{};
        if (butterfly.state <= 2) {
            butterfly.velocity.x = sin_fixed16(static_cast<std::int16_t>(butterfly.flightPhase)) * 0.0099999998f;
            butterfly.velocity.z = cos_fixed16(butterfly.flightPhase) * 0.0099999998f;
            const float flutter = sin_fixed16(static_cast<std::int16_t>(2U * butterfly.flutterPhase));
            const float flutterPhase = sin_fixed16(static_cast<std::int16_t>(butterfly.flutterPhase));
            butterfly.flightPhase = static_cast<std::uint16_t>(butterfly.flightPhase + butterflySteps0547[butterfly.flightSpeedIndex]);
            butterfly.flightReserved = 0;
            butterfly.position.y = flutter * flutterPhase * 0.1f + butterfly.height;
            butterfly.flutterPhase = static_cast<std::uint16_t>(butterfly.flutterPhase + butterflySteps0547[butterfly.flutterSpeedIndex]);
            butterfly.flutterReserved = 0;
            ProjectWorldToScreen(&butterfly.position, &screen);
            distance = DistanceSquared0547(eye, butterfly.position);
            if ((DistanceSquared0547(target, butterfly.position) <= 9.0f && butterfly.state != 3) || butterflyEscape0547) {
                const std::int32_t direction = butterfly.position.x >= 100.0f ? -1 : 1;
                const std::int32_t speed = shenGetRandomInt32(3);
                const std::uint16_t yaw = static_cast<std::uint16_t>(ryoRotation.y + direction * 8192);
                butterfly.velocity.x = sin_fixed16(static_cast<std::int16_t>(yaw)) * butterflySpeeds0547[speed];
                butterfly.velocity.y = butterflyVerticalSpeeds0547[speed];
                butterfly.velocity.z = cos_fixed16(yaw) * butterflySpeeds0547[speed];
                const std::int32_t acceleration = shenGetRandomInt32(3);
                const std::uint16_t escapeYaw = static_cast<std::uint16_t>(ryoRotation.y + direction * 21845);
                butterfly.acceleration.x = sin_fixed16(static_cast<std::int16_t>(escapeYaw)) * butterflyAccelerations0547[acceleration];
                butterfly.acceleration.y = 0.0000499999987f;
                butterfly.acceleration.z = cos_fixed16(escapeYaw) * butterflyAccelerations0547[acceleration];
                butterfly.state = 3;
            }
        } else {
            butterfly.velocity.x += butterfly.acceleration.x;
            butterfly.flightPhase = static_cast<std::uint16_t>(butterfly.flightPhase + butterflySteps0547[butterfly.flightSpeedIndex]);
            butterfly.velocity.y += butterfly.acceleration.y;
            butterfly.flutterPhase = static_cast<std::uint16_t>(butterfly.flutterPhase + butterflySteps0547[butterfly.flutterSpeedIndex]);
            butterfly.velocity.z += butterfly.acceleration.z;
            butterfly.flutterReserved = 0;
            butterfly.position.x += butterfly.velocity.x;
            butterfly.position.y += butterfly.velocity.y;
            butterfly.flightReserved = 0;
            butterfly.position.y -= sin_fixed16(static_cast<std::int16_t>(20U * butterfly.flutterPhase)) * 0.015f;
            butterfly.position.z += butterfly.velocity.z;
            ProjectWorldToScreen(&butterfly.position, &screen);
            distance = DistanceSquared0547(eye, butterfly.position);
            DistanceSquared0547(target, butterfly.position);
        }
        if (distance >= 400.0f || butterfly.position.y >= (target.y + eye.y) * 0.5f + 5.0f) {
            const std::int32_t delay = shenGetRandomInt32(30);
            butterfly.delay = delay;
            butterfly.initialDelay = delay;
            butterfly.state = 0;
        }
        if (screen.x >= 0.0f && screen.x <= 639.0f && screen.y >= 0.0f && screen.y <= 479.0f && distance < 49.0f && drawn < butterflyLimit0547) {
            ApplyTranslationRotationScaleXYZToCharaByID(butterfly.model, butterfly.animation, &butterfly.position, &butterfly.rotation, &scale);
            if (--butterfly.animationDelay < 0) {
                butterfly.animationDelay = 1;
                butterfly.animation = (butterfly.animation + 1) & 3;
            }
            ++drawn;
        }
    }
}
const ScriptAreaDefinition area0547ScriptDefinition{AREA_0547, 0, Initialize0547, Start0547};
} // namespace shenmue::s2::areas
