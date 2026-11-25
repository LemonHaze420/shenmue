#include "s2/cloth_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/cloth_data_dc.hpp"
#include "s2/model_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {
namespace {
ClothParameterRecord defaultClothParameters[1] = {
    {shen_name(UINT32_MAX), {reinterpret_cast<std::int32_t>(defaultClothBones)}, {0, 0, 0, 0x1fe00, 0, 0, 0, 0, 0, 0}}};
} // namespace

// @verified - d
std::uint32_t GetClothMeshByteSize(const void *mesh) {
    const std::uint32_t *packet = reinterpret_cast<const std::uint32_t *>(reinterpret_cast<const std::byte *>(mesh) + 0x68);
    while (true) {
        std::int8_t flags = *reinterpret_cast<const std::int8_t *>(packet);
        while (true) {
            ++packet;
            std::uint8_t count = *reinterpret_cast<const std::uint8_t *>(packet);
            ++packet;
            if (flags & 8U)
                count = static_cast<std::uint8_t>(count * 3U);
            for (std::uint8_t index = 0; index < count; ++index) {
                if (*packet & 1U)
                    packet += 8;
                else
                    packet += 2;
            }
            if (!*packet) {
                packet += 2;
                return static_cast<std::uint32_t>(reinterpret_cast<const std::byte *>(packet) - reinterpret_cast<const std::byte *>(mesh));
            }
            if (!(*packet & 0x80000000U))
                break;
            packet += 20;
            flags = *reinterpret_cast<const std::int8_t *>(packet);
        }
    }
}

// @verified - d
void ReadClothCapsules(ClothComponent *cloth, const std::byte *records) {
    cloth->boneRecords = records;
    cloth->boneRecordCount = 0;
    const std::byte *cursor = records;
    do {
        ++cloth->boneRecordCount;
        cursor += 10;
    } while (static_cast<std::uint8_t>(cursor[9]) != 0xffU);
    if (cloth->boneBuffer) {
        HLib::ClearTaskDataMark(cloth->boneBuffer);
        HLib::FreeTaskData(cloth->boneBuffer);
    }
    cloth->boneBuffer = HLib::AllocateTaskData(cloth->boneRecordCount * kClothBoneCapsuleBytes, MEMORY_CLOTH_BONES);
    HLib::AssignDynamicAlignmentForTaskData(cloth->boneBuffer);
    if (cloth->clothCount) {
        for (std::uint8_t index = 0; index < cloth->clothCount; ++index) {
            cloth->cloths[index]->boneRecordCount = cloth->boneRecordCount;
            cloth->cloths[index]->boneBuffer = cloth->boneBuffer;
        }
    }
}

// @verified - d
void SetupClothBones(ClothComponent *cloth) {
    const std::byte *records;
    if (!cloth->parameters) {
        std::uint16_t index;
        for (index = 0; defaultClothParameters[index].character != UINT32_MAX && defaultClothParameters[index].character != cloth->character; ++index) {
        }
        cloth->parameters = reinterpret_cast<std::byte *>(defaultClothParameters + index);
        records = reinterpret_cast<const std::byte *>(reinterpret_cast<const ClothParameterRecord *>(cloth->parameters)->boneRecords);
    } else {
        const std::int32_t offset = *reinterpret_cast<const std::int32_t *>(cloth->parameters + 4);
        records = cloth->source + offset;
    }
    ReadClothCapsules(cloth, records);
}

void AllocateClothNodeTree(ClothComponent *cloth, MT7Node *model) {
    std::int32_t index;
    if (model->id == -90) {
        ClothObject *const object = static_cast<ClothObject *>(HLib::AllocateTaskData(sizeof(ClothObject), MEMORY_CLOTH_OBJECT));
        HLib::AssignDynamicAlignmentForTaskData(object);
        model->taskData = object;
        object->index = cloth->clothCount;
        object->boneRecordCount = cloth->boneRecordCount;
        const std::uint8_t slot = cloth->clothCount++;
        cloth->cloths[slot] = object;
        object->token = MEMORY_CLOTH_OBJECT;
        object->flags = 0;
        object->rows = 0;
        object->columns = 0;
        object->vertexCount = 0;
        object->owner = cloth;
        object->nodeId = static_cast<std::uint16_t>(model->id);
        object->node = model;
        object->bindings = 0;
        object->previousBindings = 0;
        object->link = 0;
        object->descriptorIndex = 0xffff;
        object->flags |= 1U;
        object->columnLinks = 0;
        for (index = 0; index < 9; ++index)
            object->bindingIndices[index] = -1;
        object->sourceMesh = model->mesh;
        object->meshSize = GetClothMeshByteSize(model->mesh);
        object->mesh = HLib::AllocateTaskData(object->meshSize, MEMORY_CLOTH_MESH);
        HLib::AssignDynamicAlignmentForTaskData(object->mesh);
        std::memcpy(object->mesh, object->sourceMesh, object->meshSize);
        std::byte *const source = cloth->source;
        const std::byte *const section = source + *reinterpret_cast<const std::int32_t *>(source + 0x0c);
        const std::byte *const descriptor = source + *reinterpret_cast<const std::int32_t *>(section + 8) + object->index * 0x40U;
        object->columns = static_cast<std::uint8_t>(descriptor[0]);
        object->rows = static_cast<std::uint8_t>(descriptor[4]);
        object->vertexCount = object->rows * object->columns;
        object->descriptorData28 = source + *reinterpret_cast<const std::int32_t *>(descriptor + 0x28);
        object->descriptorData10 = source + *reinterpret_cast<const std::int32_t *>(descriptor + 0x10);
        if (*reinterpret_cast<const std::int32_t *>(descriptor + 8))
            object->flags |= 0x80U;
        if (!object->descriptorData28 || !object->descriptorData10)
            object->owner->mode |= 2U;
        object->vertices = static_cast<ClothVertexState *>(
            HLib::AllocateTaskData(static_cast<std::uint32_t>(object->vertexCount) * sizeof(ClothVertexState), MEMORY_CLOTH_VERTICES));
        HLib::AssignDynamicAlignmentForTaskData(object->vertices);
        ClothVertexState *vertex = object->vertices;
        for (index = 0; index < object->vertexCount; ++index, ++vertex) {
            vertex->bindingIndex = 0xff;
            vertex->retainedFlags = 0;
            vertex->flags = 0;
            vertex->above = 0;
            vertex->below = 0;
            vertex->left = 0;
            vertex->right = 0;
            vertex->velocity.x = 0.0f;
            vertex->velocity.y = 0.0f;
            vertex->velocity.z = 0.0f;
            vertex->aboveRestLength = 0.0f;
            vertex->leftRestLength = 0.0f;
            vertex->rightRestLength = 0.0f;
        }
        object->bindings = static_cast<ClothVertexBinding *>(
            HLib::AllocateTaskData(static_cast<std::uint32_t>(object->vertexCount) * sizeof(ClothVertexBinding), MEMORY_CLOTH_BINDINGS));
        object->previousBindings = static_cast<ClothVertexBinding *>(
            HLib::AllocateTaskData(static_cast<std::uint32_t>(object->vertexCount) * sizeof(ClothVertexBinding), MEMORY_CLOTH_PREVIOUS_BINDINGS));
        HLib::AssignDynamicAlignmentForTaskData(object->bindings);
        HLib::AssignDynamicAlignmentForTaskData(object->previousBindings);
        ClothVertexBinding *binding = object->bindings;
        ClothVertexBinding *previous = object->previousBindings;
        for (index = 0; index < object->vertexCount; ++index, ++binding, ++previous) {
            binding->sourceOffset = -1;
            previous->sourceOffset = -1;
            binding->flags = 1;
            previous->flags = 1;
        }
        object->columnLinks = static_cast<void **>(HLib::AllocateTaskData(object->columns * sizeof(void *), MEMORY_CLOTH_COLUMNS));
        HLib::AssignDynamicAlignmentForTaskData(object->columnLinks);
        for (index = 0; index < object->columns; ++index)
            object->columnLinks[index] = 0;
    } else if (model->id == -91) {
        ClothObject *const object = static_cast<ClothObject *>(model->parent->taskData);
        std::byte *const source = cloth->source;
        const std::byte *const section = source + *reinterpret_cast<const std::int32_t *>(source + 0x0c);
        const std::byte *const descriptor = source + *reinterpret_cast<const std::int32_t *>(section + 8) + object->index * 0x40U;
        std::byte *const data = source + *reinterpret_cast<const std::int32_t *>(descriptor + 0x2c);
        if (data) {
            ClothLink *const link = static_cast<ClothLink *>(HLib::AllocateTaskData(sizeof(ClothLink), MEMORY_CLOTH_OBJECT));
            HLib::AssignDynamicAlignmentForTaskData(link);
            object->link = link;
            link->primary = object;
            link->descriptorData = data;
            link->node = model;
            link->sourceMesh = model->mesh;
            const std::uint32_t bytes = GetClothMeshByteSize(model->mesh);
            link->mesh = HLib::AllocateTaskData(bytes, MEMORY_CLOTH_MESH);
            HLib::AssignDynamicAlignmentForTaskData(link->mesh);
            std::memcpy(link->mesh, link->sourceMesh, bytes);
            link->bindings = static_cast<ClothVertexBinding *>(
                HLib::AllocateTaskData(static_cast<std::uint32_t>(object->vertexCount) * sizeof(ClothVertexBinding), MEMORY_CLOTH_BINDINGS));
            HLib::AssignDynamicAlignmentForTaskData(link->bindings);
            // The linked-node path resets the primary bindings, leaving its new buffer untouched.
            ClothVertexBinding *binding = object->bindings;
            ClothVertexBinding *previous = object->previousBindings;
            for (index = 0; index < object->vertexCount; ++index, ++binding, ++previous) {
                binding->sourceOffset = -1;
                previous->sourceOffset = -1;
                binding->flags = 1;
                previous->flags = 1;
            }
        }
    }
    if (model->child)
        AllocateClothNodeTree(cloth, model->child);
    if (model->sibling)
        AllocateClothNodeTree(cloth, model->sibling);
}

// @verified - d
void AllocateClothObjects(ClothComponent *cloth) {
    MT7Node *const model = cloth->model;
    cloth->clothCount = 0;
    cloth->mode = 0;
    cloth->boneRecords = 0;
    for (std::uint8_t index = 0; index < sizeof(cloth->cloths) / sizeof(cloth->cloths[0]); ++index)
        cloth->cloths[index] = 0;
    SetupClothBones(cloth);
    if (cloth->parameters)
        cloth->mode = static_cast<std::uint16_t>(static_cast<std::uint8_t>(cloth->parameters[0x1a]) << 8U);
    AllocateClothNodeTree(cloth, model);
}

} // namespace s2
} // namespace shenmue

#endif
