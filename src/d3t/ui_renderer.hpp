#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace shenmue::d3t {

struct D3TDeviceContext;
class UIDefinition;
struct UITextureObjectDefinition;
struct UIPanelDefinition;
struct UIGaugeState;

struct UIPanelDrawState {
    std::string_view panelId;
    float opacity{1.0f};
};

struct UIRect {
    float x;
    float y;
    float width;
    float height;
};

struct UIResolvedLabel {
    UIRect bounds;
    UIRect clip;
};



std::vector<UIResolvedLabel> GetUILabelRectangles(const UIDefinition &definition);

struct UIResolvedTextureObject {
    UIRect bounds;
    UIRect clip;
};
struct UIPanelBoundsOverride {
    std::size_t panel;
    UIRect bounds;
};
UIRect ResolveUIPanelBounds(const UIPanelDefinition &panel, const UIRect &parent, float authoredParentWidth, float authoredParentHeight);

void ResolveUIDefinitionLayout(const UIDefinition &definition, std::span<const std::int32_t> parents, std::span<UIRect> panels,
    std::span<UIResolvedTextureObject> objects, std::span<UIResolvedLabel> labels, std::span<const std::int32_t> objectPanels,
    std::span<const std::int32_t> labelPanels, std::span<const UIPanelBoundsOverride> overrides = {}, std::array<float, 2> paddingScale = {1.0f, 1.0f});

struct UISharedSpriteImage {
    std::uint32_t textureHandle;
    UIRect source; // Pixel crop in the supplied texture, not normalized UV.
};

// Authored 1920x1080 coordinates, in textureObjects source order. Cache these
// at admission; animated owners adjust their own placements without allocation.
std::vector<UIResolvedTextureObject> GetUITextureObjectRectangles(const UIDefinition &definition);
bool DrawUITextureObject(const UIDefinition &definition, const UITextureObjectDefinition &object, const UIResolvedTextureObject &placement, float opacity,
    const UISharedSpriteImage *sharedImage = nullptr);



std::vector<UIRect> GetUIGaugeRectangles(const UIDefinition &definition);
std::uint32_t DrawUIGauge(const UIDefinition &definition, const UIGaugeState &gauge, const UIRect &bounds, float opacity);

struct UITexturedVertex {
    float x;
    float y;
    float u;
    float v;
    std::uint32_t colour; // RGBA bytes (0xAABBGGRR).
};
static_assert(sizeof(UITexturedVertex) == 0x14);

// Coordinates and scissor edges are in the active UI target's pixel space.
bool DrawUITexturedGeometry(
    std::uint32_t texture, std::span<const UITexturedVertex> vertices, std::span<const std::uint16_t> indices, const std::array<std::int32_t, 4> &scissor);
bool DrawUIRoundedRectangle(const UIRect &bounds, float radius, std::uint32_t colour, const UIRect &clip);
inline constexpr std::array<std::array<float, 2>, 20> UI_ROUNDED_RECT_DIRECTIONS{{
    {-1, 0}, {-0.9238795f, -0.38268343f}, {-0.70710677f, -0.70710677f}, {-0.38268343f, -0.9238795f}, {0, -1},
    {0, -1}, {0.38268343f, -0.9238795f}, {0.70710677f, -0.70710677f}, {0.9238795f, -0.38268343f}, {1, 0},
    {1, 0}, {0.9238795f, 0.38268343f}, {0.70710677f, 0.70710677f}, {0.38268343f, 0.9238795f}, {0, 1},
    {0, 1}, {-0.38268343f, 0.9238795f}, {-0.70710677f, 0.70710677f}, {-0.9238795f, 0.38268343f}, {-1, 0}}};

extern std::uint32_t g_sceneClearColour;
extern std::uint32_t g_sceneTexture, g_sceneDepthTexture, g_resolvedSceneTexture, g_legacySceneTexture;
struct SceneFrameTargets {
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t resolvedWidth;
    std::uint32_t resolvedHeight;
    std::uint32_t legacyWidth;
    std::uint32_t legacyHeight;
    bool supersampling;
};

bool ConfigureSceneFrameTargets(const SceneFrameTargets &targets, std::uint32_t inputLayout, std::uint32_t vertexShader, std::uint32_t pixelShader);
void SelectResolvedSceneFrame(bool resolved);
bool DrawFullscreenContrast(D3TDeviceContext *context, std::uint32_t vertexShader, std::uint32_t pixelShader, float contrast);

bool InitializeUIRenderer(std::uint32_t inputLayout, std::uint32_t vertexShader, std::uint32_t pixelShader, std::uint32_t gammaInputLayout,
    std::uint32_t gammaVertexShader, std::uint32_t gammaPixelShader);
bool BeginUIFrame();
bool BindSceneFrameRenderTarget();
bool BindUIFrameRenderTarget();




bool BeginUITextureRenderTarget(std::uint32_t texture, bool flipVertical = false);
void RestoreUIRenderTarget();
bool PresentSceneFrame();
void ReleaseSceneFrameTargets();
void SetSceneClearColour(std::uint32_t colour);
bool GetSceneFrameTargetSize(float *width, float *height);
bool GetSceneViewportDepthRange(float *minDepth, float *maxDepth);
bool SetSceneViewportDepthRange(float minDepth, float maxDepth);
bool DrawUIImage(std::uint32_t texture, float x, float y, float width, float height, float opacity);
std::uint32_t DrawUIDefinition(const UIDefinition &definition, float opacity);
std::uint32_t DrawUIPanels(const UIDefinition &definition, std::span<const UIPanelDrawState> panels);

} // namespace shenmue::d3t
