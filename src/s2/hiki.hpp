#pragma once

#include "s2/hlib.hpp"
#include "s2/effect_texture.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>

namespace shenmue::s2 {

inline constexpr shen_name TASK_HIKI{SHEN_NAME(HIKI)};
inline constexpr const char *HIKI_MODEL_PATH = "/scene/misc/b05k207g/Wake.fbx";
inline constexpr std::uint16_t HIKI_MODEL_FLAGS = 0x1082;
inline constexpr std::size_t HIKI_ALPHA_GRID_SIZE = 17 * 17;
inline constexpr std::uint32_t HIKI_DIFFUSE_MASK = 0x0fffffff;

// MT6 eff_wave nodes retain signed offsets relative to HLib::DynamicHeapBase after admission.
struct HikiModelNode {
    std::uint32_t flags;
    std::int32_t entityOffset;
    std::array<std::byte, 36> transform;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t matrixOffset;
    std::int32_t taskDataOffset;
};

struct HikiModelEntity {
    std::array<std::byte, 4> reserved;
    std::int32_t vertexOffset;
    std::uint32_t vertexCount;
    std::int32_t chunkOffset;
    Vector3 centre;
    float clipRadius;
};

struct HikiWaveVertex {
    float height;
    std::uint32_t diffuse;
    std::uint32_t specular;
};

struct HikiTextureHeader {
    std::array<std::uint32_t, 4> words;
};

struct HikiTextureRecord {
    LegacyEffectTexture texture;
    HikiTextureHeader header;
    std::uint32_t flags;
    std::uint32_t reserved44;
};

static_assert(sizeof(HikiModelNode) == 0x40);
static_assert(offsetof(HikiModelNode, entityOffset) == 4);
static_assert(offsetof(HikiModelNode, childOffset) == 0x2c);
static_assert(offsetof(HikiModelNode, taskDataOffset) == 0x3c);
static_assert(sizeof(HikiModelEntity) == 0x20);
static_assert(offsetof(HikiModelEntity, chunkOffset) == 0xc);
static_assert(offsetof(HikiModelEntity, centre) == 0x10);
static_assert(sizeof(HikiWaveVertex) == 12);
static_assert(sizeof(HikiTextureHeader) == 16);
static_assert(sizeof(HikiTextureRecord) == 72);
static_assert(offsetof(HikiTextureRecord, header) == 48);
static_assert(offsetof(HikiTextureRecord, flags) == 64);

extern Vector3 g_hikiPosition;
extern float g_hikiOpacity;
extern std::array<HikiWaveVertex, HIKI_ALPHA_GRID_SIZE> g_hikiWaveVertices;
extern std::array<HikiTextureRecord, 4> g_hikiTextures;
extern std::int32_t g_hikiTextureIndex;
extern std::uint32_t g_effectDiffuseMask;
extern HLib::Task *g_hikiTask;
extern HikiModelNode *g_hikiModel;
extern void *g_hikiModelImage;
extern std::uint8_t *g_hikiAlphaData;
// Map admission clears the TEXN bank's handles without releasing the direct textures.
void ResetHikiTextures();

void HIKI_Enqueue();
void Hiki_Callback(void *taskData);
// Called only by the destination map's effect initialization, after module task retirement and the HLib arena sweep.
std::int64_t ResetHikiTaskData();
void AssignHikiPosition(const Vector3 &position);
void AssignHikiLocalPosition(const Vector3 &position);
void AssignHikiRotation(std::int32_t yaw);
void AssignHikiTextureScroll(std::int16_t u, std::int16_t v);
void ParseHikiData(std::span<const std::uint8_t, HIKI_ALPHA_GRID_SIZE> alpha);
void RelocateHikiModel(HikiModelNode &node, const void *image);
void InitializeHikiWaveHeights(std::span<const float, HIKI_ALPHA_GRID_SIZE> heights, std::uint32_t diffuse);
void InitializeHikiTextureCoordinates(std::int32_t repeatU, std::int32_t repeatV, HikiModelEntity &entity);
void ApplyHikiTexture(std::int32_t textureIndex, HikiModelEntity &entity);
void ScrollHikiTexture(std::int32_t textureIndex, std::int16_t u, std::int16_t v, HikiModelEntity &entity);

} // namespace shenmue::s2
