#pragma once

#include "d3t/model.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct ModelCacheIndexTable {
    std::uint32_t *values;
    std::uint32_t *hashes;
    std::uint16_t *links;
    std::uint32_t capacity;
    std::uint32_t count;
};
struct ModelCacheSkinRange {
    std::uint32_t nodeHash;
    std::uint32_t firstBone;
    std::uint32_t boneCount;
};
struct ModelCacheExpressionBoneTable {
    std::byte reserved00[704];
    void *affectedBones;
    std::uint32_t affectedBoneCount;
    std::uint32_t affectedBoneCapacity;
};
struct ModelCacheExpressionRecord {
    std::uint32_t modelHandle;
    std::uint32_t nodeHash;
    ModelCacheExpressionBoneTable *boneTables;
    std::uint32_t boneTableCount;
    std::uint32_t boneTableCapacity;
    std::uint8_t expressionSlotCount;
    std::byte reserved19[3];
    std::uint32_t expressionSlots[33];
};
struct ModelCacheExpressionTarget {
    ModelCacheExpressionRecord *records;
    std::uint32_t count;
    std::uint32_t capacity;
    char name[64];
};
#pragma pack(push, 1)
struct ModelExpressionDefinition {
    std::uint32_t type;
    char name[64];
    std::uint16_t animationIndex;
    std::uint16_t firstFrame, lastFrame;
    char linkedA[64];
    char linkedB[64];
    std::uint8_t framesPerSecond, playbackFlags;
    float transitionDuration;
    float blendOutDuration;
};
#pragma pack(pop)
struct ModelExpressionRuntime;
struct ModelExpressionSubmission {
    const char *name;
    std::uint32_t flags, elementMask;
    float weight;
    std::uint32_t reserved14;
    float (*weightCallback)(ModelExpressionRuntime *);
    float transitionDuration, blendOutDuration;
    float playbackDuration;
    std::uint32_t reserved2c;
};
struct ModelExpressionTransform {
    Vector3 translation;
    Vector4 rotation;
    Vector3 scale;
};
struct ModelExpressionNodeRuntime {
    ModelExpressionTransform keys[4];
    std::int32_t keyFrames[4];
    ModelExpressionTransform sampled;
};
struct ModelExpressionBank {
    float weight;
    std::int32_t selectedIndex;
    std::uint64_t keyOffset;
    ModelExpressionNodeRuntime nodes[350];
};
struct ModelExpressionRuntime {
    const ModelExpressionDefinition *definition;
    const ModelExpressionDefinition *linkedA;
    const ModelExpressionDefinition *linkedB;
    float currentWeight, blendInElapsed, blendOutElapsed;
    std::uint32_t state;
    std::byte reserved28[8];
    ModelExpressionBank banks[2];
    ModelExpressionSubmission submission;
    std::uint32_t affectedNodeBits[11], reserved24f4c;
};
struct ModelExpressionDecodeTask {
    ModelCacheExpressionRecord *record;
    ModelExpressionRuntime *expression;
    const ModelExpressionDefinition *definition;
    ModelExpressionBank *bank;
    float elapsed;
    std::uint32_t reserved24;
};
struct ModelExpressionApplyTask {
    std::uint32_t cacheHandle, reserved04;
    ModelCacheExpressionRecord *record;
    float elapsed;
    std::uint32_t reserved14;
};
struct ModelExpressionMatrixContribution {
    std::uint16_t node, count;
    float weight;
    Matrix4x4 matrix;
};
static_assert(sizeof(ModelExpressionTransform) == 40);
static_assert(sizeof(ModelExpressionNodeRuntime) == 216);
static_assert(offsetof(ModelExpressionNodeRuntime, keyFrames) == 160);
static_assert(offsetof(ModelExpressionNodeRuntime, sampled) == 176);
static_assert(sizeof(ModelExpressionDecodeTask) == 40);
static_assert(sizeof(ModelExpressionApplyTask) == 24);
static_assert(sizeof(ModelExpressionMatrixContribution) == 72);
static_assert(sizeof(ModelExpressionDefinition) == 0xd4);
static_assert(offsetof(ModelExpressionDefinition, linkedA) == 0x4a);
static_assert(offsetof(ModelExpressionDefinition, animationIndex) == 0x44);
static_assert(offsetof(ModelExpressionDefinition, firstFrame) == 0x46);
static_assert(offsetof(ModelExpressionDefinition, lastFrame) == 0x48);
static_assert(offsetof(ModelExpressionDefinition, linkedB) == 0x8a);
static_assert(offsetof(ModelExpressionDefinition, framesPerSecond) == 0xca);
static_assert(offsetof(ModelExpressionDefinition, playbackFlags) == 0xcb);
static_assert(offsetof(ModelExpressionDefinition, transitionDuration) == 0xcc);
static_assert(offsetof(ModelExpressionDefinition, blendOutDuration) == 0xd0);
static_assert(sizeof(ModelExpressionSubmission) == 0x30);
static_assert(offsetof(ModelExpressionSubmission, weightCallback) == 0x18);
static_assert(offsetof(ModelExpressionSubmission, playbackDuration) == 0x28);
static_assert(sizeof(ModelExpressionBank) == 75616);
static_assert(offsetof(ModelExpressionBank, nodes) == 0x10);
static_assert(sizeof(ModelExpressionRuntime) == 151376);
static_assert(offsetof(ModelExpressionRuntime, banks) == 0x30);
static_assert(offsetof(ModelExpressionRuntime, submission) == 0x24ef0);
static_assert(offsetof(ModelExpressionRuntime, affectedNodeBits) == 0x24f20);
struct ModelCacheSlot {
    D3TModelNode *nodes;
    Matrix4x4 *matrices;
    Matrix4x4 *worldMatrices;
    std::uint32_t *constantBuffers;
    std::uint32_t *modelHandles;
    std::uint32_t *nodeHashes;
    float *alpha;
    ModelCacheIndexTable indexTable;
    char *names;
    std::uint32_t namesSize;
    std::uint32_t namesCapacity;
    std::uint32_t nodeCapacity;
    std::uint32_t nodeCount;
    std::uint32_t nextNodeHash;
    std::uint32_t reserved74;
    ModelCacheSkinRange *skinRanges;
    std::uint32_t skinRangeCount;
    std::uint32_t skinRangeCapacity;
    std::uint32_t *boneIndices;
    std::uint32_t boneIndexCount;
    std::uint32_t boneIndexCapacity;
    D3TModelSprite *sprites;
    std::uint32_t spriteCount;
    std::uint32_t spriteCapacity;
    D3TModelProbe *probes;
    std::uint32_t probeCount;
    std::uint32_t probeCapacity;
    ModelCacheExpressionTarget expressions;
    std::uint32_t skinningBuffer;
    std::uint32_t publishedBoneCount;
};
struct ModelCacheTable {
    ModelCacheSlot slots[128];
    std::uint32_t generations[128];
    std::uint32_t cursor;
    std::uint32_t reserved8a04;
};
inline constexpr Matrix4x4 kModelCacheIdentity{{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
static_assert(sizeof(ModelCacheIndexTable) == 0x20);
static_assert(offsetof(ModelCacheIndexTable, count) == 0x1c);
static_assert(sizeof(ModelCacheExpressionBoneTable) == 720);
static_assert(offsetof(ModelCacheExpressionBoneTable, affectedBones) == 704);
static_assert(sizeof(ModelCacheExpressionRecord) == 0xa0);
static_assert(offsetof(ModelCacheExpressionRecord, boneTables) == 8);
static_assert(sizeof(ModelCacheExpressionTarget) == 0x50);
static_assert(offsetof(ModelCacheSlot, indexTable) == 0x38);
static_assert(offsetof(ModelCacheSlot, nodeCount) == 0x6c);
static_assert(offsetof(ModelCacheSlot, skinRanges) == 0x78);
static_assert(offsetof(ModelCacheSlot, sprites) == 0x98);
static_assert(offsetof(ModelCacheSlot, probes) == 0xa8);
static_assert(offsetof(ModelCacheSlot, expressions) == 0xb8);
static_assert(offsetof(ModelCacheSlot, skinningBuffer) == 0x108);
static_assert(sizeof(ModelCacheSlot) == 0x110);
static_assert(offsetof(ModelCacheTable, generations) == 0x8800);
static_assert(offsetof(ModelCacheTable, cursor) == 0x8a00);
static_assert(sizeof(ModelCacheTable) == 0x8a08);


extern ModelCacheTable g_modelCache;
extern std::uint32_t g_modelExpressionCacheSlotTags[128];
extern ModelExpressionRuntime g_modelExpressionRuntime[128];
extern std::uint32_t g_nextModelExpressionSlot;
ModelExpressionRuntime *CheckModelExpressionHandle(std::uint32_t handle);
std::uint32_t FindExpressionElementMask(std::uint64_t target, const char *name);
bool QueueModelExpression(std::uint64_t target, const ModelExpressionSubmission &submission);
#if SHENMUE_IS_S2
bool InterpolateExpressionNodeChannel(ModelExpressionNodeRuntime &node, std::int32_t frame, std::int32_t keyFrame, const void *key);
void EvaluateExpressionChannel(const ModelExpressionDefinition &definition, ModelExpressionNodeRuntime &node, std::int32_t frame);
Vector3 EvaluateCatmullRomVector3(const Vector3 (&keys)[4], float fraction);
Vector4 ApplyExpressionNodeResult(const Vector4 (&keys)[4], float fraction);
void InterpolateExpressionRotation(ModelExpressionNodeRuntime &node, float fraction);
void SampleExpressionAnimationNode(ModelExpressionNodeRuntime &node, float frame);
std::int32_t DecodeExpressionAnimationNode(void *data);
void UpdateModelExpressionSlot(ModelCacheExpressionRecord &record, ModelExpressionRuntime &expression, float elapsed, std::uint32_t *&tasks);
void ApplyModelExpressionsToMatrices(std::uint32_t cacheHandle, ModelCacheExpressionRecord &record);
std::int32_t ModelExpressionNodeTaskCallback(void *data);
void UpdateModelExpressionAnimations(std::uint32_t cacheHandle, ModelCacheExpressionTarget &target, float elapsed);
void TexSortWrapper(float elapsed);
#endif
ModelCacheSlot *ResolveModelCache(std::uint32_t handle);
std::uint32_t AllocateModelCache(const char *name);
void ReleaseModelCache(std::uint32_t handle);
std::uint64_t AppendModelToCache(std::uint32_t cacheHandle, std::uint32_t modelHandle);
std::uint32_t LookupModelCacheIndex(const ModelCacheIndexTable &table, std::uint32_t nodeHash);
std::uint64_t GetModelCacheParent(std::uint64_t handle);
std::uint64_t GetModelCacheChild(std::uint64_t handle, std::uint32_t child);
void SetModelCacheTransform(std::uint64_t handle, const Matrix4x4 &transform);
Matrix4x4 GetModelCacheTransform(std::uint64_t handle);
Matrix4x4 ComputeModelCacheGlobalTransform(std::uint64_t handle);
void SetModelCacheAlpha(std::uint64_t handle, float alpha);
enum class ModelCacheFlagMode : std::uint32_t { Enable, Clear, Toggle, Set };
void UpdateModelCacheFlags(std::uint64_t handle, std::uint16_t flags, ModelCacheFlagMode mode);
const char *GetModelCacheNodeName(std::uint64_t handle);
std::uint64_t FindModelCacheNode(std::uint64_t handle, const char *name);
std::uint64_t FindModelCacheNodeContaining(std::uint64_t handle, const char *text);
void RemoveModelCacheSubtree(std::uint64_t handle);
void RetireModelCacheNodeExpressions(std::uint64_t handle);

} // namespace shenmue::d3t
