#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/math.hpp"
#include "common/types.hpp"
#include "platform/dc/motion_file.hpp"

#define SHENMUE_MOTION_REGISTRY_LAYOUT_NAME_(line) SHENMUE_MOTION_REGISTRYLayoutCheck##line
#define SHENMUE_MOTION_REGISTRY_LAYOUT_NAME(line) SHENMUE_MOTION_REGISTRY_LAYOUT_NAME_(line)
#define SHENMUE_MOTION_REGISTRY_LAYOUT(condition) typedef char SHENMUE_MOTION_REGISTRY_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_MOTION_REGISTRY_CONSTANT static const
#define SHENMUE_MOTION_REGISTRY_INITIALIZER =
namespace shenmue {
namespace s2 {

typedef s1::BaseMotionFile MotionRegistryEntry;
typedef MotionRegistryEntry MotnFileNode;
struct MotnAnimationHeader;

struct MotionBankHeader {
    std::uint32_t sequenceTableOffset;
    std::uint32_t sequenceNameOffset;
    std::uint32_t sequenceDataOffset;
    std::uint16_t endAnimation;
    std::uint16_t unknown0e;
    std::uint32_t fileSize;
    std::uint16_t firstAnimation;
    std::uint16_t reserved16;
    std::uint16_t registeredEndAnimation;
    std::uint16_t reserved1a;
    std::uint32_t enabled;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionBankHeader, endAnimation) == 0x0c);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionBankHeader, fileSize) == 0x10);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionBankHeader, firstAnimation) == 0x14);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionBankHeader, registeredEndAnimation) == 0x18);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionBankHeader, enabled) == 0x1c);
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionBankHeader) == 0x20);
struct RetiredMotionRange {
    std::uint16_t endAnimation, firstAnimation;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(RetiredMotionRange) == 4);
extern RetiredMotionRange g_retiredMotionRanges[32];
std::uint32_t RegisterMotionBank(void *data);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t ReleaseMotionBank(std::uint32_t handle);
#else
void ReleaseMotionBank(std::uint32_t handle);
#endif
bool IsMotionAnimationValid(std::uint16_t animation);
std::int32_t GetMotionFrameCount(std::uint16_t animation);
std::uint16_t MOTN_GetLoopFrame(std::uint16_t animation);
void ClearRetiredMotionRanges();

struct MotionPropEntry {
    std::uint32_t id;
    std::uintptr_t model;
};
struct MotionPropHeader {
    std::int32_t count;
    std::uint32_t reserved04;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionPropEntry) == 8);
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionPropHeader) == 8);
struct MT7Node;
#pragma pack(push, 2)
struct MotionPropDescriptor {
    std::uint16_t prop, bone;
    std::uint16_t retained04[2];
    std::uint16_t position[3], rotation[3], windowCount;
};
#pragma pack(pop)
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionPropDescriptor) == 0x16);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionPropDescriptor, windowCount) == 0x14);
MT7Node *FindMotionProp(std::uint16_t id, std::int32_t handle);

std::uint32_t RegisterMotionPropBank(void *data);
std::int32_t ReleaseMotionPropBank(std::uint32_t handle);
struct MotionPropBank {
    std::int32_t handle;
    std::byte *data;
    MotionPropBank *previous;
    MotionPropBank *next;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionPropBank) == 0x10);
#if defined(SHENMUE_TARGET_S2_DC_JP_1001)
SHENMUE_MOTION_REGISTRY_CONSTANT std::size_t kMotionPropBankCount = 6;
#else
SHENMUE_MOTION_REGISTRY_CONSTANT std::size_t kMotionPropBankCount = 4;
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint32_t *AllocateMotionFileData(std::uint32_t size);
std::int32_t MOTN_GetFootstepAttributes(std::uint16_t animationId, std::uint8_t *attributes);
void ResetMotionBankEntry(MotionRegistryEntry *entry);
std::intptr_t FreeMotionFileData(void *data);
std::uint32_t ReadMoti(const char *directory, const char *filename);
std::int32_t ReadMotiFromDirectory(std::int32_t act, shen_name map, const char *filename);
std::int32_t ReadMotionPropsFromDirectory(std::int32_t act, shen_name map, const char *filename);
std::uint32_t DeleteMotion(std::int32_t handle);
std::uint32_t DeleteMotionProps(std::int32_t handle);
#endif

struct MotionSnapshot {
    std::int32_t slot;
    shen_name character;
    std::uint8_t requestedRootMode;
    std::byte retained09[3];
    std::uint16_t motionIds[5];
    std::uint16_t defaultStepFrames[5];
    std::uint16_t regionFlags[5];
    std::byte retained2a[2];
    std::uint32_t motionFlags;
    std::uint32_t sequenceFlags;
    std::uint32_t retainedMotionParameter;
    Vector3i entityRotation;
    Vector3i worldRotation;
    Vector3 rootPosition;
    Vector3 worldPosition;
    float splitFrames[5];
    float ticksTime[5];
    float maxTime[5];
    float playbackSpeed[5];
    float constraintStepLimit[9];
    float retainedCollisionValues[3];
    std::uint32_t retainedExecutionWords[4];
    std::uint32_t executionFlags;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(sizeof(MotionSnapshot) == 0xfc);
struct MotionSnapshotAlignment {
    char preceding;
    MotionSnapshot snapshot;
};
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshotAlignment, snapshot) == 4);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, requestedRootMode) == 8);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, motionIds) == 0x0c);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, defaultStepFrames) == 0x16);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, regionFlags) == 0x20);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, motionFlags) == 0x2c);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, sequenceFlags) == 0x30);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, retainedMotionParameter) == 0x34);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, entityRotation) == 0x38);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, worldRotation) == 0x44);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, rootPosition) == 0x50);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, worldPosition) == 0x5c);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, splitFrames) == 0x68);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, ticksTime) == 0x7c);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, maxTime) == 0x90);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, playbackSpeed) == 0xa4);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, constraintStepLimit) == 0xb8);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, retainedCollisionValues) == 0xdc);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, retainedExecutionWords) == 0xe8);
SHENMUE_MOTION_REGISTRY_LAYOUT(offsetof(MotionSnapshot, executionFlags) == 0xf8);

#define MOTION_FILE_STORAGE (::shenmue::shen_name(SHEN_NAME4('M', 'O', 'T', 'I')))
#define FALLBACK_MOTION_STORAGE (::shenmue::shen_name(SHEN_NAME4('F', 'B', 'M', 'T')))
#define MOTION_SNAPSHOT_STORAGE (::shenmue::shen_name(SHEN_NAME4('U', 'N', 'F', 'O')))
#define EMPTY_MOTION_SNAPSHOT_CHARACTER (::shenmue::shen_name(SHEN_NAME4('_', '_', '_', '_')))
SHENMUE_MOTION_REGISTRY_CONSTANT std::size_t kMotionSnapshotSlots = 16;
SHENMUE_MOTION_REGISTRY_CONSTANT std::size_t kMotionSnapshotBytes = kMotionSnapshotSlots * sizeof(MotionSnapshot);

extern MotionRegistryEntry g_motionRegistry[32];
extern MotionRegistryEntry *g_motionData;
extern MotionRegistryEntry *g_battleMotionData;
extern MotionPropBank g_motionPropBanks[kMotionPropBankCount];
extern std::int8_t g_motionBankCount;
extern std::uint32_t g_registeredMotionBytes;
extern std::byte *g_fallbackMotionTable;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern std::int32_t *g_fallbackMotionSection0, *g_fallbackMotionSection1, *g_fallbackMotionSection2, *g_fallbackMotionSection3;
#else
extern std::int32_t *g_fallbackMotionSections[4];
#endif
extern MotionSnapshot *g_motionSnapshots;
extern std::uint32_t g_motionSnapshotState;

MotnFileNode *MOTN_FindFileForAnimId(std::uint16_t animationId);
const MotnAnimationHeader *MOTN_GetDataForAnimId(std::uint16_t animationId);
const std::byte *MOTN_GetMarkerData(std::uint16_t animationId);
const std::byte *MOTN_FindAnimationExtraRecord(std::uint16_t animationId, std::uint32_t tag);
std::uint32_t MOTN_GetLeadFootFlag(std::uint16_t animationId);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern const std::uint8_t motionCommandWidths[];
#else
SHENMUE_MOTION_REGISTRY_CONSTANT std::uint8_t motionCommandWidths[] SHENMUE_MOTION_REGISTRY_INITIALIZER{12, 4, 8, 16, 20, 12, 8, 4, 0, 16, 8, 12, 4, 4, 16, 8,
    4, 8, 4, 4, 4, 4, 4, 4, 4, 4, 8, 8, 4, 8, 8, 8, 0, 4, 4, 16, 8, 8, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 4, 8, 8, 8, 4, 8, 8, 8, 16, 12, 28, 4, 8, 8, 4, 8, 8, 8,
    8, 8, 12, 8, 12, 8, 8, 4, 8, 8, 4, 4, 4, 4, 16, 8, 12, 8, 4, 8, 4, 2, 0};
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern const std::uint8_t motionFrameCommandWidths[];
const std::byte *MOTN_FindFrameEvent(std::int8_t eventType, std::uint16_t animationId, float firstFrame, float lastFrame, std::int32_t occurrence);
#endif
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AllocateAndLoadMOTFiles();
#else
std::byte *AllocateAndLoadMOTFiles();
#endif
void AllocateAndSetUNFOFlag();
std::uint32_t InitializeUNFO();
void AllocateMotionBanks(std::int32_t actNumber, std::uint32_t map);
void BindFallbackMotionTable(std::byte *data);
std::byte *GetFallbackMotionTable();

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MOTION_REGISTRY_LAYOUT_NAME_
#undef SHENMUE_MOTION_REGISTRY_LAYOUT_NAME
#undef SHENMUE_MOTION_REGISTRY_LAYOUT
#undef SHENMUE_MOTION_REGISTRY_CONSTANT
#undef SHENMUE_MOTION_REGISTRY_INITIALIZER

#endif
