#include "s2/hand_dc.hpp"
#include "s2/event_matrix_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "platform/dc/matrix.hpp"
#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "platform/dc/sdk.hpp"
#endif
#include "s2/debug.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

void HMT_HandCalcSkin(HandPoseState *state, Matrix4x4 *matrices) {
    if (!state->modelData) {
        DebugLogToFileRemoved("!!!! HMT_HandCalcSkin:Error:No Envelope Data!!!!\n");
        return;
    }
    matrix_stack::Matrix4x4_Push(0);
    std::int32_t descriptorOffset;
    std::memcpy(&descriptorOffset, state->modelData + 4, sizeof(descriptorOffset));
    const HandSkinVertexHeader *header = reinterpret_cast<const HandSkinVertexHeader *>(state->modelData + 8 + descriptorOffset);
    HandSkinVertex *vertices = reinterpret_cast<HandSkinVertex *>(const_cast<HandSkinVertexHeader *>(header + 1));
    for (std::int32_t index = 0; index < header->vertexCount; ++index) {
        HandSkinVertex &vertex = vertices[index];
        vertex.deformedNormal.z = 0.0f;
        vertex.deformedNormal.y = 0.0f;
        vertex.deformedNormal.x = 0.0f;
        vertex.deformedPosition.z = 0.0f;
        vertex.deformedPosition.y = 0.0f;
        vertex.deformedPosition.x = 0.0f;
    }
    if (header->vertexCount > 0) {
        const HandSkinGroupHeader *groups = reinterpret_cast<const HandSkinGroupHeader *>(vertices + header->vertexCount);
        const std::byte *cursor = reinterpret_cast<const std::byte *>(groups + 1);
        for (std::int32_t group = 0; group < groups->groupCount; ++group) {
            const HandSkinInfluenceGroup &entry = *reinterpret_cast<const HandSkinInfluenceGroup *>(cursor);
            cursor += sizeof(HandSkinInfluenceGroup);
            if (entry.count > 0) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                matrix_stack::LoadMatrix(matrices + entry.joint);
#else
                const float *matrix = matrices[entry.joint].rows[0].data();
                asm volatile("fschg\n\tfmov @%0+,fr1\n\tfmov @%0+,fr3\n\tfmov @%0+,fr5\n\tfmov @%0+,fr7\n\t"
                             "fmov @%0+,fr9\n\tfmov @%0+,fr11\n\tfmov @%0+,fr13\n\tfmov @%0+,fr15\n\tfschg"
                    : "+r"(matrix)
                    :
                    : "memory");
#endif
            }
            for (std::int32_t influence = 0; influence < entry.count; ++influence) {
                const HandSkinInfluence &source = *reinterpret_cast<const HandSkinInfluence *>(cursor);
                HandSkinVertex &vertex = vertices[source.vertex];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                Vector3 position, normal;
                Matrix4x4::TransformPoint(vertex.position, position);
                Matrix4x4::Transform(vertex.normal, normal);
#else
                const float *positionSource = vertex.position;
                const float *normalSource = vertex.normal;
                Vector3 position, normal;
                float *positionDestination = position + 3;
                float *normalDestination = normal + 3;
                asm volatile("fmov @%0+,fr0\n\tfmov @%0+,fr1\n\tfmov @%0+,fr2\n\tfldi1 fr3\n\tftrv xmtrx,fv0\n\t"
                             "fmov @%1+,fr4\n\tfmov @%1+,fr5\n\tfmov @%1+,fr6\n\tfldi0 fr7\n\tftrv xmtrx,fv4\n\t"
                             "fmov fr2,@-%2\n\tfmov fr1,@-%2\n\tfmov fr0,@-%2\n\t"
                             "fmov fr6,@-%3\n\tfmov fr5,@-%3\n\tfmov fr4,@-%3"
                    : "+r"(positionSource), "+r"(normalSource), "+r"(positionDestination), "+r"(normalDestination)
                    :
                    : "fr0", "fr1", "fr2", "fr3", "fr4", "fr5", "fr6", "fr7", "memory");
#endif
                vertex.deformedPosition.x += position.x * source.weight;
                vertex.deformedPosition.y += position.y * source.weight;
                vertex.deformedPosition.z += position.z * source.weight;
                vertex.deformedNormal.x += normal.x * source.weight;
                vertex.deformedNormal.y += normal.y * source.weight;
                vertex.deformedNormal.z += normal.z * source.weight;
                cursor += sizeof(HandSkinInfluence);
            }
        }
        if (groups->groupCount > 0) {
            const HandSkinRemapHeader *remap = NormalizeHandSkinVertices(vertices, reinterpret_cast<const HandSkinNormalHeader *>(cursor));
            std::int32_t vertex = remap->firstVertex;
            const HandSkinRemap *mapping = reinterpret_cast<const HandSkinRemap *>(remap + 1);
            for (std::int32_t index = 0; index < remap->count; ++index) {
                const HandSkinVertex &source = vertices[vertex];
#if SHENMUE_IS_DC && SHENMUE_IS_S2
                const std::int32_t destinationOffset = mapping[index].destinationByteOffset;
                const std::int32_t nextVertex = mapping[index].nextVertex;
                std::uint32_t values[6];
                std::memcpy(values, &source.deformedPosition, sizeof(values));
                std::byte *const destination = reinterpret_cast<std::byte *>(state->meshCopy) + destinationOffset;
                for (std::int32_t word = 5; word >= 0; --word)
                    std::memcpy(destination + word * 4, values + word, 4);
                vertex = nextVertex;
#else
                const float values[6] = {source.deformedPosition.x, source.deformedPosition.y, source.deformedPosition.z, source.deformedNormal.x,
                    source.deformedNormal.y, source.deformedNormal.z};
                std::memcpy(reinterpret_cast<std::byte *>(state->meshCopy) + mapping[index].destinationByteOffset, values, sizeof(values));
                vertex = mapping[index].nextVertex;
#endif
            }
        }
    }
    PopCurrentMatrix(1);
}

} // namespace s2
} // namespace shenmue

#endif
