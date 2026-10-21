#include "s1/task/cuff.hpp"
#include "s1/file_loader.hpp"

#include "s1/matrix_stack.hpp"
#include "s1/motion.hpp"
#include "s1/mt5_render.hpp"
#include "s1/mt5_types.hpp"
#include "s1/task/char.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/scen.hpp"

#include <bit>
#include <array>
#include <cmath>
#include <cstring>
#include <iterator>

namespace shenmue::s1 {
namespace {

constexpr std::array<std::uint8_t, 12> kCuffNodeIds{0, 3, 6, 11, 17, 21, 26, 33, 38, 42, 48, 0};
constexpr std::int32_t kCuffSegmentCount = 10;
constexpr std::uint32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr std::uint32_t kCuffTaskToken = SHEN_NAME(CUFF);



struct CuffOwnerRuntime {
    PackModel *modelOwner;
    Mt5RuntimeNode *detachedNode;
    std::int32_t *detachedLink;
    std::int32_t detachedLinkValue;
    Vector3 detachedPosition;
};

CuffOwnerRuntime g_cuff{};
HLib::Task *g_CUFF_Task = nullptr;
std::int32_t g_cuffLastRelativeAngleFixed16 = 0;
std::int32_t g_intermediate_angle_val = 0;
float g_scale_factor = 0.0f;
float g_adjusted_sine = 0.0f;
float g_tmp_accumPosAdj = 0.0f;
Vector3 g_tmp_currTrackedPos{};
Vector3 g_tmp_pos_0{};
CuffTaskData *TaskPayload();
MotionNodeRuntime *FindRuntimeNode(MotionInformationWork &motion, std::int32_t selector);
const MotionNodeRuntime *FindRuntimeNode(const MotionInformationWork &motion, std::int32_t selector);

std::int32_t FixedAtan2(float value, float direction) {
    const auto roundedRatio = [](float numerator, float denominator) {
        return static_cast<std::int32_t>(std::atan(numerator / denominator) * 65536.0f / 6.2831855f + 0.5f);
    };
    if (direction < value) {
        if (direction < -value) {
            if (direction == 0.0f)
                return 0x8000;
            return static_cast<std::uint16_t>(roundedRatio(value, direction) + 0x8000);
        }
        if (value == 0.0f)
            return 0x4000;
        return static_cast<std::uint16_t>(0x4000 - roundedRatio(direction, value));
    }
    if (direction < -value) {
        if (value == 0.0f)
            return 0xc000;
        return static_cast<std::uint16_t>(0xc000 - roundedRatio(direction, value));
    }
    if (direction == 0.0f)
        return 0;
    return static_cast<std::uint16_t>(roundedRatio(value, direction));
}

void ComputeAngleScalingFactors(float distance) {
    float scale = 0.0f;
    std::int32_t adjustedAngle = 10013;
    if (static_cast<double>(distance) >= 0.11) {
        const float sine = sin_fixed16(6371);
        g_intermediate_angle_val = 12743;
        scale = distance / (sine * 2.0f);
    } else {
        const float root = std::sqrt(static_cast<float>(24.0 - static_cast<double>(distance) * 218.1818181818182));
        scale = static_cast<float>(0.11 / static_cast<double>(root));
        g_intermediate_angle_val = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<double>(root) * 10430.378350470464));
        const std::uint16_t half = static_cast<std::uint16_t>(g_intermediate_angle_val >> 1);
        const std::int32_t folded = half <= 0x8000U ? static_cast<std::int32_t>(half) : -static_cast<std::int32_t>(static_cast<std::int16_t>(half));
        adjustedAngle = 0x4000 - folded;
    }
    g_scale_factor = scale;
    g_adjusted_sine = -(sin_fixed16(static_cast<std::int16_t>(adjustedAngle)) * scale);
}

void BuildCuffSegmentLookAtMatrix(const Vector3 &startPoint, const Vector3 &endPoint, Matrix4x4 &outMatrix) {
    float dirX = startPoint.x - endPoint.x;
    const float midX = endPoint.x * 0.5f + startPoint.x * 0.5f;
    float dirY = startPoint.y - endPoint.y;
    const float midY = endPoint.y * 0.5f + startPoint.y * 0.5f;
    float dirZ = startPoint.z - endPoint.z;
    const float midZ = endPoint.z * 0.5f + startPoint.z * 0.5f;

    float axisX = (midZ - g_tmp_currTrackedPos.z) * dirY - dirZ * (midY - g_tmp_currTrackedPos.y);
    float axisY = dirZ * (midX - g_tmp_currTrackedPos.x) - (midZ - g_tmp_currTrackedPos.z) * dirX;
    float axisZ = (midY - g_tmp_currTrackedPos.y) * dirX - dirY * (midX - g_tmp_currTrackedPos.x);

    float orthoX = axisY * dirZ - axisZ * dirY;
    float orthoY = axisZ * dirX - axisX * dirZ;
    float orthoZ = axisX * dirY - axisY * dirX;

    const float directionLength = std::sqrt((dirY * dirY + dirX * dirX) + dirZ * dirZ);
    ComputeAngleScalingFactors(directionLength);

    const float normalizedDirectionLength = std::sqrt((dirY * dirY + dirX * dirX) + dirZ * dirZ);
    if (normalizedDirectionLength > 0.0f) {
        const float inverse = 1.0f / normalizedDirectionLength;
        dirX *= inverse;
        dirY *= inverse;
        dirZ *= inverse;
    }

    const float orthoLength = std::sqrt((orthoY * orthoY + orthoX * orthoX) + orthoZ * orthoZ);
    if (orthoLength > 0.0f) {
        const float inverse = 1.0f / orthoLength;
        orthoX *= inverse;
        orthoY *= inverse;
        orthoZ *= inverse;
    }

    const float axisLength = std::sqrt((axisY * axisY + axisX * axisX) + axisZ * axisZ);
    if (axisLength > 0.0f) {
        const float inverse = 1.0f / axisLength;
        axisX *= inverse;
        axisY *= inverse;
        axisZ *= inverse;
    }

    g_tmp_accumPosAdj += (midY - g_tmp_currTrackedPos.y + 100.0f) * axisY + (midX - g_tmp_currTrackedPos.x) * axisX + (midZ - g_tmp_currTrackedPos.z) * axisZ;
    g_tmp_pos_0 = {midX, midY, midZ};
    g_tmp_currTrackedPos.x += g_tmp_accumPosAdj * axisX;
    g_tmp_currTrackedPos.y += g_tmp_accumPosAdj * axisY;
    g_tmp_currTrackedPos.z += g_tmp_accumPosAdj * axisZ;

    outMatrix.rows[0] = {dirX, dirY, dirZ, 0.0f};
    outMatrix.rows[1] = {orthoX, orthoY, orthoZ, 0.0f};
    outMatrix.rows[2] = {axisX, axisY, axisZ, 0.0f};
    outMatrix.rows[3] = {g_tmp_pos_0.x, g_tmp_pos_0.y, g_tmp_pos_0.z, 1.0f};
}

void TransformPointByWorkMatrix(const Vector3 &input, Vector3 &output) {
    const Vector3 point = input;
    const Matrix4x4 &matrix = workTransformMatrix;
    output.x = (point.y * matrix.rows[1].x + point.x * matrix.rows[0].x + point.z * matrix.rows[2].x) + matrix.rows[3].x;
    output.z = (point.y * matrix.rows[1].z + point.x * matrix.rows[0].z + point.z * matrix.rows[2].z) + matrix.rows[3].z;
    output.y = (point.y * matrix.rows[1].y + point.x * matrix.rows[0].y + point.z * matrix.rows[2].y) + matrix.rows[3].y;
}

const Matrix4x4 *NodeTransform(const MotionInformationWork &motion, std::int32_t selector) {
    const MotionNodeRuntime *const node = FindRuntimeNode(motion, selector);
    return node && node->transform ? node->transform : nullptr;
}

Vector3 NodePosition(const MotionInformationWork &motion, std::int32_t selector) {
    const Matrix4x4 *const transform = NodeTransform(motion, selector);
    return transform ? Vector3{transform->rows[3].x, transform->rows[3].y, transform->rows[3].z} : Vector3{};
}

void BuildCuffEndpoint(CuffTaskData &data, const Matrix4x4 *wrist, const Vector3 &oppositePosition, Vector3 &endpoint) {
    matrix_stack::Matrix4x4_Push();
    const Matrix4x4 *const wristTransform = wrist ? wrist : g_matrixStack.top.stack;
    const Matrix4x4 savedWrist = *wristTransform;
    workTransformMatrix = savedWrist;
    Matrix4x4::InvertInPlace(nullptr);

    const float localY = oppositePosition.y * workTransformMatrix.rows[1].y + oppositePosition.x * workTransformMatrix.rows[0].y;
    const float localZ = oppositePosition.y * workTransformMatrix.rows[1].z + oppositePosition.x * workTransformMatrix.rows[0].z;
    g_cuffLastRelativeAngleFixed16 = FixedAtan2(localY + oppositePosition.z * workTransformMatrix.rows[2].y + workTransformMatrix.rows[3].y,
        localZ + oppositePosition.z * workTransformMatrix.rows[2].z + workTransformMatrix.rows[3].z);

    workTransformMatrix = savedWrist;
    const float sineZero = sin_fixed16(0);
    const float sineQuarter = sin_fixed16(0x4000);
    Matrix4x4::RotateAroundY(sineQuarter, sineZero);

    const std::uint16_t relativeAngle = static_cast<std::uint16_t>(g_cuffLastRelativeAngleFixed16);
    const std::uint16_t delta = static_cast<std::uint16_t>(0x4000U - relativeAngle);
    const std::int32_t folded = delta <= 0x8000U ? static_cast<std::int32_t>(static_cast<std::int16_t>(delta)) :
                                                   static_cast<std::int32_t>(static_cast<std::int16_t>(static_cast<std::int32_t>(relativeAngle) - 0x4000));
    Matrix4x4::RotateAroundZ(
        sin_fixed16(static_cast<std::int16_t>(0x4000 - static_cast<std::int32_t>(relativeAngle))), sin_fixed16(static_cast<std::int16_t>(0x4000 - folded)));
    TransformPointByWorkMatrix(data.chainSegmentOffset, endpoint);
    const Matrix4x4 endpointOrientation = workTransformMatrix;

    Matrix4x4::Pop();
    matrix_stack::Matrix4x4_Push();
    Matrix4x4::Multiply(&endpointOrientation);
    d3t::MT5_SubmitObjectNode(data.rootModel, 1.0f);
    Matrix4x4::Pop();
}

void CuffFunc(CuffTaskData *data) {
    if (!data || !data->rootModel)
        return;

    MotionManagerWork *const sourceManager = MOTM_GetWork(data->source.character);
    MotionManagerWork *const targetManager = MOTM_GetWork(data->target.character);
    const Matrix4x4 *const sourceWrist = sourceManager ? NodeTransform(sourceManager->motion, data->source.nodeSelector) : nullptr;
    const Matrix4x4 *const targetWrist = targetManager ? NodeTransform(targetManager->motion, data->target.nodeSelector) : nullptr;
    const Vector3 sourcePosition = sourceManager ? NodePosition(sourceManager->motion, data->source.nodeSelector) : Vector3{};
    const Vector3 targetPosition = targetManager ? NodePosition(targetManager->motion, data->target.nodeSelector) : Vector3{};

    Vector3 startPoint{};
    Vector3 endPoint{};
    BuildCuffEndpoint(*data, sourceWrist, targetPosition, startPoint);
    BuildCuffEndpoint(*data, targetWrist, sourcePosition, endPoint);

    Matrix4x4 segmentLookAt{};
    BuildCuffSegmentLookAtMatrix(startPoint, endPoint, segmentLookAt);

    matrix_stack::Matrix4x4_Push();
    workTransformMatrix = segmentLookAt;
    Matrix4x4::preTranslate(0.0f, g_adjusted_sine, 0.0f);
    const Matrix4x4 segmentMatrix = workTransformMatrix;

    workTransformMatrix = transformMatrix_prePass;
    Matrix4x4::preTranslate(0.0f, g_scale_factor, 0.0f);
    Matrix4x4::RotateAroundZ(sin_fixed16(0x4000), sin_fixed16(0));
    const Matrix4x4 inMatrix = workTransformMatrix;
    Matrix4x4::Pop();

    if (data->segmentCount < 0)
        return;
    std::int32_t segmentIndex = 0;
    std::int32_t cumulativeFixedAngle = 0xB4000;
    do {
        matrix_stack::Matrix4x4_Push();
        Matrix4x4::Multiply(&segmentMatrix);

        const std::int32_t angle = segmentIndex * g_intermediate_angle_val / data->segmentCount - g_intermediate_angle_val / 2;
        const std::int32_t foldedAngle =
            static_cast<std::uint16_t>(angle) <= 0x8000U ? angle : g_intermediate_angle_val / 2 - segmentIndex * g_intermediate_angle_val / data->segmentCount;
        Matrix4x4::RotateAroundZ(sin_fixed16(static_cast<std::int16_t>(angle)), sin_fixed16(static_cast<std::int16_t>(0x4000 - foldedAngle)));
        Matrix4x4::Multiply(&inMatrix);

        const float angleDegrees = static_cast<float>(cumulativeFixedAngle) / 45.0f;
        const std::int32_t yFixedTruncated = static_cast<std::int32_t>(angleDegrees);
        const std::uint16_t yFixed = static_cast<std::uint16_t>(yFixedTruncated);

        const std::int32_t yAngle = yFixed <= 0x8000U ? yFixedTruncated : -static_cast<std::int32_t>(static_cast<std::int16_t>(yFixed));
        Matrix4x4::RotateAroundY(sin_fixed16(static_cast<std::int16_t>(yFixed)), sin_fixed16(static_cast<std::int16_t>(0x4000 - yAngle)));
        d3t::MT5_SubmitObjectNode(data->chainSegmentModel, 1.0f);
        Matrix4x4::Pop();

        ++segmentIndex;
        cumulativeFixedAngle += 0xB4000;
    } while (segmentIndex <= data->segmentCount);
}

CuffTaskData *TaskPayload() {
    return g_CUFF_Task && g_CUFF_Task->task_data_ptr ? reinterpret_cast<CuffTaskData *>(g_CUFF_Task->task_data_ptr) : nullptr;
}

MotionNodeRuntime *FindRuntimeNode(MotionInformationWork &motion, std::int32_t selector) {
    auto *node = static_cast<MotionNodeRuntime *>(motion.activeNodes);
    const std::uint8_t nodeId = kCuffNodeIds[static_cast<std::size_t>(selector)];
    while (node && node->nodeId != 0xff) {
        if (node->nodeId == nodeId)
            return node;
        ++node;
    }
    return nullptr;
}

const MotionNodeRuntime *FindRuntimeNode(const MotionInformationWork &motion, std::int32_t selector) {
    return FindRuntimeNode(const_cast<MotionInformationWork &>(motion), selector);
}

void CUFF_MobjConstraintCallback(MotionInformationWork *motion, CuffMotionCommand *command) {
    MotionManagerWork *const otherManager = MOTM_GetWork(command->otherCommand->character);

    MotionNodeRuntime *const source = FindRuntimeNode(*motion, command->nodeSelector);
    const MotionNodeRuntime *const target = FindRuntimeNode(otherManager->motion, command->otherCommand->nodeSelector);
    Vector3 sourcePosition;
    if (source) {
        sourcePosition = {source->effectorPosition.x, source->effectorPosition.y, source->effectorPosition.z};
    }
    Vector3 targetPosition;
    if (target) {
        const Matrix4x4 *const transform = target->transform;
        targetPosition = {transform->rows[3].x, transform->rows[3].y, transform->rows[3].z};
    }

    const Vector3 delta{sourcePosition.x - targetPosition.x, sourcePosition.y - targetPosition.y, sourcePosition.z - targetPosition.z};
    const float distanceSquared = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
    const float distance = std::sqrt(distanceSquared);
    if (distance <= command->maxDistance) {
        motion->worldTargets[command->nodeSelector] = nullptr;
        return;
    }

    const float scale = command->maxDistance / distance;
    command->constrainedTarget = {targetPosition.x + delta.x * scale, targetPosition.y + delta.y * scale, targetPosition.z + delta.z * scale};
    if (source) {
        source->effectorPosition.x = command->constrainedTarget.x;
        source->effectorPosition.y = command->constrainedTarget.y;
        source->effectorPosition.z = command->constrainedTarget.z;
    }
    motion->worldTargets[command->nodeSelector] = &command->constrainedTarget;
}

Mt5RuntimeNode *ResolveNodeOffset(std::byte *base, std::int32_t offset) {
    return base && offset ? reinterpret_cast<Mt5RuntimeNode *>(base + offset) : nullptr;
}

bool DetachChainSegment(PackModel &owner, CuffTaskData &data) {
    auto *current = static_cast<Mt5RuntimeNode *>(owner.model);
    std::byte *const base = current ? MT5_ModelBaseForNode(current) : nullptr;
    if (!current || !base)
        return false;

    Mt5RuntimeNode *previous = current;
    for (;;) {
        Mt5RuntimeNode *next = ResolveNodeOffset(base, current->siblingOffset);
        if (!next)
            next = ResolveNodeOffset(base, current->childOffset);
        if (!next)
            break;
        previous = current;
        current = next;
    }
    if (current == static_cast<Mt5RuntimeNode *>(owner.model))
        return false;

    std::int32_t *const link = ResolveNodeOffset(base, previous->childOffset) == current ? &previous->childOffset : &previous->siblingOffset;
    if (!*link)
        return false;

    g_cuff.detachedNode = current;
    g_cuff.detachedLink = link;
    g_cuff.detachedLinkValue = *link;
    g_cuff.detachedPosition = current->position;
    *link = 0;

    data.rootModel = owner.model;
    data.chainSegmentModel = current;
    data.chainSegmentOffset = {current->position.x, current->position.y + 0.035f, current->position.z + 0.005f};
    current->position = {};
    return true;
}

void RestoreDetachedModel() {
    if (g_cuff.detachedNode)
        g_cuff.detachedNode->position = g_cuff.detachedPosition;
    if (g_cuff.detachedLink)
        *g_cuff.detachedLink = g_cuff.detachedLinkValue;
    g_cuff.detachedNode = nullptr;
    g_cuff.detachedLink = nullptr;
    g_cuff.detachedLinkValue = 0;
    g_cuff.detachedPosition = {};
}

} // namespace

void CUFF_ClearInstalledCallbacks() {
    if (!g_CUFF_Task)
        return;
    CuffTaskData *const data = reinterpret_cast<CuffTaskData *>(g_CUFF_Task->task_data_ptr);
    if (data->active != 1)
        return;

    data->active = 0;
    MotionManagerWork *const sourceManager = MOTM_GetWork(data->source.character);
    MotionManagerWork *const targetManager = MOTM_GetWork(data->target.character);
    sourceManager->motion.mobjCallbackUser = nullptr;
    targetManager->motion.mobjCallbackUser = nullptr;
    sourceManager->motion.mobjCallback = nullptr;
    targetManager->motion.mobjCallback = nullptr;
}

std::uint32_t CUFF_ComputeBoneMidpointAndInitCommand(
    Character *source, std::int32_t sourceNodeSelector, Character *target, std::int32_t targetNodeSelector, float maximumDistance) {
    if (!g_CUFF_Task)
        return 0xffffffffU;
    CuffTaskData *const data = reinterpret_cast<CuffTaskData *>(g_CUFF_Task->task_data_ptr);
    data->active = 1;
    data->source.character = source;
    data->source.otherCommand = &data->target;
    data->source.nodeSelector = sourceNodeSelector;
    data->target.otherCommand = &data->source;
    data->target.character = target;
    data->target.nodeSelector = targetNodeSelector;
    data->segmentCount = kCuffSegmentCount;
    data->source.maxDistance = maximumDistance;
    data->target.maxDistance = maximumDistance;
    data->source.mode = 4;
    data->target.mode = 8;
    data->source.model = data->rootModel;
    data->target.model = data->rootModel;

    MotionManagerWork *const sourceManager = MOTM_GetWork(source);
    MotionManagerWork *const targetManager = MOTM_GetWork(target);

    sourceManager->motion.mobjCallbackUser = &data->source;
    targetManager->motion.mobjCallbackUser = &data->target;
    sourceManager->motion.mobjCallback = reinterpret_cast<void *>(&CUFF_MobjConstraintCallback);
    targetManager->motion.mobjCallback = reinterpret_cast<void *>(&CUFF_MobjConstraintCallback);

    const Vector3 sourcePosition = NodePosition(sourceManager->motion, sourceNodeSelector);
    const Vector3 targetPosition = NodePosition(targetManager->motion, targetNodeSelector);
    g_tmp_pos_0 = {targetPosition.x * 0.5f + sourcePosition.x * 0.5f, targetPosition.y * 0.5f + sourcePosition.y * 0.5f,
        targetPosition.z * 0.5f + sourcePosition.z * 0.5f};
    g_tmp_currTrackedPos = g_tmp_pos_0;
    g_tmp_currTrackedPos.y -= 2.0f;
    g_tmp_accumPosAdj = 0.0f;
    return 0;
}

void CUFF_Callback(HLib::TaskData *taskData) {
    auto *const data = reinterpret_cast<CuffTaskData *>(taskData);
    if (!data || data->active != 1 || !data->rootModel)
        return;
    CuffFunc(data);
}

HLib::Task *CUFF_GetTask() {
    return g_CUFF_Task && g_CUFF_Task->mainTaskCallbackFnPtr == CUFF_Callback ? g_CUFF_Task : nullptr;
}

CuffTaskData *CUFF_GetTaskData() {
    return TaskPayload();
}

HLib::Task *CUFF_Initialize() {
    void *const modelFile = ReadFile("model/object", "TEJHI03G.MT3", static_cast<std::int32_t>(SHEN_NAME(TEJO)));
    PackModel *const model = MT5_InitRawModelAndTextures(modelFile, "TEJHI03G.MT3", nullptr);
    void *const modelRoot = model->model;

    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(CUFF_Callback, 12, 0, static_cast<std::int32_t>(kCuffTaskToken));
    CuffTaskData *data = nullptr;
    if (task) {
        if (!task->task_data_ptr) {
            data = static_cast<CuffTaskData *>(HLib_AllocHeapMemBlock(sizeof(CuffTaskData), kTaskDataToken));
            if (data)
                HLib_EnableTokenCheck(data);
            task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
        }
        if (!data) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
            task = nullptr;
        }
    }

    g_CUFF_Task = task;
    g_cuff.modelOwner = model;
    std::memset(data, 0, sizeof(*data));
    data->rootModel = modelRoot;
    DetachChainSegment(*model, *data);
    return task;
}

void CUFF_Shutdown() {
    CUFF_ClearInstalledCallbacks();
    RestoreDetachedModel();

    HLib::Task *const task = g_CUFF_Task;
    PackModel *const model = g_cuff.modelOwner;
    g_CUFF_Task = nullptr;
    g_cuff.modelOwner = nullptr;
    if (task)
        HLib_DestroyTask(task);
    if (model)
        FreePVR(model);
}

void Script_ComputeBoneMidpoint(ScenarioContext *context, void *rawParams) {
    const auto &params = *static_cast<const CuffScriptParameters *>(rawParams);
    Character *const source = CHID_Find(context->task->task, params.sourceCharacterId);
    Character *const target = CHID_Find(context->task->task, params.targetCharacterId);
    context->stack->accumulator = source && target ?
        CUFF_ComputeBoneMidpointAndInitCommand(source, params.sourceNodeSelector, target, params.targetNodeSelector, params.maximumDistance) :
        0xffffffffU;
}

void Script_cuffUtils(ScenarioContext *, void *) {
    CUFF_ClearInstalledCallbacks();
}

} // namespace shenmue::s1
