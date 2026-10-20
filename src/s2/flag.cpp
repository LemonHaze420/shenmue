#include "s2/flag.hpp"

#include "shenmue.h"
#include "s2/area_event.hpp"
#include "s2/area_jump.hpp"
#include "s2/author.hpp"
#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/conversation.hpp"
#include "s2/date_time.hpp"
#include "s2/debug_text.hpp"
#include "s2/event_runtime.hpp"
#include "s2/field_collision.hpp"
#include "s2/game_runtime.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/help.hpp"
#include "s2/hlib.hpp"
#include "s2/input.hpp"
#include "s2/inventory.hpp"
#include "s2/load_title.hpp"
#include "s2/map_control.hpp"
#include "s2/math.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/pak.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/save.hpp"
#include "s2/scene_data.hpp"
#include "s2/screen_film.hpp"
#include "s2/search.hpp"
#include "s2/sound_program.hpp"
#include "s2/story_state.hpp"
#include "s2/ui.hpp"
#include "s2/system_modules.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "d3t/dx11_renderer.hpp"

#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>

namespace shenmue::s2 {

ShenPlayerData shenPlayerData{};
std::int32_t g_systemManageDebugDisplay{};
std::uint8_t g_flagDebugIntersectionDraw{};

FlagWindObject *g_flagWindObject{};

FlagWindObject *GetCurrentFlagObjectForWind() {
    return g_flagWindObject;
}

FlagWindModel *FindFlagWindModel(const MT7Node *node) {
    FlagWindObject *const object = GetCurrentFlagObjectForWind();
    if (!object || static_cast<std::int32_t>(object->modelCount) <= 0)
        return nullptr;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(object->modelCount); ++index)
        if (object->models[index].node == node)
            return object->models + index;
    return nullptr;
}

namespace {
using WindVertexCallback = void (*)(FlagWindVertex *, Vector3 *, std::int32_t *);
Vector3 flagWindMaximum, flagWindMinimum;

bool SelectWindVertex(std::int32_t type, const Vector3 &vertex) {
    if (type == 0)
        return vertex.x > 0.050000001f && vertex.y < -0.050000001f;
    if (type == 1)
        return vertex.x > 0.050000001f;
    if (type == 7)
        return vertex.y < -0.050000001f;
    return true;
}

template <std::int32_t Type> void CountWindVertex(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if constexpr (Type == 2) {
        if (!destination && vertex)
            ++*count;
    } else if (!destination && SelectWindVertex(Type, *vertex))
        ++*count;
}

template <std::int32_t Type> void BindWindVertex(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if (SelectWindVertex(Type, *vertex)) {
        destination[*count].position = vertex;
        destination[*count].originalPosition = *vertex;
        ++*count;
    }
}

void WindModuleCallback(FlagWindVertex *context, Vector3 *, std::int32_t *) {
    ChangeModuleTask_Init(context);
}



constexpr WindVertexCallback windCountCallbacks[]{CountWindVertex<0>, CountWindVertex<1>, CountWindVertex<2>, CountWindVertex<2>, CountWindVertex<2>,
    CountWindVertex<2>, CountWindVertex<2>, CountWindVertex<7>, BindWindVertex<0>, BindWindVertex<1>};
constexpr WindVertexCallback windBindCallbacks[]{BindWindVertex<0>, BindWindVertex<1>, BindWindVertex<2>, BindWindVertex<2>, BindWindVertex<2>,
    BindWindVertex<2>, BindWindVertex<2>, BindWindVertex<7>, nullptr, WindModuleCallback};

void VisitWindVertex(WindVertexCallback callback, FlagWindVertex *destination, Vector3 *vertex, std::int32_t &count) {
    if (callback)
        callback(destination, vertex, &count);
    for (std::size_t axis = 0; axis < 3; ++axis) {
        if ((*vertex)[axis] > flagWindMaximum[axis])
            flagWindMaximum[axis] = (*vertex)[axis];
        if (flagWindMinimum[axis] > (*vertex)[axis])
            flagWindMinimum[axis] = (*vertex)[axis];
    }
}

std::uint64_t VisitWindMeshVertices(void *mesh, WindVertexCallback callback, FlagWindVertex *destination, std::int32_t &count) {
    flagWindMaximum = {-100, -100, -100};
    flagWindMinimum = {100, 100, 100};
    count = 0;
    auto *const words = static_cast<std::int32_t *>(mesh);
    if (IsModelXB01(mesh)) {
        const bool mapped = callback == BindWindVertex<0> || callback == BindWindVertex<1> || callback == BindWindVertex<2> || callback == BindWindVertex<7>;
        const auto vertexCount = static_cast<std::uint32_t>(words[words[7] + 10]) >> 5;
        auto *const vertices = mapped ? static_cast<std::byte *>(CopyForDirectX(mesh, 0, 0, 0, 0)) : reinterpret_cast<std::byte *>(words + words[7] + 11);
        for (std::uint32_t index = 0; index < vertexCount; ++index)
            VisitWindVertex(callback, destination, reinterpret_cast<Vector3 *>(vertices + 32 * index), count);
        if (mapped)
            CopyBufferToDirectX_Wrapper(mesh, 0);
        return 0;
    }
    auto *cursor = words + 6;
    while (*cursor) {
        const auto bytes = cursor[19];
        cursor += 20;
        auto *const end = cursor + bytes / 4;
        while (cursor < end) {
            const auto flags = cursor[0];
            std::int32_t vertices = cursor[1];
            cursor += 2;
            if (flags & 8)
                vertices *= 3;
            else if (flags & 4)
                vertices *= 4;
            for (std::int32_t index = 0; index < vertices; ++index) {
                if (*reinterpret_cast<std::uint8_t *>(cursor) & 1U) {
                    VisitWindVertex(callback, destination, reinterpret_cast<Vector3 *>(cursor), count);
                    cursor += 8;
                } else {
                    if (cursor[1] & 1)
                        return UINT64_C(0xffffffff);
                    cursor += 2;
                }
            }
        }
    }
    return 0;
}

std::uint32_t CountFlagWindModels(MT7Node *node) {
    const auto kind = static_cast<std::uint16_t>(node->id);
    std::uint32_t count = static_cast<std::uint16_t>(kind - 5000) <= 9U;
    if (node->runtimeChild)
        count += CountFlagWindModels(node->runtimeChild);
    if (node->runtimeSibling)
        count += CountFlagWindModels(node->runtimeSibling);
    return count;
}

std::int32_t CollectFlagWindModels(MT7Node *node, FlagWindModel *models, std::int32_t &index) {
    std::int32_t vertices = 0;
    const auto kind = static_cast<std::uint16_t>(node->id);
    if (static_cast<std::uint16_t>(kind - 5000) <= 9U) {
        auto &model = models[index++];
        model.type = static_cast<std::int16_t>(kind) - 5000;
        model.node = node;
        model.mesh = node->runtimeMesh;
        if (model.mesh)
            VisitWindMeshVertices(model.mesh, windCountCallbacks[model.type], nullptr, vertices);
        model.vertexCount = vertices;
    }
    if (node->runtimeChild)
        vertices += CollectFlagWindModels(node->runtimeChild, models, index);
    if (node->runtimeSibling)
        vertices += CollectFlagWindModels(node->runtimeSibling, models, index);
    return vertices;
}

std::uint64_t PopulateFlagWindObject(MT7Node *root, FlagWindObject &object, std::uint32_t modelCount) {
    object.root = root;
    auto *const models = static_cast<FlagWindModel *>(HLib::AllocateTaskData(sizeof(FlagWindModel) * modelCount, FLAG_WIND_COMPONENT));
    if (!models)
        return UINT64_C(0xffffffff);
    std::int32_t index = 0;
    std::int32_t count = CollectFlagWindModels(root, models, index);
    object.vertices = static_cast<FlagWindVertex *>(HLib::AllocateTaskData(sizeof(FlagWindVertex) * static_cast<std::int64_t>(count), FLAG_WIND_COMPONENT));
    if (!object.vertices)
        return UINT64_C(0xffffffff);
    object.modelCount = modelCount;
    object.models = models;
    auto *cursor = object.vertices;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(modelCount); ++index) {
        auto &model = models[index];
        model.vertices = cursor;
        VisitWindMeshVertices(model.mesh, windBindCallbacks[model.type], cursor, count);
        model.phase = IncrementSharedPrngUInt16();
        model.speed = IncrementSharedPrngUInt16() % 5 + 10;
        model.maximumX = flagWindMaximum.x;
        model.minimumX = flagWindMinimum.x;
        model.maximumY = flagWindMaximum.y;
        model.minimumY = flagWindMinimum.y;
        model.maximumZ = flagWindMaximum.z;
        model.minimumZ = flagWindMinimum.z;
        cursor += count;
    }
    return 0;
}

// @verified - p
float EvaluateWindWave(std::int16_t angle) {
    float result = sin_fixed16(static_cast<std::int16_t>(22 * angle)) * 0.3f;
    result = sin_fixed16(static_cast<std::int16_t>(31 * angle)) * 0.37f + result;
    result = sin_fixed16(static_cast<std::int16_t>(103 * angle)) * 0.15f + result;
    return sin_fixed16(static_cast<std::int16_t>(147 * angle)) * 0.18f + result;
}

// @verified - p
void ApplyWaveformTransformSinewave(FlagWindVertex *vertices, std::uint32_t count, std::int16_t phase, float amplitude) {
    for (std::uint32_t index = 0; index < count; ++index) {
        FlagWindVertex &vertex = vertices[index];
        const float x = vertex.originalPosition.x;
        const float negativeY = -vertex.originalPosition.y;
        const std::int16_t angle = static_cast<std::int16_t>(phase - FloatToShort(((x - vertex.originalPosition.y) * 65536.0f) * 0.01f));
        const float wave = EvaluateWindWave(angle);
        const float yOffset = (wave * negativeY) * 0.05f;
        const float xOffset = (wave * x) * 0.2f;
        vertex.position->z = (yOffset + xOffset) * amplitude;
        vertex.position->x = x - (0.0f > xOffset ? 0.0f : xOffset);
        vertex.position->x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.position->x) | 1U);
        vertex.position->y = -negativeY - (0.0f < yOffset ? 0.0f : yOffset);
    }
}

// @verified - p
void ApplyCurvedWindOscillation(FlagWindModel *model, float amplitude) {
    std::int32_t bend =
        amplitude >= -2147483648.0f && amplitude < 2147483648.0f ? static_cast<std::int32_t>(amplitude) : std::numeric_limits<std::int32_t>::min();
    if (bend > 64)
        bend = 64;
    else if (bend < 32)
        bend = 32;
    const std::uint16_t bendAngle = static_cast<std::uint16_t>(bend << 8);
    const float sine = sin_fixed16(static_cast<std::int16_t>(bendAngle));
    const float cosine = cos_fixed16(bendAngle);
    const float extent = model->maximumY - model->minimumY;
    for (std::int32_t index = 0; index < model->vertexCount; ++index) {
        FlagWindVertex &vertex = model->vertices[index];
        const float x = vertex.originalPosition.x;
        const float negativeY = -vertex.originalPosition.y;
        const std::int16_t angle = static_cast<std::int16_t>(model->phase - FloatToShort((((x + extent) - negativeY) * 65536.0f) * 0.01f));
        const float wave = EvaluateWindWave(angle);
        const float offset = (wave * x) * 0.4f;
        vertex.position->z = (((extent - negativeY) * wave) * 0.1f + offset) * 0.5f;
        vertex.position->x = x * sine - (0.0f > offset ? 0.0f : offset);
        vertex.position->x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.position->x) | 1U);
        vertex.position->y = (-negativeY - ((extent - negativeY) * wave) * 0.1f) - (1.0f - negativeY / extent * 0.5f) * (x * cosine);
    }
}

// @verified - p
void VerticalWaveOffsetZBend(FlagWindModel *model, float amplitude) {
    for (std::int32_t index = 0; index < model->vertexCount; ++index) {
        FlagWindVertex &vertex = model->vertices[index];
        const float height = model->maximumY - vertex.originalPosition.y;
        const float x = vertex.originalPosition.x;
        const std::int16_t angle = static_cast<std::int16_t>(model->phase - FloatToShort(((height + x) * 65536.0f) * 0.005f));
        const float offset = ((EvaluateWindWave(angle) * height) * 0.1f) * amplitude;
        vertex.position->z = offset + vertex.originalPosition.z;
        vertex.position->x = x - offset;
        vertex.position->x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.position->x) | 1U);
        vertex.position->y = (offset < 0.0f ? -offset : offset) + vertex.originalPosition.y;
    }
}

// @verified - p
void SineStackYCurve(FlagWindModel *model, float scale) {
    const float upper = model->maximumY - 0.05f;
    const float lower = model->minimumY + 0.05f;
    for (std::int32_t index = 0; index < model->vertexCount; ++index) {
        FlagWindVertex &vertex = model->vertices[index];
        const float y = vertex.originalPosition.y;
        const float x = vertex.originalPosition.x;
        if (y <= upper && lower <= y) {
            const std::int16_t angle = static_cast<std::int16_t>(model->phase - FloatToShort(((x - y) * 65536.0f) * 0.005f));
            const float wave = EvaluateWindWave(angle);
            const float yOffset = (-y * wave) * 0.05f;
            const float xOffset = wave * 0.2f;
            vertex.position->z = (yOffset + xOffset) * scale;
            vertex.position->x = x - (0.0f > xOffset ? 0.0f : xOffset);
            vertex.position->x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.position->x) | 1U);
            vertex.position->y = y - (0.0f < yOffset ? 0.0f : yOffset);
        }
    }
}

// @verified - p
void DualAxisZone(FlagWindModel *model, float scale) {
    const float upperX = model->maximumX - 0.05f;
    const float lowerX = model->minimumX + 0.05f;
    const float upperZ = model->maximumZ - 0.05f;
    const float lowerZ = model->minimumZ + 0.05f;
    for (std::int32_t index = 0; index < model->vertexCount; ++index) {
        FlagWindVertex &vertex = model->vertices[index];
        const float x = vertex.originalPosition.x;
        const float z = vertex.originalPosition.z;
        if (upperX > x && x > lowerX && upperZ > z && z > lowerZ) {
            const std::int16_t angle = static_cast<std::int16_t>(model->phase - FloatToShort(((z + x) * 65536.0f) * 0.005f));
            const float wave = EvaluateWindWave(angle);
            const float xOffset = wave * 0.1f;
            const float zOffset = wave * 0.05f;
            vertex.position->y = xOffset * scale + vertex.originalPosition.y;
            vertex.position->x = x - (0.0f > xOffset ? 0.0f : xOffset) * scale;
            vertex.position->x = std::bit_cast<float>(std::bit_cast<std::uint32_t>(vertex.position->x) | 1U);
            vertex.position->z = z - (0.0f < zOffset ? 0.0f : zOffset) * scale;
        }
    }
}

// @verified - p
void FreeWindModel(Character *character, void *payload) {
    if (!character || !payload)
        return;
    FlagWindObject &object = *static_cast<FlagWindObject *>(payload);
    if (object.vertices) {
        HLib::FreeTaskData(object.vertices);
        object.vertices = nullptr;
    }
    if (object.models) {
        HLib::FreeTaskData(object.models);
        object.models = nullptr;
    }
}

// @verified - p
void UpdateWindForChara(Character *character, void *payload) {
    if (!character)
        return;
    const Vector3 position = GetCharacterWorldPosition(character);
    const float radius = GetCharacterDisplayRadius(character);
    if (camCtrlIsInsideDepthScaledRect(position, radius))
        UpdateWindForModel(static_cast<FlagWindObject *>(payload));
}
}

void UpdateWindForFlag() {
    if (g_flagWindObject)
        UpdateWindForModel(g_flagWindObject);
}

FlagWindObject *AllocateFlagObject(MT7Node *root) {
    const auto count = CountFlagWindModels(root);
    if (!count)
        return nullptr;
    auto *const object = static_cast<FlagWindObject *>(HLib::AllocateTaskData(sizeof(FlagWindObject), FLAG_WIND_COMPONENT));
    if (object)
        PopulateFlagWindObject(root, *object, count);
    return object;
}

// @verified - p
void AttachFlagWindComponent(Character *character) {
    if (!character || !character->model)
        return;
    MT7Model &model = *static_cast<MT7Model *>(character->model);
    MT7Node *const root = &model.nodes.front();
    const std::uint32_t count = CountFlagWindModels(root);
    if (!count)
        return;
    FlagWindObject *const object =
        static_cast<FlagWindObject *>(AllocateModelComponent(character, FLAG_WIND_COMPONENT, sizeof(FlagWindObject), UpdateWindForChara));
    if (!object)
        return;
    PopulateFlagWindObject(root, *object, count);
    (reinterpret_cast<ModelComponentHeader *>(object) - 1)->cleanup = FreeWindModel;
}

namespace {
void AnimateFlagCallback(Character *character, void *payload) {
    if (!character)
        return;
    FlagAnimationObject &object = *static_cast<FlagAnimationObject *>(payload);
    const float scaled = GetEffectiveWindSpeed(GetWind()) * 40.0f;
    GetCurrentWindPhaseAccumulator(GetWind());
    const float amplitude = 1.0f > scaled ? 1.0f : scaled;
    for (std::int32_t index = 0; index < object.placementCount; ++index) {
        FlagAnimationPlacement &placement = object.placements[index];
        if (!camCtrlIsSphereVisible(placement.translation, 2.0f) || !camCtrlCheckPosition(placement.translation, 2.0f))
            continue;
        const float step = 5.0f > static_cast<float>(placement.speed) * scaled ? 5.0f : static_cast<float>(placement.speed) * scaled;
        const std::int32_t increment = step < 2147483648.0f ? static_cast<std::int32_t>(step) : std::numeric_limits<std::int32_t>::min();
        placement.phase = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(increment) + static_cast<std::uint32_t>(placement.phase)) % 65536;
        ApplyWaveformTransformSinewave(object.vertices, static_cast<std::uint32_t>(object.vertexCount), static_cast<std::int16_t>(placement.phase), amplitude);
        if (index) {
            Matrix4x4_Push(nullptr);
            Matrix4x4::preTranslate(placement.translation.x, placement.translation.y, placement.translation.z);
            if (placement.rotationZ)
                Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(placement.rotationZ));
            if (placement.rotationY)
                Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(placement.rotationY));
            ConstructMatricesForObjectRecursive(*object.model, &object.model->nodes.front());
            Matrix4x4::Pop(1);
        } else {
            shenApplyTranslationToAll(&character->entity.node, placement.translation);
            SetCharacterWorldRotation(character, Vector3i{.x = 0, .y = placement.rotationY, .z = placement.rotationZ});
        }
    }
}

void FreeAnimatedFlag(Character *character, void *payload) {
    if (!character || !payload)
        return;
    FlagAnimationObject &object = *static_cast<FlagAnimationObject *>(payload);
    if (object.vertices)
        HLib::FreeTaskData(object.vertices);
}
} // namespace

// Collect the XB01 wind vertices (x > 0.05, y < -0.05) of a scenario character's first mesh and animate them per placement.
void AssignAnimatedFlag(shen_name name, FlagAnimationPlacement *placements, std::int32_t count) {
    if (!placements || !count)
        return;
    const MapControlData *const control = static_cast<const MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = FindScenarioCharacter(control->scenarioTask, name);
    if (!character || !character->model)
        return;
    MT7Model *const model = static_cast<MT7Model *>(character->model);
    if (model->nodes.empty())
        return;
    MT7Node *const mesh = mtGetTransform(&model->nodes.front());
    if (!mesh || !mt7GetXb01Offset(mesh))
        return;
    std::int32_t vertices = 0;
    if (VisitWindMeshVertices(mesh->runtimeMesh, CountWindVertex<0>, nullptr, vertices))
        return;
    FlagAnimationObject *const object =
        static_cast<FlagAnimationObject *>(AllocateModelComponent(character, FLAG_WIND_COMPONENT, sizeof(FlagAnimationObject), AnimateFlagCallback));
    if (!object)
        return;
    object->model = model;
    object->vertices = static_cast<FlagWindVertex *>(HLib::AllocateTaskData(sizeof(FlagWindVertex) * static_cast<std::int64_t>(vertices), FLAG_WIND_COMPONENT));
    if (!object->vertices)
        return;
    object->vertexCount = vertices;
    object->placements = placements;
    object->placementCount = count;
    VisitWindMeshVertices(mesh->runtimeMesh, BindWindVertex<0>, object->vertices, vertices);
    (reinterpret_cast<ModelComponentHeader *>(object) - 1)->cleanup = FreeAnimatedFlag;
}

// @verified - p
void UpdateWindForModel(FlagWindObject *object) {
    float speed = GetEffectiveWindSpeed(GetWind());
    GetCurrentWindPhaseAccumulator(GetWind());
    if (debugSampledWindSpeedOverride <= 0.00000011920929f)
        speed = 0.06f < speed ? 0.06f : speed;
    const float rawAmplitude = speed * 40.0f;
    float amplitude = rawAmplitude;
    if (debugSampledWindSpeedOverride <= 0.00000011920929f)
        amplitude = 1.0f > rawAmplitude ? 1.0f : rawAmplitude;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(object->modelCount); ++index) {
        FlagWindModel &model = object->models[index];
        float oscillation = static_cast<float>(model.speed) * rawAmplitude;
        if (IsModelXB01(model.mesh))
            CopyForDirectX(model.mesh, 0, 0, 0, 0);
        switch (model.type) {
        case 0:
            oscillation = 15.0f > oscillation ? 15.0f : oscillation;
            ApplyWaveformTransformSinewave(model.vertices, static_cast<std::uint32_t>(model.vertexCount), static_cast<std::int16_t>(model.phase), amplitude);
            break;
        case 1:
            oscillation = 55.0f > oscillation ? 55.0f : (120.0f < oscillation ? 120.0f : oscillation);
            ApplyCurvedWindOscillation(&model, oscillation);
            break;
        case 2:
            oscillation = 15.0f > oscillation ? 15.0f : oscillation;
            amplitude = 2.0f > amplitude ? 2.0f : amplitude;
            VerticalWaveOffsetZBend(&model, amplitude);
            break;
        case 3:
            oscillation = 5.0f > oscillation ? 5.0f : oscillation;
            amplitude = 3.0f < amplitude ? 3.0f : amplitude;
            SineStackYCurve(&model, amplitude);
            break;
        case 4:
            amplitude = 2.0f < amplitude ? 2.0f : amplitude;
            oscillation = 5.0f > oscillation ? 5.0f : oscillation;
            DualAxisZone(&model, amplitude * 0.5f);
            break;
        case 5:
            oscillation = 20.0f > oscillation ? 20.0f : oscillation;
            amplitude = 2.0f > amplitude ? 2.0f : amplitude;
            VerticalWaveOffsetZBend(&model, amplitude);
            break;
        case 6:
            oscillation = 5.0f > oscillation ? 5.0f : oscillation;
            DualAxisZone(&model, amplitude);
            break;
        default:
            break;
        }
        const std::int32_t increment =
            oscillation >= -2147483648.0f && oscillation < 2147483648.0f ? static_cast<std::int32_t>(oscillation) : std::numeric_limits<std::int32_t>::min();
        model.phase = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(model.phase) + static_cast<std::uint32_t>(increment)) % 65536;
        if (IsModelXB01(model.mesh))
            CopyBufferToDirectX_Wrapper(model.mesh, 0);
    }
}

namespace {

constexpr shen_name callbackContextCycle = SHEN_NAME(cylf);
constexpr shen_name callbackContextSelf = SHEN_NAME(self);

constexpr std::array<FlagCallbackEntry, 3> flagCallbackEntries{{{1011, 1200}, {1070, 1141}, {0, 0}}};

constexpr std::array<FlagSector, 14> flagSectors{{
    {1.8f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x3000, 0x3e38, {0.3f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x3000, 0x3e38, {-0.34f, 0.0f, 0.0f}, 0.3f},
    {-1.0f, 0x4000, 0x4000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {3.0f, 0x4000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.5f, 0x8000, 0x3000, 0x3e38, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x238e, 0x238e, {0.0f, 0.0f, 0.0f}, 0.3f},
    {2.0f, 0x8000, 0x238e, 0x238e, {0.0f, 0.0f, 0.0f}, 0.3f},
}};

constexpr std::array<SleepScheduleEntry, 24> sleepScheduleEntries{{
    {16, 5940, 1379, 0x500, 40, 0, SHEN_NAME(WS00)},
    {891, 5940, 1379, 0x501, 41, 0, SHEN_NAME(WTA0)},
    {203, 5940, 1379, 0x502, 40, 0, SHEN_NAME(WTA0)},
    {52, 5940, 1260, 0x603, 80, 0, SHEN_NAME(WK00)},
    {111, 5940, 1379, 0x504, 40, 0, SHEN_NAME(WN00)},
    {205, 5940, 1170, 0x605, 0, 0, SHEN_NAME(TOFB)},
    {1125, 5940, 1679, 0x605, 40, 0, SHEN_NAME(AR03)},
    {204, 5940, 1379, 0x606, 95, 0, SHEN_NAME(AB00)},
    {127, 5940, 1560, 0x607, 60, 0, SHEN_NAME(AR02)},
    {202, 5940, 1379, 0x508, 40, 0, SHEN_NAME(WB00)},
    {455, 5940, 1499, 0x509, 77, 0, SHEN_NAME(YMQB)},
    {133, 5940, 1379, 0x50a, 0, 0, SHEN_NAME(SYW0)},
    {771, 5940, 1559, 0x50b, 40, 0, SHEN_NAME(QR00)},
    {772, 5940, 1559, 0x50c, 41, 0, SHEN_NAME(QR00)},
    {736, 5940, 780, 0x50d, 40, 0, SHEN_NAME(QUG0)},
    {241, 5940, 705, 0x50e, 0, 0, SHEN_NAME(0523)},
    {235, 5940, 990, 0x50f, 0, 0, SHEN_NAME(KWQA)},
    {501, 1320, 1380, 0x101, 86, 0, SHEN_NAME(AR01)},
    {502, 1320, 1380, 0x102, 79, 0, SHEN_NAME(WSY0)},
    {503, 1320, 1380, 0x103, 80, 0, SHEN_NAME(WTA0)},
    {504, 1320, 1380, 0x104, 95, 0, SHEN_NAME(AB00)},
    {505, 1320, 1380, 0x105, 80, 0, SHEN_NAME(QRR0)},
    {670, 5940, 5940, 0x000, 1, 0, SHEN_NAME(0000)},
    {},
}};

constexpr std::array<FlagContextSelector, 5> flagContextSelectors{{
    {1160, SHEN_NAME(rcha)},
    {620, SHEN_NAME(sel2)},
    {1205, SHEN_NAME(lcha)},
    {212, SHEN_NAME(lcha)},
    {0, {}},
}};

Vector3 flagCameraPosition{};
Vector3 currentCameraPosition{};
std::int32_t flagCameraFrame{};
float flagPlayerAlpha{};
Vector3 previousCameraPosition{};
std::int32_t flagCandidateCount{};
Vector3 orbitCameraPosition{};
Vector3 orbitCameraTarget{};
std::array<FlagRecord *, 10> flagCallStack{};
Vector3 flagLocation{};
std::int32_t systemManageFrames{};
FlagRecord *selectedFlagCandidate{};
std::array<FlagRecord *, 9> flagContextRecords{};
#if SHENMUE_IS_DC
std::array<char, 0x14> systemManageSource{};
#else
std::array<char, 0x18> systemManageSource{};
#endif
std::array<FlagCandidate, 10> flagCandidates{};
std::array<std::uint32_t, 62> flagDebugModelCommands = FLAG_DEBUG_MODEL_COMMAND_TEMPLATE;
#if SHENMUE_IS_XBOX
std::array<char, 0x14> aberdeenSearchData{};
#else
std::array<char, 0x18> aberdeenSearchData{};
#endif

std::int32_t *FlagWords() {
    return reinterpret_cast<std::int32_t *>(g_flagDescriptorData);
}

std::int32_t *FindFlagDefinition(std::int32_t flag) {
    const FlagBinHeader *const header = reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData);
    if (!header || static_cast<std::uint32_t>(flag) >= header->flagCount || flag < 0)
        return nullptr;
    std::int32_t *const words = FlagWords();
    return words + words[header->flagRecordTable + flag];
}

std::int32_t *FlagContextTable() {
    const FlagBinHeader *const header = reinterpret_cast<const FlagBinHeader *>(g_flagDescriptorData);
    return FlagWords() + header->contextTable;
}

FlagContextEntry *FindFlagContext(shen_name name) {
    FlagContextEntry *entry = reinterpret_cast<FlagContextEntry *>(FlagContextTable());
    if (entry->name.name == name.name)
        return entry;
    for (std::uint32_t current = entry->name.name; current != 0;) {
        ++entry;
        current = entry->name.name;
        if (current == name.name)
            return entry;
    }
    return nullptr;
}

FlagRecord *FlagRecordAt(std::int32_t *words, std::int32_t offset) {
    return reinterpret_cast<FlagRecord *>(words + offset);
}

FlagRecord *FlagAdvance(FlagRecord *record, std::uint32_t dwords) {
    return FlagRecordAt(reinterpret_cast<std::int32_t *>(record), static_cast<std::int32_t>(dwords));
}

// True continues with the next record; false takes the record's branch.
FlagRecord *FlagBranch(FlagRecord *record, bool condition) {
    if ((record->branch & 0x8000U) != 0)
        return FlagAdvance(record, condition ? record->branch & 0x7fffU : record->length);
    return FlagAdvance(record, condition ? record->length : record->branch);
}

Character *FlagPlayer() {
    return static_cast<Character *>(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->ryo);
}

Character *FlagChara(shen_name id) {
    return FindScenarioCharacter(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask, id);
}

// Vector3::DistanceBetween returns the squared distance.
float FlagDistanceSquared(const Vector3 &a, const Vector3 &b) {
    return ((a.y - b.y) * (a.y - b.y) + (a.x - b.x) * (a.x - b.x)) + (a.z - b.z) * (a.z - b.z);
}

std::int16_t FlagInverseAtan(float x, float z) {
    return static_cast<std::int16_t>(FixedAtan2(-x, -z));
}

bool TestCharaPositionWithinRange(shen_name charaA, const Vector3 &first, const Vector3 &second, shen_name charaB) {
    Vector3 origin{};
    Character *const character = FlagChara(charaA);
    if (charaB.name != 0) {
        const Character *const reference = FlagChara(charaB);
        if (reference)
            origin = reference->entity.node.position;
    }
    float minimumX = origin.x + first.x;
    float maximumX = origin.x + second.x;
    float minimumZ = origin.z + first.z;
    float maximumZ = origin.z + second.z;
    if (minimumX > maximumX) {
        minimumX = origin.x + second.x;
        maximumX = origin.x + first.x;
    }
    if (minimumZ > maximumZ) {
        minimumZ = origin.z + second.z;
        maximumZ = origin.z + first.z;
    }
    const Vector3 position = character ? character->entity.node.position : Vector3{};
    return position.x >= minimumX && maximumX >= position.x && position.z >= minimumZ && maximumZ >= position.z;
}

Vector3 FlagCharaBone(Character *character) {
    Vector3 position{};
    GetCharacterMotionWorldPosition(character, 5, &position);
    return position;
}


bool flagIsPlayerNearPoint(const FlagRecord *record) {
    Character *const player = FlagPlayer();
    const Vector3 position = player ? GetCharacterWorldPosition(player) : Vector3{-1000.0f, -1000.0f, -1000.0f};
    const float radius = std::bit_cast<float>(record->argument[2]);
    const Vector3 point{std::bit_cast<float>(record->argument[0]), position.y, std::bit_cast<float>(record->argument[1])};
    if ((g_flagDebugIntersectionDraw & 1U) != 0)
        flagDrawDebugCylinder(point, radius);
    if (record->argument[4] == 0x8000) {
        return (position.x - point.x) * (position.x - point.x) + (position.z - point.z) * (position.z - point.z) <= radius * radius;
    }
    return ComputeAngle(position, radius, static_cast<std::int16_t>(record->argument[3]), record->argument[4], point);
}

bool ryoCheckInRangeForFlag(const FlagRecord *record) {
    if ((g_flagDebugIntersectionDraw & 1U) != 0) {
        flagDrawDebugBox(std::bit_cast<float>(record->argument[0]), std::bit_cast<float>(record->argument[1]), std::bit_cast<float>(record->argument[2]),
            std::bit_cast<float>(record->argument[3]), 50.0f);
    }
    const Vector3 first{std::bit_cast<float>(record->argument[0]), 0.0f, std::bit_cast<float>(record->argument[1])};
    const Vector3 second{std::bit_cast<float>(record->argument[2]), 0.0f, std::bit_cast<float>(record->argument[3])};
    return TestCharaPositionWithinRange(SHEN_NAME(RYO_), first, second, shen_name{});
}

bool DoAngleCheck(const FlagSector &sector, shen_name chara, const Vector3 &playerPosition, std::int16_t playerYaw) {
    Character *const character = FlagChara(chara);
    if (!character)
        return false;
    const std::int16_t charaYaw = GetAllAngYForNode(&character->entity.node);
    Vector3 point = GetCharacterWorldPosition(character);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(point.x, point.y, point.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(charaYaw));
    Matrix4x4::TransformPoint(sector.offset, point);
    Matrix4x4::Pop(1);
    flagLocation = point;
    if (!ComputeAngle(playerPosition, sector.radius, playerYaw, sector.playerHalfAngle, point))
        return false;
    const std::int16_t yaw = static_cast<std::int16_t>(GetAllAngYForNode(&character->entity.node) + sector.yawOffset);
    return ComputeAngle(point, sector.radius, yaw, sector.charaHalfAngle, playerPosition) && playerPosition.y > point.y - sector.heightTolerance &&
        sector.heightTolerance + point.y > playerPosition.y;
}

bool shenCheckCloseAndFacing(const Vector3 &pointA, std::int16_t yawA, const Vector3 &pointB, std::int16_t yawB) {
    flagLocation = pointA;
    if (!ComputeAngle(pointB, 3.0f, yawB, 0x3000, pointA) || !ComputeAngle(pointA, 3.0f, yawA, 0x3e38, pointB))
        return false;
    return pointB.y > pointA.y - 1.0f && pointA.y + 1.0f > pointB.y;
}

bool IsWithinLocationScreenSpace(const Vector3 &point) {
    flagLocation = point;
    Character *const player = FlagPlayer();
    const Vector3 bone = FlagCharaBone(player);
    if (!(2.0f > std::fabs(point.y - bone.y)))
        return false;
    const float x = point.x - bone.x;
    const float z = point.z - bone.z;
    if (!(9.0f > z * z + x * x))
        return false;
    const std::uint16_t yaw = static_cast<std::uint16_t>(0x4000 - (player ? player->entity.node.rotation[1] : 0));
    if (static_cast<std::uint16_t>(yaw + FlagInverseAtan(x, z)) > 0x8000U)
        return false;
    Vector3 view{};
    Matrix4x4::TransformPoint(point, view);
    if (!(-0.1 > static_cast<double>(view.z)))
        return false;
    const std::int32_t width = static_cast<std::int32_t>(d3t::g_originalScreenWidth);
    const std::int32_t height = static_cast<std::int32_t>(d3t::g_originalScreenHeight);
    PushProjectionPrePass(static_cast<float>(width), static_cast<float>(height));
    Vector2 screen{};
    PushTransform(point, screen);
    if (!(screen.x > 0.0f) || !(static_cast<float>(width) > screen.x) || !(screen.y > 0.0f) || !(static_cast<float>(height) > screen.y))
        return false;
    flddToggleFlag(0x1000);
    return !flddFindIntersections(point, bone);
}

bool flddCheckCharaIntersection(const Vector3 &point, shen_name chara, std::int16_t halfAngle, float radius) {
    Character *const character = FlagChara(chara);
    if (!character)
        return false;
    flagLocation = point;
    const Vector3 bone = FlagCharaBone(character);
    if (!(2.0f > std::fabs(point.y - bone.y)))
        return false;
    const float x = point.x - bone.x;
    const float z = point.z - bone.z;
    if (!(radius * radius > z * z + x * x))
        return false;
    const std::uint16_t yaw = static_cast<std::uint16_t>(halfAngle - character->entity.node.rotation[1]);
    if (static_cast<std::uint16_t>(2 * halfAngle) < static_cast<std::uint16_t>(yaw + FlagInverseAtan(x, z)))
        return false;
    flddToggleFlag(0x1000);
    return !flddFindIntersections(point, bone);
}


void flagSetLocationToChara(shen_name chara) {
    if (Character *const character = FlagChara(chara))
        flagLocation = GetCharacterWorldPosition(character);
}

bool flagTransformCharaOffset(shen_name chara, const Vector3 &offset, Vector3 &point) {
    Character *const character = FlagChara(chara);
    if (!character)
        return false;
    const Vector3 position = GetCharacterWorldPosition(character);
    const std::int16_t yaw = GetAllAngYForNode(&character->entity.node);
    Matrix4x4_PushActiveTransform();
    Matrix4x4::preTranslate(position.x, position.y, position.z);
    Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
    Matrix4x4::TransformPoint(offset, point);
    Matrix4x4::Pop(1);
    return true;
}

// Registers an action-button record as a nearest-trigger candidate for this evaluation.
bool flagRegisterCandidate(FlagRecord *record) {
    Character *const player = FlagPlayer();
    if (shenPlayerData.queryId != 0 || !player)
        return true;
    const Vector3 position = GetCharacterWorldPosition(player);
    const float x = position.x - flagLocation.x;
    const float z = position.z - flagLocation.z;
    if (flagCandidateCount < 10) {
        FlagCandidate &candidate = flagCandidates[static_cast<std::size_t>(flagCandidateCount++)];
        candidate.distanceSquared = z * z + x * x;
        candidate.record = record;
    }
    return record == selectedFlagCandidate;
}

// Walks up to three records after a 0x000e020e action trigger and always
// reports false.
bool flagIsChainedTrigger(FlagRecord *record) {
    if (*reinterpret_cast<const std::int32_t *>(record) != 0x000e020e)
        return false;
    bool walking = true;
    for (std::int32_t count = 0; walking;) {
        record = FlagBranch(record, true);
        ++count;
        walking = record->opcode != 0x18 && count <= 3;
    }
    return false;
}

void flagCopySystemManageSource(const char *source) {
    std::size_t index = 0;
    do
        systemManageSource[index] = source[index];
    while (source[index++] != '\0');
}

std::int32_t shenLoadScene(std::uint32_t slot, std::uintptr_t argument) {
    flagCopySystemManageSource(GetAreaSlotName(slot));
    systemManageFrames = 0;
    FinishLoading();
    srchReset();
    shenToggleAreaFlags(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    shenPlayerData.requiredRecord = shenPlayerData.triggerRecord;
    shenPlayerData.requestedHelpTopic = 0;
    shenPlayerData.helpTopic = 0;
    shenPlayerData.freeQuestActive = 1;
    flagCameraFrame = 0;
    return ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(slot), argument) ? 1 : 0;
}

std::int32_t shenLoadScene2(const char *areaId, std::uintptr_t argument) {
    if (!LoadAreaFunc_Wrapper_0(areaId, 0))
        return 0;
    flagCopySystemManageSource(areaId);
    systemManageFrames = 0;
    FinishLoading();
    srchReset();
    shenToggleAreaFlags(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    shenPlayerData.requiredRecord = shenPlayerData.triggerRecord;
    shenPlayerData.requestedHelpTopic = 0;
    shenPlayerData.helpTopic = 0;
    shenPlayerData.freeQuestActive = 1;
    flagCameraFrame = 0;
    return ExecuteAreaMainFunction(areaId, argument) ? 1 : 0;
}

FlagRecord *shenMoveToFreeQuest() {
    FinishLoading();
    srchReset();
    shenToggleAreaFlags(0);
    EV_EventSetUpHelpIcon_0(0, 0);
    shenPlayerData.freeQuestActive = 1;
    shenPlayerData.requestedHelpTopic = 0;
    shenPlayerData.helpTopic = 0;
    flagCameraFrame = 0;
    shenPlayerData.requiredRecord = shenPlayerData.triggerRecord;
    return shenPlayerData.triggerRecord;
}

void SetHelpIconTopic(std::int16_t topic) {
    if (shenPlayerData.requestedHelpTopic != 0) {
        if (shenPlayerData.requestedHelpTopic == shenPlayerData.helpTopic)
            return;
        EV_EventSetUpHelpIcon_0(static_cast<std::uint8_t>(topic), 0);
        shenPlayerData.flags |= ShenPlayerFlag_HelpIconShown;
        shenPlayerData.helpTopic = shenPlayerData.requestedHelpTopic;
    } else if (shenPlayerData.helpTopic != 0) {
        EV_EventSetUpHelpIcon_0(0, 0);
        shenPlayerData.requestedHelpTopic = 0;
        shenPlayerData.helpTopic = 0;
    }
}

// Publishes the chara-relative search-target positions of the FLAG context.
std::int32_t shenUpdateFlagTargets(SearchTarget *targets) {
    std::int32_t count = 0;
    if (targets->name.name == 0)
        return count;
    for (SearchTarget *target = targets; target->name.name != 0; ++target, ++count) {
        const std::uint32_t mode = target->flags & 0xffff0000U;
        if (mode == 0x80010000U) {
            const std::int16_t offsetIndex = target->yaw;
            Character *const character = FlagChara(shen_name{target->field08});
            if (!character) {
                target->flags = 2;
                continue;
            }
            const Vector3 position = GetCharacterWorldPosition(character);
            const std::int16_t yaw = GetAllAngYForNode(&character->entity.node);
            const FlagTargetOffset &offset = flagTargetOffsets[static_cast<std::size_t>(offsetIndex)];
            Matrix4x4_PushActiveTransform();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
            Matrix4x4::TransformPoint(offset.position, target->position);
            Matrix4x4::Pop(1);
            target->flags = 0;
            target->field08 = 0;
            target->yaw = static_cast<std::int16_t>(offset.yaw + yaw);
            target->lockonAngle = flagTargetOffsets[0].lockonAngle;
            target->yawLimit = offset.yawLimit;
        } else if (mode == 0x80020000U) {
            Character *const character = FlagChara(shen_name{target->field08});
            if (!character) {
                target->flags = 2;
                continue;
            }
            const Vector3 position = GetCharacterWorldPosition(character);
            const std::int16_t yaw = GetAllAngYForNode(&character->entity.node);
            const Vector3 local = target->position;
            Matrix4x4_PushActiveTransform();
            Matrix4x4::preTranslate(position.x, position.y, position.z);
            Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(yaw));
            Matrix4x4::TransformPoint(local, target->position);
            Matrix4x4::Pop(1);
            target->yaw = static_cast<std::int16_t>(target->yaw + yaw);
            target->flags = 0;
            target->field08 = 0;
            target->yawLimit = flagTargetOffsets[4].yawLimit;
        } else if (mode == 0x80030000U) {
            target->flags = 0;
            target->field08 = 0;
        }
    }
    return count;
}

// Evaluates the context record lists selected by flagRelated.
void shenEvaluateFlagContexts() {
    for (std::size_t index = 0; flagContextRecords[index]; ++index) {
        shenPlayerData.record = flagContextRecords[index];
        shenEvaluateFlag(shenPlayerData, 0);
    }
}

std::int32_t stepFindSleepSchedule(std::int32_t flag) {
    std::int32_t index = 0;
    for (const SleepScheduleEntry *entry = sleepScheduleEntries.data(); entry->area.name != 0; ++entry, ++index) {
        if (entry->flag == flag)
            return GetShenmueFlag(entry->flag) ? index : -1;
    }
    return -1;
}

void stepSetDefaultStats() {
    for (std::int16_t stat = 16; stat <= 31; ++stat)
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, stat, 85);
}

void stepSelectSleepSchedule(const std::int32_t *flags) {
    const std::int32_t lateSleepWindow = GetShenmueFlag(528);
    GetCurrentStep();
    if (g_actNumber == 1 && !GetShenmueFlag(179)) {
        stepEvaluateFlagStepRange(179, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 2 && !GetShenmueFlag(90)) {
        stepEvaluateFlagStepRange(90, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 3 && !GetShenmueFlag(91)) {
        stepEvaluateFlagStepRange(91, StepRangeMode_ApplyAndSetStep);
    } else if (g_actNumber == 4 && !GetShenmueFlag(110)) {
        stepEvaluateFlagStepRange(110, StepRangeMode_ApplyAndSetStep);
    }

    std::int32_t index = -1;
    for (std::int8_t argument = 0; index < 0 && flags[argument] != 0; ++argument)
        index = stepFindSleepSchedule(flags[argument]);
    if (index < 0) {
        if (static_cast<std::int32_t>(GetCurrentStep()) >= 700000) {
            plymSetSceneContext(5940, 5970, static_cast<std::int16_t>(g_actNumber), SHEN_NAME(KRH1), 50);
            return;
        }
        shenPlayerData.sleepScheduleFlag = -1;
        index = 0;
    }
    const SleepScheduleEntry &entry = sleepScheduleEntries[static_cast<std::size_t>(index)];
    PlayerTargetDate target{};
    BuildTargetDateFromFrameClock(target);
    std::int16_t start = entry.holdWindowStartMinute;
    std::int16_t end = entry.holdWindowEndMinute;
    if (lateSleepWindow && (entry.kind & 0xff00U) == 0x100U) {
        end = 1680;
        start = 1620;
    }
    plymSetSceneContext(start, end, static_cast<std::int16_t>(g_actNumber), entry.area, entry.entryId);
    if (target.dateTime.minute + 60 * target.dateTime.hour < entry.holdWindowEndMinute || GetInternalFlag(1010)) {
        EV_setHoldStatesFlags(2);
        EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, 1010, 0);
    }
    shenPlayerData.sleepScheduleFlag = entry.flag;
}

std::int32_t stepRandomizeStats(const std::int32_t *arguments) {
    std::int32_t consumed = 1;
    for (const std::int32_t *stat = arguments + 1; *stat != -1; ++stat, ++consumed) {
        const float random = IncrementSharedPrngFloat();
        EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, static_cast<std::int16_t>(*stat), static_cast<std::int32_t>(random * static_cast<float>(arguments[0])));
    }
    return consumed + 1;
}

std::int32_t stepUpdateCurfewFlag(const std::int32_t *flags) {
    shenPlayerData.flags &= ~ShenPlayerFlag_CurfewReached;
    std::int32_t consumed = 0;
    for (const std::int32_t *flag = flags; *flag != -1; ++flag, ++consumed) {
        if (GetShenmueFlag(*flag))
            shenPlayerData.flags |= ShenPlayerFlag_CurfewReached;
    }
    return consumed + 1;
}

// Returns the argument dwords consumed after the extended command.
std::int32_t stepExecuteExtendedCommand(std::int32_t operation, std::int32_t *arguments) {
    switch (static_cast<StepFlagExtendedOp>(operation)) {
    case StepFlagExtendedOp::ScheduleFlagCallback:
        plymSetCallback(shenRestartPlayerDataFlagEvaluation);
        plymScheduleCycleFlagCallback(0);
        return 0;
    case StepFlagExtendedOp::Skip3:
        return 3;
    case StepFlagExtendedOp::SetDefaultStats:
        stepSetDefaultStats();
        return 0;
    case StepFlagExtendedOp::SelectSleepSchedule:
        stepSelectSleepSchedule(arguments);
        return 0;
    case StepFlagExtendedOp::RandomizeStats:
        return stepRandomizeStats(arguments);
    case StepFlagExtendedOp::UpdateCurfewFlag:
        return stepUpdateCurfewFlag(arguments);
    default:
        return 0;
    }
}

} // namespace


std::uint32_t flagDrawDebugQuad(const Vector3 &first, const Vector3 &second, const Vector3 &third, const Vector3 &fourth) {
    Vector3 camera{}, normal{};
    camCtrlGetNextCamPos(&camera);
    for (std::size_t offset : {2U, 10U})
        std::memcpy(flagDebugModelCommands.data() + offset, &camera, sizeof(camera));
    const std::uint16_t angle = static_cast<std::uint16_t>(g_rootTaskIterationCount << 10U);
    const float pulse = FixedAngleSin(angle) * 0.25f + 0.5f;
    flagDebugModelCommands[17] = std::bit_cast<std::uint32_t>(0.5f);
    flagDebugModelCommands[18] = 0;
    flagDebugModelCommands[19] = std::bit_cast<std::uint32_t>(pulse);
    flagDebugModelCommands[20] = std::bit_cast<std::uint32_t>(pulse);
    ComputeTriangleNormal(first, second, third, normal);
    const Vector3 *vertices[]{&first, &third, &second, &fourth};
    for (std::size_t index = 0; index < 4; ++index) {
        std::uint32_t *const vertex = flagDebugModelCommands.data() + 28 + index * 8;
        std::memcpy(vertex, vertices[index], sizeof(Vector3));
        std::memcpy(vertex + 3, &normal, sizeof(normal));
        vertex[0] |= 1U;
    }
    return SubmitLegacyFlagDebugQuadCompatibility(std::as_bytes(std::span(flagDebugModelCommands)));
}

// @verified - p
std::uint32_t flagDrawDebugCylinder(const Vector3 &location, float radius) {
    Character *const player = FlagPlayer();
    const Vector3 playerPosition = player ? GetCharacterWorldPosition(player) : Vector3{-1000.0f, -1000.0f, -1000.0f};
    (void)playerPosition;
    Vector3 centre = location;
    std::uint32_t result{};
    for (std::uint16_t index = 0; index < 128; index += 16) {
        const std::uint16_t angle = static_cast<std::uint16_t>((index + g_rootTaskIterationCount) << 9U);
        Vector3 first{FixedAngleSin(angle) * radius + location.x, location.y - 100.0f, FixedAngleCos(angle) * radius + location.z};
        Vector3 second{FixedAngleSin(static_cast<std::uint16_t>(angle + 4096)) * radius + location.x, location.y + 200.0f,
            FixedAngleCos(static_cast<std::uint16_t>(angle + 4096)) * radius + location.z};
        Vector3 third{FixedAngleSin(static_cast<std::uint16_t>(angle + 8192)) * radius + location.x, location.y + 200.0f,
            FixedAngleCos(static_cast<std::uint16_t>(angle + 8192)) * radius + location.z};
        flagDrawDebugQuad({first.x, second.y, first.z}, first, second, {second.x, first.y, second.z});
        second.y = location.y - 100.0f;
        flagDrawDebugQuad({second.x, third.y, second.z}, second, third, {third.x, second.y, third.z});
        centre.y = location.y + 1.0f;
        first.y = second.y = third.y = centre.y;
        centre.y = shenFindIntersectionWithScene(centre, centre.y) + 0.1f;
        first.y = shenFindIntersectionWithScene(first, centre.y) + 0.1f;
        second.y = shenFindIntersectionWithScene(second, first.y) + 0.1f;
        third.y = shenFindIntersectionWithScene(third, second.y) + 0.1f;
        result = flagDrawDebugQuad(centre, first, second, third);
    }
    return result;
}

// @verified - p
std::uint32_t flagDrawDebugBox(float firstX, float firstZ, float secondX, float secondZ, float y) {
    Vector3 first{firstX, y + 1.0f, firstZ}, second{firstX, y + 1.0f, secondZ};
    Vector3 third{secondX, y + 1.0f, secondZ}, fourth{secondX, y + 1.0f, firstZ};
    flagDrawDebugQuad({firstX, y - 5.0f, secondZ}, {firstX, y + 200.0f, secondZ}, {firstX, y - 5.0f, firstZ}, {firstX, y + 200.0f, firstZ});
    flagDrawDebugQuad({secondX, y - 5.0f, secondZ}, {secondX, y + 200.0f, secondZ}, {firstX, y - 5.0f, secondZ}, {firstX, y + 200.0f, secondZ});
    flagDrawDebugQuad({secondX, y - 5.0f, firstZ}, {secondX, y + 200.0f, firstZ}, {secondX, y - 5.0f, secondZ}, {secondX, y + 200.0f, secondZ});
    flagDrawDebugQuad({firstX, y - 5.0f, firstZ}, {firstX, y + 200.0f, firstZ}, {secondX, y - 5.0f, firstZ}, {secondX, y + 200.0f, firstZ});
    first.y = shenFindIntersectionWithScene(first, firstX) + 0.1f;
    second.y = shenFindIntersectionWithScene(second, first.y) + 0.1f;
    third.y = shenFindIntersectionWithScene(third, second.y) + 0.1f;
    fourth.y = shenFindIntersectionWithScene(fourth, third.y) + 0.1f;
    return flagDrawDebugQuad(first, second, third, fourth);
}

// @verified - p
void nullsub(...) noexcept {
}

// @verified - p
void nullsub(void *) noexcept {
}

bool ComputeAngle(const Vector3 &origin, float radius, std::int16_t yaw, std::int32_t halfAngle, const Vector3 &point) {
    const Vector3 delta{point.x - origin.x, point.y - origin.y, point.z - origin.z};
    if (std::sqrt((delta.x * delta.x + delta.y * delta.y) + delta.z * delta.z) > radius)
        return false;
    std::uint16_t difference = static_cast<std::uint16_t>(yaw - FlagInverseAtan(delta.x, delta.z));
    if (difference >= 0x8000U)
        difference = static_cast<std::uint16_t>(-difference);
    return static_cast<std::int32_t>(difference) < halfAngle;
}

std::int32_t GetShenmueFlag(std::int32_t flag) {
    std::int8_t result = 0;
    std::uint8_t state = 2;
    const std::int32_t *command = FindFlagDefinition(flag) + 2;
    for (;;) {
        const std::uint32_t instruction = static_cast<std::uint32_t>(*command++);
        const std::int32_t operand = static_cast<std::int32_t>(instruction & 0xffffffU);
        switch (static_cast<ShenFlagOp>(instruction >> 24)) {
        case ShenFlagOp::End:
            return result;
        case ShenFlagOp::StepAtLeastToggleA:
        case ShenFlagOp::StepAtLeastToggleB:
            if (static_cast<std::int32_t>(GetCurrentStep()) >= operand)
                state ^= 2;
            break;
        case ShenFlagOp::RequireInternalFlagSet:
            if (!GetInternalFlag(static_cast<std::int16_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp::RequireInternalFlagClear:
            if (GetInternalFlag(static_cast<std::int16_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp::CommitGroup:
            if (state == 0)
                result = 1;
            state &= 0x0e;
            break;
        case ShenFlagOp::ResetGroup:
            if (state == 0)
                result = 1;
            state = 2;
            break;
        case ShenFlagOp::ToggleMode:
            state ^= 2;
            break;
        case ShenFlagOp::RequireFlag:
            if (!GetShenmueFlag(operand))
                state |= 1;
            break;
        case ShenFlagOp::RequireCalmWind:
            state |= operand != 1 || !(0.25f > GetWeatherWindVector()) ? 1 : 0;
            break;
        case ShenFlagOp::RequireTimeInRange: {
            const std::uint32_t range = static_cast<std::uint32_t>(*command++);
            if (!IsCurrentTimeRange(static_cast<std::uint16_t>(range), static_cast<std::uint16_t>(range >> 16)))
                state |= 1;
            break;
        }
        case ShenFlagOp::RequireItem:
            if (!GetItemState(static_cast<std::uint32_t>(operand)))
                state |= 1;
            break;
        case ShenFlagOp::RequireStatAtLeast:
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(operand >> 16)) < static_cast<std::int16_t>(operand))
                state |= 1;
            break;
        case ShenFlagOp::RequireStatBelow:
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, static_cast<std::int16_t>(operand >> 16)) >= static_cast<std::int16_t>(operand))
                state |= 1;
            break;
        default:
            break;
        }
    }
}

std::int32_t EV_GetShenmueFlag(std::int32_t flag) {
    return GetShenmueFlag(flag);
}

std::uint32_t stepUpdateFlagUtil(std::uint8_t mode, std::int32_t flag) {
    std::uint32_t result = 0;
    std::int32_t *const definition = FindFlagDefinition(flag);
    if (!definition || mode >= reinterpret_cast<const FlagDefinition *>(definition)->stepModeCount)
        return 0;
    std::int32_t *command = definition + definition[definition[0] + mode];
    for (;;) {
        const std::uint32_t instruction = static_cast<std::uint32_t>(*command++);
        const std::int32_t operand = static_cast<std::int32_t>(instruction & 0xffffffU);
        switch (static_cast<StepFlagOp>(instruction >> 24)) {
        case StepFlagOp::End:
            return result;
        case StepFlagOp::SetInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 1);
            break;
        case StepFlagOp::ClearInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 0);
            break;
        case StepFlagOp::SetStep:
            SetSTEP(operand);
            result |= 1;
            break;
        case StepFlagOp::RaiseStep:
            if (GetCurrentStep() < static_cast<std::uint32_t>(operand)) {
                SetSTEP(operand);
                result |= 1;
            }
            break;
        case StepFlagOp::ApplyInvertedFlagSteps:
            stepUpdateFlagUtil(static_cast<std::uint8_t>(GetShenmueFlag(operand) ^ 1), operand);
            break;
        case StepFlagOp::Extended:
            command += stepExecuteExtendedCommand(operand, command);
            break;
        case StepFlagOp::ClearStat:
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, static_cast<std::int16_t>(operand), 0);
            break;
        case StepFlagOp::AddStat: {
            const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
            std::int32_t value = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, stat)) + static_cast<std::int16_t>(operand);
            if (value > 255)
                value = 255;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, stat, value);
            break;
        }
        case StepFlagOp::SubtractStat: {
            const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
            std::int32_t value = static_cast<std::int32_t>(EV_GetPlayerFlag(PlayerFlagReadMode::Stat, stat)) - static_cast<std::int16_t>(operand);
            if (value < 0)
                value = 0;
            EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, stat, value);
            break;
        }
        default:
            break;
        }
    }
}

std::int32_t stepEvaluateFlagStepRange(std::int32_t flag, std::uint32_t mode) {
    std::int32_t maximumStep = 0;
    std::int32_t minimumStep = 3000000;
    const std::int32_t *const definition = FindFlagDefinition(flag);
    if (!definition)
        return -1;
    for (const std::int32_t *command = definition + 2;;) {
        const std::uint32_t instruction = static_cast<std::uint32_t>(*command++);
        const std::int32_t operand = static_cast<std::int32_t>(instruction & 0xffffffU);
        switch (static_cast<StepRangeOp>(instruction >> 24)) {
        case StepRangeOp::End:
        case StepRangeOp::EndGroup:
        case StepRangeOp::EndGroupB:
        case StepRangeOp::EndGroupC:
            if (EV_GetPlayerFlag(PlayerFlagReadMode::Internal, 1013) != 0)
                minimumStep = 100000;
            if (mode == StepRangeMode_Minimum)
                return minimumStep;
            if (mode == StepRangeMode_Maximum)
                return maximumStep;
            if (mode == StepRangeMode_ApplyAndSetStep)
                SetSTEP(minimumStep);
            return 0;
        case StepRangeOp::MinimumStep:
            if (operand < minimumStep)
                minimumStep = operand;
            break;
        case StepRangeOp::MaximumStep:
            if (maximumStep < operand)
                maximumStep = operand;
            break;
        case StepRangeOp::SetInternalFlag:
            if ((mode & StepRangeMode_ApplyFlags) != 0)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 1);
            break;
        case StepRangeOp::ClearInternalFlag:
            if ((mode & StepRangeMode_ApplyFlags) != 0)
                EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(operand), 0);
            break;
        case StepRangeOp::ResetRange:
            minimumStep = 0;
            maximumStep = 0;
            break;
        case StepRangeOp::Include:
            stepEvaluateFlagStepRange(operand, mode);
            break;
        case StepRangeOp::RaiseStat:
            if ((mode & StepRangeMode_ApplyFlags) != 0) {
                const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
                const std::int32_t value = operand & 0xff;
                if (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, stat) < value)
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, stat, value);
            }
            break;
        case StepRangeOp::LowerStat:
            if ((mode & StepRangeMode_ApplyFlags) != 0) {
                const std::int16_t stat = static_cast<std::int16_t>(operand >> 16);
                const std::int32_t value = operand & 0xff;
                if (EV_GetPlayerFlag(PlayerFlagReadMode::Stat, stat) >= value && value != 0)
                    EV_SetPlayerFlag(PlayerFlagWriteMode::Stat, stat, value - 1);
            }
            break;
        default:
            break;
        }
    }
}

std::int32_t stepScheduleFlagCallback() {
    for (const FlagCallbackEntry *entry = flagCallbackEntries.data(); entry->flag != 0; ++entry) {
        if (!GetShenmueFlag(entry->flag))
            continue;
        const std::int32_t result = plymScheduleOneShotFlagCallback(static_cast<std::int16_t>(entry->minute));
        shenPlayerData.callbackContext = callbackContextSelf;
        return result;
    }
    const std::int32_t result = GetShenmueFlag(277);
    if (result != 0)
        return result;
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    return plymScheduleCycleFlagCallback(0);
}

void EV_stepSetFlags(std::uint8_t mode, std::int32_t flag) {
    if (flag != 0 && (stepUpdateFlagUtil(mode, flag) & 1U) != 0)
        stepUpdateFlagUtil(0, 200);
    stepUpdateFlagUtil(0, 500);
    SetWeatherShouldUpdateDay(GetInternalFlag(1001) ? 1 : 0);
    stepScheduleFlagCallback();
    if (flag == 100)
        EV_ReconfigureNpcRoutesForStoryStep();
}

std::int32_t flagInitContext(ShenPlayerData &data, shen_name context) {
    data.loadingPhase = 0;
    data.flags = 0;
    data.fadeHoldFrames = 0;
    data.updateCount = 0;
    data.messageFrames = 0;
    data.deferredRecord = nullptr;
    data.triggerRecord = nullptr;
    data.requiredRecord = nullptr;
    data.targets = nullptr;
    data.idleEvaluations = 3;
    data.voice = nullptr;
    data.callbackContext = {};
    data.helpTopic = 0;
    data.callDepth = 0;
    data.record = nullptr;
    std::int32_t *const table = FlagContextTable();
    const FlagContextEntry *entry = FindFlagContext(context);
    data.firstRecord = entry ? FlagRecordAt(table, entry->recordList) : nullptr;
    if (!data.firstRecord)
        return 0;
    data.context = context;
    data.record = data.firstRecord;
    entry = FindFlagContext(context);
    data.targets = entry ? reinterpret_cast<SearchTarget *>(table + entry->targetTable) : nullptr;
    if (plymConsumeFlags8())
        data.flags |= ShenPlayerFlag_CurfewSceneChange;
    selectedFlagCandidate = nullptr;
    return 1;
}

std::int32_t flagRelated(shen_name map) {
    std::size_t count = 0;
    const std::int32_t initialized = flagInitContext(shenPlayerData, map);
    shenPlayerData.callbackContext = callbackContextCycle;
    stepScheduleFlagCallback();
    if (!initialized)
        return initialized;
    shenPlayerData.flags |= ShenPlayerFlag_Active;
    srchSetTarget(1, shenPlayerData.targets);
    for (const FlagContextSelector *selector = flagContextSelectors.data(); selector->context.name != 0; ++selector) {
        if (!GetShenmueFlag(selector->flag))
            continue;
        const FlagContextEntry *const entry = FindFlagContext(selector->context);
        flagContextRecords[count++] = entry ? FlagRecordAt(FlagContextTable(), entry->recordList) : nullptr;
    }
    flagContextRecords[count] = nullptr;
    return initialized;
}

std::int32_t shenUpdatePlayerDataFlags(std::int32_t active) {
    const std::int32_t previous = (shenPlayerData.flags & ShenPlayerFlag_Active) != 0;
    if (active != 0)
        shenPlayerData.flags |= ShenPlayerFlag_Active;
    else
        shenPlayerData.flags &= ~ShenPlayerFlag_Active;
    return previous;
}

std::int64_t shenRestartPlayerDataFlagEvaluation() {
    ShenPlayerData context;
    flagInitContext(context, shenPlayerData.callbackContext);
    shenPlayerData.record = context.record;
    shenPlayerData.callDepth = context.callDepth;
    return shenEvaluateFlag(shenPlayerData, 0);
}

std::int32_t shenScheduleFlagCallback(std::int16_t minute) {
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    return plymScheduleCycleFlagCallback(minute);
}

std::int32_t npcSetCycleTimeLimitExpiry(shen_name, std::int16_t timeLimit) {
    if (timeLimit == 0) {
        plymSetCallback(shenRestartPlayerDataFlagEvaluation);
        return plymScheduleCycleFlagCallback(0);
    }
    DateTime dateTime{};
    GetCurrentDateTime(dateTime);
    const std::int16_t minute = static_cast<std::int16_t>(dateTime.hour * 60 + dateTime.minute + timeLimit);
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    return plymScheduleCycleFlagCallback(minute);
}

SearchTarget *shenFindFlagTarget(shen_name id) {
    SearchTarget *target = shenPlayerData.targets;
    if (!target || target->name.name == 0)
        return nullptr;
    for (; target->name.name != id.name; ++target) {
        if (target[1].name.name == 0)
            return nullptr;
    }
    return target;
}

std::int32_t ryoIsCurrentScenarioFlagSatisfied() {
    const FlagRecord *const record = shenPlayerData.requiredRecord;
    if (record->opcode == static_cast<std::uint8_t>(FlagOp::IfPlayerNearPoint))
        return flagIsPlayerNearPoint(record);
    if (record->opcode == static_cast<std::uint8_t>(FlagOp::IfPlayerInRect))
        return ryoCheckInRangeForFlag(record);
    return 1;
}

std::int32_t shenGetMinutesUntilNextFrameMinute() {
    if ((shenPlayerData.flags & ShenPlayerFlag_CurfewReached) != 0)
        return 0;
    if (shenPlayerData.sleepScheduleFlag == 670)
        return 999;
    DateTime now{};
    GetCurrentDateTime(now);
    PlayerTargetDate target{};
    BuildTargetDateFromFrameClock(target);
    DateTime_AddMinutesInPlace(target.dateTime, 1);
    const std::int32_t minutes = DiffMinutes(target.dateTime, now);
    return minutes < 0 ? 0 : minutes;
}

std::int16_t shenGetSleepScheduleFlag() {
    return shenPlayerData.sleepScheduleFlag;
}

std::uint32_t shenIsCurfewSceneChange() {
    return shenPlayerData.flags & ShenPlayerFlag_CurfewSceneChange;
}

void shenSetFreeQuestInactive() {
    shenPlayerData.freeQuestActive = 0;
}

std::uint32_t GetLoadingPhaseFlags() {
    return shenPlayerData.loadingPhase;
}

void SetLoadingPhaseFlags(std::uint32_t flags) {
    shenPlayerData.loadingPhase = flags;
}

std::int32_t shenSetPlayerDataFields(std::int32_t queryId, std::int8_t queryArgument) {
    if (!shenPlayerData.record)
        return 0;
    shenPlayerData.queryId = queryId;
    shenPlayerData.queryArgument = queryArgument;
    return shenEvaluateFlag(shenPlayerData, 2);
}

std::int32_t shenEvaluateFlagFor(std::int32_t queryId, std::int8_t queryArgument) {
    if (!shenPlayerData.record)
        return 0;
    shenPlayerData.queryId = queryId;
    shenPlayerData.queryArgument = queryArgument;
    return shenEvaluateFlag(shenPlayerData, 3);
}

std::int32_t shenUpdateFlagEvaluation(std::int32_t mode) {
    ++shenPlayerData.updateCount;
    std::uint32_t flags = shenPlayerData.flags;
    flagCandidateCount = 0;
    shenPlayerData.queryId = 0;
    shenPlayerData.queryArgument = 0;
    if ((flags & ShenPlayerFlag_Active) == 0 || !shenPlayerData.record)
        return 0;
    if (mode == 1) {
        shenUpdateFlagTargets(shenPlayerData.targets);
        flags = shenPlayerData.flags;
    }
    if ((flags & ShenPlayerFlag_FadeHold) != 0 && --shenPlayerData.fadeHoldFrames == 0) {
        flags &= ~ShenPlayerFlag_FadeHold;
        shenPlayerData.flags = flags;
    }
    if ((flags & ShenPlayerFlag_EventMessageShown) != 0) {
        if (--shenPlayerData.messageFrames == 0) {
            scenDestroySubtitles(static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask))->scenarioTask);
            shenPlayerData.flags &= ~ShenPlayerFlag_EventMessageShown;
        }
        return 0;
    }
    if ((flags & ShenPlayerFlag_SearchReleasePending) != 0) {
        if (srchHasActiveLockonTarget())
            return 0;
        flags = shenPlayerData.flags & ~ShenPlayerFlag_SearchReleasePending;
        shenPlayerData.flags = flags;
    }
    if ((flags & ShenPlayerFlag_DetachedVoicePending) != 0)
        return 0;
    if ((flags & ShenPlayerFlag_VoicePending) != 0) {
        if (!cvsHasValidConverseData(shenPlayerData.voice)) {
            std::array<std::int32_t, 2> bank{shenPlayerData.record->argument[0], 0};
            EV_FreeAuthorPak(0, reinterpret_cast<const char *>(bank.data()));
            FlagRecord *const record = shenPlayerData.record;
            shenPlayerData.flags &= ~ShenPlayerFlag_VoicePending;
            shenPlayerData.record = FlagBranch(record, true);
        }
        return 0;
    }
    if ((flags & ShenPlayerFlag_PackLoadPending) != 0) {
        if (PakLoadWait() || !plymGetSpecialFlag() || flagCameraFrame != 0)
            return 1;
        flags = (shenPlayerData.flags & ~ShenPlayerFlag_PackLoadPending) | ShenPlayerFlag_PackExecutePending;
        shenPlayerData.flags = flags;
    }
    if ((flags & ShenPlayerFlag_PackExecutePending) != 0) {
        shenPlayerData.flags = flags & ~ShenPlayerFlag_PackExecutePending;
        shenMoveToFreeQuest();
        FlagRecord *record = shenPlayerData.record;
        const std::uintptr_t argument = record->opcode == static_cast<std::uint8_t>(FlagOp::DeferEventPackNext) ?
            reinterpret_cast<std::uintptr_t>(&record->argument[4]) :
            static_cast<std::uintptr_t>(static_cast<std::intptr_t>(record->argument[4]));
        ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(record->argument[3]), argument);
        record = shenPlayerData.record;
        if (record->opcode == static_cast<std::uint8_t>(FlagOp::DeferEventPackRestartValue)) {
            shenPlayerData.record = shenPlayerData.firstRecord;
            shenPlayerData.callDepth = 0;
            return 1;
        }
        shenPlayerData.record = FlagBranch(record, true);
        return 1;
    }
    if ((flags & ShenPlayerFlag_NpcReleasePending) != 0) {
        if (PrepareNpcAllFromArray())
            shenPlayerData.flags &= ~ShenPlayerFlag_NpcReleasePending;
        return 1;
    }
    if ((flags & ShenPlayerFlag_SpecialPosePending) != 0) {
        if (!plymGetSpecialFlag())
            return 1;
        shenPlayerData.flags &= ~ShenPlayerFlag_SpecialPosePending;
    }
    flagCandidateCount = 0;
    shenPlayerData.requestedHelpTopic = 0;
    if (SearchTarget *target = shenPlayerData.targets; target && target->name.name != 0) {
        for (; target->name.name != 0; ++target) {
            if (static_cast<std::uint16_t>(target->name.name) == 0x4345U)
                target->flags |= 2;
        }
    }
    if (mode == 0)
        shenPlayerData.state &= ~1U;
    const std::int32_t result = shenEvaluateFlag(shenPlayerData, mode);
    if (!result)
        shenEvaluateFlagContexts();
    SetHelpIconTopic(static_cast<std::int16_t>(shenPlayerData.requestedHelpTopic));
    if (flagCandidateCount != 0) {
        float nearest = 9999.0f;
        FlagRecord *selected = flagCandidates[0].record;
        for (std::int32_t index = 0; index < flagCandidateCount; ++index) {
            const FlagCandidate &candidate = flagCandidates[static_cast<std::size_t>(index)];
            if (nearest > candidate.distanceSquared) {
                selected = candidate.record;
                nearest = candidate.distanceSquared;
            }
        }
        selectedFlagCandidate = selected;
    }
    return result;
}

std::int32_t shenEvaluateFlag(ShenPlayerData &data, std::int32_t mode) {
    using enum CameraState;
    std::uint32_t flags = shenPlayerData.flags;
    FlagRecord *record = shenPlayerData.record;
    if ((flags & ShenPlayerFlag_FadeHold) != 0)
        return 1;
    if ((flags & ShenPlayerFlag_WaitTimeAdvance) != 0) {
        FlagRecord *const deadline = shenPlayerData.deadlineRecord;
        const std::uint64_t held = plymGetAreaExitState();
        if (held == 0xffffffffULL)
            return 1;
        if (held == 0) {
            flags = shenPlayerData.flags & ~ShenPlayerFlag_WaitTimeAdvance;
            shenPlayerData.flags = flags;
            shenPlayerData.idleEvaluations = 0;
            record = FlagAdvance(deadline, static_cast<std::uint32_t>(deadline->argument[3]));
        } else {
            if (held == 1) {
                shenPlayerData.flags &= ~ShenPlayerFlag_WaitTimeAdvance;
                data.deferredRecord = nullptr;
                record = FlagAdvance(deadline, static_cast<std::uint32_t>(deadline->argument[2]));
            }
            flags = shenPlayerData.flags;
        }
    }
    if ((flags & ShenPlayerFlag_ActionChoicePending) != 0) {
        if (GetShouldRenderIcons(RenderIconSlot::ActionSelector))
            return 1;
        const std::uint16_t choice = static_cast<std::uint16_t>(shenGetCtrlSpeakIconFlag());
        shenPlayerData.idleEvaluations = 0;
        const std::int32_t offset = choice == static_cast<std::uint32_t>(record->argument[0]) ? record->argument[3] : record->argument[4];
        shenPlayerData.flags &= ~ShenPlayerFlag_ActionChoicePending;
        record = FlagAdvance(record, static_cast<std::uint32_t>(offset));
    }

    std::int8_t result = 0;
    bool actionButtonRegistered = false;
    bool continueEvaluation = true;
    const std::int32_t searchTarget = static_cast<std::int32_t>(srchGetTargetName(0).name);
    Character *const player = FlagPlayer();
    Vector3 playerPosition{-1000.0f, -1000.0f, -1000.0f};
    std::int16_t playerYaw = 0;
    if (player) {
        playerPosition = GetCharacterWorldPosition(player);
        playerYaw = GetAllAngYForNode(&player->entity.node);
    }
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    for (;;) {
        const std::int32_t *const arguments = record->argument;
        switch (static_cast<FlagOp>(record->opcode)) {
        case FlagOp::End:
            if (shenPlayerData.callDepth != 0) {
                continueEvaluation = true;
                record = flagCallStack[static_cast<std::size_t>(--shenPlayerData.callDepth)];
                continue;
            }
            shenPlayerData.record = shenPlayerData.firstRecord;
            shenPlayerData.callDepth = 0;
            if (ryoNotBusy()) {
                data.deferredRecord = nullptr;
                ++shenPlayerData.idleEvaluations;
            }
            return result;
        case FlagOp::IfPlayerNearPoint: {
            if ((g_flagDebugIntersectionDraw & 1U) != 0) {
                flagDrawDebugCylinder(
                    {std::bit_cast<float>(arguments[0]), playerPosition.y, std::bit_cast<float>(arguments[1])}, std::bit_cast<float>(arguments[2]));
            }
            flagLocation = {std::bit_cast<float>(arguments[0]), playerPosition.y, std::bit_cast<float>(arguments[1])};
            const bool condition = flagIsPlayerNearPoint(record);
            if (condition)
                shenPlayerData.triggerRecord = record;
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::IfPlayerInRect: {
            if ((g_flagDebugIntersectionDraw & 1U) != 0) {
                flagDrawDebugBox(std::bit_cast<float>(arguments[0]), std::bit_cast<float>(arguments[1]), std::bit_cast<float>(arguments[2]),
                    std::bit_cast<float>(arguments[3]), playerPosition.y);
            }
            flagLocation = {std::bit_cast<float>(arguments[0]), 0.0f, std::bit_cast<float>(arguments[1])};
            const bool condition = ryoCheckInRangeForFlag(record);
            if (condition)
                shenPlayerData.triggerRecord = record;
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::IfShenmueFlag:
            record = FlagBranch(record, GetShenmueFlag(arguments[0]) != 0);
            break;
        case FlagOp::IfEntryPoint:
        case FlagOp::IfEntryPointSaveScene:
            if (g_entryPoint != arguments[0]) {
                record = FlagBranch(record, false);
                break;
            }
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::IfEntryPointSaveScene))
                SetSceneInfos(g_actNumber, g_currentMap, arguments[1]);
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfCharaSector: {
            const FlagSector &sector = flagSectors[static_cast<std::size_t>(arguments[1])];
            record = FlagBranch(record, DoAngleCheck(sector, shen_name{static_cast<std::uint32_t>(arguments[0])}, playerPosition, playerYaw));
            break;
        }
        case FlagOp::SceneChange:
        case FlagOp::SceneChangeFadeMode9:
        case FlagOp::SceneChangeFadeMode8:
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::SceneChangeFadeMode9))
                scenSetTaskFlags(9);
            else if (record->opcode == static_cast<std::uint8_t>(FlagOp::SceneChangeFadeMode8))
                scenSetTaskFlags(8);
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(2);
            shenQueueChangeModuleAndScene(SystemModule::Game, g_actNumber, shen_name{static_cast<std::uint32_t>(arguments[0])}, arguments[1]);
            return 1;
        case FlagOp::LoadAreaRestart:
        case FlagOp::LoadAreaRestartValue:
        case FlagOp::LoadAreaNext:
        case FlagOp::LoadAreaNextValue: {
            const std::array<std::int32_t, 3> areaId{arguments[0], arguments[1], 0};
            const bool value = record->opcode == static_cast<std::uint8_t>(FlagOp::LoadAreaRestartValue) ||
                record->opcode == static_cast<std::uint8_t>(FlagOp::LoadAreaNextValue);
            const std::uintptr_t argument =
                value ? static_cast<std::uintptr_t>(static_cast<std::intptr_t>(arguments[2])) : reinterpret_cast<std::uintptr_t>(&arguments[2]);
            if (!shenLoadScene2(reinterpret_cast<const char *>(areaId.data()), argument)) {
                data.deferredRecord = nullptr;
                shenPlayerData.record = record;
                return result;
            }
            result = 1;
            if (static_cast<std::uint8_t>(record->opcode - 7U) <= 1U) {
                shenPlayerData.record = shenPlayerData.firstRecord;
                shenPlayerData.callDepth = 0;
                continueEvaluation = false;
            } else {
                record = FlagBranch(record, true);
            }
            break;
        }
        case FlagOp::IfCharaWithinDistance: {
            Character *const character = FlagChara(shen_name{static_cast<std::uint32_t>(arguments[0])});
            if (!character) {
                record = FlagBranch(record, false);
                break;
            }
            Vector3 delta = GetCharacterWorldPosition(character);
            flagLocation = delta;
            delta = {delta.x - playerPosition.x, delta.y - playerPosition.y, delta.z - playerPosition.z};
            record = FlagBranch(record, std::bit_cast<float>(arguments[1]) > std::sqrt((delta.x * delta.x + delta.y * delta.y) + delta.z * delta.z));
            break;
        }
        case FlagOp::SetStepFlags:
            EV_stepSetFlags(static_cast<std::uint8_t>(arguments[1]), arguments[0]);
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfInternalFlagEquals:
            record = FlagBranch(record, static_cast<std::int32_t>(GetInternalFlag(static_cast<std::int16_t>(arguments[0]))) == arguments[1]);
            break;
        case FlagOp::IfEvaluationMode1:
            record = FlagBranch(record, mode == 1);
            break;
        case FlagOp::IfEvaluationMode0:
            record = FlagBranch(record, mode == 0);
            break;
        case FlagOp::IfEvaluationMode2:
            record = FlagBranch(record, mode == 2);
            break;
        case FlagOp::IfEvaluationMode3:
            record = FlagBranch(record, mode == 3);
            break;
        case FlagOp::IfActionButton:
        case FlagOp::IfNamedActionButton:
        case FlagOp::IfActionButtonOrSearch: {
            bool condition = false;
            if (shenGetMinutesUntilNextFrameMinute() == 0) {
                record = FlagBranch(record, condition);
                break;
            }
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::IfActionButtonOrSearch) && searchTarget != 0) {
                record = FlagBranch(record, true);
                break;
            }
            const bool named = record->opcode == static_cast<std::uint8_t>(FlagOp::IfNamedActionButton);
            const shen_name button{named ? static_cast<std::uint32_t>(arguments[0]) : SHEN_NAME(BTEV)};
            const std::uint32_t bindings = GetMediatedButtonBindings(button);
            const bool nearest = flagRegisterCandidate(record);
            if (((searchTarget != 0 && srchHasActiveLockonTarget()) || (nearest && !srchHasActiveLockonTarget())) && bindings != 0 && btnCheck(button) != 0) {
                condition = !flagIsChainedTrigger(record);
            } else if (!actionButtonRegistered) {
                if (named)
                    GetBattleDataConfig(0, button);
                else if (!flagIsChainedTrigger(record))
                    RegisterButtonMediation(button, &flagLocation, (ButtonMediationShowButtonHint | ButtonMediationBindA));
                actionButtonRegistered = true;
            }
            const std::uint32_t argumentCount = named ? 1U : 0U;
            if (record->length >= argumentCount + 2U && (nearest || searchTarget != 0)) {
                shenPlayerData.requestedHelpTopic = static_cast<std::uint16_t>(arguments[argumentCount]);
            }
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::IfRyoNotBusy:
            record = FlagBranch(record, ryoNotBusy());
            break;
        case FlagOp::IfAuthorNotLoaded:
            record = FlagBranch(record, EV_authSceneCtrl(AuthSceneCtrl::IsLoaded) == 0);
            break;
        case FlagOp::LockPlayerControl:
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(1);
            }
            result = 1;
            record = FlagBranch(record, true);
            break;
        case FlagOp::CallContextRecords: {
            flagCallStack[static_cast<std::size_t>(shenPlayerData.callDepth++)] = FlagAdvance(record, 2);
            std::int32_t *const table = FlagContextTable();
            record = FlagRecordAt(table, table[3 * arguments[0] + 1]);
            break;
        }
        case FlagOp::IfPlayerControlEnabled:
            record = FlagBranch(record, plymIsSearchStartRequested());
            break;
        case FlagOp::FadeOutSoundAndHold:
        case FlagOp::FadeOutAndHold: {
            shenPlayerData.flags |= ShenPlayerFlag_FadeHold;
            shenPlayerData.fadeHoldFrames = 30;
            shenPlayerData.record = FlagAdvance(record, record->length);
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(1);
            }
            const ScreenFilmColourRequest request{.start = {0, 0, 0, 0}, .target = {255, 0, 0, 0}, .durationFrames = 30};
            WaitForNextTask(control->screenFilmTask, request, 0);
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::FadeOutSoundAndHold))
                spmFadeOut();
            nullsub();
            return 1;
        }
        case FlagOp::SetInternalFlag:
            EV_SetPlayerFlag(PlayerFlagWriteMode::Internal, static_cast<std::int16_t>(arguments[0]), arguments[1]);
            record = FlagBranch(record, true);
            break;
        case FlagOp::SetTimeOfDay: {
            DateTime dateTime{};
            GetCurrentDateTime(dateTime);
            dateTime.hour = static_cast<std::int8_t>(static_cast<std::uint8_t>(arguments[0]));
            dateTime.minute = static_cast<std::int8_t>(static_cast<std::uint8_t>(arguments[1]));
            SetCurrentDateTimeTo(dateTime);
            record = FlagBranch(record, true);
            break;
        }
        case FlagOp::IfQronStream:
            record = FlagBranch(record, static_cast<std::int32_t>(GetQronNextStream()) == arguments[0]);
            break;
        case FlagOp::LoadPackRestart:
        case FlagOp::LoadPackRestartValue:
        case FlagOp::LoadEventPackRestartValue:
        case FlagOp::DeferEventPackRestartValue:
        case FlagOp::LoadPackNext:
        case FlagOp::LoadPackNextAlt:
        case FlagOp::DeferEventPackNext:
        case FlagOp::DeferEventPackNextValue: {
            const std::array<std::int32_t, 3> areaId{arguments[1], arguments[2], 0};
            const char *const area = reinterpret_cast<const char *>(areaId.data());
            flagCopySystemManageSource(area);
            systemManageFrames = 0;
            const std::uint8_t opcode = record->opcode;
            PakGlobalData *pack = nullptr;
            if (static_cast<std::uint8_t>(opcode - 0x2cU) <= 1U || static_cast<std::uint8_t>(opcode - 0x41U) <= 1U) {
                npcReleaseUnusedStreams();
#if SHENMUE_IS_DC
                void *const destination = reinterpret_cast<void *>(eventExecutionRegions[static_cast<std::size_t>(arguments[3])] - 0x20U);
                pack = LoadMapPackage(Filepath_Gen(arguments[0] != 0 ? FilepathGenMode::PackDirectory : FilepathGenMode::SceneDirectory), area, destination);
#else
                pack = LoadMapPackage(Filepath_Gen(arguments[0] != 0 ? FilepathGenMode::PackDirectory : FilepathGenMode::SceneDirectory), area);
#endif
                if (!pack) {
                    data.deferredRecord = nullptr;
                    shenPlayerData.record = record;
                    return result;
                }
                shenLoadEventFrom(area, static_cast<EventFunctionType>(arguments[3]));
            } else {
                EV_NPC_NewLoadSw(0);
                EV_ResetFreeTalkFlags();
                pack = LoadMapPackage(Filepath_Gen(arguments[0] != 0 ? FilepathGenMode::PackDirectory : FilepathGenMode::SceneDirectory), area);
            }
            if (!pack) {
                data.deferredRecord = nullptr;
                shenPlayerData.record = record;
                return result;
            }
            const std::uint8_t immediate = static_cast<std::uint8_t>(opcode - 0x18U);
            if (immediate > 0x28U || ((0x18000100003ULL >> immediate) & 1U) == 0) {
                EV_NPC_NewLoadSw(0);
                if (plymIsSearchStartRequested()) {
                    SetPlayerControlEnabled(0);
                    shenToggleAreaFlags(1);
                }
                shenPlayerData.flags |= ShenPlayerFlag_PackLoadPending;
                shenPlayerData.pendingPack = pack;
                shenPlayerData.pendingEventType = arguments[3];
                shenPlayerData.record = record;
                continueEvaluation = false;
                result = 1;
                break;
            }
            LoadPak();
            if (arguments[3] == 0) {
                result = 1;
                break;
            }
            shenMoveToFreeQuest();
            const bool value =
                opcode == static_cast<std::uint8_t>(FlagOp::LoadPackRestartValue) || opcode == static_cast<std::uint8_t>(FlagOp::LoadEventPackRestartValue);
            const std::uintptr_t argument =
                value ? static_cast<std::uintptr_t>(static_cast<std::intptr_t>(arguments[4])) : reinterpret_cast<std::uintptr_t>(&arguments[4]);
            ExecuteCurrentAreaMainFunction(static_cast<EventFunctionType>(arguments[3]), argument);
            result = 1;
            if (static_cast<std::uint8_t>(record->opcode - 0x18U) <= 1U || record->opcode == static_cast<std::uint8_t>(FlagOp::LoadEventPackRestartValue)) {
                shenPlayerData.record = shenPlayerData.firstRecord;
                shenPlayerData.callDepth = 0;
                continueEvaluation = false;
            } else {
                record = FlagBranch(record, true);
            }
            break;
        }
        case FlagOp::WarmStartNpcsRelease:
        case FlagOp::WarmStartNpcs:
        case FlagOp::WarmStartHumanNpcs:
            EV_NPC_NewLoadSw(0);
            if (record->length > 1) {
                for (std::uint32_t index = 0; index + 1 < record->length; ++index) {
                    const shen_name name{static_cast<std::uint32_t>(arguments[index])};
                    if (record->opcode == static_cast<std::uint8_t>(FlagOp::WarmStartHumanNpcs))
                        npcWarmStartHumanRoute(name);
                    else
                        EV_CheckWarmStartNpcRoute(name);
                }
            }
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::WarmStartNpcsRelease))
                EV_ResetFreeTalkFlags();
            record = FlagBranch(record, true);
            break;
        case FlagOp::WaitForIdleEvaluations:
            if (shenPlayerData.idleEvaluations > 2 && data.deferredRecord != record) {
                data.deferredRecord = record;
                record = FlagBranch(record, true);
                break;
            }
            shenPlayerData.record = shenPlayerData.firstRecord;
            shenPlayerData.callDepth = 0;
            return result;
        case FlagOp::IfPlayerHeightNear: {
            const float height = std::bit_cast<float>(arguments[0]);
            record = FlagBranch(record, playerPosition.y > height - 1.0f && height + 1.0f > playerPosition.y);
            break;
        }
        case FlagOp::ExecAreaSlotRestart:
        case FlagOp::ExecAreaSlotRestartValue:
        case FlagOp::ExecAreaSlotNext:
        case FlagOp::ExecAreaSlotNextValue: {
            const bool value = record->opcode == static_cast<std::uint8_t>(FlagOp::ExecAreaSlotRestartValue) ||
                record->opcode == static_cast<std::uint8_t>(FlagOp::ExecAreaSlotNextValue);
            const std::uintptr_t argument =
                value ? static_cast<std::uintptr_t>(static_cast<std::intptr_t>(arguments[1])) : reinterpret_cast<std::uintptr_t>(&arguments[1]);
            shenLoadScene(static_cast<std::uint32_t>(arguments[0]), argument);
            result = 1;
            if (static_cast<std::uint8_t>(record->opcode - 0x20U) <= 1U) {
                shenPlayerData.record = shenPlayerData.firstRecord;
                shenPlayerData.callDepth = 0;
                continueEvaluation = false;
            } else {
                record = FlagBranch(record, true);
            }
            break;
        }
        case FlagOp::ShowEventMessage:
            shenPlayerData.flags |= ShenPlayerFlag_EventMessageShown;
            shenPlayerData.messageFrames = 60;
            shenPlayerData.record = FlagAdvance(record, record->length);
            SCEN_ShowEventMessage(control->scenarioTask, 1, reinterpret_cast<const char *>(&arguments[0]));
            return result;
        case FlagOp::IfPlayerIdle:
            record = FlagBranch(record, shenCheckIsBusy());
            break;
        case FlagOp::IfPlayerNotRunning:
            record = FlagBranch(record, !plymIsInterpolationActive());
            break;
        case FlagOp::Repeat:
            break;
        case FlagOp::PlayPlayerMotionF003: {
            MotionWork *const motionWork = FindMotionWork(player);
            SetPlayerControlEnabled(0);
            shenToggleAreaFlags(0);
            mtwkSetRequestedRootMode(motionWork, 1);
            mtwkSetExtraFlag(motionWork, 15, 0);
            mtwkRequestMotion(motionWork, 0xf003, MotionRegion::LowerBody);
            SetMotionFrameTime(motionWork, 16, MotionRegion::LowerBody);
            result = 1;
            record = FlagBranch(record, true);
            break;
        }
        case FlagOp::ReturnToFreeQuest:
            shenMoveToFreeQuest();
            shenToggleAreaFlags(0);
            SetPlayerControlEnabled(1);
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfCashAtLeast:
            record = FlagBranch(record, GetCurrentCash() >= arguments[0]);
            break;
        case FlagOp::WaitAreaSlotUnloaded: {
            const char *const slot = GetAreaSlotName(static_cast<std::uint32_t>(arguments[0]));
            if (slot && GetAreaSlotName(static_cast<std::uint32_t>(arguments[0]))[0] != '\0' && shenPlayerData.freeQuestActive != 0) {
                shenPlayerData.record = record;
                return 1;
            }
            record = FlagBranch(record, true);
            break;
        }
        case FlagOp::IfScenarioRecord:
            flagSetLocationToChara(shen_name{static_cast<std::uint32_t>(arguments[0])});
            record = FlagBranch(record, static_cast<std::int32_t>(GetActiveScenarioCharacter().name) == arguments[0]);
            break;
        case FlagOp::IfItemOwned:
            record = FlagBranch(record, GetItemState(static_cast<std::uint32_t>(arguments[0])) != 0);
            break;
        case FlagOp::IfPlayerNotInMenu:
            record = FlagBranch(record, shenGetIsPlayerInMenu());
            break;
        case FlagOp::IfSearchTarget:
            if (searchTarget == 0 || searchTarget != arguments[0]) {
                record = FlagBranch(record, false);
                break;
            }
            RetrieveSearchFlagFromPtr(flagLocation);
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfCharaSectorOrSearch: {
            const FlagSector &sector = flagSectors[static_cast<std::size_t>(arguments[2])];
            flagSetLocationToChara(shen_name{static_cast<std::uint32_t>(arguments[0])});
            if (searchTarget != 0) {
                record = FlagBranch(record, searchTarget == arguments[1]);
                break;
            }
            record = FlagBranch(record, DoAngleCheck(sector, shen_name{static_cast<std::uint32_t>(arguments[0])}, playerPosition, playerYaw));
            break;
        }
        case FlagOp::SetLoadingPhaseBits:
            shenPlayerData.loadingPhase |= static_cast<std::uint32_t>(arguments[0]);
            record = FlagBranch(record, true);
            if ((shenPlayerData.loadingPhase & 1U) != 0)
                result = 1;
            break;
        case FlagOp::IfPointOnScreen: {
            if (searchTarget != 0) {
                record = FlagBranch(record, true);
                break;
            }
            const Vector3 point{std::bit_cast<float>(arguments[0]), std::bit_cast<float>(arguments[1]), std::bit_cast<float>(arguments[2])};
            flagLocation = point;
            const bool condition = IsWithinLocationScreenSpace(point);
            if (condition)
                shenPlayerData.triggerRecord = record;
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::IfCharaPointOnScreen: {
            flagSetLocationToChara(shen_name{static_cast<std::uint32_t>(arguments[0])});
            if (searchTarget != 0) {
                record = FlagBranch(record, searchTarget == arguments[1]);
                break;
            }
            bool condition = false;
            const Vector3 offset{std::bit_cast<float>(arguments[2]), std::bit_cast<float>(arguments[3]), std::bit_cast<float>(arguments[4])};
            Vector3 point{};
            if (flagTransformCharaOffset(shen_name{static_cast<std::uint32_t>(arguments[0])}, offset, point))
                condition = IsWithinLocationScreenSpace(point);
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::IfPointOnScreenOrSearch:
        case FlagOp::IfPointOnScreenOrSearchB: {
            const Vector3 point{std::bit_cast<float>(arguments[0]), std::bit_cast<float>(arguments[1]), std::bit_cast<float>(arguments[2])};
            flagLocation = point;
            record = FlagBranch(record, searchTarget != 0 ? searchTarget == arguments[4] : IsWithinLocationScreenSpace(point));
            break;
        }
        case FlagOp::IfCloseAndFacing: {
            const Vector3 point{std::bit_cast<float>(arguments[0]), std::bit_cast<float>(arguments[1]), std::bit_cast<float>(arguments[2])};
            flagLocation = point;
            record = FlagBranch(record, shenCheckCloseAndFacing(point, static_cast<std::int16_t>(arguments[3]), playerPosition, playerYaw));
            break;
        }
        case FlagOp::IfEntityEnabledOnScreen:
        case FlagOp::IfEntityOnScreenEnable: {
            SearchTarget *const target = shenFindFlagTarget(shen_name{static_cast<std::uint32_t>(arguments[0])});
            if (!target) {
                record = FlagBranch(record, false);
                break;
            }
            if (record->opcode == static_cast<std::uint8_t>(FlagOp::IfEntityOnScreenEnable)) {
                target->flags &= ~2U;
            } else if ((target->flags & 2U) != 0) {
                record = FlagBranch(record, false);
                break;
            }
            flagLocation = target->position;
            record = FlagBranch(record, searchTarget != 0 ? searchTarget == arguments[0] : IsWithinLocationScreenSpace(target->position));
            break;
        }
        case FlagOp::IfCharaVisible: {
            if (searchTarget != 0) {
                record = FlagBranch(record, true);
                break;
            }
            Character *const character = FlagChara(shen_name{static_cast<std::uint32_t>(arguments[1])});
            bool condition = false;
            if (character) {
                const Vector3 bone = FlagCharaBone(character);
                condition = flddCheckCharaIntersection(
                    bone, shen_name{static_cast<std::uint32_t>(arguments[0])}, static_cast<std::int16_t>(arguments[2]), std::bit_cast<float>(arguments[3]));
            }
            record = FlagBranch(record, condition);
            break;
        }
        case FlagOp::AlwaysFalse:
            record = FlagBranch(record, false);
            break;
        case FlagOp::IfQueryMode2:
        case FlagOp::IfQueryMode3: {
            const std::int32_t requiredMode = record->opcode == static_cast<std::uint8_t>(FlagOp::IfQueryMode2) ? 2 : 3;
            record = FlagBranch(record, mode == requiredMode && arguments[0] == shenPlayerData.queryId && arguments[1] == shenPlayerData.queryArgument);
            break;
        }
        case FlagOp::IfTimeReached: {
            DateTime dateTime{};
            GetCurrentDateTime(dateTime);
            std::uint8_t hour = static_cast<std::uint8_t>(dateTime.hour);
            if (dateTime.hour <= 6)
                hour = static_cast<std::uint8_t>(hour + 24);
            if (static_cast<std::uint32_t>(hour) >= static_cast<std::uint32_t>(arguments[0]) &&
                (static_cast<std::uint32_t>(hour) != static_cast<std::uint32_t>(arguments[0]) || dateTime.minute >= arguments[1])) {
                record = FlagAdvance(record, static_cast<std::uint32_t>(arguments[2]));
                break;
            }
            if (shenPlayerData.idleEvaluations > 2 && data.deferredRecord != record) {
                shenPlayerData.deadlineRecord = record;
                record = FlagBranch(record, true);
                break;
            }
            shenPlayerData.idleEvaluations = 0;
            record = FlagBranch(record, false);
            break;
        }
        case FlagOp::AdvanceTimeToDeadline: {
            FlagRecord *const deadline = shenPlayerData.deadlineRecord;
            shenToggleAreaFlags(1);
            plymAdvanceTime(static_cast<std::uint8_t>(deadline->argument[0]), static_cast<std::uint8_t>(deadline->argument[1]));
            SetScenarioFollowCharacter(shen_name{});
            ReleaseNpcScriptRouteOwner();
            shenPlayerData.flags |= ShenPlayerFlag_WaitTimeAdvance;
            data.deferredRecord = deadline;
            shenPlayerData.record = deadline;
            return 1;
        }
        case FlagOp::WaitNpcRelease:
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(1);
            }
            shenPlayerData.flags |= ShenPlayerFlag_NpcReleasePending;
            shenPlayerData.record = FlagBranch(record, true);
            return 1;
        case FlagOp::WaitSpecialPose:
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(1);
            }
            shenPlayerData.flags |= ShenPlayerFlag_SpecialPosePending;
            shenPlayerData.record = FlagBranch(record, true);
            return 1;
        case FlagOp::ChooseAction: {
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(1);
            }
            std::array<std::int16_t, 4> choices{-1, static_cast<std::int16_t>(arguments[0]), static_cast<std::int16_t>(arguments[1]), -1};
            SetupControlSpeakAndUI(choices.data(), 9999, static_cast<std::int16_t>(arguments[2]), 2);
            shenPlayerData.flags |= ShenPlayerFlag_ActionChoicePending;
            data.deferredRecord = record;
            shenPlayerData.record = record;
            return 1;
        }
        case FlagOp::LockPlayerControlMode3:
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(3);
            }
            result = 1;
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfSegaCoins:
            record = FlagBranch(record, GetCurrentSegaCoins() != 0);
            break;
        case FlagOp::PlayVoice: {
            const std::array<std::int32_t, 2> bank{arguments[0], 0};
            const std::array<std::int32_t, 3> voiceCode{arguments[0], arguments[1], 0};
            if (!EV_FreeAuthorPak(1, reinterpret_cast<const char *>(bank.data()))) {
                record = FlagBranch(record, true);
                break;
            }
            shenPlayerData.voice = EV_PlayDialogue(reinterpret_cast<const char *>(voiceCode.data()));
            if ((shenPlayerData.flags & ShenPlayerFlag_Active) != 0 && shenPlayerData.record)
                shenPlayerData.flags |= ShenPlayerFlag_VoicePending;
            else
                shenPlayerData.flags |= ShenPlayerFlag_DetachedVoicePending;
            shenPlayerData.record = record;
            return 1;
        }
        case FlagOp::IfCurfewSceneChange:
            record = FlagBranch(record, (shenPlayerData.flags & ShenPlayerFlag_CurfewSceneChange) != 0);
            break;
        case FlagOp::UnlockPartyCharas:
            if (record->length > 1) {
                for (std::uint32_t index = 0; index + 1 < record->length; ++index) {
                    const shen_name name{static_cast<std::uint32_t>(arguments[index])};
                    if (FindPTYCForChara(name))
                        ptycCharaUnlock(name);
                }
            }
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfSavedAreaJump:
            record = FlagBranch(record, (shenPlayerData.state & 1U) != 0);
            break;
        case FlagOp::IfRyoNotBusyOrSearch:
            record = FlagBranch(record, ryoNotBusy() || srchHasActiveLockonTarget());
            break;
        case FlagOp::WaitSearchRelease:
            record = FlagBranch(record, true);
            if (!srchHasActiveLockonTarget())
                break;
            srchRequestLockonExit();
            shenPlayerData.flags |= ShenPlayerFlag_SearchReleasePending;
            shenPlayerData.record = record;
            return 1;
        case FlagOp::IfActEquals:
            record = FlagBranch(record, g_actNumber == arguments[0]);
            break;
        case FlagOp::StartFlagCamera:
            if (0.040000003f > FlagDistanceSquared(currentCameraPosition, flagCameraPosition) && shenPlayerData.updateCount > 2) {
                if (camCtrlGetContextVal() == 2) {
                    flagCameraFrame = 101;
                    camCtrlSetContext(0, 0);
                } else {
                    flagCameraFrame = 1;
                    Vector3 position{};
                    camCtrlGetNextCamPos(&position);
                    camCtrlSetState(Event);
                    camCtrlSetNextPosition(position);
                }
            }
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfMapEquals:
            record = FlagBranch(record, static_cast<std::int32_t>(g_currentMap.name) == arguments[0]);
            break;
        case FlagOp::LockPlayerControlMode2:
            if (plymIsSearchStartRequested()) {
                SetPlayerControlEnabled(0);
                shenToggleAreaFlags(2);
            }
            result = 1;
            record = FlagBranch(record, true);
            break;
        case FlagOp::IfCurfewWithin:
            record = FlagBranch(record, shenGetMinutesUntilNextFrameMinute() <= arguments[0]);
            break;
        case FlagOp::ApplyInvertedFlagSteps:
            stepUpdateFlagUtil(static_cast<std::uint8_t>(GetShenmueFlag(arguments[0]) ^ 1), arguments[0]);
            record = FlagBranch(record, true);
            break;
        case FlagOp::SetPlayerFlag1000:
            plymAddFlags(0x1000U);
            record = FlagBranch(record, true);
            break;
        default:
            record = FlagAdvance(record, 1);
            break;
        }
        if (!continueEvaluation)
            return result;
    }
}

void Queue_FLAG_CheckTask() {
    plymSetCallback(shenRestartPlayerDataFlagEvaluation);
    SetWeatherShouldUpdateDay(GetInternalFlag(1001) ? 1 : 0);
    RemoveInventoryItem(69, 1);
    flagRelated(g_currentMap);
    HLib::EnqueueTaskWithoutParameter(FLAG_CheckTask_Callback, 12, 0x0e, static_cast<std::int32_t>(SHEN_NAME(FLGS)));
    shenPlayerData.state = 0;
    if (HasSavedAreaJumpLocation())
        shenPlayerData.state |= 1U;
}

void FLAG_CheckTask_Callback(void *) {
    using enum CameraState;
    flagCameraPosition = previousCameraPosition;
    previousCameraPosition = currentCameraPosition;
    camCtrlGetNextCamPos(&currentCameraPosition);
    if (flagCameraFrame > 100) {
        if (++flagCameraFrame > 130) {
            flagCameraFrame = 0;
            camCtrlSetContext(1, 0);
            Vector3 position{};
            camCtrlGetNextCamPos(&position);
            camCtrlSetState(Event);
            camCtrlSetNextPosition(position);
        }
    } else if (flagCameraFrame != 0) {
        Character *const player = FlagPlayer();
        if (flagCameraFrame == 1) {
            Vector3 position = GetCharacterWorldPosition(player);
            const std::int16_t yaw = static_cast<std::int16_t>(GetCharacterWorldRotation(player).y);
            camCtrlComputeOrbitCameraFromTarget(&position, yaw, &orbitCameraPosition, &orbitCameraTarget);
            GetCharacterMotionWorldPosition(player, 5, &position);
            if (0.5f > FlagDistanceSquared(currentCameraPosition, position)) {
                flagPlayerAlpha = 0.0f;
                modelSetCharaAlpha(player, 0.0f);
            } else {
                flagPlayerAlpha = 1.0f;
            }
        }
        ++flagCameraFrame;
        flagCameraPosition.x = (orbitCameraPosition.x - currentCameraPosition.x) * 0.30000001f + currentCameraPosition.x;
        flagCameraPosition.z = (orbitCameraPosition.z - currentCameraPosition.z) * 0.30000001f + currentCameraPosition.z;
        flagCameraPosition.y = (orbitCameraPosition.y - currentCameraPosition.y) * 0.30000001f + currentCameraPosition.y;
        camCtrlSetNextPosition(flagCameraPosition);
        if (1.0f > flagPlayerAlpha) {
            flagPlayerAlpha = std::fmin(flagPlayerAlpha + 0.1f, 1.0f);
            modelSetCharaAlpha(player, flagPlayerAlpha);
        } else {
            if (flagCameraFrame > 30)
                flagCameraFrame = 0;
            if (0.00039999999f > FlagDistanceSquared(flagCameraPosition, orbitCameraPosition))
                flagCameraFrame = 0;
        }
    }

    if ((shenPlayerData.flags & ShenPlayerFlag_DetachedVoicePending) == 0 || cvsHasValidConverseData(shenPlayerData.voice))
        return;
    const std::array<std::int32_t, 2> bank{shenPlayerData.record->argument[0], 0};
    EV_FreeAuthorPak(0, reinterpret_cast<const char *>(bank.data()));
    FlagRecord *const record = shenPlayerData.record;
    shenPlayerData.flags &= ~ShenPlayerFlag_DetachedVoicePending;
    shenPlayerData.record = FlagBranch(record, true);
    shenToggleAreaFlags(0);
    SetPlayerControlEnabled(1);
}

void CtrlSystemManage_Callback(void *) {
    if (systemManageSource[0] == '\0') {
        systemManageFrames = 0;
    } else if (++systemManageFrames > 30 && plymIsSearchStartRequested()) {
        systemManageSource[0] = '\0';
    }
    if (g_systemManageDebugDisplay && systemManageSource[0] != '\0') {
        SetDebugStringPosition(24, 3);
        DebugTextPrintf("%8s-%7.2f", systemManageSource.data(), static_cast<double>(static_cast<float>(systemManageFrames)) / 30.0);
    }
}

HLib::Task *initCtrlSystemManage() {
    HLib::Task *const task = HLib::EnqueueTaskWithoutParameter(CtrlSystemManage_Callback, 14, 2, static_cast<std::int32_t>(SHEN_NAME(SYSM)));
    systemManageSource[0] = '\0';
    return task;
}

const char *GetAberdeenSearchData() noexcept {
    std::size_t index = 0;
    for (; index < 0x14 && systemManageSource[index] != '\0'; ++index)
        aberdeenSearchData[index] = systemManageSource[index];
    for (; index < 0x14; ++index)
        aberdeenSearchData[index] = '\0';
    return aberdeenSearchData.data();
}

} // namespace shenmue::s2
