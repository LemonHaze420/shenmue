#include "shenmue.h"
#if (SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "s2/weather_particles_dc.hpp"
#endif

#if SHENMUE_IS_DC && (SHENMUE_IS_DC && SHENMUE_IS_S2)
#include "platform/dc/matrix.hpp"
#include "platform/dc/sdk.hpp"
#include "platform/dc/ninja.hpp"
namespace shenmue { namespace s2 {
typedef WeatherParticleGroup WeatherNodeData;
typedef WeatherRainParticle WeatherSnowParticle;
struct WeatherParticlePool {
    void *records;
    std::int32_t capacity, activeCount, renderCount;
    WeatherFloat *fade;
    std::int32_t availableCount;
};
#define g_weatherFrameStep weatherParticleUnusedParameter
namespace {
#if SHENMUE_S2_GCC295
extern "C" void syCachePREF(void *address, std::uint32_t size);
#endif
struct WeatherPacketCursor {
    std::uint32_t opaque;
    std::uint32_t transparent;
};
void SetWeatherStoreQueue(std::uint32_t address) {
    *reinterpret_cast<volatile std::uint32_t *>(0xff000038U) = address;
    *reinterpret_cast<volatile std::uint32_t *>(0xff00003cU) = address;
}
void FlushWeatherStoreQueue(std::uint32_t address) {
#if SHENMUE_S2_GCC295
    syCachePREF(reinterpret_cast<void *>(address), 32);
#else
    __builtin_prefetch(reinterpret_cast<const void *>(address), 1, 3);
#endif
}
void WriteWeatherWord(std::uint32_t address, std::uint32_t value) {
    *reinterpret_cast<volatile std::uint32_t *>(address) = value;
}
void WriteWeatherFloat(std::uint32_t address, float value) {
    WriteWeatherWord(address, floatAsU32(value));
}
void WriteWeatherMaterial(std::uint32_t &cursor, const dc::sdk::ModelPolygonContext &material, std::uint32_t colour) {
    WriteWeatherWord(cursor, material.flags);
    WriteWeatherWord(cursor + 4, material.isp);
    WriteWeatherWord(cursor + 8, material.tsp);
    WriteWeatherWord(cursor + 12, material.texture);
    WriteWeatherWord(cursor + 16, colour);
    FlushWeatherStoreQueue(cursor);
    cursor += 32;
}
void RestoreWeatherTransform(const Matrix4x4 &matrix) {
    matrix_stack::LoadMatrix(&matrix);
}

// @verified - d
void DrawWeatherSnow(WeatherNodeData *node, WeatherParticlePool &pool, std::uint32_t flags, float size, std::uint32_t &cursor) {
    WeatherSnowParticle *particle = static_cast<WeatherSnowParticle *>(pool.records);
    const bool suppressCallbacks = (flags & 0x10000000U) != 0;
    for (std::int32_t index = 0; index < pool.renderCount; ++index, ++particle) {
        if (!suppressCallbacks)
            particle->update(particle, node);
        if (!(particle->flags & 0x80000000U))
            continue;
        Vector4 projected;
        matrix_stack::TransformPosition(&particle->position, &projected);
        if (!(projected.w > g_weatherFrameStep))
            continue;
        const float inverseDepth = 1.0f / projected.w;
        WriteWeatherWord(cursor, 0xffffffffU);
        float x = projected.x * inverseDepth;
        float y = projected.y * inverseDepth;
        if (x > 640.0f || y > 480.0f || !(x > 0.0f) || !(y > 0.0f))
            continue;
        const float extent = size * inverseDepth;
        WriteWeatherFloat(cursor + 4, x);
        WriteWeatherFloat(cursor + 8, y);
        WriteWeatherFloat(cursor + 12, inverseDepth);
        WriteWeatherFloat(cursor + 16, x);
        y -= extent;
        WriteWeatherFloat(cursor + 20, y);
        WriteWeatherFloat(cursor + 24, inverseDepth);
        x += extent;
        WriteWeatherFloat(cursor + 28, x);
        FlushWeatherStoreQueue(cursor);
        cursor += 32;
        WriteWeatherFloat(cursor, y);
        WriteWeatherFloat(cursor + 4, inverseDepth);
        WriteWeatherFloat(cursor + 8, x);
        y += extent;
        WriteWeatherFloat(cursor + 12, y);
        WriteWeatherWord(cursor + 20, 0x00003f80U);
        WriteWeatherWord(cursor + 24, 0);
        WriteWeatherWord(cursor + 28, 0x3f800000U);
        FlushWeatherStoreQueue(cursor);
        cursor += 32;
    }
}

void SetWeatherParticleTransform(const Vector3 &position, const Vector3i &angles) {
    Vector4 centre;
    matrix_stack::TransformPosition(&position, &centre);
    Matrix4x4 matrix;
    matrix_stack::GetMatrix(&matrix);
    matrix.rows[3] = centre;
    matrix_stack::LoadMatrix(&matrix);
    dc::sdk::njRotateX(0, angles.x);
    dc::sdk::njRotateY(0, angles.y);
    dc::sdk::njRotateZ(0, angles.z);
}

// @verified - d
void DrawWeatherQuad(std::uint32_t &cursor, float width, float height, std::uint32_t colour, bool textured) {
    Vector3 corner = {-width, height, 0.0f};
    Vector4 projected;
    matrix_stack::TransformPosition(&corner, &projected);
    cursor += 24;
    WriteWeatherWord(cursor, colour);
    if (textured) {
        WriteWeatherFloat(cursor - 4, 1.0f);
        WriteWeatherFloat(cursor - 8, 1.0f);
    }
    cursor -= 8;
    if (!(projected.w > g_weatherFrameStep)) {
        cursor -= 16;
        return;
    }
    float inverseDepth = 1.0f / projected.w;
    corner.y -= height;
    corner.y -= height;
    cursor -= 4;
    WriteWeatherFloat(cursor, inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.y * inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.x * inverseDepth);
    cursor -= 4;
    WriteWeatherWord(cursor, cursor);
    std::uint32_t previous = cursor;

    matrix_stack::TransformPosition(&corner, &projected);
    cursor += 56;
    WriteWeatherWord(cursor, colour);
    if (textured) {
        WriteWeatherFloat(cursor - 4, 0.0f);
        WriteWeatherFloat(cursor - 8, 1.0f);
    }
    cursor -= 8;
    if (!(projected.w > g_weatherFrameStep)) {
        WriteWeatherWord(previous, 0xffffffffU);
        FlushWeatherStoreQueue(previous);
        cursor -= 48;
        return;
    }
    FlushWeatherStoreQueue(previous);
    inverseDepth = 1.0f / projected.w;
    corner.x += width;
    corner.x += width;
    corner.y += height;
    corner.y += height;
    cursor -= 4;
    WriteWeatherFloat(cursor, inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.y * inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.x * inverseDepth);
    cursor -= 4;
    WriteWeatherWord(cursor, cursor);
    previous = cursor;

    matrix_stack::TransformPosition(&corner, &projected);
    cursor += 56;
    WriteWeatherWord(cursor, colour);
    if (textured) {
        WriteWeatherFloat(cursor - 4, 1.0f);
        WriteWeatherFloat(cursor - 8, 0.0f);
    }
    cursor -= 8;
    if (!(projected.w > g_weatherFrameStep)) {
        WriteWeatherWord(previous, 0xffffffffU);
        FlushWeatherStoreQueue(previous);
        cursor -= 80;
        return;
    }
    FlushWeatherStoreQueue(previous);
    inverseDepth = 1.0f / projected.w;
    corner.y -= height;
    corner.y -= height;
    Vector4 final;
    matrix_stack::TransformPosition(&corner, &final);
    if (!(final.w > g_weatherFrameStep)) {
        const std::uint32_t failed = cursor - 16;
        WriteWeatherWord(failed, 0xffffffffU);
        FlushWeatherStoreQueue(failed);
        cursor -= 80;
        return;
    }
    const float finalInverseDepth = 1.0f / final.w;
    cursor -= 4;
    WriteWeatherFloat(cursor, inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.y * inverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, projected.x * inverseDepth);
    cursor -= 4;
    WriteWeatherWord(cursor, cursor);
    FlushWeatherStoreQueue(cursor);
    cursor += 56;
    WriteWeatherWord(cursor, colour);
    if (textured) {
        WriteWeatherFloat(cursor - 4, 0.0f);
        WriteWeatherFloat(cursor - 8, 0.0f);
    }
    cursor -= 12;
    WriteWeatherFloat(cursor, finalInverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, final.y * finalInverseDepth);
    cursor -= 4;
    WriteWeatherFloat(cursor, final.x * finalInverseDepth);
    cursor -= 4;
    WriteWeatherWord(cursor, 0xffffffffU);
    FlushWeatherStoreQueue(cursor);
    cursor += 32;
}

// @verified - d
void DrawWeatherRain(WeatherNodeData *node, WeatherParticlePool &pool, std::uint32_t flags, float width, float height, std::uint32_t colour,
    const Vector3i &rotation, std::uint32_t &cursor) {
    Matrix4x4 projection;
    matrix_stack::GetMatrix(&projection);
    WeatherSnowParticle *particle = static_cast<WeatherSnowParticle *>(pool.records);
    for (std::int32_t index = 0; index < pool.renderCount; ++index, ++particle) {
        if (!(flags & 0x10000000U))
            particle->update(particle, node);
        if (particle->flags & 0x80000000U) {
            const Vector3i angles = {node->rainYaw, node->rainPitch, node->rainRoll};
            SetWeatherParticleTransform(particle->position, angles);
            DrawWeatherQuad(cursor, width, height, node->rainColour, false);
        }
        RestoreWeatherTransform(projection);
    }
}

// @verified - d
void DrawWeatherRipples(WeatherNodeData *node, WeatherParticlePool &pool, std::uint32_t flags, float size, std::uint32_t &cursor) {
    Matrix4x4 projection;
    matrix_stack::GetMatrix(&projection);
    WeatherRippleParticle *particle = static_cast<WeatherRippleParticle *>(pool.records);
    for (std::int32_t index = 0; index < pool.renderCount; ++index, ++particle) {
        if (!(flags & 0x10000000U))
            particle->update(particle, node);
        if (particle->flags & 0x80000000U) {
            const Vector3i angles = {particle->angleX, particle->angleY, particle->angleZ};
            SetWeatherParticleTransform(particle->position, angles);
            const float extent = size * particle->scale;
            DrawWeatherQuad(cursor, extent, extent, *particle->colour, true);
        }
        RestoreWeatherTransform(projection);
    }
}

// @verified - d
void ExecuteWeatherCallbacks(WeatherNodeData *node, WeatherParticlePool &pool, std::uint32_t flags, std::size_t stride) {
    if (flags & 0x10000000U)
        return;
    Matrix4x4 projection;
    matrix_stack::GetMatrix(&projection);
    std::byte *particle = static_cast<std::byte *>(pool.records);
    for (std::int32_t index = 0; index < pool.renderCount; ++index, particle += stride) {
        reinterpret_cast<WeatherSnowParticle *>(particle)->update(reinterpret_cast<WeatherSnowParticle *>(particle), node);
        RestoreWeatherTransform(projection);
    }
}

void DrawWeatherGroup(WeatherNodeData *node, WeatherParticlePool &pool, std::uint32_t flags, const dc::sdk::ModelPolygonContext &material,
    std::uint32_t colour, float width, float height, const Vector3i &rotation, float projectionScale, WeatherPacketCursor &cursors, std::size_t stride) {
    if (!(flags & 0x80000000U) || !pool.renderCount || !pool.records)
        return;
    std::uint32_t &cursor = flags & 0x40000000U ? cursors.transparent : cursors.opaque;
    SetWeatherStoreQueue(flags & 0x40000000U ? 12 : 16);
    WriteWeatherMaterial(cursor, material, colour);
    if (flags & 0x20000000U) {
        ExecuteWeatherCallbacks(node, pool, flags, stride);
        return;
    }
    switch ((flags >> 25) & 7U) {
    case 1:
        DrawWeatherRain(node, pool, flags, width, height, colour, rotation, cursor);
        break;
    case 3:
        DrawWeatherRipples(node, pool, flags, width, cursor);
        break;
    case 6:
    case 7:
        DrawWeatherSnow(node, pool, flags, projectionScale * width, cursor);
        break;
    default:
        break;
    }
}
}

// @verified - d
void WeatherParticle_Callback(void *parameter) {
    const Matrix4x4 screen = {{{320.0f, 0, 0, 0}, {0, -240.0f, 0, 0}, {0, 0, -0.5f, 0}, {320.0f, 240.0f, 0.5f, 1}}};
    matrix_stack::Matrix4x4_Push(&screen);
    Matrix4x4::Multiply(matrix_stack::g_matrixStack.entries[2].stack);
    Matrix4x4 projection;
    matrix_stack::GetMatrix(&projection);
    matrix_stack::MultiplyPreviousMatrix();
    WeatherPacketCursor cursors = {
        (dc::sdk::g_sdkVertexBufferPointers[0] & 0x03ffffffU) | 0xe0000000U,
        (dc::sdk::g_sdkVertexBufferPointers[2] & 0x03ffffffU) | 0xe0000000U};
    WeatherNodeData *node = static_cast<WeatherNodeData *>(parameter);
    const Vector3i zeroRotation = {};
    do {
        WeatherParticlePool rain = {node->rainParticles, node->rainCapacity, node->rainLiveCount,
            node->rainDrawCount, node->rainCountLflt, node->rainCurrentCount};
        const dc::sdk::ModelPolygonContext rainMaterial = {node->rainPacket.command, node->rainPacket.isp, node->rainPacket.tsp, node->rainPacket.tcw};
        const Vector3i rainRotation = {node->rainYaw, node->rainPitch, node->rainRoll};
        DrawWeatherGroup(node, rain, node->rainFlags, rainMaterial, node->rainColour,
            node->rainPressureParam0, node->rainPressureParam1, rainRotation, projection.rows[0].x, cursors, sizeof(WeatherRainParticle));
        WeatherParticlePool snow = {node->snowParticles, node->snowCapacity, node->snowLiveCount,
            node->snowDrawCount, node->snowCountLflt, node->snowCurrentCount};
        const dc::sdk::ModelPolygonContext snowMaterial = {node->snowPacket.command, node->snowPacket.isp, node->snowPacket.tsp, node->snowPacket.tcw};
        DrawWeatherGroup(node, snow, node->snowFlags, snowMaterial, node->snowColour,
            node->snowWidth, 0.0f, zeroRotation, projection.rows[0].x, cursors, sizeof(WeatherRainParticle));
        WeatherParticlePool ripple = {node->rippleParticles, node->rippleCapacity, node->rippleLiveCount,
            node->rippleDrawCount, node->rippleCountLflt, node->rippleCurrentCount};
        const dc::sdk::ModelPolygonContext rippleMaterial = {
            node->ripplePacket.command, node->ripplePacket.isp, node->ripplePacket.tsp, node->ripplePacket.tcw};
        DrawWeatherGroup(node, ripple, node->rippleFlags, rippleMaterial, node->rippleColour,
            node->rippleDeltaTime, 0.0f, zeroRotation, projection.rows[0].x, cursors, sizeof(WeatherRippleParticle));
        node = node->next;
    } while (node);
    Matrix4x4::Pop(1);
    dc::sdk::g_sdkVertexBufferPointers[2] =
        (cursors.transparent & 0x03ffffffU) | (dc::sdk::g_sdkVertexBufferPointers[2] & -std::uint32_t(0x03ffffffU));
    SetWeatherStoreQueue(12);
}
#undef g_weatherFrameStep

} } // namespace shenmue

#endif
