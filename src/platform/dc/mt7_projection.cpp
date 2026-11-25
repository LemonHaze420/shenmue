#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"

namespace shenmue {
namespace dc {
namespace sdk {

__attribute__((aligned(32))) Matrix4x4 g_mt7UnmirroredScreenTransform = {};
__attribute__((aligned(32))) Matrix4x4 g_mt7ViewportTransform = {};
__attribute__((aligned(32))) Matrix4x4 g_mt7ProjectionTransform = {};
Vector4 g_mt7ProjectionFrustumPlanes[4];
float g_mt7ProjectionOffsetX = 0.0f, g_mt7ProjectionOffsetY = 0.0f;
float g_mt7HorizontalClipCoefficient = 0.0f, g_mt7VerticalClipCoefficient = 0.0f;
float g_mt7HorizontalClipScale = 0.0f, g_mt7VerticalClipScale = 0.0f, g_mt7ProjectionExtent = 0.0f;
float g_mt7NearClip, g_mt7FarClip = 0.0f, g_mt7InverseNearClip = 0.0f;
std::int16_t g_mt7DisplayWidth = 0, g_mt7DisplayHeight = 0, g_mt7Supersampling = 0;
std::uint32_t g_mt7ExtendedPAL = 0, g_mt7PALHeightRatio = 0;
std::int32_t g_mt7FramebufferFormat = 0;
float g_mt7ViewportX = 0.0f, g_mt7ViewportY = 0.0f, g_mt7ViewportWidth = 0.0f, g_mt7ViewportHeight = 0.0f;

namespace {
extern "C" std::int32_t kmSetPixelClipping(std::int32_t left, std::int32_t top, std::int32_t right, std::int32_t bottom);
} // namespace

// @verified - d
void SetMT7PALHeightRatio(std::uint32_t ratio) {
    g_mt7PALHeightRatio = ratio;
}

// @verified - d
void SetMT7PixelClipping(float x, float y, float width, float height) {
    std::int32_t left = static_cast<std::int32_t>(x), top = static_cast<std::int32_t>(y);
    std::int32_t right = static_cast<std::int32_t>(x + width), bottom = static_cast<std::int32_t>(y + height);
    if (left < 0)
        left = 0;
    else if (left >= g_mt7DisplayWidth)
        left = g_mt7DisplayWidth;
    if (right < 0)
        right = 0;
    else if (right >= g_mt7DisplayWidth)
        right = g_mt7DisplayWidth;
    if (top < 0)
        top = 0;
    else if (top >= g_mt7DisplayHeight)
        top = g_mt7DisplayHeight;
    if (bottom < 0)
        bottom = 0;
    else if (bottom >= g_mt7DisplayHeight)
        bottom = g_mt7DisplayHeight;
    if (g_mt7ExtendedPAL == 1) {
        float scale = 1.0f;
        if (g_mt7PALHeightRatio < 5)
            scale = kMT7PALHeightScales[g_mt7PALHeightRatio];
        top = static_cast<std::int32_t>(static_cast<float>(top) * scale);
        if (g_mt7FramebufferFormat != 4) {
            --right;
            --bottom;
        }
        bottom = static_cast<std::int32_t>(static_cast<float>(bottom) * scale);
    } else if (g_mt7FramebufferFormat != 4) {
        --right;
        --bottom;
    }
    kmSetPixelClipping(left, top, right, bottom);
}

// @verified - d
void SetMT7Viewport(float x, float y, float width, float height) {
    float effectiveY = y, effectiveHeight = height;
    if (g_mt7Supersampling && g_mt7DisplayHeight * 2 <= 480) {
        effectiveY = y + y;
        effectiveHeight = height + height;
    }
    g_mt7ViewportX = x;
    g_mt7ViewportY = y;
    g_mt7ViewportWidth = width;
    g_mt7ViewportHeight = height;
    matrix_stack::g_projectionViewportOriginY = effectiveY;
    matrix_stack::g_projectionViewportHeight = effectiveHeight;
    if (g_mt7Supersampling) {
        matrix_stack::g_projectionViewportOriginX = x + x;
        matrix_stack::g_projectionViewportWidth = width + width;
    } else {
        matrix_stack::g_projectionViewportOriginX = x;
        matrix_stack::g_projectionViewportWidth = width;
    }
    g_mt7HorizontalClipScale = g_mt7HorizontalClipCoefficient * matrix_stack::g_projectionViewportWidth;
    g_mt7VerticalClipScale = g_mt7VerticalClipCoefficient * matrix_stack::g_projectionViewportHeight;
    g_mt7ProjectionExtent = g_mt7HorizontalClipScale > g_mt7VerticalClipScale ? g_mt7HorizontalClipScale : g_mt7VerticalClipScale;
    BuildMT7ViewportTransform(&g_mt7ViewportTransform);
    SetMT7PixelClipping(x, y, width, height);
}

// @verified - d
void BuildMT7ViewportTransform(Matrix4x4 *matrix) {
    const float halfWidth = matrix_stack::g_projectionViewportWidth * 0.5f;
    const float halfHeight = -matrix_stack::g_projectionViewportHeight * 0.5f;
    const float x = halfWidth + matrix_stack::g_projectionViewportOriginX;
    float y = matrix_stack::g_projectionViewportOriginY;
    const float half = 0.5f;
    const float height = matrix_stack::g_projectionViewportHeight;
    asm volatile("fmov %1,fr0\n\tfmac fr0,%2,%0" : "+f"(y) : "f"(height), "f"(half) : "fr0");
    *matrix = (Matrix4x4){SHEN_FIELD(rows){{SHEN_FIELD(x) halfWidth, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) halfHeight, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) - 0.5f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) x, SHEN_FIELD(y) y, SHEN_FIELD(z) half, SHEN_FIELD(w) 1.0f}}};
}

// @verified - d
void RefreshMT7ScreenTransform() {
    matrix_stack::GetMatrix(&g_mt7ProjectionTransform);
    LoadMT7RasterMatrix(&g_mt7ViewportTransform);
    MultiplyMT7ProjectionRows(&g_mt7ProjectionTransform);
    matrix_stack::GetMatrix(&g_mt7ScreenTransform);
    matrix_stack::GetMatrix(&g_mt7UnmirroredScreenTransform);
    matrix_stack::LoadMatrix(&g_mt7ProjectionTransform);
}

// @verified - d
void SetMT7Perspective(std::int32_t angle, float aspectScale, float nearClip, float farClip) {
    const std::int32_t halfAngle = angle >> 1;
    const float cotangent = 1.0f / TanFixed(halfAngle);
    const std::uint32_t horizontalAngle = ArcTanFixed(aspectScale / cotangent);
    float horizontalSecant;
    if (g_mt7ProjectionOffsetX != 0.0f) {
        const float tangent = TanFixed(static_cast<std::int32_t>(horizontalAngle));
        const std::uint32_t adjusted = ArcTanFixed(tangent * (g_mt7ProjectionOffsetX + 1.0f));
        horizontalSecant = 1.0f / CosFixed(static_cast<std::int32_t>(adjusted));
    } else {
        horizontalSecant = 1.0f / CosFixed(static_cast<std::int32_t>(horizontalAngle));
    }
    float verticalSecant;
    if (g_mt7ProjectionOffsetY != 0.0f) {
        const float tangent = TanFixed(halfAngle);
        const std::uint32_t adjusted = ArcTanFixed(tangent * (g_mt7ProjectionOffsetY + 1.0f));
        verticalSecant = 1.0f / CosFixed(static_cast<std::int32_t>(adjusted));
    } else {
        verticalSecant = 1.0f / CosFixed(halfAngle);
    }
    g_mt7ProjectionOffsetX = 0.0f;
    g_mt7ProjectionOffsetY = 0.0f;
    float horizontalScale = cotangent / aspectScale;
    horizontalScale *= 0.5f;
    horizontalScale *= horizontalSecant;
    float verticalScale = cotangent * 0.5f;
    verticalScale *= verticalSecant;
    g_mt7HorizontalClipCoefficient = horizontalScale;
    g_mt7VerticalClipCoefficient = verticalScale;
    g_mt7HorizontalClipScale = horizontalScale * matrix_stack::g_projectionViewportWidth;
    g_mt7VerticalClipScale = verticalScale * matrix_stack::g_projectionViewportHeight;
    g_mt7ProjectionExtent = g_mt7HorizontalClipScale > g_mt7VerticalClipScale ? g_mt7HorizontalClipScale : g_mt7VerticalClipScale;

    const std::int32_t horizontalHalfAngle = static_cast<std::int32_t>(ArcTanFixed(TanFixed(halfAngle) * aspectScale));
    const float horizontalCosine = CosFixed(horizontalHalfAngle);
    const float horizontalSine = SinFixed(horizontalHalfAngle);
    const float verticalCosine = CosFixed(halfAngle);
    const float verticalSine = SinFixed(halfAngle);
    g_mt7ProjectionFrustumPlanes[0] = (Vector4){SHEN_FIELD(x) horizontalCosine, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) horizontalSine, SHEN_FIELD(w) 0.0f};
    g_mt7ProjectionFrustumPlanes[1] = (Vector4){SHEN_FIELD(x) - horizontalCosine, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) horizontalSine, SHEN_FIELD(w) 0.0f};
    g_mt7ProjectionFrustumPlanes[2] = (Vector4){SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) verticalCosine, SHEN_FIELD(z) verticalSine, SHEN_FIELD(w) 0.0f};
    g_mt7ProjectionFrustumPlanes[3] = (Vector4){SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) - verticalCosine, SHEN_FIELD(z) verticalSine, SHEN_FIELD(w) 0.0f};

    const float depthRange = nearClip - farClip;
    const float depthScale = (nearClip + farClip) / depthRange;
    float depthOffset = nearClip + nearClip;
    depthOffset *= farClip;
    depthOffset /= depthRange;
    const Matrix4x4 projection = {SHEN_FIELD(rows){{SHEN_FIELD(x) cotangent / aspectScale, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) cotangent, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 0.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) depthScale, SHEN_FIELD(w) - 1.0f},
        {SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) depthOffset, SHEN_FIELD(w) 0.0f}}};
    g_mt7NearClip = nearClip;
    g_mt7FarClip = farClip;
    g_mt7InverseNearClip = 1.0f / nearClip;
    g_mt7LightingParameters.nearClip = nearClip;
    g_mt7LightingParameters.negativeNearClip = -nearClip;
    g_mt7LightingParameters.farClip = farClip;
    g_mt7LightingParameters.inverseNearClip = g_mt7InverseNearClip;
    matrix_stack::LoadMatrix(&projection);
    RefreshMT7ScreenTransform();
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
