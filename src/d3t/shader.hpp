#pragma once

#include <cstdint>

namespace shenmue::d3t {

enum class ShaderType : std::int32_t {
    Vertex = 0,
    Pixel = 1,
    Compute = 2,
};

using ShaderCleanup = void (*)(void *);

std::uint32_t CreateShader(std::int32_t type);
bool RetainShader(std::uint32_t shader);
void DestroyShader(std::uint32_t shader);
void DestroyAllShaders();
bool CreateShaderResource(std::uint32_t shader, const void *bytecode, std::uint64_t bytecodeLength, ShaderType type);
void SetShaderCleanup(std::uint32_t shader, ShaderCleanup cleanup, void *context);
void *GetInputLayout(std::uint32_t vertexShader, std::uint32_t fvfHandle);
void *GetNativeShader(std::uint32_t shader, ShaderType expectedType);
std::uint32_t GetFVFStride(std::uint32_t fvfIndex, std::uint32_t slot);
std::uint32_t GetFVFOffset(std::uint32_t fvfIndex, std::uint32_t slot);

} // namespace shenmue::d3t
