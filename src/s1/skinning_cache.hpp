#pragma once

#include "common/math.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct MurmurValueTable {
    std::uint32_t *values;
    std::uint32_t *hashes;
    std::uint16_t *probes;
    std::uint32_t capacity;
};

#pragma pack(push, 4)
struct CachedSkinningBufferSlot {
    std::byte *entries;
    Matrix4x4 *matrices;
    Matrix4x4 *secondaryMatrices;
    std::uint32_t *constantBufferHandles;
    std::uint32_t *renderHandles;
    std::uint32_t *parentHashes;
    float *floatValues;
    MurmurValueTable valueTable;
    char *names;
    std::uint32_t namesSize;
    std::uint32_t namesCapacity;
    std::uint32_t entryCapacity;
    std::uint32_t entryCount;
    std::uint32_t nextNodeHash;
    std::uint32_t reserved_74;
    std::uint32_t *skinRanges;
    std::uint32_t skinRangeCount;
    std::uint32_t skinRangeCapacity;
    std::uint32_t *boneIndices;
    std::uint32_t boneIndexCount;
    std::uint32_t boneIndexCapacity;
    std::byte *sprites;
    std::uint32_t spriteCount;
    std::uint32_t spriteCapacity;
    std::byte *probes;
    std::uint32_t probeCount;
    std::uint32_t probeCapacity;
    std::byte animationBatchAndName[0x50];
    std::uint32_t skinningBuffer;
    std::uint32_t publishedBoneCount;
};
#pragma pack(pop)



struct HeadRigState {
    std::uint32_t active;
    std::uint32_t initialized;
    std::uint32_t cacheHandle;
    std::uint32_t sourceHash;
    std::uint32_t targetHash;
    Matrix4x4 localTransform;
};

static_assert(sizeof(HeadRigState) == 0x54);
static_assert(offsetof(HeadRigState, localTransform) == 0x14);

struct alignas(16) SkinningCacheStorage {
    std::array<std::array<std::byte, 0x110>, 128> entries;
    std::array<std::uint32_t, 128> generations;
};

struct SkinningTexSortBatch {
    std::byte *records;
    std::uint32_t recordCount;
    std::uint32_t capacity;
};

struct SkinningTexSortRecord {
    std::uint32_t renderHandle;
    std::uint32_t baseBoneHash;
    std::byte *boneTables;
    std::uint32_t boneTableCount;
    std::uint32_t boneTableCapacity;
    std::uint8_t cacheHandleCount;
    std::byte reserved_19[3];
    std::uint32_t cacheHandles[32];
    std::byte reserved_9c[4];
};

struct SkinningKeyframe {
    Vector3 translation;
    Vector4 rotation;
    Vector3 scale;
};

struct SkinningTrackState {
    SkinningKeyframe keyframes[4];
    std::int32_t frames[4];
    Vector3 translation;
    Vector4 rotation;
    Vector3 scale;
};

struct FacialExpressionRequest {
    const char *name;
    std::uint8_t flags;
    std::byte reserved_09[3];
    std::uint32_t targetMask;
    float blend;
    std::byte reserved_14[0x0c];
    float fadeSeconds;
    std::byte reserved_24[0x0c];
};

constexpr std::size_t kSkinningWorkCacheEntrySize = 0x24f50;

struct alignas(16) SkinningWorkCacheStorage {
    std::array<std::array<std::byte, kSkinningWorkCacheEntrySize>, 128> entries;
    std::array<std::uint32_t, 128> generations;
};

static_assert(offsetof(SkinningCacheStorage, generations) == 0x8800);
static_assert(sizeof(SkinningCacheStorage) == 0x8a00);
static_assert(offsetof(MurmurValueTable, capacity) == 0x18);
static_assert(sizeof(MurmurValueTable) == 0x20);
static_assert(offsetof(CachedSkinningBufferSlot, matrices) == 0x08);
static_assert(offsetof(CachedSkinningBufferSlot, constantBufferHandles) == 0x18);
static_assert(offsetof(CachedSkinningBufferSlot, renderHandles) == 0x20);
static_assert(offsetof(CachedSkinningBufferSlot, parentHashes) == 0x28);
static_assert(offsetof(CachedSkinningBufferSlot, floatValues) == 0x30);
static_assert(offsetof(CachedSkinningBufferSlot, valueTable) == 0x38);
static_assert(offsetof(CachedSkinningBufferSlot, names) == 0x58);
static_assert(offsetof(CachedSkinningBufferSlot, entryCount) == 0x6c);
static_assert(offsetof(CachedSkinningBufferSlot, skinRanges) == 0x78);
static_assert(offsetof(CachedSkinningBufferSlot, boneIndices) == 0x88);
static_assert(offsetof(CachedSkinningBufferSlot, sprites) == 0x98);
static_assert(offsetof(CachedSkinningBufferSlot, probes) == 0xa8);
static_assert(offsetof(CachedSkinningBufferSlot, skinningBuffer) == 0x108);
static_assert(offsetof(CachedSkinningBufferSlot, publishedBoneCount) == 0x10c);
static_assert(sizeof(CachedSkinningBufferSlot) == 0x110);
static_assert(offsetof(SkinningTexSortBatch, recordCount) == 8);
static_assert(sizeof(SkinningTexSortBatch) == 0x10);
static_assert(offsetof(SkinningTexSortRecord, cacheHandleCount) == 0x18);
static_assert(offsetof(SkinningTexSortRecord, cacheHandles) == 0x1c);
static_assert(sizeof(SkinningTexSortRecord) == 0xa0);
static_assert(sizeof(SkinningKeyframe) == 0x28);
static_assert(offsetof(SkinningTrackState, frames) == 0xa0);
static_assert(offsetof(SkinningTrackState, translation) == 0xb0);
static_assert(offsetof(SkinningTrackState, rotation) == 0xbc);
static_assert(offsetof(SkinningTrackState, scale) == 0xcc);
static_assert(sizeof(SkinningTrackState) == 0xd8);
static_assert(offsetof(FacialExpressionRequest, targetMask) == 0x0c);
static_assert(offsetof(FacialExpressionRequest, fadeSeconds) == 0x20);
static_assert(sizeof(FacialExpressionRequest) == 0x30);
static_assert(offsetof(SkinningWorkCacheStorage, generations) == 0x127a800);
static_assert(sizeof(SkinningWorkCacheStorage) == 0x127aa00);

extern SkinningCacheStorage g_CachedSkinningBuffers;
extern SkinningWorkCacheStorage g_skinningWorkCache;
extern std::uint32_t g_skinningWorkCacheCounter;
extern std::uint64_t g_mainSkinningRoot;
extern std::uint64_t g_sdOverrideSkinningRoot;
extern std::uint64_t g_skySkinningRoot;
extern std::uint64_t g_skyProbeSkinningRoot;

std::uint64_t AllocateCachedSkinningBuffer(const char *name);
std::uint64_t AppendModelToCachedSkinningBuffer(std::uint32_t cacheHandle, std::uint32_t modelHandle);
const char *GetSkinningNodeName(std::uint64_t handle);
std::uint32_t GetSkinningNodeChildCount(std::uint64_t handle);
std::uint64_t GetSkinningNodeChild(std::uint64_t handle, std::uint32_t child);
void ReleaseCachedSkinningBuffer(std::uint64_t handle);
std::uint64_t AllocateSkinningNode(std::uint64_t parentHandle, const char *nodeName, std::uint32_t renderHandle = 0, bool modelOwnedName = false);
bool RemoveSkinningNode(std::uint64_t handle);

void InitializeSkinningWorkCache();
CachedSkinningBufferSlot *ResolveCachedSkinningBuffer(std::uint32_t handle);
std::byte *ResolveSkinningWorkCache(std::uint32_t handle);
std::uint64_t AssignNode(std::uint64_t skinningHash, const char *nodeName);

bool UpdateSkinningTrackHistory(SkinningTrackState &state, std::int32_t requestedFrame, std::int32_t keyframeFrame, const SkinningKeyframe &keyframe);
void ClampSkinningTrackHistory(const std::byte *animation, SkinningTrackState &state, std::int32_t requestedFrame);
void InterpolateSkinningTrack(SkinningTrackState &state, float frame);
std::int32_t TexSort2(void *data);
std::int32_t TexSort(void *data);
void BuildSkinningMatrices(std::uint32_t skinningHandle, SkinningTexSortRecord *record);
void ReleaseSkinningTexSortRecord(SkinningTexSortRecord *record);
SkinningTexSortRecord *AppendSkinningTexSortRecord(SkinningTexSortBatch *batch);
SkinningTexSortRecord *BuildSkinningTexSortRecord(std::uint32_t renderHandle, SkinningTexSortBatch *batch, std::uint64_t skinningHash);
SkinningTexSortRecord *LookupOrBuildSkinningTexSortRecord(std::uint64_t skinningHash);
std::uint32_t FindFacialTargetMask(std::uint64_t skinningHash, const char *name);
bool StartFacialExpressionWork(std::uint64_t skinningHash, const FacialExpressionRequest *request);
void DoTexSort2(std::uint32_t *renderRecord, std::byte *workCache, float frameTime, std::uint32_t **taskCursor);

void ProcessSkinningCacheTexSort(std::uint32_t handle, void *data, float frameTime);
void TexSortWrapper(float frameTime);
std::uint32_t GetValueFromMurmurHash(const void *table, std::uint32_t key);
bool AppendHashForSkinningBuffer(std::uint64_t handle, const Matrix4x4 &transform);
bool LookupSkinningTransform(std::uint64_t handle, Matrix4x4 &transform);
bool PublishOsageSkinningTransform(std::uint64_t handle, const Vector3 &position, const Vector3 *directionTarget);
std::uint64_t GetHashForSkinningBuffer(std::uint64_t handle);
Matrix4x4 ComputeGlobalSkinningTransform(std::uint64_t handle);
void HeadRig_Publish(const HeadRigState *head);
void ModifyArrayElementsByMurmurHash(std::uint64_t handle, std::uint16_t mask, std::int32_t operation);
std::uint64_t SetSkinningNodeDrawFlag(std::uint64_t handle, std::int32_t operation);
bool ConvertSkinningNodeToParentSpace(std::uint64_t handle, Matrix4x4 &published);
std::uint32_t GetSkinningNodeDescendantCount(std::uint64_t handle);
void HeadRig_Init(HeadRigState *head, std::uint32_t entryNodeHash);
std::uint32_t GetSkinningNodeRenderHandle(std::uint64_t handle);
std::uint64_t FindModelNodeByName(std::uint32_t modelHandle, const char *name);
std::uint64_t AllocateSkinningNodeFromRenderHandle(std::uint32_t cacheHandle, std::uint64_t modelNode, std::uint64_t parentHandle);

} // namespace shenmue::s1
