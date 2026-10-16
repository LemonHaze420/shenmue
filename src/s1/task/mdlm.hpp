#pragma once

#include "s1/hlib.hpp"
#include "common/types.hpp"
#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
struct PackModel;
struct PackSlot;

struct MdlmImageEntry {
    PackModel *model;
    char *imageName;
    std::int32_t selector;
    std::uint32_t flags;
};

struct MdlmRecord {
    MdlmRecord *next;
    char *name;
    std::int32_t imageCount;
    std::uint32_t reserved14;
    MdlmImageEntry *images;
    std::int32_t referenceCount;
    std::uint32_t reserved24;
    PackSlot *ownerPack;
};

struct MdlmImageSlot {
    std::uint32_t flags;
    std::int32_t selectionMetric;
    MdlmRecord *record;
    void *activeModel;
    PackModel *selectedModel;
    std::int32_t childIndex;
    std::int32_t nodeKind;
    std::int32_t occurrence;
    std::uint32_t reserved2c;
};

struct MdlmTaskData {
    std::uint32_t tag;
    std::uint32_t field04;
    char stringPool[0xc78];
    std::uint32_t stringBytes;
    std::uint32_t reservedC84;
    MdlmRecord *buckets[19];
    MdlmImageSlot imageSlots[256];
};

static_assert(sizeof(MdlmImageEntry) == 0x18);
static_assert(sizeof(MdlmRecord) == 0x30);
static_assert(offsetof(MdlmRecord, ownerPack) == 0x28);
static_assert(sizeof(MdlmImageSlot) == 0x30);
static_assert(offsetof(MdlmTaskData, stringBytes) == 0xc80);
static_assert(offsetof(MdlmTaskData, buckets) == 0xc88);
static_assert(offsetof(MdlmTaskData, imageSlots) == 0xd20);
static_assert(sizeof(MdlmTaskData) == 0x3d20);

extern HLib::Task *g_mdlmTask;

HLib::Task *MDLM_Init();
void MDLM_Callback(HLib::TaskData *taskData);
MdlmTaskData *MDLM_GetTaskData(HLib::Task *task = nullptr);

MdlmRecord *MDLM_StoreCharacterName(HLib::Task *owner, const char *name);
MdlmRecord *MDLM_DefineImages(const char *name, std::int32_t imageCount, PackSlot *ownerPack);
MdlmImageEntry *MDLM_InsertImageDefinition(MdlmRecord *record, std::int32_t populatedCount, const char *imageName, std::int32_t selector);
void MDLM_PublishImage(MdlmImageEntry *entry, PackModel *model, bool ownsStandaloneModel);
const char *MDLM_FindModelImageName(PackModel *model);
MdlmImageSlot *MDLM_AcquireImageSlot(HLib::Task *owner, const char *name);
void MDLM_ReleaseImageSlot(HLib::Task *owner, MdlmImageSlot *slot);
void MDLM_UpdateImageDistance(MdlmImageSlot *slot, Vector3 *position);
void *MDLM_CreateAssignedImage(MdlmImageSlot *slot);

bool MDLM_RegisterImage(
    const char *chidName, const char *imageName, PackModel *model, std::int32_t selector = 0, PackSlot *ownerPack = nullptr, bool ownsStandaloneModel = false);
PackModel *MDLM_FindImage(const char *chidName, std::int32_t selectionMetric = 0, PackSlot *scope = nullptr);
PackModel *MDLM_FindImage(shen_name chidName, std::int32_t selectionMetric = 0, PackSlot *scope = nullptr);
bool MDLM_ActivateCharacterImage(Character *character, const char *chidName, std::uint32_t imageCount = 0);
bool MDLM_ActivateCharacterImage(Character *character, shen_name chidName, std::uint32_t imageCount = 0);
bool MDLM_ReplaceCharacterImage(
    Character *character, const char *chidName, PackModel *replacement, bool ownsStandaloneModel = false, PackSlot *ownerPack = nullptr);
bool MDLM_ReplaceCharacterImage(
    Character *character, shen_name chidName, PackModel *replacement, bool ownsStandaloneModel = false, PackSlot *ownerPack = nullptr);
bool MDLM_UnloadImage(const char *chidName);
bool MDLM_UnloadImage(shen_name chidName);
void MDLM_UnloadPackImages(PackSlot *ownerPack);

} // namespace shenmue::s1
