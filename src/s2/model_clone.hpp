#pragma once

#include "common/math.hpp"
#include "common/types.hpp"

#include <cstddef>
#include <cstdint>
#include <vector>

namespace shenmue::s2 {

struct ModelCloneNode {
    std::int32_t id;
    Vector3 position;
    Vector3i rotation;
    Vector3 scale;
    std::int32_t meshOffset;
    std::int32_t childOffset;
    std::int32_t siblingOffset;
    std::int32_t parentOffset;
    std::int32_t transformOffset;
    std::int32_t taskDataOffset;
};
static_assert(sizeof(ModelCloneNode) == 0x40);
static_assert(offsetof(ModelCloneNode, meshOffset) == 0x28);
static_assert(offsetof(ModelCloneNode, childOffset) == 0x2c);
static_assert(offsetof(ModelCloneNode, siblingOffset) == 0x30);
static_assert(offsetof(ModelCloneNode, parentOffset) == 0x34);
static_assert(offsetof(ModelCloneNode, transformOffset) == 0x38);
static_assert(offsetof(ModelCloneNode, taskDataOffset) == 0x3c);

inline constexpr shen_name MEMORY_MODEL_CLONE{SHEN_NAME(HMDL)};
inline constexpr std::uint32_t MODEL_MDC7 = SHEN_NAME(MDC7);
inline constexpr std::uint32_t MODEL_MDH7 = SHEN_NAME(MDH7);
inline constexpr std::uint32_t MODEL_MDCX = SHEN_NAME(MDCX);
inline constexpr std::uint32_t MODEL_MDHX = SHEN_NAME(MDHX);
inline constexpr std::uint32_t MODEL_MDP7 = SHEN_NAME(MDP7);
inline constexpr std::uint32_t MODEL_MDPX = SHEN_NAME(MDPX);
inline constexpr std::uint32_t MODEL_WBTR = SHEN_NAME(WBTR);
inline constexpr std::uint32_t MODEL_XB01 = SHEN_NAME(XB01);

extern std::uint32_t g_modelCloneNodeCount;
extern std::uint32_t g_modelCloneMeshCount;
extern std::uint32_t g_modelCloneNonXb01Count;
extern std::uint32_t g_modelCloneMdcBytes;
extern std::uint32_t g_modelCloneMdpBytes;

std::uint32_t GetModelMdcCloneBlockSize(const ModelCloneNode *node);
std::int64_t CountModelNodeTreeForClone(const ModelCloneNode *node);
std::int64_t AccumulateModelMdcCloneBytes(const ModelCloneNode *node);
std::int64_t AccumulateModelMdpCloneBytes(const ModelCloneNode *node);
std::uint64_t MeasureModelNodeTreeClone(const ModelCloneNode *node);
std::byte *CopyModelNodeTree(ModelCloneNode *parent, const ModelCloneNode *source, std::byte *destination);
ModelCloneNode *CloneModelNodeTree(const void *sourceKey, const ModelCloneNode *node, std::uint8_t flags);

struct MT7Model;
struct MT7Node;
// Open while FreeModelComponents runs its cleanups; a clone released in that pass parks its node storage here.
extern std::vector<std::vector<MT7Node>> *g_componentTeardownNodes;
MT7Model *CloneMT7ModelNodeTree(const void *sourceKey, const MT7Model &model, std::uint8_t flags);
MT7Model *CloneMT7ModelBranch(const void *sourceKey, const MT7Model &model, std::uint32_t rootIndex, std::uint8_t flags);
void ReleaseMT7ModelNodeTreeClone(MT7Model *model);

} // namespace shenmue::s2
