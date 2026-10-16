#include "s2/legacy_draw.hpp"
#include "s2/matrix_stack.hpp"
#include "s2/camera.hpp"
#include "s2/renderer_initialization.hpp"
#include "d3t/dx11_renderer.hpp"
#include <cmath>

namespace shenmue::s2 {
using legacy_draw_detail::temporaryModelScale;
namespace {

Matrix4x4 screenOutputMatrix{};
Matrix4x4 worldScreenProjection{};
float screenWidth{}, screenHeight{}, screenLeft{}, screenTop{};
std::int32_t domeRender{};
} // namespace

void SetupLegacyScreenMatrix(float left, float top, float width, float height) {
    if (d3t::D3TDeviceContext *context = d3t::DX11::g_ppD3DDeviceContext) {
        d3t::D3TRenderState &state = d3t::GetRenderState(*context);
        state.viewport = {static_cast<float>(static_cast<std::int32_t>(left)), static_cast<float>(static_cast<std::int32_t>(top)),
            static_cast<float>(static_cast<std::int32_t>(width)), static_cast<float>(static_cast<std::int32_t>(height)), 0.0f, 1.0f};
        state.viewportDirtyMask |= 1;
    }
    screenWidth = width;
    screenHeight = height;
    screenLeft = left;
    screenTop = top;
    temporaryModelScale = 1.0f;
    screenOutputMatrix = {{{width * 0.5f, 0.0f, 0.0f, 0.0f}, {0.0f, height * -0.5f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f},
        {width * 0.5f + left, height * 0.5f + top, 0.0f, 1.0f}}};
}

void CacheWorldScreenProjection() {
    Matrix4x4_Push(g_matrixStack.entries[2].stack);
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.top.stack - 1);
    worldScreenProjection = workTransformMatrix;
    Matrix4x4::Pop(1);
}

std::uint64_t ProjectCachedWorldPosition(const Vector3 *position, Vector2 *output) {
    Matrix4x4_Push(&worldScreenProjection);
    const Matrix4x4 &matrix = workTransformMatrix;
    const float x = ((matrix.rows[1].x * position->y + matrix.rows[0].x * position->x) + matrix.rows[2].x * position->z) + matrix.rows[3].x;
    const float y = ((matrix.rows[1].y * position->y + matrix.rows[0].y * position->x) + matrix.rows[2].y * position->z) + matrix.rows[3].y;
    float w = ((matrix.rows[1].w * position->y + matrix.rows[0].w * position->x) + matrix.rows[2].w * position->z) + matrix.rows[3].w;
    if (w < 0.0000001f)
        w += 0.0000001f;
    const float inverse = 1.0f / w;
    output->x = (((x * inverse + 1.0f) * screenWidth) * 0.5f) + screenLeft;
    output->y = (((1.0f - y * inverse) * screenHeight) * 0.5f) + screenTop;
    const std::int32_t index = g_matrixStack.top.info.index < 1 ? g_matrixStack.top.info.index : 1;
    const std::int32_t retreat =
        g_matrixStack.top.info.index > g_matrixStack.top.info.capacity ? index + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index : index;
    Matrix4x4::Pop(1);
    return static_cast<std::uint64_t>(static_cast<std::int64_t>(retreat) * sizeof(Matrix4x4));
}

std::uint64_t ProjectWorldToScreen(const Vector3 *position, Vector2 *output) {
    CacheWorldScreenProjection();
    return ProjectCachedWorldPosition(position, output);
}

// @verified - p
void SetShouldDomeRenderTo(std::int32_t enabled) {
    domeRender = enabled;
}

void shenPushScreenOutputMatrixAndMultiply() {
    Matrix4x4_Push(&screenOutputMatrix);
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.entries[2].stack);
    Matrix4x4::MultiplyWithPrePassMatrix(g_matrixStack.top.stack - 1);

    const std::int32_t index = g_matrixStack.top.info.index < 1 ? g_matrixStack.top.info.index : 1;
    const std::int32_t retreat =
        g_matrixStack.top.info.index > g_matrixStack.top.info.capacity ? index + g_matrixStack.top.info.capacity - g_matrixStack.top.info.index : index;
    g_matrixStack.top.info.index = static_cast<std::int16_t>(g_matrixStack.top.info.index - index);
    g_matrixStack.top.stack -= retreat;
}

bool GetModelCheck(const Vector3 *centre, float radius) {
    if (std::abs(temporaryModelScale) < 0.0000001f)
        return false;
    const d3t::D3TDeviceContext *context = d3t::DX11::g_ppD3DDeviceContext;
    if (!context)
        return false;
    const d3t::D3TRenderState &state = d3t::GetRenderState(*context);
    const std::int32_t left = static_cast<std::int32_t>(state.viewport[0]);
    const std::int32_t top = static_cast<std::int32_t>(state.viewport[1]);
    const std::int32_t width = static_cast<std::int32_t>(state.viewport[2]);
    const std::int32_t height = static_cast<std::int32_t>(state.viewport[3]);
    const float scale = temporaryModelScale * radius;
    Vector3 point{};
    Matrix4x4::TransformPoint(*centre, point);
    if (!domeRender && (-point.z - scale > g_projectionFarClip + 0.1f || g_projectionNearClip - 0.050000001f > scale - point.z))
        return false;
    Matrix4x4_PushActiveTransform();
    shenPushScreenOutputMatrixAndMultiply();
    const Matrix4x4 &matrix = workTransformMatrix;
    const float projectedX = (matrix.rows[1].x * point.y + matrix.rows[0].x * point.x) + matrix.rows[2].x * point.z + matrix.rows[3].x;
    const float projectedY = (matrix.rows[1].y * point.y + matrix.rows[0].y * point.x) + matrix.rows[2].y * point.z + matrix.rows[3].y;
    float w = (matrix.rows[1].w * point.y + matrix.rows[0].w * point.x) + matrix.rows[2].w * point.z + matrix.rows[3].w;
    if (w < 0.0000001f)
        w += 0.0000001f;
    Matrix4x4::Pop(1);
    const float reciprocal = 1.0f / w;
    const float x = projectedX * reciprocal;
    const float y = projectedY * reciprocal;
    const float projectedRadius = scale * reciprocal;

    if (reciprocal >= 0.0f && x + projectedRadius <= static_cast<float>(left) - 3.0f && static_cast<float>(width + left) + 3.0f <= x - projectedRadius &&
        y + projectedRadius <= static_cast<float>(top) - 3.0f)
        return static_cast<float>(height + top) + 3.0f > y - projectedRadius;
    return true;
}
} // namespace shenmue::s2
