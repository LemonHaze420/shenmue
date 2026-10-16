#pragma once

#include "common/math.hpp"
#include "common/types.hpp"
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {
struct FileState;
}
namespace shenmue::s2 {
namespace HLib {
struct Task;
}
struct Character;
struct MT7Model;
struct MT7Node;
struct ModelManagerEntry;
struct PakGlobalData;

inline constexpr shen_name TASK_IMGM{SHEN_NAME(IMGM)};
inline constexpr shen_name MEMORY_MAIX{SHEN_NAME(MAIX)};
inline constexpr shen_name MEMORY_MAIL{SHEN_NAME(MAIL)};
inline constexpr shen_name MEMORY_HIMG{SHEN_NAME(HIMG)};
inline constexpr shen_name MEMORY_MODEL_FILE{make_shen_name('M', 'D', 'L', ' ')};

struct ModelImageLevel {
    MT7Model *model;
    char *filename;
    std::uint32_t threshold;
    std::uint32_t reserved14;
};
struct ModelImageIndex {
    ModelImageIndex *next;
    char *name;
    std::uint32_t levelCount;
    std::int32_t reserved14;
    ModelImageLevel *levels;
    std::uint32_t referenceCount;
    std::uint32_t reserved24;
    void *pak;
};
#pragma pack(push, 4)
struct ImageModuleComponent {
    HLib::Task *manager;
    ModelManagerEntry **entries;
    std::uint8_t count;
    std::uint8_t selected;
    std::uint8_t previous;
    std::uint8_t forceFirst;
    std::uint32_t rigType;
    std::uint32_t field18;
    std::uint32_t sourceType;
    std::uint32_t field20;
    std::uint32_t field24;
    float field28;
    float field2c;
    float field30;
    std::uint32_t field34;
};
#pragma pack(pop)
static_assert(sizeof(ModelImageLevel) == 0x18);
static_assert(sizeof(ModelImageIndex) == 0x30);
static_assert(offsetof(ModelImageIndex, levels) == 0x18);
static_assert(sizeof(ImageModuleComponent) == 0x38);
static_assert(offsetof(ImageModuleComponent, count) == 0x10);
struct ModelImageFileRead {
    MT7Model **destination;
    char directory[64];
    char filename[16];
    d3t::FileState *file;
    void *allocation;
};
static_assert(sizeof(ModelImageFileRead) == 0x68);
static_assert(offsetof(ModelImageFileRead, file) == 0x58);
static_assert(offsetof(ModelImageFileRead, allocation) == 0x60);

void BeginModelImageFileReads();
void QueueModelImageFileRead(MT7Model **destination, const char *directory, const char *filename);
void FinishModelImageFileReads();
static_assert(offsetof(ImageModuleComponent, field2c) == 0x2c);
inline constexpr const char *modelRigNames[]{
    "AKI", "BIG", "CHA", "DEB", "KKO", "GEN", "IWA", "JIJ", "KOD", "KOK", "OHS", "REN", "SYP", "YKI", "YUA", "SIN", "CAT", "YAG", "DOG", "PNW", "KCT"};
void InitRigType(Character *character, const char *name);

ModelImageIndex *AllocateModelImageIndex(HLib::Task *manager, const char *name);
bool AllocateModelImageLevel(HLib::Task *manager, const char *name, const char *directory, const char *filename);
ImageModuleComponent *SetupImageModuleHandlers(Character *character);
ImageModuleComponent *AssignImageModuleParameters(Character *character, std::uint8_t mode, std::int32_t image, float blend, std::int32_t flags);
void SetupImageModule(Character *character, HLib::Task *manager, const char *name, std::uint8_t flags);
void SetupImageModuleVariants(Character *character, HLib::Task *manager, const char *name, std::uint32_t count, std::uint8_t flags);
void ReplaceScenarioImageModule(shen_name character, const char *name, std::uint32_t count);
void SetScenarioImageModule(shen_name character, const char *name);
MT7Model *GetImageModuleSource(Character *character);
ImageModuleComponent *SelectImageModule(Character *character, std::uint8_t selected);
ImageModuleComponent *SelectScenarioImageModule(shen_name character, std::uint8_t selected);
const void *GetImageModuleSelectedTransform(const Character *character);
std::int16_t GetCharacterImageSourceType(const Character *character);
std::uint32_t AdvanceImageModelFrame(MT7Node *root, std::uint32_t flags, float *frame);
void IMGM_AssignMTWK(Character *character);
void DrawImageCharacterCore(Character *character);
void ReleaseModelImageManager(HLib::Task *manager);
void ReleasePackageModelImages(HLib::Task *manager, PakGlobalData *pak);
} // namespace shenmue::s2
