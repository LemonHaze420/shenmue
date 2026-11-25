#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"
#include "s2/model_dc.hpp"
#include <cstddef>
#include <cstdint>

#define SHENMUE_MODEL_IMAGE_LAYOUT_NAME_(line) SHENMUE_MODEL_IMAGELayoutCheck##line
#define SHENMUE_MODEL_IMAGE_LAYOUT_NAME(line) SHENMUE_MODEL_IMAGE_LAYOUT_NAME_(line)
#define SHENMUE_MODEL_IMAGE_LAYOUT(condition) typedef char SHENMUE_MODEL_IMAGE_LAYOUT_NAME(__LINE__)[(condition) ? 1 : -1]
#define SHENMUE_MODEL_IMAGE_CONSTANT static const
#define SHENMUE_MODEL_IMAGE_STRINGS static const char *const
#define SHENMUE_MODEL_IMAGE_INITIALIZER =
#define SHENMUE_MODEL_IMAGE_NULL 0
namespace shenmue {
namespace s2 {

struct PakGlobalData;
#define TASK_MDLM (::shenmue::shen_name(SHEN_NAME4('M', 'D', 'L', 'M')))
#define MEMORY_MAIX (::shenmue::shen_name(SHEN_NAME4('M', 'A', 'I', 'X')))
#define MEMORY_MAIL (::shenmue::shen_name(SHEN_NAME4('M', 'A', 'I', 'L')))
#define MEMORY_MODEL_FILE (::shenmue::shen_name(SHEN_NAME4('M', 'D', 'L', ' ')))
#define MODEL_MANAGER_MARKER (::shenmue::shen_name(SHEN_NAME4('M', 'M', 'T', 'W')))
struct ModelImageLevel {
    MT7Model *model;
    char *filename;
    std::int32_t threshold;
};
struct ModelImageIndex {
    ModelImageIndex *next;
    char *name;
    std::int32_t levelCount;
    ModelImageLevel *levels;
    std::uint32_t referenceCount;
    PakGlobalData *pak;
};
struct ModelManagerEntry {
    std::uint32_t flags;
    std::uint32_t distancePriority;
    ModelImageIndex *image;
    MT7Node *instance;
    MT7Model *source;
    std::uint32_t childIndex;
    std::int32_t groupId;
    std::uint32_t variant;
};
struct ModelManagerTaskData {
    shen_name marker;
    std::uint32_t state;
    char managerState[0xc78];
    std::uint32_t entryCount;
    ModelImageIndex *buckets[19];
    ModelManagerEntry entries[256];
};
struct ModelImageFileRead {
    MT7Model **destination;
    char directory[64];
    char filename[16];
};
SHENMUE_MODEL_IMAGE_LAYOUT(sizeof(ModelImageLevel) == 12);
SHENMUE_MODEL_IMAGE_LAYOUT(sizeof(ModelImageIndex) == 0x18);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelImageIndex, pak) == 0x14);
SHENMUE_MODEL_IMAGE_LAYOUT(sizeof(ModelManagerEntry) == 0x20);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerEntry, image) == 8);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerEntry, instance) == 0x0c);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerEntry, source) == 0x10);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerEntry, childIndex) == 0x14);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerTaskData, entryCount) == 0xc80);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerTaskData, buckets) == 0xc84);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelManagerTaskData, entries) == 0xcd0);
SHENMUE_MODEL_IMAGE_LAYOUT(sizeof(ModelManagerTaskData) == 0x2cd0);
SHENMUE_MODEL_IMAGE_LAYOUT(sizeof(ModelImageFileRead) == 0x54);
SHENMUE_MODEL_IMAGE_LAYOUT(offsetof(ModelImageFileRead, filename) == 0x44);
extern ModelImageFileRead *modelImageReads;
extern std::int32_t modelImageReadCount, modelImageReadStride;
typedef std::int32_t ModelImageDefinitionCommand;
enum {
    ModelImageDefinitionCommand_Image = 0,
    ModelImageDefinitionCommand_Avail = 1,
    ModelImageDefinitionCommand_NotAvail = 2,
    ModelImageDefinitionCommand_Dynamic = 3,
    ModelImageDefinitionCommand_AvailExt = 4
};
SHENMUE_MODEL_IMAGE_STRINGS modelImageDefinitionTokens[] SHENMUE_MODEL_IMAGE_INITIALIZER{
    "Image", "Avail", "NotAvail", "Dynamic", "Avail_Ext", SHENMUE_MODEL_IMAGE_NULL};
MT7Model *FindPreloadedCharacterImageNullsub(const char *filename);
void BeginModelImageFileReads();
void QueueModelImageFileRead(MT7Model **destination, const char *directory, const char *filename);
bool ModelImageReadComesAfter(const ModelImageFileRead *first, const ModelImageFileRead *second);
void FinishModelImageFileReads();
HLib::Task *EnqueueModelManagerTask(std::uint8_t index);
void ModelManagerTaskCallback(void *data);
std::uint32_t ImageBucket(const char *name);
char *StoreImageString(ModelManagerTaskData *manager, const char *text);
ModelImageIndex *FindImageIndex(HLib::Task *task, const char *name);
ModelImageIndex *AllocateModelImageIndex(HLib::Task *task, const char *name);
bool AllocateModelImageLevel(HLib::Task *task, const char *name, const char *directory, const char *filename);
void ReleaseImageIndex(HLib::Task *task, const char *name);
void ReleasePackageModelImages(HLib::Task *task, PakGlobalData *package);

} // namespace s2
} // namespace shenmue
#undef SHENMUE_MODEL_IMAGE_LAYOUT_NAME_
#undef SHENMUE_MODEL_IMAGE_LAYOUT_NAME
#undef SHENMUE_MODEL_IMAGE_LAYOUT
#undef SHENMUE_MODEL_IMAGE_CONSTANT
#undef SHENMUE_MODEL_IMAGE_STRINGS
#undef SHENMUE_MODEL_IMAGE_INITIALIZER
#undef SHENMUE_MODEL_IMAGE_NULL

#endif
