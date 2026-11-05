#include "api/render.h"
#include "d3t/shader_load.hpp"

#include "d3t/content.hpp"

#if defined(SHENMUE_GFX_STANDALONE)
#include "gfx/standalone.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <stdexcept>
#include <string>
#endif

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

#if defined(SHENMUE_GFX_STANDALONE)
#pragma region StandaloneGraphics
void LoadStandaloneShader(const char *path, std::uint32_t shader, ShaderType type) {
    const void *source = nullptr;
    std::size_t size = 0;
    if (!gfxGetShader(path, static_cast<int>(type), &source, &size))
        throw std::runtime_error(std::string("Shader not found: ") + path);
    void *bytecode = std::malloc(size);
    if (!bytecode)
        throw std::bad_alloc{};
    std::memcpy(bytecode, source, size);
    const bool loaded = shen_render_load_shader(shader, bytecode, size, static_cast<int32_t>(type));
    if (type != ShaderType::Vertex)
        std::free(bytecode);
    if (!loaded)
        throw std::runtime_error(std::string("Cannot create shader: ") + path);
    SetShaderCleanup(shader, ShaderCleanupNoOp, nullptr);
}
#pragma endregion
#endif

#if !defined(SHENMUE_GFX_STANDALONE)
void CompileShaderAsync(void *data, std::uint32_t contentHandle, void *bytecode, std::uint64_t bytecodeLength, const char *) {
    auto *const shaderInfo = static_cast<ShaderInfo *>(data);
    if (!shaderInfo)
        return;
    shen_render_load_shader(shaderInfo->shader, bytecode, bytecodeLength, static_cast<int32_t>(shaderInfo->type));
    SetShaderCleanup(shaderInfo->shader, ShaderCleanupNoOp, nullptr);
    ProcessThreadStates(contentHandle, shaderInfo->type == ShaderType::Vertex ? 1 : 0);
}
#endif

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
#if defined(SHENMUE_GFX_STANDALONE)
#pragma region StandaloneGraphics
    (void)renderHandle;
    LoadStandaloneShader(path, shader, type);
#pragma endregion
#else
    const D3TShaderLoadTask task{path, renderHandle, 0, {CompileShaderAsync, &record->shaders[0]}, nullptr, 0, 0, {}};
    PushShaderLoadToThreadQueue(&task);
#endif
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
#if defined(SHENMUE_GFX_STANDALONE)
#pragma region StandaloneGraphics
    (void)renderHandle;
    LoadStandaloneShader(path.data(), record->shaders[0].shader, ShaderType::Vertex);
#pragma endregion
#else
    D3TShaderLoadTask task{path.data(), renderHandle, 0, {CompileShaderAsync, &record->shaders[0]}, nullptr, 0, 0, {}};
    PushShaderLoadToThreadQueue(&task);
#endif

    std::snprintf(path.data(), path.size(), "%s_ps.hlsl", shaderPath);
    record->shaders[1] = {ShaderType::Pixel, CreateShader(1)};
#if defined(SHENMUE_GFX_STANDALONE)
#pragma region StandaloneGraphics
    LoadStandaloneShader(path.data(), record->shaders[1].shader, ShaderType::Pixel);
#pragma endregion
#else
    task.thread.data = &record->shaders[1];
    task.path = path.data();
    PushShaderLoadToThreadQueue(&task);
#endif
    return handle;
}

std::uint32_t GetLoadedShader(std::uint32_t loadHandle, std::size_t shaderIndex) {
    ShaderInfo *const info = GetAsyncShaderInfo(loadHandle, shaderIndex);
    return info ? info->shader : 0;
}

} // namespace shenmue::d3t
