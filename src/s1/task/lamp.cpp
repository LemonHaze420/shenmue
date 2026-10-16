#include "s1/task/lamp.hpp"
#include "s1/file_loader.hpp"

#include "s1/hlib.hpp"
#include "s1/lighting.hpp"
#include "s1/matrix_stack.hpp"
#include "s1/mt5_render.hpp"
#include "s1/task/char.hpp"
#include "s1/task/pack.hpp"

#include <array>
#include <bit>

namespace shenmue::s1 {
namespace {

constexpr std::uint32_t kLampLightSlot = 29;
constexpr std::uint32_t kNoPublishedLightSlot = 33;
constexpr std::int16_t kLampDrawTaskIndex = 12;
constexpr std::int32_t kTaskDataToken = SHEN_NAME(TASK);
constexpr float kGlowStep = -0.037500001f;
constexpr Vector3 kLightLocalPosition{0.0f, 0.5f, 1.2f};
constexpr Vector4 kLightColour{1.0f, 1.0f, 0.0f, 0.0f};

#pragma pack(push, 1)
struct LampDrawTaskData {
    Matrix4x4 transform;
    void *modelRoot;
    float opacity;
    float opacityStep;
};
#pragma pack(pop)

static_assert(sizeof(LampDrawTaskData) == 0x50);
static_assert(offsetof(LampDrawTaskData, modelRoot) == 0x40);
static_assert(offsetof(LampDrawTaskData, opacity) == 0x48);
static_assert(offsetof(LampDrawTaskData, opacityStep) == 0x4c);

void ApplyEntityTransform(const CharacterNode *node) {
    if (!node)
        return;
    ApplyEntityTransform(node->child);
    Matrix4x4::preTranslate(node->position.x, node->position.y, node->position.z);
    if (node->rotation.z) {
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node->rotation.z));
    }
    if (node->rotation.y) {
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node->rotation.y));
    }
    if (node->rotation.x) {
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node->rotation.x));
    }
}

void BuildLampFrame(Character &character, Matrix4x4 &drawTransform, std::uint32_t lightIndex) {
    const Matrix4x4 callerTransform = workTransformMatrix;
    Matrix4x4::SetActiveTransformToPrePass();
    ApplyEntityTransform(&character.entity.node);
    const Matrix4x4 entityTransform = workTransformMatrix;

    if (lightIndex != kNoPublishedLightSlot) {
        Matrix4x4::preTranslate(kLightLocalPosition.x, kLightLocalPosition.y, kLightLocalPosition.z);
        const Vector3 lightPosition{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        SetLightDirectionVectorByIndex(lightIndex, &lightPosition);
        lightSetSlotEnabled(lightIndex, 1);
    }

    workTransformMatrix = callerTransform;
    Matrix4x4::Multiply(&entityTransform);
    drawTransform = workTransformMatrix;
    Matrix4x4::Pop();
}

void PushDrawTransform(const Matrix4x4 &transform) {
    *g_matrixStack.top.stack = workTransformMatrix;
    ++g_matrixStack.top.info.index;
    if (g_matrixStack.top.info.index < g_matrixStack.top.info.capacity) {
        ++g_matrixStack.top.stack;
    }
    workTransformMatrix = transform;
}

void PopDrawTransform() {
    std::int32_t count = 1;
    if (g_matrixStack.top.info.index < 1)
        count = g_matrixStack.top.info.index;
    std::int32_t stackCount = count;
    if (g_matrixStack.top.info.index > g_matrixStack.top.info.capacity) {
        stackCount = count + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index;
    }
    g_matrixStack.top.info.index -= count;
    g_matrixStack.top.stack -= stackCount;
    workTransformMatrix = *g_matrixStack.top.stack;
}

void DisableLampLight(const LampComponentPayload &lamp) {
    if (lamp.lightIndex != kNoPublishedLightSlot && lamp.lightIndex < 128U)
        lightSetSlotEnabled(lamp.lightIndex, 0);
}
void QueueLampTaskDestruction(HLib::Task *task) {
    if (!task)
        return;
    task->initCallbackFnPtr = nullptr;
    task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
    task->autoInitTaskCallbackFnPtr = nullptr;
}

} // namespace

LampComponentPayload *LAMP_GetWork(Character *character) {
    CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLampComponentId);
    return component ? reinterpret_cast<LampComponentPayload *>(component + 1) : nullptr;
}

LampComponentPayload *LAMP_Init(Character *character) {
    auto *const lamp = static_cast<LampComponentPayload *>(AllocateHeapMemoryMODL(character, kLampComponentId, sizeof(LampComponentPayload), EnqueueLAMP));

    lamp->modelOwner = nullptr;
    lamp->lightIndex = 0;
    lamp->active = 0;
    lamp->glowLevel = 0;
    lamp->reserved_14 = 0;
    void *const modelFile = ReadFile("model/object", "TALS500G.MT5", static_cast<std::int32_t>(kLampComponentId));
    lamp->modelOwner = MT5_InitRawModelAndTextures(modelFile, "TALS500G.MT5", nullptr);
    lamp->lightIndex = LGHT_GetMode() == 1 ? kNoPublishedLightSlot : kLampLightSlot;
    lamp->active = 0;
    lamp->glowLevel = 128;
    lamp->reserved_14 = 0;

    if (character) {
        CharacterComponentNode *const component = HLib_GetCharacterComponent(character, kLampComponentId);
        component->destroyCallback = LAMP_Cleanup;
    }

    if (lamp->lightIndex != kNoPublishedLightSlot) {
        lightResetSlotToDefaults(lamp->lightIndex);
        SetLightBaseType(lamp->lightIndex, 3);
        SetRGBStruct(lamp->lightIndex, &kLightColour);
        SetLightFalloffRangeByIndex(lamp->lightIndex, 1.4f);
        lightSetSlotEnabled(lamp->lightIndex, 0);
    }
    return lamp;
}

void EnqueueLAMP(Character *character, void *payload) {
    auto *const lamp = static_cast<LampComponentPayload *>(payload);
    if (lamp->active == 0) {
        DisableLampLight(*lamp);
        return;
    }

    HLib::Task *const task = HLib_EnqueueTaskWithoutParameter(LAMP_Callback, kLampDrawTaskIndex, 0, static_cast<std::int32_t>(kLampComponentId));
    if (!task)
        return;
    if (task->task_data_ptr) {
        QueueLampTaskDestruction(task);
        return;
    }

    auto *const draw = static_cast<LampDrawTaskData *>(HLib_AllocHeapMemBlock(sizeof(LampDrawTaskData), kTaskDataToken));
    if (!draw) {
        QueueLampTaskDestruction(task);
        return;
    }
    HLib_EnableTokenCheck(draw);
    task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(draw);

    draw->opacityStep = kGlowStep;
    draw->opacity = static_cast<float>(static_cast<std::int32_t>(lamp->glowLevel)) * (1.0f / 512.0f) + 0.25f;
    draw->modelRoot = lamp->modelOwner->model;
    BuildLampFrame(*character, draw->transform, lamp->lightIndex);
}

void LAMP_Callback(HLib::TaskData *taskData) {
    auto *const draw = reinterpret_cast<LampDrawTaskData *>(taskData);

    PushDrawTransform(draw->transform);
    d3t::MT5_PushAndReadNodeTree(draw->modelRoot, nullptr, draw->opacity);
    PopDrawTransform();

    draw->opacity += draw->opacityStep;
    if (draw->opacity <= 0.0f)
        HLib_DestroyTask(CurrentTask);
}

void LAMP_Cleanup(Character *, void *payload) {
    auto *const lamp = static_cast<LampComponentPayload *>(payload);
    if (lamp->lightIndex < 128U)
        lightSetSlotEnabled(lamp->lightIndex, 0);
    FreePVR(lamp->modelOwner);
}

bool LAMP_SetScriptState(Character *character, std::uint32_t mode, std::uint32_t value) {
    LampComponentPayload *const lamp = LAMP_GetWork(character);
    if (!lamp)
        return false;
    if (mode == 0) {
        lamp->active = value;
        return true;
    }
    if (mode == 1 && value <= 0x100U) {
        lamp->glowLevel = value;
        return true;
    }
    return false;
}

} // namespace shenmue::s1
