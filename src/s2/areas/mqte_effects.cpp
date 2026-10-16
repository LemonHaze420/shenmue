#include "s2/areas/mqte_effects.hpp"
#include "d3t/dx11_renderer.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/maze.hpp"
#include "s2/motion_work.hpp"
#include "s2/world_render.hpp"
#include <cmath>

namespace shenmue::s2::areas {
namespace {
void RealignForestDummy(shen_name dummy, shen_name character) {
    Vector3 position, target;
    Vector3i rotation, targetRotation;
    EV_GetCharaPosition(dummy, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    EV_GetCharaPosition(character, CHARACTER_TRANSLATION_ROOT, &target, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(dummy, &rotation, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(character, &targetRotation, EventTransformFlags::World);
    position.y = target.y;
    position.x = (target.x - position.x) * .2f + position.x;
    position.z = (target.z - position.z) * .2f + position.z;
    const std::int16_t difference = static_cast<std::int16_t>(targetRotation.y - rotation.y);
    rotation.y = static_cast<std::int16_t>(static_cast<std::int32_t>(difference * .06f + rotation.y));
    EV_AT_SetPos(dummy, EventTransformFlags::World | EventTransformFlags::AllAxes, position);
    EV_AT_SetAngY(dummy, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
}
void ForestCamera(void *data) {
    ForestCameraWork &work = *static_cast<ForestCameraWork *>(data);
    if (static_cast<std::int32_t>(GetCharacterMazeUnitIndex(ctrlGetCharaByName(MQTE_SHENFA))) == -1)
        EVT_AT_SetCam(work.camera, work.eye, work.target, 0, 0, 0);
    if (work.cleanup)
        HLib::DestroyCurrentTask();
}
void QueueForestCamera(std::int32_t camera, shen_name eye, shen_name target, std::int32_t delay) {
    if (delay <= 0) {
        if (static_cast<std::int32_t>(GetCharacterMazeUnitIndex(ctrlGetCharaByName(MQTE_SHENFA))) == -1)
            EVT_AT_SetCam(camera, eye, target, 0, 0, 0);
    } else {
        HLib::Task *const task = HLib::EnqueueTaskWithParameter(ForestCamera, 6, 0x0b, sizeof(ForestCameraWork), MQTE_EVENT);
        ForestCameraWork &work = *static_cast<ForestCameraWork *>(HLib::GetTaskParameter(task));
        work.camera = camera;
        work.eye = eye;
        work.target = target;
        work.cleanup = 1;
        HLib::SetupTimedTask(task, delay);
    }
}
} // namespace
std::int32_t ForestCharacterRoadIndex(shen_name name) {
    Vector3 position;
    std::int32_t block = -1, index = -1;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    FindNearestMazeRoad(&position, &block, &index);
    return index;
}
void UpdateForestCharacterRoute(shen_name name) {
    const Vector3 target = GetMazeRoads(forestStage)[ForestCharacterRoadIndex(name) + 1].position;
    Vector3 position;
    Vector3i rotation;
    EV_GetCharaPosition(name, CHARACTER_TRANSLATION_ROOT, &position, EventTransformFlags::World);
    GetScenarioCharacterEulerAngles(name, &rotation, EventTransformFlags::World);
    const float x = target.x - position.x, z = target.z - position.z;
    if (std::sqrt(x * x + z * z) > 1.5f) {
        const std::int16_t yaw = static_cast<std::int16_t>(atan2_fixed16(x, z));
        const std::int16_t difference = static_cast<std::int16_t>(yaw - static_cast<std::int16_t>(rotation.y));
        rotation.y = static_cast<std::int16_t>(static_cast<std::int32_t>(difference * .1f + rotation.y));
        EV_AT_SetAngY(name, EventTransformFlags::World | EventTransformFlags::AllAxes, rotation);
    }
}
void UpdateForestShenfaSpacing() {
    Vector3 shenfa, ryo;
    EV_GetCharaPosition(MQTE_SHENFA, CHARACTER_TRANSLATION_ROOT, &shenfa, EventTransformFlags::World);
    EV_GetCharaPosition(MQTE_RYO, CHARACTER_TRANSLATION_ROOT, &ryo, EventTransformFlags::World);
    const float x = shenfa.x - ryo.x, z = shenfa.z - ryo.z, distance = std::sqrt(x * x + z * z);
    const Vector3 target{1, 1, distance > 20 ? -1.0f : distance > 10 ? -.7f : distance > 8 ? -.5f : distance > 5 ? -.3f : distance > 3 ? -.2f : 0};
    SetCharaTargetTransformOffset(MQTE_SHENFA, &target, 1);
}
void ForestQteDummyControl(void *) {
    if (DoesScenarioCharacterExist(MQTE_DUMMY_RYO)) {
        RealignForestDummy(MQTE_DUMMY_RYO, MQTE_RYO);
        RealignForestDummy(MQTE_DUMMY_SHENFA, MQTE_SHENFA);
    }
}
void ForestQteRunShenfa(void *) {
    UpdateForestCharacterRoute(MQTE_SHENFA);
    const std::int32_t previous = forestShenfaTicks--;
    if (previous < 0)
        UpdateForestShenfaSpacing();
}
void SetForestObstacleCamera(shen_name dummy, std::int32_t obstacle) {
    if (dummy.name == MQTE_DUMMY_SHENFA.name) {
        do {
            forestSelectedCamera = forestShenfaCameras[obstacle][shenGetRandomInt32(4)];
        } while (forestSelectedCamera < 0);
        forestSelectedCamera = forestShenfaCameras[obstacle][0];
    } else {
        do {
            forestSelectedCamera = forestRyoCameras[obstacle][shenGetRandomInt32(4)];
        } while (forestSelectedCamera < 0);
    }
    EVT_AT_SetCam(forestSelectedCamera, dummy, dummy, 0, 0, 0);
}
void QueueForestRoadCameras(std::int32_t unit, std::int32_t delay) {
    const ForestRoadCameraSequence *table = forestOtherRoadCameras;
    if (!forestStage)
        table = forestFirstRoadCameras;
    else if (forestStage == 1)
        table = forestSecondRoadCameras;
    else if (forestStage == 2)
        table = forestThirdRoadCameras;
    std::int32_t selected = -1;
    for (const ForestRoadCamera *entry = table[unit].cameras; entry->camera; ++entry) {
        std::int32_t camera = entry->camera;
        shen_name eye = MQTE_INVALID, target = MQTE_INVALID;
        if (camera >= 0) {
            if (camera == 6020)
                target = MQTE_DUMMY_SHENFA;
        } else {
            if (selected < 0)
                do {
                    selected = forestRandomRoadCameras[shenGetRandomInt32(7)];
                } while (selected < 0);
            camera = selected;
            eye = target = selected >= 5500 ? MQTE_DUMMY_SHENFA : MQTE_DUMMY_RYO;
        }
        QueueForestCamera(camera, eye, target, delay);
        delay += entry->frames;
    }
}
void InitializeForestFireflies(std::int32_t profile) {
    const ForestFireflyProfile &config = forestFireflyProfiles[profile];
    ForestFireflyWork &work = *static_cast<ForestFireflyWork *>(HLib::GetTaskParameter(forestFireflyTask));
    forestFireflyAmplitude = config.amplitude;
    forestFireflyScale = config.scale;
    forestFireflyCount = config.count;
    std::int32_t i = 0;
    for (; i < forestFireflyCount; ++i) {
        ForestFirefly &particle = work.particles[i];
        const Vector3 &center = config.centers[shenGetRandomInt32(4)];
        particle.position.x = (GetRandomNumberInRange(config.extent.x) + center.x) - config.extent.x * .5f;
        particle.position.y = (GetRandomNumberInRange(config.extent.y) + center.y) - config.extent.y * .5f;
        particle.position.z = (GetRandomNumberInRange(config.extent.z) + center.z) - config.extent.z * .5f;
        particle.alpha = 0;
        particle.frame = 0;
        particle.onFrames = forestFireflyTiming[shenGetRandomInt32(4)][0];
        particle.offFrames = forestFireflyTiming[shenGetRandomInt32(4)][1];
        particle.fadeOutFrames = forestFireflyTiming[shenGetRandomInt32(4)][2];
        particle.fadeInFrames = forestFireflyTiming[shenGetRandomInt32(4)][3];
        particle.state = 0;
    }
    for (; i < 500; ++i)
        work.particles[i].position = {};
}
void ForestFireflyLoop(void *data) {
    ForestFireflyWork &work = *static_cast<ForestFireflyWork *>(data);
    Vector3 camera, next, ground;
    camCtrlGetTargetPosition(&camera);
    camCtrlGetNextCamPos(&next);
    camCtrlGetTargetPosition(&ground);
    ground.y = 100;
    ground.y = shenFindIntersectionWithScene(ground, 100);
    EV_AT_SetPos(MQTE_FIRE_DUMMY, EventTransformFlags::AllAxes, {0, 0, 0});
    EulerRotation16 cameraRotation;
    camCtrlGetRot(&cameraRotation);
    const Vector3i rotation{cameraRotation.pitch, cameraRotation.yaw, cameraRotation.roll};
    std::int32_t drawn = 0;
    for (std::int32_t i = 0; i < forestFireflyCount; ++i) {
        ForestFirefly &particle = work.particles[i];
        if (!particle.state) {
            if (particle.angle <= 0) {
                particle.angle = shenGetRandomInt32(0xffff);
                particle.state = shenGetRandomInt32(4) + 1;
            } else
                --particle.angle;
            continue;
        }
        if (static_cast<std::uint32_t>(particle.state - 1) > 3)
            continue;
        ++particle.frame;
        particle.angle = static_cast<std::uint16_t>(particle.angle + 112);
        Vector3 position{sin_fixed16(static_cast<std::int16_t>(particle.angle)) + particle.position.x,
            (sin_fixed16(static_cast<std::int16_t>(2 * particle.angle)) * sin_fixed16(static_cast<std::int16_t>(particle.angle))) * .3f + particle.position.y,
            sin_fixed16(static_cast<std::int16_t>(particle.angle)) + particle.position.z};
        Vector2 screen;
        ProjectWorldToScreen(&position, &screen);
        const float x = next.x - position.x, y = next.y - position.y, z = next.z - position.z;
        (void)std::sqrt(x * x + y * y + z * z);
        const float width = static_cast<float>(d3t::g_originalScreenWidth), height = static_cast<float>(d3t::g_originalScreenHeight);
        screen.x = std::fmin(((width / height) - 1.3333334f) + 1.0f, 1.33f) * ((screen.x / width) * 640);
        screen.y = (screen.y / height) * 480;
        if (!(screen.x >= 0 && screen.x <= 639 && screen.y >= 0 && screen.y <= 479 && drawn < forestFireflyCount))
            continue;
        switch (particle.state) {
        case 1:
            particle.alpha = forestFireflyAmplitude / particle.fadeInFrames + particle.alpha;
            if (particle.frame > particle.fadeInFrames) {
                particle.alpha = forestFireflyAmplitude;
                particle.state = 3;
                particle.frame = 0;
            }
            break;
        case 2:
            particle.alpha -= forestFireflyAmplitude / particle.fadeOutFrames;
            if (particle.frame > particle.fadeOutFrames) {
                particle.alpha = 0;
                particle.state = 4;
                particle.frame = 0;
            }
            break;
        case 3:
            if (particle.frame > particle.onFrames) {
                particle.state = 2;
                particle.frame = 0;
            }
            break;
        case 4:
            if (particle.frame > particle.offFrames) {
                particle.state = 1;
                particle.frame = 0;
            }
            break;
        default:
            break;
        }
        const float size = ((sin_fixed16(static_cast<std::int16_t>(particle.angle)) + 1) * .5f) * forestFireflyAmplitude + forestFireflyScale;
        const Vector3 scale{size, size, size};
        ApplyTranslationRotationScaleXYZToCharaByID(MQTE_FIREFLY, 1, &position, &rotation, &scale);
        ++drawn;
    }
}
void ForestFireflyAuthor(void *) {
    const std::int32_t frame = static_cast<std::int32_t>(EV_authSceneCtrl(AuthSceneCtrl::GetCurrentFrame));
    switch (frame) {
    case 0:
        InitializeForestFireflies(0);
        return;
    case 151:
        InitializeForestFireflies(1);
        return;
    case 481:
        InitializeForestFireflies(2);
        break;
    case 821:
        InitializeForestFireflies(3);
        break;
    case 2486:
        InitializeForestFireflies(4);
        break;
    default:
        if (frame <= 180)
            return;
        break;
    }
    const float alpha = std::fabs(sin_fixed16(static_cast<std::int16_t>(592 * static_cast<std::int16_t>(frame))));
    for (shen_name name : forestAuthorFireflies)
        SetScenarioCharacterTransparency(name, alpha);
}
} // namespace shenmue::s2::areas
