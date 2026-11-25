#include "platform/dc/particle_renderer.hpp"

#if SHENMUE_IS_DC && (SHENMUE_IS_DEMO || SHENMUE_IS_S1 || (SHENMUE_IS_DC && SHENMUE_IS_S2))

#include "common/types.hpp"
#include "platform/dc/matrix.hpp"
#include "platform/dc/particle_simulation.hpp"
#include <cstdint>

namespace shenmue { namespace dc { namespace sdk {
namespace {

#if SHENMUE_S2_GCC295
extern "C" void syCachePREF(void *address, std::uint32_t size);
#endif

void FlushParticlePacket(std::uint32_t cursor) {
#if SHENMUE_S2_GCC295
    syCachePREF(reinterpret_cast<void *>(cursor), 32);
#else
    __builtin_prefetch(reinterpret_cast<const void *>(cursor), 1, 3);
#endif
}

void StoreParticlePacket(std::uint32_t cursor, std::uint32_t word, std::uint32_t value) {
    reinterpret_cast<volatile std::uint32_t *>(cursor)[word] = value;
}

void StoreParticleFloat(std::uint32_t cursor, std::uint32_t word, float value) {
    StoreParticlePacket(cursor, word, floatAsU32(value));
}

void StoreParticlePosition(std::uint32_t cursor, const Vector4 &position, float inverseDepth, std::uint32_t control) {
    StoreParticleFloat(cursor, 3, inverseDepth);
    StoreParticleFloat(cursor, 2, position.y * inverseDepth);
    StoreParticleFloat(cursor, 1, position.x * inverseDepth);
    StoreParticlePacket(cursor, 0, control);
}

Vector4 TransformParticleCorner(float x, float y) {
    const Vector3 point = {x, y, 0.0f};
    Vector4 transformed;
    matrix_stack::TransformPosition(&point, &transformed);
    return transformed;
}

void StoreParticleTexcoords(std::uint32_t cursor, std::uint32_t corner, bool reverse) {
    float u = corner >= 2 ? 1.0f : 0.0f;
    float v = corner & 1U ? 1.0f : 0.0f;
    if (reverse) {
        u = 1.0f - u;
        v = 1.0f - v;
    }
    StoreParticleFloat(cursor, 5, v);
    StoreParticleFloat(cursor, 4, u);
}

std::uint32_t DrawParticleQuad(std::uint32_t cursor, float width, float height, std::uint32_t colour, std::uint32_t mode, bool callbacks) {
    const bool textured = (mode & 2U) != 0;
    const bool delayedFlush = (mode & 1U) != 0;
    const float threshold = delayedFlush ? g_particleClipDepthThreshold : 0.0f;
    const std::uint32_t start = cursor;
    float x = -width;
    float y = height;
    Vector4 position = TransformParticleCorner(x, y);
    StoreParticlePacket(cursor, 6, colour);
    if (mode == 3)
        StoreParticleTexcoords(cursor, 0, true);
    if (!(position.w > threshold))
        return start;
    float inverseDepth = 1.0f / position.w;
    if (mode == 2)
        StoreParticleTexcoords(cursor, 0, false);
    y -= height;
    y -= height;
    StoreParticlePosition(cursor, position, inverseDepth, cursor + 4);
    if (!delayedFlush)
        FlushParticlePacket(cursor);

    position = TransformParticleCorner(x, y);
    cursor += 32;
    StoreParticlePacket(cursor, 6, colour);
    if (mode == 3)
        StoreParticleTexcoords(cursor, 1, true);
    if (!(position.w > threshold)) {
        if (delayedFlush) {
            StoreParticlePacket(cursor - 32, 0, UINT32_MAX);
            FlushParticlePacket(cursor - 32);
        }
        return start;
    }
    if (delayedFlush)
        FlushParticlePacket(cursor - 32);
    inverseDepth = 1.0f / position.w;
    if (mode == 2)
        StoreParticleTexcoords(cursor, 1, false);
    x += width;
    x += width;
    y += height;
    y += height;
    StoreParticlePosition(cursor, position, inverseDepth, cursor + 4);
    if (!delayedFlush)
        FlushParticlePacket(cursor);

    position = TransformParticleCorner(x, y);
    cursor += 32;
    StoreParticlePacket(cursor, 6, colour);
    if (mode == 3)
        StoreParticleTexcoords(cursor, 2, true);
    if (!(position.w > threshold)) {
        if (delayedFlush) {
            StoreParticlePacket(cursor - 32, 0, UINT32_MAX);
            FlushParticlePacket(cursor - 32);
            FlushParticlePacket(cursor - 32);
        }
        return start;
    }
    if (delayedFlush)
        FlushParticlePacket(cursor - 32);
    inverseDepth = 1.0f / position.w;
    if (mode == 2)
        StoreParticleTexcoords(cursor, 2, false);
    y -= height;
    y -= height;
    const Vector4 last = TransformParticleCorner(x, y);
    if (!(last.w > threshold)) {
        if (delayedFlush) {
            StoreParticlePacket(cursor, 0, UINT32_MAX);
            FlushParticlePacket(cursor);
        }
        return mode == 2 && !callbacks ? start - 8 : start;
    }
    const float lastInverse = 1.0f / last.w;
    StoreParticlePosition(cursor, position, inverseDepth, cursor + 4);
    FlushParticlePacket(cursor);
    cursor += 32;
    StoreParticlePacket(cursor, 6, colour);
    if (textured)
        StoreParticleTexcoords(cursor, 3, mode == 3);
    StoreParticlePosition(cursor, last, lastInverse, UINT32_MAX);
    FlushParticlePacket(cursor);
    return cursor + 32;
}

std::uint32_t DrawParticleBillboard(std::uint32_t cursor, float width, float height, std::uint32_t colour) {
    float x = -width;
    float y = height;
    Vector4 first = TransformParticleCorner(x, y);
    y -= height;
    y -= height;
    Vector4 second = TransformParticleCorner(x, y);
    if (!(first.w > 0.0f) || !(second.w > 0.0f))
        return cursor;
    const std::uint32_t start = cursor;
    const float firstInverse = 1.0f / first.w;
    const float secondInverse = 1.0f / second.w;
    x += width;
    x += width;
    y += height;
    y += height;
    StoreParticlePacket(cursor, 6, colour);
    StoreParticleTexcoords(cursor, 0, false);
    StoreParticlePosition(cursor, first, firstInverse, cursor + 4);
    FlushParticlePacket(cursor);
    cursor += 32;
    Vector4 third = TransformParticleCorner(x, y);
    StoreParticlePacket(cursor, 6, colour);
    if (!(third.w > 0.0f))
        return start;
    const float thirdInverse = 1.0f / third.w;
    y -= height;
    y -= height;
    StoreParticleTexcoords(cursor, 1, false);
    StoreParticlePosition(cursor, second, secondInverse, cursor + 4);
    FlushParticlePacket(cursor);
    cursor += 32;
    Vector4 fourth = TransformParticleCorner(x, y);
    StoreParticlePacket(cursor, 6, colour);
    if (!(fourth.w > 0.0f))
        return start;
    const float fourthInverse = 1.0f / fourth.w;
    StoreParticleTexcoords(cursor, 2, false);
    StoreParticlePosition(cursor, third, thirdInverse, cursor + 4);
    FlushParticlePacket(cursor);
    cursor += 32;
    StoreParticlePacket(cursor, 6, colour);
    StoreParticleTexcoords(cursor, 3, false);
    StoreParticlePosition(cursor, fourth, fourthInverse, UINT32_MAX);
    FlushParticlePacket(cursor);
    return cursor + 32;
}

void SaveParticleMatrix(Matrix4x4 &saved) {
    Matrix4x4 matrix;
    njGetMatrix(&matrix);
    for (std::uint32_t pair = 0; pair < 8; ++pair)
        for (std::uint32_t word = 0; word < 2; ++word)
            saved.rows[pair / 2].data()[(pair % 2) * 2 + word] = matrix.rows[(7 - pair) / 2].data()[((7 - pair) % 2) * 2 + word];
}

void RestoreParticleMatrix(const Matrix4x4 &saved) {
    Matrix4x4 matrix;
    for (std::uint32_t pair = 0; pair < 8; ++pair)
        for (std::uint32_t word = 0; word < 2; ++word)
            matrix.rows[pair / 2].data()[(pair % 2) * 2 + word] = saved.rows[(7 - pair) / 2].data()[((7 - pair) % 2) * 2 + word];
    njSetMatrix(0, &matrix);
}

void SetParticleTranslation(const Vector4 &center, bool unitAxes) {
    Matrix4x4 matrix;
    njGetMatrix(&matrix);
    if (unitAxes) {
        matrix.rows[0].x = 1.0f;
        matrix.rows[0].y = 0.0f;
        matrix.rows[0].z = 0.0f;
        matrix.rows[1].x = 0.0f;
        matrix.rows[1].y = 1.0f;
        matrix.rows[1].z = 0.0f;
        matrix.rows[2].x = 0.0f;
        matrix.rows[2].y = 0.0f;
        matrix.rows[2].z = 1.0f;
    }
    matrix.rows[3] = center;
    njSetMatrix(0, &matrix);
}

void RotateParticle(std::int32_t angle, std::uint32_t axis) {
    Vector2 rotation;
    CalculateFixedSinCos(static_cast<std::uint32_t>(angle), &rotation);
    switch (axis) {
    case 0:
        matrix_stack::RotateXSinCos(rotation.x, rotation.y);
        break;
    case 1:
        matrix_stack::RotateYSinCos(rotation.x, rotation.y);
        break;
    case 2:
        matrix_stack::RotateZSinCos(rotation.x, rotation.y);
        break;
    }
}

std::uint32_t DrawParticleQuads(ParticleChannel &channel, std::uint32_t cursor, std::uint32_t mode, bool callbacks) {
    Matrix4x4 *saved;
    if (callbacks) {
        if (mode == 0)
            saved = g_particleFullSecondMatrix;
        else if (mode == 1)
            saved = &g_particleXyMatrix;
        else if (mode == 2)
            saved = g_particleFullMatrix;
        else
            saved = &g_particleXyzMatrix;
    } else {
        if (mode == 0)
            saved = g_particleCompactSecondMatrix;
        else if (mode == 1)
            saved = &g_particleCompactXyMatrix;
        else if (mode == 2)
            saved = g_particleCompactMatrix;
        else
            saved = &g_particleCompactXyzMatrix;
    }
    SaveParticleMatrix(*saved);
    const float width = channel.scaleX;
    const float height = channel.scaleY;
    if (!(mode & 1U))
        matrix_stack::GetPreviousMatrix(saved + 1);
    Particle *particle = static_cast<Particle *>(channel.particles);
    std::uint32_t count = static_cast<std::uint32_t>(channel.count);
    do {
        if (!(mode & 1U))
            matrix_stack::GetPreviousMatrix(0);
        if (callbacks)
            particle->update(particle, &channel);
        if (particle->flags & 0x80000000U) {
            Vector4 center;
            matrix_stack::TransformPosition(&particle->position, &center);
            SetParticleTranslation(center, !(mode & 1U));
            if (mode & 1U) {
                RotateParticle(particle->phase0, 0);
                RotateParticle(particle->phase1, 1);
                RotateParticle(particle->phase2, 2);
            } else {
                RotateParticle(particle->phase1, 2);
                if (mode == 0 || !callbacks)
                    RotateParticle(particle->phase2, 1);
                matrix_stack::Matrix4x4_Push(&g_particleRenderMatrix);
                matrix_stack::MultiplyPreviousMatrix();
                DiscardParticleMatrix(1);
            }
            const float particleWidth = width * particle->scale;
            const float particleHeight = height * particle->scale;
            cursor = mode == 2 && callbacks ? DrawParticleBillboard(cursor, particleWidth, particleHeight, particle->colour) :
                                              DrawParticleQuad(cursor, particleWidth, particleHeight, particle->colour, mode, callbacks);
        }
        RestoreParticleMatrix(*saved);
        ++particle;
    } while (--count);
    return cursor;
}

std::uint32_t DrawParticleSprites(ParticleChannel &channel, std::uint32_t cursor, bool textured, bool callbacks, float width, float height) {
    CompactParticle *particle = static_cast<CompactParticle *>(channel.particles);
    std::uint32_t count = static_cast<std::uint32_t>(channel.count);
    do {
        if (callbacks)
            particle->update(reinterpret_cast<Particle *>(particle), &channel);
        if (particle->flags & 0x80000000U) {
            Vector4 position;
            matrix_stack::TransformPosition(&particle->position, &position);
            if (position.w > 0.0f) {
                const float inverseDepth = 1.0f / position.w;
                float x = position.x * inverseDepth;
                float y = position.y * inverseDepth;
                const float spriteWidth = width * inverseDepth;
                const float spriteHeight = height * inverseDepth;
                if (textured && callbacks) {
                    const float right = x + spriteWidth;
                    const float top = y - spriteHeight;
                    StoreParticleFloat(cursor, 6, inverseDepth);
                    StoreParticleFloat(cursor, 7, right);
                    StoreParticleFloat(cursor, 4, x);
                    StoreParticleFloat(cursor, 5, top);
                    StoreParticleFloat(cursor, 2, y);
                    StoreParticleFloat(cursor, 3, inverseDepth);
                    StoreParticleFloat(cursor, 1, x);
                    StoreParticlePacket(cursor, 0, UINT32_MAX);
                    FlushParticlePacket(cursor);
                    cursor += 32;
                    StoreParticlePacket(cursor, 6, 0);
                    StoreParticleFloat(cursor, 7, 1.0f);
                    StoreParticleFloat(cursor, 4, x);
                    StoreParticlePacket(cursor, 5, 0x00003f80U);
                    StoreParticleFloat(cursor, 2, right);
                    StoreParticleFloat(cursor, 3, top + spriteHeight);
                    StoreParticleFloat(cursor, 0, top);
                    StoreParticleFloat(cursor, 1, inverseDepth);
                    FlushParticlePacket(cursor);
                    cursor += 32;
                } else {
                    StoreParticlePacket(cursor, 0, UINT32_MAX);
                    StoreParticleFloat(cursor, 1, x);
                    StoreParticleFloat(cursor, 2, y);
                    StoreParticleFloat(cursor, 3, inverseDepth);
                    StoreParticleFloat(cursor, 4, x);
                    y -= spriteHeight;
                    StoreParticleFloat(cursor, 5, y);
                    StoreParticleFloat(cursor, 6, inverseDepth);
                    x += spriteWidth;
                    StoreParticleFloat(cursor, 7, x);
                    FlushParticlePacket(cursor);
                    cursor += 32;
                    StoreParticleFloat(cursor, 0, y);
                    StoreParticleFloat(cursor, 1, inverseDepth);
                    StoreParticleFloat(cursor, 2, x);
                    y += spriteHeight;
                    StoreParticleFloat(cursor, 3, y);
                    if (textured) {
                        StoreParticlePacket(cursor, 5, 0x00003f80U);
                        StoreParticlePacket(cursor, 6, 0);
                        StoreParticleFloat(cursor, 7, 1.0f);
                    }
                    FlushParticlePacket(cursor);
                    cursor += 32;
                }
            }
        }
        ++particle;
    } while (--count);
    return cursor;
}

void RunParticleCallbacks(ParticleChannel &channel, bool compact, bool callbacks) {
    SaveParticleMatrix(g_particleCallbackMatrix);
    std::byte *particle = static_cast<std::byte *>(channel.particles);
    std::uint32_t count = static_cast<std::uint32_t>(channel.count);
    do {
        if (callbacks)
            reinterpret_cast<Particle *>(particle)->update(reinterpret_cast<Particle *>(particle), &channel);
        particle += compact ? sizeof(CompactParticle) : sizeof(Particle);
        RestoreParticleMatrix(g_particleCallbackMatrix);
    } while (--count);
}

} // namespace

void DrawParticleChannels() {
    PrepareParticleTransform();
    std::uint32_t *const vertexBuffers = g_sdkVertexBufferPointers;
    std::uint32_t cursors[2] = {(vertexBuffers[0] & 0x03ffffffU) | 0xe0000000U, (vertexBuffers[2] & 0x03ffffffU) | 0xe0000000U};
    for (std::size_t channelIndex = 0; channelIndex < 16; ++channelIndex) {
        ParticleChannel &channel = g_particleChannels[channelIndex];
        const std::uint32_t flags = channel.flags;
        if (!(flags & 0x80000000U))
            continue;
        const float width = g_particleProjectionScales.x * channel.scaleX;
        const float height = g_particleProjectionScales.w * channel.scaleY;
        const std::uint32_t list = flags >> 30U & 1U;
        std::uint32_t cursor = cursors[list];
        const std::uint32_t queue = list ? 12 : 16;
        *reinterpret_cast<volatile std::uint32_t *>(0xff000038U) = queue;
        *reinterpret_cast<volatile std::uint32_t *>(0xff00003cU) = queue;
        StoreParticlePacket(cursor, 4, channel.colour);
        StoreParticlePacket(cursor, 3, channel.context.texture);
        StoreParticlePacket(cursor, 2, channel.context.tsp);
        StoreParticlePacket(cursor, 1, channel.context.isp);
        StoreParticlePacket(cursor, 0, channel.context.flags);
        FlushParticlePacket(cursor);
        cursor += 32;
        const bool callbacks = !(flags & 0x02000000U);
        if (flags & 0x04000000U)
            RunParticleCallbacks(channel, flags & 0x20000000U, callbacks);
        else {
            const std::uint32_t mode = flags >> 27U & 7U;
            cursor = mode < 4 ? DrawParticleQuads(channel, cursor, mode, callbacks) : DrawParticleSprites(channel, cursor, mode >= 6, callbacks, width, height);
        }
        cursors[list] = cursor;
    }
    Matrix4x4::Pop(1);
    vertexBuffers[2] = (cursors[1] & 0x03ffffffU) | (vertexBuffers[2] & 0xfc000001U);
    *reinterpret_cast<volatile std::uint32_t *>(0xff000038U) = 12;
    *reinterpret_cast<volatile std::uint32_t *>(0xff00003cU) = 12;
}

} } } // namespace shenmue::dc::sdk

#endif
