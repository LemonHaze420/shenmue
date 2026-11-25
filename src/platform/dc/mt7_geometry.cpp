#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/am2_vector.hpp"
#include "platform/dc/matrix.hpp"
#include <cmath>
#include <cstring>

inline void *operator new(std::size_t, void *storage) {
    return storage;
}

namespace shenmue {
namespace dc {
namespace sdk {
namespace {

struct GeometryLightInfluence {
    std::uint32_t kind;
    Vector4 colour;
    Vector3 position, direction;
    float rangeSquared, falloffPower, coneCosine;
};
typedef char GeometryLightLayoutCheck1[(sizeof(GeometryLightInfluence) == 0x38) ? 1 : -1];
static const std::int32_t kGeometryLightLimit = 128;
std::int32_t geometryLightCount;
GeometryLightInfluence geometryLights[kGeometryLightLimit];

template <typename T> T Read(const std::byte *data, std::size_t offset = 0) {
    T value;
    std::memcpy(&value, data + offset, sizeof(value));
    return value;
}

template <typename T> void Write(std::byte *data, std::size_t offset, const T &value) {
    std::memcpy(data + offset, &value, sizeof(value));
}

std::int32_t Truncate(float value) {
    return static_cast<std::int32_t>(value);
}

Vector4 UnpackGeometryColour(const std::uint32_t *source) {
    const std::int32_t red = *source >> 24;
    const std::int32_t green = (*source >> 16) & 255U;
    const std::int32_t blue = (*source >> 8) & 255U;
    const std::int32_t alpha = *reinterpret_cast<const std::uint8_t *>(source);
    Vector4 colour;
    colour.x = red / 255.0f;
    colour.y = green / 255.0f;
    colour.z = blue / 255.0f;
    colour.w = alpha / 255.0f;
    return colour;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t PackGeometryColour(const Vector4 &colour) {
    const std::uint8_t red = colour.x < 0.0f ? 0 : colour.x > 1.0f ? 255 : static_cast<std::uint8_t>(colour.x * 255.0f);
    const std::uint8_t green = colour.y < 0.0f ? 0 : colour.y > 1.0f ? 255 : static_cast<std::uint8_t>(colour.y * 255.0f);
    const std::uint8_t blue = colour.z < 0.0f ? 0 : colour.z > 1.0f ? 255 : static_cast<std::uint8_t>(colour.z * 255.0f);
    const std::uint8_t alpha = colour.w < 0.0f ? 0 : colour.w > 1.0f ? 255 : static_cast<std::uint8_t>(colour.w * 255.0f);
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}
#else
// @verified - d
std::uint32_t PackGeometryColour(const Vector4 &colour) {
    std::uint32_t packed = 0;
    for (std::uint32_t index = 0; index < 4; ++index) {
        const float value = colour[index];
        const std::uint32_t channel = value < 0.0f ? 0U : value > 1.0f ? 255U : static_cast<std::uint32_t>(Truncate(value * 255.0f)) & 255U;
        packed = (packed << 8) | channel;
    }
    return packed;
}
#endif

Vector3 UnpackGeometryNormal(std::uint32_t packed) {
    return (Vector3){static_cast<float>(static_cast<std::int8_t>(static_cast<std::uint8_t>(packed >> 16))) / 127.0f,
        static_cast<float>(static_cast<std::int8_t>(static_cast<std::uint8_t>(packed >> 8))) / 127.0f,
        static_cast<float>(static_cast<std::int8_t>(static_cast<std::uint8_t>(packed))) / 127.0f};
}

// @verified - d
float GeometryLightDistance(const Vector3 &position, const Vector3 &light, Vector3 &delta, float rangeSquared) {
    delta.x = light.x - position.x;
    delta.y = light.y - position.y;
    delta.z = light.z - position.z;
    const float square = am2Vector3Dot(&delta, &delta);
    if (square > rangeSquared)
        return -1.0f;
    return InverseSqrt(square);
}

} // namespace

Vector2 g_fftForwardTwiddles[32];
Vector2 g_fftBackwardTwiddles[32];

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ResetMT7GeometryLightMesh(void *meshData) {
    std::int32_t *const mesh = static_cast<std::int32_t *>(meshData);
    std::int32_t *command;
    std::int32_t *shaderSlot;
    std::int32_t header;
    std::int32_t shader = 0;
    std::int32_t marker;
    std::int32_t normal;
    std::uint32_t count;
    if (mesh[0] < 0 || !mesh[6] || !(mesh[1] & 0x20))
        return;
    mesh[1] &= ~0x20;
    command = mesh + 6;
    for (;;) {
        header = *command;
        if (!header)
            break;
        if (header < 0) {
            command += 9;
            shaderSlot = command;
            shader = *command++;
            command += 10;
            header = *command;
            marker = command[5];
            if (shader == -3) {
                if (marker & 0xff000000)
                    *shaderSlot = (marker & 0x01000000) ? 0 : -1;
            }
        }
        command++;
        count = *command++;
        if (header & 8)
            count *= 3;
        while (count-- > 0) {
            if (*command & 1) {
                normal = command[3];
                if (shader == -3) {
                    if (normal & 0xff000000) {
                        const std::int32_t x = static_cast<std::int8_t>(normal >> 16);
                        const std::int32_t y = static_cast<std::int8_t>(normal >> 8);
                        const std::int32_t z = static_cast<std::int8_t>(normal);
                        Vector3 *const target = reinterpret_cast<Vector3 *>(command + 3);
                        target->x = x / 127.0f;
                        target->y = y / 127.0f;
                        target->z = z / 127.0f;
                    }
                }
                command += 8;
            } else {
                command += 2;
            }
        }
    }
}
#else
// @verified - d
void ResetMT7GeometryLightMesh(void *meshData) {
    std::byte *const mesh = static_cast<std::byte *>(meshData);
    const std::uint32_t flags = Read<std::uint32_t>(mesh, 4);
    if (Read<std::int32_t>(mesh) < 0 || !Read<std::uint32_t>(mesh, 24) || !(flags & 0x20U))
        return;
    Write(mesh, 4, flags & ~0x20U);
    std::byte *command = mesh + 24;
    std::int32_t shader = 0;
    while (Read<std::uint32_t>(command)) {
        if (Read<std::int32_t>(command) < 0) {
            shader = Read<std::int32_t>(command, 36);
            if (shader == -3) {
                const std::uint32_t marker = Read<std::uint32_t>(command, 100);
                if (marker & 0xff000000U)
                    Write(command, 36, std::int32_t(marker & 0x01000000U ? 0 : -1));
            }
            command += 80;
        }
        std::uint32_t count = Read<std::uint32_t>(command, 4);
        if (Read<std::uint32_t>(command) & 8U)
            count *= 3;
        command += 8;
        for (std::uint32_t index = 0; index < count; ++index) {
            if (!(Read<std::uint32_t>(command) & 1U)) {
                command += 8;
                continue;
            }
            const std::uint32_t normal = Read<std::uint32_t>(command, 12);
            if (shader == -3 && (normal & 0xff000000U))
                Write(command, 12, UnpackGeometryNormal(normal));
            command += 32;
        }
    }
}
#endif

std::int32_t BakeMT7GeometryLightMesh(void *meshData, std::int32_t lightCount, const void *lights, const Vector4 *baseColour) {
    std::uint32_t *command = static_cast<std::uint32_t *>(meshData);
    const MT7GeometryLight *data = static_cast<const MT7GeometryLight *>(lights);
    std::int32_t index;
    const Vector4 *material = 0;
    float intensity = 0.0f;
    std::int32_t shader = 0;
    command[1] |= 0x20U;
    command += 2;
    const float *bounds = reinterpret_cast<const float *>(command);
    Vector3 centre;
    centre.x = *bounds++;
    centre.y = *bounds++;
    centre.z = *bounds++;
    const float radius = *bounds;
    Matrix4x4::TransformPoint(centre, centre);
    command += 4;
    geometryLightCount = 0;
    GeometryLightInfluence *light = geometryLights;
    for (index = 0; index < lightCount; ++index, ++data) {
        if (!data->enabled || !(data->kind & 8U))
            continue;
        const std::uint32_t kind = data->kind & 7U;
        light->kind = kind;
        switch (kind) {
        case 3:
        case 4: {
            Vector3 delta;
            delta.x = centre.x - data->position.x;
            delta.y = centre.y - data->position.y;
            delta.z = centre.z - data->position.z;
            if (Vector3::CalcMagnitude(delta) > data->range + radius)
                continue;
        }
        }
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::InvertRigidTransform();
        Matrix4x4::TransformPoint(data->position, light->position);
        Matrix4x4::Transform(data->direction, light->direction);
        Vector3::NormalizeInPlace(light->direction);
        Matrix4x4::Pop(1);
        light->colour = data->colour;
        light->colour.y *= data->intensity;
        light->colour.z *= data->intensity;
        light->colour.w *= data->intensity;
        light->rangeSquared = data->range * data->range;
        float falloff;
        light->falloffPower = (falloff = data->falloff * data->range, falloff *= falloff, falloff *= falloff, falloff * falloff);
        light->coneCosine = CosFixed(data->coneAngle);
        if (geometryLightCount < kGeometryLightLimit) {
            ++geometryLightCount;
            ++light;
        }
    }
    std::uint32_t flags;
    for (;;) {
        flags = *command;
        if (!flags)
            break;
        if (static_cast<std::int32_t>(flags) < 0) {
            command += 9;
            shader = static_cast<std::int32_t>(*command);
            *command++ = static_cast<std::uint32_t>(-3);
            intensity = *reinterpret_cast<const float *>(command++);
            material = reinterpret_cast<const Vector4 *>(command);
            command += 9;
            flags = *command++;
        } else {
            command++;
        }
        std::uint32_t count = *command++;
        if (flags & 8U)
            count *= 3;
        while (count-- > 0) {
            if ((*command & 1U) == 0) {
                command += 2;
                continue;
            }
            const Vector3 *vertex = reinterpret_cast<const Vector3 *>(command);
            Vector3 normal;
            Vector4 colour;
            if (shader != -3) {
                normal = *reinterpret_cast<const Vector3 *>(command + 3);
                const std::uint8_t x = static_cast<std::int32_t>(normal.x * 127.0f);
                const std::uint8_t y = static_cast<std::int32_t>(normal.y * 127.0f);
                const std::uint8_t z = static_cast<std::int32_t>(normal.z * 127.0f);
                command[3] = 0xff000000U | (x << 16) | (y << 8) | z;
                if (shader == -1) {
                    command[3] &= ~0x01000000U;
                    colour = *material;
                    command[4] = PackGeometryColour(colour);
                    command += 8;
                    continue;
                }
                colour.x = material->x;
                colour.y = baseColour->y * material->y * intensity;
                colour.z = baseColour->z * material->z * intensity;
                colour.w = baseColour->w * material->w * intensity;
            } else {
                const std::int32_t packed = static_cast<std::int32_t>(command[3]);
                if (!(packed & 0x01000000)) {
                    command += 8;
                    continue;
                }
                const std::int8_t x = packed >> 16;
                const std::int8_t y = packed >> 8;
                const std::int8_t z = packed;
                normal.x = x / 127.0f;
                normal.y = y / 127.0f;
                normal.z = z / 127.0f;
                new (&colour) Vector4(UnpackGeometryColour(command + 4));
            }
            float strength, attenuation;
            for (index = 0, light = geometryLights; index < geometryLightCount; ++index, ++light) {
                switch (light->kind) {
                case 2: {
                    strength = -njInnerProduct(&normal, &light->direction);
                    if (strength > 0.0f) {
                        colour.y += light->colour.y * material->y * strength;
                        colour.z += light->colour.z * material->z * strength;
                        colour.w += light->colour.w * material->w * strength;
                    }
                    break;
                }
                case 3: {
                    Vector3 delta;
                    attenuation = GeometryLightDistance(*vertex, light->position, delta, light->rangeSquared);
                    if (attenuation > 0.0f) {
                        strength = njInnerProduct(&normal, &delta);
                        if (strength > 0.0f) {
                            strength *= attenuation;
                            attenuation *= attenuation;
                            attenuation *= attenuation;
                            attenuation *= attenuation;
                            attenuation *= light->falloffPower;
                            if (attenuation > 1.0f)
                                attenuation = 1.0f;
                            strength *= attenuation;
                            colour.y += light->colour.y * material->y * strength;
                            colour.z += light->colour.z * material->z * strength;
                            colour.w += light->colour.w * material->w * strength;
                        }
                    }
                    break;
                }
                case 4: {
                    Vector3 delta;
                    attenuation = GeometryLightDistance(*vertex, light->position, delta, light->rangeSquared);
                    if (attenuation > 0.0f) {
                        strength = njInnerProduct(&normal, &delta);
                        if (strength > 0.0f) {
                            strength *= attenuation;
                            float cone = njInnerProduct(&delta, &light->direction) * attenuation;
                            if (light->coneCosine > cone)
                                break;
                            cone = (cone - light->coneCosine) / (1.0f - light->coneCosine);
                            attenuation *= attenuation;
                            attenuation *= attenuation;
                            attenuation *= attenuation;
                            attenuation *= light->falloffPower;
                            if (attenuation > 1.0f)
                                attenuation = 1.0f;
                            strength *= attenuation * cone;
                            colour.y += light->colour.y * material->y * strength;
                            colour.z += light->colour.z * material->z * strength;
                            colour.w += light->colour.w * material->w * strength;
                        }
                    }
                    break;
                }
                }
            }
            command[4] = PackGeometryColour(colour);
            command += 8;
        }
    }
    return geometryLightCount >= kGeometryLightLimit ? -1 : geometryLightCount;
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
