#include "s1/task/cach.hpp"
#include "s1/task/esop.hpp"
#include "s1/task/came.hpp"
#include "s1/task/srch.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/updb.hpp"

#include "s1/boot.hpp"
#include "s1/task/char.hpp"
#include "s1/face.hpp"
#include "s1/face_cache.hpp"
#include "s1/hlib.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/d3t/renderer.hpp"
#include "s1/motion_registry.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/file_loader.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/player_control.hpp"
#include "s1/timing.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/padc.hpp"
#include "s1/task/sond.hpp"
#include "s1/ui_runtime.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <algorithm>
#include <cstdint>
#include <cmath>
#include <cstdio>
#include <limits>
#include <cstring>

namespace shenmue::s1 {

void AssignHNDP();
void HND_SetMatrixPools(Matrix4x4 *localMatrices, Matrix4x4 *worldMatrices, Matrix4x4 *inverseMatrices);
void CACH_Callback(HLib::TaskData *taskData);
extern Character *g_playerCharacter;
void Shenmue_FileRead_ALL(std::int64_t model, char *formattedPath, std::uint32_t readMode, std::int16_t flags);
extern std::uint8_t g_notebookHelpOverrideActive;
extern std::int32_t g_framesPerSecond;
void *g_handNodeRight = nullptr;
void *g_handNodeLeft = nullptr;
void *g_handNodePoolPrimary = nullptr;
void *g_handNodePoolSecondary = nullptr;
void *g_handOptionalMatrixPool = nullptr;
void *g_handCollisionPool = nullptr;
void *g_handGlobalTaskData[2]{};
void *g_handRuntime_18888 = nullptr;
void *g_handRuntime_188a8 = nullptr;
void *g_cameraControl = nullptr;
void *g_camera = nullptr;
void *g_cacheData = nullptr;
void *g_cacheTemporaryCharacter = nullptr;
void *g_shenmueCamera0 = nullptr;
void *g_cameraTargetEntity = nullptr;
void *g_cameraAnchorB = nullptr;
void *g_cameraAnchorA = nullptr;
void *g_specialAssetType = nullptr;
std::uint32_t g_soundCommand = 0;
std::uint32_t g_globalSoundCommand = 0;
std::int32_t g_cameraGlobalState = -1;
std::int32_t g_cameraLock = 0;
Vector3i g_cameraTargetSwayPhase{};
Vector3 g_cameraCurvePosition{};
Vector3 g_temporaryAcceleratedVector{};
HLib::Task *g_cachTask = nullptr;

std::int32_t camCtrlGetState() {
    if (!g_cameraAnchorA)
        return -1;
    const auto *const control = static_cast<const CameraControlState *>(g_cameraControl);
    const auto *const camera = static_cast<const CachContextState *>(g_camera);
    const auto *const cache = static_cast<const CachContextState *>(g_cacheData);
    std::int32_t state = control->mode != 0 ? 1 : 0;
    if (camera->state != 0)
        state |= 2;
    if (cache->state != 0)
        state |= 4;
    return state;
}
static_assert(offsetof(PackModel, model) == 0x08);

void CACH_Callback(HLib::TaskData *) {
    if (!g_cameraAnchorA) {
        return;
    }

    auto *const camera = static_cast<CachContextState *>(g_camera);
    auto *const cache = static_cast<CachContextState *>(g_cacheData);
    auto *const control = static_cast<CameraControlState *>(g_cameraControl);
    if (camera->state) {
        CACH_Related(camera);
    }
    if (cache->state) {
        CACH_Related(cache);
    }

    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive) {
        return;
    }
    if (!control->investigateMode && !camera->observeMode) {
        g_cameraTargetSwayPhase = {};
        return;
    }

    g_cameraTargetSwayPhase.x += 728;
    g_cameraTargetSwayPhase.y += 546;
    g_cameraTargetSwayPhase.z += 364;
    g_cameraTargetSwayPhase.x = static_cast<std::uint16_t>(g_cameraTargetSwayPhase.x);
    Vector3 relativePosition{};
    relativePosition.x = static_cast<float>(
        static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_cameraTargetSwayPhase.x))) * 0.005 + static_cast<double>(g_cameraCurvePosition.x));
    g_cameraTargetSwayPhase.y = static_cast<std::uint16_t>(g_cameraTargetSwayPhase.y);
    relativePosition.y = static_cast<float>(
        static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_cameraTargetSwayPhase.y))) * 0.005 + static_cast<double>(g_cameraCurvePosition.y));
    g_cameraTargetSwayPhase.z = static_cast<std::uint16_t>(g_cameraTargetSwayPhase.z);
    relativePosition.z = static_cast<float>(
        static_cast<double>(sin_fixed16(static_cast<std::int16_t>(g_cameraTargetSwayPhase.z))) * 0.005 + static_cast<double>(g_cameraCurvePosition.z));
    TranslateCameraRelative(relativePosition);

    if (!g_cameraLock) {
        g_CameraTargetPos = g_temporaryAcceleratedVector;
        shenApplyTranslationToAll(g_cameraTargetNode, g_CameraTargetPos);
    }
    if (control->investigateMode < 0) {
        control->investigateMode = 0;
    }
    if (camera->observeMode < 0) {
        camera->observeMode = 0;
    }
}

void CACH_Related(CachContextState *cameraContext) {
    if (!cameraContext || !g_updateRenderFrameMode)
        return;

    for (const CachAttachmentRecord &attachment : cameraContext->attachments) {
        const std::uintptr_t ownerAddress = reinterpret_cast<std::uintptr_t>(attachment.modelOwner);
        if ((ownerAddress & ~std::uintptr_t{0xf}) == 0)
            continue;

        void *const hmdl = attachment.modelOwner->model;
        if (!hmdl)
            continue;

        SubmitRiggedSceneAttachment(hmdl, cameraContext->attachmentOpacity, cameraContext->attachmentRootPosition, cameraContext->attachmentRootRotation,
            attachment.position, attachment.rotation);
    }
}

void *allocateHandPoolFromBack(std::uint64_t size, std::uint32_t token) {
    HLib::TaskData *block = g_pLastTaskData;
    while (block && (block->status == 0x20455355 || static_cast<std::uint64_t>(block->content_size) < size)) {
        block = block->prev_ptr;
    }
    if (!block) {
        return nullptr;
    }

    const std::uint64_t oldSize = static_cast<std::uint64_t>(block->content_size);
    if (oldSize > size + 0x40) {
        const std::uint64_t remainingSize = oldSize - size;
        auto *const allocated = reinterpret_cast<HLib::TaskData *>(reinterpret_cast<std::byte *>(block) + remainingSize);
        allocated->prev_ptr = block;
        allocated->next_ptr = block->next_ptr;
        block->next_ptr->prev_ptr = allocated;
        block->next_ptr = allocated;
        block->content_size = static_cast<std::int64_t>(remainingSize);
        allocated->taskToken = static_cast<std::int32_t>(token);
        allocated->content_size = static_cast<std::int64_t>(size);
        allocated->status = 0x20455355;
        allocated->alignment = 4;
        return allocated + 1;
    }

    block->taskToken = static_cast<std::int32_t>(token);
    block->status = 0x20455355;
    block->alignment = 4;
    return block + 1;
}

void CACH_Enqueue() {
    g_cameraControl = HLib_AllocHeapMemBlock(0x2c0, SHEN_NAME(CACH));
    g_camera = HLib_AllocHeapMemBlock(0x300, SHEN_NAME(CACH));
    g_cacheData = HLib_AllocHeapMemBlock(0x300, SHEN_NAME(CACH));
    g_cacheTemporaryCharacter = nullptr;
    g_shenmueCamera0 = nullptr;
    g_cameraTargetEntity = nullptr;
    g_cameraAnchorB = nullptr;
    g_cameraAnchorA = nullptr;
    g_specialAssetType = nullptr;
    g_soundCommand = 0;
    g_globalSoundCommand = 0;
    g_cameraGlobalState = -1;
    g_cachTask = HLib_EnqueueTaskWithoutParameter(CACH_Callback, 8, 0, SHEN_NAME(CACH));
}

void CACH_Shutdown() {
    g_cameraAnchorA = nullptr;
    g_cameraAnchorB = nullptr;
    g_cameraTargetEntity = nullptr;
    g_cacheTemporaryCharacter = nullptr;
    g_shenmueCamera0 = nullptr;
    g_specialAssetType = nullptr;
    g_soundCommand = 0;
    g_globalSoundCommand = 0;
    g_cameraGlobalState = -1;
    g_cameraTargetSwayPhase = {};

    UPDB_Shutdown();
    if (g_cachTask)
        HLib_DestroyTask(g_cachTask);
    g_cachTask = nullptr;

    if (g_cacheData) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_cacheData), 0);
    }
    if (g_camera) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_camera), 0);
    }
    if (g_cameraControl) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_cameraControl), 0);
    }
    g_cacheData = nullptr;
    g_camera = nullptr;
    g_cameraControl = nullptr;

    HND_SetMatrixPools(nullptr, nullptr, nullptr);
    if (g_handCollisionPool) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_handCollisionPool), 0);
    }
    if (g_handOptionalMatrixPool) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_handOptionalMatrixPool), 0);
    }
    if (g_handNodePoolPrimary) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_handNodePoolPrimary), 0);
    }
    if (g_handNodePoolSecondary) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(g_handNodePoolSecondary), 0);
    }
    g_handCollisionPool = nullptr;
    g_handOptionalMatrixPool = nullptr;
    g_handNodePoolPrimary = nullptr;
    g_handNodePoolSecondary = nullptr;
    g_handNodeRight = nullptr;
    g_handNodeLeft = nullptr;
    g_handGlobalTaskData[0] = nullptr;
    g_handGlobalTaskData[1] = nullptr;
    g_handRuntime_18888 = nullptr;
    g_handRuntime_188a8 = nullptr;
}

HLib::Task *CACH_GetTask() {
    return g_cachTask;
}

namespace {

float g_quadraticCurveRemaining{};
float g_quadraticCurveProgress{};
struct CachQuadraticCurve {
    Vector3 acceleration{};
    Vector3 velocity{};
    Vector3 position{};
};

CachQuadraticCurve g_cachCurveStart{};
CachQuadraticCurve g_cachCurveEnd{};
CachQuadraticCurve g_cachCurveAnchorA{};
CachQuadraticCurve g_cachCurveAnchorB{};


Vector3i g_cachArmRotationA{};
Vector3i g_cachArmRotationB{};
Vector3i g_cachHandRotationA{};
Vector3i g_cachHandRotationB{};



void FitCachQuadraticCurve(const Vector3 &sampleTimes, CachQuadraticCurve &curve) {
    const float firstTime = sampleTimes.x;
    const float middleTime = sampleTimes.y;
    const float lastTime = sampleTimes.z;
    const float middleSquared = middleTime * middleTime;
    const float firstMinusMiddle = firstTime - middleTime;
    const float middleMinusLast = middleTime - lastTime;
    const float firstSquaredMinusMiddleSquared = firstTime * firstTime - middleSquared;
    const float denominator = middleMinusLast * firstSquaredMinusMiddleSquared - (middleSquared - lastTime * lastTime) * firstMinusMiddle;

    const auto fitAxis = [firstTime, firstMinusMiddle, middleMinusLast, firstSquaredMinusMiddleSquared, denominator](
                             float &acceleration, float &velocity, float &position) {
        const float firstValue = acceleration;
        const float middleValue = velocity;
        const float lastValue = position;
        const float fittedAcceleration = ((firstValue - middleValue) * middleMinusLast - (middleValue - lastValue) * firstMinusMiddle) / denominator;
        acceleration = fittedAcceleration;
        const float fittedVelocity = ((firstValue - middleValue) - fittedAcceleration * firstSquaredMinusMiddleSquared) / firstMinusMiddle;
        velocity = fittedVelocity;
        position = firstValue - fittedAcceleration * firstTime * firstTime - fittedVelocity * firstTime;
    };

    fitAxis(curve.acceleration.x, curve.velocity.x, curve.position.x);
    fitAxis(curve.acceleration.y, curve.velocity.y, curve.position.y);
    fitAxis(curve.acceleration.z, curve.velocity.z, curve.position.z);
}


void camCtrlSetAnchorNodeTo(const std::int8_t *pose, float blendFrames, Character *target) {
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    anchorA.gripTarget = target;
    if (!target)
        std::memset(anchorA.fingerBlendLimits, 0, sizeof(anchorA.fingerBlendLimits));
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    anchorB.gripTarget = target;
    if (!target)
        std::memset(anchorB.fingerBlendLimits, 0, sizeof(anchorB.fingerBlendLimits));
    HND_SetEffectFlags(anchorA, pose, blendFrames);
    HND_SetEffectFlags(anchorB, pose, blendFrames);
}

void ResetCachInteractionCurve(float duration) {
    g_quadraticCurveRemaining = duration;
    g_quadraticCurveProgress = 0.0f;
}



bool AdvanceQuadraticCurve(float targetProgress, float stepSize) {
    const float initialRemaining = g_quadraticCurveRemaining;
    if ((targetProgress * targetProgress / stepSize) * 0.5f <= g_quadraticCurveRemaining) {
        g_quadraticCurveProgress = std::min(
            targetProgress, g_quadraticCurveProgress + stepSize);
    } else {
        g_quadraticCurveProgress = std::max(stepSize, g_quadraticCurveProgress - stepSize);
    }
    g_quadraticCurveRemaining -= g_quadraticCurveProgress;
    if (initialRemaining - g_quadraticCurveProgress > 0.0f)
        return false;
    g_quadraticCurveRemaining = 0.0f;
    return true;
}

void ClearCachAttachments(CachContextState &context, bool release) {
    for (CachAttachmentRecord &attachment : context.attachments) {
        const auto owner = reinterpret_cast<std::uintptr_t>(attachment.modelOwner);
        if (release && (owner & ~std::uintptr_t{0xf}) != 0) {
            FreePVR(attachment.modelOwner);
        }
        attachment.modelOwner = nullptr;
    }
}

template <typename T> T &CachField(void *state, std::size_t offset) {
    return *reinterpret_cast<T *>(static_cast<std::byte *>(state) + offset);
}

template <typename T> const T &CachField(const void *state, std::size_t offset) {
    return *reinterpret_cast<const T *>(static_cast<const std::byte *>(state) + offset);
}

void UpdateHandMotData() {
    constexpr Vector3 kHiddenPosition{100000.0f, 100000.0f, 100000.0f};
    auto *const entity = static_cast<Character *>(g_cameraTargetEntity);
    auto *const cameraEntity = static_cast<Character *>(g_shenmueCamera0);

    const auto hideCharacter = [](Character *character) {
        if (!character)
            return;
        const bool wasVisible = (character->motionFlags & 4U) != 0;
        character->motionFlags &= static_cast<std::uint8_t>(~4U);
        if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0 && wasVisible) {
            CHAR_LogNPCVisibility(character, false);
        }
    };
    const auto resetAnchor = [](void *anchor) {
        auto *const hand = static_cast<HandModelWork *>(anchor);
        if (!hand)
            return;
        if (hand->primaryRoot) {
            hand->modelSelector = (hand->modelSelector & ~std::int64_t{0xffff}) | 0x101;
            HND_SetNewData(*hand, 0, 0, 0, 0, 1, 1.0f, 0);
            HND_ApplyRootEuler(*hand, {});
        }
    };




    if (entity) {
        entity->entity.node.child = nullptr;
        shenApplyTranslationToAll(entity->entity.node, kHiddenPosition);
        hideCharacter(entity);
    }
    if (cameraEntity) {
        cameraEntity->entity.node.child = nullptr;
        shenApplyTranslationToAll(cameraEntity->entity.node, kHiddenPosition);
        hideCharacter(cameraEntity);
    }
    resetAnchor(g_cameraAnchorA);
    resetAnchor(g_cameraAnchorB);
}


void ApplyPosAndEulerAnglesForNodes() {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    control.chara->entity.node.child = control.savedLink;
    if (control.chara)
        shenApplyTranslationToAll(control.chara->entity.node, control.savedPosition);
    if (control.chara)
        ApplyEulerAnglesToNode(control.chara->entity.node, control.savedRotation);
}



Character *SelectAutomaticCachTarget() {
    if (!g_mapControlTask || !g_mapControlTask->task_data_ptr)
        return nullptr;
    auto *const map = reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr);
    Character *selected = nullptr;
    float maxFarDistance = 10000000.0f;
    Matrix4x4_Push();
    for (void *bucket : map->characterBuckets) {
        for (auto *node = static_cast<CharacterBucketNode *>(bucket); node; node = node->next) {
            Character *const candidate = node->character;
            if (!candidate || candidate->entity.name != SHEN_NAME(CEWP) ||
                ((candidate->flags.value & 1U) == 0 && ((candidate->flags.value & 2U) == 0 || (candidate->motionFlags & 1U) == 0)) ||
                HLib_GetCharacterComponent(candidate, SHEN_NAME(MOTM))) {
                continue;
            }
            const Vector3 world = GetPositionForEntity(candidate->entity.node);
            Matrix4x4_RotateAndPush();
            Vector2 screen{};
            WorldToScreen(world, screen);
            const auto xLimit = static_cast<float>(static_cast<float>(g_originalScreenWidth) * 0.0625);
            const float x = screen.x - static_cast<float>(g_originalScreenWidth) * 0.5f;
            const auto yLimit = static_cast<float>(static_cast<float>(g_originalScreenHeight) * 0.0625);
            const float y = screen.y - static_cast<float>(g_originalScreenHeight) * 0.5f;
            const float distance = x <= -xLimit || xLimit <= x || y <= -yLimit || yLimit <= y ? maxFarDistance + 1.0f : y * y + x * x;
            if (maxFarDistance > distance) {
                maxFarDistance = distance;
                selected = candidate;
            }
        }
    }
    Matrix4x4::Pop(1);
    return selected;
}

void CacheActiveInteraction() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &cached = *static_cast<CachContextState *>(g_cacheData);
    const auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    if (camera.state == 4 && cached.state == 0 && control.mode == 0 && g_cameraGlobalState < 0) {
        std::memcpy(&cached, &camera, 0x2b0);
        camera.state = 0;
        camera.chara = nullptr;
        camera.observeMode = 0;
        ClearCachAttachments(camera, false);
    }
}



float CameraInputDelta(float oldValue, std::int32_t newValue) {
    float delta = static_cast<float>(newValue) - oldValue;
    if (delta > 3.0f || -3.0f > delta)
        return static_cast<float>(static_cast<double>(delta) * 0.03333);
    if (static_cast<double>(delta) > 0.1)
        delta = 0.1f;
    if (-0.1 > static_cast<double>(delta))
        return -0.1f;
    return delta;
}

void NormalizeEulerSigned16(Vector3i &angles) {
    angles.x = static_cast<std::int16_t>(angles.x);
    angles.y = static_cast<std::int16_t>(angles.y);
    angles.z = static_cast<std::int16_t>(angles.z);
}


void ConvertTargetSpeedToEuler(const CameraControlState &control, Vector3i &output) {
    const float normalized = static_cast<float>(static_cast<double>(control.speedCurrent) * 0.01);
    switch (control.inputMode) {
    case 2:
    case 3:
        output.x = static_cast<std::int32_t>(normalized * 5461.0f - 2730.0f);
        output.z = static_cast<std::int32_t>(normalized * 25485.0f - 1820.0f);
        break;
    case 4:
        output.x = static_cast<std::int32_t>(normalized * 25486.0f - 12743.0f);
        output.z = static_cast<std::int32_t>(normalized * 5461.0f - 7281.0f);
        break;
    case 5:
    case 9:
        output.x = static_cast<std::int32_t>(normalized * 25486.0f);
        output.z = 0;
        break;
    case 8:
        output.x = static_cast<std::int32_t>(normalized * 25486.0f - 12743.0f);
        output.z = 0;
        break;
    default:
        output.x = static_cast<std::int32_t>(normalized * 25486.0f - 12743.0f);
        output.z = static_cast<std::int32_t>(normalized * 5461.0f - 2730.0f);
        break;
    }
    output.x = static_cast<std::int16_t>(output.x);
    output.y = 0;
    output.z = static_cast<std::int16_t>(output.z);
}

void ConfigureTargetModeCamera(CameraControlState &control, const Vector3 &eye, const Vector3 &target, std::int32_t baseFieldOfView) {
    constexpr double kInputRotationScale = 0.005208;
    const Vector3 &input = CachField<const Vector3>(&control, 0xbc);
    const float yawSin = CachField<const float>(&control, 0x238);
    const float yawCos = CachField<const float>(&control, 0x23c);

    const float horizontal = static_cast<float>(static_cast<double>(CachField<const float>(&control, 0x128) * input.x) * kInputRotationScale);
    const float vertical = static_cast<float>(static_cast<double>(-(input.y * CachField<const float>(&control, 0x12c))) * kInputRotationScale);
    const float depth = static_cast<float>(static_cast<double>(input.y * CachField<const float>(&control, 0x130)) * kInputRotationScale);

    g_temporaryAcceleratedVector = {target.x + yawCos * horizontal - yawSin * depth, target.y + vertical, target.z + yawSin * horizontal + yawCos * depth};

    const float halfHorizontal = static_cast<float>(static_cast<double>(horizontal) * 0.5);
    const float halfVertical = static_cast<float>(static_cast<double>(vertical) * 0.5);
    const float halfDepth = static_cast<float>(static_cast<double>(depth) * 0.5);
    g_cameraCurvePosition = {eye.x + yawCos * halfHorizontal - yawSin * halfDepth, eye.y + halfVertical, eye.z + yawSin * halfHorizontal + yawCos * halfDepth};

    const auto fieldOfView = static_cast<std::int32_t>((static_cast<double>(input.z) * 0.0078125 + 1.0) * static_cast<double>(baseFieldOfView));
    CAME_SetFieldOfView(fieldOfView);
}

void BuildTargetTransitionCurveA(CameraControlState &control) {
    ResetCachInteractionCurve(60.0f);
    g_cachCurveAnchorA.acceleration = CachField<const Vector3>(&control, 0xc8);
    g_cachCurveAnchorA.position = CachField<const Vector3>(&control, 0xec);
    g_cachCurveStart.acceleration = CachField<const Vector3>(&control, 0xb0);
    g_cachCurveStart.position = g_cameraCurvePosition;

    const Vector3 &destination = CachField<const Vector3>(&control, 0x1e8);
    const auto blendDouble = [](float from, float to, double amount) {
        return static_cast<float>(static_cast<double>(to - from) * amount + static_cast<double>(from));
    };
    const bool alternate = (g_cameraStateFlags & 0x10U) != 0;
    if (alternate) {
        g_cachCurveAnchorA.velocity.x = blendDouble(g_cachCurveAnchorA.acceleration.x, destination.x, 0.7);
        g_cachCurveAnchorA.velocity.y = static_cast<float>(static_cast<double>(g_cachCurveAnchorA.acceleration.y) + 0.12);
        g_cachCurveAnchorA.velocity.z = blendDouble(g_cachCurveAnchorA.acceleration.z, destination.z, 0.7);
        g_cachCurveStart.velocity.x = blendDouble(g_cachCurveStart.acceleration.x, g_cachCurveStart.position.x, 0.7);
        g_cachCurveStart.velocity.y = blendDouble(g_cachCurveStart.acceleration.y, g_cachCurveStart.position.y, 0.3);
        g_cachCurveStart.velocity.z = blendDouble(g_cachCurveStart.acceleration.z, g_cachCurveStart.position.z, 0.7);
    } else {
        g_cachCurveAnchorA.velocity.x = blendDouble(g_cachCurveAnchorA.acceleration.x, destination.x, 0.3);
        g_cachCurveAnchorA.velocity.y = blendDouble(g_cachCurveAnchorA.acceleration.y, destination.y, 0.7);
        g_cachCurveAnchorA.velocity.z = blendDouble(g_cachCurveAnchorA.acceleration.z, destination.z, 0.3);
        g_cachCurveStart.velocity.x = blendDouble(g_cachCurveStart.acceleration.x, g_cachCurveStart.position.x, 0.3);
        g_cachCurveStart.velocity.y = blendDouble(g_cachCurveStart.acceleration.y, g_cachCurveStart.position.y, 0.7);
        g_cachCurveStart.velocity.z = blendDouble(g_cachCurveStart.acceleration.z, g_cachCurveStart.position.z, 0.3);
    }

    FitCachQuadraticCurve({0.0f, alternate ? 27.0f : 21.0f, 60.0f}, g_cachCurveStart);
    FitCachQuadraticCurve({0.0f, kSeqFramesPerSecond, 60.0f}, g_cachCurveAnchorA);
}

void BuildTargetTransitionCurveB(CameraControlState &control) {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    Vector3 &eye = CachField<Vector3>(&control, 0x80);
    Vector3 &target = CachField<Vector3>(&control, 0x8c);
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_quadraticCurveRemaining = 40.0f;

    if (camera.state == 0) {
        SRCH_UpdateCameraEyeAndTarget(eye, target);
    } else {
        eye = camera.lastCurvePosition;
        target = camera.lastAcceleratedTarget;
        CachField<std::int32_t>(&control, 0x248) = camera.examineFov;
        camera.analogDelta = {};
    }

    const float lateral = (g_cameraStateFlags & 0x1000U) != 0 ? -0.050000001f : 0.050000001f;
    float depth = 0.30000001f;
    float height = static_cast<float>(static_cast<double>(std::fmin(g_cachCurveStart.position.y, eye.y)) - 0.3);
    if ((g_cameraStateFlags & 2U) != 0) {
        depth = 0.20000002f;
        height = static_cast<float>(static_cast<double>(height) + 0.1);
    }

    g_cachCurveStart.acceleration = eye;
    g_cachCurveStart.velocity = {
        (g_cachCurveStart.position.x - eye.x) * 0.30000001f + eye.x,
        (g_cachCurveStart.position.y - eye.y) * 0.30000001f + eye.y,
        (g_cachCurveStart.position.z - eye.z) * 0.30000001f + eye.z,
    };
    FitCachQuadraticCurve({0.0f, 26.0f, 40.0f}, g_cachCurveStart);

    g_cachCurveEnd.acceleration = target;
    g_cachCurveEnd.velocity = {
        (g_cachCurveEnd.position.x - target.x) * 0.30000001f + target.x,
        (g_cachCurveEnd.position.y - target.y) * 0.30000001f + target.y,
        (g_cachCurveEnd.position.z - target.z) * 0.30000001f + target.z,
    };
    FitCachQuadraticCurve({0.0f, 26.0f, 40.0f}, g_cachCurveEnd);

    const float yawSin = CachField<const float>(&control, 0x238);
    const float yawCos = CachField<const float>(&control, 0x23c);
    g_cachCurveAnchorA.acceleration = {
        lateral * yawCos + eye.x - depth * yawSin,
        (height - eye.y) + eye.y,
        lateral * yawSin + eye.z + depth * yawCos,
    };
    g_cachCurveAnchorA.position = CachField<const Vector3>(&control, 0xec);
    g_cachCurveAnchorA.velocity = {
        (g_cachCurveAnchorA.position.x - g_cachCurveAnchorA.acceleration.x) * 0.40000001f + g_cachCurveAnchorA.acceleration.x,
        (g_cachCurveAnchorA.position.y - g_cachCurveAnchorA.acceleration.y) * 0.40000001f + g_cachCurveAnchorA.acceleration.y,
        (g_cachCurveAnchorA.position.z - g_cachCurveAnchorA.acceleration.z) * 0.40000001f + g_cachCurveAnchorA.acceleration.z,
    };
    FitCachQuadraticCurve({0.0f, 20.0f, 40.0f}, g_cachCurveAnchorA);
}



bool SettleTargetModeInput(CameraControlState &control) {
    Vector3 &input = control.analogDelta;
    const auto resting = [](float value) { return 0.0001 > static_cast<double>(value) && !(static_cast<double>(value) <= -0.0001); };
    const bool settled = resting(input.x) && resting(input.y) && resting(input.z);
    const auto settle = [](float value) {
        const float remaining = -value;
        float step;
        if (remaining > 10.0f || -10.0f > remaining) {
            step = static_cast<float>(static_cast<double>(remaining) * 0.1);
        } else {
            step = 1.0f < remaining ? 1.0f : remaining;
            if (-1.0f > step)
                step = -1.0f;
        }
        return value + step;
    };
    if (settled) {
        input = {};
    } else {
        input.x = settle(input.x);
        input.y = settle(input.y);
        input.z = settle(input.z);
    }
    const auto hold = [](float base, float delta) { return static_cast<float>(static_cast<double>(delta) * 0.7 + static_cast<double>(base)); };
    const Vector3 target{hold(control.gripOffset[0].x, control.gripOffset[1].x), hold(control.gripOffset[0].y, control.gripOffset[1].y),
        hold(control.gripOffset[0].z, control.gripOffset[1].z)};
    ConfigureTargetModeCamera(control, control.eye, target, control.targetFov);
    return settled;
}

void InitializeTargetTransitionSpeed(CameraControlState &control) {
    switch (control.inputMode) {
    case 2:
    case 3:
        control.speedCurrent = 20.0f;
        break;
    case 4:
        control.speedCurrent = 70.0f;
        break;
    case 5:
    case 8:
    case 9:
        control.speedCurrent = 50.0f;
        break;
    case 6:
    case 7:
        control.speedCurrent = 60.0f;
        break;
    default:
        control.speedCurrent = 40.0f;
        break;
    }
    control.speedVelocity = 0.0f;
}

std::int32_t CameraSpeedPercentage(const CameraControlState &control) {
    const std::int32_t current = static_cast<std::int32_t>(control.speedCurrent);
    const std::int32_t minimum = static_cast<std::int32_t>(control.speedMinimum);
    const std::int32_t maximum = static_cast<std::int32_t>(control.speedMaximum);
    return 100 * (current - minimum) / (maximum - minimum);
}


void ApplyInteractionResult(CachContextState &context) {
    auto *const primary = static_cast<Mt5RuntimeNode *>(context.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(context.secondaryTransform);
    switch (context.interactionType) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 40:
        if (context.chara)
            shenApplyTranslationToAll(context.chara->entity.node, context.appliedPosition);
        break;
    case 10:
    case 11:
        if (primary)
            primary->position.x = context.appliedPosition.x;
        if (secondary)
            secondary->position.x = context.secondaryTranslationScalar;
        break;
    case 20:
    case 21:
        if (primary)
            std::memcpy(&primary->rotation.y, &context.target.y, sizeof(std::int32_t));
        if (secondary)
            std::memcpy(&secondary->rotation.y, &context.target.z, sizeof(std::int32_t));
        break;
    case 30:
        primary->position = context.appliedPosition;
        break;
    case 50:
        if (context.chara) {
            shenApplyTranslationToAll(context.chara->entity.node, context.appliedPosition);
            Vector3i rotation = GetEulerAnglesForEntity(context.chara->entity.node);
            std::memcpy(&rotation.x, &context.target.y, sizeof(rotation.x));
            ApplyEulerAnglesToNode(context.chara->entity.node, rotation);
        }
        break;
    default:
        break;
    }
}

void PlayCachQueuedSound(std::uint32_t &command) {
    if (!command)
        return;
    shenExecuteAICACommand(command, 0, 0);
    command = 0;
}



void shenStartObserveMode() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    camera.gameState = g_temporaryGameState;
    camera.previousAuthorState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
    camera.previousEye = g_CameraEyePos;
    camera.previousTarget = g_CameraTargetPos;
    g_temporaryGameState = 0;
    ApplyPendingGameState(0);
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);
    g_currentHelpIconState = 0;
    camera.observeMode = 1;
    g_cameraCurvePosition = camera.previousEye;
    g_temporaryAcceleratedVector = camera.previousTarget;
    camera.previousHelpTokenId = g_defaultHelpTokenId;
    camera.previousButtonId = static_cast<std::int8_t>(g_helpButtonId);
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = 29;
    g_currentHelpTokenId = 29;
    g_helpButtonId = 0;
}

Vector3 EvaluateCachCurve(const CachQuadraticCurve &curve, float t) {
    const float tt = t * t;
    return {curve.velocity.x * t + curve.acceleration.x * tt + curve.position.x, curve.velocity.y * t + curve.acceleration.y * tt + curve.position.y,
        curve.velocity.z * t + curve.acceleration.z * tt + curve.position.z};
}


void camCtrlGetCurveEyeAndTarget(Vector3 *eye, Vector3 *target) {
    if (eye)
        *eye = g_cameraCurvePosition;
    if (target)
        *target = g_temporaryAcceleratedVector;
}



void Matrix4x4_ComposeFromRotation(Vector3i &angles) {
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    angles.x = static_cast<std::int16_t>(angles.x);
    angles.y = static_cast<std::int16_t>(angles.y);
    angles.z = static_cast<std::int16_t>(angles.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(angles.y));
    Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(angles.z));
    Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(angles.x));
    Matrix4x4_ExtractEulerAngles(angles);
    Matrix4x4::Pop();
}



void Vector3_RotateVectorWithinLimits(const Vector3i &reference, Vector3i &angles) {
    const std::int32_t x = angles.x;
    const std::int32_t y = angles.y;
    const std::int32_t z = angles.z;
    const std::int32_t direct = (fixed16_angle_magnitude(x - reference.x) > 0x4000) + (fixed16_angle_magnitude(y - reference.y) > 0x4000) +
        (fixed16_angle_magnitude(z - reference.z) > 0x4000);
    const std::int32_t flipped = (fixed16_angle_magnitude(x - reference.x + 0x8000) > 0x4000) + (fixed16_angle_magnitude(0x8000 - reference.y - y) > 0x4000) +
        (fixed16_angle_magnitude(z - reference.z + 0x8000) > 0x4000);
    if (direct > flipped) {
        angles.x = static_cast<std::int16_t>(x - 0x8000);
        angles.y = static_cast<std::int16_t>(0x8000 - y);
        angles.z = static_cast<std::int16_t>(z - 0x8000);
    }
}


void BlendEulerAngles(Vector3i &angles, const Vector3i &target, float amount) {
    const auto x = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(target.x - angles.x)) * amount + static_cast<float>(angles.x));
    angles.x = x;
    const auto y = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(target.y - angles.y)) * amount + static_cast<float>(angles.y));
    angles.y = y;
    const auto z = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(target.z - angles.z)) * amount + static_cast<float>(angles.z));
    angles.x = static_cast<std::int16_t>(x);
    angles.y = static_cast<std::int16_t>(y);
    angles.z = static_cast<std::int16_t>(z);
}


void ApplyPositionDeltaToEntity(Character *character, const Vector3 &delta) {
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    if (character)
        shenApplyTranslationToAll(character->entity.node, {position.x + delta.x, position.y + delta.y, position.z + delta.z});
}



void ApplyRelativePositionToEntity(Character *character, const HandModelWork &anchor, const Vector3 &target) {
    Vector3 handPoint{};
    std::memcpy(&handPoint, anchor.jointPositions + 60 * sizeof(Vector3), sizeof(handPoint));
    const float dz = target.z - handPoint.z;
    const float dx = target.x - handPoint.x;
    const float dy = target.y - handPoint.y;
    const Vector3 position = character ? GetPositionForEntity(character->entity.node) : Vector3{};
    if (character)
        shenApplyTranslationToAll(character->entity.node, {position.x + dx, position.y + dy, position.z + dz});
}



void ResolveCompositeTransformEuler(Character *arm, HandModelWork &anchor, Vector3i &handRotation, Vector3i &composite) {
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(composite.y, composite.x, composite.z);
    Matrix4x4 rotation = workTransformMatrix;
    HND_ApplyRootEuler(anchor, {});
    ApplyHandRootTransform(arm, anchor);
    Matrix4x4::InvertInPlace(nullptr);
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::Multiply(&rotation);
    Matrix4x4_ExtractEulerAngles(composite);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::SetViewMatrixFromEuler(handRotation.y, handRotation.x, handRotation.z);
    rotation = workTransformMatrix;
    Matrix4x4::Pop();
    Matrix4x4::Multiply(&rotation);
    Matrix4x4_ExtractEulerAngles(handRotation);
    HND_ApplyRootEuler(anchor, handRotation);
}



void camCtrlUpdateMatrixStack() {
    matrix_stack::Matrix4x4_Push();
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    HND_ApplyRootEuler(anchorA, g_cachHandRotationA);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    ApplyHandRootTransform(armA, anchorA);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    Matrix4x4 handFrame = workTransformMatrix;
    HND_ApplyRootEuler(anchorA, {});
    ApplyHandRootTransform(armA, anchorA);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::Multiply(&handFrame);
    Matrix4x4_ExtractEulerAngles(g_cachHandRotationA);
    HND_ApplyRootEuler(anchorA, g_cachHandRotationA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    HND_ApplyRootEuler(anchorB, g_cachHandRotationB);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    ApplyHandRootTransform(armB, anchorB);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    handFrame = workTransformMatrix;
    HND_ApplyRootEuler(anchorB, {});
    ApplyHandRootTransform(armB, anchorB);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::Multiply(&handFrame);
    Matrix4x4_ExtractEulerAngles(g_cachHandRotationB);
    HND_ApplyRootEuler(anchorB, g_cachHandRotationB);
    Matrix4x4::Pop();
}



void UpdateEntityPositionRotation(Character *target) {
    CharacterNode *const parent = target ? &target->entity.node : nullptr;
    if (auto *const armA = static_cast<Character *>(g_cameraTargetEntity)) {
        const Vector3 position = GetPositionForEntity(armA->entity.node);
        armA->entity.node.child = parent;
        shenApplyTranslationToAll(armA->entity.node, position);
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    }
    if (auto *const armB = static_cast<Character *>(g_shenmueCamera0)) {
        const Vector3 position = GetPositionForEntity(armB->entity.node);
        armB->entity.node.child = parent;
        shenApplyTranslationToAll(armB->entity.node, position);
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    }
}


void SyncCurrentPositionAndRotationFromCamera() {
    UpdateEntityPositionRotation(static_cast<CachContextState *>(g_camera)->chara);
    g_cachHandRotationA = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA));
    g_cachHandRotationB = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB));
}



void shenEndObserveMode() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_bIsConversationCamInactive = 0;
    g_temporaryGameState = camera.gameState;
    ApplyPendingGameState(camera.gameState);
    const std::int32_t previousAuthorState = camera.previousAuthorState;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = previousAuthorState;
    ApplyPendingGameState(camera.gameState);
    g_currentHelpIconState = 0;
    camera.observeMode = -1;
    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t token = static_cast<std::int8_t>(camera.previousHelpTokenId) >= 68 ? 0 : camera.previousHelpTokenId;
    g_helpButtonId = static_cast<std::uint8_t>(camera.previousButtonId);
    g_defaultHelpTokenId = token;
    g_currentHelpTokenId = token;
}




void camCtrlUpdateAcceleration() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const std::int32_t type = camera.interactionType;
    float vertical = 0.0f;
    float depth = 0.0f;
    if (type >= 0 && (type <= 3 || type == 30)) {
        depth = static_cast<float>(static_cast<double>(camera.halfExtents.z * camera.analogDelta.y) * 0.005208);
    } else if (type == 60) {
        vertical = static_cast<float>(static_cast<double>(-(camera.analogDelta.y * camera.halfExtents.y)) * 0.005208);
        depth = static_cast<float>(static_cast<double>(camera.analogDelta.y * camera.halfExtents.z) * 0.005208);
    } else {
        vertical = static_cast<float>(static_cast<double>(-(camera.halfExtents.y * camera.analogDelta.y)) * 0.005208);
    }
    const float cosine = camera.yawCosine;
    const float sine = camera.yawSine;
    const auto lateral = static_cast<float>(static_cast<double>(camera.halfExtents.x * camera.analogDelta.x) * 0.005208);
    const Vector3 target{lateral * cosine + camera.lastAcceleratedTarget.x - sine * depth, vertical + camera.lastAcceleratedTarget.y,
        lateral * sine + camera.lastAcceleratedTarget.z + cosine * depth};
    const auto eyeDepth = static_cast<float>(static_cast<double>(depth) * 0.6666);
    const auto eyeLateral = static_cast<float>(static_cast<double>(lateral) * 0.6666);
    const Vector3 eye{cosine * eyeLateral + camera.lastCurvePosition.x - sine * eyeDepth,
        static_cast<float>(static_cast<double>(vertical) * 0.6666) + camera.lastCurvePosition.y,
        sine * eyeLateral + camera.lastCurvePosition.z + cosine * eyeDepth};
    const auto fov = static_cast<std::int32_t>((static_cast<double>(camera.analogDelta.z) * 0.0078125 + 1.0) * static_cast<double>(camera.examineFov));
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    CAME_SetFieldOfView(fov);
}



bool camCtrlUpdateAccelTerm() {
    Vector3 &delta = static_cast<CachContextState *>(g_camera)->analogDelta;
    const auto settled = [](float value) { return static_cast<double>(value) < 0.0001 && static_cast<double>(value) > -0.0001; };
    if (settled(delta.x) && settled(delta.y) && settled(delta.z)) {
        delta = {};
        camCtrlUpdateAcceleration();
        return true;
    }
    const auto easeStep = [](float value) {
        const float toward = -value;
        if (toward > 10.0f || toward < -10.0f)
            return static_cast<float>(static_cast<double>(toward) * 0.1);
        const float step = std::min(1.0f, toward);
        return step < -1.0f ? -1.0f : step;
    };
    delta.x += easeStep(delta.x);
    delta.y += easeStep(delta.y);
    delta.z += easeStep(delta.z);
    camCtrlUpdateAcceleration();
    return false;
}

// The examination shows one hand arm: the first while camera flag 0x2000 is set, else the second.
void ShowExaminationArm(Character *armA, Character *armB) {
    Character *const shown = (g_cameraStateFlags & 0x2000U) != 0 ? armA : armB;
    if (!shown)
        return;
    const bool wasVisible = (shown->motionFlags & 4U) != 0;
    shown->motionFlags |= 4U;
    if (g_bDebugLogVisibility && (shown->flags.value & 0x400U) != 0 && !wasVisible)
        CHAR_LogNPCVisibility(shown, true);
}




void ApplyFullEntityAndCameraTransform() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armA)
        armA->entity.node.child = nullptr;
    if (armB)
        armB->entity.node.child = nullptr;
    ShowExaminationArm(armA, armB);

    auto &camera = *static_cast<CachContextState *>(g_camera);
    const std::int32_t type = camera.interactionType;
    float lateral = 0.0f;
    float forward = 0.0f;
    float height = 0.0f;
    if (type == 0) {
        forward = -0.015f;
    } else if (type == 1) {
        lateral = 0.03f;
        height = -0.01f;
        forward = 0.015f;
    } else {
        height = type == 2 ? -0.04f : -0.03f;
    }
    if (0.2 > static_cast<double>(camera.halfExtents.z))
        forward = static_cast<float>(static_cast<double>(forward) + 0.01);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    camera.hand[0].position = {-lateral * cosine + camera.eye.x - forward * sine, height + camera.eye.y, -lateral * sine + camera.eye.z + forward * cosine};
    camera.hand[1].position = {lateral * cosine + camera.eye.x - forward * sine, height + camera.eye.y, lateral * sine + camera.eye.z + forward * cosine};

    float spread = -0.04f;
    float rise = 0.0f;
    if (type == 0) {
        spread = -0.01f;
    } else if (type == 1) {
        spread = 0.0f;
    } else {
        rise = 0.02f;
        if (type != 2)
            spread = -0.05f;
    }
    const float cosineZero = cosine * 0.0f;
    const float cosineSpread = cosine * spread;
    const float sineSpread = sine * spread;
    const float sineZero = sine * 0.0f;
    const auto stepY = static_cast<float>(static_cast<double>(rise) * 0.2);
    camera.hand[0].step = {
        static_cast<float>(static_cast<double>(-cosineZero - sineSpread) * 0.2), stepY, static_cast<float>(static_cast<double>(cosineSpread - sineZero) * 0.2)};
    camera.hand[1].step = {
        static_cast<float>(static_cast<double>(cosineZero - sineSpread) * 0.2), stepY, static_cast<float>(static_cast<double>(cosineSpread + sineZero) * 0.2)};

    auto *const player = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float playerY = player ? GetPositionForEntity(player->entity.node).y : 0.0f;
    const float targetHeight = camera.targetPosition.y - playerY;
    const auto reach = static_cast<float>(static_cast<double>(targetHeight) - 1.4);
    const double pitch = static_cast<double>(std::max(-0.5f, std::min(0.5f, reach)));
    const std::int32_t yawA = camera.yaw + 12743;
    const std::int32_t yawB = camera.yaw + 20024;
    const auto jointPitch = static_cast<std::uint16_t>(static_cast<std::int32_t>((pitch * 50.0 + 90.0) * 8192.0 / 45.0));
    const auto endPitch = static_cast<std::uint16_t>(static_cast<std::int32_t>((pitch * 100.0 + 90.0) * 8192.0 / 45.0));
    const auto armPitch = static_cast<std::uint16_t>(static_cast<std::int32_t>((pitch * 120.0 + 90.0) * 8192.0 / 45.0));
    camera.armJointRotation[0] = {0, yawA, jointPitch};
    camera.armJointRotation[1] = {0, yawB, jointPitch};
    camera.armEndRotation[0] = {0, yawA - 3640, endPitch};
    camera.armEndRotation[1] = {0, yawB + 3640, endPitch};
    camera.armRotation[0] = {0, yawA - 5461, armPitch};
    camera.armRotation[1] = {0, yawB + 5461, armPitch};
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    g_cachArmRotationA = camera.armRotation[0];
    g_cachArmRotationB = camera.armRotation[1];

    const auto drop =
        static_cast<float>(static_cast<double>(std::min(camera.eyePosition.y, camera.previousEye.y)) - (static_cast<double>(camera.previousEye.y) + 0.35));
    camera.handTarget[0] = {
        camera.previousEye.x - cosine * 0.25f - sine * 0.3f, drop + camera.previousEye.y, camera.previousEye.z - sine * 0.25f + cosine * 0.3f};
    camera.handTarget[1] = {
        cosine * 0.25f + camera.previousEye.x - sine * 0.3f, drop + camera.previousEye.y, sine * 0.25f + camera.previousEye.z + cosine * 0.3f};
    const float reachDepth = camera.halfExtents.z * 1.5f;
    camera.acceleration3 = {cosine * 0.0f + camera.appliedPosition.x - reachDepth * sine, camera.appliedPosition.y + 0.0f,
        sine * 0.0f + camera.appliedPosition.z + reachDepth * cosine};

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    if (type == 1) {
        Matrix4x4::SetViewMatrixFromEuler(-6371, 0, 0);
        Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
        Matrix4x4::Pop();
        Matrix4x4::SetViewMatrixFromEuler(6371, 0, 0);
        Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
        g_cachHandRotationA = camera.handBaseRotation[0];
        g_cachHandRotationB = camera.handBaseRotation[1];
    } else {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::SetViewMatrixFromEuler(-11832, 0, -16384);
        Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
        Matrix4x4::Pop();
        Matrix4x4::SetViewMatrixFromEuler(-11832, 0, 5461);
        Matrix4x4_ExtractEulerAngles(g_cachHandRotationA);
        Matrix4x4::Pop();
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::SetViewMatrixFromEuler(11832, 0, -16384);
        Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
        Matrix4x4::Pop();
        Matrix4x4::SetViewMatrixFromEuler(11832, 0, 5461);
        Matrix4x4_ExtractEulerAngles(g_cachHandRotationB);
    }
    if (armA) {
        ApplyEulerAnglesToNode(armA->entity.node, camera.armJointRotation[0]);
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    }
    camera.handRotation[0] = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB) {
        ApplyEulerAnglesToNode(armB->entity.node, camera.armJointRotation[1]);
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    }
    camera.handRotation[1] = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    Matrix4x4::Pop();
}

// Fits a curve from its current point toward its destination over the sample times, the middle sample drawn the given share
// of the way back toward the start.
void FitCachCurveToward(CachQuadraticCurve &curve, const Vector3 &destination, float pull, const Vector3 &sampleTimes) {
    curve.acceleration = destination;
    curve.velocity = {(curve.position.x - destination.x) * pull + destination.x, (curve.position.y - destination.y) * pull + destination.y,
        (curve.position.z - destination.z) * pull + destination.z};
    FitCachQuadraticCurve(sampleTimes, curve);
}



void ApplySceneSplineAndRotationSetup() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    Vector3 sampleTimes{0.0f, 10.5f, 30.0f};
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_quadraticCurveRemaining = 30.0f;
    g_quadraticCurveProgress = 0.0f;
    FitCachCurveToward(g_cachCurveStart, camera.cameraAux, 0.30000001f, sampleTimes);
    FitCachCurveToward(g_cachCurveEnd, camera.targetPosition, 0.30000001f, sampleTimes);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const float lead = static_cast<double>((g_EnvPrngSeed >> 16) & 0x7fffU) * (1.0 / 32768.0) <= 0.5 ? 17.1f : 12.9f;
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    std::memcpy(&g_cachCurveAnchorA.position, anchorA.jointPositions + 60 * sizeof(Vector3), sizeof(Vector3));
    sampleTimes.y = lead;
    FitCachCurveToward(g_cachCurveAnchorA, camera.hand[0].position, 0.30000001f, sampleTimes);
    std::memcpy(&g_cachCurveAnchorB.position, anchorB.jointPositions + 60 * sizeof(Vector3), sizeof(Vector3));
    sampleTimes.y = 30.0f - lead;
    FitCachCurveToward(g_cachCurveAnchorB, camera.hand[1].position, 0.30000001f, sampleTimes);
    g_cachArmRotationA = camera.armRotation[0];
    g_cachArmRotationB = camera.armRotation[1];
    if (auto *const armA = static_cast<Character *>(g_cameraTargetEntity))
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (auto *const armB = static_cast<Character *>(g_shenmueCamera0))
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    HND_ApplyRootEuler(anchorA, g_cachHandRotationA);
    HND_ApplyRootEuler(anchorB, g_cachHandRotationB);
    Vector3_RotateVectorWithinLimits(g_cachHandRotationA, camera.handRotation[0]);
    Vector3_RotateVectorWithinLimits(g_cachHandRotationB, camera.handRotation[1]);
}



float AdvanceCachCurves(bool &finished, Vector3 &eye, Vector3 &target, Vector3 &handA, Vector3 *handB) {
    finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    handA = EvaluateCachCurve(g_cachCurveAnchorA, t);
    if (handB)
        *handB = EvaluateCachCurve(g_cachCurveAnchorB, t);
    eye = EvaluateCachCurve(g_cachCurveStart, t);
    target = EvaluateCachCurve(g_cachCurveEnd, t);
    return static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
}



bool AdjustCameraViewAndFoV() {
    bool finished = false;
    Vector3 eye{};
    Vector3 target{};
    Vector3 handA{};
    Vector3 handB{};
    const float share = AdvanceCachCurves(finished, eye, target, handA, &handB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    BlendEulerAngles(g_cachArmRotationA, camera.armJointRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armJointRotation[1], share);
    BlendEulerAngles(g_cachHandRotationA, camera.handRotation[0], share);
    BlendEulerAngles(g_cachHandRotationB, camera.handRotation[1], share);
    auto *const player = static_cast<Character *>(g_cacheTemporaryCharacter);
    const std::int32_t fovDelta = static_cast<std::int16_t>(camera.examineFov - currentPerspectiveFov);
    const double opacity = static_cast<double>(camera.savedPlayerOpacity < player->renderOpacity ? camera.savedPlayerOpacity : player->renderOpacity);
    const auto fadedOpacity = static_cast<float>((0.0 - opacity) * static_cast<double>(share) + opacity);
    const auto fov = static_cast<std::int32_t>(static_cast<float>(fovDelta) * share + static_cast<float>(currentPerspectiveFov));
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    ApplyRelativePositionToEntity(armA, anchorA, handA);
    ApplyRelativePositionToEntity(armB, anchorB, handB);
    HND_ApplyRootEuler(anchorA, g_cachHandRotationA);
    HND_ApplyRootEuler(anchorB, g_cachHandRotationB);
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    player->renderOpacity = fadedOpacity;
    CAME_SetFieldOfView(fov);
    return finished;
}



bool StepHeldExaminationHands() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    Mt5RuntimeNode *lid = nullptr;
    if (camera.interactionType == 2) {
        auto *const model = static_cast<Mt5RuntimeNode *>(camera.chara->model);
        if (model->childOffset)
            lid = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(model) + model->childOffset);
    }
    const float remaining = g_quadraticCurveRemaining + -2.0f;
    g_quadraticCurveRemaining = remaining;
    if (remaining >= 2.0f) {
        const auto steps = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<double>(remaining * 0.5f) * 0.5));
        if (lid && steps != 0) {
            const std::int32_t angle = lid->rotation.x;
            lid->rotation.x = angle + static_cast<std::int16_t>(-0x4000 - angle) / steps;
        }
    } else {
        if (lid)
            lid->rotation.x = -0x4000;
        g_quadraticCurveRemaining = -2.0f;
        if (remaining < -2.0f)
            return true;
    }
    ApplyPositionDeltaToEntity(static_cast<Character *>(g_cameraTargetEntity), camera.hand[0].step);
    ApplyPositionDeltaToEntity(static_cast<Character *>(g_shenmueCamera0), camera.hand[1].step);
    return false;
}



void FitHeldExaminationViewCurves() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    float depth = camera.halfExtents.z;
    if (0.2 > static_cast<double>(depth))
        depth = static_cast<float>(static_cast<double>(depth) - 0.2 + static_cast<double>(depth));
    const float rise = camera.eyePosition.y - camera.targetPosition.y;
    const float sine = camera.yawSine;
    const float centerY = camera.modelBoundsCenterWorld.y + 0.0f;
    g_quadraticCurveRemaining = 30.0f;
    const Vector3 sampleTimes{0.0f, 15.0f, 30.0f};
    const auto setBack = static_cast<float>(static_cast<double>(static_cast<float>(static_cast<double>(depth) - static_cast<double>(rise) * 0.05)) * 1.5);
    const float cosine = camera.yawCosine;
    const float viewX = cosine * 0.0f + camera.modelBoundsCenterWorld.x - sine * setBack;
    const float viewZ = sine * 0.0f + camera.modelBoundsCenterWorld.z + cosine * setBack;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachCurveToward(g_cachCurveAnchorA, camera.acceleration3, 0.30000001f, sampleTimes);
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    const auto eyeDistance = static_cast<float>(static_cast<double>(camera.halfExtents.z) * 1.1);
    const float eyeY = camera.halfExtents.y + camera.eyePosition.y - centerY + centerY;
    const Vector3 eyePoint{cosine * 0.0f + viewX - eyeDistance * sine, eyeY, sine * 0.0f + viewZ + eyeDistance * cosine};
    FitCachCurveToward(g_cachCurveStart, eyePoint, 0.30000001f, sampleTimes);
    FitCachCurveToward(g_cachCurveEnd, {viewX, centerY, viewZ}, 0.30000001f, sampleTimes);
}



bool Camera_UpdateRailProgress() {
    bool finished = false;
    Vector3 eye{};
    Vector3 target{};
    Vector3 objectPosition{};
    const float share = AdvanceCachCurves(finished, eye, target, objectPosition, nullptr);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], share);
    if (camera.chara)
        shenApplyTranslationToAll(camera.chara->entity.node, objectPosition);
    camCtrlUpdateMatrixStack();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    return finished;
}


bool AdvanceHeldExaminationReturn() {
    bool finished = false;
    Vector3 eye{};
    Vector3 target{};
    Vector3 objectPosition{};
    const float share = AdvanceCachCurves(finished, eye, target, objectPosition, nullptr);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    BlendEulerAngles(g_cachArmRotationA, camera.armJointRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armJointRotation[1], share);
    if (camera.chara)
        shenApplyTranslationToAll(camera.chara->entity.node, objectPosition);
    camCtrlUpdateMatrixStack();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    return finished;
}



void PlaceHandsForRelease() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    ShowExaminationArm(armA, armB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto lift = static_cast<float>(static_cast<double>(camera.modelBoundsCenterWorld.y) + 0.02 - static_cast<double>(camera.acceleration3.y));
    const float inset = 0.2 > static_cast<double>(camera.halfExtents.z) ? 0.015f : 0.0f;
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    camera.hand[0].position = {camera.acceleration3.x - cosine * 0.059999999f - inset * sine, lift + camera.acceleration3.y,
        camera.acceleration3.z - sine * 0.059999999f + inset * cosine};
    camera.hand[1].position = {cosine * 0.059999999f + camera.acceleration3.x - inset * sine, lift + camera.acceleration3.y,
        sine * 0.059999999f + camera.acceleration3.z + inset * cosine};
    g_cachArmRotationA = camera.armEndRotation[0];
    g_cachArmRotationB = camera.armEndRotation[1];
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(-5461, 0, 25486);
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
    Matrix4x4::Pop();
    Matrix4x4::SetViewMatrixFromEuler(-12743, 0, 0);
    Matrix4x4_ExtractEulerAngles(camera.handRotation[0]);
    Matrix4x4::Pop();
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::SetViewMatrixFromEuler(5461, 0, 25486);
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
    Matrix4x4::Pop();
    Matrix4x4::SetViewMatrixFromEuler(12743, 0, 0);
    Matrix4x4_ExtractEulerAngles(camera.handRotation[1]);
    if (armA) {
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    }
    g_cachHandRotationA = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    if (armB) {
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    }
    g_cachHandRotationB = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    Matrix4x4::Pop();
}



void FitHeldExaminationReturnCurves() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 sampleTimes{0.0f, 15.0f, 30.0f};
    g_quadraticCurveRemaining = 30.0f;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachCurveToward(g_cachCurveAnchorA, camera.appliedPosition, 0.30000001f, sampleTimes);
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    FitCachCurveToward(g_cachCurveStart, camera.cameraAux, 0.30000001f, sampleTimes);
    FitCachCurveToward(g_cachCurveEnd, camera.targetPosition, 0.30000001f, sampleTimes);
}




void UpdateWithCurve() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &cached = *static_cast<CachContextState *>(g_cacheData);
    Vector3 sampleTimes{0.0f, 10.5f, 30.0f};
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_quadraticCurveRemaining = 30.0f;
    g_quadraticCurveProgress = 0.0f;
    if (cached.state == 0) {
        SRCH_UpdateCameraEyeAndTarget(camera.previousEye, camera.previousTarget);
    } else {
        camera.previousEye = cached.lastCurvePosition;
        camera.previousTarget = cached.lastAcceleratedTarget;
        camera.savedFov = cached.examineFov;
        cached.analogDelta = {};
    }
    const float side = static_cast<std::uint32_t>(camera.interactionType - 10) > 1U ? 0.30000001f : 0.050000001f;
    const float eyeY = camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(std::min(g_cachCurveStart.position.y, eyeY)) - (static_cast<double>(eyeY) + 0.35));
    FitCachCurveToward(g_cachCurveStart, camera.previousEye, 0.30000001f, sampleTimes);
    FitCachCurveToward(g_cachCurveEnd, camera.previousTarget, 0.30000001f, sampleTimes);
    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const float lead = static_cast<double>((g_EnvPrngSeed >> 16) & 0x7fffU) * (1.0 / 32768.0) <= 0.5 ? 17.1f : 12.9f;
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const Vector3 &eye = camera.previousEye;
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    sampleTimes.y = lead;
    std::memcpy(&g_cachCurveAnchorA.position, anchorA.jointPositions + 60 * sizeof(Vector3), sizeof(Vector3));
    FitCachCurveToward(
        g_cachCurveAnchorA, {-side * cosine + eye.x - sine * 0.2f, drop + eye.y, -side * sine + eye.z + cosine * 0.2f}, 0.60000002f, sampleTimes);
    sampleTimes.y = 30.0f - lead;
    std::memcpy(&g_cachCurveAnchorB.position, anchorB.jointPositions + 60 * sizeof(Vector3), sizeof(Vector3));
    FitCachCurveToward(g_cachCurveAnchorB, {side * cosine + eye.x - sine * 0.2f, drop + eye.y, side * sine + eye.z + cosine * 0.2f}, 0.60000002f, sampleTimes);
    Vector3_RotateVectorWithinLimits(g_cachHandRotationA, camera.handRotation[0]);
    Vector3_RotateVectorWithinLimits(g_cachHandRotationB, camera.handRotation[1]);
}



bool CalculateAndAdjustVectorsForCameraFieldOfView() {
    bool finished = false;
    Vector3 eye{};
    Vector3 target{};
    Vector3 handA{};
    Vector3 handB{};
    const float share = AdvanceCachCurves(finished, eye, target, handA, &handB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    BlendEulerAngles(g_cachArmRotationA, camera.armRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armRotation[1], share);
    BlendEulerAngles(g_cachHandRotationA, camera.handRotation[0], share);
    BlendEulerAngles(g_cachHandRotationB, camera.handRotation[1], share);
    auto *const player = static_cast<Character *>(g_cacheTemporaryCharacter);
    const std::int32_t fovDelta = static_cast<std::int16_t>(camera.savedFov - currentPerspectiveFov);
    const float opacity = player->renderOpacity + (camera.savedPlayerOpacity - player->renderOpacity) * share;
    const auto fov = static_cast<std::int32_t>(static_cast<float>(fovDelta) * share + static_cast<float>(currentPerspectiveFov));
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    ApplyRelativePositionToEntity(armA, anchorA, handA);
    ApplyRelativePositionToEntity(armB, anchorB, handB);
    HND_ApplyRootEuler(anchorA, g_cachHandRotationA);
    HND_ApplyRootEuler(anchorB, g_cachHandRotationB);
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    player->renderOpacity = opacity;
    CAME_SetFieldOfView(fov);
    return finished;
}





std::int32_t PollHeldExamination() {
    std::int32_t result = 0;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    const std::int32_t step = g_cameraGlobalState;
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            ApplyFullEntityAndCameraTransform();
            ++g_cameraGlobalState;
            break;
        case 1:
            if ((anchorA.control & 8U) == 0)
                break;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(camera.interactionType == 0 ? kExamineReachPoseType0 :
                    camera.interactionType == 1                ? kExamineReachPoseType1 :
                                                                 kExamineReachPose,
                static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.7), nullptr);
            g_cameraGlobalState = step + 1;
            break;
        case 2:
            if (!AdjustCameraViewAndFoV())
                break;
            g_quadraticCurveRemaining = 10.0f;
            if (camera.interactionType == 1)
                camCtrlSetAnchorNodeTo(kExamineGraspPoseType1, 10.0f, camera.chara);
            else
                camCtrlSetAnchorNodeTo(camera.interactionType == 0 ? kExamineGraspPoseType0 : kExamineGraspPose, 10.0f, nullptr);
            g_cameraGlobalState = step + 1;
            break;
        case 3:
            if (StepHeldExaminationHands()) {
                PlayCachQueuedSound(g_globalSoundCommand);
                SyncCurrentPositionAndRotationFromCamera();
                FitHeldExaminationViewCurves();
                g_cameraGlobalState = step + 1;
            }
            break;
        case 4:
            if (Camera_UpdateRailProgress()) {
                PlayCachQueuedSound(g_soundCommand);
                UpdateEntityPositionRotation(nullptr);
                if (camera.interactionType == 2) {
                    auto *const model = static_cast<Mt5RuntimeNode *>(camera.chara->model);
                    if (model->childOffset)
                        reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(model) + model->childOffset)->rotation.x = 0;
                }
                UpdateHandMotData();
                ++g_cameraGlobalState;
            }
            break;
        default:
            g_cameraGlobalState = -1;
            result = 1;
            break;
        }
    } else if (camera.state == 5) {
        switch (step) {
        case 0:
            if (camCtrlUpdateAccelTerm()) {
                camCtrlSetAnchorNodeTo(kTargetPoseMode8, 0.0f, nullptr);
                PlaceHandsForRelease();
                ++g_cameraGlobalState;
            }
            break;
        case 1:
            if ((anchorA.control & 8U) != 0) {
                ApplyRelativePositionToEntity(static_cast<Character *>(g_cameraTargetEntity), anchorA, camera.hand[0].position);
                ApplyRelativePositionToEntity(
                    static_cast<Character *>(g_shenmueCamera0), *static_cast<HandModelWork *>(g_cameraAnchorB), camera.hand[1].position);
                PlayCachQueuedSound(g_globalSoundCommand);
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                camCtrlSetAnchorNodeTo(kTargetGripPoseMode8, 15.0f, camera.chara);
                SyncCurrentPositionAndRotationFromCamera();
                FitHeldExaminationReturnCurves();
                g_cameraGlobalState = step + 1;
            }
            break;
        case 2:
            if (AdvanceHeldExaminationReturn()) {
                PlayCachQueuedSound(g_soundCommand);
                UpdateEntityPositionRotation(nullptr);
                g_cameraGlobalState = step + 1;
            }
            break;
        case 3:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            break;
        case 4:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            break;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            result = 1;
            break;
        }
    } else {
        camera.state = 0;
        result = 1;
    }
    camera.attachmentRootPosition = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    camera.attachmentRootRotation = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    return result;
}



void ExpandExaminationBounds(Mt5RuntimeNode *node, Vector3 &minimum, Vector3 &maximum) {
    while (true) {
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        if (node->nodeKind != 254) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            std::byte *bindingBase = nullptr;
            const auto *const binding = static_cast<const std::byte *>(MT5_SceneBindingForNode(node, &bindingBase));
            if (binding && bindingBase) {
                std::int32_t vertexOffset{};
                std::int32_t vertexCount{};
                std::memcpy(&vertexOffset, binding + 4, sizeof(vertexOffset));
                std::memcpy(&vertexCount, binding + 8, sizeof(vertexCount));
                for (std::int32_t index = 0; index < vertexCount; ++index) {
                    Vector3 local{};
                    std::memcpy(&local, bindingBase + vertexOffset + index * 24, sizeof(local));
                    matrix_stack::Matrix4x4_Push();
                    Matrix4x4::preTranslate(local.x, local.y, local.z);
                    const Vector3 point{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
                    Matrix4x4::Pop();
                    if (minimum.x > point.x)
                        minimum.x = point.x;
                    if (minimum.y > point.y)
                        minimum.y = point.y;
                    if (minimum.z > point.z)
                        minimum.z = point.z;
                    if (point.x > maximum.x)
                        maximum.x = point.x;
                    if (point.y > maximum.y)
                        maximum.y = point.y;
                    if (point.z > maximum.z)
                        maximum.z = point.z;
                }
            }
            if (node->childOffset)
                ExpandExaminationBounds(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), minimum, maximum);
        }
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop();
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
}





std::int32_t PickExaminationTransforms(Mt5RuntimeNode *node, float accumulatedX, float centerX) {
    std::byte *const base = MT5_ModelBaseForNode(node);
    if (node->siblingOffset)
        matrix_stack::Matrix4x4_Push();
    const float x = node->position.x + accumulatedX;
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
    Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (!camera.primaryTransform && node->nodeKind == 13)
        camera.primaryTransform = node;
    if (!camera.secondaryTransform && node->nodeKind == 8)
        camera.secondaryTransform = node;
    const Vector3 world{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    std::int32_t result = 0;
    if (node->nodeKind == 254) {
        Mt5RuntimeNode *current = node;
        if (static_cast<std::uint16_t>(camera.interactionType - 10) <= 0x13U) {
            while (current->parentOffset && !result) {
                current = reinterpret_cast<Mt5RuntimeNode *>(base + current->parentOffset);
                if (current->nodeKind == 13) {
                    camera.primaryTransform = current;
                    result = 1;
                    camera.eye = world;
                }
                if (current->nodeKind == 8) {
                    camera.secondaryTransform = current;
                    result = 1;
                    camera.acceleration = world;
                }
            }
        }
        if (!result) {
            result = 2;
            const std::int32_t type = camera.interactionType;
            bool secondary = false;
            if (type == 10 || type == 11)
                secondary = static_cast<double>(centerX) + 0.03 <= static_cast<double>(x);
            else if (type == 20 || type == 21)
                secondary = static_cast<double>(x) <= static_cast<double>(centerX) - 0.03;
            if (secondary) {
                camera.secondaryTransform = current;
                camera.acceleration = world;
            } else {
                camera.primaryTransform = current;
                camera.eye = world;
            }
        }
    }
    if (node->childOffset)
        result |= PickExaminationTransforms(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), x, centerX);
    if (!node->siblingOffset)
        return result;
    Matrix4x4::Pop();
    return result | PickExaminationTransforms(reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset), accumulatedX, centerX);
}



std::int32_t InitializeExaminationTarget(Character *target) {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    camera.chara = target;
    auto *const model = target ? static_cast<Mt5RuntimeNode *>(target->model) : nullptr;
    if (!model)
        camera.interactionType = -1;
    if (camera.interactionType < 0)
        return 1;
    matrix_stack::Matrix4x4_Push();
    camera.attachmentRootPosition = GetPositionForEntity(camera.chara->entity.node);
    const Vector3i rotation = GetEulerAnglesForEntity(camera.chara->entity.node);
    camera.attachmentRootRotation = {static_cast<std::int16_t>(rotation.x), static_cast<std::int16_t>(rotation.y), static_cast<std::int16_t>(rotation.z)};
    camera.attachmentOpacity = 1.0f;
    camera.yaw = camera.attachmentRootRotation.y;
    camera.yawSine = sin_fixed16(static_cast<std::int16_t>(-camera.yaw));
    camera.yawCosine = cos_fixed16(static_cast<std::uint16_t>(-camera.yaw));
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::Scale(camera.chara->scale.x, camera.chara->scale.y, camera.chara->scale.z);
    Vector3 minimum{10000.0f, 10000.0f, 10000.0f};
    Vector3 maximum{-10000.0f, -10000.0f, -10000.0f};
    ExpandExaminationBounds(model, minimum, maximum);
    camera.halfExtents = {static_cast<float>(static_cast<double>(maximum.x - minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y - minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z - minimum.z) * 0.5)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(camera.attachmentRootPosition.x, camera.attachmentRootPosition.y, camera.attachmentRootPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(camera.attachmentRootRotation.y, camera.attachmentRootRotation.x, camera.attachmentRootRotation.z);
    matrix_stack::Matrix4x4_Push();
    const auto centerX = static_cast<float>(static_cast<double>(maximum.x + minimum.x) * 0.5);
    Matrix4x4::preTranslate(
        centerX, static_cast<float>(static_cast<double>(maximum.y + minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z + minimum.z) * 0.5));
    camera.modelBoundsCenterWorld = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    camera.acceleration = camera.attachmentRootPosition;
    camera.eye = camera.attachmentRootPosition;
    camera.secondaryTransform = nullptr;
    camera.primaryTransform = nullptr;
    if (PickExaminationTransforms(model, 0.0f, centerX))
        g_cameraStateFlags |= 0x100U;
    else
        g_cameraStateFlags &= ~0x100U;
    if (!camera.primaryTransform && !camera.secondaryTransform)
        camera.primaryTransform = model;
    Matrix4x4::Pop();
    return 0;
}



void BeginHeldExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    camera.appliedPosition = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    camera.targetPosition = camera.eye;
    float radius = static_cast<float>(static_cast<double>(camera.halfExtents.z) + static_cast<double>(camera.halfExtents.z));
    radius = 0.25f > radius ? 0.25f : radius;
    if (static_cast<double>(radius) > 0.65)
        radius = 0.65f;
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const auto height = static_cast<float>(static_cast<double>(camera.halfExtents.y + camera.modelBoundsCenterWorld.y) + 0.3);
    camera.eyePosition = {camera.targetPosition.x - sine * radius, height, cosine * radius + camera.targetPosition.z};
    const auto nearRadius = static_cast<float>(static_cast<double>(radius) * 0.75);
    camera.cameraAux = {camera.targetPosition.x - sine * nearRadius, height, cosine * nearRadius + camera.targetPosition.z};
}



void TransformMatrixWithStruct(Mt5RuntimeNode *node) {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    while (true) {
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        if (node->nodeKind == 254) {
            g_cameraStateFlags |= 1U;
            control.anchorPosition = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4_ExtractEulerAngles(control.anchorRotation);
        }
        if (node->childOffset)
            TransformMatrixWithStruct(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset));
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop();
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
}


void ExpandExaminationBoundsWithin(Mt5RuntimeNode *node, Vector3 &minimum, Vector3 &maximum, const Vector3 &boxMinimum, const Vector3 &boxMaximum) {
    while (true) {
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        if (node->nodeKind != 254) {
            Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
            Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
            Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
            std::byte *bindingBase = nullptr;
            const auto *const binding = static_cast<const std::byte *>(MT5_SceneBindingForNode(node, &bindingBase));
            if (binding && bindingBase) {
                std::int32_t vertexOffset{};
                std::int32_t vertexCount{};
                std::memcpy(&vertexOffset, binding + 4, sizeof(vertexOffset));
                std::memcpy(&vertexCount, binding + 8, sizeof(vertexCount));
                for (std::int32_t index = 0; index < vertexCount; ++index) {
                    Vector3 local{};
                    std::memcpy(&local, bindingBase + vertexOffset + index * 24, sizeof(local));
                    matrix_stack::Matrix4x4_Push();
                    Matrix4x4::preTranslate(local.x, local.y, local.z);
                    const Vector3 point{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
                    Matrix4x4::Pop();
                    if (!(point.x <= boxMinimum.x) && point.y > boxMinimum.y && point.z > boxMinimum.z && boxMaximum.x > point.x && boxMaximum.y > point.y &&
                        boxMaximum.z > point.z) {
                        if (minimum.x > point.x)
                            minimum.x = point.x;
                        if (minimum.y > point.y)
                            minimum.y = point.y;
                        if (minimum.z > point.z)
                            minimum.z = point.z;
                        if (point.x > maximum.x)
                            maximum.x = point.x;
                        if (point.y > maximum.y)
                            maximum.y = point.y;
                        if (point.z > maximum.z)
                            maximum.z = point.z;
                    }
                }
            }
            if (node->childOffset)
                ExpandExaminationBoundsWithin(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), minimum, maximum, boxMinimum, boxMaximum);
        }
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop();
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
}






void InitializeTargetMode(std::uint16_t requestedMode) {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    auto *const model = static_cast<Mt5RuntimeNode *>(control.chara->model);
    if (!model) {
        control.inputMode = -1;
        return;
    }
    auto *examination = static_cast<CachContextState *>(g_cacheData);
    if (examination->state == 0)
        examination = static_cast<CachContextState *>(g_camera);
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    const Vector3 scale = control.chara->scale;
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    Vector3 minimum{10000.0f, 10000.0f, 10000.0f};
    Vector3 maximum{-10000.0f, -10000.0f, -10000.0f};
    ExpandExaminationBounds(model, minimum, maximum);
    const Vector3 size{maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z};
    const Vector3 position = control.chara ? GetPositionForEntity(control.chara->entity.node) : Vector3{};
    const Vector3i euler = control.chara ? GetEulerAnglesForEntity(control.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::preTranslate(static_cast<float>(static_cast<double>(maximum.x + minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y + minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z + minimum.z) * 0.5));
    control.boundsCenter = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float ryoY = ryo ? GetPositionForEntity(ryo->entity.node).y : 0.0f;
    const auto eyeHeight = static_cast<float>(static_cast<double>(ryoY) + 1.65);
    std::uint16_t flags;
    if (examination->state != 0) {
        control.eye = g_CameraEyePos;
        const std::int32_t type = examination->interactionType;
        if (static_cast<std::uint32_t>(type) <= 3U || type == 30) {
            flags = static_cast<std::uint16_t>((g_cameraStateFlags & 0xffefU) | 2U);
        } else {
            flags = static_cast<std::uint16_t>((g_cameraStateFlags & 0xfffdU) | 0x10U);
            const double dx = control.eye.x - control.boundsCenter.x;
            control.eye.x = static_cast<float>(dx + dx + static_cast<double>(control.boundsCenter.x));
            const double dz = control.eye.z - control.boundsCenter.z;
            control.eye.z = static_cast<float>(dz + dz + static_cast<double>(control.boundsCenter.z));
            const double y = control.eye.y;
            if (static_cast<double>(eyeHeight) > y + 0.25)
                control.eye.y = static_cast<float>(y + 0.25);
            else if (y - 0.25 > static_cast<double>(eyeHeight))
                control.eye.y = static_cast<float>(y - 0.25);
            else
                control.eye.y = eyeHeight;
        }
    } else {
        float dx = g_CameraEyePos.x - control.boundsCenter.x;
        float dz = g_CameraEyePos.z - control.boundsCenter.z;
        float lengthSquared = dz * dz + dx * dx;
        if (0.0025 > static_cast<double>(lengthSquared)) {
            dx = 0.050000001f;
            lengthSquared = 0.0024999999f;
            dz = 0.050000001f;
        }
        const float length = std::sqrt(lengthSquared);
        flags = static_cast<std::uint16_t>(g_cameraStateFlags & 0xffedU);
        control.eye.y = eyeHeight;
        const auto pull = static_cast<float>(0.4 / static_cast<double>(length));
        control.eye.z = pull * dz + control.boundsCenter.z;
        control.eye.x = pull * dx + control.boundsCenter.x;
    }

    const std::uint16_t facing = atan2_fixed16(control.eye.x - control.boundsCenter.x, control.eye.z - control.boundsCenter.z);
    control.yaw = facing;
    const auto nearLevel = [](std::int32_t angle) {
        return static_cast<std::uint32_t>(angle - 0xe38) > 0xe38fU || static_cast<std::uint32_t>(angle - 0x71c8) <= 0x1c6fU;
    };
    const auto nearUpright = [](std::int32_t angle) {
        return static_cast<std::uint32_t>(angle - 0x31c8) <= 0x1c6fU || static_cast<std::uint32_t>(angle - 0xb1c8) <= 0x1c6fU;
    };
    std::int32_t axis = -1;
    float along = size.x;
    float across = size.z;
    if (nearLevel(rotation.z)) {
        if (nearLevel(rotation.x)) {
            axis = 1;
        } else if (nearUpright(rotation.x)) {
            axis = 1;
            across = size.y;
        }
    } else if (nearUpright(rotation.z)) {
        if (nearLevel(rotation.y)) {
            axis = 0;
            along = size.y;
        } else if (nearUpright(rotation.y)) {
            if (nearLevel(rotation.x)) {
                axis = 0;
                along = size.y;
                across = size.x;
            } else if (nearUpright(rotation.x)) {
                axis = 2;
                along = size.z;
                across = size.x;
            }
        }
    }
    float difference = along - across;
    if (0.0f > difference)
        difference = -difference;
    if (axis >= 0 && !(0.02 > static_cast<double>(difference))) {
        const std::int32_t angles[3]{rotation.x, rotation.y, rotation.z};
        auto delta = static_cast<std::uint16_t>(angles[axis] - facing);
        if (delta >= 0x2000) {
            if (delta < 0x6000)
                delta -= 0x4000;
            else if (delta < 0xa000)
                delta -= 0x8000;
            else if (delta < 0xe000)
                delta -= 0xc000;
        }
        control.yaw = static_cast<std::uint16_t>(delta + facing);
    }
    const auto turn = static_cast<std::uint16_t>(-control.yaw);
    control.yawSine = sin_fixed16(static_cast<std::int16_t>(turn));
    control.yawCosine = cos_fixed16(turn);

    workTransformMatrix = transformMatrix_prePass;
    g_cameraStateFlags = flags & 0xfffeU;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    TransformMatrixWithStruct(model);
    Vector3i &anchor = control.anchorRotation;
    anchor = {static_cast<std::int16_t>(anchor.x), static_cast<std::int16_t>(anchor.y), static_cast<std::int16_t>(anchor.z)};
    if (static_cast<std::uint32_t>(anchor.z + 0x2000) > 0x4000U) {
        anchor = {static_cast<std::int16_t>(anchor.x - 0x8000), static_cast<std::int16_t>(0x8000 - anchor.y), static_cast<std::int16_t>(anchor.z - 0x8000)};
    }

    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-control.yaw));
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    minimum = {10000.0f, 10000.0f, 10000.0f};
    maximum = {-10000.0f, -10000.0f, -10000.0f};
    ExpandExaminationBounds(model, minimum, maximum);
    control.halfExtents = {maximum.x - minimum.x, maximum.y - minimum.y, maximum.z - minimum.z};
    const double sizeX = control.halfExtents.x;
    const double sizeY = control.halfExtents.y;
    const double sizeZ = control.halfExtents.z;
    if ((0.01 > sizeX && 0.01 > sizeY && 0.01 > sizeZ) || (sizeX > 0.85 && sizeY > 0.85 && sizeZ > 0.85))
        control.inputMode = -1;
    else if (sizeY >= 0.09 && sizeZ >= 0.13)
        control.inputMode = sizeX >= 0.13 ? 8 : sizeZ > sizeY * 1.8 ? 3 : 1;
    else if (sizeY >= 0.09 && !(sizeX + 0.02 <= sizeZ))
        control.inputMode = sizeX <= sizeY * 1.8 ? 0 : 2;
    else if (sizeY >= 0.09)
        control.inputMode = sizeZ > sizeY * 1.8 ? 3 : 1;
    else if (sizeZ >= 0.13)
        control.inputMode = sizeX < 0.13 ? 3 : sizeX >= 0.36 || sizeZ >= 0.36 ? 9 : 5;
    else
        control.inputMode = sizeX + 0.02 > sizeZ ? 2 : 3;
    if (requestedMode <= 9U)
        control.inputMode = static_cast<std::int16_t>(requestedMode);
    const std::uint16_t stateFlags = static_cast<std::uint16_t>(g_cameraStateFlags);
    if ((stateFlags & 1U) != 0) {
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::SetViewMatrixFromEuler(anchor.y, anchor.x, anchor.z);
        Matrix4x4::preTranslate(0.0f, 0.0f, 1.0f);
        const double up = workTransformMatrix.rows[3].y;
        control.inputMode = up > 0.7 || -0.7 > up ? 6 : 7;
    }
    control.halfExtents = {static_cast<float>(sizeX * 0.5), static_cast<float>(sizeY * 0.5), static_cast<float>(sizeZ * 0.5)};
    const Vector3 center{static_cast<float>(static_cast<double>(maximum.x + minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y + minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z + minimum.z) * 0.5)};
    const auto offset = [](float value, double delta) { return static_cast<float>(static_cast<double>(value) + delta); };
    Vector3 boxMinimum{-10000.0f, -10000.0f, -10000.0f};
    Vector3 boxMaximum{10000.0f, 10000.0f, 10000.0f};
    switch (control.inputMode) {
    case 0:
        boxMinimum = {offset(maximum.x, -0.12), offset(center.y, -0.08), -10000.0f};
        boxMaximum = {offset(maximum.x, 0.04), offset(center.y, 0.08), 10000.0f};
        break;
    case 1:
        boxMinimum = {-10000.0f, offset(center.y, -0.08), offset(maximum.z, -0.12)};
        boxMaximum = {10000.0f, offset(center.y, 0.08), offset(maximum.z, 0.04)};
        break;
    case 2:
        boxMinimum = {offset(center.x, -0.08), offset(maximum.y, -0.12), -10000.0f};
        boxMaximum = {offset(center.x, 0.08), offset(maximum.y, 0.04), 10000.0f};
        break;
    case 3:
        boxMinimum = {-10000.0f, offset(maximum.y, -0.12), offset(maximum.z, -0.12)};
        boxMaximum = {10000.0f, offset(maximum.y, 0.04), offset(maximum.z, 0.04)};
        break;
    case 4:
        boxMinimum = {offset(maximum.x, -0.12), offset(maximum.y, -0.12), -10000.0f};
        boxMaximum = {offset(maximum.x, 0.04), offset(maximum.y, 0.04), 10000.0f};
        break;
    case 8:
        boxMinimum = {-10000.0f, offset(center.y, -0.08), offset(center.z, -0.08)};
        boxMaximum = {10000.0f, offset(center.y, 0.08), offset(center.z, 0.08)};
        break;
    default:
        break;
    }
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-control.yaw));
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    minimum = {10000.0f, 10000.0f, 10000.0f};
    maximum = {-10000.0f, -10000.0f, -10000.0f};
    ExpandExaminationBoundsWithin(model, minimum, maximum, boxMinimum, boxMaximum);
    control.gripExtents = {static_cast<float>(static_cast<double>(maximum.x - minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y - minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z - minimum.z) * 0.5)};
    if (0.0f > control.gripExtents.x || 0.0f > control.gripExtents.y || 0.0f > control.gripExtents.z)
        control.gripExtents = control.halfExtents;

    const float halfY = control.halfExtents.y;
    const float halfZ = control.halfExtents.z;
    if ((stateFlags & 2U) != 0)
        control.reach = halfZ + examination->halfExtents.z + halfY;
    else
        control.reach = static_cast<float>(static_cast<double>(halfZ) * 4.0 + static_cast<double>(halfY));
    const float centerX = control.boundsCenter.x;
    const float centerZ = control.boundsCenter.z;
    const float distance = std::sqrt((control.eye.z - centerZ) * (control.eye.z - centerZ) + (control.eye.x - centerX) * (control.eye.x - centerX));
    auto radius = static_cast<float>(static_cast<double>(halfY + control.halfExtents.x + halfZ) * 1.5 + 0.1);
    if (0.35 > static_cast<double>(radius))
        radius = 0.34999999f;
    if (static_cast<double>(radius) > 0.7)
        radius = 0.69999999f;
    radius = radius < distance ? radius : distance;
    if (0.3 > static_cast<double>(radius))
        radius = 0.30000001f;
    std::int32_t viewYaw = control.yaw;
    switch (control.inputMode) {
    case 0:
    case 2:
    case 4:
    case 5:
        viewYaw += (stateFlags & 0x1000U) != 0 ? 910 : -910;
        break;
    case 1:
    case 3:
        viewYaw += (stateFlags & 0x1000U) != 0 ? 2730 : -2730;
        break;
    default:
        break;
    }
    const auto viewTurn = static_cast<std::uint16_t>(-static_cast<std::int32_t>(static_cast<std::uint16_t>(viewYaw)));
    control.eye.x = centerX - sin_fixed16(static_cast<std::int16_t>(viewTurn)) * radius;
    control.eye.z = cos_fixed16(viewTurn) * radius + centerZ;
    control.viewEye = control.eye;
    if ((stateFlags & 2U) == 0)
        control.viewEye.y = static_cast<float>(static_cast<double>(control.eye.y - control.boundsCenter.y) * 0.2 + static_cast<double>(control.boundsCenter.y));
    if (control.inputMode == 5 || control.inputMode == 9)
        control.viewEye.y = static_cast<float>(static_cast<double>(control.boundsCenter.y) + 0.4);
    control.eye.x = control.eye.x - control.reach * control.yawSine;
    control.handBase = control.boundsCenter;
    control.eye.z = control.reach * control.yawCosine + control.eye.z;

    workTransformMatrix = transformMatrix_prePass;
    switch (control.inputMode) {
    case 1:
        Matrix4x4::RotateAroundY_Wrapper((stateFlags & 0x1000U) != 0 ? 0xc000 : 0x4000);
        break;
    case 2:
        Matrix4x4::SetViewMatrixFromEuler(-0x78e3, 0x71c7, 0xe38);
        break;
    case 3:
        Matrix4x4::SetViewMatrixFromEuler(20024, 9102, -20024);
        break;
    default:
        break;
    }
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4_ExtractEulerAngles(control.referenceRotation);
    Vector3_RotateVectorWithinLimits(Vector3i{}, control.referenceRotation);
    control.gripOffset[0] = control.handBase;
    control.gripOffset[1] = {};
    control.gripOffset[2] = {};
    control.swayPhase[0] = 0;
    control.swayPhase[1] = 0;
    control.swayPhase[2] = 0;
    control.gripRotation = control.referenceRotation;
    Matrix4x4::Pop();
    control.analogDelta = {};
    control.savedPlayerOpacity = static_cast<Character *>(g_cacheTemporaryCharacter)->renderOpacity;
}

// Target mode shows a hand arm inline (motionFlags bit 4), logging the change under the visibility debug flag.
void ShowCachCharacter(Character *character) {
    if (!character)
        return;
    const bool wasVisible = (character->motionFlags & 4U) != 0;
    character->motionFlags |= 4U;
    if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0 && !wasVisible)
        CHAR_LogNPCVisibility(character, true);
}

// The examination hides a character inline (motionFlags bit 4), logging the change under the visibility debug flag.
void HideCachCharacter(Character *character) {
    if (!character)
        return;
    const bool wasVisible = (character->motionFlags & 4U) != 0;
    character->motionFlags &= static_cast<std::uint8_t>(~4U);
    if (g_bDebugLogVisibility && (character->flags.value & 0x400U) != 0 && wasVisible)
        CHAR_LogNPCVisibility(character, false);
}



void Matrix4x4_ApplyViewTransformAndInverse(Character *arm, HandModelWork &anchor, const Vector3i *rotation) {
    if (rotation) {
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::SetViewMatrixFromEuler(rotation->y, rotation->x, rotation->z);
    }
    Matrix4x4 target = workTransformMatrix;
    HND_ApplyRootEuler(anchor, {});
    ApplyHandRootTransform(arm, anchor);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::Multiply(&target);
    Vector3i euler{};
    Matrix4x4_ExtractEulerAngles(euler);
    HND_ApplyRootEuler(anchor, euler);
}






void InitializeTargetModeHands() {
    auto *examination = static_cast<CachContextState *>(g_cacheData);
    if (examination->state == 0)
        examination = static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    if (control.inputMode >= 8) {
        ShowCachCharacter(armA);
        ShowCachCharacter(armB);
    } else if ((g_cameraStateFlags & 0x1000U) != 0) {
        ShowCachCharacter(armA);
    } else {
        ShowCachCharacter(armB);
    }
    const std::uint16_t flags = static_cast<std::uint16_t>(g_cameraStateFlags);
    matrix_stack::Matrix4x4_Push();
    const float height = ((flags & 1U) != 0 ? control.anchorPosition.y : control.boundsCenter.y) - control.viewEye.y;
    float tilt = 0.5f < height ? 0.5f : height;
    tilt = -0.5f > tilt ? -0.5f : tilt;
    const float pitch = (flags & 2U) != 0 ? -40.0f : static_cast<float>(static_cast<double>(tilt) * 50.0);
    control.armJointRotation[1].x = 0;
    control.armJointRotation[0].x = 0;
    const std::int32_t side = control.yaw + 0x4000;
    control.armJointRotation[1].y = side;
    control.armJointRotation[0].y = side;
    const std::int32_t lean = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(pitch) + 90.0) * 8192.0 / 45.0));
    control.armJointRotation[1].z = lean;
    control.armJointRotation[0].z = lean;
    if (control.inputMode == 1 || control.inputMode == 3) {
        control.armJointRotation[1].y += 9102;
        control.armJointRotation[0].y = side - 9102;
    } else if (control.inputMode != 8) {
        control.armJointRotation[1].y += 6371;
        control.armJointRotation[0].y = side - 6371;
    }
    const std::int32_t left = control.armJointRotation[0].y;
    const std::int32_t right = control.armJointRotation[1].y;
    control.armMidRotation[0] = {0, left - 1820, 0x4000};
    control.armMidRotation[1] = {0, right + 1820, 0x4000};
    control.armEndRotation[0] = {0, left - 3640, 0x4000};
    control.armEndRotation[1] = {0, right + 3640, 0x4000};
    control.armRotation[0] = {0, left - 0x2000, lean - 0x2000};
    control.armRotation[1] = {0, right + 0x2000, lean - 0x2000};
    Matrix4x4_ComposeFromRotation(control.armRotation[0]);
    Matrix4x4_ComposeFromRotation(control.armRotation[1]);
    Matrix4x4_ComposeFromRotation(control.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(control.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(control.armMidRotation[0]);
    Matrix4x4_ComposeFromRotation(control.armMidRotation[1]);
    Matrix4x4_ComposeFromRotation(control.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(control.armEndRotation[1]);
    Vector3_RotateVectorWithinLimits(control.armRotation[0], control.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(control.armRotation[1], control.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(control.armJointRotation[0], control.armMidRotation[0]);
    Vector3_RotateVectorWithinLimits(control.armJointRotation[1], control.armMidRotation[1]);
    Vector3_RotateVectorWithinLimits(control.armMidRotation[0], control.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(control.armMidRotation[1], control.armEndRotation[1]);

    const float lowest = control.eye.y < control.savedEye.y ? control.eye.y : control.savedEye.y;
    float drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(control.savedEye.y) + 0.35));
    float spread = 0.2f;
    float forward = 0.2f;
    if ((flags & 2U) != 0) {
        spread = 0.25f;
        forward = 0.15f;
        drop = static_cast<float>(static_cast<double>(drop) + 0.15);
    }
    const float sine = control.yawSine;
    const float cosine = control.yawCosine;
    const Vector3 &eye = control.savedEye;
    control.handTarget[0] = {-spread * cosine + eye.x - forward * sine, drop + eye.y, -spread * sine + eye.z + forward * cosine};
    control.handTarget[1] = {spread * cosine + eye.x - forward * sine, drop + eye.y, spread * sine + eye.z + forward * cosine};

    const std::int16_t mode = control.inputMode;
    if (mode == 6 || mode == 7) {
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::preTranslate(control.anchorPosition.x, control.anchorPosition.y, control.anchorPosition.z);
        Matrix4x4::SetViewMatrixFromEuler(control.anchorRotation.y, control.anchorRotation.x, control.anchorRotation.z);
        Matrix4x4::RotateAroundZ_Wrapper(0xc000);
        Matrix4x4::RotateAroundX_Wrapper(0xc000);
        float x = 0.0f;
        float z = 0.0f;
        if (mode == 6) {
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(control.yaw - control.anchorRotation.z));
            x = (flags & 0x1000U) != 0 ? -0.029999999f : 0.029999999f;
        } else {
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-control.anchorRotation.z));
            z = (flags & 0x1000U) != 0 ? -0.0099999998f : 0.0099999998f;
        }
        Matrix4x4::preTranslate(x, 0.029999999f, z);
        control.handPoint[1] = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        control.handPoint[0] = control.handPoint[1];
    } else {
        const auto atLeastZero = [](float value) { return 0.0f > value ? 0.0f : value; };
        float lateral;
        float depth = -0.0099999998f;
        switch (mode) {
        case 1:
            lateral = 0.0099999998f;
            depth = atLeastZero(static_cast<float>(static_cast<double>(control.halfExtents.z) - 0.035));
            break;
        case 2:
            lateral = -0.015f;
            break;
        case 3:
            lateral = 0.0099999998f;
            break;
        case 4:
            lateral = atLeastZero(static_cast<float>(static_cast<double>(control.halfExtents.x) - 0.045));
            break;
        case 5:
        case 9:
            lateral = static_cast<float>(static_cast<double>(control.gripExtents.x) - 0.02);
            depth = -0.02f;
            break;
        case 8:
            lateral = static_cast<float>(static_cast<double>(control.gripExtents.x) + 0.02);
            depth = -0.035f;
            break;
        default:
            lateral = atLeastZero(static_cast<float>(static_cast<double>(control.halfExtents.x) - 0.035));
            break;
        }
        const float halfY = control.halfExtents.y;
        float rise;
        if (mode >= 2 && mode <= 4) {
            float floor = static_cast<float>(static_cast<double>(halfY) - 0.035);
            if (-halfY > floor)
                floor = -halfY;
            const float length = mode == 3 ? control.halfExtents.x : control.halfExtents.z;
            const auto span = static_cast<float>(static_cast<double>(length) * 3600.0);
            const float clamped = 180.0f < span ? 180.0f : span;
            const auto angle = static_cast<std::uint16_t>(static_cast<std::int32_t>(clamped * 8192.0f / 45.0f));
            rise = static_cast<float>((static_cast<double>(sin_fixed16(static_cast<std::int16_t>(angle))) - 1.0) * 0.01 + static_cast<double>(floor));
            if (mode == 3)
                rise = static_cast<float>(static_cast<double>(rise) - 0.003);
        } else if (mode == 5 || mode == 9) {
            const auto partial = static_cast<float>(0.09 - static_cast<double>(halfY) + 0.055);
            rise = static_cast<float>(static_cast<double>(partial) - (0.08 - static_cast<double>(halfY)));
        } else if (mode == 8) {
            rise = static_cast<float>(0.09 - static_cast<double>(halfY));
            if (0.065 > static_cast<double>(rise))
                rise = 0.064999998f;
        } else {
            rise = static_cast<float>(0.09 - static_cast<double>(halfY));
            if (0.06 > static_cast<double>(rise))
                rise = 0.059999999f;
        }
        const Vector3 &base = control.handBase;
        control.handPoint[0] = {-lateral * cosine + base.x - depth * sine, rise + base.y, -lateral * sine + base.z + depth * cosine};
        control.handPoint[1] = {lateral * cosine + base.x - depth * sine, rise + base.y, lateral * sine + base.z + depth * cosine};
    }

    control.destination.x = control.handBase.x - control.reach * sine;
    if ((flags & 2U) != 0) {
        const double examinationHalfY = examination->halfExtents.y;
        control.destination.y = static_cast<float>(examinationHalfY + examinationHalfY + static_cast<double>(examination->modelBoundsCenterWorld.y) +
            static_cast<double>(control.halfExtents.y) + static_cast<double>(control.halfExtents.z) + 0.1);
    } else {
        control.destination.y = static_cast<float>(static_cast<double>(control.eye.y) - 0.25);
    }
    control.destination.z = control.reach * cosine + control.handBase.z;
    if (control.handBase.y > control.destination.y)
        g_cameraStateFlags = static_cast<std::uint16_t>(flags | 0x10U);
    else if (static_cast<double>(control.handBase.y) + 0.05 > static_cast<double>(control.destination.y))
        control.destination.y = static_cast<float>(static_cast<double>(control.handBase.y) + 0.05);
    const float dx = control.destination.x - control.eye.x;
    const float dy = control.destination.y - control.eye.y;
    const float dz = control.destination.z - control.eye.z;
    control.previousFov = currentPerspectiveFov;
    const float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    float wide = control.halfExtents.z > control.halfExtents.x ? control.halfExtents.z : control.halfExtents.x;
    if (0.025 > static_cast<double>(wide))
        wide = 0.025f;
    if (static_cast<double>(wide) > 0.4)
        wide = 0.40000001f;
    float tall = control.halfExtents.y;
    if (0.025 > static_cast<double>(tall))
        tall = 0.025f;
    if (static_cast<double>(tall) > 0.4)
        tall = 0.40000001f;
    const std::uint16_t wideFov = atan2_fixed16(static_cast<float>(static_cast<double>(wide) * 6.0), distance);
    control.targetFov = wideFov;
    const std::uint16_t tallFov = atan2_fixed16(static_cast<float>(static_cast<double>(tall) * 8.0), distance);
    if (wideFov < tallFov)
        control.targetFov = tallFov;
    if (control.targetFov < 1820)
        control.targetFov = 1820;
    if (control.targetFov > 20024)
        control.targetFov = 20024;

    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(control.yaw));
    Vector3i &baseA = control.handBaseRotation[0];
    Vector3i &baseB = control.handBaseRotation[1];
    switch (mode) {
    case 1:
        baseA = {0, control.yaw - 23665, 0};
        baseB = {0, control.yaw + 9102, 0};
        NormalizeEulerSigned16(baseA);
        NormalizeEulerSigned16(baseB);
        break;
    case 2:
    case 4:
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::RotateAroundY_Wrapper(0x8000);
        Matrix4x4::SetViewMatrixFromEuler(-9102, -30037, -19114);
        Matrix4x4_ExtractEulerAngles(baseA);
        Matrix4x4::Pop();
        Matrix4x4::SetViewMatrixFromEuler(9102, 30037, -19114);
        Matrix4x4_ExtractEulerAngles(baseB);
        break;
    case 3:
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::RotateAroundY_Wrapper(0x8000);
        Matrix4x4::SetViewMatrixFromEuler(-30947, -16384, 26396);
        Matrix4x4_ExtractEulerAngles(baseA);
        Matrix4x4::Pop();
        Matrix4x4::SetViewMatrixFromEuler(30947, 0x4000, 26396);
        Matrix4x4_ExtractEulerAngles(baseB);
        break;
    case 5:
    case 9:
        Matrix4x4::SetViewMatrixFromEuler(0, -16384, -21845);
        Matrix4x4_ExtractEulerAngles(baseB);
        baseA = baseB;
        break;
    case 6:
    case 7:
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::SetViewMatrixFromEuler(control.anchorRotation.y, control.anchorRotation.x, control.anchorRotation.z);
        Matrix4x4::RotateAroundZ_Wrapper(0xc000);
        Matrix4x4::RotateAroundX_Wrapper(0xc000);
        Matrix4x4::RotateAroundY_Wrapper(
            static_cast<std::uint16_t>(mode == 6 ? control.yaw - control.anchorRotation.z - 7281 : -3640 - control.anchorRotation.z));
        Matrix4x4_ExtractEulerAngles(baseB);
        baseA = baseB;
        break;
    case 8:
        baseA = {0, control.yaw + 14563, -3640};
        baseB = {0, control.yaw + 18204, -3640};
        Matrix4x4_ComposeFromRotation(baseA);
        Matrix4x4_ComposeFromRotation(baseB);
        break;
    default:
        baseA = {0, control.yaw - 7281, 0};
        baseB = {0, control.yaw + 25486, 0};
        NormalizeEulerSigned16(baseA);
        NormalizeEulerSigned16(baseB);
        break;
    }
    g_cachArmRotationA = control.armRotation[0];
    if (armA) {
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
        shenApplyTranslationToAll(armA->entity.node, control.handTarget[0]);
    }
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    Matrix4x4_ApplyViewTransformAndInverse(armA, anchorA, &baseA);
    g_cachArmRotationB = control.armRotation[1];
    if (armB) {
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
        shenApplyTranslationToAll(armB->entity.node, control.handTarget[1]);
    }
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    Matrix4x4_ApplyViewTransformAndInverse(armB, anchorB, &baseB);
    Matrix4x4::Pop();
    switch (mode) {
    case 2:
    case 3:
        control.speedCurrent = 20.0f;
        control.speedMinimum = 5.0f;
        control.speedMaximum = 85.0f;
        break;
    case 4:
        control.speedCurrent = 70.0f;
        control.speedMaximum = 80.0f;
        control.speedMinimum = 0.0f;
        break;
    case 5:
    case 9:
        control.speedCurrent = 50.0f;
        control.speedMaximum = 100.0f;
        control.speedMinimum = 0.0f;
        break;
    case 6:
    case 7:
        control.speedCurrent = 60.0f;
        control.speedMinimum = 20.0f;
        control.speedMaximum = 100.0f;
        break;
    case 8:
        control.speedCurrent = 50.0f;
        control.speedMinimum = 10.0f;
        control.speedMaximum = 90.0f;
        break;
    default:
        control.speedCurrent = 40.0f;
        control.speedMaximum = 80.0f;
        control.speedMinimum = 0.0f;
        break;
    }
    control.speedVelocity = 0.0f;
    anchorA.gripTarget = nullptr;
    std::memset(anchorA.fingerBlendLimits, 0, sizeof(anchorA.fingerBlendLimits));
    anchorB.gripTarget = nullptr;
    std::memset(anchorB.fingerBlendLimits, 0, sizeof(anchorB.fingerBlendLimits));
    HND_SetEffectFlags(anchorA, kTargetGripPoseDefault, 0.0f);
    HND_SetEffectFlags(anchorB, kTargetGripPoseDefault, 0.0f);
}


Vector3 HandFingerMidpoint(const HandModelWork &anchor) {
    Vector3 first{};
    Vector3 second{};
    std::memcpy(&first, anchor.jointPositions + 52 * sizeof(Vector3), sizeof(first));
    std::memcpy(&second, anchor.jointPositions + 56 * sizeof(Vector3), sizeof(second));
    return {static_cast<float>(static_cast<double>(second.x + first.x) * 0.5), first.y, static_cast<float>(static_cast<double>(second.z + first.z) * 0.5)};
}





void CalculateQuadraticCurves() {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    Vector3 sampleTimes{0.0f, 14.0f, 40.0f};
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_cachCurveStart.acceleration = control.viewEye;
    g_quadraticCurveRemaining = 40.0f;
    g_quadraticCurveProgress = 0.0f;
    const auto toward = [](float from, float to) { return (to - from) * 0.30000001f + from; };
    g_cachCurveStart.velocity = {toward(g_cachCurveStart.acceleration.x, g_cameraCurvePosition.x),
        toward(g_cachCurveStart.acceleration.y, g_cameraCurvePosition.y), toward(g_cachCurveStart.acceleration.z, g_cameraCurvePosition.z)};
    FitCachQuadraticCurve(sampleTimes, g_cachCurveStart);
    g_cachCurveEnd.acceleration = static_cast<std::uint32_t>(control.inputMode - 6) > 1U ? control.boundsCenter : control.handPoint[1];
    g_cachCurveEnd.velocity = {toward(g_cachCurveEnd.acceleration.x, g_cachCurveEnd.position.x),
        toward(g_cachCurveEnd.acceleration.y, g_cachCurveEnd.position.y), toward(g_cachCurveEnd.acceleration.z, g_cachCurveEnd.position.z)};
    FitCachQuadraticCurve(sampleTimes, g_cachCurveEnd);
    g_cachCurveAnchorA.acceleration = control.handPoint[0];
    g_cachCurveAnchorA.position = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorA));
    g_cachCurveAnchorA.velocity = {toward(g_cachCurveAnchorA.acceleration.x, g_cachCurveAnchorA.position.x),
        toward(g_cachCurveAnchorA.acceleration.y, g_cachCurveAnchorA.position.y), toward(g_cachCurveAnchorA.acceleration.z, g_cachCurveAnchorA.position.z)};
    g_cachCurveAnchorB.acceleration = control.handPoint[1];
    g_cachCurveAnchorB.position = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorB));
    g_cachCurveAnchorB.velocity = {toward(g_cachCurveAnchorB.acceleration.x, g_cachCurveAnchorB.position.x),
        toward(g_cachCurveAnchorB.acceleration.y, g_cachCurveAnchorB.position.y), toward(g_cachCurveAnchorB.acceleration.z, g_cachCurveAnchorB.position.z)};
    const std::int16_t mode = control.inputMode;
    if (static_cast<std::uint32_t>(mode - 2) <= 2U || mode == 7) {
        g_cachCurveAnchorA.velocity.y = static_cast<float>(static_cast<double>(control.handPoint[0].y) + 0.09);
        g_cachCurveAnchorB.velocity.y = static_cast<float>(static_cast<double>(control.handPoint[1].y) + 0.09);
    } else {
        float spread = 0.0f;
        if (mode != 1)
            spread = mode >= 8 ? 0.02f : 0.050000001f;
        const float sine = control.yawSine;
        const float cosine = control.yawCosine;
        const Vector3 &pointA = control.handPoint[0];
        const Vector3 &pointB = control.handPoint[1];
        const float riseA = g_cachCurveAnchorA.velocity.y - pointA.y;
        g_cachCurveAnchorA.velocity = {-spread * cosine + pointA.x - sine * 0.15000001f, riseA + pointA.y, -spread * sine + pointA.z + cosine * 0.15000001f};
        const float riseB = g_cachCurveAnchorB.velocity.y - pointB.y;
        g_cachCurveAnchorB.velocity = {spread * cosine + pointB.x - sine * 0.15000001f, riseB + pointB.y, spread * sine + pointB.z + cosine * 0.15000001f};
        if ((g_cameraStateFlags & 0x10U) != 0) {
            g_cachCurveAnchorA.velocity.y = g_cachCurveAnchorA.acceleration.y;
            g_cachCurveAnchorB.velocity.y = g_cachCurveAnchorB.acceleration.y;
        }
    }
    float lead = 20.0f;
    if (control.inputMode >= 8) {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        lead = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16) & 0x7fffU) * 0.000030517578125) > 0.5f ? 17.200001f : 22.799999f;
    }
    sampleTimes.y = lead;
    FitCachQuadraticCurve(sampleTimes, g_cachCurveAnchorA);
    sampleTimes.y = 40.0f - lead;
    FitCachQuadraticCurve(sampleTimes, g_cachCurveAnchorB);
}



void ApplyFingerPositionToEntity(Character *arm, const HandModelWork &anchor, const Vector3 &target) {
    const Vector3 fingers = HandFingerMidpoint(anchor);
    const float dy = target.y - fingers.y;
    const float dx = target.x - fingers.x;
    const float dz = target.z - fingers.z;
    const Vector3 position = arm ? GetPositionForEntity(arm->entity.node) : Vector3{};
    if (arm)
        shenApplyTranslationToAll(arm->entity.node, {position.x + dx, position.y + dy, position.z + dz});
}



bool AdvanceTargetReach() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 handA = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 handB = EvaluateCachCurve(g_cachCurveAnchorB, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    const Vector3 target = EvaluateCachCurve(g_cachCurveEnd, t);
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    BlendEulerAngles(g_cachArmRotationA, control.armJointRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, control.armJointRotation[1], blend);
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float opacity = control.savedPlayerOpacity < ryo->renderOpacity ? control.savedPlayerOpacity : ryo->renderOpacity;
    const auto faded = static_cast<float>((0.0 - static_cast<double>(opacity)) * static_cast<double>(blend) + static_cast<double>(opacity));
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fov = static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(10922 - currentFov)) * blend + static_cast<float>(currentFov));
    matrix_stack::Matrix4x4_Push();
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    ApplyFingerPositionToEntity(armA, anchorA, handA);
    Matrix4x4_ApplyViewTransformAndInverse(armA, anchorA, &control.handBaseRotation[0]);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    ApplyFingerPositionToEntity(armB, anchorB, handB);
    Matrix4x4_ApplyViewTransformAndInverse(armB, anchorB, &control.handBaseRotation[1]);
    Matrix4x4::Pop();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    ryo->renderOpacity = faded;
    CAME_SetFieldOfView(fov);
    return finished;
}




void CaptureTargetGrip() {
    matrix_stack::Matrix4x4_Push();
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    Character *const target = control.chara;
    control.savedLink = target->entity.node.child;
    control.savedPosition = target ? GetPositionForEntity(target->entity.node) : Vector3{};
    control.savedRotation = control.chara ? GetEulerAnglesForEntity(control.chara->entity.node) : Vector3i{};
    NormalizeEulerSigned16(control.savedRotation);
    control.chara->entity.node.child = nullptr;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(control.gripOffset[1].x, control.gripOffset[1].y, control.gripOffset[1].z);
    Matrix4x4::preTranslate(control.gripOffset[0].x, control.gripOffset[0].y, control.gripOffset[0].z);
    Matrix4x4::preTranslate(control.gripOffset[2].x, control.gripOffset[2].y, control.gripOffset[2].z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(control.yaw));
    Matrix4x4::SetViewMatrixFromEuler(control.gripRotation.y, control.gripRotation.x, control.gripRotation.z);
    Matrix4x4::InvertInPlace(nullptr);
    Matrix4x4::preTranslate(control.savedPosition.x, control.savedPosition.y, control.savedPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(control.savedRotation.y, control.savedRotation.x, control.savedRotation.z);
    control.gripTransform = workTransformMatrix;
    if (control.chara)
        shenApplyTranslationToAll(control.chara->entity.node, control.savedPosition);
    if (control.chara)
        ApplyEulerAnglesToNode(control.chara->entity.node, control.savedRotation);
    Matrix4x4::Pop();
}



bool UpdateAccelerationCurve() {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    const auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    if (static_cast<std::uint32_t>(control.inputMode - 6) <= 1U) {
        g_cameraCurvePosition = control.viewEye;
        g_temporaryAcceleratedVector = HandFingerMidpoint(anchorB);
    }
    return (anchorB.control & 8U) != 0;
}



void CalculateMatrixEulerAngles() {
    matrix_stack::Matrix4x4_Push();
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(control.gripOffset[1].x, control.gripOffset[1].y, control.gripOffset[1].z);
    Matrix4x4::preTranslate(control.gripOffset[0].x, control.gripOffset[0].y, control.gripOffset[0].z);
    Matrix4x4::preTranslate(control.gripOffset[2].x, control.gripOffset[2].y, control.gripOffset[2].z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(control.yaw));
    Matrix4x4::SetViewMatrixFromEuler(control.gripRotation.y, control.gripRotation.x, control.gripRotation.z);
    Matrix4x4::Multiply(&control.gripTransform);
    const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Vector3i euler{};
    Matrix4x4_ExtractEulerAngles(euler);
    if (control.chara)
        shenApplyTranslationToAll(control.chara->entity.node, position);
    if (control.chara)
        ApplyEulerAnglesToNode(control.chara->entity.node, euler);
    Matrix4x4::Pop();
}



void TransformMatrixCoordinatesAndCalculateEulerAngles() {
    matrix_stack::Matrix4x4_Push();
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (armA)
        armA->entity.node.rotation = control.armHoldRotation[0];
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    HND_ApplyRootEuler(anchorA, control.handHoldRotation[0]);
    ApplyHandRootTransform(armA, anchorA);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    Matrix4x4_ApplyViewTransformAndInverse(armA, anchorA, nullptr);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armB)
        armB->entity.node.rotation = control.armHoldRotation[1];
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    HND_ApplyRootEuler(anchorB, control.handHoldRotation[1]);
    ApplyHandRootTransform(armB, anchorB);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    Matrix4x4_ApplyViewTransformAndInverse(armB, anchorB, nullptr);
    Matrix4x4::Pop();
}




void ResetCurve() {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    g_quadraticCurveRemaining = 60.0f;
    g_quadraticCurveProgress = 0.0f;
    Vector3 sampleTimes{0.0f, 0.0f, 60.0f};
    g_cachCurveAnchorA.acceleration = control.destination;
    g_cachCurveAnchorA.position = control.gripOffset[0];
    g_cachCurveStart.acceleration = control.eye;
    g_cachCurveStart.position = g_cameraCurvePosition;
    const auto blend = [](float from, float to, double amount) {
        return static_cast<float>(static_cast<double>(to - from) * amount + static_cast<double>(from));
    };
    const Vector3 &base = control.handBase;
    const Vector3 &destination = control.destination;
    const Vector3 &eye = g_cachCurveStart.acceleration;
    if ((g_cameraStateFlags & 0x10U) != 0) {
        g_cachCurveAnchorA.velocity = {
            blend(base.x, destination.x, 0.7), static_cast<float>(static_cast<double>(base.y) + 0.12), blend(base.z, destination.z, 0.7)};
        g_cachCurveStart.velocity = {
            blend(eye.x, g_cameraCurvePosition.x, 0.3), blend(eye.y, g_cameraCurvePosition.y, 0.7), blend(eye.z, g_cameraCurvePosition.z, 0.3)};
        sampleTimes.y = 33.0f;
    } else {
        g_cachCurveAnchorA.velocity = {blend(base.x, destination.x, 0.3), blend(base.y, destination.y, 0.5), blend(base.z, destination.z, 0.3)};
        g_cachCurveStart.velocity = {
            blend(eye.x, g_cameraCurvePosition.x, 0.7), blend(eye.y, g_cameraCurvePosition.y, 0.5), blend(eye.z, g_cameraCurvePosition.z, 0.7)};
        sampleTimes.y = 39.0f;
    }
    FitCachQuadraticCurve(sampleTimes, g_cachCurveStart);
    sampleTimes.y = kSeqFramesPerSecond;
    FitCachQuadraticCurve(sampleTimes, g_cachCurveAnchorA);
    if (auto *const armA = static_cast<Character *>(g_cameraTargetEntity))
        control.armHoldRotation[0] = armA->entity.node.rotation;
    control.handHoldRotation[0] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA));
    if (auto *const armB = static_cast<Character *>(g_shenmueCamera0))
        control.armHoldRotation[1] = armB->entity.node.rotation;
    control.handHoldRotation[1] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB));
    CalculateMatrixEulerAngles();
    TransformMatrixCoordinatesAndCalculateEulerAngles();
}





bool UpdateCurveAndApply() {
    const bool finished = AdvanceQuadraticCurve(3.0f, 0.2f);
    const float remaining = g_quadraticCurveRemaining;
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    control.gripOffset[0] = EvaluateCachCurve(g_cachCurveAnchorA, remaining);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, remaining);
    Vector3i gripTarget{};
    Vector3i armTargetA{};
    Vector3i armTargetB{};
    float frames = 0.0f;
    if (remaining <= kSeqFramesPerSecond || (g_cameraStateFlags & 0x10U) == 0) {
        ConvertTargetSpeedToEuler(control, gripTarget);
        armTargetA = control.armMidRotation[0];
        armTargetB = control.armMidRotation[1];
        frames = remaining;
    } else {
        gripTarget = control.gripRotation;
        armTargetA = g_cachArmRotationA;
        armTargetB = g_cachArmRotationB;
    }
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(frames) + 1.0));
    BlendEulerAngles(control.gripRotation, gripTarget, blend);
    BlendEulerAngles(g_cachArmRotationA, armTargetA, blend);
    BlendEulerAngles(g_cachArmRotationB, armTargetB, blend);
    Vector3 target = control.gripOffset[0];
    if (static_cast<std::uint32_t>(control.inputMode - 6) <= 1U) {
        const Vector3 fingers = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorB));
        const Vector3 &hold = control.gripOffset[0];
        float amount = 0.0f;
        if (!(2.0f > remaining)) {
            const auto fraction = static_cast<float>(static_cast<double>(remaining) / 60.0);
            amount = 1.0f < fraction ? 1.0f : fraction;
        }
        target = {(fingers.x - hold.x) * amount + hold.x, (fingers.y - hold.y) * amount + hold.y, (fingers.z - hold.z) * amount + hold.z};
    }
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fovDelta = static_cast<std::int16_t>(control.targetFov - currentFov);
    CalculateMatrixEulerAngles();
    TransformMatrixCoordinatesAndCalculateEulerAngles();
    const auto ease = static_cast<float>(1.0 / (static_cast<double>(remaining) + 1.0));
    ConfigureTargetModeCamera(control, eye, target, static_cast<std::int32_t>(static_cast<float>(fovDelta) * ease + static_cast<float>(currentFov)));
    auto *const camera0 = static_cast<Character *>(g_shenmueCamera0);
    control.camera0Position = camera0 ? GetPositionForEntity(camera0->entity.node) : Vector3{};
    return finished;
}




bool AdvanceCameraRailCurveForFieldOfView() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    control.gripOffset[0] = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    const Vector3 target = EvaluateCachCurve(g_cachCurveEnd, t);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    Vector3i gripTarget{};
    ConvertTargetSpeedToEuler(control, gripTarget);
    BlendEulerAngles(control.gripRotation, gripTarget, blend);
    BlendEulerAngles(g_cachArmRotationA, control.armEndRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, control.armEndRotation[1], blend);
    for (std::int32_t index = 1; index < 3; ++index) {
        Vector3 &offset = control.gripOffset[index];
        offset.x = offset.x - offset.x * blend;
        offset.y = offset.y - offset.y * blend;
        offset.z = offset.z - offset.z * blend;
    }
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fovDelta = static_cast<std::int16_t>(10922 - currentFov);
    const float opacity = ryo->renderOpacity + (control.savedPlayerOpacity - ryo->renderOpacity) * blend;
    CalculateMatrixEulerAngles();
    TransformMatrixCoordinatesAndCalculateEulerAngles();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    ryo->renderOpacity = opacity;
    CAME_SetFieldOfView(static_cast<std::int32_t>(static_cast<float>(fovDelta) * blend + static_cast<float>(currentFov)));
    return finished;
}




bool AdjustCameraPositionAndOrientationBasedOnDistance() {
    const bool finished = AdvanceQuadraticCurve(3.0f, 0.15f);
    const float t = g_quadraticCurveRemaining;
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    control.gripOffset[0] = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    const float frames = t <= kSeqFramesPerSecond || (g_cameraStateFlags & 0x10U) == 0 ? t : static_cast<float>(static_cast<double>(t) - 30.0);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(frames) + 1.0));
    BlendEulerAngles(control.gripRotation, control.referenceRotation, blend);
    BlendEulerAngles(g_cachArmRotationA, control.armJointRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, control.armJointRotation[1], blend);
    const Vector3 &hold = control.gripOffset[0];
    Vector3 point = hold;
    if (static_cast<std::uint32_t>(control.inputMode - 6) <= 1U) {
        const Vector3 fingers = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorB));
        float amount = 0.0f;
        if (!(2.0f > t)) {
            const auto fraction = static_cast<float>(static_cast<double>(t) / 60.0);
            amount = 1.0f < fraction ? 1.0f : fraction;
        }
        const auto weight = static_cast<float>(1.0 - static_cast<double>(amount));
        point = {(fingers.x - hold.x) * weight + hold.x, (fingers.y - hold.y) * weight + hold.y, (fingers.z - hold.z) * weight + hold.z};
    }
    const auto ease = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    for (std::int32_t index = 1; index < 3; ++index) {
        Vector3 &offset = control.gripOffset[index];
        offset.x = offset.x - offset.x * ease;
        offset.y = offset.y - offset.y * ease;
        offset.z = offset.z - offset.z * ease;
    }
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fovDelta = static_cast<std::int16_t>(10922 - currentFov);
    const auto toward = [](float offset, float base) { return static_cast<float>(static_cast<double>(offset) * 0.7 + static_cast<double>(base)); };
    const Vector3 target{toward(control.gripOffset[1].x, point.x), toward(control.gripOffset[1].y, point.y), toward(control.gripOffset[1].z, point.z)};
    CalculateMatrixEulerAngles();
    TransformMatrixCoordinatesAndCalculateEulerAngles();
    ConfigureTargetModeCamera(control, eye, target, static_cast<std::int32_t>(static_cast<float>(fovDelta) * ease + static_cast<float>(currentFov)));
    return finished;
}



void BuildTargetReleaseCurves() {
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    Vector3 sampleTimes{0.0f, 26.0f, 40.0f};
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_quadraticCurveRemaining = 40.0f;
    g_quadraticCurveProgress = 0.0f;
    if (camera.state == 0) {
        SRCH_UpdateCameraEyeAndTarget(control.savedEye, control.savedTarget);
    } else {
        control.savedEye = camera.lastCurvePosition;
        control.savedTarget = camera.lastAcceleratedTarget;
        control.previousFov = camera.examineFov;
        camera.analogDelta = {};
    }
    float spread = 0.2f;
    float forward = 0.2f;
    const float lowest = g_cachCurveStart.position.y < control.savedEye.y ? g_cachCurveStart.position.y : control.savedEye.y;
    auto drop = static_cast<float>(static_cast<double>(lowest) - 0.35);
    if ((g_cameraStateFlags & 2U) != 0) {
        spread = 0.25f;
        forward = -0.049999997f;
        drop = static_cast<float>(static_cast<double>(drop) + 0.1);
    }
    const auto toward = [](float from, float to) { return (to - from) * 0.69999999f + from; };
    g_cachCurveStart.acceleration = control.savedEye;
    g_cachCurveStart.velocity = {toward(g_cachCurveStart.acceleration.x, g_cachCurveStart.position.x),
        toward(g_cachCurveStart.acceleration.y, g_cachCurveStart.position.y), toward(g_cachCurveStart.acceleration.z, g_cachCurveStart.position.z)};
    FitCachQuadraticCurve(sampleTimes, g_cachCurveStart);
    g_cachCurveEnd.acceleration = control.savedTarget;
    g_cachCurveEnd.velocity = {toward(g_cachCurveEnd.acceleration.x, g_cachCurveEnd.position.x),
        toward(g_cachCurveEnd.acceleration.y, g_cachCurveEnd.position.y), toward(g_cachCurveEnd.acceleration.z, g_cachCurveEnd.position.z)};
    FitCachQuadraticCurve(sampleTimes, g_cachCurveEnd);
    const float sine = control.yawSine;
    const float cosine = control.yawCosine;
    const Vector3 &eye = control.savedEye;
    const float rise = drop - eye.y;
    g_cachCurveAnchorA.acceleration = {-spread * cosine + eye.x - forward * sine, rise + eye.y, -spread * sine + eye.z + forward * cosine};
    g_cachCurveAnchorA.position = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorA));
    g_cachCurveAnchorA.velocity = {toward(g_cachCurveAnchorA.acceleration.x, g_cachCurveAnchorA.position.x),
        toward(g_cachCurveAnchorA.acceleration.y, g_cachCurveAnchorA.position.y), toward(g_cachCurveAnchorA.acceleration.z, g_cachCurveAnchorA.position.z)};
    g_cachCurveAnchorB.acceleration = {spread * cosine + eye.x - forward * sine, rise + eye.y, spread * sine + eye.z + forward * cosine};
    g_cachCurveAnchorB.position = HandFingerMidpoint(*static_cast<HandModelWork *>(g_cameraAnchorB));
    g_cachCurveAnchorB.velocity = {toward(g_cachCurveAnchorB.acceleration.x, g_cachCurveAnchorB.position.x),
        toward(g_cachCurveAnchorB.acceleration.y, g_cachCurveAnchorB.position.y), toward(g_cachCurveAnchorB.acceleration.z, g_cachCurveAnchorB.position.z)};
    const std::int16_t mode = control.inputMode;
    if (static_cast<std::uint32_t>(mode - 2) <= 2U || mode == 7) {
        g_cachCurveAnchorA.velocity.y = static_cast<float>(static_cast<double>(control.handPoint[0].y) + 0.09);
        g_cachCurveAnchorB.velocity.y = static_cast<float>(static_cast<double>(control.handPoint[1].y) + 0.09);
    } else {
        const float dz = g_cachCurveAnchorA.acceleration.z - g_cachCurveAnchorA.position.z;
        const float dx = g_cachCurveAnchorA.acceleration.x - g_cachCurveAnchorA.position.x;
        const double reach = std::sqrt(dz * dz + dx * dx);
        float side = 0.0f;
        double bow;
        if (mode == 1) {
            bow = reach * 0.5;
        } else if (mode >= 8) {
            side = 0.02f;
            bow = reach * 0.4;
        } else {
            side = 0.079999998f;
            bow = reach * 0.2;
        }
        const auto out = static_cast<float>(bow);
        const Vector3 &pointA = control.handPoint[0];
        const Vector3 &pointB = control.handPoint[1];
        const float riseA = g_cachCurveAnchorA.velocity.y - pointA.y;
        g_cachCurveAnchorA.velocity = {-side * cosine + pointA.x - out * sine, riseA + pointA.y, -side * sine + pointA.z + out * cosine};
        const float riseB = g_cachCurveAnchorB.velocity.y - pointB.y;
        g_cachCurveAnchorB.velocity = {side * cosine + pointB.x - out * sine, riseB + pointB.y, side * sine + pointB.z + out * cosine};
        if ((g_cameraStateFlags & 0x10U) != 0) {
            g_cachCurveAnchorA.velocity.y = g_cachCurveAnchorA.position.y;
            g_cachCurveAnchorB.velocity.y = g_cachCurveAnchorB.position.y;
        }
    }
    float lead = 20.0f;
    if (control.inputMode >= 8) {
        g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
        lead = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16) & 0x7fffU) * 0.000030517578125) > 0.5f ? 17.200001f : 22.799999f;
    }
    sampleTimes.y = lead;
    FitCachQuadraticCurve(sampleTimes, g_cachCurveAnchorA);
    sampleTimes.y = 40.0f - lead;
    FitCachQuadraticCurve(sampleTimes, g_cachCurveAnchorB);
}




bool UpdateCameraAlongCurve() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 handA = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 handB = EvaluateCachCurve(g_cachCurveAnchorB, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    const Vector3 target = EvaluateCachCurve(g_cachCurveEnd, t);
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    BlendEulerAngles(g_cachArmRotationA, control.armRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, control.armRotation[1], blend);
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const std::int32_t currentFov = currentPerspectiveFov;
    const float opacity = ryo->renderOpacity + (control.savedPlayerOpacity - ryo->renderOpacity) * blend;
    const auto fov =
        static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(control.previousFov - currentFov)) * blend + static_cast<float>(currentFov));
    matrix_stack::Matrix4x4_Push();
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    ApplyFingerPositionToEntity(armA, anchorA, handA);
    Matrix4x4_ApplyViewTransformAndInverse(armA, anchorA, &control.handBaseRotation[0]);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    ApplyFingerPositionToEntity(armB, anchorB, handB);
    Matrix4x4_ApplyViewTransformAndInverse(armB, anchorB, &control.handBaseRotation[1]);
    Matrix4x4::Pop();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    ryo->renderOpacity = opacity;
    CAME_SetFieldOfView(fov);
    if ((anchorB.control & 8U) != 0)
        camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, t, nullptr);
    return finished;
}



void Matrix4x4_UpdateTransformMatrices_Recursive(Mt5RuntimeNode *node, Matrix4x4 &primary, Matrix4x4 &secondary) {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    while (true) {
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        if (node == camera.primaryTransform)
            primary = workTransformMatrix;
        if (node == camera.secondaryTransform)
            secondary = workTransformMatrix;
        if (node->childOffset)
            Matrix4x4_UpdateTransformMatrices_Recursive(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), primary, secondary);
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop();
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
}




void UpdateCameraPosAndRot() {
    matrix_stack::Matrix4x4_Push();
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 primary = transformMatrix_prePass;
    Matrix4x4 secondary = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4_UpdateTransformMatrices_Recursive(static_cast<Mt5RuntimeNode *>(camera.chara->model), primary, secondary);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    const auto placeArm = [](Character *arm) {
        const Vector3 armPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Vector3i armRotation{};
        Matrix4x4_ExtractEulerAngles(armRotation);
        if (arm) {
            shenApplyTranslationToAll(arm->entity.node, armPosition);
            ApplyEulerAnglesToNode(arm->entity.node, armRotation);
        }
    };
    workTransformMatrix = primary;
    Matrix4x4::Multiply(&camera.primaryAuthoredTransform);
    placeArm(armA);
    workTransformMatrix = secondary;
    Matrix4x4::Multiply(&camera.secondaryAuthoredTransform);
    placeArm(armB);
    const auto keepHand = [](Character *arm, HandModelWork &anchor, Vector3i &handRotation, const Vector3i &armRotation) {
        HND_ApplyRootEuler(anchor, handRotation);
        ApplyHandRootTransform(arm, anchor);
        if (arm)
            ApplyEulerAnglesToNode(arm->entity.node, armRotation);
        const Matrix4x4 handFrame = workTransformMatrix;
        HND_ApplyRootEuler(anchor, {});
        ApplyHandRootTransform(arm, anchor);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(&handFrame);
        Matrix4x4_ExtractEulerAngles(handRotation);
        HND_ApplyRootEuler(anchor, handRotation);
    };
    g_cachHandRotationA = camera.handBaseRotation[0];
    keepHand(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, g_cachArmRotationA);
    g_cachHandRotationB = camera.handBaseRotation[1];
    keepHand(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, g_cachArmRotationB);
    Matrix4x4::Pop();
}




void ManipulateTransformationMatrix(float x, float y, float z) {
    matrix_stack::Matrix4x4_Push();
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 primary = transformMatrix_prePass;
    Matrix4x4 secondary = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4_UpdateTransformMatrices_Recursive(static_cast<Mt5RuntimeNode *>(camera.chara->model), primary, secondary);
    const auto toWorld = [](const Matrix4x4 &frame, CachHandPlacement &hand, float nudgeX, float nudgeY, float nudgeZ) {
        workTransformMatrix = frame;
        Matrix4x4::preTranslate(hand.position.x, hand.position.y, hand.position.z);
        const Vector3 origin{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Matrix4x4::preTranslate(nudgeX, nudgeY, nudgeZ);
        hand.position = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        workTransformMatrix = frame;
        Matrix4x4::preTranslate(hand.step.x, hand.step.y, hand.step.z);
        hand.step = {workTransformMatrix.rows[3].x - origin.x, workTransformMatrix.rows[3].y - origin.y, workTransformMatrix.rows[3].z - origin.z};
    };
    toWorld(primary, camera.hand[0], -x, y, z);
    toWorld(secondary, camera.hand[1], x, y, z);
    Matrix4x4::Pop();
}





void CaptureSlidingGrip() {
    matrix_stack::Matrix4x4_Push();
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 primary = transformMatrix_prePass;
    Matrix4x4 secondary = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4_UpdateTransformMatrices_Recursive(static_cast<Mt5RuntimeNode *>(camera.chara->model), primary, secondary);
    Matrix4x4::InvertInPlace(&primary);
    Matrix4x4::InvertInPlace(&secondary);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    const Vector3 armAPosition = armA ? GetPositionForEntity(armA->entity.node) : Vector3{};
    workTransformMatrix = primary;
    Matrix4x4::preTranslate(armAPosition.x, armAPosition.y, armAPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(g_cachArmRotationA.y, g_cachArmRotationA.x, g_cachArmRotationA.z);
    camera.primaryAuthoredTransform = workTransformMatrix;
    camera.handBaseRotation[0] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA));
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    const Vector3 armBPosition = armB ? GetPositionForEntity(armB->entity.node) : Vector3{};
    workTransformMatrix = secondary;
    Matrix4x4::preTranslate(armBPosition.x, armBPosition.y, armBPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(g_cachArmRotationB.y, g_cachArmRotationB.x, g_cachArmRotationB.z);
    camera.secondaryAuthoredTransform = workTransformMatrix;
    camera.handBaseRotation[1] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB));
    const auto local = [](const Matrix4x4 &frame, const Vector3 &point) {
        workTransformMatrix = frame;
        Matrix4x4::preTranslate(point.x, point.y, point.z);
        return Vector3{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    };
    CachHandPlacement &handA = camera.hand[0];
    handA.step = local(primary, {handA.position.x + handA.step.x, handA.position.y + handA.step.y, handA.position.z + handA.step.z});
    handA.position = local(primary, handA.position);
    CachHandPlacement &handB = camera.hand[1];
    handB.step = local(secondary, {handB.step.x + handB.position.x, handB.step.y + handB.position.y, handB.step.z + handB.position.z});
    handB.position = local(secondary, handB.position);
    Matrix4x4::Pop();
}





void PlaceSlidingExaminationHands() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (camera.primaryTransform && armA)
        ShowCachCharacter(armA);
    if (camera.secondaryTransform && armB)
        ShowCachCharacter(armB);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    camera.hand[0].position = {cosine * -0.0f + camera.eye.x - sine * -0.015f, camera.eye.y + 0.0f, sine * -0.0f + camera.eye.z - cosine * 0.015f};
    camera.hand[1].position = {
        cosine * 0.0f + camera.acceleration.x - sine * -0.015f, camera.acceleration.y + 0.0f, sine * 0.0f + camera.acceleration.z - cosine * 0.015f};
    camera.hand[0].step.y = 0.0f;
    camera.hand[1].step.y = 0.0f;
    const auto fifth = [](float value) { return static_cast<float>(static_cast<double>(value) * 0.2); };
    camera.hand[0].step.x = fifth(-(cosine * 0.0f) - sine * -0.02f);
    camera.hand[0].step.z = fifth(cosine * -0.02f - sine * 0.0f);
    camera.hand[1].step.x = fifth(cosine * 0.0f - sine * -0.02f);
    camera.hand[1].step.z = fifth(cosine * -0.02f + sine * 0.0f);
    const float lowest = camera.eyePosition.y < camera.previousEye.y ? camera.eyePosition.y : camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(camera.previousEye.y) + 0.35));
    const Vector3 &saved = camera.previousEye;
    camera.handTarget[0] = {saved.x - cosine * 0.1f - sine * 0.2f, drop + saved.y, saved.z - sine * 0.1f + cosine * 0.2f};
    camera.handTarget[1] = {cosine * 0.1f + saved.x - sine * 0.2f, drop + saved.y, sine * 0.1f + saved.z + cosine * 0.2f};
    auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform);
    auto slide = static_cast<float>(static_cast<double>(camera.halfExtents.x) * 1.5);
    if (primary) {
        if (secondary)
            slide = static_cast<float>(static_cast<double>(slide) * 0.5);
        camera.acceleration3.x = slide + primary->position.x;
    }
    if (secondary)
        camera.smoothedInput.x = secondary->position.x - slide;
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float ryoY = ryo ? GetPositionForEntity(ryo->entity.node).y : 0.0f;
    auto height = static_cast<float>(static_cast<double>(camera.targetPosition.y - ryoY) - 1.4);
    if (static_cast<double>(height) > 0.8)
        height = 0.80000001f;
    height = -0.5f > height ? -0.5f : height;
    float lean;
    float leanB;
    if (0.0f > height) {
        lean = static_cast<float>(static_cast<double>(height) * 30.0);
        leanB = static_cast<float>(static_cast<double>(lean) * 3.0);
    } else {
        lean = static_cast<float>(static_cast<double>(height) * 50.0);
        leanB = lean;
    }
    const std::int32_t yaw = camera.yaw;
    const auto toAngle = [](float degrees) {
        return static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(degrees) + 90.0) * 8192.0 / 45.0)));
    };
    camera.armJointRotation[1].x = 0;
    camera.armJointRotation[0].x = 0;
    camera.armJointRotation[0].y = yaw + 0x38e3;
    camera.armEndRotation[1].x = 0;
    camera.armJointRotation[1].y = yaw + 0x471c;
    camera.armEndRotation[0].x = 0;
    const std::int32_t jointLean = toAngle(lean);
    camera.armJointRotation[1].z = jointLean;
    camera.armJointRotation[0].z = jointLean;
    camera.armEndRotation[0].y = yaw + 0x238e;
    camera.armEndRotation[1].z = jointLean;
    camera.armEndRotation[1].y = yaw + 0x5c71;
    camera.armEndRotation[0].z = jointLean;
    camera.armRotation[1].x = 0;
    camera.armRotation[0].x = 0;
    camera.armRotation[1].y = yaw + 0x4000;
    camera.armRotation[0].y = yaw + 0x4000;
    const std::int32_t armLean = toAngle(leanB);
    camera.armRotation[1].z = armLean;
    camera.armRotation[0].z = armLean;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-15473));
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
    Matrix4x4::Pop();
    Matrix4x4::RotateAroundY_Wrapper(15473);
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    g_cachArmRotationA = camera.armRotation[0];
    g_cachArmRotationB = camera.armRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, camera.armJointRotation[0]);
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationA = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, camera.armJointRotation[1]);
    camera.handRotation[1] = camera.handBaseRotation[1];
    g_cachHandRotationB = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    Matrix4x4::Pop();
}




void PlaceHandsForSlidingRelease() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    void *const primary = camera.primaryTransform;
    camera.primaryTransform = camera.secondaryTransform;
    camera.secondaryTransform = primary;
    const Vector3 handPosition = camera.hand[0].position;
    camera.hand[0].position = camera.hand[1].position;
    camera.hand[1].position = handPosition;
    const float appliedX = camera.appliedPosition.x;
    camera.appliedPosition.x = camera.secondaryTranslationScalar;
    camera.secondaryTranslationScalar = appliedX;
    if (camera.primaryTransform && armA)
        ShowCachCharacter(armA);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (camera.secondaryTransform && armB)
        ShowCachCharacter(armB);
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    g_cachArmRotationA = camera.armJointRotation[0];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    g_cachHandRotationA = camera.handBaseRotation[0];
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA);
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    g_cachHandRotationB = camera.handBaseRotation[1];
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB);
}



bool camCtrlResetForLooped() {
    const float remaining = g_quadraticCurveRemaining + -2.0f;
    g_quadraticCurveRemaining = remaining;
    bool finished = false;
    if (!(g_quadraticCurveRemaining >= 2.0f)) {
        g_quadraticCurveRemaining = -2.0f;
        finished = remaining < -2.0f;
    }
    if (!finished) {
        auto &camera = *static_cast<CachContextState *>(g_camera);
        ApplyPositionDeltaToEntity(static_cast<Character *>(g_cameraTargetEntity), camera.hand[0].step);
        ApplyPositionDeltaToEntity(static_cast<Character *>(g_shenmueCamera0), camera.hand[1].step);
    }
    return finished;
}


bool ApplyRelativeSmoothedCameraPos() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 handA = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 handB = EvaluateCachCurve(g_cachCurveAnchorB, t);
    ApplyRelativePositionToEntity(static_cast<Character *>(g_cameraTargetEntity), *static_cast<HandModelWork *>(g_cameraAnchorA), handA);
    ApplyRelativePositionToEntity(static_cast<Character *>(g_shenmueCamera0), *static_cast<HandModelWork *>(g_cameraAnchorB), handB);
    return finished;
}




bool SlideExaminationPart() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(g_quadraticCurveRemaining) + 1.0));
    auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform);
    float primaryX = 0.0f;
    if (primary)
        primaryX = primary->position.x + (camera.acceleration3.x - primary->position.x) * blend;
    float secondaryX = 0.0f;
    if (secondary)
        secondaryX = secondary->position.x + (camera.smoothedInput.x - secondary->position.x) * blend;
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], blend);
    const Vector3 eye{(camera.eyePosition.x - g_cameraCurvePosition.x) * blend + g_cameraCurvePosition.x,
        g_cameraCurvePosition.y + (camera.eyePosition.y - g_cameraCurvePosition.y) * blend,
        g_cameraCurvePosition.z + (camera.eyePosition.z - g_cameraCurvePosition.z) * blend};
    if (primary)
        primary->position.x = primaryX;
    if (secondary)
        secondary->position.x = secondaryX;
    UpdateCameraPosAndRot();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}



bool SlideExaminationPartBack() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(g_quadraticCurveRemaining) + 1.0));
    auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform);
    float primaryX = 0.0f;
    if (primary)
        primaryX = primary->position.x + (camera.appliedPosition.x - primary->position.x) * blend;
    float secondaryX = 0.0f;
    if (secondary)
        secondaryX = secondary->position.x + (camera.secondaryTranslationScalar - secondary->position.x) * blend;
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], blend);
    const Vector3 eye{(camera.cameraAux.x - g_cameraCurvePosition.x) * blend + g_cameraCurvePosition.x,
        g_cameraCurvePosition.y + (camera.cameraAux.y - g_cameraCurvePosition.y) * blend,
        g_cameraCurvePosition.z + (camera.cameraAux.z - g_cameraCurvePosition.z) * blend};
    if (primary)
        primary->position.x = primaryX;
    if (secondary)
        secondary->position.x = secondaryX;
    UpdateCameraPosAndRot();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}




void BeginSlidingExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (camera.primaryTransform)
        camera.appliedPosition.x = static_cast<Mt5RuntimeNode *>(camera.primaryTransform)->position.x;
    if (camera.secondaryTransform)
        camera.secondaryTranslationScalar = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform)->position.x;
    camera.targetPosition = camera.modelBoundsCenterWorld;
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    camera.eyePosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    camera.eyePosition.y = static_cast<float>(static_cast<double>(camera.eyePosition.y) + 1.65);
    const auto top = static_cast<float>(static_cast<double>(camera.halfExtents.y) * 1.5 + static_cast<double>(camera.modelBoundsCenterWorld.y));
    if (camera.eyePosition.y > top)
        camera.eyePosition.y = top;
    const float bottom = camera.modelBoundsCenterWorld.y - camera.halfExtents.y;
    if (bottom > camera.eyePosition.y)
        camera.eyePosition.y = bottom;
    float radius = static_cast<float>(static_cast<double>(camera.halfExtents.x) + static_cast<double>(camera.halfExtents.x));
    radius = 0.25f > radius ? 0.25f : radius;
    if (static_cast<double>(radius) > 0.65)
        radius = 0.64999998f;
    camera.eyePosition.x = camera.targetPosition.x - radius * camera.yawSine;
    camera.eyePosition.z = radius * camera.yawCosine + camera.targetPosition.z;
    camera.cameraAux = camera.eyePosition;
}


std::int32_t PollSlidingExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    const std::int32_t step = g_cameraGlobalState;
    const auto handPoint = [](const HandModelWork &anchor) {
        Vector3 point{};
        std::memcpy(&point, anchor.jointPositions + 60 * sizeof(Vector3), sizeof(point));
        return point;
    };
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            PlaceSlidingExaminationHands();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(kTargetPoseMode8, static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.85), nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (AdjustCameraViewAndFoV()) {
                g_quadraticCurveRemaining = 10.0f;
                camCtrlSetAnchorNodeTo(kExamineReachPoseType0, 10.0f, camera.chara);
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (camCtrlResetForLooped()) {
                PlayCachQueuedSound(g_globalSoundCommand);
                CaptureSlidingGrip();
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4: {
            if (!SlideExaminationPart())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            ManipulateTransformationMatrix(0.0f, 0.0f, 0.0f);
            const Vector3 sampleTimes{0.0f, 15.0f, 30.0f};
            g_cachCurveAnchorA.position = handPoint(anchorA);
            g_quadraticCurveRemaining = 30.0f;
            FitCachCurveToward(g_cachCurveAnchorA, camera.handTarget[0], 0.60000002f, sampleTimes);
            g_cachCurveAnchorB.position = handPoint(anchorB);
            FitCachCurveToward(g_cachCurveAnchorB, camera.handTarget[1], 0.60000002f, sampleTimes);
            camCtrlSetAnchorNodeTo(kTargetGripPoseMode8, 15.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        }
        case 5:
            if (ApplyRelativeSmoothedCameraPos()) {
                UpdateHandMotData();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    if (camera.state == 5) {
        switch (step) {
        case 0:
            if (camCtrlUpdateAccelTerm()) {
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
                PlaceHandsForSlidingRelease();
                ++g_cameraGlobalState;
            }
            return 0;
        case 1:
            if ((anchorA.control & 8U) != 0) {
                const float frames = g_quadraticCurveRemaining;
                camCtrlSetAnchorNodeTo(kTargetPoseMode8, static_cast<float>(static_cast<double>(frames) * 0.85), nullptr);
                const Vector3 sampleTimes{0.0f, static_cast<float>(static_cast<double>(frames) * 0.5), frames};
                g_cachCurveAnchorA.position = handPoint(anchorA);
                FitCachCurveToward(g_cachCurveAnchorA, camera.hand[0].position, 0.30000001f, sampleTimes);
                g_cachCurveAnchorB.position = handPoint(anchorB);
                FitCachCurveToward(g_cachCurveAnchorB, camera.hand[1].position, 0.30000001f, sampleTimes);
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 2:
            if (ApplyRelativeSmoothedCameraPos()) {
                PlayCachQueuedSound(g_globalSoundCommand);
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                camCtrlSetAnchorNodeTo(kExamineReachPoseType0, 15.0f, camera.chara);
                CaptureSlidingGrip();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (!SlideExaminationPartBack())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            g_quadraticCurveRemaining = kSeqFramesPerSecond;
            camCtrlSetAnchorNodeTo(kTargetGripPoseMode8, 15.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            return 0;
        case 5:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    camera.state = 0;
    return 1;
}




bool SwingExaminationPart() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(g_quadraticCurveRemaining) + 1.0));
    const auto turn = [blend](std::int32_t from, std::int32_t to) {
        return static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(to - from)) * blend + static_cast<float>(from));
    };
    auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform);
    std::int32_t primaryYaw = 0;
    if (primary)
        primaryYaw = turn(primary->rotation.y, camera.partAngleOffset[0]);
    std::int32_t secondaryYaw = 0;
    if (secondary)
        secondaryYaw = turn(secondary->rotation.y, camera.partAngleOffset[1]);
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], blend);
    const Vector3 eye{(camera.eyePosition.x - g_cameraCurvePosition.x) * blend + g_cameraCurvePosition.x,
        g_cameraCurvePosition.y + (camera.eyePosition.y - g_cameraCurvePosition.y) * blend,
        g_cameraCurvePosition.z + (camera.eyePosition.z - g_cameraCurvePosition.z) * blend};
    if (primary)
        primary->rotation.y = primaryYaw;
    if (secondary)
        secondary->rotation.y = secondaryYaw;
    UpdateCameraPosAndRot();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}



bool SwingExaminationPartBack() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(g_quadraticCurveRemaining) + 1.0));
    const auto turn = [blend](std::int32_t from, std::int32_t to) {
        return static_cast<std::int32_t>(static_cast<float>(static_cast<std::int16_t>(to - from)) * blend + static_cast<float>(from));
    };
    std::int32_t savedPrimary = 0;
    std::int32_t savedSecondary = 0;
    std::memcpy(&savedPrimary, &camera.target.y, sizeof(savedPrimary));
    std::memcpy(&savedSecondary, &camera.target.z, sizeof(savedSecondary));
    auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform);
    std::int32_t primaryYaw = 0;
    if (primary)
        primaryYaw = turn(primary->rotation.y, savedPrimary);
    std::int32_t secondaryYaw = 0;
    if (secondary)
        secondaryYaw = turn(secondary->rotation.y, savedSecondary);
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], blend);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], blend);
    const Vector3 eye{(camera.cameraAux.x - g_cameraCurvePosition.x) * blend + g_cameraCurvePosition.x,
        g_cameraCurvePosition.y + (camera.cameraAux.y - g_cameraCurvePosition.y) * blend,
        g_cameraCurvePosition.z + (camera.cameraAux.z - g_cameraCurvePosition.z) * blend};
    if (primary)
        primary->rotation.y = primaryYaw;
    if (secondary)
        secondary->rotation.y = secondaryYaw;
    UpdateCameraPosAndRot();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}




void PlaceHingedExaminationHands() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (camera.primaryTransform && armA)
        ShowCachCharacter(armA);
    if (camera.secondaryTransform && armB)
        ShowCachCharacter(armB);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    camera.hand[0].position = {camera.eye.x - cosine * 0.015f - sine * 0.0f, camera.eye.y + 0.0f, camera.eye.z - sine * 0.015f + cosine * 0.0f};
    camera.hand[1].position = {
        cosine * 0.015f + camera.acceleration.x - sine * 0.0f, camera.acceleration.y + 0.0f, sine * 0.015f + camera.acceleration.z + cosine * 0.0f};
    const float stepA = cosine * -0.04f;
    const float stepB = sine * -0.04f;
    camera.hand[0].step.y = 0.0f;
    camera.hand[1].step.y = 0.0f;
    const auto fifth = [](float value) { return static_cast<float>(static_cast<double>(value) * 0.2); };
    camera.hand[0].step.x = fifth(-stepA - stepB);
    const float cross = fifth(stepA - stepB);
    camera.hand[0].step.z = cross;
    camera.hand[1].step.x = cross;
    camera.hand[1].step.z = fifth(stepB + stepA);
    const auto width = static_cast<float>(static_cast<double>(camera.halfExtents.x) + static_cast<double>(camera.halfExtents.x));
    const float lowest = camera.eyePosition.y < camera.previousEye.y ? camera.eyePosition.y : camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(camera.previousEye.y) + 0.35));
    const Vector3 &saved = camera.previousEye;
    camera.handTarget[0] = {cosine * -width + saved.x - sine * 0.2f, drop + saved.y, -width * sine + saved.z + cosine * 0.2f};
    camera.handTarget[1] = {width * cosine + saved.x - sine * 0.2f, drop + saved.y, width * sine + saved.z + cosine * 0.2f};
    if (auto *const primary = static_cast<Mt5RuntimeNode *>(camera.primaryTransform))
        camera.partAngleOffset[0] = primary->rotation.y - 0x4e38;
    if (auto *const secondary = static_cast<Mt5RuntimeNode *>(camera.secondaryTransform))
        camera.partAngleOffset[1] = secondary->rotation.y + 0x4e38;
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float ryoY = ryo ? GetPositionForEntity(ryo->entity.node).y : 0.0f;
    auto height = static_cast<float>(static_cast<double>(camera.targetPosition.y - ryoY) - 1.4);
    height = 0.5f < height ? 0.5f : height;
    height = -0.5f > height ? -0.5f : height;
    const double lean = height;
    const auto toAngle = [](double degrees) {
        return static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::int32_t>((degrees + 90.0) * 8192.0 / 45.0)));
    };
    const std::int32_t yaw = camera.yaw;
    camera.armJointRotation[1].x = 0;
    camera.armJointRotation[0].x = 0;
    camera.armJointRotation[0].y = yaw + 0x2aaa;
    camera.armEndRotation[1].x = 0;
    camera.armJointRotation[1].y = yaw + 0x5555;
    camera.armEndRotation[0].x = 0;
    const std::int32_t jointLean = toAngle(lean * 40.0);
    camera.armJointRotation[1].z = jointLean;
    camera.armJointRotation[0].z = jointLean;
    camera.armEndRotation[0].y = yaw + 0x38e3;
    camera.armRotation[1].x = 0;
    camera.armEndRotation[1].y = yaw + 0x471c;
    camera.armRotation[0].x = 0;
    const std::int32_t endLean = toAngle(lean * 60.0);
    camera.armEndRotation[1].z = endLean;
    camera.armEndRotation[0].z = endLean;
    camera.armRotation[1].y = yaw + 0x4000;
    camera.armRotation[0].y = yaw + 0x4000;
    const std::int32_t armLean = toAngle(lean * 100.0);
    camera.armRotation[1].z = armLean;
    camera.armRotation[0].z = armLean;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-18204));
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
    Matrix4x4::Pop();
    Matrix4x4::RotateAroundY_Wrapper(18204);
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    g_cachArmRotationA = camera.armRotation[0];
    g_cachArmRotationB = camera.armRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, camera.armJointRotation[0]);
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationA = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, camera.armJointRotation[1]);
    camera.handRotation[1] = camera.handBaseRotation[1];
    g_cachHandRotationB = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    Matrix4x4::Pop();
}





void PlaceHandsForHingedRelease() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    if (camera.primaryTransform && armA)
        ShowCachCharacter(armA);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    if (camera.secondaryTransform && armB)
        ShowCachCharacter(armB);
    const auto reach = static_cast<float>(static_cast<double>(camera.halfExtents.x) * 5.0);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const Vector3 &eye = camera.eyePosition;
    camera.handTarget[0] = {-reach * cosine + eye.x - sine * 0.1f, camera.handTarget[0].y - eye.y + eye.y, -reach * sine + eye.z + cosine * 0.1f};
    camera.handTarget[1] = {reach * cosine + eye.x - sine * 0.1f, camera.handTarget[1].y - eye.y + eye.y, reach * sine + eye.z + cosine * 0.1f};
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(0x8000);
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float ryoY = ryo ? GetPositionForEntity(ryo->entity.node).y : 0.0f;
    auto height = static_cast<float>(static_cast<double>(camera.targetPosition.y - ryoY) - 1.4);
    height = 0.5f < height ? 0.5f : height;
    height = -0.5f > height ? -0.5f : height;
    const double lean = height;
    const auto toAngle = [](double degrees) {
        return static_cast<std::int32_t>(static_cast<std::uint16_t>(static_cast<std::int32_t>((degrees + 90.0) * 8192.0 / 45.0)));
    };
    const std::int32_t yaw = camera.yaw;
    camera.armJointRotation[1].x = 0;
    camera.armJointRotation[0].x = 0;
    camera.armJointRotation[0].y = yaw + 0x238e;
    camera.armJointRotation[1].y = yaw + 0x5c71;
    const std::int32_t jointLean = toAngle(lean * 40.0);
    camera.armEndRotation[1].x = 0;
    camera.armEndRotation[0].x = 0;
    camera.armJointRotation[1].z = jointLean;
    camera.armJointRotation[0].z = jointLean;
    camera.armEndRotation[0].y = yaw + 0x31c7;
    camera.armEndRotation[1].y = yaw + 0x4e38;
    const std::int32_t endLean = toAngle(lean * 60.0);
    camera.armRotation[1].x = 0;
    camera.armEndRotation[1].z = endLean;
    camera.armEndRotation[0].z = endLean;
    camera.armRotation[0].x = 0;
    camera.armRotation[0].y = yaw + 0x1c71;
    camera.armRotation[1].y = yaw + 0x638e;
    const std::int32_t armLean = toAngle(lean * 100.0);
    camera.armRotation[1].z = armLean;
    camera.armRotation[0].z = armLean;
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-20024));
    Matrix4x4::RotateAroundZ_Wrapper(25486);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(-5461));
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
    Matrix4x4::Pop();
    Matrix4x4::RotateAroundY_Wrapper(0xc000);
    Matrix4x4_ExtractEulerAngles(camera.handRotation[0]);
    Matrix4x4::Pop();
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::RotateAroundY_Wrapper(20024);
    Matrix4x4::RotateAroundZ_Wrapper(25486);
    Matrix4x4::RotateAroundY_Wrapper(5461);
    Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[1]);
    Matrix4x4::Pop();
    Matrix4x4::RotateAroundY_Wrapper(0x4000);
    Matrix4x4_ExtractEulerAngles(camera.handRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    g_cachArmRotationA = camera.armJointRotation[0];
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    g_cachHandRotationA = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    camera.handBaseRotation[0] = g_cachHandRotationA;
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    g_cachHandRotationB = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    camera.handBaseRotation[1] = g_cachHandRotationB;
    Matrix4x4::Pop();
}




void BeginHingedExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (camera.primaryTransform)
        std::memcpy(&camera.target.y, &static_cast<Mt5RuntimeNode *>(camera.primaryTransform)->rotation.y, sizeof(std::int32_t));
    if (camera.secondaryTransform)
        std::memcpy(&camera.target.z, &static_cast<Mt5RuntimeNode *>(camera.secondaryTransform)->rotation.y, sizeof(std::int32_t));
    camera.targetPosition = camera.modelBoundsCenterWorld;
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    camera.eyePosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    camera.eyePosition.y = static_cast<float>(static_cast<double>(camera.eyePosition.y) + 1.65);
    const double reach = static_cast<double>(camera.halfExtents.y) * 1.5;
    const auto top = static_cast<float>(static_cast<double>(camera.modelBoundsCenterWorld.y) + reach);
    if (camera.eyePosition.y > top)
        camera.eyePosition.y = top;
    const auto bottom = static_cast<float>(static_cast<double>(camera.modelBoundsCenterWorld.y) - reach);
    if (bottom > camera.eyePosition.y)
        camera.eyePosition.y = bottom;
    float radius = static_cast<float>(static_cast<double>(camera.halfExtents.x) + static_cast<double>(camera.halfExtents.x));
    radius = 0.25f > radius ? 0.25f : radius;
    if (static_cast<double>(radius) > 0.65)
        radius = 0.64999998f;
    camera.eyePosition.x = camera.targetPosition.x - radius * camera.yawSine;
    camera.eyePosition.z = radius * camera.yawCosine + camera.targetPosition.z;
    camera.cameraAux = camera.eyePosition;
}


std::int32_t PollHingedExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    auto &anchorB = *static_cast<HandModelWork *>(g_cameraAnchorB);
    const std::int32_t step = g_cameraGlobalState;
    const auto handPoint = [](const HandModelWork &anchor) {
        Vector3 point{};
        std::memcpy(&point, anchor.jointPositions + 60 * sizeof(Vector3), sizeof(point));
        return point;
    };
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            PlaceHingedExaminationHands();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(kExamineReachPose, static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.85), nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (!AdjustCameraViewAndFoV())
                return 0;
            g_quadraticCurveRemaining = 10.0f;
            camCtrlSetAnchorNodeTo(kExamineGraspPose, 10.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 3:
            if (camCtrlResetForLooped()) {
                PlayCachQueuedSound(g_globalSoundCommand);
                CaptureSlidingGrip();
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4:
            if (!SwingExaminationPart())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            ManipulateTransformationMatrix(0.050000001f, 0.029999999f, 0.0099999998f);
            UpdateHandMotData();
            ++g_cameraGlobalState;
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    if (camera.state == 5) {
        switch (step) {
        case 0:
            if (!camCtrlUpdateAccelTerm())
                return 0;
            g_quadraticCurveRemaining = 3.0f;
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            PlaceHandsForHingedRelease();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) != 0) {
                camCtrlSetAnchorNodeTo(kTargetPoseMode8, 0.0f, nullptr);
                const float frames = g_quadraticCurveRemaining;
                const Vector3 sampleTimes{0.0f, static_cast<float>(static_cast<double>(frames) * 0.5), frames};
                g_cachCurveAnchorA.position = handPoint(anchorA);
                FitCachCurveToward(g_cachCurveAnchorA, camera.hand[0].position, 0.30000001f, sampleTimes);
                g_cachCurveAnchorB.position = handPoint(anchorB);
                FitCachCurveToward(g_cachCurveAnchorB, camera.hand[1].position, 0.30000001f, sampleTimes);
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 2:
            if (ApplyRelativeSmoothedCameraPos()) {
                PlayCachQueuedSound(g_globalSoundCommand);
                g_quadraticCurveRemaining = kSeqFramesPerSecond;
                camCtrlSetAnchorNodeTo(kTargetPoseMode8, 15.0f, camera.chara);
                CaptureSlidingGrip();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (!SwingExaminationPartBack())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            g_quadraticCurveRemaining = kSeqFramesPerSecond;
            camCtrlSetAnchorNodeTo(kTargetGripPoseMode8, 15.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            return 0;
        case 5:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    camera.state = 0;
    return 1;
}



void FitPositionExaminationCurves() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 sampleTimes{0.0f, 15.0f, 30.0f};
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    g_quadraticCurveRemaining = 30.0f;
    FitCachCurveToward(g_cachCurveStart, camera.eyePosition, 0.30000001f, sampleTimes);
    FitCachCurveToward(g_cachCurveEnd, camera.targetPosition, 0.30000001f, sampleTimes);
}



void EndPositionExaminationObserve() {
    g_bIsConversationCamInactive = 0;
    g_temporaryGameState = 0;
    ApplyPendingGameState(0);
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 0;
    ApplyPendingGameState(0);
    g_quadraticCurveRemaining = kSeqFramesPerSecond;
    static_cast<CachContextState *>(g_camera)->observeMode = -1;
}



bool AdjustFieldOfView() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const float t = g_quadraticCurveRemaining;
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    const Vector3 target = EvaluateCachCurve(g_cachCurveEnd, t);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    const float opacity = camera.savedPlayerOpacity < ryo->renderOpacity ? camera.savedPlayerOpacity : ryo->renderOpacity;
    camera.attachmentOpacity =
        static_cast<float>((1.0 - static_cast<double>(camera.attachmentOpacity)) * static_cast<double>(blend) + static_cast<double>(camera.attachmentOpacity));
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fovDelta = static_cast<std::int16_t>(camera.examineFov - currentFov);
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = target;
    ryo->renderOpacity = static_cast<float>((0.0 - static_cast<double>(opacity)) * static_cast<double>(blend) + static_cast<double>(opacity));
    CAME_SetFieldOfView(static_cast<std::int32_t>(static_cast<float>(fovDelta) * blend + static_cast<float>(currentFov)));
    return finished;
}



bool camCtrlRecalcPerspectiveFov() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto blend = static_cast<float>(1.0 / (static_cast<double>(g_quadraticCurveRemaining) + 1.0));
    camera.attachmentOpacity =
        static_cast<float>((0.0 - static_cast<double>(camera.attachmentOpacity)) * static_cast<double>(blend) + static_cast<double>(camera.attachmentOpacity));
    const std::int32_t currentFov = currentPerspectiveFov;
    const auto fovDelta = static_cast<std::int16_t>(camera.savedFov - currentFov);
    CAME_SetFieldOfView(static_cast<std::int32_t>(static_cast<float>(fovDelta) * blend + static_cast<float>(currentFov)));
    return finished;
}




void BeginPositionExamination(const Vector3 *position) {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    camera.attachmentRootPosition = position ? *position : Vector3{};
    camera.attachmentRootRotation = {};
    camera.modelBoundsCenterWorld = camera.attachmentRootPosition;
    camera.attachmentOpacity = 0.0f;
    camera.primaryTransform = nullptr;
    camera.halfExtents = {};
    camera.eyePosition = g_CameraEyePos;
    camera.targetPosition = g_CameraTargetPos;
    camera.yaw = atan2_fixed16(camera.eyePosition.x - camera.targetPosition.x, camera.eyePosition.z - camera.targetPosition.z);
    const auto facing = [&camera] {
        const auto turn = static_cast<std::uint16_t>(-camera.yaw);
        camera.yawSine = sin_fixed16(static_cast<std::int16_t>(turn));
        camera.yawCosine = cos_fixed16(turn);
    };
    facing();
    CachAttachmentRecord *attachment = nullptr;
    for (CachAttachmentRecord &candidate : camera.attachments) {
        if (candidate.modelOwner) {
            attachment = &candidate;
            break;
        }
    }
    if (!attachment)
        return;
    matrix_stack::Matrix4x4_Push();
    Vector3 minimum{-0.5f, -0.5f, -0.5f};
    Vector3 maximum{0.5f, 0.5f, 0.5f};
    if ((reinterpret_cast<std::uintptr_t>(attachment->modelOwner) & ~std::uintptr_t{0xf}) != 0) {
        camera.primaryTransform = attachment->modelOwner->model;
        workTransformMatrix = transformMatrix_prePass;
        minimum = {10000.0f, 10000.0f, 10000.0f};
        maximum = {-10000.0f, -10000.0f, -10000.0f};
        ExpandExaminationBounds(static_cast<Mt5RuntimeNode *>(camera.primaryTransform), minimum, maximum);
    } else {
        camera.primaryTransform = reinterpret_cast<void *>(std::uintptr_t{1});
    }
    camera.halfExtents = {static_cast<float>(static_cast<double>(maximum.x - minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y - minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z - minimum.z) * 0.5)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(camera.attachmentRootPosition.x, camera.attachmentRootPosition.y, camera.attachmentRootPosition.z);
    Matrix4x4::preTranslate(attachment->position.x, attachment->position.y, attachment->position.z);
    Matrix4x4::preTranslate(static_cast<float>(static_cast<double>(maximum.x + minimum.x) * 0.5),
        static_cast<float>(static_cast<double>(maximum.y + minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z + minimum.z) * 0.5));
    camera.modelBoundsCenterWorld = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    camera.yaw = attachment->rotation.y;
    facing();
    const Vector3 &center = camera.modelBoundsCenterWorld;
    camera.targetPosition = center;
    camera.eyePosition = {center.x - camera.yawSine * 0.60000002f, center.y, camera.yawCosine * 0.60000002f + center.z};
}



std::int32_t PollPositionExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const std::int32_t step = g_cameraGlobalState;
    if (camera.state == 4) {
        if (step == 0) {
            if (camera.primaryTransform) {
                shenStartObserveMode();
                FitPositionExaminationCurves();
                ++g_cameraGlobalState;
                return 0;
            }
            camera.previousEye = g_CameraEyePos;
            camera.previousTarget = g_CameraTargetPos;
            g_cameraCurvePosition = camera.previousEye;
            g_temporaryAcceleratedVector = camera.previousTarget;
            g_cameraGlobalState = step + 2;
            return 0;
        }
        if (step == 1) {
            if (!AdjustFieldOfView())
                return 0;
            g_cameraGlobalState = step + 1;
            return 0;
        }
        g_cameraGlobalState = -1;
        return 1;
    }
    if (camera.state != 5) {
        camera.state = 0;
        return 1;
    }
    if (step == 0) {
        if (!camera.primaryTransform) {
            g_cameraGlobalState = step + 2;
            return 0;
        }
        if (camCtrlUpdateAccelTerm()) {
            g_cameraGlobalState = step + 1;
            EndPositionExaminationObserve();
        }
        return 0;
    }
    if (step != 1) {
        camera.state = 0;
        g_cameraGlobalState = -1;
        return 1;
    }
    if (!camCtrlRecalcPerspectiveFov())
        return 0;
    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t token = static_cast<std::int8_t>(camera.previousHelpTokenId) >= 68 ? 0 : camera.previousHelpTokenId;
    g_helpButtonId = static_cast<std::uint8_t>(camera.previousButtonId);
    g_defaultHelpTokenId = token;
    g_currentHelpTokenId = token;
    g_currentHelpIconState = 0;
    SRCH_ExecuteCommand(SearchCommand::Reset);
    ++g_cameraGlobalState;
    return 0;
}


void UpdateWorldTransformRecursive(Mt5RuntimeNode *node, Matrix4x4 &primary) {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    while (true) {
        std::byte *const base = MT5_ModelBaseForNode(node);
        if (node->siblingOffset)
            matrix_stack::Matrix4x4_Push();
        Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
        Matrix4x4::SetViewMatrixFromEuler(node->rotation.y, node->rotation.x, node->rotation.z);
        Matrix4x4::Scale(node->scale.x, node->scale.y, node->scale.z);
        if (node == camera.primaryTransform)
            primary = workTransformMatrix;
        if (node->childOffset)
            UpdateWorldTransformRecursive(reinterpret_cast<Mt5RuntimeNode *>(base + node->childOffset), primary);
        if (!node->siblingOffset)
            return;
        Matrix4x4::Pop();
        node = reinterpret_cast<Mt5RuntimeNode *>(base + node->siblingOffset);
    }
}



void CalculateMatrixTransformAndApply() {
    matrix_stack::Matrix4x4_Push();
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 primary = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    UpdateWorldTransformRecursive(static_cast<Mt5RuntimeNode *>(camera.chara->model), primary);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    const auto placeArm = [](Character *arm) {
        const Vector3 armPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        Vector3i armRotation{};
        Matrix4x4_ExtractEulerAngles(armRotation);
        if (arm) {
            shenApplyTranslationToAll(arm->entity.node, armPosition);
            ApplyEulerAnglesToNode(arm->entity.node, armRotation);
        }
    };
    workTransformMatrix = primary;
    Matrix4x4::Multiply(&camera.primaryAuthoredTransform);
    placeArm(armA);
    workTransformMatrix = primary;
    Matrix4x4::Multiply(&camera.secondaryAuthoredTransform);
    placeArm(armB);
    const auto keepHand = [](Character *arm, HandModelWork &anchor, Vector3i &handRotation, const Vector3i &armRotation) {
        HND_ApplyRootEuler(anchor, handRotation);
        ApplyHandRootTransform(arm, anchor);
        if (arm)
            ApplyEulerAnglesToNode(arm->entity.node, armRotation);
        const Matrix4x4 handFrame = workTransformMatrix;
        HND_ApplyRootEuler(anchor, {});
        ApplyHandRootTransform(arm, anchor);
        Matrix4x4::InvertInPlace(nullptr);
        Matrix4x4::Multiply(&handFrame);
        Matrix4x4_ExtractEulerAngles(handRotation);
        HND_ApplyRootEuler(anchor, handRotation);
    };
    g_cachHandRotationA = camera.handBaseRotation[0];
    keepHand(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, g_cachArmRotationA);
    g_cachHandRotationB = camera.handBaseRotation[1];
    keepHand(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, g_cachArmRotationB);
    Matrix4x4::Pop();
}






void UpdateSceneFromCameraMatrixAndRotations() {
    matrix_stack::Matrix4x4_Push();
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    const auto flags = static_cast<std::uint16_t>(g_cameraStateFlags);
    if ((flags & 0x100U) == 0 || (flags & 0x2000U) != 0)
        ShowCachCharacter(armA);
    if ((flags & 0x100U) == 0 || (flags & 0x2000U) == 0)
        ShowCachCharacter(armB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const std::int32_t side = camera.yaw + 0x4000;
    camera.armEndRotation[0] = {-3640, side, 0x4000};
    camera.armEndRotation[1] = {3640, side, 0x4000};
    camera.armJointRotation[0] = {-3640, side, 14564};
    camera.armJointRotation[1] = {3640, side, 14564};
    camera.armRotation[0] = {-3640, side, 6372};
    camera.armRotation[1] = {3640, side, 6372};
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
    Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const float lowest = camera.eyePosition.y < camera.previousEye.y ? camera.eyePosition.y : camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(camera.previousEye.y) + 0.35));
    const Vector3 &saved = camera.previousEye;
    camera.handTarget[0] = {saved.x - cosine * 0.2f - sine * 0.2f, drop + saved.y, saved.z - sine * 0.2f + cosine * 0.2f};
    camera.handTarget[1] = {cosine * 0.2f + saved.x - sine * 0.2f, drop + saved.y, sine * 0.2f + saved.z + cosine * 0.2f};
    const bool defaultPoses = (flags & 0x100U) != 0;
    if (defaultPoses) {
        const Vector3 &grip = camera.eye;
        camera.hand[0].position = {cosine * -0.0f + grip.x - sine * -0.02f, grip.y + 0.015f, sine * -0.0f + grip.z - cosine * 0.02f};
        camera.hand[1].position = {cosine * 0.0f + grip.x - sine * -0.02f, grip.y + 0.015f, sine * 0.0f + grip.z - cosine * 0.02f};
    } else {
        const auto width = static_cast<float>(static_cast<double>(camera.halfExtents.x) + 0.02);
        float height = camera.halfExtents.y;
        if (0.03 > static_cast<double>(height))
            height = 0.029999999f;
        const Vector3 &centre = camera.targetPosition;
        camera.hand[0].position = {-width * cosine + centre.x - sine * -0.035f, height + centre.y, -width * sine + centre.z - cosine * 0.035f};
        camera.hand[1].position = {width * cosine + centre.x - sine * -0.035f, height + centre.y, width * sine + centre.z - cosine * 0.035f};
    }
    const float depth = camera.halfExtents.z;
    const auto reach = static_cast<float>(static_cast<double>(camera.halfExtents.x) * 3.0);
    float out = 1.0f > reach ? 1.0f : reach;
    const bool leftArm = (flags & 0x2000U) != 0;
    if (leftArm)
        out = -out;
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(camera.appliedPosition.x, camera.appliedPosition.y, camera.appliedPosition.z);
    std::uint16_t facing = 0;
    if (camera.chara) {
        for (const CharacterNode *node = &camera.chara->entity.node; node; node = node->child)
            facing = static_cast<std::uint16_t>(facing + node->rotation.y);
    }
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw + static_cast<std::int16_t>(facing)));
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(out, 0.1f, depth);
    camera.acceleration3 = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    Matrix4x4::Pop();
    Matrix4x4::preTranslate(static_cast<float>(static_cast<double>(out) * 0.1), 0.060000002f, static_cast<float>(static_cast<double>(depth) * 0.1));
    camera.smoothedInput = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
    if (defaultPoses) {
        workTransformMatrix = transformMatrix_prePass;
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(camera.yaw));
        if (leftArm) {
            Matrix4x4::RotateAroundY_Wrapper(0x8000);
            camera.handBaseRotation[0].y = -9102;
            camera.handBaseRotation[0].x = 30037;
        } else {
            camera.handBaseRotation[0].y = 9102;
            camera.handBaseRotation[0].x = -30037;
        }
        camera.handBaseRotation[0].z = -19114;
        Matrix4x4::SetViewMatrixFromEuler(camera.handBaseRotation[0].y, camera.handBaseRotation[0].x, -19114);
        Matrix4x4_ExtractEulerAngles(camera.handBaseRotation[0]);
    } else {
        camera.handBaseRotation[0] = {0, static_cast<std::int16_t>(camera.yaw + 0x4000), 0};
    }
    camera.handBaseRotation[1] = camera.handBaseRotation[0];
    g_cachArmRotationA = camera.armJointRotation[0];
    if (armA) {
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    }
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    ResolveCompositeTransformEuler(armA, *static_cast<HandModelWork *>(g_cameraAnchorA), g_cachHandRotationA, camera.handRotation[0]);
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armB) {
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    }
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
    ResolveCompositeTransformEuler(armB, *static_cast<HandModelWork *>(g_cameraAnchorB), g_cachHandRotationB, camera.handRotation[1]);
    Matrix4x4::Pop();
}



void PerformMatrixTransformations() {
    matrix_stack::Matrix4x4_Push();
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4 primary = transformMatrix_prePass;
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    UpdateWorldTransformRecursive(static_cast<Mt5RuntimeNode *>(camera.chara->model), primary);
    Matrix4x4::InvertInPlace(&primary);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    const Vector3 armAPosition = armA ? GetPositionForEntity(armA->entity.node) : Vector3{};
    workTransformMatrix = primary;
    Matrix4x4::preTranslate(armAPosition.x, armAPosition.y, armAPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(g_cachArmRotationA.y, g_cachArmRotationA.x, g_cachArmRotationA.z);
    camera.primaryAuthoredTransform = workTransformMatrix;
    camera.handBaseRotation[0] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA));
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    const Vector3 armBPosition = armB ? GetPositionForEntity(armB->entity.node) : Vector3{};
    workTransformMatrix = primary;
    Matrix4x4::preTranslate(armBPosition.x, armBPosition.y, armBPosition.z);
    Matrix4x4::SetViewMatrixFromEuler(g_cachArmRotationB.y, g_cachArmRotationB.x, g_cachArmRotationB.z);
    camera.secondaryAuthoredTransform = workTransformMatrix;
    camera.handBaseRotation[1] = HND_GetRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB));
    Matrix4x4::Pop();
}




void calculate3DPointQuadraticCoefficients() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_quadraticCurveRemaining = 40.0f;
    const auto *const part = static_cast<const Mt5RuntimeNode *>(camera.primaryTransform);
    g_cachCurveAnchorA.acceleration = camera.acceleration3;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_cachCurveAnchorA.position = part->position;
    FitCachQuadraticCurve({0.0f, 32.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.acceleration = g_cameraCurvePosition;
    const auto depth = static_cast<double>(camera.halfExtents.z);
    g_cachCurveStart.velocity = {static_cast<float>(static_cast<double>(camera.targetPosition.x) - depth * 1.2 * static_cast<double>(camera.yawSine)),
        static_cast<float>(static_cast<double>(camera.halfExtents.y) * 1.5 + static_cast<double>(camera.eyePosition.y)),
        static_cast<float>(depth * 1.2 * static_cast<double>(camera.yawCosine) + static_cast<double>(camera.targetPosition.z))};
    g_cachCurveStart.position = g_cameraCurvePosition;
    FitCachQuadraticCurve({-40.0f, 0.0f, 40.0f}, g_cachCurveStart);
}



void CopyAndFitCameraCurve() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_quadraticCurveRemaining = 40.0f;
    const auto *const part = static_cast<const Mt5RuntimeNode *>(camera.primaryTransform);
    g_cachCurveAnchorA.acceleration = camera.appliedPosition;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_cachCurveAnchorA.position = part->position;
    FitCachQuadraticCurve({0.0f, 8.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.acceleration = g_cameraCurvePosition;
    g_cachCurveStart.velocity = camera.cameraAux;
    g_cachCurveStart.position = g_cameraCurvePosition;
    FitCachQuadraticCurve({-40.0f, 0.0f, 40.0f}, g_cachCurveStart);
}



bool AdjustCurveByScalarForTransform() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 part = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto share = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], share);
    static_cast<Mt5RuntimeNode *>(camera.primaryTransform)->position = part;
    CalculateMatrixTransformAndApply();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}



void FinishLiftedExaminationEnter() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto keep = [&camera](std::size_t side, Character *arm, const Vector3i &armRotation, const HandModelWork &anchor) {
        camera.armEndRotation[side] = camera.armJointRotation[side];
        camera.armJointRotation[side] = armRotation;
        camera.hand[side].position = arm ? GetPositionForEntity(arm->entity.node) : Vector3{};
        const Vector3i root = HND_GetRootEuler(anchor);
        camera.handBaseRotation[side] = {static_cast<std::int16_t>(root.x), static_cast<std::int16_t>(root.y), static_cast<std::int16_t>(root.z)};
    };
    keep(0, static_cast<Character *>(g_cameraTargetEntity), g_cachArmRotationA, *static_cast<HandModelWork *>(g_cameraAnchorA));
    keep(1, static_cast<Character *>(g_shenmueCamera0), g_cachArmRotationB, *static_cast<HandModelWork *>(g_cameraAnchorB));
}




void PlaceHandsForLiftedRelease() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    const auto flags = static_cast<std::uint16_t>(g_cameraStateFlags);
    if ((flags & 0x100U) == 0 || (flags & 0x2000U) != 0)
        ShowCachCharacter(armA);
    if ((flags & 0x100U) == 0 || (flags & 0x2000U) == 0)
        ShowCachCharacter(armB);
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.hand[0].position);
    g_cachArmRotationA = camera.armJointRotation[0];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), camera.handBaseRotation[0]);
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.hand[1].position);
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), camera.handBaseRotation[1]);
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
}




void BeginLiftedExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const Vector3 position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    const Vector3i euler = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const Vector3 scale = camera.chara ? camera.chara->scale : Vector3{};
    const Vector3i rotation{static_cast<std::int16_t>(euler.x), static_cast<std::int16_t>(euler.y), static_cast<std::int16_t>(euler.z)};
    const bool defaultPoses = (g_cameraStateFlags & 0x100U) != 0;
    if (defaultPoses) {
        camera.modelBoundsCenterWorld.y = camera.eye.y;
    } else {
        auto *const part = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
        if (part->childOffset) {
            matrix_stack::Matrix4x4_Push();
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::Scale(scale.x, scale.y, scale.z);
            Matrix4x4::preTranslate(part->position.x, part->position.y, part->position.z);
            Matrix4x4::SetViewMatrixFromEuler(part->rotation.y, part->rotation.x, part->rotation.z);
            Matrix4x4::Scale(part->scale.x, part->scale.y, part->scale.z);
            auto *const child = reinterpret_cast<Mt5RuntimeNode *>(MT5_ModelBaseForNode(part) + part->childOffset);
            camera.primaryTransform = child;
            Vector3 minimum{10000.0f, 10000.0f, 10000.0f};
            Vector3 maximum{-10000.0f, -10000.0f, -10000.0f};
            ExpandExaminationBounds(child, minimum, maximum);
            camera.halfExtents = {static_cast<float>(static_cast<double>(maximum.x - minimum.x) * 0.5),
                static_cast<float>(static_cast<double>(maximum.y - minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z - minimum.z) * 0.5)};
            workTransformMatrix = transformMatrix_prePass;
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
            Matrix4x4::preTranslate(static_cast<float>(static_cast<double>(maximum.x + minimum.x) * 0.5),
                static_cast<float>(static_cast<double>(maximum.y + minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z + minimum.z) * 0.5));
            camera.modelBoundsCenterWorld = {workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
            Matrix4x4::Pop();
        }
        camera.modelBoundsCenterWorld.y -= camera.halfExtents.y;
    }
    camera.appliedPosition = static_cast<Mt5RuntimeNode *>(camera.primaryTransform)->position;
    camera.targetPosition = camera.modelBoundsCenterWorld;
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    camera.eyePosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    float eyeY = static_cast<float>(static_cast<double>(camera.eyePosition.y) + 1.65);
    const float half = camera.halfExtents.y;
    const float centreY = camera.modelBoundsCenterWorld.y;
    if (eyeY > centreY + half)
        eyeY = centreY + half;
    if (centreY - half > eyeY)
        eyeY = centreY - half;
    camera.eyePosition = g_CameraEyePos;
    camera.eyePosition.y = eyeY;
    float dx = camera.eyePosition.x - camera.targetPosition.x;
    float dz = camera.eyePosition.z - camera.targetPosition.z;
    float distanceSquared = dz * dz + dx * dx;
    if (0.0025 > static_cast<double>(distanceSquared)) {
        dx = 0.050000001f;
        dz = 0.050000001f;
        distanceSquared = 0.0024999999f;
        camera.eyePosition.x = camera.targetPosition.x + 0.050000001f;
        camera.eyePosition.z = camera.targetPosition.z + 0.050000001f;
    }
    camera.yaw = atan2_fixed16(camera.eyePosition.x - camera.targetPosition.x, camera.eyePosition.z - camera.targetPosition.z);
    const auto toRadius = static_cast<float>(0.4 / static_cast<double>(std::sqrt(distanceSquared)));
    camera.eyePosition.z = toRadius * dz + camera.targetPosition.z;
    camera.eyePosition.x = toRadius * dx + camera.targetPosition.x;
    camera.eyePosition.y = static_cast<float>(static_cast<double>(camera.targetPosition.y) + 0.5);
    if (!defaultPoses) {
        // The view's yaw snaps to the object's yaw plus the nearest whole quarter turn.
        std::uint32_t delta = static_cast<std::uint16_t>(rotation.y - camera.yaw);
        if (delta >= 0x2000) {
            if (delta < 0x6000)
                delta -= 0x4000;
            else if (delta < 0xA000)
                delta -= 0x8000;
            else if (delta < 0xE000)
                delta -= 0xC000;
        }
        camera.yaw = static_cast<std::uint16_t>(camera.yaw + delta);
        const auto turn = static_cast<std::uint16_t>(-camera.yaw);
        const float sine = sin_fixed16(static_cast<std::int16_t>(turn));
        camera.eyePosition.x = static_cast<float>(static_cast<double>(camera.targetPosition.x) - static_cast<double>(sine) * 0.4);
        const float cosine = cos_fixed16(turn);
        camera.eyePosition.z = static_cast<float>(static_cast<double>(cosine) * 0.4 + static_cast<double>(camera.targetPosition.z));
    }
    camera.cameraAux = camera.eyePosition;
    const auto turn = static_cast<std::uint16_t>(-camera.yaw);
    camera.yawSine = sin_fixed16(static_cast<std::int16_t>(turn));
    camera.yawCosine = cos_fixed16(turn);
    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::RotateAroundY_Wrapper(turn);
    Matrix4x4::SetViewMatrixFromEuler(rotation.y, rotation.x, rotation.z);
    Matrix4x4::Scale(scale.x, scale.y, scale.z);
    auto *const part = static_cast<Mt5RuntimeNode *>(camera.primaryTransform);
    if (part->parentOffset) {
        const auto *const parent = reinterpret_cast<const Mt5RuntimeNode *>(MT5_ModelBaseForNode(part) + part->parentOffset);
        Matrix4x4::preTranslate(parent->position.x, parent->position.y, parent->position.z);
        Matrix4x4::SetViewMatrixFromEuler(parent->rotation.y, parent->rotation.x, parent->rotation.z);
        Matrix4x4::Scale(parent->scale.x, parent->scale.y, parent->scale.z);
    }
    Vector3 minimum{10000.0f, 10000.0f, 10000.0f};
    Vector3 maximum{-10000.0f, -10000.0f, -10000.0f};
    ExpandExaminationBounds(part, minimum, maximum);
    if (maximum.x > minimum.x) {
        camera.halfExtents = {static_cast<float>(static_cast<double>(maximum.x - minimum.x) * 0.5),
            static_cast<float>(static_cast<double>(maximum.y - minimum.y) * 0.5), static_cast<float>(static_cast<double>(maximum.z - minimum.z) * 0.5)};
    }
    Matrix4x4::Pop();
}



std::int32_t PollLiftedExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    const std::int32_t step = g_cameraGlobalState;
    const bool defaultPoses = (g_cameraStateFlags & 0x100U) != 0;
    const std::int8_t *const reachPose = defaultPoses ? kTargetPoseDefault : kTargetPoseMode8;
    const std::int8_t *const gripPose = defaultPoses ? kTargetGripPoseDefault : kTargetGripPoseMode8;
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            UpdateSceneFromCameraMatrixAndRotations();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(reachPose, static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.7), nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (!AdjustCameraViewAndFoV())
                return 0;
            camCtrlSetAnchorNodeTo(gripPose, 20.0f, camera.chara);
            g_cameraGlobalState = step + 1;
            return 0;
        case 3:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                PerformMatrixTransformations();
                calculate3DPointQuadraticCoefficients();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4:
            if (!AdjustCurveByScalarForTransform())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            FinishLiftedExaminationEnter();
            UpdateHandMotData();
            ++g_cameraGlobalState;
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    if (camera.state == 5) {
        switch (step) {
        case 0:
            if (!camCtrlUpdateAccelTerm())
                return 0;
            camCtrlSetAnchorNodeTo(reachPose, 0.0f, nullptr);
            PlaceHandsForLiftedRelease();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            camCtrlSetAnchorNodeTo(gripPose, 20.0f, camera.chara);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                PerformMatrixTransformations();
                CopyAndFitCameraCurve();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (!AdjustCurveByScalarForTransform())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            camCtrlSetAnchorNodeTo(reachPose, 10.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            return 0;
        case 5:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    camera.state = 0;
    return 1;
}





void PlaceTakenExaminationHands() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    const bool leftArm = (g_cameraStateFlags & 0x2000U) != 0;
    ShowCachCharacter(leftArm ? armA : armB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto rise = static_cast<float>(static_cast<double>(camera.modelBoundsCenterWorld.y + camera.halfExtents.y - camera.eyePosition.y) + 0.1);
    float clamped = 0.5f < rise ? 0.5f : rise;
    clamped = -0.5f > clamped ? -0.5f : clamped;
    const auto pitch = static_cast<std::uint16_t>(static_cast<std::int32_t>((static_cast<double>(clamped) * 100.0 + 90.0) * 8192.0 / 45.0));
    camera.armEndRotation[0] = {static_cast<std::int16_t>(pitch), static_cast<std::int16_t>(camera.yaw - 0x238E), 0};
    camera.armEndRotation[1] = {static_cast<std::int16_t>(-static_cast<std::int32_t>(pitch)), static_cast<std::int16_t>(camera.yaw + 0xA38E), 0};
    camera.armJointRotation[0] = camera.armEndRotation[0];
    g_cachArmRotationA = camera.armEndRotation[0];
    camera.armJointRotation[1] = camera.armEndRotation[1];
    g_cachArmRotationB = camera.armEndRotation[1];
    camera.armRotation[0] = {camera.armJointRotation[0].x - 0x2000, camera.armJointRotation[0].y, camera.armJointRotation[0].z};
    camera.armRotation[1] = {camera.armJointRotation[1].x + 0x2000, camera.armJointRotation[1].y, camera.armJointRotation[1].z};
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const float lowest = camera.eyePosition.y < camera.previousEye.y ? camera.eyePosition.y : camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(camera.previousEye.y) + 0.35));
    const Vector3 &saved = camera.previousEye;
    camera.handTarget[0] = {saved.x - cosine * 0.2f - sine * 0.2f, drop + saved.y, saved.z - sine * 0.2f + cosine * 0.2f};
    camera.handTarget[1] = {cosine * 0.2f + saved.x - sine * 0.2f, drop + saved.y, sine * 0.2f + saved.z + cosine * 0.2f};
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    const Vector3 &centre = camera.modelBoundsCenterWorld;
    const float top = camera.halfExtents.y + centre.y;
    camera.hand[0].position = {centre.x - cosine * 0.0099999998f - sine * -0.0099999998f, top, centre.z - sine * 0.0099999998f - cosine * 0.0099999998f};
    camera.hand[1].position = {cosine * 0.0099999998f + centre.x - sine * -0.0099999998f, top, sine * 0.0099999998f + centre.z - cosine * 0.0099999998f};
    const auto reach = static_cast<float>(static_cast<double>(camera.halfExtents.x) * 3.0);
    float out = 1.0f > reach ? 1.0f : reach;
    if (leftArm)
        out = -out;
    const Vector3 &stood = camera.appliedPosition;
    camera.acceleration3 = {out * cosine + stood.x - sine * 0.15000001f, stood.y - 0.2f, out * sine + stood.z + cosine * 0.15000001f};
    const auto near = static_cast<float>(static_cast<double>(out) * 0.07);
    camera.smoothedInput = {near * cosine + stood.x - sine * 0.105f, stood.y + 0.029999999f, near * sine + stood.z + cosine * 0.105f};
    camera.handBaseRotation[0] = {0, 0x1555, 0};
    camera.handBaseRotation[1] = {0, -0x1555, 0};
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), camera.handBaseRotation[0]);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), camera.handBaseRotation[1]);
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
}




void FitCameraCurvesToLookaheadPoint() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_cachCurveAnchorA.acceleration = camera.acceleration3;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_quadraticCurveRemaining = 40.0f;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachQuadraticCurve({0.0f, 28.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    const float centreY = camera.modelBoundsCenterWorld.y;
    FitCachCurveToward(g_cachCurveStart, {camera.eyePosition.x, centreY, camera.eyePosition.z}, 0.30000001f, {0.0f, 20.0f, 40.0f});
    FitCachCurveToward(g_cachCurveEnd, {camera.targetPosition.x, centreY, camera.targetPosition.z}, 0.30000001f, {0.0f, 20.0f, 40.0f});
}



void ResetCurveRefit() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_cachCurveAnchorA.acceleration = camera.appliedPosition;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_quadraticCurveRemaining = 40.0f;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachQuadraticCurve({0.0f, 12.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.position = g_cameraCurvePosition;
    g_cachCurveEnd.position = g_temporaryAcceleratedVector;
    FitCachCurveToward(g_cachCurveStart, camera.cameraAux, 0.30000001f, {0.0f, 20.0f, 40.0f});
    FitCachCurveToward(g_cachCurveEnd, camera.targetPosition, 0.30000001f, {0.0f, 20.0f, 40.0f});
}




void camCtrlApplyBlended() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    ShowCachCharacter((g_cameraStateFlags & 0x2000U) != 0 ? armA : armB);
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.hand[0].position);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.hand[1].position);
    g_cachArmRotationA = camera.armJointRotation[0];
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), camera.handBaseRotation[0]);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), camera.handBaseRotation[1]);
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
}




void BeginTakenExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    camera.appliedPosition = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    camera.targetPosition = camera.modelBoundsCenterWorld;
    camera.targetPosition.y += camera.halfExtents.y;
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    camera.eyePosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    const float half = camera.halfExtents.y;
    const float top = camera.modelBoundsCenterWorld.y + half;
    camera.eyePosition.y = static_cast<float>(static_cast<double>(camera.eyePosition.y) + 1.65);
    if (camera.eyePosition.y > top)
        camera.eyePosition.y = top;
    const float bottom = camera.modelBoundsCenterWorld.y - half;
    if (bottom > camera.eyePosition.y)
        camera.eyePosition.y = bottom;
    camera.eyePosition.x = camera.modelBoundsCenterWorld.x - camera.yawSine * 0.64999998f;
    camera.eyePosition.z = camera.yawCosine * 0.64999998f + camera.modelBoundsCenterWorld.z;
    camera.cameraAux = camera.eyePosition;
    if (top > camera.cameraAux.y)
        camera.cameraAux.y = static_cast<float>(static_cast<double>(camera.cameraAux.y + camera.modelBoundsCenterWorld.y + half) * 0.5);
}



std::int32_t PollTakenExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    const std::int32_t step = g_cameraGlobalState;
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            PlaceTakenExaminationHands();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(kExamineReachPoseType40, static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.7), nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (!AdjustCameraViewAndFoV())
                return 0;
            camCtrlSetAnchorNodeTo(kExamineGraspPoseType40, 20.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 3:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                SyncCurrentPositionAndRotationFromCamera();
                FitCameraCurvesToLookaheadPoint();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4:
            if (!Camera_UpdateRailProgress())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            FinishLiftedExaminationEnter();
            UpdateEntityPositionRotation(nullptr);
            UpdateHandMotData();
            HideCachCharacter(camera.chara);
            ++g_cameraGlobalState;
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    if (camera.state == 5) {
        switch (step) {
        case 0:
            if (!camCtrlUpdateAccelTerm())
                return 0;
            ShowCachCharacter(camera.chara);
            camCtrlSetAnchorNodeTo(kExamineReachPoseType40, 0.0f, nullptr);
            camCtrlApplyBlended();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            camCtrlSetAnchorNodeTo(kExamineGraspPoseType40, 20.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                SyncCurrentPositionAndRotationFromCamera();
                ResetCurveRefit();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (!Camera_UpdateRailProgress())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            UpdateEntityPositionRotation(nullptr);
            camCtrlSetAnchorNodeTo(kExamineReachPoseType40, 10.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            return 0;
        case 5:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    camera.state = 0;
    return 1;
}






void PlaceFramedExaminationHands() {
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    armA->entity.node.child = nullptr;
    armB->entity.node.child = nullptr;
    ShowCachCharacter(armA);
    ShowCachCharacter(armB);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const bool wide = (g_cameraStateFlags & 0x200U) != 0;
    if (wide) {
        const std::int32_t yaw = camera.yaw;
        camera.armEndRotation[0] = {-0x4000, yaw + 0x8000, 0};
        camera.armEndRotation[1] = {0x4000, yaw, 0};
        camera.armJointRotation[0] = {-0x5555, yaw + 0x8000, 0};
        camera.armJointRotation[1] = {0x5555, yaw, 0};
        camera.armRotation[0] = {-0x3555, yaw + 0x8000, 0};
        camera.armRotation[1] = {0x3555, yaw, 0};
    } else {
        const std::int32_t side = camera.yaw + 0x4000;
        camera.armEndRotation[0] = {0, side, 0x4000};
        camera.armEndRotation[1] = {0, side, 0x4000};
        camera.armJointRotation[0] = {0, side, 0x5555};
        camera.armJointRotation[1] = {0, side, 0x5555};
        camera.armRotation[0] = {0, side, 0x3555};
        camera.armRotation[1] = {0, side, 0x3555};
        Matrix4x4_ComposeFromRotation(camera.armJointRotation[0]);
        Matrix4x4_ComposeFromRotation(camera.armJointRotation[1]);
        Matrix4x4_ComposeFromRotation(camera.armEndRotation[0]);
        Matrix4x4_ComposeFromRotation(camera.armEndRotation[1]);
        Matrix4x4_ComposeFromRotation(camera.armRotation[0]);
        Matrix4x4_ComposeFromRotation(camera.armRotation[1]);
    }
    Vector3_RotateVectorWithinLimits(camera.armRotation[0], camera.armJointRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armRotation[1], camera.armJointRotation[1]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[0], camera.armEndRotation[0]);
    Vector3_RotateVectorWithinLimits(camera.armJointRotation[1], camera.armEndRotation[1]);
    g_cachArmRotationA = camera.armJointRotation[0];
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    const float sine = camera.yawSine;
    const float cosine = camera.yawCosine;
    const float lowest = camera.eyePosition.y < camera.previousEye.y ? camera.eyePosition.y : camera.previousEye.y;
    const auto drop = static_cast<float>(static_cast<double>(lowest) - (static_cast<double>(camera.previousEye.y) + 0.35));
    const Vector3 &saved = camera.previousEye;
    camera.handTarget[0] = {saved.x - cosine * 0.2f - sine * 0.2f, drop + saved.y, saved.z - sine * 0.2f + cosine * 0.2f};
    camera.handTarget[1] = {cosine * 0.2f + saved.x - sine * 0.2f, drop + saved.y, sine * 0.2f + saved.z + cosine * 0.2f};
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.handTarget[0]);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.handTarget[1]);
    float height = 0.0f;
    float depth = -0.050000001f;
    float width;
    if (wide) {
        height = static_cast<float>(static_cast<double>(-camera.halfExtents.y) - 0.04);
        depth = static_cast<float>(static_cast<double>(-camera.halfExtents.z) - 0.07);
        width = static_cast<float>(static_cast<double>(camera.halfExtents.x) * 0.5);
    } else {
        width = static_cast<float>(static_cast<double>(camera.halfExtents.x) + 0.04);
    }
    const Vector3 &centre = camera.modelBoundsCenterWorld;
    camera.hand[0].position = {-width * cosine + centre.x - depth * sine, height + centre.y, -width * sine + centre.z + depth * cosine};
    camera.hand[1].position = {width * cosine + centre.x - depth * sine, height + centre.y, width * sine + centre.z + depth * cosine};
    const auto deep = static_cast<float>(static_cast<double>(-camera.halfExtents.y) * 3.0);
    const float lowered = -1.0f < deep ? -1.0f : deep;
    const Vector3 &stood = camera.appliedPosition;
    camera.acceleration3 = {cosine * -0.0f + stood.x - sine * 0.1f, lowered + stood.y, sine * -0.0f + stood.z + cosine * 0.1f};
    camera.smoothedInput = {cosine * 0.0f + stood.x - sine * 0.1f, stood.y + 0.1f, sine * 0.0f + stood.z + cosine * 0.1f};
    if (wide) {
        camera.handBaseRotation[0] = {0, 0x1555, 0};
        camera.handBaseRotation[1] = {0, -0x1555, 0};
    } else {
        camera.handBaseRotation[0] = {0, 0, -0x1555};
        camera.handBaseRotation[1] = {0, 0, -0x1555};
    }
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), camera.handBaseRotation[0]);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), camera.handBaseRotation[1]);
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
}




void FitFramedTakeCurves() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_quadraticCurveRemaining = 40.0f;
    g_cachCurveAnchorA.acceleration = camera.acceleration3;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachQuadraticCurve({0.0f, 28.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.position = g_cameraCurvePosition;
    FitCachCurveToward(g_cachCurveStart, camera.eyePosition, 0.30000001f, {0.0f, 20.0f, 40.0f});
    const std::int32_t pitch = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node).x : 0;
    const std::int32_t tilted = pitch - 3640;
    std::memcpy(&camera.target.y, &pitch, sizeof(pitch));
    std::memcpy(&camera.target.z, &tilted, sizeof(tilted));
}



void FitFramedReturnCurves() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    g_quadraticCurveRemaining = 40.0f;
    g_cachCurveAnchorA.acceleration = camera.appliedPosition;
    g_cachCurveAnchorA.velocity = camera.smoothedInput;
    g_cachCurveAnchorA.position = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    FitCachQuadraticCurve({0.0f, 12.0f, 40.0f}, g_cachCurveAnchorA);
    g_cachCurveStart.position = g_cameraCurvePosition;
    FitCachCurveToward(g_cachCurveStart, camera.cameraAux, 0.30000001f, {0.0f, 20.0f, 40.0f});
}




bool camCtrlApplyBlended_0() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 object = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto share = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], share);
    Vector3i rotation = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    if (kSeqFramesPerSecond > t) {
        std::int32_t tilted;
        std::memcpy(&tilted, &camera.target.z, sizeof(tilted));
        const auto delta = static_cast<std::int16_t>(tilted - rotation.x);
        rotation.x = static_cast<std::int32_t>(static_cast<float>(delta) * share + static_cast<float>(rotation.x));
    }
    if (camera.chara) {
        shenApplyTranslationToAll(camera.chara->entity.node, object);
        ApplyEulerAnglesToNode(camera.chara->entity.node, rotation);
    }
    camCtrlUpdateMatrixStack();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}




bool CalcCurveWithAngles() {
    const bool finished = AdvanceQuadraticCurve(2.0f, 0.2f);
    const float t = g_quadraticCurveRemaining;
    const Vector3 object = EvaluateCachCurve(g_cachCurveAnchorA, t);
    const Vector3 eye = EvaluateCachCurve(g_cachCurveStart, t);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto share = static_cast<float>(1.0 / (static_cast<double>(t) + 1.0));
    BlendEulerAngles(g_cachArmRotationA, camera.armEndRotation[0], share);
    BlendEulerAngles(g_cachArmRotationB, camera.armEndRotation[1], share);
    Vector3i rotation = camera.chara ? GetEulerAnglesForEntity(camera.chara->entity.node) : Vector3i{};
    const auto remaining = static_cast<float>(static_cast<double>(t) - 10.0);
    const float quotient = 2.0f / (1.0f > remaining ? 1.0f : remaining);
    const float pitchShare = 1.0f < quotient ? 1.0f : quotient;
    std::int32_t saved;
    std::memcpy(&saved, &camera.target.y, sizeof(saved));
    const auto delta = static_cast<std::int16_t>(saved - rotation.x);
    rotation.x = static_cast<std::int32_t>(static_cast<float>(delta) * pitchShare + static_cast<float>(rotation.x));
    if (camera.chara) {
        shenApplyTranslationToAll(camera.chara->entity.node, object);
        ApplyEulerAnglesToNode(camera.chara->entity.node, rotation);
    }
    camCtrlUpdateMatrixStack();
    g_cameraCurvePosition = eye;
    g_temporaryAcceleratedVector = camera.targetPosition;
    return finished;
}




void PlaceHandsForFramedRelease() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto *const armA = static_cast<Character *>(g_cameraTargetEntity);
    auto *const armB = static_cast<Character *>(g_shenmueCamera0);
    ShowCachCharacter(armA);
    ShowCachCharacter(armB);
    if (armA)
        shenApplyTranslationToAll(armA->entity.node, camera.hand[0].position);
    if (armB)
        shenApplyTranslationToAll(armB->entity.node, camera.hand[1].position);
    g_cachArmRotationA = camera.armJointRotation[0];
    g_cachArmRotationB = camera.armJointRotation[1];
    if (armA)
        ApplyEulerAnglesToNode(armA->entity.node, g_cachArmRotationA);
    if (armB)
        ApplyEulerAnglesToNode(armB->entity.node, g_cachArmRotationB);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorA), camera.handBaseRotation[0]);
    HND_ApplyRootEuler(*static_cast<HandModelWork *>(g_cameraAnchorB), camera.handBaseRotation[1]);
    g_cachHandRotationA = camera.handBaseRotation[0];
    camera.handRotation[0] = camera.handBaseRotation[0];
    g_cachHandRotationB = camera.handBaseRotation[1];
    camera.handRotation[1] = camera.handBaseRotation[1];
}



void BeginFramedExamination(Character *target) {
    if (InitializeExaminationTarget(target))
        return;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    if (static_cast<double>(camera.halfExtents.x) > 0.35)
        g_cameraStateFlags |= 0x200U;
    else
        g_cameraStateFlags &= ~0x200U;
    camera.appliedPosition = camera.chara ? GetPositionForEntity(camera.chara->entity.node) : Vector3{};
    camera.targetPosition = camera.modelBoundsCenterWorld;
    auto *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    camera.eyePosition = ryo ? GetPositionForEntity(ryo->entity.node) : Vector3{};
    camera.eyePosition.y = static_cast<float>(static_cast<double>(camera.eyePosition.y) + 1.65);
    const float half = camera.halfExtents.y;
    const float centreY = camera.modelBoundsCenterWorld.y;
    if (camera.eyePosition.y > centreY + half)
        camera.eyePosition.y = centreY + half;
    if (centreY - half > camera.eyePosition.y)
        camera.eyePosition.y = centreY - half;
    camera.eyePosition.x = camera.modelBoundsCenterWorld.x - camera.yawSine * 0.64999998f;
    camera.eyePosition.z = camera.yawCosine * 0.64999998f + camera.modelBoundsCenterWorld.z;
    camera.cameraAux = camera.eyePosition;
}


std::int32_t PollFramedExamination() {
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &anchorA = *static_cast<HandModelWork *>(g_cameraAnchorA);
    const std::int32_t step = g_cameraGlobalState;
    if (camera.state == 4) {
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartObserveMode();
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 0.0f, nullptr);
            PlaceFramedExaminationHands();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            ApplySceneSplineAndRotationSetup();
            camCtrlSetAnchorNodeTo(kTargetPoseDefault, static_cast<float>(static_cast<double>(g_quadraticCurveRemaining) * 0.7), nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (!AdjustCameraViewAndFoV())
                return 0;
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 20.0f, camera.chara);
            g_cameraGlobalState = step + 1;
            return 0;
        case 3:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                SyncCurrentPositionAndRotationFromCamera();
                FitFramedTakeCurves();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4:
            if (!camCtrlApplyBlended_0())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            FinishLiftedExaminationEnter();
            UpdateEntityPositionRotation(nullptr);
            UpdateHandMotData();
            HideCachCharacter(camera.chara);
            ++g_cameraGlobalState;
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    if (camera.state == 5) {
        switch (step) {
        case 0:
            if (!camCtrlUpdateAccelTerm())
                return 0;
            ShowCachCharacter(camera.chara);
            camCtrlSetAnchorNodeTo(kTargetPoseDefault, 0.0f, nullptr);
            PlaceHandsForFramedRelease();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            if ((anchorA.control & 8U) == 0)
                return 0;
            camCtrlSetAnchorNodeTo(kTargetGripPoseDefault, 20.0f, camera.chara);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if ((anchorA.control & 8U) != 0) {
                PlayCachQueuedSound(g_globalSoundCommand);
                SyncCurrentPositionAndRotationFromCamera();
                FitFramedReturnCurves();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 3:
            if (!CalcCurveWithAngles())
                return 0;
            PlayCachQueuedSound(g_soundCommand);
            UpdateEntityPositionRotation(nullptr);
            camCtrlSetAnchorNodeTo(kTargetPoseMode8, 10.0f, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            UpdateWithCurve();
            g_cameraGlobalState = step + 1;
            return 0;
        case 5:
            if (CalculateAndAdjustVectorsForCameraFieldOfView()) {
                UpdateHandMotData();
                shenEndObserveMode();
                ++g_cameraGlobalState;
            }
            return 0;
        default:
            camera.state = 0;
            g_cameraGlobalState = -1;
            return 1;
        }
    }
    camera.state = 0;
    return 1;
}

const char *SpecialAssetString(std::int32_t index) {
    const char *value = static_cast<const char *>(g_specialAssetType);
    while (index-- > 0) {
        value += std::strlen(value) + 1;
    }
    return value;
}

} // namespace

std::int32_t shenResetCamInteractStates(Character *entity, Character *cameraEntity) {
    g_cameraGlobalState = -1;
    auto *const control = static_cast<CameraControlState *>(g_cameraControl);
    auto *const camera = static_cast<CachContextState *>(g_camera);
    auto *const cached = static_cast<CachContextState *>(g_cacheData);
    Character *const ryo = g_playerCharacter;
    CharacterComponentNode *const anchorANode = HLib_GetCharacterComponent(entity, SHEN_NAME(HNDL));
    CharacterComponentNode *const anchorBNode = HLib_GetCharacterComponent(cameraEntity, SHEN_NAME(HNDR));
    if (!control || !camera || !cached || !entity || !cameraEntity || !ryo || !anchorANode || !anchorBNode || HND_LoadModels(entity) < 1 ||
        HND_LoadModels(cameraEntity) < 1) {
        g_cacheTemporaryCharacter = nullptr;
        g_shenmueCamera0 = nullptr;
        g_cameraTargetEntity = nullptr;
        g_cameraAnchorB = nullptr;
        g_cameraAnchorA = nullptr;
        return -1;
    }

    g_cacheTemporaryCharacter = ryo;
    g_cameraTargetEntity = entity;
    g_cameraAnchorA = anchorANode + 1;
    g_shenmueCamera0 = cameraEntity;
    g_cameraAnchorB = anchorBNode + 1;
    ccowDisableCharaFlags(entity, 7);
    ccowDisableCharaFlags(cameraEntity, 7);
    UpdateHandMotData();
    camera->state = 0;
    control->mode = 0;
    camera->chara = nullptr;
    control->chara = nullptr;
    camera->observeMode = 0;
    control->investigateMode = 0;
    ClearCachAttachments(*camera, false);
    std::memcpy(cached, camera, 0x2b0);
    return 0;
}

std::int32_t shenApplyTransformsForInteractStates() {
    if (!g_cameraAnchorA) {
        return -1;
    }
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &cached = *static_cast<CachContextState *>(g_cacheData);
    if ((control.mode != 0 && (control.mode != 1 || g_cameraGlobalState >= 0)) || (camera.state != 0 && (camera.state != 4 || g_cameraGlobalState >= 0)) ||
        (cached.state != 0 && (cached.state != 4 || g_cameraGlobalState >= 0))) {
        return -1;
    }
    if (control.mode == 1) {
        ApplyPosAndEulerAnglesForNodes();
    }
    if (camera.state == 4) {
        ApplyInteractionResult(camera);
    }
    if (cached.state == 4) {
        ApplyInteractionResult(cached);
    }
    for (std::size_t index = 0; index < 2; ++index) {
        for (CachAttachmentRecord *const attachment : {&camera.attachments[index], &cached.attachments[index]}) {
            if ((reinterpret_cast<std::uintptr_t>(attachment->modelOwner) & ~std::uintptr_t{0xf}) != 0)
                FreePVR(attachment->modelOwner);
            attachment->modelOwner = nullptr;
        }
    }
    camera.state = 0;
    cached.state = 0;
    g_cameraGlobalState = -1;
    control.mode = 0;
    camera.chara = nullptr;
    cached.chara = nullptr;
    control.chara = nullptr;
    camera.observeMode = 0;
    cached.observeMode = 0;
    control.investigateMode = 0;
    UpdateHandMotData();
    return 0;
}



std::int32_t CACH_BeginTargetMode(Character *target, std::uint16_t requestedMode) {
    if (!g_cameraAnchorA)
        return -1;
    std::int32_t result = 1;
    if (!target)
        target = SelectAutomaticCachTarget();
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    g_cameraStateFlags &= ~0x1000U;
    if (control.mode != 0)
        return -1;
    control.chara = target;
    if (target) {
        InitializeTargetMode(requestedMode);
        if (control.inputMode >= 0) {
            control.mode = 1;
            result = 0;
            g_cameraGlobalState = 0;
        }
    }
    return result;
}




static void shenStartInvestigateMode(CameraControlState &control) {
    control.gameState = g_temporaryGameState;
    control.newShenState = g_temporaryGameState == 0 ? g_nAuthorState : 0;
    control.savedEye = g_CameraEyePos;
    control.savedTarget = g_CameraTargetPos;
    g_temporaryGameState = 0;
    ApplyPendingGameState(0);
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = 9;
    ApplyPendingGameState(9);
    g_currentHelpIconState = 0;
    control.investigateMode = 1;
    g_cameraCurvePosition = control.savedEye;
    g_temporaryAcceleratedVector = control.savedTarget;
    control.helpToken = g_defaultHelpTokenId;
    control.helpButton = g_helpButtonId;
    g_previousHelpTokenId = g_currentHelpTokenId;
    g_defaultHelpTokenId = 2;
    g_currentHelpTokenId = 2;
    g_helpButtonId = 0;
}



static void SetHelpTokenID(CameraControlState &control) {
    g_bIsConversationCamInactive = 0;
    g_temporaryGameState = control.gameState;
    ApplyPendingGameState(control.gameState);
    const std::int32_t previousAuthorState = control.newShenState;
    SRCH_ReleaseLockOnCamera();
    g_previousAuthorState = previousAuthorState;
    ApplyPendingGameState(control.gameState);
    g_currentHelpIconState = 0;
    control.investigateMode = -1;
    g_previousHelpTokenId = g_currentHelpTokenId;
    const std::uint8_t token = static_cast<std::int8_t>(control.helpToken) >= 68 ? 0 : control.helpToken;
    g_helpButtonId = control.helpButton;
    g_defaultHelpTokenId = token;
    g_currentHelpTokenId = token;
}




std::int32_t CACH_UpdateTargetModeStateMachine() {
    if (!g_cameraAnchorA)
        return -1;
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive)
        return 0;

    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    const auto playQueued = [](std::uint32_t &command) {
        if (!command)
            return;
        shenExecuteAICACommand(command, 0, 0);
        command = 0;
    };
    const std::int32_t step = g_cameraGlobalState;
    switch (control.mode) {
    case 1:
        switch (step) {
        case 0:
            UpdateHandMotData();
            shenStartInvestigateMode(control);
            InitializeTargetModeHands();
            ++g_cameraGlobalState;
            return 0;
        case 1:
            CalculateQuadraticCurves();
            camCtrlSetAnchorNodeTo(control.inputMode == 8 ? kTargetPoseMode8 : kTargetPoseDefault, g_quadraticCurveRemaining, nullptr);
            g_cameraGlobalState = step + 1;
            return 0;
        case 2:
            if (!AdvanceTargetReach())
                return 0;
            CaptureTargetGrip();
            camCtrlSetAnchorNodeTo(control.inputMode == 8 ? kTargetGripPoseMode8 : kTargetGripPoseDefault, 20.0f, control.chara);
            g_cameraGlobalState = step + 1;
            return 0;
        case 3:
            if (UpdateAccelerationCurve()) {
                playQueued(g_globalSoundCommand);
                UpdateEntityPositionRotation(control.chara);
                ResetCurve();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        case 4:
            if (!UpdateCurveAndApply())
                return 0;
            playQueued(g_soundCommand);
            g_cameraGlobalState = step + 1;
            return 0;
        default:
            g_cameraGlobalState = -1;
            return 1;
        }
    case 2:
        switch (step) {
        case 0:
            if (!SettleTargetModeInput(control))
                return 0;
            Vector3_RotateVectorWithinLimits(control.referenceRotation, control.gripRotation);
            InitializeTargetTransitionSpeed(control);
            BuildTargetTransitionCurveA(control);
            playQueued(g_globalSoundCommand);
            g_cameraGlobalState = step + 1;
            return 0;
        case 1:
            if (AdjustCameraPositionAndOrientationBasedOnDistance()) {
                playQueued(g_soundCommand);
                UpdateEntityPositionRotation(nullptr);
                camCtrlSetAnchorNodeTo(control.inputMode == 8 ? kTargetPoseMode8 : kTargetPoseDefault, 20.0f, nullptr);
                g_cameraGlobalState = step + 1;
                g_quadraticCurveRemaining = 5.0f;
            }
            return 0;
        case 2:
            g_quadraticCurveRemaining = g_quadraticCurveRemaining + -2.0f;
            if (g_quadraticCurveRemaining < 2.0f)
                ++g_cameraGlobalState;
            return 0;
        case 3:
            BuildTargetReleaseCurves();
            g_cameraGlobalState = step + 1;
            return 0;
        case 4:
            if (UpdateCameraAlongCurve()) {
                ApplyPosAndEulerAnglesForNodes();
                g_cameraGlobalState = step + 1;
            }
            return 0;
        default:
            g_cameraGlobalState = 0;
            control.mode = 6;
            return 0;
        }
    case 3:
        if (step == 0) {
            if (!SettleTargetModeInput(control))
                return 0;
            InitializeTargetTransitionSpeed(control);
            BuildTargetTransitionCurveB(control);
            playQueued(g_globalSoundCommand);
            ++g_cameraGlobalState;
            return 0;
        }
        if (step == 1) {
            if (!AdvanceCameraRailCurveForFieldOfView())
                return 0;
            playQueued(g_soundCommand);
            UpdateEntityPositionRotation(nullptr);
            ApplyPosAndEulerAnglesForNodes();
            if (Character *const target = control.chara) {
                const bool wasVisible = (target->motionFlags & 4U) != 0;
                target->motionFlags &= static_cast<std::uint8_t>(~4U);
                if (g_bDebugLogVisibility && (target->flags.value & 0x400U) != 0 && wasVisible)
                    CHAR_LogNPCVisibility(target, false);
            }
            if (control.chara)
                shenApplyTranslationToAll(control.chara->entity.node, {100000.0f, 100000.0f, 100000.0f});
            ++g_cameraGlobalState;
            return 0;
        }
        g_cameraGlobalState = 0;
        control.mode = 6;
        return 0;
    case 6:
        if (step == 0) {
            UpdateHandMotData();
            SetHelpTokenID(control);
            ++g_cameraGlobalState;
            return 0;
        }
        control.mode = 0;
        g_cameraGlobalState = -1;
        return 1;
    default:
        control.mode = 0;
        return 1;
    }
}




std::int32_t CACH_UpdateInterpolatedTargetSpeed() {
    if (!g_cameraAnchorA)
        return -1;
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive)
        return CameraSpeedPercentage(control);
    if (control.mode != 1 || g_cameraGlobalState >= 0)
        return -1;
    auto *const map = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const PadCaptureEntry &pad = *static_cast<PadCaptureEntry *>(map->padCaptureEntry);
    std::int32_t direction = 0;
    if ((pad.held & 0x9000U) != 0) {
        if ((pad.held & 0x6000U) == 0)
            direction = 1;
    } else if ((pad.held & 0x6000U) != 0) {
        direction = 2;
    }
    float &velocity = control.speedVelocity;
    const auto wide = [](float value) { return static_cast<double>(value); };
    if (direction == 1) {
        if (wide(velocity) > -0.4) {
            velocity = static_cast<float>(wide(velocity) - 0.8);
        } else {
            const float room = control.speedMinimum - control.speedCurrent;
            const auto stopping = static_cast<float>((wide(room) + wide(room)) / wide(velocity));
            const auto braking = static_cast<float>(wide(velocity) / -0.6000000000000001);
            if (stopping > braking) {
                velocity = static_cast<float>(wide(velocity) - 0.4);
                if (-6.0f > velocity)
                    velocity = -6.0f;
            } else {
                velocity = static_cast<float>(wide(velocity) + 0.6000000000000001);
                if (wide(velocity) > -0.4)
                    velocity = -0.40000001f;
            }
        }
    } else if (direction == 2) {
        if (0.4 > wide(velocity)) {
            velocity = static_cast<float>(wide(velocity) + 0.8);
        } else {
            const float room = control.speedMaximum - control.speedCurrent;
            const auto stopping = static_cast<float>((wide(room) + wide(room)) / wide(velocity));
            const auto braking = static_cast<float>(wide(velocity) / 0.6000000000000001);
            if (stopping > braking) {
                velocity = static_cast<float>(wide(velocity) + 0.4);
                if (velocity > 6.0f)
                    velocity = 6.0f;
            } else {
                velocity = static_cast<float>(wide(velocity) - 0.6000000000000001);
                if (0.4 > wide(velocity))
                    velocity = 0.40000001f;
            }
        }
    } else if (velocity > 0.0f) {
        velocity = static_cast<float>(wide(velocity) - 0.6000000000000001);
        if (0.0f > velocity)
            velocity = 0.0f;
    } else {
        velocity = static_cast<float>(wide(velocity) + 0.6000000000000001);
        if (velocity > 0.0f)
            velocity = 0.0f;
    }
    const bool smoothing = g_bEnableFrameTimeSmoothing;
    const float frameMs = fFrameTimeMs;
    float step = velocity;
    if (smoothing)
        step = step * kSeqFramesPerSecond * frameMs;
    control.speedCurrent = step + control.speedCurrent;
    if (control.speedMinimum > control.speedCurrent)
        control.speedCurrent = control.speedMinimum;
    if (control.speedCurrent > control.speedMaximum)
        control.speedCurrent = control.speedMaximum;
    ConvertTargetSpeedToEuler(control, control.gripRotation);
    control.gripOffset[1] = {};
    control.gripOffset[2] = {};
    CalculateMatrixEulerAngles();
    auto *const camera0 = static_cast<Character *>(g_shenmueCamera0);
    Vector3 &follow = control.gripOffset[1];
    follow = camera0 ? GetPositionForEntity(camera0->entity.node) : Vector3{};
    const auto response = static_cast<float>(static_cast<double>(control.speedCurrent) / 100.0 * 0.2 + 0.6);
    follow = {(control.camera0Position.x - follow.x) * response, (control.camera0Position.y - follow.y) * response,
        (control.camera0Position.z - follow.z) * response};
    const float stepX = smoothing ? frameMs * 10920.0f : 364.0f;
    control.swayPhase[0] = static_cast<std::int32_t>(static_cast<float>(control.swayPhase[0]) + stepX);
    const float stepY = smoothing ? frameMs * 16380.0f : 546.0f;
    control.swayPhase[1] = static_cast<std::int32_t>(static_cast<float>(control.swayPhase[1]) + stepY);
    const float stepZ = smoothing ? frameMs * 21840.0f : 728.0f;
    control.swayPhase[2] = static_cast<std::int32_t>(static_cast<float>(control.swayPhase[2]) + stepZ);
    const auto wobble = [](std::int32_t &phase) {
        phase = static_cast<std::uint16_t>(phase);
        return static_cast<float>(static_cast<double>(sin_fixed16(static_cast<std::int16_t>(phase))) * 0.005);
    };
    control.gripOffset[2].x = wobble(control.swayPhase[0]);
    control.gripOffset[2].y = wobble(control.swayPhase[1]);
    control.gripOffset[2].z = wobble(control.swayPhase[2]);
    CalculateMatrixEulerAngles();
    TransformMatrixCoordinatesAndCalculateEulerAngles();
    const auto look = [smoothing, frameMs](float &axis, std::int32_t reading) {
        float delta = CameraInputDelta(axis, reading);
        if (smoothing)
            delta = delta * kSeqFramesPerSecond * frameMs;
        axis = axis + delta;
    };
    look(control.analogDelta.x, static_cast<std::int32_t>(pad.rightStick & 0xffU) - 128);
    look(control.analogDelta.y, static_cast<std::int32_t>(pad.rightStick >> 8) - 128);
    look(control.analogDelta.z, (static_cast<std::int32_t>(pad.triggers >> 8) - static_cast<std::int32_t>(pad.triggers & 0xffU) - 256) / 8);
    const auto hold = [](float base, float offset) { return static_cast<float>(static_cast<double>(offset) * 0.7 + static_cast<double>(base)); };
    const Vector3 target{hold(control.gripOffset[0].x, control.gripOffset[1].x), hold(control.gripOffset[0].y, control.gripOffset[1].y),
        hold(control.gripOffset[0].z, control.gripOffset[1].z)};
    ConfigureTargetModeCamera(control, control.eye, target, control.targetFov);
    return CameraSpeedPercentage(control);
}

std::int32_t CACH_BeginInteractionMode(void *targetOrPosition, std::int32_t type) {
    if (!g_cameraAnchorA)
        return -1;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    Character *const ryo = static_cast<Character *>(g_cacheTemporaryCharacter);
    CacheActiveInteraction();
    camera.analogDelta = {};
    camera.savedPlayerOpacity = ryo ? ryo->renderOpacity : 1.0f;
    camera.savedFov = currentPerspectiveFov;
    g_cameraStateFlags &= ~0x2000U;
    camera.examineFov = 10922;
    if (camera.state != 0 || control.mode != 0)
        return -1;

    camera.interactionType = static_cast<std::int16_t>(type);
    switch (camera.interactionType) {
    case 0:
    case 1:
    case 2:
    case 3:
        BeginHeldExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 10:
    case 11:
        BeginSlidingExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 20:
    case 21:
        BeginHingedExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 30:
        BeginLiftedExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 40:
        BeginTakenExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 50:
        BeginFramedExamination(static_cast<Character *>(targetOrPosition));
        break;
    case 60:
        BeginPositionExamination(static_cast<const Vector3 *>(targetOrPosition));
        break;
    default:
        camera.interactionType = -1;
        break;
    }
    if (camera.interactionType < 0)
        return 1;
    g_cameraGlobalState = 0;
    camera.state = 4;
    return 0;
}

std::int32_t CACH_PollInteractionMode() {
    if (!g_cameraAnchorA)
        return -1;
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive)
        return 0;
    auto &camera = *static_cast<CachContextState *>(g_camera);
    std::int32_t result;
    switch (camera.interactionType) {
    case 0:
    case 1:
    case 2:
    case 3:
        result = PollHeldExamination();
        break;
    case 10:
    case 11:
        result = PollSlidingExamination();
        break;
    case 20:
    case 21:
        result = PollHingedExamination();
        break;
    case 30:
        result = PollLiftedExamination();
        break;
    case 40:
        result = PollTakenExamination();
        break;
    case 50:
        result = PollFramedExamination();
        break;
    case 60:
        result = PollPositionExamination();
        break;
    default:
        return -1;
    }
    if (camera.state == 4 && g_cameraGlobalState < 0)
        camCtrlGetCurveEyeAndTarget(&camera.lastCurvePosition, &camera.lastAcceleratedTarget);
    if (camera.state == 0) {
        ClearCachAttachments(camera, true);
        auto &cached = *static_cast<CachContextState *>(g_cacheData);
        if (cached.state != 0) {
            std::memcpy(&camera, &cached, 0x2b0);
            cached.state = 0;
            cached.chara = nullptr;
            cached.observeMode = 0;
            ClearCachAttachments(cached, false);
        }
    }
    return result;
}

std::int32_t CACH_LoadSpecialAssets(const SpecialAssetScriptRecord records[2]) {
    if (!g_cameraAnchorA) {
        return -1;
    }
    auto &camera = *static_cast<CachContextState *>(g_camera);
    auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    CacheActiveInteraction();
    if (camera.attachments[0].modelOwner || camera.attachments[1].modelOwner || !g_specialAssetType || camera.state != 0 || control.mode != 0) {
        return -1;
    }
    for (std::size_t index = 0; index < 2; ++index) {
        const SpecialAssetScriptRecord &record = records[index];
        if (record.stringIndex < 0) {
            continue;
        }
        const char *assetName = SpecialAssetString(record.stringIndex);
        if (!*assetName) {
            continue;
        }
        char directory[64]{};
        const char *filename = assetName;
        switch (*filename) {
        case '@':
            std::snprintf(directory, sizeof(directory), "scene/%02u/model/chara", g_sceneId);
            ++filename;
            break;
        case '%':
            std::memcpy(directory, "model/item", sizeof("model/item"));
            ++filename;
            break;
        case '$':
            std::snprintf(directory, sizeof(directory), "scene/%02u/%c%c%c%c", g_sceneId, static_cast<char>(g_areaId), static_cast<char>(g_areaId >> 8),
                static_cast<char>(g_areaId >> 16), static_cast<char>(g_areaId >> 24));
            ++filename;
            break;
        case '#':
            std::snprintf(directory, sizeof(directory), "scene/%02u/model/item", g_sceneId);
            ++filename;
            break;
        default:
            std::memcpy(directory, "model/chara", sizeof("model/chara"));
            break;
        }
        void *const file = ReadFile(directory, filename, static_cast<std::int32_t>(SHEN_NAME(CACH)));
        PackModel *owner = reinterpret_cast<PackModel *>(1);
        if (file) {
            char fullPath[260]{};
            std::snprintf(fullPath, sizeof(fullPath), "%s/%s", directory, filename);
            owner = MT5_InitRawModelAndTextures(file, filename, fullPath);
            Shenmue_FileRead_ALL(reinterpret_cast<std::int64_t>(owner->model), fullPath, 0, 2);
        }
        camera.attachments[index].modelOwner = owner;
        camera.attachments[index].position = record.position;
        camera.attachments[index].rotation = record.rotation;
    }
    camera.attachmentOpacity = 0.0f;
    return 0;
}



std::int32_t CACH_UpdateSpecialAssetInput() {
    if (!g_cameraAnchorA) {
        return -1;
    }
    if (!g_isPlayerGameStateActive || g_notebookHelpOverrideActive) {
        return 0;
    }
    auto &camera = *static_cast<CachContextState *>(g_camera);
    const auto &control = *static_cast<CameraControlState *>(g_cameraControl);
    if (camera.state != 4 || control.mode != 0 || g_cameraGlobalState >= 0) {
        return -1;
    }
    auto *const map = g_mapControlTask ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    const PadCaptureEntry &pad = *static_cast<PadCaptureEntry *>(map->padCaptureEntry);
    std::int32_t x = (pad.held & 0x4000U) == 0 ? pad.rightStick & 0xffU : 0;
    if (pad.held & 0x8000U)
        x = 255;
    std::int32_t y = (pad.held & 0x1000U) == 0 ? pad.rightStick >> 8 : 0;
    if (pad.held & 0x2000U)
        y = 255;
    const auto triggerHigh = static_cast<std::int32_t>(pad.triggers >> 8);
    const auto triggerLow = static_cast<std::int32_t>(pad.triggers & 0xffU);
    const std::int32_t z = (triggerHigh - triggerLow - 256) / 8;
    camera.analogDelta.x += CameraInputDelta(camera.analogDelta.x, x - 128);
    camera.analogDelta.y += CameraInputDelta(camera.analogDelta.y, y - 128);
    camera.analogDelta.z += CameraInputDelta(camera.analogDelta.z, z);
    camCtrlUpdateAcceleration();
    return 0;
}

std::int32_t CACH_RequestTargetTransition(std::int16_t mode) {
    auto *const control = static_cast<CameraControlState *>(g_cameraControl);
    if (!g_cameraAnchorA || control->mode != 1 || g_cameraGlobalState >= 0) {
        return -1;
    }
    control->mode = mode;
    g_cameraGlobalState = 0;
    return 0;
}

std::int32_t CACH_RequestInteractionExit() {
    auto *const camera = static_cast<CachContextState *>(g_camera);
    auto *const control = static_cast<CameraControlState *>(g_cameraControl);
    if (!g_cameraAnchorA || camera->state != 4 || control->mode != 0 || g_cameraGlobalState >= 0) {
        return -1;
    }
    camera->state = 5;
    g_cameraGlobalState = 0;
    return 0;
}

std::int32_t CACH_SetSpecialAssetList(void *list) {
    if (!g_cameraAnchorA) {
        return -1;
    }
    g_specialAssetType = list;
    return 0;
}

void CACH_QueueSoundCommands(std::uint32_t globalCommand, std::uint32_t soundCommand) {
    g_globalSoundCommand = globalCommand;
    g_soundCommand = soundCommand;
}

void AssignHNDP() {
    g_handNodeRight = nullptr;
    g_handNodeLeft = nullptr;
    g_handNodePoolSecondary = allocateHandPoolFromBack(0x1200, SHEN_NAME(HNDP));
    g_handNodePoolPrimary = allocateHandPoolFromBack(0x1200, SHEN_NAME(HNDP));
    g_handOptionalMatrixPool = allocateHandPoolFromBack(0x1200, SHEN_NAME(HNDP));
    g_handGlobalTaskData[0] = nullptr;
    g_handGlobalTaskData[1] = nullptr;
    g_handMotionHead = nullptr;
    g_handRuntime_18888 = nullptr;
    g_handRuntime_188a8 = nullptr;

    HND_ResetMotionModels();
    HND_SetMatrixPools(
        static_cast<Matrix4x4 *>(g_handNodePoolSecondary), static_cast<Matrix4x4 *>(g_handNodePoolPrimary), static_cast<Matrix4x4 *>(g_handOptionalMatrixPool));

    g_handCollisionPool = allocateHandPoolFromBack(0x25c0, SHEN_NAME(HNDC));
    CACH_Enqueue();
}

void CACH_Init() {
    for (FaceModelCacheSlot &slot : g_faceModelCache) {
        slot.state = 0;
        slot.modelRoot = nullptr;
        slot.modelOwner = nullptr;
        std::memcpy(slot.filename, "NULL", 5);
    }

    AssignHNDP();
    if (!g_TaskInfo.bTaskPauseEnqueue && HLib::NextTask) {
        ESOP_Init();
        if (HLib::NextTask)
            UPDB_Init();
    }
    UpdateScenarioProgressStep();
}

} // namespace shenmue::s1
