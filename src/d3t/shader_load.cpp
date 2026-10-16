#include "d3t/shader_load.hpp"

#include "d3t/content.hpp"

#include <array>
#include <atomic>
#include <cstdio>

namespace shenmue::d3t {
namespace {

struct ShaderInfo {
    ShaderType type;
    std::uint32_t shader;
};

struct AsyncShaderRecord {
    std::atomic<std::uint16_t> generation;
    std::byte reserved_02[0x0e];
    std::array<ShaderInfo, 8> shaders;
};

static_assert(sizeof(ShaderInfo) == 8);
static_assert(sizeof(AsyncShaderRecord) == 0x50);
static_assert(offsetof(AsyncShaderRecord, shaders) == 0x10);

std::array<AsyncShaderRecord, 0x2000> asyncShaderRecords{};
std::atomic<std::uint16_t> nextAsyncShaderRecord{0};

void ShaderCleanupNoOp(void *) {
}

void CompileShaderAsync(void *data, std::uint32_t contentHandle, void *bytecode, std::uint64_t bytecodeLength, const char *) {
    auto *const shaderInfo = static_cast<ShaderInfo *>(data);
    if (!shaderInfo)
        return;
    CreateShaderResource(shaderInfo->shader, bytecode, bytecodeLength, shaderInfo->type);
    SetShaderCleanup(shaderInfo->shader, ShaderCleanupNoOp, nullptr);
    ProcessThreadStates(contentHandle, shaderInfo->type == ShaderType::Vertex ? 1 : 0);
}

AsyncShaderRecord *AllocateAsyncShaderRecord(std::uint32_t &handle) {
    const std::uint16_t index = static_cast<std::uint16_t>(nextAsyncShaderRecord.fetch_add(1, std::memory_order_seq_cst) & 0x1fffU);
    AsyncShaderRecord &record = asyncShaderRecords[index];
    const std::uint16_t generation = static_cast<std::uint16_t>(record.generation.fetch_add(1, std::memory_order_seq_cst) + 1);
    handle = (static_cast<std::uint32_t>(generation) << 16U) | index;
    return generation != 0 ? &record : nullptr;
}

ShaderInfo *GetAsyncShaderInfo(std::uint32_t handle, std::size_t shaderIndex) {
    const std::uint16_t index = static_cast<std::uint16_t>(handle);
    if (index >= asyncShaderRecords.size() || shaderIndex >= 8)
        return nullptr;
    AsyncShaderRecord &record = asyncShaderRecords[index];
    return record.generation.load(std::memory_order_seq_cst) == static_cast<std::uint16_t>(handle >> 16U) ? &record.shaders[shaderIndex] : nullptr;
}

} // namespace

std::uint32_t d3t_AllocateAsyncLoadRecord(void **data) {
    std::uint32_t handle = 0;
    AsyncShaderRecord *const record = AllocateAsyncShaderRecord(handle);
    *data = record && record->generation.load(std::memory_order_seq_cst) == static_cast<std::uint16_t>(handle >> 16U) ? record->shaders.data() : nullptr;
    return handle;
}

void *d3t_GetAsyncLoadRecord(std::uint32_t handle) {
    const std::uint16_t index = static_cast<std::uint16_t>(handle);
    if (index >= asyncShaderRecords.size())
        return nullptr;
    AsyncShaderRecord &record = asyncShaderRecords[index];
    return record.generation.load(std::memory_order_seq_cst) == static_cast<std::uint16_t>(handle >> 16U) ? record.shaders.data() : nullptr;
}

std::uint32_t LoadShaderHLSL(const char *path, std::uint32_t renderHandle, std::uint32_t shader, ShaderType type) {
    std::uint32_t handle = 0;
    AsyncShaderRecord *const record = AllocateAsyncShaderRecord(handle);
    if (!record)
        return handle;
    record->shaders[0] = {type, shader};
    const D3TShaderLoadTask task{path, renderHandle, 0, {CompileShaderAsync, &record->shaders[0]}, nullptr, 0, 0, {}};
    PushShaderLoadToThreadQueue(&task);
    return handle;
}

std::uint32_t LoadPrecompiledShader(const char *shaderPath, std::uint32_t renderHandle) {
    std::uint32_t handle = 0;
    AsyncShaderRecord *const record = AllocateAsyncShaderRecord(handle);
    if (!record)
        return handle;

    std::array<char, 240> path{};
    std::snprintf(path.data(), path.size(), "%s_vs.hlsl", shaderPath);
    record->shaders[0] = {ShaderType::Vertex, CreateShader(0)};
    D3TShaderLoadTask task{path.data(), renderHandle, 0, {CompileShaderAsync, &record->shaders[0]}, nullptr, 0, 0, {}};
    PushShaderLoadToThreadQueue(&task);

    std::snprintf(path.data(), path.size(), "%s_ps.hlsl", shaderPath);
    record->shaders[1] = {ShaderType::Pixel, CreateShader(1)};
    task.thread.data = &record->shaders[1];
    task.path = path.data();
    PushShaderLoadToThreadQueue(&task);
    return handle;
}

std::uint32_t GetLoadedShader(std::uint32_t loadHandle, std::size_t shaderIndex) {
    ShaderInfo *const info = GetAsyncShaderInfo(loadHandle, shaderIndex);
    return info ? info->shader : 0;
}

} // namespace shenmue::d3t
