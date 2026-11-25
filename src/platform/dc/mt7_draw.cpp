#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#include "s2/model_render_dc.hpp"
#include <cstring>

namespace shenmue { namespace dc { namespace sdk {

MT7LightingParameters g_mt7LightingParameters = {};
__attribute__((aligned(32))) Matrix4x4 g_mt7ScreenTransform = {};
Vector4 g_mt7SingleLightDirection = {SHEN_FIELD(x) 1.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 0.0f, SHEN_FIELD(w) 1.0f};
Vector3 g_mt7ViewDirection = (Vector3){SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 1.0f};
Vector3 g_mt7ViewAxis = (Vector3){SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 1.0f};
Vector3 g_mt7ReflectedWorldDirection = {}, g_mt7ReflectedModelDirection = {};
Vector3 *g_mt7ReflectedDirection = 0;
std::int32_t g_mt7AlternateLighting = 0;

namespace {

// @verified - d
void TransformMT7Vector(const Vector3 &input, Vector3 &output) {
    const float *source = input.data();
    float *destination = output.data() + 3;
    asm volatile("fmov @%0+,fr4\n\tfmov @%0+,fr5\n\tfmov @%0+,fr6\n\tfldi0 fr7\n\tftrv xmtrx,fv4\n\t"
                 "fmov fr6,@-%1\n\tfmov fr5,@-%1\n\tfmov fr4,@-%1"
                 : "+r"(source), "+r"(destination) : : "fr4", "fr5", "fr6", "fr7", "memory");
}

// @verified - d
void TransformMT7Point(const Vector3 &input, Vector3 &output) {
    njCalcPoint(0, const_cast<Vector3 *>(&input), &output);
}

} // namespace

// @verified - d
void MultiplyMT7WorkMatrix(const Matrix4x4 *matrix) {
    Matrix4x4 work;
    njGetMatrix(&work);
    njSetMatrix(0, matrix);
    njMultiMatrix(0, &work);
}

// @verified - d
void LoadMT7RasterMatrix(const Matrix4x4 *matrix) {
    Matrix4x4 raster;
    for (std::uint32_t row = 0; row < 4; ++row) {
        const Vector4 &source = matrix->rows[row];
        raster.rows[row].x = source.w;
        raster.rows[row].y = source.x;
        raster.rows[row].z = source.y;
        raster.rows[row].w = -source.w;
    }
    njSetMatrix(0, &raster);
}

// @verified - d
void TransposeMT7WorkMatrix() {
    njTransposeMatrix(0);
}

// @verified - d
void SetMT7SingleLightDirection(float x, float y, float z) {
    g_mt7SingleLightDirection.x = x;
    g_mt7SingleLightDirection.y = y;
    g_mt7SingleLightDirection.z = z;
    z += 1.0f;
    g_mt7SingleLightDirection.w = z;
    g_mt7ViewDirection = (Vector3){SHEN_FIELD(x) x, SHEN_FIELD(y) y, SHEN_FIELD(z) z};
    g_mt7ViewAxis = (Vector3){SHEN_FIELD(x) 0.0f, SHEN_FIELD(y) 0.0f, SHEN_FIELD(z) 1.0f};
}

// @verified - d
void SetMT7SingleLightDirection(const Vector3 *direction) {
    SetMT7SingleLightDirection(direction->x, direction->y, direction->z);
}

// @verified - d
void SetMT7ReflectedDirection(const Vector3 *direction) {
    if (direction) {
        const Vector3 negative = {SHEN_FIELD(x) -direction->x, SHEN_FIELD(y) -direction->y, SHEN_FIELD(z) -direction->z};
        TransformMT7Vector(negative, g_mt7ReflectedWorldDirection);
        g_mt7ReflectedDirection = &g_mt7ReflectedModelDirection;
    } else {
        g_mt7ReflectedDirection = 0;
    }
}

// @verified - d
std::uint32_t PrepareMT7Mesh(void *mesh, const void *textures) {
    std::uint32_t *material = 0;
    std::uint32_t result = 0;
    do {
        result |= PrepareMT7MeshMaterial(mesh, textures, material, &material, 1);
    } while (material);
    return result;
}

// @verified - d
void PrepareMT7SingleLight() {
    TransposeMT7WorkMatrix();
    const Vector3 direction = {SHEN_FIELD(x) g_mt7SingleLightDirection.x, SHEN_FIELD(y) g_mt7SingleLightDirection.y, SHEN_FIELD(z) g_mt7SingleLightDirection.z};
    TransformMT7Vector(direction, g_mt7LightingParameters.lightDirection);
    TransformMT7Vector(g_mt7ViewDirection, g_mt7LightingParameters.viewDirection);
    const float scale = kMT7ViewDirectionLength / njScalor(&g_mt7LightingParameters.viewDirection);
    g_mt7LightingParameters.viewDirection.x *= scale;
    g_mt7LightingParameters.viewDirection.y *= scale;
    g_mt7LightingParameters.viewDirection.z *= scale;
    TransposeMT7WorkMatrix();
}

// @verified - d
void PrepareMT7LightTransforms() {
    matrix_stack::Matrix4x4_Push(0);
    Matrix4x4::NormalizeAndSetTransformMatrix();
    for (std::uint32_t index = 0; index < kLightCount; ++index) {
        std::uint32_t *const light = g_lightRecords[index];
        if (static_cast<std::uint16_t>(light[0]) != 0) {
            Vector3 direction, position;
            std::memcpy(&direction, light + 0x68 / 4, sizeof(direction));
            TransformMT7Vector(direction, direction);
            std::memcpy(light + 0x80 / 4, &direction, sizeof(direction));
            std::memcpy(&position, light + 0x5c / 4, sizeof(position));
            TransformMT7Point(position, position);
            std::memcpy(light + 0x74 / 4, &position, sizeof(position));
        }
    }
    Matrix4x4::Pop(1);
}

// @verified - d
void PrepareMT7Projection() {
    matrix_stack::Matrix4x4_Push(0);
    MultiplyMT7WorkMatrix(&g_mt7ScreenTransform);
}

// @verified - d
void SubmitSingleLightMT7Mesh(void *mesh) {
    const s2::MT7MeshHeader *const header = reinterpret_cast<const s2::MT7MeshHeader *>(mesh);
    if (header->state == 0 || header->state == 1)
        ExecuteSingleLightMT7Packets(mesh, &g_mt7LightingParameters);
    Matrix4x4::Pop(1);
}

// @verified - d
void SubmitSingleLightMT7MeshAlpha(void *mesh, float alpha) {
    const s2::MT7MeshHeader *const header = reinterpret_cast<const s2::MT7MeshHeader *>(mesh);
    if (header->state == 0 || header->state == 1)
        ExecuteSingleLightMT7PacketsAlpha(mesh, &g_mt7LightingParameters, alpha);
    Matrix4x4::Pop(1);
}

// @verified - d
void SubmitLitMT7Mesh(void *mesh) {
    if (reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->state >= 0) {
        if (!g_mt7AlternateLighting)
            ExecuteLitMT7Packets(mesh, &g_mt7LightingParameters);
        else
            ExecuteAlternateLitMT7Packets(mesh, &g_mt7LightingParameters);
    }
    Matrix4x4::Pop(1);
}

// @verified - d
void SubmitLitMT7MeshAlpha(void *mesh, float alpha) {
    *reinterpret_cast<volatile std::uint32_t *>(kStoreQueueAddressControl0) = 0x0c;
    *reinterpret_cast<volatile std::uint32_t *>(kStoreQueueAddressControl1) = 0x0c;
    if (reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->state >= 0) {
        if (!g_mt7AlternateLighting)
            ExecuteLitMT7PacketsAlpha(mesh, &g_mt7LightingParameters, alpha);
        else
            ExecuteAlternateLitMT7PacketsAlpha(mesh, &g_mt7LightingParameters, alpha);
    }
    Matrix4x4::Pop(1);
}

// @verified - d
void DrawSingleLightMT7Mesh(void *mesh) {
    if (g_mt7ReflectedDirection) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::InvertRigidTransform();
        TransformMT7Vector(g_mt7ReflectedWorldDirection, *g_mt7ReflectedDirection);
        Matrix4x4::Pop(1);
        g_mt7ReflectedDirection->x /= g_modelScale;
        g_mt7ReflectedDirection->y /= g_modelScale;
        g_mt7ReflectedDirection->z /= g_modelScale;
    }
    if (!(reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->flags & 2U))
        PrepareMT7SingleLight();
    PrepareMT7Projection();
    SubmitSingleLightMT7Mesh(mesh);
}

// @verified - d
void DrawSingleLightMT7MeshAlpha(void *mesh, float alpha) {
    if (g_mt7ReflectedDirection) {
        matrix_stack::Matrix4x4_Push(0);
        Matrix4x4::InvertRigidTransform();
        TransformMT7Vector(g_mt7ReflectedWorldDirection, *g_mt7ReflectedDirection);
        Matrix4x4::Pop(1);
        g_mt7ReflectedDirection->x /= g_modelScale;
        g_mt7ReflectedDirection->y /= g_modelScale;
        g_mt7ReflectedDirection->z /= g_modelScale;
    }
    if (!(reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->flags & 2U))
        PrepareMT7SingleLight();
    PrepareMT7Projection();
    SubmitSingleLightMT7MeshAlpha(mesh, alpha);
}

// @verified - d
void DrawLitMT7Mesh(void *mesh) {
    if (reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->flags & 0x10U)
        PrepareMT7SingleLight();
    PrepareMT7LightTransforms();
    PrepareMT7Projection();
    SubmitLitMT7Mesh(mesh);
}

// @verified - d
void DrawLitMT7MeshAlpha(void *mesh, float alpha) {
    if (reinterpret_cast<const s2::MT7MeshHeader *>(mesh)->flags & 0x10U)
        PrepareMT7SingleLight();
    PrepareMT7LightTransforms();
    PrepareMT7Projection();
    SubmitLitMT7MeshAlpha(mesh, alpha);
}

} } } // namespace shenmue::dc::sdk

#endif
