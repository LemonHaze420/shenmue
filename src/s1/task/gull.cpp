#include "s1/task/gull.hpp"

#include "s1/boot.hpp"
#include "s1/file_loader.hpp"
#include "s1/legacy_renderer.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/ctrl.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"
#include "s1/task/came.hpp"
#include "s1/task/drwp.hpp"
#include "common/types.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>

namespace shenmue::s1 {

extern std::int32_t g_actNumber;




HLib::Task *g_pGULLTaskPtr = nullptr;
GullSequencePayload *ActiveGULLData = nullptr;
GullSplinePayload *g_gullSplines = nullptr;
std::array<PackModel *, kGullModelCount> g_birdTypes{};
std::array<void *, kGullModelCount> g_birdTypeHmdls{};
std::array<Mt5RuntimeNode *, kGullModelCount * kGullFrameNodeCount> g_gullFrameNodes{};
std::uint8_t CurrentGULL = 0;
std::uint8_t CurrentMaxGULLs = 0;
std::uint8_t curr_GULL_index = 0;
std::uint8_t GULL_max = 0;

namespace {

constexpr std::int32_t kGullPayloadToken = static_cast<std::int32_t>(SHEN_NAME(GULP));
constexpr std::int32_t kGullSplineToken = static_cast<std::int32_t>(SHEN_NAME(GULS));
constexpr std::int32_t kGullTaskToken = static_cast<std::int32_t>(SHEN_NAME(GULL));
constexpr std::int32_t kMaximumSplineKeys = 512;
constexpr float kGullTimeStep = 0.1f;

bool IsFinite(const Vector3 &value) {
    return std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
}

void FreeTaskAllocation(void *allocation) {
    if (allocation) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(allocation), 0);
    }
}

void ReleaseSplineStorage(GullSplinePayload &spline) {
    FreeTaskAllocation(spline.keyTimes[0]);
    for (float *slopes : spline.keySlopes)
        FreeTaskAllocation(slopes);
}

void ReleaseRuntime(bool destroyTask) {
    if (destroyTask && g_pGULLTaskPtr)
        HLib_DestroyTask(g_pGULLTaskPtr);
    g_pGULLTaskPtr = nullptr;

    if (g_gullSplines) {
        for (std::uint32_t index = 0; index < curr_GULL_index; ++index) {
            ReleaseSplineStorage(g_gullSplines[index]);
        }
    }
    FreeTaskAllocation(ActiveGULLData);
    FreeTaskAllocation(g_gullSplines);
    ActiveGULLData = nullptr;
    g_gullSplines = nullptr;
    CurrentGULL = 0;
    curr_GULL_index = 0;
    CurrentMaxGULLs = 0;
    GULL_max = 0;
    for (PackModel *model : g_birdTypes) {
        if (model)
            FreePVR(model);
    }
    for (void *model : g_birdTypeHmdls) {
        if (model)
            HMDL_FreeAssignedModel(model);
    }
}

void AllocateSplineStorage(GullSplinePayload &spline, std::int32_t keyCount) {
    const std::int64_t timeBytes = static_cast<std::int64_t>(keyCount) * sizeof(float);
    const std::int64_t slopeBytes = timeBytes * 2;
    spline.keyTimes[0] = static_cast<float *>(HLib_AllocHeapMemBlock(timeBytes, kGullPayloadToken));
    for (std::size_t axis = 0; axis < 3; ++axis) {
        spline.endpointSlopes[axis] = nullptr;
        spline.keySlopes[axis] = static_cast<float *>(HLib_AllocHeapMemBlock(slopeBytes, kGullPayloadToken));
    }
}

float NormalizeSplineTime(const float value) {
    return static_cast<float>(static_cast<std::int32_t>(value * 60.0f + 0.0083333338f)) / 60.0f;
}

const float *AxisValues(const GullSplinePayload &spline, std::size_t axis) {
    if (axis == 0)
        return spline.keyTimes[1];
    return spline.keyValues[axis - 1];
}

std::int32_t InterpolateAxis(
    const float time, const std::int32_t keyCount, const float *const keyTimes, const float *const keyValues, const float *const coefficients, float &output) {
    const float normalizedTime = NormalizeSplineTime(time);
    if (NormalizeSplineTime(keyTimes[0]) > normalizedTime || keyCount < 2) {
        output = keyValues[0];
        return -1;
    }

    std::int32_t index = 1;
    while (index < keyCount && NormalizeSplineTime(keyTimes[index]) <= normalizedTime) {
        ++index;
    }
    if (index == keyCount) {
        output = keyValues[keyCount - 1];
        return 0;
    }

    const std::int32_t previousIndex = index - 1;
    const float endTime = keyTimes[index];
    const float timeToEnd = endTime - time;
    const float timeFromStart = time - keyTimes[previousIndex];
    const float span = endTime - keyTimes[previousIndex];
    const float timeToEndSquared = timeToEnd * timeToEnd;
    const float timeFromStartSquared = timeFromStart * timeFromStart;
    output = (timeToEndSquared * timeToEnd * keyValues[previousIndex] + timeFromStart * timeToEndSquared * coefficients[index * 2] +
                 timeFromStartSquared * timeToEnd * coefficients[index * 2 + 1] + timeFromStartSquared * timeFromStart * keyValues[index]) /
        (span * span * span);
    return index;
}

std::int32_t Interpolate3D(const GullSplinePayload &spline, const float time, Vector3 &output) {
    const std::int32_t result = InterpolateAxis(time, spline.keyCount[0], spline.keyTimes[0], spline.keyTimes[1], spline.keySlopes[0], output.x);
    if (result > 0) {
        InterpolateAxis(time, spline.keyCount[0], spline.keyTimes[0], spline.keyValues[0], spline.keySlopes[1], output.y);
        InterpolateAxis(time, spline.keyCount[0], spline.keyTimes[0], spline.keyValues[1], spline.keySlopes[2], output.z);
    }
    return result;
}

void PopulateFrameNodes() {
    for (std::size_t modelIndex = 0; modelIndex < g_birdTypeHmdls.size(); ++modelIndex) {
        auto *const root = static_cast<Mt5RuntimeNode *>(g_birdTypeHmdls[modelIndex]);
        std::byte *const modelBase = g_pCurrentTaskData;
        auto *node = root->childOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + root->childOffset) : nullptr;
        for (std::size_t frameIndex = 0; frameIndex < kGullFrameNodeCount; ++frameIndex) {
            g_gullFrameNodes[modelIndex * kGullFrameNodeCount + frameIndex] = node;
            node = node->siblingOffset ? reinterpret_cast<Mt5RuntimeNode *>(modelBase + node->siblingOffset) : nullptr;
        }
    }
}

Vector2 DirectionBasis(const Vector3 &from, const Vector3 &to) {
    const float dx = to.x - from.x;
    const float dy = to.y - from.y;
    const float dz = to.z - from.z;
    const float length = std::sqrt(dx * dx + dy * dy + dz * dz);
    if (std::fabs(length) < std::numeric_limits<float>::epsilon()) {
        return Vector2{1.0f, 0.0f};
    }
    return Vector2{dx / length, dz / length};
}

Vector3 TransformPointByWorkMatrix(const Vector3 &point) {
    return {point.x * workTransformMatrix.rows[0].x + point.y * workTransformMatrix.rows[1].x + point.z * workTransformMatrix.rows[2].x +
            workTransformMatrix.rows[3].x,
        point.x * workTransformMatrix.rows[0].y + point.y * workTransformMatrix.rows[1].y + point.z * workTransformMatrix.rows[2].y +
            workTransformMatrix.rows[3].y,
        point.x * workTransformMatrix.rows[0].z + point.y * workTransformMatrix.rows[1].z + point.z * workTransformMatrix.rows[2].z +
            workTransformMatrix.rows[3].z};
}

bool PassesNativeViewCull(const Vector3 &viewPosition) {
    if (!IsFinite(viewPosition))
        return false;

    constexpr float kNativeMargin = 5.0f;
    const float depthWithMargin = -viewPosition.z + kNativeMargin;
    if (depthWithMargin < 0.0f)
        return false;

    const Vector2 scale = prevScreenSpaceToWorldScale;
    const float xLimit = depthWithMargin * scale.x + kNativeMargin;
    const float yLimit = depthWithMargin * scale.y + kNativeMargin;
    return viewPosition.x <= xLimit && -xLimit < viewPosition.x && viewPosition.y <= yLimit && -yLimit < viewPosition.y;
}

void SubmitObjectTransform(Mt5RuntimeNode *node) {
    d3t::MT5_SubmitObjectNode(node, 1.0f);
}

template <typename T> T *TaskDataPointer(std::int32_t offset) {
    if (!offset)
        return nullptr;
    return reinterpret_cast<T *>(g_pCurrentTaskData + offset);
}

} // namespace

std::int32_t HermiteSpline_InterpolateFCVKey(float *const output, const float time, const std::int32_t keyCount, const float *const keyTimes,
    const float *const keyValues, const float *const keySlopes) {
    return InterpolateAxis(time, keyCount, keyTimes, keyValues, keySlopes, *output);
}

std::int32_t HermiteSpline_GetInterpolated3D(const GullSplinePayload *const spline, const float time, Vector3 *const output) {
    return Interpolate3D(*spline, time, *output);
}

void SPLI_Alloc(GullSplinePayload *const spline) {
    const std::int32_t keyCount = spline->keyCount[0];
    const float *const xValues = spline->keyTimes[1];
    const float *const yValues = spline->keyValues[0];
    const float *const zValues = spline->keyValues[1];
    float *const keyTimes = spline->keyTimes[0];

    keyTimes[0] = 0.0f;
    for (std::int32_t index = 1; index < keyCount; ++index) {
        const float dx = xValues[index] - xValues[index - 1];
        const float dy = yValues[index] - yValues[index - 1];
        const float dz = zValues[index] - zValues[index - 1];
        keyTimes[index] = keyTimes[index - 1] + std::sqrt(dx * dx + dy * dy + dz * dz);
    }

    const std::array<const float *, 3> values{xValues, yValues, zValues};
    for (std::size_t axis = 0; axis < values.size(); ++axis) {
        AllocSPLI(spline->keySlopes[axis], keyCount, keyTimes, values[axis], spline->endpointSlopes[axis]);
    }
}

std::uint32_t GULL_Enqueue(std::int32_t maxGulls, std::int32_t maxSplines) {
    if (ActiveGULLData || g_gullSplines)
        return 0;

    CurrentMaxGULLs = static_cast<std::uint8_t>(maxGulls);
    CurrentGULL = 0;
    curr_GULL_index = 0;
    GULL_max = static_cast<std::uint8_t>(maxSplines);
    ActiveGULLData =
        static_cast<GullSequencePayload *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(maxGulls) * sizeof(GullSequencePayload), kGullPayloadToken));
    g_gullSplines =
        static_cast<GullSplinePayload *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(maxSplines) * sizeof(GullSplinePayload), kGullSplineToken));

    const char *const sceneRoot = g_actNumber == 2 ? "scene/02/MFSY" : "scene/03/MFSY";
    constexpr std::array<const char *, kGullModelCount> kModelNames{"Z5E201P.MT5", "Z6E201P.MT5", "Z7E201P.MT5"};
    constexpr std::array<std::int32_t, kGullModelCount> kModelTokens{
        static_cast<std::int32_t>(SHEN_NAME(GULE)), static_cast<std::int32_t>(SHEN_NAME(GULM)), static_cast<std::int32_t>(SHEN_NAME(GULH))};
    for (std::size_t index = 0; index < kModelNames.size(); ++index) {
        void *const modelFile = ReadFile(sceneRoot, kModelNames[index], kModelTokens[index]);
        PackModel *const model = MT5_InitRawModelAndTextures(modelFile, kModelNames[index], nullptr);
        g_birdTypes[index] = model;
        void *const modelRoot = model->model;
        g_birdTypeHmdls[index] = modelRoot ? hmdlAssignFor(model) : nullptr;
    }
    PopulateFrameNodes();

    g_pGULLTaskPtr = HLib_EnqueueTaskWithoutParameter(GULL_TaskCallback, 12, 0, kGullTaskToken);
    return 0xffffffffU;
}

GullSplinePayload *GULL_Create(
    const Vector3 *position, std::int32_t keyCount, GullSplinePayload *existingSpline, const float *xValues, const float *yValues, const float *zValues) {
    const std::uint8_t sequenceIndex = CurrentGULL;
    if (sequenceIndex >= CurrentMaxGULLs)
        return nullptr;

    GullSplinePayload *spline = existingSpline;
    if (!spline) {
        const std::uint8_t splineIndex = curr_GULL_index;
        if (splineIndex >= GULL_max)
            return nullptr;
        curr_GULL_index = static_cast<std::uint8_t>(splineIndex + 1U);

        spline = &g_gullSplines[splineIndex];
        spline->keyValues[0] = const_cast<float *>(yValues);
        spline->keyValues[1] = const_cast<float *>(zValues);
        spline->keyTimes[1] = const_cast<float *>(xValues);
        spline->keyCount[0] = keyCount;
        AllocateSplineStorage(*spline, keyCount);

        SPLI_Alloc(spline);
        spline->duration = spline->keyTimes[0][keyCount - 3];
    }

    CurrentGULL = static_cast<std::uint8_t>(sequenceIndex + 1U);
    GullSequencePayload &sequence = ActiveGULLData[sequenceIndex];
    sequence.callback = GULL_SequenceCallback;
    sequence.spline = spline;
    sequence.wingFrame = 0;
    sequence.frameDelay = 3;
    sequence.flapCooldown = 8;
    sequence.flapVariant = 1;
    sequence.basePosition = *position;

    g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
    const float random = static_cast<float>(static_cast<double>((g_EnvPrngSeed >> 16U) & 0x7fffU) / 32768.0);
    sequence.time = random * (spline->duration - kGullTimeStep);
    HermiteSpline_GetInterpolated3D(spline, sequence.time, &sequence.previousPosition);
    sequence.time += kGullTimeStep;
    HermiteSpline_GetInterpolated3D(spline, sequence.time, &sequence.nextPosition);
    return spline;
}

void GULL_UpdateInternal(GullSequencePayload *sequence) {
    Matrix4x4::SetActiveTransformToPrePass();
    Matrix4x4::preTranslate(sequence->basePosition.x, sequence->basePosition.y, sequence->basePosition.z);
    sequence->nextPosition = TransformPointByWorkMatrix(sequence->nextPosition);
    ::shenmue::Matrix4x4::Pop();

    const Vector2 directionBasis = DirectionBasis(sequence->previousPosition, sequence->nextPosition);

    Matrix4x4 billboard{};
    billboard.rows[0] = {directionBasis.y, 0.0f, -directionBasis.x, 0.0f};
    billboard.rows[1] = {0.0f, 1.0f, 0.0f, 0.0f};
    billboard.rows[2] = {directionBasis.x, 0.0f, directionBasis.y, 0.0f};
    billboard.rows[3] = {0.0f, 0.0f, 0.0f, 1.0f};

    ::shenmue::matrix_stack::Matrix4x4_Push();
    Matrix4x4::preTranslate(sequence->nextPosition.x, sequence->nextPosition.y, sequence->nextPosition.z);
    Matrix4x4::Multiply(&billboard);
    const std::uint8_t previousDelay = sequence->frameDelay;
    sequence->frameDelay = static_cast<std::uint8_t>(previousDelay - 1U);
    if (previousDelay == 0) {
        const std::uint8_t previousFrame = sequence->wingFrame;
        sequence->frameDelay = 1;
        sequence->wingFrame = static_cast<std::uint8_t>(previousFrame + 1U);
        if (previousFrame >= 14U)
            sequence->wingFrame = 0;
        if (sequence->wingFrame == 8U) {
            const std::uint8_t previousCooldown = sequence->flapCooldown;
            sequence->flapCooldown = static_cast<std::uint8_t>(previousCooldown - 1U);
            if (previousCooldown == 0) {
                g_EnvPrngSeed = LcgStep(g_EnvPrngSeed);
                sequence->flapCooldown = static_cast<std::uint8_t>(((g_EnvPrngSeed >> 16U) & 0x0fU) + 1U);
                sequence->flapVariant ^= 1U;
            }
        }
    }
    const Vector3 viewPosition = TransformPointByWorkMatrix({});
    if (PassesNativeViewCull(viewPosition)) {
        const std::size_t frameIndex = 2U * kGullFrameNodeCount + (sequence->flapVariant ? static_cast<std::size_t>(sequence->wingFrame) : 16U);
        SubmitObjectTransform(g_gullFrameNodes[frameIndex]);
    }
    ::shenmue::Matrix4x4::Pop();
    sequence->previousPosition = sequence->nextPosition;
}

void GULL_SequenceCallback(GullSequencePayload *sequence) {
    GullSplinePayload &spline = *sequence->spline;
    sequence->time += kGullTimeStep;
    if (sequence->time > spline.duration)
        sequence->time = sequence->time - spline.duration + spline.keyTimes[0][2];
    HermiteSpline_GetInterpolated3D(&spline, sequence->time, &sequence->nextPosition);
    GULL_UpdateInternal(sequence);
}

void GULL_TaskCallback(HLib::TaskData *) {
    if (!g_bShouldDraw)
        return;
    const auto *const control = g_mapControlTask ? reinterpret_cast<const MapControlData *>(g_mapControlTask->task_data_ptr) : nullptr;
    if ((control->mapRuntimeFlags & 1U) == 0)
        return;

    for (std::uint32_t index = 0; index < CurrentGULL; ++index) {
        GullSequencePayload &sequence = ActiveGULLData[index];
        sequence.callback(&sequence);
    }
}

void GULL_Destroy() {
    if (g_pGULLTaskPtr)
        ReleaseRuntime(true);
}



void GULL_ClearMapState() {
    g_pGULLTaskPtr = nullptr;
    g_gullSplines = nullptr;
    ActiveGULLData = nullptr;
    curr_GULL_index = 0;
    CurrentGULL = 0;
    GULL_max = 0;
    CurrentMaxGULLs = 0;
}

void Script_GULLProgram_Utils(ScenarioContext *context, void *rawParams) {
    const auto &parameters = *static_cast<const GullScriptParameters *>(rawParams);
    switch (parameters.mode) {
    case 0:
        context->stack->accumulator = GULL_Enqueue(parameters.positionOffsetOrMaxGulls, parameters.keyCountOrMaxSplines);
        break;
    case 1: {
        GullSplinePayload *const spline = GULL_Create(TaskDataPointer<const Vector3>(parameters.positionOffsetOrMaxGulls), parameters.keyCountOrMaxSplines,
            TaskDataPointer<GullSplinePayload>(parameters.splineOffset), TaskDataPointer<const float>(parameters.xValuesOffset),
            TaskDataPointer<const float>(parameters.yValuesOffset), TaskDataPointer<const float>(parameters.zValuesOffset));
        std::uint32_t result = 0;
        if (spline) {
            const std::uintptr_t base = reinterpret_cast<std::uintptr_t>(g_pCurrentTaskData);
            const std::uintptr_t pointer = reinterpret_cast<std::uintptr_t>(spline);
            result = static_cast<std::uint32_t>(pointer - base);
        }
        context->stack->accumulator = result;
        break;
    }
    case 2:
        GULL_Destroy();
        break;
    default:
        break;
    }
}

} // namespace shenmue::s1
