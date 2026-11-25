#include "platform/dc/mt7.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace dc {
namespace sdk {

float g_mt7AmbientIntensity = 0.30000001192092896f;
float g_mt7SpecularLookup[kMT7SpecularRows][kMT7SpecularSamples] = {};
const void *g_mt7PaletteData = 0;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
Vector4 g_mt7DiffuseChannels = {1.0f, 1.0f, 1.0f, 0.0f};
Vector4 g_mt7SpecularChannels = {1.0f, 1.0f, 1.0f, 0.0f};
#else
Vector4 g_mt7DiffuseChannels = {x : 0.0f, y : 1.0f, z : 1.0f, w : 1.0f};
Vector4 g_mt7SpecularChannels = {x : 0.0f, y : 1.0f, z : 1.0f, w : 1.0f};
#endif
Callback g_mt7VertexColourCallback = 0;
std::uint32_t g_mt7AlphaBlendMask = 0;
const std::uint8_t g_mt7VertexAngleLookup[128] = {
    0,
    0,
    1,
    1,
    1,
    2,
    2,
    2,
    3,
    3,
    3,
    3,
    4,
    4,
    4,
    5,
    5,
    5,
    6,
    6,
    6,
    7,
    7,
    7,
    8,
    8,
    8,
    8,
    9,
    9,
    9,
    10,
    10,
    10,
    11,
    11,
    11,
    11,
    12,
    12,
    12,
    13,
    13,
    13,
    13,
    14,
    14,
    14,
    15,
    15,
    15,
    15,
    16,
    16,
    16,
    17,
    17,
    17,
    17,
    18,
    18,
    18,
    18,
    19,
    19,
    19,
    19,
    20,
    20,
    20,
    20,
    21,
    21,
    21,
    21,
    22,
    22,
    22,
    22,
    23,
    23,
    23,
    23,
    23,
    24,
    24,
    24,
    24,
    25,
    25,
    25,
    25,
    25,
    26,
    26,
    26,
    26,
    26,
    27,
    27,
    27,
    27,
    27,
    28,
    28,
    28,
    28,
    28,
    29,
    29,
    29,
    29,
    29,
    29,
    30,
    30,
    30,
    30,
    30,
    31,
    31,
    31,
    31,
    31,
    31,
    32,
    32,
    32,
};

std::byte g_mt7PacketScratch[0x70] __attribute__((aligned(32))) = {};
std::byte g_mt7FloatingLightWorkspace[0x254] __attribute__((aligned(32))) = {};
std::byte g_mt7PackedLightWorkspace[0x294] __attribute__((aligned(32))) = {};

// @verified - d
void InitializeMT7Materials() {
    g_mt7LightingParameters.ambient = g_mt7AmbientIntensity;
    g_mt7PaletteData = 0;
    for (std::int32_t row = 0; row < static_cast<std::int32_t>(kMT7SpecularRows); ++row) {
        for (std::int32_t sample = 0; sample < static_cast<std::int32_t>(kMT7SpecularSamples); ++sample) {
            float value = static_cast<float>(sample) * 0.29289299249649048f;
            value /= 127.0f;
            value += 0.70710700750350952f;
            g_mt7SpecularLookup[row][sample] = 0.0f;
            value *= value;
            value += value;
            value -= 1.0f;
            value *= value;
            if (row < 2) {
                g_mt7SpecularLookup[row][sample] = value;
            } else {
                float result = 1.0f;
                for (std::int32_t exponent = 0; exponent < row; ++exponent)
                    result *= value;
                g_mt7SpecularLookup[row][sample] = result;
            }
        }
    }
}

// @verified - d
void SetMT7AmbientIntensity(float value) {
    g_mt7AmbientIntensity = value;
    g_mt7LightingParameters.ambient = value;
}

// @verified - d
void SetMT7AlphaBlendMode(std::uint32_t source, std::uint32_t destination) {
    g_mt7AlphaBlendMask = ((~source & 7U) << 5U) + ((~destination & 7U) << 2U);
}

// @verified - d
std::int32_t GetMT7VertexColourType() {
    std::int32_t type;
    if (!g_mt7VertexColourCallback)
        return 0;
    if (g_mt7VertexColourCallback == ProcessMT7FloatVertexColours)
        type = 1;
    else if (g_mt7VertexColourCallback == ProcessMT7PackedVertexColours)
        type = 2;
    return type;
}

// @verified - d
void SetMT7VertexColourType(std::int32_t type) {
    if (!type)
        g_mt7VertexColourCallback = 0;
    else if (type == 1)
        g_mt7VertexColourCallback = ProcessMT7FloatVertexColours;
    else if (type == 2)
        g_mt7VertexColourCallback = ProcessMT7PackedVertexColours;
}

// @verified - d
void MT7TexturePreparationNullsub() {
}

} // namespace sdk
} // namespace dc
} // namespace shenmue

#endif
