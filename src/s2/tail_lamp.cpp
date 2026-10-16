#include "s2/model_component.hpp"
#include "d3t/buffer.hpp"
#include "d3t/dx11_renderer.hpp"
#include "d3t/object_store.hpp"
#include "d3t/model_cache.hpp"

#include "s2/archive_data.hpp"
#include "s2/author.hpp"
#include "s2/character.hpp"
#include "s2/camera.hpp"
#include "s2/field_collision.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/map_control.hpp"
#include "s2/pak.hpp"
#include "s2/motion_work.hpp"
#include "s2/motion.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/math.hpp"
#include "s2/motion_pose.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_image.hpp"
#include "s2/hand.hpp"
#include "s2/handcuffs.hpp"
#include "s2/face.hpp"
#include "s2/lighting.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/door.hpp"
#include "s2/maze.hpp"
#include "s2/navi.hpp"
#include "s2/game_runtime.hpp"
#include "s2/event_runtime.hpp"
#include "s2/weather.hpp"
#include "s2/world_render.hpp"
#include "s2/pause.hpp"
#include "s2/player.hpp"
#include "s2/string_utility.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <memory>
#include <new>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include "s2/area_names.hpp"
#include "s2/model_component_internal.hpp"

namespace shenmue::s2 {
using namespace model_component_detail;

namespace model_component_detail {

// @verified - p
void TailLampTaskCallback(void *taskData) {
    TailLampTaskData *const data = static_cast<TailLampTaskData *>(taskData);
    Matrix4x4_Push();
    Matrix4x4::SetActiveTransform(&data->transform);
    charaPrepareNodesForDraw(&data->resource->model, data->resource->model.nodes.data(), data->alpha);
    Matrix4x4::Pop(1);
    data->alpha += data->fadeDelta;
    if (data->alpha <= 0.0f)
        HLib::DestroyCurrentTask();
}

// @verified - p
void UpdateTailLamp(Character *character, void *payload) {
    if (!character)
        return;
    TailLampComponent *const component = static_cast<TailLampComponent *>(payload);
    if (!component->active) {
        if (component->lightSlot != kTailLampNoLightSlot)
            SetDynamicLightEnabled(component->lightSlot, false);
        return;
    }
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(TailLampTaskCallback, 13, 0, sizeof(TailLampTaskData), kTailLampToken);
    if (!task)
        return;
    TailLampTaskData *const data = static_cast<TailLampTaskData *>(HLib::GetTaskParameter(task));
    data->fadeDelta = -0.037500001f;
    data->alpha = static_cast<float>(component->lifetime) * 0.001953125f + 0.25f;
    data->resource = component->resource;
    Matrix4x4_PushActiveTransform();
    const CharacterNode &node = character->entity.node;
    if (node.child)
        ApplyRecursiveCharacterNodeTransforms(node.child);
    Matrix4x4::preTranslate(node.position.x, node.position.y, node.position.z);
    if (node.rotation[2])
        Matrix4x4::RotateAroundZ_Wrapper(static_cast<std::uint16_t>(node.rotation[2]));
    if (node.rotation[1])
        Matrix4x4::RotateAroundY_Wrapper(static_cast<std::uint16_t>(node.rotation[1]));
    if (node.rotation[0])
        Matrix4x4::RotateAroundX_Wrapper(static_cast<std::uint16_t>(node.rotation[0]));
    Matrix4x4::GetTransformMatrix(&data->transform);
    if (component->lightSlot != kTailLampNoLightSlot) {
        Matrix4x4::preTranslate(0.0f, 0.5f, 1.2f);
        const Vector3 position{workTransformMatrix.rows[3].x, workTransformMatrix.rows[3].y, workTransformMatrix.rows[3].z};
        shenSetLightPosition(component->lightSlot, &position);
        SetDynamicLightEnabled(component->lightSlot, true);
    }
    workTransformMatrix = *(g_matrixStack.top.stack - 1);
    Matrix4x4::MultiplyWithPrePassMatrix(&data->transform);
    Matrix4x4::GetTransformMatrix(&data->transform);
    Matrix4x4::Pop(1);
}

// @verified - p
void CleanupTailLamp(Character *character, void *payload) {
    if (!character)
        return;
    TailLampComponent *const component = static_cast<TailLampComponent *>(payload);
    SetDynamicLightEnabled(component->lightSlot, false);
    if (component->resource) {
        ReleaseModelFormat7(component->resource->model);
        delete component->resource;
    }
}

} // namespace model_component_detail

bool AttachTailLampComponent(Character *character) {
    if (!character)
        return false;
    ModelComponentHeader *const existing = FindHeader(character, kTailLampToken);
    if (existing)
        return existing->update == UpdateTailLamp && existing->cleanup == CleanupTailLamp;
    TailLampComponent *const component =
        static_cast<TailLampComponent *>(AllocateModelComponent(character, kTailLampToken, sizeof(TailLampComponent), UpdateTailLamp, CleanupTailLamp));
    if (!component)
        return false;
    *component = {};
    std::unique_ptr<TailLampResource> resource = std::make_unique<TailLampResource>();
    constexpr char filename[] = "TALS500G.MT5";
    constexpr char archivePath[] = "/model/object/TALS500G.MT5";
    const std::span<const std::byte> packageEntry = FindLoadedMapPackageEntry(filename);
    if (!packageEntry.empty()) {
        resource->bytes.assign(packageEntry.begin(), packageEntry.end());
    } else {
        const std::uint64_t size = ArchiveData_GetFileSize(archivePath);
        if (size && size <= (std::numeric_limits<std::size_t>::max)()) {
            resource->bytes.resize(static_cast<std::size_t>(size));
            if (!ArchiveData_Read(archivePath, 0, resource->bytes.data(), resource->bytes.size()))
                resource->bytes.clear();
        }
    }
    if (!resource->bytes.empty() && ReadModelFormat7(resource->bytes, "model/object/TALS500G.MT5", resource->model, 0))
        component->resource = resource.release();
    component->lightSlot = GetCurrentActiveLight() == 1 ? kTailLampNoLightSlot : kTailLampLightSlot;
    component->lifetime = 128;
    if (component->lightSlot != kTailLampNoLightSlot) {
        ResetLightEntry(component->lightSlot);
        SetLightKind(component->lightSlot, 3);
        shenSetLightColour(component->lightSlot, kTailLampColour);
        shenSetLightScale2(component->lightSlot, 1.4f);
        SetDynamicLightEnabled(component->lightSlot, false);
    }
    return true;
}

} // namespace shenmue::s2
