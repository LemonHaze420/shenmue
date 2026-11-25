#include "s2/flag_wind_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/colour_bar_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/camera_dc.hpp"
#include "s2/field_depth_dc.hpp"
#include "s2/character_dc.hpp"
#include "s2/character_model_dc.hpp"
#include "s2/character_placement_dc.hpp"
#include "s2/model_render_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/weather_wind_dc.hpp"
#include "platform/dc/matrix.hpp"

#include "platform/dc/sdk.hpp"
#include <cmath>

namespace shenmue {
namespace s2 {

namespace {

typedef void (*WindVertexCallback)(FlagWindVertex *, Vector3 *, std::int32_t *);

void CountWindVertexPositiveXNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void BindWindVertexPositiveXNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void CountWindVertexPositiveX(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void BindWindVertexPositiveX(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void CountWindVertexNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void BindWindVertexNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void CountWindVertexAll(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);
void BindWindVertexAll(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count);

} // namespace

void ApplyWaveformTransformSinewave(FlagWindVertex *vertices, std::uint32_t count, std::int32_t phase, float amplitude);

float flagWindMaximumX, flagWindMaximumY, flagWindMaximumZ, flagWindMinimumX, flagWindMinimumY, flagWindMinimumZ;

namespace {

const WindVertexCallback windCountCallbacks[] = {CountWindVertexPositiveXNegativeY, CountWindVertexPositiveX, CountWindVertexAll, CountWindVertexAll,
    CountWindVertexAll, CountWindVertexAll, CountWindVertexAll, CountWindVertexNegativeY, BindWindVertexPositiveXNegativeY, BindWindVertexPositiveX};

const WindVertexCallback windBindCallbacks[] = {BindWindVertexPositiveXNegativeY, BindWindVertexPositiveX, BindWindVertexAll, BindWindVertexAll,
    BindWindVertexAll, BindWindVertexAll, BindWindVertexAll, BindWindVertexNegativeY, reinterpret_cast<WindVertexCallback>(COLB_Module), 0};

// @verified - d
std::int32_t VisitWindMeshVertices(void *mesh, WindVertexCallback callback, FlagWindVertex *destination, std::int32_t &count) {
    flagWindMaximumX = -100.0f;
    flagWindMaximumY = -100.0f;
    flagWindMaximumZ = -100.0f;
    flagWindMinimumX = 100.0f;
    flagWindMinimumY = 100.0f;
    flagWindMinimumZ = 100.0f;
    count = 0;
    std::int32_t *cursor = static_cast<std::int32_t *>(mesh) + 6;
    while (*cursor) {
        const std::int32_t bytes = cursor[19];
        cursor += 20;
        std::int32_t *const end = cursor + bytes / 4;
        while (cursor < end) {
            const std::int32_t flags = cursor[0];
            std::int32_t vertices = cursor[1];
            cursor += 2;
            if (flags & 8)
                vertices *= 3;
            else if (flags & 4)
                vertices *= 4;
            for (std::int32_t index = 0; index < vertices; ++index) {
                if (*reinterpret_cast<const std::uint8_t *>(cursor) & 1U) {
                    Vector3 &vertex = *reinterpret_cast<Vector3 *>(cursor);
                    if (callback)
                        callback(destination, &vertex, &count);
                    if (vertex.x > flagWindMaximumX)
                        flagWindMaximumX = vertex.x;
                    if (vertex.y > flagWindMaximumY)
                        flagWindMaximumY = vertex.y;
                    if (vertex.z > flagWindMaximumZ)
                        flagWindMaximumZ = vertex.z;
                    if (flagWindMinimumX > vertex.x)
                        flagWindMinimumX = vertex.x;
                    if (flagWindMinimumY > vertex.y)
                        flagWindMinimumY = vertex.y;
                    if (flagWindMinimumZ > vertex.z)
                        flagWindMinimumZ = vertex.z;
                    cursor += 8;
                } else {
                    if (cursor[1] & 1)
                        return -1;
                    cursor += 2;
                }
            }
        }
    }
    return 0;
}

void CountWindVertexPositiveXNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if (!destination && vertex->x > 0.050000001f && vertex->y < -0.050000001f)
        ++*count;
}

void BindWindVertexPositiveXNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    FlagWindVertex *const entry = &destination[*count];
    if (vertex->x > 0.050000001f && vertex->y < -0.050000001f) {
        entry->position = vertex;
        entry->originalPosition.x = vertex->x;
        entry->originalPosition.y = vertex->y;
        entry->originalPosition.z = vertex->z;
        ++*count;
    }
}

void CountWindVertexPositiveX(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if (!destination && vertex->x > 0.050000001f)
        ++*count;
}

void BindWindVertexPositiveX(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    FlagWindVertex *const entry = &destination[*count];
    if (vertex->x > 0.050000001f) {
        entry->position = vertex;
        entry->originalPosition.x = vertex->x;
        entry->originalPosition.y = vertex->y;
        entry->originalPosition.z = vertex->z;
        ++*count;
    }
}

void CountWindVertexNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if (!destination && vertex->y < -0.050000001f)
        ++*count;
}

void BindWindVertexNegativeY(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    FlagWindVertex *const entry = &destination[*count];
    if (vertex->y < -0.050000001f) {
        entry->position = vertex;
        entry->originalPosition.x = vertex->x;
        entry->originalPosition.y = vertex->y;
        entry->originalPosition.z = vertex->z;
        ++*count;
    }
}

void CountWindVertexAll(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    if (!destination && vertex)
        ++*count;
}

void BindWindVertexAll(FlagWindVertex *destination, Vector3 *vertex, std::int32_t *count) {
    FlagWindVertex *const entry = &destination[*count];
    entry->position = vertex;
    entry->originalPosition.x = vertex->x;
    entry->originalPosition.y = vertex->y;
    entry->originalPosition.z = vertex->z;
    ++*count;
}

// @verified - d
void AnimateFlagCallback(Character *character, void *payload) {
    FlagAnimationObject *const object = static_cast<FlagAnimationObject *>(payload);
    if (!character)
        return;
    float scaled = GetEffectiveWindSpeed(GetWind());
    GetCurrentWindPhaseAccumulator(GetWind());
    float amplitude = scaled * 40.0f;
    scaled = amplitude;
    if (1.0f > amplitude)
        amplitude = 1.0f;
    FlagAnimationPlacement *placement = object->placements;
    const std::int32_t count = object->placementCount;
    for (std::int32_t index = 0; index < count; ++placement, ++index) {
        if (!camCtrlIsSphereVisible(placement->translation, 2.0f) || !camCtrlCheckWorldDepthPosition(placement->translation, 2.0f))
            continue;
        float step = static_cast<float>(placement->speed) * scaled;
        if (5.0f > step)
            step = 5.0f;
        placement->phase = (static_cast<std::int32_t>(step) + placement->phase) % 65536;
        ApplyWaveformTransformSinewave(object->vertices, object->vertexCount, placement->phase, amplitude);
        if (index) {
            matrix_stack::Matrix4x4_Push(0);
            Matrix4x4::preTranslate(placement->translation.x, placement->translation.y, placement->translation.z);
            if (placement->rotationZ)
                Matrix4x4::RotateAroundZ_Wrapper(placement->rotationZ);
            if (placement->rotationY)
                Matrix4x4::RotateAroundY_Wrapper(placement->rotationY);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            DrawCharacterModelBranch(object->root);
#else
            reinterpret_cast<void (*)(MT7Node *)>(DrawCharacterModelBranch)(object->root);
#endif
            PopCurrentMatrix(1);
        } else {
            Vector3i rotation;
            rotation.x = 0;
            rotation.y = placement->rotationY;
            rotation.z = placement->rotationZ;
            SetCharacterWorldPosition(character, &placement->translation);
            charaSetWorldRot(character, rotation);
        }
    }
}

// @verified - d
void FreeAnimatedFlag(Character *character, void *payload) {
    if (character && payload) {
        FlagAnimationObject *const object = static_cast<FlagAnimationObject *>(payload);
        if (object->vertices)
            HLib::FreeTaskData(object->vertices);
    }
}

} // namespace

// @verified - d
float EvaluateWindWave(std::int32_t angle) {
    const float first = dc::sdk::njSin(22 * angle), second = dc::sdk::njSin(31 * angle);
    const float third = dc::sdk::njSin(103 * angle), fourth = dc::sdk::njSin(147 * angle);
    return first * 0.3f + second * 0.37f + third * 0.15f + fourth * 0.18f;
}

// @verified - d
void ApplyWaveformTransformSinewave(FlagWindVertex *vertices, std::uint32_t count, std::int32_t phase, float amplitude) {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(count); ++index, ++vertices) {
        const float x = vertices->originalPosition.x;
        const float negativeY = -vertices->originalPosition.y;
        const std::int32_t phaseOffset = static_cast<std::int32_t>((x + negativeY) * 65536.0f * 0.01f);
        const float wave = EvaluateWindWave((phase + (0x10000 - phaseOffset)) % 0x10000);
        float xOffset = wave * x * 0.2f;
        float yOffset = wave * negativeY * 0.05f;
        vertices->position->z = (xOffset + yOffset) * amplitude;
        if (0.0f > xOffset)
            xOffset = 0.0f;
        if (yOffset > 0.0f)
            yOffset = 0.0f;
        vertices->position->x = x - xOffset;
        *reinterpret_cast<std::uint32_t *>(&vertices->position->x) |= 1U;
        vertices->position->y = -negativeY - yOffset;
    }
}

// @verified - d
void ApplyCurvedWindOscillation(FlagWindModel *model, float amplitude) {
    FlagWindVertex *vertex = model->vertices;
    const std::int32_t count = model->vertexCount;
    std::int32_t bend = static_cast<std::int32_t>(amplitude);
    const std::int32_t phase = model->phase;
    if (bend > 64)
        bend = 64;
    else if (bend < 32)
        bend = 32;
    bend <<= 8;
    const float sine = dc::sdk::SinFixed(bend);
    const float cosine = dc::sdk::CosFixed(bend);
    const float extent = model->maximumY - model->minimumY;
    for (std::int32_t index = 0; index < count; ++index, ++vertex) {
        const float x = vertex->originalPosition.x;
        const float negativeY = -vertex->originalPosition.y;
        const std::int32_t phaseOffset = static_cast<std::int32_t>((x + extent - negativeY) * 65536.0f * 0.01f);
        const float wave = EvaluateWindWave((phase + (0x10000 - phaseOffset)) % 0x10000);
        float offset = wave * x * 0.4f;
        const float bendOffset = wave * (extent - negativeY) * 0.1f;
        vertex->position->z = (offset + bendOffset) * 0.5f;
        if (0.0f > offset)
            offset = 0.0f;
        vertex->position->x = sine * x - offset;
        *reinterpret_cast<std::uint32_t *>(&vertex->position->x) |= 1U;
        vertex->position->y = (-negativeY - bendOffset) - cosine * x * (1.0f - negativeY / extent * 0.5f);
    }
}

// @verified - d
void VerticalWaveOffsetZBend(FlagWindModel *model, float amplitude) {
    FlagWindVertex *vertex = model->vertices;
    const std::int32_t count = model->vertexCount;
    const std::int32_t phase = model->phase;
    for (std::int32_t index = 0; index < count; ++index, ++vertex) {
        const float x = vertex->originalPosition.x;
        const float height = model->maximumY - vertex->originalPosition.y;
        const std::int32_t phaseOffset = static_cast<std::int32_t>((x + height) * 65536.0f * 0.005f);
        const float offset = EvaluateWindWave((phase + (0x10000 - phaseOffset)) % 0x10000) * height * 0.1f * amplitude;
        vertex->position->z = vertex->originalPosition.z + offset;
        float magnitude;
        if (0.0f > offset)
            magnitude = -offset;
        else
            magnitude = offset;
        vertex->position->x = x - offset;
        *reinterpret_cast<std::uint32_t *>(&vertex->position->x) |= 1U;
        vertex->position->y = vertex->originalPosition.y + magnitude;
    }
}

// @verified - d
void SineStackYCurve(FlagWindModel *model, float scale) {
    FlagWindVertex *vertex = model->vertices;
    std::int32_t count = model->vertexCount;
    std::int32_t phase = model->phase;
    float upper = model->maximumY - 0.05f;
    float lower = model->minimumY + 0.05f;
    for (std::int32_t index = 0; index < count; ++index, ++vertex) {
        float x = vertex->originalPosition.x;
        float y = vertex->originalPosition.y;
        if (y > upper || lower > y)
            continue;
        std::int32_t phaseOffset = static_cast<std::int32_t>((x - y) * 65536.0f * 0.005f);
        float wave = EvaluateWindWave((phase + (0x10000 - phaseOffset)) % 0x10000);
        float xOffset = wave * 0.2f;
        float yOffset = wave * -y * 0.05f;
        vertex->position->z = (xOffset + yOffset) * scale;
        if (0.0f > xOffset)
            xOffset = 0.0f;
        if (yOffset > 0.0f)
            yOffset = 0.0f;
        vertex->position->x = x - xOffset;
        *reinterpret_cast<std::uint32_t *>(&vertex->position->x) |= 1U;
        vertex->position->y = y - yOffset;
    }
}

// @verified - d
void DualAxisZone(FlagWindModel *model, float scale) {
    FlagWindVertex *vertex = model->vertices;
    const std::int32_t count = model->vertexCount;
    const std::int32_t phase = model->phase;
    const float upperX = model->maximumX - 0.05f;
    const float lowerX = model->minimumX + 0.05f;
    const float upperZ = model->maximumZ - 0.05f;
    const float lowerZ = model->minimumZ + 0.05f;
    for (std::int32_t index = 0; index < count; ++index, ++vertex) {
        const float x = vertex->originalPosition.x;
        const float z = vertex->originalPosition.z;
        if (upperX > x && x > lowerX && upperZ > z && z > lowerZ) {
            const std::int32_t phaseOffset = static_cast<std::int32_t>((x + z) * 65536.0f * 0.005f);
            const float wave = EvaluateWindWave((phase + (0x10000 - phaseOffset)) % 0x10000);
            float xOffset = wave * 0.1f;
            float zOffset = wave * 0.05f;
            vertex->position->y = xOffset * scale + vertex->originalPosition.y;
            if (0.0f > xOffset)
                xOffset = 0.0f;
            if (zOffset > 0.0f)
                zOffset = 0.0f;
            vertex->position->x = x - xOffset * scale;
            *reinterpret_cast<std::uint32_t *>(&vertex->position->x) |= 1U;
            vertex->position->z = z - zOffset * scale;
        }
    }
}

// @verified - d
void AssignAnimatedFlag(shen_name name, FlagAnimationPlacement *placements, std::int32_t count) {
    if (!placements || !count)
        return;
    const MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = scenFindChara(control->scenarioTask, name);
    if (!character || !character->model || !character->model->child || !character->model->child->mesh)
        return;
    MT7Node *const root = character->model;
    void *const mesh = root->child->mesh;
    std::int32_t vertices = 0;
    if (VisitWindMeshVertices(mesh, CountWindVertexPositiveXNegativeY, 0, vertices))
        return;
    FlagAnimationObject *const object =
        static_cast<FlagAnimationObject *>(AllocateModelComponent(character, shen_name(FLAG_WIND_COMPONENT), sizeof(FlagAnimationObject), AnimateFlagCallback));
    if (!object)
        return;
    object->root = root;
    object->vertices = static_cast<FlagWindVertex *>(HLib::AllocateTaskData(vertices * sizeof(FlagWindVertex), shen_name(FLAG_WIND_COMPONENT)));
    if (!object->vertices)
        return;
    object->vertexCount = vertices;
    object->placements = placements;
    object->placementCount = count;
    VisitWindMeshVertices(mesh, BindWindVertexPositiveXNegativeY, object->vertices, vertices);
    SetModelComponentCleanup(character, shen_name(FLAG_WIND_COMPONENT), FreeAnimatedFlag);
}

// @verified - d
std::uint32_t CountFlagWindModels(MT7Node *node) {
    std::uint32_t count = static_cast<std::uint16_t>(static_cast<std::uint16_t>(node->id) - 5000U) < 10U;
    if (node->child)
        count += CountFlagWindModels(node->child);
    if (node->sibling)
        count += CountFlagWindModels(node->sibling);
    return count;
}

// @verified - d
std::int32_t CollectFlagWindModels(MT7Node *node, FlagWindModel *models, std::int32_t &index) {
    std::int32_t vertices = 0;
    if (static_cast<std::uint16_t>(static_cast<std::uint16_t>(node->id) - 5000U) < 10U) {
        models[index].type = node->id - 5000;
        models[index].mesh = node->mesh;
        if (node->mesh)
            VisitWindMeshVertices(node->mesh, windCountCallbacks[models[index].type], 0, vertices);
        models[index].vertexCount = vertices;
        ++index;
    }
    if (node->child)
        vertices += CollectFlagWindModels(node->child, models, index);
    if (node->sibling)
        vertices += CollectFlagWindModels(node->sibling, models, index);
    return vertices;
}

void UpdateWindForModel(FlagWindObject *object) {
    float speed = GetEffectiveWindSpeed(GetWind());
    GetCurrentWindPhaseAccumulator(GetWind());
    if (speed > 0.06f)
        speed = 0.06f;
    speed *= 40.0f;
    float amplitude = speed;
    if (amplitude < 1.0f)
        amplitude = 1.0f;
    const std::int32_t count = object->modelCount;
    FlagWindModel *model = object->models;
    for (std::int32_t index = 0; index < count; ++index, ++model) {
        float oscillation = static_cast<float>(model->speed);
        oscillation *= speed;
        switch (model->type) {
        case 0:
            if (oscillation < 15.0f)
                oscillation = 15.0f;
            ApplyWaveformTransformSinewave(model->vertices, model->vertexCount, model->phase, amplitude);
            break;
        case 1:
            if (oscillation < 55.0f)
                oscillation = 55.0f;
            else if (oscillation > 120.0f)
                oscillation = 120.0f;
            ApplyCurvedWindOscillation(model, oscillation);
            break;
        case 2:
            if (oscillation < 15.0f)
                oscillation = 15.0f;
            if (amplitude < 2.0f)
                amplitude = 2.0f;
            VerticalWaveOffsetZBend(model, amplitude);
            break;
        case 3:
            if (oscillation < 5.0f)
                oscillation = 5.0f;
            if (amplitude > 3.0f)
                amplitude = 3.0f;
            SineStackYCurve(model, amplitude);
            break;
        case 4:
            if (oscillation < 5.0f)
                oscillation = 5.0f;
            if (amplitude > 2.0f)
                amplitude = 2.0f;
            DualAxisZone(model, amplitude * 0.5f);
            break;
        case 5:
            if (oscillation < 20.0f)
                oscillation = 20.0f;
            if (amplitude < 2.0f)
                amplitude = 2.0f;
            VerticalWaveOffsetZBend(model, amplitude);
            break;
        case 6:
            if (oscillation < 5.0f)
                oscillation = 5.0f;
            DualAxisZone(model, amplitude);
            break;
        }
        model->phase = (static_cast<std::int32_t>(oscillation) + model->phase) % 65536;
    }
}

// @verified - d
void UpdateWindForChara(Character *character, void *payload) {
    if (character) {
        Vector3 position;
        GetCharacterWorldPosition(character, &position);
        if (camCtrlIsInsideDepthScaledRect(position, GetCharacterDisplayRadius(character)))
            UpdateWindForModel(static_cast<FlagWindObject *>(payload));
    }
}

// @verified - d
void FreeWindObject(FlagWindObject *payload) {
    if (payload) {
        if (payload->vertices) {
            HLib::FreeTaskData(payload->vertices);
            payload->vertices = 0;
        }
        if (payload->models) {
            HLib::FreeTaskData(payload->models);
            payload->models = 0;
        }
    }
}

// @verified - d
void FreeWindModel(Character *character, void *payload) {
    if (character && payload)
        FreeWindObject(static_cast<FlagWindObject *>(payload));
}

// @verified - d
std::int32_t PopulateFlagWindObject(MT7Node *root, FlagWindObject &object, std::uint32_t modelCount) {
    object.root = root;
    FlagWindModel *models = static_cast<FlagWindModel *>(HLib::AllocateTaskData(sizeof(FlagWindModel) * modelCount, FLAG_WIND_COMPONENT));
    if (!models)
        return -1;
    std::int32_t index = 0;
    std::int32_t count = CollectFlagWindModels(root, models, index);
    object.vertices = static_cast<FlagWindVertex *>(HLib::AllocateTaskData(sizeof(FlagWindVertex) * count, FLAG_WIND_COMPONENT));
    if (!object.vertices)
        return -1;
    object.modelCount = modelCount;
    object.models = models;
    FlagWindVertex *cursor = object.vertices;
    for (std::int32_t modelIndex = 0; modelIndex < static_cast<std::int32_t>(modelCount); ++modelIndex, ++models) {
        FlagWindModel &model = *models;
        model.vertices = cursor;
        VisitWindMeshVertices(model.mesh, windBindCallbacks[model.type], cursor, count);
        model.phase = static_cast<std::int32_t>(dc::sdk::RandomInt());
        model.speed = static_cast<std::int32_t>(dc::sdk::RandomInt()) % 5 + 10;
        model.maximumX = flagWindMaximumX;
        model.minimumX = flagWindMinimumX;
        model.maximumY = flagWindMaximumY;
        model.minimumY = flagWindMinimumY;
        model.maximumZ = flagWindMaximumZ;
        model.minimumZ = flagWindMinimumZ;
        cursor += count;
    }
    return 0;
}

// @verified - d
void AttachFlagWindComponent(Character *character) {
    if (!character)
        return;
    MT7Node *const model = character->model;
    if (!model)
        return;
    const std::uint32_t count = CountFlagWindModels(model);
    if (!count)
        return;
    FlagWindObject *const object =
        static_cast<FlagWindObject *>(AllocateModelComponent(character, FLAG_WIND_COMPONENT, sizeof(FlagWindObject), UpdateWindForChara));
    if (object) {
        PopulateFlagWindObject(model, *object, count);
        SetModelComponentCleanup(character, FLAG_WIND_COMPONENT, FreeWindModel);
    }
}

// @verified - d
FlagWindObject *AllocateFlagObject(MT7Node *root) {
    const std::uint32_t count = CountFlagWindModels(root);
    if (!count)
        return 0;
    FlagWindObject *const object = static_cast<FlagWindObject *>(HLib::AllocateTaskData(sizeof(FlagWindObject), FLAG_WIND_COMPONENT));
    if (object)
        PopulateFlagWindObject(root, *object, count);
    return object;
}

} // namespace s2
} // namespace shenmue

#endif
