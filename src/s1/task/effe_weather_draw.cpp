#include "s1/task/effe_internal.hpp"

namespace shenmue::s1::effe_detail {


Vector4 ApplyWeatherViewTransform(const Vector3 &point) {
    const Matrix4x4 &m = workTransformMatrix;
    return {((m.rows[1].x * point.y + m.rows[0].x * point.x) + m.rows[2].x * point.z) + m.rows[3].x,
        ((m.rows[1].y * point.y + m.rows[0].y * point.x) + m.rows[2].y * point.z) + m.rows[3].y,
        ((m.rows[1].z * point.y + m.rows[0].z * point.x) + m.rows[2].z * point.z) + m.rows[3].z,
        ((m.rows[1].w * point.y + m.rows[0].w * point.x) + m.rows[2].w * point.z) + m.rows[3].w};
}

// Matrix4x4_ApplyTransform: a homogeneous row vector through the work matrix.
Vector4 ApplyWeatherTransform(const Vector4 &value) {
    const Matrix4x4 &m = workTransformMatrix;
    return {((m.rows[1].x * value.y + m.rows[0].x * value.x) + m.rows[2].x * value.z) + m.rows[3].x * value.w,
        ((m.rows[1].y * value.y + m.rows[0].y * value.x) + m.rows[2].y * value.z) + m.rows[3].y * value.w,
        ((m.rows[1].z * value.y + m.rows[0].z * value.x) + m.rows[2].z * value.z) + m.rows[3].z * value.w,
        ((m.rows[1].w * value.y + m.rows[0].w * value.x) + m.rows[2].w * value.z) + m.rows[3].w * value.w};
}

float WeatherAngleCosine(std::int32_t angle) {
    const std::int32_t low = static_cast<std::int32_t>(static_cast<std::uint16_t>(angle));
    const std::int32_t folded = low <= 0x8000 ? low : -low;
    return sin_fixed16(static_cast<std::int16_t>(0x4000 - folded));
}

// The constructors premultiply the work matrix with inline X, Y and Z rotation rows.
void RotateWeatherQuad(std::int32_t angleX, std::int32_t angleY, std::int32_t angleZ) {
    Matrix4x4 rotated = workTransformMatrix;
    float cosine = WeatherAngleCosine(angleX);
    float sine = sin_fixed16(static_cast<std::int16_t>(angleX));
    rotated.rows[1] = ApplyWeatherTransform({0.0f, cosine, sine, 0.0f});
    rotated.rows[2] = ApplyWeatherTransform({0.0f, -sine, cosine, 0.0f});
    workTransformMatrix = rotated;
    cosine = WeatherAngleCosine(angleY);
    sine = sin_fixed16(static_cast<std::int16_t>(angleY));
    rotated.rows[0] = ApplyWeatherTransform({cosine, 0.0f, -sine, 0.0f});
    rotated.rows[2] = ApplyWeatherTransform({sine, 0.0f, cosine, 0.0f});
    workTransformMatrix = rotated;
    cosine = WeatherAngleCosine(angleZ);
    sine = sin_fixed16(static_cast<std::int16_t>(angleZ));
    rotated.rows[0] = ApplyWeatherTransform({cosine, sine, 0.0f, 0.0f});
    rotated.rows[1] = ApplyWeatherTransform({-sine, cosine, 0.0f, 0.0f});
    workTransformMatrix = rotated;
}

// Six PTD_T vertices per quad: corners 0,1,2 then 0,2,3; all four must project in front (W > 0).
bool WriteWeatherQuad(EffeParticleVertex *output, float halfWidth, float halfHeight, std::uint32_t colour) {
    const Vector4 corner0 = ApplyWeatherTransform({-halfWidth, halfHeight, 0.0f, 1.0f});
    const Vector4 corner3 = ApplyWeatherTransform({-halfWidth, -halfHeight, 0.0f, 1.0f});
    const Vector4 corner1 = ApplyWeatherTransform({halfWidth, halfHeight, 0.0f, 1.0f});
    const Vector4 corner2 = ApplyWeatherTransform({halfWidth, -halfHeight, 0.0f, 1.0f});
    if (!(corner0.w > 0.0f) || !(corner1.w > 0.0f) || !(corner2.w > 0.0f) || !(corner3.w > 0.0f))
        return false;
    const auto vertex = [colour](const Vector4 &corner, float u, float v) {
        return EffeParticleVertex{
            .position = {corner.x / corner.w, corner.y / corner.w, corner.z / corner.w, 1.0f / corner.w}, .diffuse = colour, .u = u, .v = v};
    };
    output[0] = vertex(corner0, 0.0f, 0.0f);
    output[1] = vertex(corner1, 1.0f, 0.0f);
    output[2] = vertex(corner2, 1.0f, 1.0f);
    output[3] = vertex(corner0, 0.0f, 0.0f);
    output[4] = vertex(corner2, 1.0f, 1.0f);
    output[5] = vertex(corner3, 0.0f, 1.0f);
    return true;
}

// ConstructAndCommitVertsForDrawing_0: rain streaks rotated by the bucket's +0x6c/+0x70/+0x74 angles.
std::uint32_t ConstructAndCommitVertsForDrawing_0(D3TDeviceContext *context, const WeatherDrawGroup &group) {
    EffeParticleVertex *output = MapEffeParticleVertices(context, static_cast<std::uint32_t>(group.count));
    if (!output)
        return 0;
    const std::uint32_t colour = group.owner->rain.colour;
    const std::int32_t angleX = group.owner->rain.angleX;
    const std::int32_t angleY = group.owner->rain.angleY;
    const std::int32_t angleZ = group.owner->rain.angleZ;
    const Matrix4x4 saved = workTransformMatrix;
    std::uint32_t polys = 0;
    for (std::int32_t index = 0; index < group.count; ++index) {
        auto *const particle = reinterpret_cast<RainParticleNative *>(group.records + static_cast<std::size_t>(index) * sizeof(RainParticleNative));
        particle->random(particle, group.owner);
        if ((particle->flags & 0x80000000U) != 0) {
            workTransformMatrix.rows[3] = ApplyWeatherViewTransform({particle->position.x, particle->position.y, particle->position.z});
            RotateWeatherQuad(angleX, angleY, angleZ);
            if (WriteWeatherQuad(output, group.width, group.height, colour)) {
                output += 6;
                ++polys;
            }
        }
        workTransformMatrix = saved;
    }
    return polys;
}

// ConstructAndCommitVertsForDrawing_1: screen-aligned snow squares past the 0.3 near cut.
std::uint32_t ConstructAndCommitVertsForDrawing_1(D3TDeviceContext *context, const WeatherDrawGroup &group) {
    EffeParticleVertex *output = MapEffeParticleVertices(context, static_cast<std::uint32_t>(group.count));
    if (!output)
        return 0;
    const std::uint32_t colour = group.owner->snow.colour;
    std::uint32_t polys = 0;
    for (std::int32_t index = 0; index < group.count; ++index) {
        auto *const particle = reinterpret_cast<RainParticleNative *>(group.records + static_cast<std::size_t>(index) * sizeof(RainParticleNative));
        particle->random(particle, group.owner);
        if ((particle->flags & 0x80000000U) == 0)
            continue;
        const Vector4 view = ApplyWeatherViewTransform({particle->position.x, particle->position.y, particle->position.z});
        if (!(kWeatherSnowNearCut < view.w))
            continue;
        const float rhw = 1.0f / view.w;
        const float z = view.z * rhw;
        const float x = view.x * rhw;
        const float y = view.y * rhw;
        const float size = group.scaledWidth * rhw;
        output[0] = {.position = {x, y, z, rhw}, .diffuse = colour, .u = 0.0f, .v = 0.0f};
        output[1] = {.position = {x + size, y - size, z, rhw}, .diffuse = colour, .u = 1.0f, .v = 1.0f};
        output[2] = {.position = {x + size, y, z, rhw}, .diffuse = colour, .u = 1.0f, .v = 0.0f};
        output[3] = {.position = {x, y, z, rhw}, .diffuse = colour, .u = 0.0f, .v = 0.0f};
        output[4] = {.position = {x, y - size, z, rhw}, .diffuse = colour, .u = 0.0f, .v = 1.0f};
        output[5] = {.position = {x + size, y - size, z, rhw}, .diffuse = colour, .u = 1.0f, .v = 1.0f};
        output += 6;
        ++polys;
    }
    return polys;
}

// ConstructAndCommitVertsForDrawing_2: ground ripples tilted by each record's probed slope angles.
// Unlike _0/_1 the record callbacks still run when the vertex map fails.
std::uint32_t ConstructAndCommitVertsForDrawing_2(D3TDeviceContext *context, const WeatherDrawGroup &group) {
    EffeParticleVertex *output = MapEffeParticleVertices(context, static_cast<std::uint32_t>(group.count));
    const Matrix4x4 saved = workTransformMatrix;
    std::uint32_t polys = 0;
    for (std::int32_t index = 0; index < group.count; ++index) {
        auto *const ripple = reinterpret_cast<RippleParticleNative *>(group.records + static_cast<std::size_t>(index) * sizeof(RippleParticleNative));
        ripple->random(reinterpret_cast<RainParticleNative *>(ripple), group.owner);
        if ((ripple->flags & 0x80000000U) != 0) {
            const float size = group.scaledWidth * ripple->scale;
            const std::uint32_t colour = *ripple->colour;
            workTransformMatrix.rows[3] = ApplyWeatherViewTransform(ripple->position);
            RotateWeatherQuad(ripple->angleX, ripple->angleY, ripple->angleZ);
            if (!output) {
                ++polys;
            } else if (WriteWeatherQuad(output, size, size, colour)) {
                output += 6;
                ++polys;
            }
        }
        workTransformMatrix = saved;
    }
    return output ? polys : 0U;
}

// apply_effe_xform: alternate 0x20000000 groups only advance their record callbacks.
void ApplyWeatherRecordCallbacks(const WeatherDrawGroup &group) {
    if ((group.flags & 0x10000000U) != 0)
        return;
    const Matrix4x4 saved = workTransformMatrix;
    for (std::int32_t index = 0; index < group.count; ++index) {
        auto *const record = reinterpret_cast<RainParticleNative *>(group.records + static_cast<std::size_t>(index) * static_cast<std::size_t>(group.stride));
        record->random(record, group.owner);
        workTransformMatrix = saved;
    }
}

void SubmitWeatherParticleGroup(
    D3TDeviceContext *context, const WeatherDrawGroup &group, const EffeMaterialWords &groupMaterial, const std::uint32_t &groupTextureHandle) {
    if ((group.flags & 0x20000000U) != 0) {
        ApplyWeatherRecordCallbacks(group);
        return;
    }
    using WeatherConstructor = std::uint32_t (*)(D3TDeviceContext *, const WeatherDrawGroup &);
    // EFFE_FnSet: selectors 0, 2, 4 and 5 are the Script_NoOp nullsub.
    constexpr std::array<WeatherConstructor, 8> kConstructors{nullptr, ConstructAndCommitVertsForDrawing_0, nullptr, ConstructAndCommitVertsForDrawing_2,
        nullptr, nullptr, ConstructAndCommitVertsForDrawing_1, ConstructAndCommitVertsForDrawing_1};
    const WeatherConstructor constructor = kConstructors[(group.flags >> 25U) & 7U];
    if (!constructor)
        return;
    const EffeMaterialWords material = groupMaterial;
    const std::uint32_t handle = groupTextureHandle;
    const std::uint32_t texture = (material.texture & 8U) != 0 && handle ? handle : d3t::g_defaultWhiteTexture;
    const std::uint32_t polys = constructor(context, group);
    const std::uint32_t submitted = CommitEffeParticleVertices(context, polys, texture, (group.flags & 0x40000000U) != 0, &material);
    g_effeSubmittedParticleCount += submitted;
    if (submitted)
        ++g_effeSubmittedParticleSystems;
}

} // namespace shenmue::s1::effe_detail
