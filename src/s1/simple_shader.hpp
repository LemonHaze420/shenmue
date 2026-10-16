#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct FoliageAnimationParameters {
    float bendiness;
    float maxHeight;
    float startHeight;
    float wiggleSpeed;
    float deadzone;
};

static_assert(sizeof(FoliageAnimationParameters) == 0x14);

struct SimpleShaderParameterBlock {
    std::array<std::byte, 0x138> data;
};

static_assert(sizeof(SimpleShaderParameterBlock) == 0x138);

extern std::array<SimpleShaderParameterBlock, 16> g_simpleShaderParameterBlocks;
extern std::array<std::uint32_t, 4> g_simpleVertexBufferHandles;
extern std::array<void *, 4> g_simpleCpuBuffers;
extern std::array<std::uint32_t, 4> g_simpleIndexBufferHandles;
extern FoliageAnimationParameters *g_foliageAnimationParameters;
extern std::uint32_t g_foliageAnimationParameterCount;
extern std::uint32_t g_simpleShaderLoad;
extern std::uint32_t g_simpleShaderTemporaryBuffer;
extern std::uint32_t g_simpleShaderFileHandle;
extern std::uint32_t g_postFxTexture;
extern float g_simpleShaderMainParameter;
extern std::uint32_t g_simpleShaderMode;
extern std::uint32_t g_postFxEnabled;
extern std::int32_t g_HasPPFXRendered;
bool IsPostProcessPass();
void UpdatePPFX();


void GPUPostEffect_InitializeAutoExposure(std::int32_t effect, float minimum, float maximum);
void GPUPostEffect_SetAutoExposure(std::int32_t effect);

void InitPostFX();
void AllocateSimpleShaderBuffers();
std::uint32_t LoadSimpleShader();

} // namespace shenmue::s1
