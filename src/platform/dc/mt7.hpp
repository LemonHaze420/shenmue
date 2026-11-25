#pragma once

#include "platform/dc/sdk.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#if SHENMUE_S2_GCC295
#define SHENMUE_MT7_DATA static const
#define SHENMUE_MT7_INITIALIZER =
#else
#include <bit>
#define SHENMUE_MT7_DATA inline constexpr
#define SHENMUE_MT7_INITIALIZER
#endif

namespace shenmue {
namespace dc {
namespace sdk {

SHENMUE_MT7_DATA float kMT7ViewDirectionLength = 436.5f;
SHENMUE_MT7_DATA std::size_t kMT7SpecularRows = 32, kMT7SpecularSamples = 128;
SHENMUE_MT7_DATA std::uintptr_t kStoreQueueAddressControl0 = 0xff000038;
SHENMUE_MT7_DATA std::uintptr_t kStoreQueueAddressControl1 = 0xff00003c;

struct MT7SystemDisplayMode {
    std::uint32_t videoStandard;
    std::uint32_t hardwareMode;
    std::int16_t width, height;
};
#if SHENMUE_S2_GCC295
typedef char MT7DisplayModeSizeCheck[sizeof(MT7SystemDisplayMode) == 12 ? 1 : -1];
typedef char MT7DisplayModeWidthOffsetCheck[offsetof(MT7SystemDisplayMode, width) == 8 ? 1 : -1];
#else
static_assert(sizeof(MT7SystemDisplayMode) == 12);
static_assert(offsetof(MT7SystemDisplayMode, width) == 8);
#endif
SHENMUE_MT7_DATA MT7SystemDisplayMode kMT7SystemDisplayModes[28] SHENMUE_MT7_INITIALIZER{
    {SHEN_FIELD(videoStandard) 1, SHEN_FIELD(hardwareMode) 1, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0, SHEN_FIELD(width) 320, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 4, SHEN_FIELD(width) 320, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x10, SHEN_FIELD(width) 640, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x14, SHEN_FIELD(width) 640, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x20, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x28, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x24, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x30, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x38, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 0, SHEN_FIELD(hardwareMode) 0x34, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 2, SHEN_FIELD(width) 320, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 6, SHEN_FIELD(width) 320, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x12, SHEN_FIELD(width) 640, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x16, SHEN_FIELD(width) 640, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x22, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x2a, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x26, SHEN_FIELD(width) 320, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x32, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x3a, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x36, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 1, SHEN_FIELD(hardwareMode) 1, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 1, SHEN_FIELD(hardwareMode) 1, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 1, SHEN_FIELD(hardwareMode) 1, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x7a, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x52, SHEN_FIELD(width) 640, SHEN_FIELD(height) 240},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x7a, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
    {SHEN_FIELD(videoStandard) 2, SHEN_FIELD(hardwareMode) 0x76, SHEN_FIELD(width) 640, SHEN_FIELD(height) 480},
};
SHENMUE_MT7_DATA std::int32_t kMT7FramebufferFormats[6] SHENMUE_MT7_INITIALIZER{0, 1, -1, 3, 4, 5};
#if SHENMUE_S2_GCC295
static const float kMT7PALHeightScales[5] = {1.033f, 1.066f, 1.1f, 1.133f, 1.166f};
#else
SHENMUE_MT7_DATA float kMT7PALHeightScales[5]{std::bit_cast<float>(0x3f843958U), std::bit_cast<float>(0x3f8872b0U), std::bit_cast<float>(0x3f8ccccdU),
    std::bit_cast<float>(0x3f910625U), std::bit_cast<float>(0x3f953f7dU)};
#endif

struct MT7LightingParameters {
    Vector3 viewDirection;
    float negativeNearClip;
    Vector3 lightDirection;
    float ambient;
    float inverseNearClip;
    float nearClip;
    float farClip;
};
struct MT7DrawControls {
    std::uint32_t flags;
    std::uint32_t tsp;
};
struct MT7GeometryLight {
    std::uint8_t enabled, kind;
    std::int16_t coneAngle;
    Vector4 colour;
    float intensity, range, falloff;
    Vector3 position, direction;
};
#if SHENMUE_S2_GCC295
typedef char MT7GeometryLightSizeCheck[sizeof(MT7GeometryLight) == 0x38 ? 1 : -1];
typedef char MT7GeometryLightColourOffsetCheck[offsetof(MT7GeometryLight, colour) == 4 ? 1 : -1];
typedef char MT7GeometryLightPositionOffsetCheck[offsetof(MT7GeometryLight, position) == 0x20 ? 1 : -1];
typedef char MT7GeometryLightDirectionOffsetCheck[offsetof(MT7GeometryLight, direction) == 0x2c ? 1 : -1];
typedef char MT7DrawControlsSizeCheck[sizeof(MT7DrawControls) == 8 ? 1 : -1];
typedef char MT7DrawControlsTspOffsetCheck[offsetof(MT7DrawControls, tsp) == 4 ? 1 : -1];
typedef char MT7LightingParametersSizeCheck[sizeof(MT7LightingParameters) == 0x2c ? 1 : -1];
typedef char MT7LightingParametersLightDirectionOffsetCheck[offsetof(MT7LightingParameters, lightDirection) == 0x10 ? 1 : -1];
typedef char MT7LightingParametersAmbientOffsetCheck[offsetof(MT7LightingParameters, ambient) == 0x1c ? 1 : -1];
typedef char MT7LightingParametersNearClipOffsetCheck[offsetof(MT7LightingParameters, nearClip) == 0x24 ? 1 : -1];
#else
static_assert(sizeof(MT7GeometryLight) == 0x38);
static_assert(offsetof(MT7GeometryLight, colour) == 4);
static_assert(offsetof(MT7GeometryLight, position) == 0x20);
static_assert(offsetof(MT7GeometryLight, direction) == 0x2c);
static_assert(sizeof(MT7DrawControls) == 8);
static_assert(offsetof(MT7DrawControls, tsp) == 4);
static_assert(sizeof(MT7LightingParameters) == 0x2c);
static_assert(offsetof(MT7LightingParameters, lightDirection) == 0x10);
static_assert(offsetof(MT7LightingParameters, ambient) == 0x1c);
static_assert(offsetof(MT7LightingParameters, nearClip) == 0x24);
#endif

extern MT7LightingParameters g_mt7LightingParameters;
extern Matrix4x4 g_mt7ScreenTransform;
extern Matrix4x4 g_mt7UnmirroredScreenTransform;
extern Matrix4x4 g_mt7ViewportTransform;
extern Matrix4x4 g_mt7ProjectionTransform;
extern Vector4 g_mt7ProjectionFrustumPlanes[4];
extern float g_mt7ProjectionOffsetX, g_mt7ProjectionOffsetY;
extern float g_mt7HorizontalClipCoefficient, g_mt7VerticalClipCoefficient;
extern float g_mt7HorizontalClipScale, g_mt7VerticalClipScale, g_mt7ProjectionExtent;
extern float g_mt7NearClip, g_mt7FarClip, g_mt7InverseNearClip;
extern std::int16_t g_mt7DisplayWidth, g_mt7DisplayHeight, g_mt7Supersampling;
extern std::uint32_t g_mt7ExtendedPAL, g_mt7PALHeightRatio;
extern std::int32_t g_mt7FramebufferFormat;
extern float g_mt7ViewportX, g_mt7ViewportY, g_mt7ViewportWidth, g_mt7ViewportHeight;
extern Vector4 g_mt7SingleLightDirection;
extern Vector3 g_mt7ViewDirection;
extern Vector3 g_mt7ViewAxis;
extern Vector3 g_mt7ReflectedWorldDirection, g_mt7ReflectedModelDirection;
extern Vector3 *g_mt7ReflectedDirection;
extern std::int32_t g_mt7AlternateLighting;
extern float g_mt7AmbientIntensity;
extern float g_mt7SpecularLookup[kMT7SpecularRows][kMT7SpecularSamples];
extern const void *g_mt7PaletteData;
extern MT7DrawControls g_mt7DrawControls asm("_shenmue_dc_mt7_draw_controls");
extern Vector4 g_mt7DiffuseChannels, g_mt7SpecularChannels;
extern Callback g_mt7VertexColourCallback;
extern std::uint32_t g_mt7AlphaBlendMask;
extern const std::uint8_t g_mt7VertexAngleLookup[128];
extern std::byte g_mt7PacketScratch[0x70];
extern std::byte g_mt7FloatingLightWorkspace[0x254], g_mt7PackedLightWorkspace[0x294];
extern Vector2 g_fftForwardTwiddles[32];
extern Vector2 g_fftBackwardTwiddles[32];

void InitializeMT7Materials();
void SetMT7AmbientIntensity(float value);
void SetMT7SingleLightDirection(float x, float y, float z);
void SetMT7SingleLightDirection(const Vector3 *direction);
void SetMT7ReflectedDirection(const Vector3 *direction);
void SetMT7AlphaBlendMode(std::uint32_t source, std::uint32_t destination);
std::int32_t GetMT7VertexColourType() asm("_shenmue_dc_mt7_get_vertex_colour_type");
void SetMT7VertexColourType(std::int32_t type);
void ProcessMT7FloatVertexColours() asm("_shenmue_dc_mt7_float_vertex_colours");
void ProcessMT7PackedVertexColours() asm("_shenmue_dc_mt7_packed_vertex_colours");
void ResetMT7GeometryLightMesh(void *mesh);
std::int32_t BakeMT7GeometryLightMesh(void *mesh, std::int32_t lightCount, const void *lights, const Vector4 *baseColour);
void MT7TexturePreparationNullsub();
void SetMT7PolygonFogMode(std::uint32_t mode);
void SetMT7PolygonColourClamp(std::uint32_t enabled);
void SetMT7UserClipMode(std::uint32_t mode);
std::uint32_t PrepareMT7Mesh(void *mesh, const void *textures);
std::uint32_t PrepareMT7MeshMaterial(void *mesh, const void *textures, std::uint32_t *material, std::uint32_t **next, std::int32_t remap);
void PrepareMT7SingleLight();
void PrepareMT7LightTransforms();
void PrepareMT7Projection();
void BuildMT7ViewportTransform(Matrix4x4 *matrix);
void SetMT7PALHeightRatio(std::uint32_t ratio);
void SetMT7PixelClipping(float x, float y, float width, float height);
void SetMT7Viewport(float x, float y, float width, float height);
void RefreshMT7ScreenTransform();
void SetMT7Perspective(std::int32_t angle, float aspectScale, float nearClip, float farClip);
void SubmitSingleLightMT7Mesh(void *mesh);
void SubmitSingleLightMT7MeshAlpha(void *mesh, float alpha);
void SubmitLitMT7Mesh(void *mesh);
void SubmitLitMT7MeshAlpha(void *mesh, float alpha);
void ExecuteSingleLightMT7Packets(void *mesh, MT7LightingParameters *lighting) asm("_shenmue_dc_mt7_single_packets");
void ExecuteSingleLightMT7PacketsAlpha(void *mesh, MT7LightingParameters *lighting, float alpha) asm("_shenmue_dc_mt7_single_packets_alpha")
    __attribute__((renesas));
void ExecuteLitMT7Packets(void *mesh, MT7LightingParameters *lighting) asm("_shenmue_dc_mt7_lit_packets");
void ExecuteLitMT7PacketsAlpha(void *mesh, MT7LightingParameters *lighting, float alpha) asm("_shenmue_dc_mt7_lit_packets_alpha") __attribute__((renesas));
void ExecuteAlternateLitMT7Packets(void *mesh, MT7LightingParameters *lighting) asm("_shenmue_dc_mt7_alternate_packets");
void ExecuteAlternateLitMT7PacketsAlpha(void *mesh, MT7LightingParameters *lighting, float alpha) asm("_shenmue_dc_mt7_alternate_packets_alpha")
    __attribute__((renesas));
void TransposeMT7WorkMatrix();
void MultiplyMT7WorkMatrix(const Matrix4x4 *matrix) asm("_shenmue_dc_mt7_multiply_work_matrix");
void LoadMT7RasterMatrix(const Matrix4x4 *matrix) asm("_shenmue_dc_mt7_load_raster_matrix");
void MultiplyMT7ProjectionRows(const Matrix4x4 *matrix) asm("_shenmue_dc_mt7_multiply_projection_rows");

} // namespace sdk
} // namespace dc
} // namespace shenmue

namespace shenmue {
namespace s2 {
std::int32_t ReleaseTextureIndex(std::int32_t index);
}
} // namespace shenmue

#undef SHENMUE_MT7_DATA
#undef SHENMUE_MT7_INITIALIZER

#endif
