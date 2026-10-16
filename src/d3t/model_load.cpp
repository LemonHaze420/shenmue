#include "d3t/model.hpp"
#include "d3t/content.hpp"
#include "d3t/material.hpp"
#include "d3t/shader_load.hpp"
#include "shenmue.h"

#if SHENMUE_IS_S1
#include "s1/initialization.hpp"
#elif SHENMUE_IS_S2
#include "s2/renderer_initialization.hpp"
#endif

#include <mutex>

namespace shenmue::d3t {
#if SHENMUE_IS_S1
using s1::g_doLowMipsOnly;
#elif SHENMUE_IS_S2
using s2::g_doLowMipsOnly;
#endif
namespace {
std::mutex g_d3tModelAllocationLock;

void ModelReferenceLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t, const char *sourcePath) {
    D3TModelReferenceLoad *const load = static_cast<D3TModelReferenceLoad *>(data);
    *load->modelHandle = AllocateModelHandle();
    D3TModelRecord *const model = ResolveModel(*load->modelHandle);
    if (!model) {
        ProcessThreadStates(contentHandle, 0);
        return;
    }
    ReadModel(*model, loadedData, sourcePath, g_doLowMipsOnly);
    CreateModelMaterials(*model, load->instanceHandle, g_doLowMipsOnly);
    ProcessThreadStates(contentHandle, 1);
}

void QueueModelReferenceLoad(const char *path, std::uint32_t instanceHandle, std::uint32_t *modelHandle) {
    void *data = nullptr;
    d3t_AllocateAsyncLoadRecord(&data);
    if (!data)
        return;
    D3TModelReferenceLoad *const load = static_cast<D3TModelReferenceLoad *>(data);
    load->modelHandle = modelHandle;
    load->instanceHandle = instanceHandle;
    const D3TShaderLoadTask task{.path = path,
        .renderHandle = instanceHandle,
        .reserved_0c = 0,
        .thread = {.callback = ModelReferenceLoaded, .data = data},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    PushShaderLoadToThreadQueue(&task);
}

void QueueModelPartLoads(D3TModelRecord &model, std::uint32_t instanceHandle) {
    for (std::uint32_t index = 0; index < model.partCount; ++index) {
        const D3TModelPart &part = model.parts[index];
        for (std::uint32_t reference = 0; reference < part.pathCount; ++reference) {
            const std::uint32_t offset = part.pathOffsets[reference];
            const char *const path = offset < model.stringsSize ? model.strings + offset : nullptr;
            if (path && *path == '/')
                QueueModelReferenceLoad(path, instanceHandle, &model.partHandles[index * 3 + reference]);
        }
    }
}

void ModelLoaded(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t, const char *sourcePath) {
    D3TModelLoad *const load = static_cast<D3TModelLoad *>(data);
    {
        const std::lock_guard lock(g_d3tModelAllocationLock);
        load->modelHandle = AllocateModelHandle();
    }
    D3TModelRecord *const model = ResolveModel(load->modelHandle);
    if (!model) {
        ProcessThreadStates(contentHandle, 0);
        return;
    }
    ReadModel(*model, loadedData, sourcePath, g_doLowMipsOnly);
    model->loadState = 0;
    CreateModelMaterials(*model, load->instanceHandle, g_doLowMipsOnly);
    for (std::uint32_t index = 0; index < model->referenceCount; ++index) {
        const std::uint32_t offset = model->references[index].pathOffset;
        const char *const path = offset < model->stringsSize ? model->strings + offset : nullptr;
        if (path && *path == '/')
            QueueModelReferenceLoad(path, load->instanceHandle, &model->referenceHandles[index]);
    }
    QueueModelPartLoads(*model, load->instanceHandle);
    ProcessThreadStates(contentHandle, 1);
}
} // namespace

std::uint32_t DispatchModelLoad(const char *path, std::uint32_t instanceHandle) {
    void *data = nullptr;
    const std::uint32_t handle = d3t_AllocateAsyncLoadRecord(&data);
    if (!(handle >> 16U))
        return handle;
    D3TModelLoad *const load = static_cast<D3TModelLoad *>(data);
    load->instanceHandle = instanceHandle;
    load->modelHandle = 0;
    const D3TShaderLoadTask task{.path = path,
        .renderHandle = instanceHandle,
        .reserved_0c = 0,
        .thread = {.callback = ModelLoaded, .data = data},
        .destination = nullptr,
        .destinationCapacity = 0,
        .option = 0,
        .reserved_31 = {}};
    PushShaderLoadToThreadQueue(&task);
    return handle;
}
} // namespace shenmue::d3t
