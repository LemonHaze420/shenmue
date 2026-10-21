#pragma once

#include "d3t/buffer.hpp"
#include "d3t/ui_runtime.hpp"

#include "common/math.hpp"
#include "s1/screen_space.hpp"

#include <array>
#include <cstdint>
#include <span>
#include <cstddef>

namespace shenmue::s1 {

using D3TDeviceContext = d3t::D3TDeviceContext;

struct RenderPassHandles {
    std::uint32_t texture2D;
};

enum class DrawPassOpType : std::uint32_t {
    None = 0,
    GammaCorrect = 1,
    PreserveSourceAspect = 2,
    AlphaBlend = 4,
};

void CreateTexture2DInstance(D3TDeviceContext *context);
std::int64_t DrawSuperSampled(D3TDeviceContext *context, std::uint32_t texture2D, std::uint32_t outputTexture2D);
void DrawCharacterShadowComposite(D3TDeviceContext *context);
D3TDeviceContext *DrawFullPass_Quad(const RenderPassHandles *renderPass, DrawPassOpType passType);
void Do_contrast_Pass();
void HangOnRender_DrawQuad(std::uint32_t texture2D);
struct NuklearQuad {
    float x;
    float y;
    float width;
    float height;
    float u0;
    float v0;
    float u1;
    float v1;
    std::uint32_t colour;

    float rotation{};
};

bool BeginNuklearFrame(D3TDeviceContext *context, std::uint32_t renderTarget);
bool DrawNuklearImage(
    D3TDeviceContext *context, std::uint32_t renderTarget, std::uint32_t texture2D, float x, float y, float width, float height, float opacity);
bool DrawNuklearQuads(D3TDeviceContext *context, std::uint32_t renderTarget, std::uint32_t texture2D, std::span<const NuklearQuad> quads);
void DrawTexturedQuad(D3TDeviceContext *context, const std::uint32_t *texture2D);
bool CompositeNuklearFrame(D3TDeviceContext *context, std::uint32_t texture2D);



extern Vector4 g_currentUIViewportRect;
extern Vector2 g_currentUIViewportDepth;


extern bool g_bOrthographicRender;
extern Matrix4x4 g_notebookPageMatrix;


extern std::array<float, 2> g_nuklearArmedImageSize;

extern bool g_nuklearTargetRedirectPending;
extern const std::uint32_t *g_nuklearTargetRedirect;

UIViewport AdjustScreenForAspectRatio(std::int32_t mode);
void CreateOrthographicViewMatrix(std::int32_t width, std::int32_t height, Matrix4x4 *matrix);
void Nuklear_BindRenderTarget();
void NuklearUpdateForAspectRatio(std::int32_t mode);


void SetRenderTarget(D3TDeviceContext *context, std::uint32_t texture2D);
void SetViewport(D3TDeviceContext *context, float width, float height);
void SetShaderState(D3TDeviceContext *context, std::uint32_t vertexShader, std::uint32_t pixelShader, std::uint32_t inputLayout);
void SetCommonQuadState(D3TDeviceContext *context, std::uint64_t blend);
void SetTopology(D3TDeviceContext *context, std::uint32_t topology);
void BindPixelTexture8(D3TDeviceContext *context, std::uint32_t texture2D);
void SubmitQuad(D3TDeviceContext *context);

void DrawVertices_original_strip00_HN(std::int32_t vertexCount, const float *packet);
void CalculateViewCameraTransforms(const float *packet, std::int32_t texture);
void Do_original_strip_03_Pass_HN(void *argument);
void SetRenderFlags_4(const float *packet);
void ApplyFadeToPacket(void *rawStrip, float maxExtent);
void UnmapAndDraw_SkyRelated_2D(const float *packet, std::int32_t surface);
void SetRenderFlags_UnmapAndDraw(const float *packet, std::int32_t surface);
void SetRenderFlags_6(const float *packet, std::int32_t surface);

struct ProjectedOriginalStripDepths {
    float corner0;
    float corner1;
    float corner2;
    float corner3;
};
struct EffeMaterialWords {
    std::uint32_t texture;
    std::uint32_t render;
    std::uint32_t alpha;
};

inline constexpr std::size_t kOriginalStrip03BatchQuadCapacity = 4000U / 6U;
std::uint32_t DrawProjectedOriginalStrip03Batch(D3TDeviceContext *context, std::span<const OriginalStrip> strips,
    std::span<const ProjectedOriginalStripDepths> depths, std::uint32_t texture2D, bool transparent, const EffeMaterialWords *material = nullptr);


struct EffeParticleVertex {
    Vector4 position;
    std::uint32_t diffuse;
    float u;
    float v;
};
static_assert(sizeof(EffeParticleVertex) == 0x1c);
static_assert(offsetof(EffeParticleVertex, diffuse) == 0x10);
static_assert(offsetof(EffeParticleVertex, u) == 0x14);

EffeParticleVertex *MapEffeParticleVertices(D3TDeviceContext *context, std::uint32_t particleCount);
std::uint32_t CommitEffeParticleVertices(
    D3TDeviceContext *context, std::uint32_t particleCount, std::uint32_t texture2D, bool transparent, const EffeMaterialWords *material = nullptr);



struct HmdlInstanceRecord {
    Vector3 position;
    float cornerOffsets[4];
    float textureRectangle[4];
    std::uint32_t colour;
    std::uint32_t particleAddressLow;
};
static_assert(sizeof(HmdlInstanceRecord) == 0x34);
static_assert(offsetof(HmdlInstanceRecord, cornerOffsets) == 0x0c);
static_assert(offsetof(HmdlInstanceRecord, textureRectangle) == 0x1c);
static_assert(offsetof(HmdlInstanceRecord, colour) == 0x2c);
static_assert(offsetof(HmdlInstanceRecord, particleAddressLow) == 0x30);



bool InitializeHmdlInstanceStorage(std::uint32_t renderEntryHandle, std::uint32_t instanceCapacity);
bool UploadHmdlInstanceRecords(D3TDeviceContext *context, std::uint64_t skinningKey, std::span<const HmdlInstanceRecord> instances);
void ReleaseHmdlInstanceCpuMirrors();

extern D3TDeviceContext *g_RenderDeviceContext;
extern float g_fContrastValue;
extern float g_fGammaValue;
extern bool g_fFxaaEnabled;
extern bool g_fBloomEnabled;

} // namespace shenmue::s1
