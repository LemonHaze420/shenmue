#include "s2/model_image.hpp"

#include "s2/camera.hpp"
#include "s2/character.hpp"
#include "s2/face.hpp"
#include "s2/hlib.hpp"
#include "s2/game_runtime.hpp"
#include "s2/frame_pacing.hpp"
#include "s2/world_render.hpp"
#include "s2/legacy_draw.hpp"
#include "s2/model_clone.hpp"
#include "s2/model_component.hpp"
#include "s2/motion_work.hpp"
#include "s2/event_runtime.hpp"
#include "s2/map_assets.hpp"
#include "s2/map_control.hpp"
#include "s2/mt7.hpp"
#include "s2/npc_package.hpp"
#include "s2/osage.hpp"
#include "s2/pak.hpp"
#include "s2/archive_data.hpp"
#include "s2/scene_data.hpp"
#include "d3t/file_state.hpp"
#include <cctype>
#include <bit>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
#include <new>

namespace shenmue::s2 {
namespace {
struct ModelImageFile : MT7Model {
    void *file;
};
ModelImageFileRead *modelImageReads;
std::int32_t modelImageReadCount;
std::ptrdiff_t modelImageReadStride;

bool ModelImageReadComesAfter(const ModelImageFileRead &first, const ModelImageFileRead &second) {
    for (const char *firstText : {first.directory, first.filename}) {
        const char *secondText = firstText == first.directory ? second.directory : second.filename;
        for (std::size_t index = 0;; ++index) {
            const int left = std::toupper(static_cast<unsigned char>(firstText[index]));
            const int right = std::toupper(static_cast<unsigned char>(secondText[index]));
            if (left != right)
                return left > right;
            if (!left)
                break;
        }
    }
    return false;
}

ModelManagerTaskData *ManagerData(HLib::Task *task) {
    return static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
}
// @verified - p
std::uint32_t ImageBucket(const char *name) {
    std::uint32_t sum = 0;
    for (const unsigned char *cursor = reinterpret_cast<const unsigned char *>(name); *cursor; ++cursor)
        sum += *cursor;
    return sum % 19U;
}
// @verified - p
ModelImageIndex *FindImageIndex(ModelManagerTaskData &manager, const char *name) {
    PakGlobalData *const context = GetPakLoadParams() ? pakLoadGetNext() : nullptr;
    for (ModelImageIndex *image = reinterpret_cast<ModelImageIndex *>(manager.buckets[ImageBucket(name)]); image; image = image->next)
        if (std::strcmp(image->name, name) == 0 && (!context || image->pak == context))
            return image;
    return nullptr;
}
// @verified - p
char *StoreImageString(ModelManagerTaskData &manager, const char *value) {
    const std::size_t size = std::strlen(value) + 1;
    char *const destination = reinterpret_cast<char *>(manager.managerState) + manager.entryCount;
    std::memcpy(destination, value, size);
    manager.entryCount += static_cast<std::uint32_t>(size);
    return destination;
}
// @verified - p
void RemoveImageString(ModelManagerTaskData &manager, char *value) {
    const std::size_t size = std::strlen(value) + 1;
    char *const next = value + size;
    char *const end = reinterpret_cast<char *>(manager.managerState) + manager.entryCount;
    std::memmove(value, next, static_cast<std::size_t>(end - next));
    for (std::uintptr_t bucket : manager.buckets) {
        for (ModelImageIndex *image = reinterpret_cast<ModelImageIndex *>(bucket); image; image = image->next) {
            if (image->name >= next)
                image->name -= size;
            for (std::int32_t level = 0; level < std::bit_cast<std::int32_t>(image->levelCount); ++level)
                if (image->levels[level].filename >= next)
                    image->levels[level].filename -= size;
        }
    }
    manager.entryCount -= static_cast<std::uint32_t>(size);
}
void ReleaseImageFile(MT7Model *model) {
    if (!model || FindRetainingModelPackage(model))
        return;
    ModelImageFile *const owner = static_cast<ModelImageFile *>(model);
    void *const file = owner->file;
    ReleaseModelFormat7(*owner);
    owner->~ModelImageFile();
    HLib::FreeTaskData(file);
    HLib::FreeTaskData(owner);
}
// @verified - p
void ReleaseImageIndex(HLib::Task *task, ModelImageIndex *image) {
    ModelManagerTaskData &manager = *ManagerData(task);
    for (std::int32_t level = 0; level < std::bit_cast<std::int32_t>(image->levelCount); ++level) {
        ReleaseImageFile(image->levels[level].model);
        image->levels[level].model = nullptr;
    }
    RemoveImageString(manager, image->name);
    for (std::int32_t level = 0; level < std::bit_cast<std::int32_t>(image->levelCount); ++level)
        RemoveImageString(manager, image->levels[level].filename);
    for (std::uintptr_t &bucket : manager.buckets) {
        ModelImageIndex **link = reinterpret_cast<ModelImageIndex **>(&bucket);
        while (*link && *link != image)
            link = &(*link)->next;
        if (*link) {
            *link = image->next;
            break;
        }
    }
    if (image->levels)
        HLib::FreeTaskData(image->levels);
    HLib::FreeTaskData(image);
}
// @verified - p
ModelManagerEntry *AllocateImageEntry(HLib::Task *task, const char *name) {
    ModelManagerTaskData &manager = *ManagerData(task);
    ModelManagerEntry *entry = nullptr;
    for (ModelManagerEntry &candidate : manager.entries)
        if (!(candidate.flags & 1U)) {
            entry = &candidate;
            break;
        }
    if (!entry)
        return nullptr;
    ModelImageIndex *image = FindImageIndex(manager, name);
    if (!image)
        image = AllocateModelImageIndex(task, name);
    if (!image)
        return nullptr;
    entry->image = image;
    entry->flags = 1;
    entry->distancePriority = 0;
    ++image->referenceCount;
    entry->instance = nullptr;
    entry->source = nullptr;
    entry->childIndex = 0;
    entry->groupId = -1;
    entry->variant = 0;
    return entry;
}
// @verified - p
void UpdateImageDistance(ModelManagerEntry &entry, Vector3 *position) {
    if (!position) {
        entry.distancePriority = 0;
        return;
    }
    Vector3 eye;
    camCtrlGetNextCamPos(&eye);
    position->x = (position->x - eye.x) * (position->x - eye.x);
    position->y = (position->y - eye.y) * (position->y - eye.y);
    position->z = (position->z - eye.z) * (position->z - eye.z);
    const float distanceSquared = (position->y + position->x) + position->z;
    if (0.00000011920929f > std::fabs(distanceSquared))
        entry.distancePriority = 0;
    else {
        const float priority = 1000000.0f / std::sqrt(distanceSquared);
        entry.distancePriority = priority > 2147483648.0f ? 0x7fffffffU : static_cast<std::uint32_t>(static_cast<std::int32_t>(priority));
    }
}
// @verified - p
MT7Model *CloneImageSelection(const MT7Model &source, const ModelManagerEntry &entry, std::uint8_t flags) {
    if (source.nodes.empty())
        return nullptr;
    std::int32_t selected = 0;
    if (entry.childIndex) {
        selected = source.nodes[0].firstChild;
        for (std::int32_t child = 1; selected >= 0 && child < static_cast<std::int32_t>(entry.childIndex); ++child)
            selected = source.nodes[static_cast<std::size_t>(selected)].nextSibling;
        if (selected < 0)
            return nullptr;
    }
    const MT7Node &root = source.nodes[static_cast<std::size_t>(selected)];
    const std::int16_t signature = static_cast<std::int16_t>(root.id);
    if (entry.groupId != -1 && (signature == 62 || signature > 28672)) {
        for (std::int32_t group = root.firstChild; group >= 0; group = source.nodes[static_cast<std::size_t>(group)].nextSibling) {
            if (static_cast<std::int16_t>(source.nodes[static_cast<std::size_t>(group)].id) != entry.groupId)
                continue;
            std::int32_t variant = source.nodes[static_cast<std::size_t>(group)].firstChild;
            for (std::int32_t index = 0; variant >= 0 && index < static_cast<std::int32_t>(entry.variant); ++index)
                variant = source.nodes[static_cast<std::size_t>(variant)].nextSibling;
            if (variant >= 0)
                selected = variant;
            break;
        }
    }
    return CloneMT7ModelBranch(GetMT7ReaderData(&source), source, static_cast<std::uint32_t>(selected), flags);
}
// @verified - p
MT7Model *ResolveImageEntry(ModelManagerEntry &entry, std::uint8_t flags) {
    MT7Model *source = nullptr;
    if (entry.image && entry.image->levels) {
        ModelImageLevel *selected = entry.image->levels;
        for (std::int32_t level = 0; level < static_cast<std::int32_t>(entry.image->levelCount); ++level) {
            ModelImageLevel &candidate = entry.image->levels[level];
            if (!candidate.model || static_cast<std::int32_t>(entry.distancePriority) < static_cast<std::int32_t>(candidate.threshold))
                break;
            selected = &candidate;
        }
        source = selected->model;
    }
    if ((entry.flags & 2U) || source != entry.source) {
        if (entry.instance)
            ReleaseMT7ModelNodeTreeClone(entry.instance);
        entry.instance = nullptr;
        entry.source = nullptr;
        if (source)
            entry.instance = CloneImageSelection(*source, entry, flags);
        entry.flags &= ~2U;
        entry.source = source;
    }
    return entry.instance;
}
// @verified - p
void ReleaseImageModule(ImageModuleComponent &image) {
    if (!image.entries)
        return;
    for (std::uint32_t index = 0; index < image.count; ++index) {
        ModelManagerEntry &entry = *image.entries[index];
        ModelImageIndex *const source = entry.image;
        if (source && --source->referenceCount == 0) {
            ModelImageIndex *const selected = FindImageIndex(*ManagerData(image.manager), source->name);
            if (selected)
                ReleaseImageIndex(image.manager, selected);
        }
        if (entry.instance)
            ReleaseMT7ModelNodeTreeClone(entry.instance);
        entry.instance = nullptr;
        entry.flags = 0;
    }
    HLib::FreeTaskData(image.entries);
    image.entries = nullptr;
    image.count = image.selected = image.previous = image.forceFirst = 0;
    image.rigType &= 0xffffff00U;
    image.field18 = 0;
    image.field2c = image.field30 = 0.0f;
}
// @verified - p
MT7Model *ResolveImageModule(ImageModuleComponent &image, Vector3 *position, std::uint8_t flags) {
    if (!image.entries)
        return nullptr;
    if (image.forceFirst) {
        image.selected = 0;
        image.entries[0]->childIndex = 0;
        image.entries[0]->groupId = 0;
        image.entries[0]->variant = 0;
    }
    for (std::uint32_t index = 0; index < image.count; ++index)
        UpdateImageDistance(*image.entries[index], position);
    ModelManagerEntry &entry = *image.entries[image.selected];
    MT7Model *const result = ResolveImageEntry(entry, flags);
    const std::uint16_t sourceType = entry.source ? static_cast<std::uint16_t>(entry.source->entityType) : 0;
    image.sourceType = (image.sourceType & 0xffff0000U) | sourceType;
    return result;
}
// @verified - p
void ImageModuleUpdate(Character *character, void *parameter) {
    ImageModuleComponent &image = *static_cast<ImageModuleComponent *>(parameter);
    Vector3 position;
    Vector3 *visiblePosition = nullptr;
    if (IsOfThisVisibilityType(character, 0)) {
        position = GetCharacterWorldPosition(character);
        visiblePosition = &position;
    }
    MT7Model *const model = ResolveImageModule(image, visiblePosition, 0);
    if (image.previous != image.selected) {
        character->model = model;
        mtwkSetSequenceFlags(character);
        image.previous = image.selected;
    }
}
// @verified - p
void ImageModuleCleanup(Character *character, void *parameter) {
    ReleaseImageModule(*static_cast<ImageModuleComponent *>(parameter));
    character->model = nullptr;
}
}
// @verified - p
void BeginModelImageFileReads() {
    modelImageReadCount = 0;
    modelImageReads = nullptr;
}

void QueueModelImageFileRead(MT7Model **destination, const char *directory, const char *filename) {
    ModelImageFileRead *const read = reinterpret_cast<ModelImageFileRead *>(allocateInGlobalHeap(sizeof(ModelImageFileRead)));
    read->destination = destination;
    std::strcpy(read->directory, directory);
    std::strcpy(read->filename, filename);
    read->file = d3t::Archive_GetNextUnusedFileState();
    char physicalName[16];
    std::strcpy(physicalName, filename);
    if (char *const extension = std::strstr(physicalName, ".MT5"))
        std::strcpy(extension, ".MT7");
    std::snprintf(read->file->path, sizeof(read->file->path), "%s/%s", directory, physicalName);
    read->file->offset = 0;
    const std::uint64_t size = ArchiveData_GetFileSize(read->file->path);
    read->allocation = HLib::AllocateTaskData(size, MEMORY_MODEL_FILE);
    ReadLegacySceneFileIntoBuffer(read->file, read->allocation, size);
    if (!modelImageReads) {
        modelImageReads = read;
        void *const next = allocateInGlobalHeap(sizeof(ModelImageFileRead));
        SetTaskHeapData(next);
        modelImageReadStride = static_cast<std::byte *>(next) - reinterpret_cast<std::byte *>(read);
    }
    ++modelImageReadCount;
}

void FinishModelImageFileReads() {
    if (!modelImageReads || !modelImageReadCount)
        return;
    const std::uintptr_t first = reinterpret_cast<std::uintptr_t>(modelImageReads);
    for (std::int32_t index = 1; index < modelImageReadCount; ++index) {
        ModelImageFileRead read = *reinterpret_cast<ModelImageFileRead *>(first + index * modelImageReadStride);
        std::int32_t previous = index - 1;
        while (previous >= 0) {
            ModelImageFileRead *const candidate = reinterpret_cast<ModelImageFileRead *>(first + previous * modelImageReadStride);
            if (!ModelImageReadComesAfter(*candidate, read))
                break;
            *reinterpret_cast<ModelImageFileRead *>(first + (previous + 1) * modelImageReadStride) = *candidate;
            --previous;
        }
        *reinterpret_cast<ModelImageFileRead *>(first + (previous + 1) * modelImageReadStride) = read;
    }
    bool complete;
    do {
        complete = true;
        for (std::int32_t index = 0; index < modelImageReadCount; ++index) {
            ModelImageFileRead &read = *reinterpret_cast<ModelImageFileRead *>(first + index * modelImageReadStride);
            if (!read.file)
                continue;
            if (d3t::Archive_HasPendingRead(read.file->handle)) {
                complete = false;
                continue;
            }
            if (read.allocation) {
                const std::size_t size = static_cast<std::size_t>(ArchiveData_GetFileSize(read.file->path));
                ModelImageFile *const owner = new (HLib::AllocateTaskData(sizeof(ModelImageFile), MEMORY_MODEL_FILE)) ModelImageFile{};
                owner->file = read.allocation;
                char path[256];
                const int length = std::snprintf(path, sizeof(path), "/%s/%s", read.directory, read.filename);
                std::memcpy(path + length - 3, "fbx", 3);
                if (ReadModelFormat7({static_cast<const std::byte *>(read.allocation), size}, path, *owner, 0x22)) {
                    BindMT7WritableRuntimeMeshes(*owner, {static_cast<std::byte *>(read.allocation), size});
                    *read.destination = owner;
                } else
                    ReleaseImageFile(owner);
            }
            d3t::ReadFile_0(0, &read.file->handle);
            read.file = nullptr;
        }
    } while (!complete);
    for (std::int32_t index = modelImageReadCount - 1; index >= 0; --index)
        SetTaskHeapData(reinterpret_cast<void *>(first + index * modelImageReadStride));
    modelImageReadCount = 0;
    modelImageReads = nullptr;
}

// @verified - p
ModelImageIndex *AllocateModelImageIndex(HLib::Task *task, const char *name) {
    ModelManagerTaskData &manager = *ManagerData(task);
    if (ModelImageIndex *const existing = FindImageIndex(manager, name))
        return existing;
    ModelImageIndex *const image = static_cast<ModelImageIndex *>(HLib::AllocateTaskData(sizeof(ModelImageIndex), MEMORY_MAIX));
    if (!image)
        return nullptr;
    image->name = StoreImageString(manager, name);
    image->levels = nullptr;
    image->levelCount = 0;
    image->referenceCount = 0;
    const std::uint32_t bucket = ImageBucket(name);
    image->next = reinterpret_cast<ModelImageIndex *>(manager.buckets[bucket]);
    manager.buckets[bucket] = reinterpret_cast<std::uintptr_t>(image);
    return image;
}
bool AllocateModelImageLevel(HLib::Task *task, const char *name, const char *directory, const char *filename) {
    ModelManagerTaskData &manager = *ManagerData(task);
    ModelImageIndex *const image = AllocateModelImageIndex(task, name);
    if (!image)
        return false;
    if (!image->levelCount) {
        image->levelCount = 1;
        image->levels = static_cast<ModelImageLevel *>(HLib::AllocateTaskData(sizeof(ModelImageLevel), MEMORY_MAIL));
        if (!image->levels)
            return false;
        image->levels->filename = StoreImageString(manager, filename);
    }
    image->levels->threshold = 1;
    std::uint32_t size = 0;
    void *file = LoadLegacySceneFile(directory, image->levels->filename, MEMORY_MODEL_FILE, &size);
    if (!file) {
        image->levels->model = nullptr;
        return true;
    }
    void *const allocation = HLib::AllocateTaskData(sizeof(ModelImageFile), MEMORY_MODEL_FILE);
    if (!allocation) {
        HLib::FreeTaskData(file);
        return false;
    }
    ModelImageFile *const model = new (allocation) ModelImageFile{};
    model->file = file;
    char path[256];
    std::snprintf(path, sizeof(path), "%s/%s", directory, image->levels->filename);
    if (!ReadModelFormat7({static_cast<const std::byte *>(file), size}, path, *model, 0x22)) {
        ReleaseImageFile(model);
        image->levels->model = nullptr;
    } else {
        BindMT7WritableRuntimeMeshes(*model, {static_cast<std::byte *>(file), size});
        image->levels->model = model;
    }
    return true;
}

// @verified - p
std::uint32_t AdvanceImageModelFrame(MT7Node *root, std::uint32_t flags, float *frame) {
    MT7Node *group = root->runtimeChild;
    for (std::uint32_t index = (flags >> 8U) & 15U; index; --index)
        group = group->runtimeSibling;
    MT7Node *const first = group->runtimeChild;
    std::int32_t last = 0;
    for (MT7Node *node = first; node && node->runtimeSibling; node = node->runtimeSibling)
        ++last;
    float delta = 0.0f;
    const std::uint32_t speed = flags & 0xf0000U;
    if (speed != 0x80000U && (speed != 0x20000U || (g_rootTaskIterationCount & 3U)) && (speed != 0x40000U || (g_rootTaskIterationCount & 7U)))
        delta = shenGetScaledDeltaTickTime(1.0f);
    std::uint32_t reverse = flags & 0x100000U;
    if (flags & 0x2000U)
        *frame = reverse ? *frame - delta : delta + *frame;
    else {
        *frame = delta + *frame;
        reverse = 0;
    }
    std::int32_t selected = *frame >= -2147483648.0f && *frame < 2147483648.0f ? static_cast<std::int32_t>(*frame) : std::numeric_limits<std::int32_t>::min();
    if ((flags & 0x2000U) && selected < 0) {
        selected = 0;
        *frame = 0.0f;
        reverse = 0;
    }
    if (selected > last) {
        if (flags & 0x2000U) {
            selected = last;
            *frame = static_cast<float>(last) + 0.99999988f;
            reverse = 0x100000U;
        } else {
            selected = 0;
            *frame = 0.0f;
        }
    }
    MT7Node *node = first;
    for (std::int32_t index = selected; index > 0; --index)
        node = node->runtimeSibling;
    return static_cast<std::uint32_t>(selected) | reverse | (flags & 0xfff00U);
}

// @verified - p
void IMGM_AssignMTWK(Character *character) {
    const ImageModuleComponent *const image = static_cast<const ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    if (image->forceFirst) {
        charaAssignCoreCallback(character, DrawImageCharacterCore);
        imgmAssignUpdateCallback(character, imgmDrawModelNode);
    }
}
// @verified - p
ImageModuleComponent *SetupImageModuleHandlers(Character *character) {
    ImageModuleComponent *const image =
        static_cast<ImageModuleComponent *>(AllocateModelComponent(character, TASK_IMGM, sizeof(ImageModuleComponent), ImageModuleUpdate, ImageModuleCleanup));
    if (!image)
        return nullptr;
    image->manager = nullptr;
    image->entries = nullptr;
    image->count = image->selected = image->previous = image->forceFirst = 0;
    image->rigType &= 0xffffff00U;
    image->field18 = image->field20 = 0;
    image->field24 = 0;
    image->field28 = image->field2c = image->field30 = 0.0f;
    SetFlagForToken(character, TASK_IMGM, 0);
    return image;
}
// @verified - p
ImageModuleComponent *AssignImageModuleParameters(Character *character, std::uint8_t mode, std::int32_t image, float blend, std::int32_t flags) {
    ImageModuleComponent *const component = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    component->field24 = 0;
    component->field28 = 0.0f;
    component->field30 = 0.0f;
    component->field2c = blend;
    component->forceFirst = mode;
    component->field20 = static_cast<std::uint32_t>(flags);
    component->rigType &= 0xffffff00U;
    component->field18 = static_cast<std::uint32_t>(image);
    return component;
}

// @verified - p
std::int16_t GetCharacterImageSourceType(const Character *character) {
    const ImageModuleComponent *const image = static_cast<const ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    return image ? static_cast<std::int16_t>(image->sourceType) : 0;
}


// @verified - p
void SetupImageModule(Character *character, HLib::Task *manager, const char *name, std::uint8_t flags) {
    SetupImageModuleVariants(character, manager, name, 0, flags);
}

// @verified - p
void SetupImageModuleVariants(Character *character, HLib::Task *manager, const char *name, std::uint32_t count, std::uint8_t flags) {
    ClearCharacterFacialModel(character);
    ImageModuleComponent &image = *static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    ReleaseImageModule(image);
    image.manager = manager;
    image.count = static_cast<std::uint8_t>(count ? count : 1U);
    const std::uint32_t entryCount = count ? count : 1U;
    image.entries = static_cast<ModelManagerEntry **>(HLib::AllocateTaskData(sizeof(ModelManagerEntry *) * entryCount, MEMORY_HIMG));
    for (std::uint32_t index = 0; index < entryCount; ++index) {
        image.entries[index] = AllocateImageEntry(manager, name);
        ModelManagerEntry &entry = *image.entries[index];
        const std::uint32_t childIndex = count ? index + 1U : 0U;
        if (entry.childIndex != childIndex || entry.groupId != 63 || entry.variant != 0) {
            entry.flags |= 2U;
            entry.childIndex = childIndex;
            entry.groupId = 63;
            entry.variant = 0;
        }
        entry.distancePriority = 1;
    }
    image.selected = 0;
    Vector3 position;
    Vector3 *visiblePosition = nullptr;
    if (IsOfThisVisibilityType(character, 0)) {
        position = GetCharacterWorldPosition(character);
        visiblePosition = &position;
    }
    character->model = ResolveImageModule(image, visiblePosition, flags);
    mtwkSetSequenceFlags(character);

    MT7Model *const source = GetImageModuleSource(character);
    character->facialModel = source && source->facial ? &source->facial->data : nullptr;
    if (character->facialModel && character->model)
        GraftFacialModelNode(*static_cast<MT7Model *>(character->model), *source->facial);
}

// @verified - p
void ReplaceScenarioImageModule(shen_name characterName, const char *name, std::uint32_t count) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(control->scenarioTask, characterName);
    if (name) {
        RestoreClothMeshBindings(character);
        MapControlData *const current = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModuleVariants(character, current->modelManagerTask, name, count, 0);
        if (FindMotionWork(character)) {
            FindCLMD(character);
            FindOSAG(character);
        }
    } else {
        ClearCharacterFacialModel(character);
        ReleaseImageModule(*static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM)));
        character->model = nullptr;
        ResetHandMeasurementBindings(character);
    }
}
// @verified - p
void SetScenarioImageModule(shen_name characterName, const char *name) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(control->scenarioTask, characterName);
    if (name) {
        MapControlData *const current = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
        SetupImageModule(character, current->modelManagerTask, name, 0);
        MT7Model *const model = static_cast<MT7Model *>(character->model);
        if (model)
            SetupModelForImageModule(model->nodes.data());
    } else {
        ClearCharacterFacialModel(character);
        ReleaseImageModule(*static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM)));
        character->model = nullptr;
    }
}
// @verified - p
MT7Model *GetImageModuleSource(Character *character) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    if (!image || !image->entries)
        return nullptr;
    ModelManagerEntry *const entry = image->entries[image->selected];
    return entry ? entry->source : nullptr;
}
// @verified - p
ImageModuleComponent *SelectImageModule(Character *character, std::uint8_t selected) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    image->selected = selected;
    return image;
}
// @verified - p
ImageModuleComponent *SelectScenarioImageModule(shen_name characterName, std::uint8_t selected) {
    MapControlData *const control = static_cast<MapControlData *>(HLib::GetTaskParameter(g_mapControlTask));
    Character *const character = GetScenarioCharacterNode(control->scenarioTask, characterName);
    return character ? SelectImageModule(character, selected) : nullptr;
}
// @verified - p
const void *GetImageModuleSelectedTransform(const Character *character) {
    const ImageModuleComponent *const image = static_cast<const ImageModuleComponent *>(FindModelComponent(character, TASK_IMGM));
    if (!image || !image->entries)
        return nullptr;
    const ModelManagerEntry *const entry = image->entries[image->selected];
    const MT7Model *const source = entry ? entry->source : nullptr;
    return source && !source->nodes.empty() ? GetMT7NodeTransformOverride(source->nodes[0]) : nullptr;
}
// @verified - p
void InitRigType(Character *character, const char *name) {
    MT7Model *const source = GetImageModuleSource(character);
    for (std::int32_t index = 0; index < 21; ++index) {
        if (std::strcmp(name, modelRigNames[index]) == 0) {
            source->entityType = index;
            break;
        }
    }
}

// @verified - p
void ReleasePackageModelImages(HLib::Task *task, PakGlobalData *pak) {
    if (!pak)
        return;
    ModelManagerTaskData &manager = *ManagerData(task);
    for (std::uintptr_t &bucket : manager.buckets) {
        ModelImageIndex *image = reinterpret_cast<ModelImageIndex *>(bucket);
        while (image) {
            ModelImageIndex *next = image->next;
            if (image->pak == pak) {
                ModelImageIndex *const selected = FindImageIndex(manager, image->name);
                if (selected) {
                    if (selected == next)
                        next = selected->next;
                    ReleaseImageIndex(task, selected);
                }
            }
            image = next;
        }
    }
}
void ReleaseModelImageManager(HLib::Task *task) {
    ModelManagerTaskData *const manager = ManagerData(task);
    if (!manager)
        return;
    for (ModelManagerEntry &entry : manager->entries) {
        if (!(entry.flags & 1U))
            continue;
        if (entry.instance)
            ReleaseMT7ModelNodeTreeClone(entry.instance);
        entry.instance = nullptr;
        entry.flags = 0;
    }
    for (std::uintptr_t &bucket : manager->buckets)
        while (bucket)
            ReleaseImageIndex(task, reinterpret_cast<ModelImageIndex *>(bucket));
}
} // namespace shenmue::s2
