#pragma once

#include "d3t/light_tiles.hpp"
#include "s1/model_lighting.hpp"
#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct PostFxState {
    std::int32_t enabled;
    std::array<std::uint8_t, 0x220> parameters;
};
static_assert(sizeof(PostFxState) == 0x224);
static_assert(offsetof(PostFxState, parameters) == 4);
inline constexpr PostFxState kPostFxDefaults{.enabled = 1, .parameters = {1, 1, 0, 0, 0x3f, 0x42, 0x0f}};
extern PostFxState g_postFxState;

struct FoliageAnimationParameters {
    float bendiness;
    float maxHeight;
    float startHeight;
    float wiggleSpeed;
    float deadzone;
};

static_assert(sizeof(FoliageAnimationParameters) == 0x14);

using d3t ::g_simpleVertexBufferHandles;
using d3t ::g_simpleCpuBuffers;
using d3t ::g_simpleIndexBufferHandles;
extern FoliageAnimationParameters *g_foliageAnimationParameters;
extern std::uint32_t g_foliageAnimationParameterCount;
extern std::uint32_t g_simpleShaderLoad;
extern std::uint32_t g_simpleShaderTemporaryBuffer;
extern std::uint32_t g_simpleShaderFileHandle;
extern std::uint32_t g_postFxTexture;
extern float g_simpleShaderMainParameter;
extern std::uint32_t g_postFxEnabled;
extern std::int32_t g_HasPPFXRendered;
bool IsPostProcessPass();
void UpdatePPFX();


void GPUPostEffect_InitializeAutoExposure(std::int32_t effect, float minimum, float maximum);
void GPUPostEffect_SetAutoExposure(std::int32_t effect);

void InitPostFX();
using d3t::AllocateSimpleShaderBuffers;
std::uint32_t LoadSimpleShader();

} // namespace shenmue::s1
