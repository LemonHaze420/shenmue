#include "s1/task/hato.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/char.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/drwp.hpp"
#include "s1/task/obje.hpp"
#include "s1/task/cycle_man.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/sond.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

extern std::int32_t g_actNumber;



HatoTaskData *TempHATO = nullptr;
std::int32_t g_bHatoEnabled = 0;
std::int16_t g_hato_ticks = 0;

namespace {

constexpr std::int32_t kHatoToken = static_cast<std::int32_t>(SHEN_NAME(HATO));
constexpr float kFixedAngleScale = 8192.0f / 45.0f;
constexpr float kRandomScale = 1.0f / 32768.0f;
constexpr std::uint32_t kUpdatePending = 0x8000U;
constexpr std::size_t kHatoAllocationSize = 0x5700;
constexpr float kStepSize = 0.2f;



std::array<PackModel *, 4> g_hatoModelOwners{};

std::uint32_t StepPrng(std::uint32_t seed) noexcept {
    return LcgStep(seed);
}

float Random15(std::uint32_t seed) noexcept {
    return static_cast<float>((seed >> 16U) & 0x7fffU) * kRandomScale;
}

float SinFixed16(std::int32_t angle) noexcept {
    return ::shenmue::sin_fixed16(static_cast<std::int16_t>(angle));
}

float DistanceXZ(const Vector3 &lhs, const Vector3 &rhs) noexcept {
    const float dx = lhs.x - rhs.x;
    const float dz = lhs.z - rhs.z;
    return std::sqrt(dx * dx + dz * dz);
}

std::int16_t AddFixedAngle(std::int16_t angle, float delta) noexcept {
    const auto result = static_cast<std::int32_t>(static_cast<float>(angle) + delta);
    return static_cast<std::int16_t>(result & 0xffff);
}

std::int32_t NativeAtanQuotient(float numerator, float denominator) {
    return static_cast<std::int32_t>(std::atan(numerator / denominator) * 65536.0f / 6.2831855f + 0.5f);
}

std::uint16_t NativeFixedAtan2(float value, float direction) {
    if (direction < value) {
        if (direction < -value) {
            return direction == 0.0f ? 0x8000U : static_cast<std::uint16_t>(NativeAtanQuotient(value, direction) + 0x8000);
        }
        return value == 0.0f ? 0x4000U : static_cast<std::uint16_t>(0x4000 - NativeAtanQuotient(direction, value));
    }
    if (direction < -value) {
        return value == 0.0f ? 0xc000U : static_cast<std::uint16_t>(0xc000 - NativeAtanQuotient(direction, value));
    }
    return direction == 0.0f ? 0U : static_cast<std::uint16_t>(NativeAtanQuotient(value, direction));
}

float CalculatePigeonAngle(const Vector3 &target, const Vector3 &pigeon, std::int16_t angle) noexcept {
    const float dx = target.x - pigeon.x;
    const float dz = target.z - pigeon.z;
    const std::uint16_t negativeYaw = static_cast<std::uint16_t>(-angle);
    const float sine = ::shenmue::sin_fixed16(static_cast<std::int16_t>(negativeYaw));
    const float cosine = ::shenmue::sin_fixed16(static_cast<std::int16_t>(::shenmue::fixed16_cosine_angle(negativeYaw)));
    const float rotatedX = dx * cosine + dz * sine;
    const float rotatedZ = -dx * sine + dz * cosine;
    const std::uint16_t fixed = NativeFixedAtan2(-rotatedZ, rotatedX);
    return static_cast<float>(fixed) / 182.04445f - 90.0f;
}

bool IsPointNearSegmentXZ(const Vector3 &start, const Vector3 &end, const Vector3 &point, float distance) noexcept {
    if (start.x == end.x && start.z == end.z)
        return true;
    if (DistanceXZ(point, start) < distance || DistanceXZ(point, end) < distance) {
        return true;
    }

    const float dx = end.x - start.x;
    const float dz = end.z - start.z;
    const float lengthSquared = dx * dx + dz * dz;
    if (lengthSquared <= 0.0f)
        return true;
    const float projection = (dx * (point.x - start.x) + dz * (point.z - start.z)) / lengthSquared;
    if (projection < 0.0f || projection > 1.0f)
        return false;
    const Vector3 closest{start.x + dx * projection, point.y, start.z + dz * projection};
    return DistanceXZ(closest, point) <= distance;
}

MapControlData *CurrentMapControl() noexcept {
    return g_mapControlTask && g_mapControlTask->task_data_ptr ? reinterpret_cast<MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
}

Character *CurrentRyo() noexcept {
    MapControlData *const control = CurrentMapControl();
    return control ? static_cast<Character *>(control->ryo) : nullptr;
}

Vector3 CharacterPosition(Character *character) noexcept {
    return character ? GetPositionForEntity(character->entity.node) : Vector3{};
}

void PlayTakeoffSound(const Vector3 &position) {
    Execute3DSoundCommand(0x705a9U, 4U, &position, 4.0f, 40.0f, 0U);
}

float ApplyAvoidanceTarget(HatoEntity &entity, const Vector3 &target, float &angleSum, std::int32_t &angleCount) {
    entity.pitch = 0;
    entity.roll = 0;
    const float distance = DistanceXZ(target, entity.position);
    if (distance >= 2.5f)
        return 255.0f;

    if (entity.state != 4U) {
        if (distance >= 1.5f) {
            entity.state = 2;
        } else {
            entity.state = 4;
            if (g_hato_ticks <= 0) {
                PlayTakeoffSound(entity.position);
                g_hato_ticks = 15;
            }
        }
    }

    float direction = CalculatePigeonAngle(target, entity.position, entity.yaw);
    angleSum += direction;
    ++angleCount;
    if (direction <= 0.0f || direction >= 180.0f) {
        if (direction < 0.0f)
            direction += 360.0f;
        entity.yaw = AddFixedAngle(entity.yaw, (direction - 180.0f) / 5.0f * kFixedAngleScale);
    } else {
        entity.yaw = AddFixedAngle(entity.yaw, (180.0f - direction) / 5.0f * -kFixedAngleScale);
    }
    return distance;
}

float ApplyCharacterAvoidance(HatoEntity &entity, float &averageAngle) {
    float angleSum = 0.0f;
    std::int32_t angleCount = 0;
    Character *const ryo = CurrentRyo();
    float result = ApplyAvoidanceTarget(entity, CharacterPosition(ryo), angleSum, angleCount);

    for (CycleManager::NpcRecord &npc : g_cycleManagerRuntime.npcRecords) {
        if (npc.programList != CycleProgramList_SimSet || !npc.cycle->obj || npc.cycle->obj == ryo) {
            continue;
        }
        result = ApplyAvoidanceTarget(entity, CharacterPosition(npc.cycle->obj), angleSum, angleCount);
    }

    averageAngle = angleCount > 0 ? angleSum / static_cast<float>(angleCount) : 0.0f;
    return result;
}

void UpdateEntity(HatoEntity &entity) {
    HatoTaskData *const data = TempHATO;
    const Vector3 ryoPosition = CharacterPosition(CurrentRyo());
    const float ryoDistance = DistanceXZ(ryoPosition, entity.position);

    if (ryoDistance > 30.0f && entity.alpha > 0.0f) {
        entity.alpha -= 0.05f;
        if (entity.alpha > 0.0f) {
            entity.renderFlag = 0;
        } else {
            entity.alpha = 0.0f;
            entity.renderFlag = 1;
        }
    } else {
        if (ryoDistance >= 28.0f && entity.alpha <= 0.0f) {
            entity.renderFlag = 0;
            return;
        }
        entity.alpha = std::min(1.0f, entity.alpha + 0.05f);
        entity.renderFlag = 1;
    }

    float lastAvoidanceDistance = 0.0f;
    if (entity.state == 4U && entity.flightAngle > 180.0f) {
        entity.flightAngle = 0.0f;
        entity.state = 3;
    }
    if (entity.state != 4U) {
        entity.state = 3;
        if (static_cast<std::int8_t>(entity.avoidanceTimer) > 0) {
            --entity.avoidanceTimer;
        } else {
            float averageAngle = 0.0f;
            lastAvoidanceDistance = ApplyCharacterAvoidance(entity, averageAngle);
        }
    }

    entity.pitch = 0;
    entity.roll = 0;
    const float centerDistance = DistanceXZ(data->center, entity.position);
    const float maxDistance = data->maxDistance;
    bool randomTurn = false;
    float randomTurnDegrees = 0.0f;
    float proportion = 0.0f;
    std::uint32_t newSeed = g_EnvPrngSeed;

    if (centerDistance > maxDistance || static_cast<std::int8_t>(entity.turnTimer) <= 0) {
        if (((centerDistance - maxDistance) - 1.0f) <= 2.0f) {
            if (centerDistance <= maxDistance) {
                randomTurn = true;
                newSeed = StepPrng(newSeed);
                randomTurnDegrees = Random15(newSeed) * 4.0f + 3.0f;
            } else if (lastAvoidanceDistance > 255.0f) {
                const std::uint32_t first = StepPrng(newSeed);
                proportion = Random15(first) * 19.0f + 1.0f;
                newSeed = StepPrng(first);
                entity.turnTimer = static_cast<std::uint8_t>(10 * (1 - static_cast<std::int32_t>(Random15(newSeed) * -1.0f)));
            } else {
                newSeed = StepPrng(newSeed);
                entity.avoidanceTimer = 30;
                proportion = Random15(newSeed) * 5.0f + 3.0f;
            }
            g_EnvPrngSeed = newSeed;
        } else {
            proportion = 1.0f;
        }

        float targetAngle = CalculatePigeonAngle(data->center, entity.position, entity.yaw);
        float adjustedAngle = targetAngle;
        float delta = 0.0f;
        if (targetAngle <= 0.0f || targetAngle >= 180.0f) {
            if (targetAngle < 0.0f)
                adjustedAngle += 360.0f;
            delta = randomTurn ? randomTurnDegrees * -kFixedAngleScale : (180.0f - (adjustedAngle - 180.0f)) / proportion * -kFixedAngleScale;
        } else {
            delta = randomTurn ? randomTurnDegrees * kFixedAngleScale : targetAngle / proportion * kFixedAngleScale;
        }
        entity.yaw = AddFixedAngle(entity.yaw, delta);
    }

    if (entity.state == 4U) {
        const std::uint16_t aheadAngle = static_cast<std::uint16_t>(entity.yaw + 0x8000);
        const std::uint16_t reflected = aheadAngle <= 0x8000U ? aheadAngle : static_cast<std::uint16_t>(-aheadAngle);
        Vector3 ahead = entity.position;
        ahead.x += SinFixed16(aheadAngle) * 4.0f;
        ahead.z += SinFixed16(0x4000 - reflected) * 4.0f;
        if (IsPointNearSegmentXZ(entity.position, ahead, ryoPosition, 0.5f)) {
            const float direction = CalculatePigeonAngle(ryoPosition, entity.position, entity.yaw);
            entity.yaw = static_cast<std::int16_t>(entity.yaw + ((direction <= 0.0f || direction >= 180.0f) ? 1456 : -1456));
        }
    }

    const float speed =
        entity.state == 4U ? kStepSize : ((lastAvoidanceDistance < 255.0f || static_cast<std::int8_t>(entity.avoidanceTimer) > 0) ? 0.09f : 0.015f);
    const std::uint16_t heading = static_cast<std::uint16_t>(entity.yaw + 0x8000);
    const std::uint16_t reflectedHeading = heading <= 0x8000U ? heading : static_cast<std::uint16_t>(-heading);
    entity.position.x += SinFixed16(heading) * speed;
    entity.position.z += SinFixed16(0x4000 - reflectedHeading) * speed;
    if (entity.state == 4U) {
        entity.flightAngle += 3.0f;
        entity.position.y = SinFixed16(static_cast<std::int32_t>(entity.flightAngle * kFixedAngleScale)) + 0.11f;
    } else {
        entity.position.y = 0.11f;
    }

    if (static_cast<std::int8_t>(entity.turnTimer) <= 0) {
        newSeed = StepPrng(newSeed);
        g_EnvPrngSeed = newSeed;
        entity.turnTimer = static_cast<std::uint8_t>(10 * (1 - static_cast<std::int32_t>(Random15(newSeed) * -1.0f)));
    } else {
        --entity.turnTimer;
    }
}

Mt5RuntimeNode *HatoFrameNode(const HatoEntity &entity) {
    const std::int32_t variant = static_cast<std::int8_t>(entity.modelVariant & 0xffU);
    if (variant < 0 || variant >= static_cast<std::int32_t>(g_hatoModelOwners.size())) {
        return nullptr;
    }

    const std::uint32_t state = entity.state & ~kUpdatePending;
    const std::int32_t frame = static_cast<std::int8_t>(static_cast<std::int32_t>(entity.animationFrame));
    auto *const root = static_cast<Mt5RuntimeNode *>(TempHATO->models[static_cast<std::size_t>(variant)]);
    std::byte *const modelBase = root ? MT5_ModelBaseForNode(root) : nullptr;
    if (!modelBase || !root->childOffset)
        return nullptr;
    auto *node = reinterpret_cast<Mt5RuntimeNode *>(modelBase + root->childOffset);

    auto advanceSiblings = [&node, modelBase](std::int32_t count) {
        while (node && count-- > 0) {
            node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->siblingOffset) : nullptr;
        }
    };
    if ((state & 0x1cU) != 0) {
        advanceSiblings(frame);
    } else if ((state & 3U) != 0) {
        advanceSiblings(12);
        advanceSiblings(frame);
    }
    return node;
}

void DrawEntity(HatoEntity &entity) {
    if (!entity.renderFlag || !shenIsLocalSphereInsideViewCullVolume(entity.position, 2.0f)) {
        return;
    }
    Mt5RuntimeNode *const node = HatoFrameNode(entity);
    if (!node)
        return;

    if (entity.alpha < 1.0f)
        entity.alpha += 0.05f;
    const Matrix4x4 cameraTransform = workTransformMatrix;
    Matrix4x4::preTranslate(entity.position.x, entity.position.y, entity.position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(entity.yaw));
    Matrix4x4::RotateAroundX_Wrapper(entity.pitch);

    const std::int32_t siblingOffset = node->siblingOffset;
    node->siblingOffset = 0;
    d3t::MT5_PushAndReadNodeTree(node, nullptr, entity.alpha);
    workTransformMatrix = cameraTransform;

    SubmitCharacterShadow(nullptr, node, node, entity.position,
        Vector3i{static_cast<std::int16_t>(entity.pitch), entity.yaw, static_cast<std::int16_t>(entity.roll)}, Vector3{1.0f, 1.0f, 1.0f}, 0.0f, -1.0f);
    node->siblingOffset = siblingOffset;
}

void ReleaseModels() {
    for (PackModel *&model : g_hatoModelOwners) {
        if (model)
            FreePVR(model);
        model = nullptr;
    }
}

} // namespace

bool HATO_Init() {
    auto *const data = static_cast<HatoTaskData *>(HLib_AllocHeapMemBlock(kHatoAllocationSize, kHatoToken));
    TempHATO = data;

    const char *const sceneRoot = g_actNumber == 2 ? "scene/02/MFSY" : "scene/03/MFSY";
    constexpr std::array<const char *, 4> modelNames{"Z4H02L1P.MT5", "Z4H02L2P.MT5", "Z4H02L3P.MT5", "Z4H02L5P.MT5"};
    constexpr std::array<std::int32_t, 4> modelTokens{static_cast<std::int32_t>(SHEN_NAME(HATG)), static_cast<std::int32_t>(SHEN_NAME(HATB)),
        static_cast<std::int32_t>(SHEN_NAME(HATM)), static_cast<std::int32_t>(SHEN_NAME(HATW))};
    std::array<void *, 4> modelFiles{};
    for (std::size_t index = 0; index < modelNames.size(); ++index)
        modelFiles[index] = ReadFile(sceneRoot, modelNames[index], modelTokens[index]);
    for (std::size_t index = 0; index < modelNames.size(); ++index) {
        PackModel *const model = MT5_InitRawModelAndTextures(modelFiles[index], modelNames[index], nullptr);
        g_hatoModelOwners[index] = model;
        data->models[index] = model->model;
    }
    g_hato_ticks = 0;
    data->entityCount = 0;
    data->task = nullptr;
    data->center = {};
    data->maxDistance = 5.0f;
    return true;
}

bool HATO_Setup(const Vector3 &center, float maxDistance) {
    if (!TempHATO)
        return false;
    TempHATO->center = center;
    TempHATO->maxDistance = maxDistance;
    g_hato_ticks = 0;
    return true;
}

std::int32_t HATO_Enqueue(std::int32_t radius, std::int32_t totalNum) {
    HatoTaskData *const data = TempHATO;
    if (!data || totalNum == 0)
        return -1;

    const std::int32_t previousCount = data->entityCount;
    std::int32_t index = 0;
    std::uint32_t seed = g_EnvPrngSeed;
    while (index < totalNum && index < 20) {
        HatoEntity &entity = data->entities[static_cast<std::size_t>(index)];
        const std::uint32_t random4 = StepPrng(seed);
        const std::uint32_t random4Next = StepPrng(random4);
        const std::uint32_t random3 = StepPrng(random4Next);
        entity.position.y = 0.11f;
        entity.position.x = static_cast<float>(static_cast<double>(Random15(random4) * data->maxDistance) * 1.4) + data->center.x - data->maxDistance;
        entity.velocity.y = 0.0f;
        entity.position.z = static_cast<float>(static_cast<double>(Random15(random4Next) * data->maxDistance) * 1.4) + data->center.z - data->maxDistance;
        const float speed = static_cast<float>(static_cast<double>(Random15(random3)) * 0.015);
        entity.velocity.x = speed;
        entity.velocity.z = std::sqrt(static_cast<float>(0.000225 - static_cast<double>(speed * speed)));
        const std::uint32_t random2 = StepPrng(random3);
        if ((random2 & 0x10000U) != 0)
            entity.velocity.x = static_cast<float>(static_cast<double>(speed) * -1.0);
        const std::uint32_t random1 = StepPrng(random2);
        if ((random1 & 0x10000U) != 0)
            entity.velocity.z = static_cast<float>(static_cast<double>(entity.velocity.z) * -1.0);
        entity.pitch = 0;
        entity.roll = 0;
        entity.animationFrame = 12.0f;
        entity.state = 1;
        entity.avoidanceTimer = 0;
        entity.turnTimer = 0;
        entity.flightAngle = 0.0f;
        entity.renderFlag = 0;
        entity.alpha = 0.0f;
        const std::uint32_t angleSeed = StepPrng(random1);
        seed = StepPrng(angleSeed);
        g_EnvPrngSeed = seed;
        const float degrees = static_cast<float>(static_cast<double>(Random15(angleSeed)) * 360.0);
        const float scaled = static_cast<float>(static_cast<double>(degrees) * 8192.0);
        entity.yaw = static_cast<std::int16_t>(static_cast<std::int32_t>(static_cast<float>(static_cast<double>(scaled) / 45.0)));
        const auto variant = static_cast<std::uint8_t>(static_cast<std::int32_t>(Random15(seed) * static_cast<float>(radius)));
        entity.modelVariant = static_cast<std::uint16_t>((entity.modelVariant & 0xff00U) | variant);
        ++index;
    }

    if (!data->task) {
        data->task = HLib_EnqueueTaskWithoutParameter(HATO_Callback, 12, 0, kHatoToken);
    }
    data->entityCount = static_cast<std::uint8_t>(index);
    g_bHatoEnabled = 1;
    return index - static_cast<std::int8_t>(previousCount);
}

void HATO_Callback(HLib::TaskData *) {
    HatoTaskData *const data = TempHATO;
    MapControlData *const control = CurrentMapControl();
    if (!data || !g_bHatoEnabled || !g_bShouldDraw || !control || (control->mapRuntimeFlags & 1U) == 0) {
        return;
    }

    if (g_hato_ticks > 0)
        --g_hato_ticks;
    const std::int32_t count = static_cast<std::int8_t>(data->entityCount);
    for (std::int32_t index = 0; index < count; ++index) {
        HatoEntity &entity = data->entities[static_cast<std::size_t>(index)];
        UpdateEntity(entity);
        const std::uint32_t state = entity.state;
        if ((state & 3U) != 0) {
            entity.animationFrame += 0.3f;
            if (entity.animationFrame >= 7.0f)
                entity.animationFrame -= 7.0f;
        } else if ((state & 2U) != 0) {
            entity.animationFrame += 1.0f;
            if (entity.animationFrame >= 7.0f)
                entity.animationFrame -= 7.0f;
        } else if ((state & 4U) != 0) {
            entity.animationFrame += 2.0f;
            if (entity.animationFrame >= 12.0f)
                entity.animationFrame -= 12.0f;
        }
        entity.state = state | kUpdatePending;
    }
    HATO_FinishUpdate();
}

void HATO_FinishUpdate() {
    HatoTaskData *const data = TempHATO;
    if (!data || g_updateRenderFrameMode == 0)
        return;

    const std::int32_t count = static_cast<std::int8_t>(data->entityCount);
    for (std::int32_t index = 0; index < count; ++index) {
        HatoEntity &entity = data->entities[static_cast<std::size_t>(index)];
        DrawEntity(entity);
        entity.state &= ~kUpdatePending;
    }
}

void HATO_Stop() {
    if (!TempHATO)
        return;
    g_bHatoEnabled = 0;
    TempHATO->entityCount = 0;
}

void HATO_Cleanup() {
    HatoTaskData *const data = TempHATO;
    if (data && data->task) {
        HLib_DestroyTask(data->task);
        data->task = nullptr;
    }
    g_bHatoEnabled = 0;
    g_hato_ticks = 0;
    ReleaseModels();
    if (data) {
        data->entityCount = 0;
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
    }
    TempHATO = nullptr;
}

HatoTaskData *HATO_GetTaskData() noexcept {
    return TempHATO;
}

bool HATO_IsEnabled() noexcept {
    return g_bHatoEnabled != 0;
}

void Script_HATO_Funcs(ScenarioContext *context, void *rawParams) {
    const auto &params = *static_cast<const HatoScriptParameters *>(rawParams);
    switch (params.mode) {
    case 0:
        context->stack->accumulator = HATO_Init() ? 0xffffffffU : 0U;
        break;
    case 1: {
        if (!TempHATO) {
            context->stack->accumulator = 0;
            break;
        }
        const auto *const center = params.radius ? reinterpret_cast<const Vector3 *>(g_pCurrentTaskData + params.radius) : nullptr;
        const float maxDistance = std::bit_cast<float>(static_cast<std::uint32_t>(params.totalNum));
        context->stack->accumulator = HATO_Setup(*center, maxDistance) ? 0xffffffffU : 0U;
        break;
    }
    case 2:
        context->stack->accumulator = static_cast<std::uint32_t>(HATO_Enqueue(params.radius, params.totalNum));
        break;
    case 3:
        HATO_Stop();
        break;
    case 4:
        if (HatoTaskData *const data = TempHATO) {
            data->entityCount = 0;
            HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(data), 0);
            g_bHatoEnabled = 0;
        }
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
