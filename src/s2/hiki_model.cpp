#include "s2/hiki.hpp"

#include <bit>
#include <cstdlib>
#include <cstring>
#include <limits>
#if defined(_M_X64) || defined(__x86_64__)
#include <emmintrin.h>
#endif

namespace shenmue::s2 {

float g_hikiOpacity{};
std::array<HikiWaveVertex, HIKI_ALPHA_GRID_SIZE> g_hikiWaveVertices{};
std::array<HikiTextureRecord, 4> g_hikiTextures{};

// @verified - p
void RelocateHikiModel(HikiModelNode &node, const void *image) {
    const std::byte *const base = static_cast<const std::byte *>(image);
    if (node.entityOffset) {
        node.entityOffset = HLib::GetDynamicHeapOffset(base + node.entityOffset);
        HikiModelEntity *const entity = static_cast<HikiModelEntity *>(HLib::GetPointerFromDynamicHeapOffsetBase(node.entityOffset));
        if (entity->vertexOffset)
            entity->vertexOffset = HLib::GetDynamicHeapOffset(base + entity->vertexOffset);
        if (entity->chunkOffset)
            entity->chunkOffset = HLib::GetDynamicHeapOffset(base + entity->chunkOffset);
    }
    if (node.childOffset) {
        node.childOffset = HLib::GetDynamicHeapOffset(base + node.childOffset);
        RelocateHikiModel(*static_cast<HikiModelNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(node.childOffset)), image);
    }
    if (node.siblingOffset) {
        node.siblingOffset = HLib::GetDynamicHeapOffset(base + node.siblingOffset);
        RelocateHikiModel(*static_cast<HikiModelNode *>(HLib::GetPointerFromDynamicHeapOffsetBase(node.siblingOffset)), image);
    }
    if (node.parentOffset)
        node.parentOffset = HLib::GetDynamicHeapOffset(base + node.parentOffset);
    if (node.matrixOffset)
        node.matrixOffset = HLib::GetDynamicHeapOffset(base + node.matrixOffset);
    if (node.taskDataOffset)
        node.taskDataOffset = HLib::GetDynamicHeapOffset(base + node.taskDataOffset);
}

// @verified - p
void InitializeHikiWaveHeights(std::span<const float, HIKI_ALPHA_GRID_SIZE> heights, std::uint32_t diffuse) {
    std::size_t destination = 0;
    for (std::size_t row = 1; row < 17; ++row) {
        for (std::size_t column = 0; column < 17; ++column) {
            if (row == 1) {
                g_hikiWaveVertices[destination].height = heights[column];
                g_hikiWaveVertices[destination++].diffuse = diffuse;
            }
            g_hikiWaveVertices[destination].height = heights[row * 17 + column];
            g_hikiWaveVertices[destination++].diffuse = diffuse;
        }
    }
    g_hikiWaveVertices[32].diffuse = g_hikiWaveVertices[0].diffuse;
    g_hikiWaveVertices[32].specular = g_hikiWaveVertices[0].specular;
    g_hikiWaveVertices[33].diffuse = g_hikiWaveVertices[1].diffuse;
    g_hikiWaveVertices[33].specular = g_hikiWaveVertices[1].specular;
    for (std::size_t index = 50; index <= 288; index += 17) {
        g_hikiWaveVertices[index].diffuse = g_hikiWaveVertices[index - 16].diffuse;
        g_hikiWaveVertices[index].specular = g_hikiWaveVertices[index - 16].specular;
    }
    for (std::size_t column = 0; column < 17; ++column) {
        g_hikiWaveVertices[272 + column].diffuse = g_hikiWaveVertices[column * 2].diffuse;
        g_hikiWaveVertices[272 + column].specular = g_hikiWaveVertices[column * 2].specular;
    }
}

namespace {
std::int64_t TruncateHikiOpacity(float value) {
#if defined(_M_X64) || defined(__x86_64__)
    return _mm_cvttss_si64(_mm_set_ss(value));
#else
    return value >= -0x1p63f && value < 0x1p63f ? static_cast<std::int64_t>(value) : std::numeric_limits<std::int64_t>::min();
#endif
}
}

// @verified - p
void ParseHikiData(std::span<const std::uint8_t, HIKI_ALPHA_GRID_SIZE> alpha) {
    const float opacityScale = g_hikiOpacity;
    std::size_t destination = 0;
    for (std::size_t row = 0; row < 17; ++row) {
        for (std::size_t column = 0; column < 2; ++column) {
            const std::uint32_t opacity = static_cast<std::uint32_t>(TruncateHikiOpacity(alpha[row * 17 + column] * opacityScale));
            HikiWaveVertex &vertex = g_hikiWaveVertices[destination++];
            vertex.diffuse = (vertex.diffuse & 0x00ffffffU) | (opacity << 24U);
        }
    }
    for (std::size_t column = 2; column < 17; ++column) {
        for (std::size_t row = 0; row < 17; ++row) {
            const std::uint32_t opacity = static_cast<std::uint32_t>(TruncateHikiOpacity(alpha[row * 17 + column] * opacityScale));
            HikiWaveVertex &vertex = g_hikiWaveVertices[destination++];
            vertex.diffuse = (vertex.diffuse & 0x00ffffffU) | (opacity << 24U);
        }
    }
}

// @verified - p
void InitializeHikiTextureCoordinates(std::int32_t repeatU, std::int32_t repeatV, HikiModelEntity &entity) {
    std::uint32_t coordinates[HIKI_ALPHA_GRID_SIZE];
    const float denominatorV = static_cast<float>(16 / repeatV);
    const float denominatorU = static_cast<float>(16 / repeatU);
    for (std::int32_t row = 0; row < 17; ++row) {
        for (std::int32_t column = 0; column < 17; ++column) {
            const std::uint16_t u = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(row) * 256.0f / denominatorU));
            const std::uint16_t v = static_cast<std::uint16_t>(static_cast<std::int32_t>(static_cast<float>(column) * 256.0f / denominatorV));
            coordinates[row * 17 + column] = (static_cast<std::uint32_t>(u) << 16U) | v;
        }
    }
    const std::uint32_t *first = coordinates;
    const std::uint32_t *remaining = coordinates + 17;
    std::uint32_t *command = static_cast<std::uint32_t *>(HLib::GetPointerFromDynamicHeapOffsetBase(entity.chunkOffset));
    for (;;) {
        const std::uint8_t type = static_cast<std::uint8_t>(*command);
        if (type <= 5) {
            std::uint32_t strips = command[5];
            std::int32_t paired = 17;
            command += 6;
            while (strips--) {
                std::uint32_t count = static_cast<std::uint32_t>(std::abs(static_cast<std::int32_t>(*command++)));
                while (count--) {
                    const std::uint32_t flags = *command++;
                    if (paired > 0) {
                        if (flags & 1U) {
                            command[5] = *first++;
                            command[14] = *remaining++;
                            command += 17;
                            --count;
                        }
                        --paired;
                    } else if (flags & 1U) {
                        command[5] = *remaining++;
                        command += 8;
                    }
                }
            }
        } else if (type == 7) {
            command[1] = 256;
            command += 2;
        } else if (type == 255) {
            return;
        } else {
            command = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(command) + (static_cast<std::int32_t>(*command) >> 8));
        }
    }
}

// @verified - p
void ApplyHikiTexture(std::int32_t textureIndex, HikiModelEntity &entity) {
    const HikiTextureRecord &texture = g_hikiTextures[textureIndex];
    const HikiWaveVertex *vertex = g_hikiWaveVertices.data();
    std::uint32_t *command = static_cast<std::uint32_t *>(HLib::GetPointerFromDynamicHeapOffsetBase(entity.chunkOffset));
    while (static_cast<std::uint8_t>(*command) != 255) {
        const std::int32_t bytes = static_cast<std::int32_t>(*command) >> 8;
        if (static_cast<std::uint8_t>(*command) <= 5) {
            *command = (static_cast<std::uint32_t>(bytes) << 8U) | ((texture.flags & 1U) ? 3U : 4U);
            std::memcpy(command + 1, &texture.header, sizeof(texture.header));
            std::uint32_t strips = command[5];
            std::uint32_t *data = command + 6;
            while (strips--) {
                std::uint32_t count = static_cast<std::uint32_t>(std::abs(static_cast<std::int32_t>(*data++)));
                while (count--) {
                    if (*data++ & 1U) {
                        data[0] = std::bit_cast<std::uint32_t>(vertex->height);
                        data[6] = vertex->diffuse;
                        data[7] = vertex->specular;
                        data += 8;
                        ++vertex;
                    }
                }
            }
        }
        command = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(command) + bytes);
    }
}

// @verified - p
void ScrollHikiTexture(std::int32_t textureIndex, std::int16_t u, std::int16_t v, HikiModelEntity &entity) {
    const HikiTextureRecord &texture = g_hikiTextures[textureIndex];
    const HikiWaveVertex *vertex = g_hikiWaveVertices.data();
    std::int32_t period = 0;
    std::uint32_t *command = static_cast<std::uint32_t *>(HLib::GetPointerFromDynamicHeapOffsetBase(entity.chunkOffset));
    for (;;) {
        const std::uint8_t type = static_cast<std::uint8_t>(*command);
        const std::int32_t bytes = static_cast<std::int32_t>(*command) >> 8;
        if (type <= 5) {
            const std::int16_t firstV = static_cast<std::int16_t>(v + command[13]);
            const std::int16_t firstU = static_cast<std::int16_t>(u + (command[13] >> 16U));
            const std::int32_t signU = firstU > period ? 1 : (firstU < 0 ? -1 : 0);
            const std::int32_t signV = firstV > period ? 1 : (firstV < 0 ? -1 : 0);
            *command = (static_cast<std::uint32_t>(bytes) << 8U) | ((texture.flags & 1U) ? 3U : 4U);
            std::memcpy(command + 1, &texture.header, sizeof(texture.header));
            std::int32_t paired = 17;
            std::uint32_t strips = command[5];
            std::uint32_t *data = command + 6;
            while (strips--) {
                std::uint32_t count = static_cast<std::uint32_t>(std::abs(static_cast<std::int32_t>(*data++)));
                while (count--) {
                    if (*data++ & 1U) {
                        data[0] = std::bit_cast<std::uint32_t>(vertex->height);
                        const std::uint16_t shiftedV = static_cast<std::uint16_t>(v + data[5] - period * signV);
                        const std::uint16_t shiftedU = static_cast<std::uint16_t>(u + (data[5] >> 16U) - period * signU);
                        data[5] = (static_cast<std::uint32_t>(shiftedU) << 16U) | shiftedV;
                        data[6] = vertex->diffuse;
                        data[7] = vertex->specular;
                        ++vertex;
                        if (paired > 0) {
                            data[9] = std::bit_cast<std::uint32_t>(vertex->height);
                            const std::uint16_t secondV = static_cast<std::uint16_t>(v + data[14] - period * signV);
                            const std::uint16_t secondU = static_cast<std::uint16_t>(u + (data[14] >> 16U) - period * signU);
                            data[14] = (static_cast<std::uint32_t>(secondU) << 16U) | secondV;
                            data[15] = vertex->diffuse;
                            data[16] = vertex->specular;
                            ++vertex;
                            data += 17;
                            --count;
                        } else {
                            data += 8;
                        }
                    }
                    if (paired > 0)
                        --paired;
                }
            }
            command = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(command) + bytes);
        } else if (type == 7) {
            period = static_cast<std::int32_t>(command[1]);
            command += 2;
        } else if (type == 255) {
            return;
        } else {
            command = reinterpret_cast<std::uint32_t *>(reinterpret_cast<std::byte *>(command) + bytes);
        }
    }
}

} // namespace shenmue::s2
