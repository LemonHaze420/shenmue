#pragma once

#include <cstdint>
#include <cstddef>
#include "common/math.hpp"

namespace shenmue::d3t {

enum ObjectSourceFlag : std::uint32_t { OSF_Unknown02 = 0x2, OSF_DedicatedShadow = 0x4, OSF_HasLightBindings = 0x200, OSF_CopyMaterials = 0x400 };
enum ObjectPlacementFlag : std::uint8_t { OPF_Sky = 0x2, OPF_SkyProbe = 0x4 };

enum ModelCacheNodeFlag : std::uint16_t { MCF_Unknown01 = 0x1, MCF_Unknown08 = 0x8, MCF_Day = 0x40, MCF_Night = 0x80 };
struct ModelCacheIndexTable;
extern std::int32_t g_hashModelRequireAssignedNodes;
extern std::int32_t g_hashModelRestrictCharacterPaths;
extern ModelCacheIndexTable g_hashModelCharacterPaths;



struct ModelRuntimeNode {
    std::uint32_t type;
    std::uint32_t reserved04;
    const void *node;
    std::int16_t kind;
    std::uint16_t reserved12;
    std::uint32_t nodeHash;
    std::uint32_t attachmentIndex;
    std::uint32_t reserved1c;
};
struct HandMatrixAttachment {
    std::uint32_t cacheHandle;
    std::uint32_t reserved04;
    const void *node;
    std::uint32_t nodeHash;
    std::uint32_t matrixCount;
    std::int32_t nodeKind;
    std::uint32_t enabled;
    std::uint32_t ready;
    std::uint32_t slot;
    std::uint32_t boneHashes[20];
    std::uint8_t matrixIndices[20];
    std::uint32_t reserved8c;
    const Matrix4x4 *matrices;
    std::uint8_t hasChildren;
    std::byte reserved99[7];
};
struct ModelRuntimeState {
    std::uint32_t nodeCount;
    char modelName[260];
    std::uint32_t nodeTableAvailable;
    std::uint32_t nodesAssigned;
    std::uint32_t cacheHandle;
    std::uint32_t rootHash;
    std::uint32_t skeletonHash;
    std::uint32_t recordCount;
    ModelRuntimeNode nodes[300];
    std::byte clothRuntime[6080];
    HandMatrixAttachment hands[2];
    std::byte cachedMatrixEventState[88];
    std::byte dynamicModelRuntime[17664];
};
static_assert(sizeof(ModelRuntimeNode) == 0x20);
static_assert(sizeof(HandMatrixAttachment) == 0xa0);
static_assert(offsetof(HandMatrixAttachment, matrixIndices) == 0x78);
static_assert(offsetof(HandMatrixAttachment, matrices) == 0x90);
static_assert(sizeof(ModelRuntimeState) == 0x84f8);
static_assert(offsetof(ModelRuntimeState, nodes) == 0x120);
static_assert(offsetof(ModelRuntimeState, hands) == 0x3e60);
ModelRuntimeState *GetModelRuntimeState(const void *placement);
std::uint64_t GetObjectModelPathHash(const void *placement);

const void *GetObjectModelTraversalSource(const void *placement);
#if SHENMUE_IS_S2

void UpdateModelExpressions(const void *placement);


void ApplyEyeLookTarget(const void *placement, std::uint32_t eye, std::uint16_t yAngle, std::int32_t zAngle, std::int16_t xAngle, const Vector3 *target);

enum class FacialExpressionLayer : std::uint32_t { Upper, Lower, Full };
void SetObjectFacialExpression(const void *placement, std::uint32_t expression, FacialExpressionLayer layer, float transitionFrames, float weight);
void SetObjectMouthExpression(const void *placement, std::uint32_t expression, float transitionFrames, float weight);
void SetObjectEyeLidExpressionWeights(const void *placement, float leftWeight, float rightWeight, float transitionFrames);
#endif
std::uint64_t ResolveModelRuntimeNode(const void *placement, const char *name, std::int16_t kind);




bool RegisterObjectSource(const void *source, const char *modelPath, std::uint16_t flags);
void ReleaseObjectSource(const void *source);
// The source's realized D3T model handle (ResolveSourceObjectEntry +0x10), 0 when it has none.
std::uint32_t GetObjectSourceRenderHandle(const void *source);

bool AuthoredAssetExists(const char *path);
std::int16_t SetObjectSourceActiveFlag(const void *source, std::int32_t active);



bool RegisterObjectPlacement(const void *source, const void *placement, std::uint8_t flags);
bool RegisterObjectAlias(const void *publicKey, const void *placement);
void RetireObjectPlacement(const void *placement);

bool ProcessObjectStore(bool checkShaders, std::int32_t maximumOperations);

// Map admission retires every placement, preserves flag-0x40 sources and clears both alias maps.
void ResetObjectStoreForMap();



bool HashModelWithAlpha(const void *placement, float alpha, std::uint32_t projectionMode);
bool HashModel(const void *placement, std::uint32_t projectionMode);


void SetHashedObjectTransform(const void *placement);
// S2 428930/428740: road placement admission writes left/right joins to
// the cached Bone_02/Bone_01 transforms before releasing draw flag 1.
bool FindHashedObject(const void *placement, const Vector3 *joins, std::uint32_t projectionMode);

#if SHENMUE_IS_S2
void RestoreObjectStoreFrameFlags();
bool IsObjectModelRuntime(const void *placement);
std::uint32_t GetShouldHashFlag();
void IncrementHashedModelCount(std::int32_t models, std::int32_t culled);
void ResetHashedModelCounts();
extern std::int32_t g_uniqueModelCount;
extern std::int32_t g_culledModelCount;
#endif
} // namespace shenmue::d3t
