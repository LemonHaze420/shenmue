#include "s2/model_render_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// GameJam keeps the texture-animation offset and the motion rig's pose mode in the same word.
Vector2 modelTextureOffset asm("_shenmue_s2_texanim_pose_word") = {0.0f, 0.0f};
#else
static Vector2 modelTextureOffset = {0.0f, 0.0f};
#endif

// @verified - d
void nlSetTexanimUV(float u, float v) {
    modelTextureOffset.x = u;
    modelTextureOffset.y = v;
}

// @verified - d
void ApplyTexAnimUVs(void *mesh) {
    MT7MeshHeader *const header = static_cast<MT7MeshHeader *>(mesh);
    float u = modelTextureOffset.x, v = modelTextureOffset.y;
    if (header->flags & 0x200U)
        u += header->flags & 0x100U ? 1.0f : -1.0f;
    if (header->flags & 0x2000U)
        v += header->flags & 0x1000U ? 1.0f : -1.0f;
    header->flags &= 0xffff00ffU;
    std::uint32_t uRange = 0, vRange = 0;
    std::uint32_t *cursor = reinterpret_cast<std::uint32_t *>(header + 1);
    while (*cursor) {
        std::uint32_t primitive = *cursor;
        if (static_cast<std::int32_t>(primitive) < 0) {
            cursor[2] &= 0xfffe7fffU;
            cursor += sizeof(MT7MeshMaterialCommand) / sizeof(*cursor);
            primitive = *cursor;
        }
        std::uint32_t count = cursor[1];
        cursor += 2;
        if (primitive & MT7PrimitiveTriangleList)
            count *= 3U;
        while (count--) {
            if (*cursor & 1U) {
                MT7PacketVertex *const vertex = reinterpret_cast<MT7PacketVertex *>(cursor);
                vertex->u += u;
                uRange |= vertex->u < 0.0f ? 0x100U : vertex->u < 1.0f ? 0x10U : 1U;
                vertex->v += v;
                vRange |= vertex->v < 0.0f ? 0x100U : vertex->v < 1.0f ? 0x10U : 1U;
                cursor[7] |= 1U;
                cursor += sizeof(MT7PacketVertex) / sizeof(*cursor);
            } else {
                cursor += 2;
            }
        }
    }
    if (uRange == 0x100U)
        header->flags |= 0x300U;
    else if (uRange == 1U)
        header->flags |= 0x200U;
    if (vRange == 0x100U)
        header->flags |= 0x3000U;
    else if (vRange == 1U)
        header->flags |= 0x2000U;
}

// @verified - d
void RecursiveApplyTexAnimUVs(MT7Node *node) {
    if (!(node->kind & 0x10U) && (node->mesh || node->child)) {
        if (node->mesh)
            ApplyTexAnimUVs(node->mesh);
        if (node->child)
            RecursiveApplyTexAnimUVs(node->child);
    }
    if (node->sibling)
        RecursiveApplyTexAnimUVs(node->sibling);
}

// @verified - d
void UpdateModelTextureCoordinates(MT7ModelChunk *model) {
    if (model->name == SHEN_NAME4('M', 'D', 'L', '7') || model->name == SHEN_NAME4('M', 'D', 'O', '7'))
        RecursiveApplyTexAnimUVs(model->root);
}

// @verified - d
void UpdateMT7MeshTextureCoordinates(void *mesh) {
    MT7MeshHeader *const header = static_cast<MT7MeshHeader *>(mesh);
    if (header->state < 0)
        return;
    MT7MeshMaterialCommand *material = reinterpret_cast<MT7MeshMaterialCommand *>(header + 1);
    for (;;) {
        bool packedNormal = material->vertexFormat < -2;
        std::byte *cursor = reinterpret_cast<std::byte *>(material + 1);
        std::int32_t primitive = *reinterpret_cast<const std::int32_t *>(cursor);
        cursor += sizeof(primitive);
        bool processPrimitive = true;
        if (material->vertexFormat == -2 || !(primitive & MT7PrimitiveEnvironmentMapped)) {
            cursor = reinterpret_cast<std::byte *>(material + 1) + material->vertexBytes;
            primitive = *reinterpret_cast<const std::int32_t *>(cursor);
            cursor += sizeof(primitive);
            packedNormal = false;
            processPrimitive = primitive > 0;
        }
        if (processPrimitive) {
            do {
                std::uint32_t count = *reinterpret_cast<const std::uint32_t *>(cursor);
                cursor += sizeof(count);
                if (primitive & MT7PrimitiveTriangleList)
                    count *= 3U;
                do {
                    if (*reinterpret_cast<const std::uint32_t *>(cursor) & 1U) {
                        MT7PacketVertex *const vertex = reinterpret_cast<MT7PacketVertex *>(cursor);
                        const Vector3 normal = packedNormal ?
                            (Vector3){SHEN_FIELD(x) static_cast<float>(vertex->normal.packed.x), SHEN_FIELD(y) static_cast<float>(vertex->normal.packed.y),
                                SHEN_FIELD(z) static_cast<float>(vertex->normal.packed.z)} :
                            vertex->normal.floating;
                        const std::uint32_t retainedVBit = floatAsU32(vertex->v) & 1U;
                        Vector3 transformed;
                        Matrix4x4::Transform(normal, transformed);
                        const float scale = packedNormal ? 0.00390625f : 0.5f;
                        float u = 0.5f, v = 0.5f;
                        asm volatile("fmov %2,fr0\n\tfmac fr0,%3,%0\n\tfmac fr0,%4,%1"
                            : "+&f"(u), "+&f"(v)
                            : "f"(scale), "f"(transformed.x), "f"(transformed.y)
                            : "fr0");
                        vertex->u = u;
                        vertex->v = u32AsFloat((floatAsU32(v) & ~1U) | retainedVBit);
                        cursor += sizeof(MT7PacketVertex);
                    } else {
                        cursor += 8;
                    }
                } while (--count);
                primitive = *reinterpret_cast<const std::int32_t *>(cursor);
                cursor += sizeof(primitive);
            } while (primitive > 0);
        }
        if (!primitive)
            return;
        material = reinterpret_cast<MT7MeshMaterialCommand *>(cursor - sizeof(primitive));
    }
}

} // namespace s2
} // namespace shenmue

#endif
