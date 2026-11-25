#include "s2/model_image_component_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/character_components_dc.hpp"
#include "s2/scene_state_dc.hpp"
#include "s2/pause.hpp"
#include <cmath>
#include <cstring>
#include "s2/model_image_dc.hpp"
#include "s2/event_character_model_dc.hpp"
#include "s2/pak_dc.hpp"
#include "s2/face_controller_dc.hpp"
#include "s2/cloth_dc.hpp"
#include "s2/osage_dc.hpp"
#include "s2/hand_pose_dc.hpp"
#include "s2/map_control_dc.hpp"
#include "s2/motion_node_dc.hpp"
#include "s2/motion_work_dc.hpp"
#include "s2/prop_dc.hpp"
#include "s2/scenario_dc.hpp"
#include "s2/character_table_dc.hpp"
#include "s2/file_loader_dc.hpp"
#include "s2/scene_data.hpp"
#include <cstdio>
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include <ctype.h>
#endif

namespace shenmue {
namespace s2 {

void ModelManagerTaskCallback(void *);
void ReleaseImageIndex(HLib::Task *task, const char *name);
void ImageModuleUpdate(Character *character, void *data);
void ImageModuleCleanup(Character *character, void *data);
void SetupImageModuleVariants(Character *character, HLib::Task *manager, const char *name, std::int32_t count);
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AssignImageModuleParameters(Character *character, std::int32_t mode, std::int32_t remaining, float step, std::uint32_t group);
#else
void AssignImageModuleParameters(Character *character, std::uint8_t mode, std::int32_t remaining, float step, std::uint32_t group);
#endif
void DrawImageCharacterCore(Character *character);

void ClearCharacterFacialModel(Character *character);
void BindCharacterFacialModel(Character *character);
void ClearCharacterFacialModel(Character *character);
void BindCharacterFacialModel(Character *character);
std::uint32_t AdvanceImageModelFrame(MT7Node *root, std::uint32_t flags);
void DrawImageModelSelection(Character *character, std::uint32_t flags);
MT7Model *InitializeTypedMT7Model(void *source, const char *filename);
ModelImageFileRead *modelImageReads;
std::int32_t modelImageReadCount, modelImageReadStride;

namespace {

inline void RemoveImageString(ModelManagerTaskData *manager, char *value) {
    const std::size_t bytes = std::strlen(value) + 1;
    char *const next = value + bytes;
    char *const end = manager->managerState + manager->entryCount;
    std::memmove(value, next, end - next);
    for (std::size_t rangeIndex110 = 0; rangeIndex110 < sizeof(manager->buckets) / sizeof((manager->buckets)[0]); ++rangeIndex110) {
        ModelImageIndex *bucket = (manager->buckets)[rangeIndex110];

        for (ModelImageIndex *image = bucket; image; image = image->next) {
            if (image->name >= next)
                image->name -= bytes;
            for (std::int32_t index = 0; index < image->levelCount; ++index)
                if (image->levels[index].filename >= next)
                    image->levels[index].filename -= bytes;
        }
    }
    manager->entryCount -= bytes;
}

} // namespace

// @verified - d
HLib::Task *EnqueueModelManagerTask(std::uint8_t index) {
    const std::int8_t priority = static_cast<std::int8_t>(index);
    HLib::Task *const task = HLib::EnqueueTaskWithParameter(ModelManagerTaskCallback, priority, 2, sizeof(ModelManagerTaskData), TASK_MDLM);
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    manager->marker = MODEL_MANAGER_MARKER;
    manager->entryCount = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(manager->buckets) / sizeof(manager->buckets[0])); ++index)
        manager->buckets[index] = 0;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(sizeof(manager->entries) / sizeof(manager->entries[0])); ++index)
        manager->entries[index].flags = 0;
    manager->state = 0;
    HLib::SetupTimedTask(task, 0);
    return task;
}

// @verified - d
void ModelManagerTaskCallback(void *) {
    HLib::SetupTaskDelay(0);
}

// @verified - d
std::uint32_t ImageBucket(const char *name) {
    std::int32_t sum = 0;
    while (*name)
        sum += static_cast<unsigned char>(*name++);
    return sum % 19;
}

// @verified - d
char *StoreImageString(ModelManagerTaskData *manager, const char *text) {
    const std::size_t bytes = std::strlen(text) + 1;
    char *const destination = manager->managerState + manager->entryCount;
    std::memcpy(destination, text, bytes);
    manager->entryCount += bytes;
    return destination;
}

// @verified - d
ModelImageIndex *FindImageIndex(HLib::Task *task, const char *name) {
    PakGlobalData *package;
    if (GetPakLoadParams())
        package = pakLoadGetNext();
    else
        package = 0;
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    for (ModelImageIndex *image = manager->buckets[ImageBucket(name)]; image; image = image->next)
        if (!std::strcmp(image->name, name) && (!package || image->pak == package))
            return image;
    return 0;
}

// @verified - d
ModelImageIndex *AllocateModelImageIndex(HLib::Task *task, const char *name) {
    PakGlobalData *package;
    if (GetPakLoadParams())
        package = pakLoadGetNext();
    else
        package = 0;
    do {
        ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
        const std::uint32_t bucket = ImageBucket(name);
        for (ModelImageIndex *image = manager->buckets[bucket]; image; image = image->next)
            if (!std::strcmp(image->name, name) && (!package || package == image->pak))
                return image;
        ModelImageIndex *const image = static_cast<ModelImageIndex *>(HLib::AllocateTaskData(sizeof(ModelImageIndex), MEMORY_MAIX));
        image->name = StoreImageString(manager, name);
        image->levels = 0;
        image->levelCount = 0;
        image->referenceCount = 0;
        image->next = manager->buckets[bucket];
        manager->buckets[bucket] = image;
        return image;
    } while (0);
}

// @verified - d
void ReleasePackageModelImages(HLib::Task *task, PakGlobalData *package) {
    HLib::Task *const owner = task;
    if (!package)
        return;
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(owner));
    for (std::int32_t index = 0; index < 19; ++index) {
        ModelImageIndex *const bucket = manager->buckets[index];
        for (ModelImageIndex *image = bucket; image; image = image->next)
            if (image->pak == package)
                ReleaseImageIndex(owner, image->name);
    }
}

void ModelManagerNullsub0() {
}

// @verified - d
void BeginModelImageFileReads() {
    modelImageReadCount = 0;
    modelImageReads = 0;
}

// @verified - d
void QueueModelImageFileRead(MT7Model **destination, const char *directory, const char *filename) {
    ModelImageFileRead *read = reinterpret_cast<ModelImageFileRead *>(allocateInGlobalHeap(sizeof(ModelImageFileRead)));
    read->destination = destination;
    std::strcpy(read->directory, directory);
    std::strcpy(read->filename, filename);
    if (!modelImageReads) {
        modelImageReads = read;
        read = reinterpret_cast<ModelImageFileRead *>(allocateInGlobalHeap(sizeof(ModelImageFileRead)));
        SetTaskHeapData(read);
        modelImageReadStride = static_cast<std::int32_t>(reinterpret_cast<std::byte *>(read) - reinterpret_cast<std::byte *>(modelImageReads));
    }
    ++modelImageReadCount;
}

bool ModelImageReadComesAfter(const ModelImageFileRead *first, const ModelImageFileRead *second) {
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
    std::int8_t a, b;
    const char *left = first->directory, *right = second->directory;
    a = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*left++)));
    b = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*right++)));
    while (a && b && a == b) {
        a = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*left++)));
        b = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*right++)));
    }
    if (a > b)
        return true;
    if (a < b)
        return false;
    left = first->filename;
    right = second->filename;
    a = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*left)));
    b = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*right)));
    ++left;
    ++right;
    while (a && b && a == b) {
        a = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*left)));
        b = static_cast<std::int8_t>(toupper(static_cast<unsigned char>(*right)));
        ++left;
        ++right;
    }
    if (a > b)
        return true;
    return false;
#else
    const std::int32_t order = CompareCharacterTableTokens(first->directory, second->directory);
    return order ? order > 0 : CompareCharacterTableTokens(first->filename, second->filename) > 0;
#endif
}

// @verified - d
void FinishModelImageFileReads() {
    std::int32_t index;
    if (!modelImageReads || !modelImageReadCount)
        return;
    for (index = 1; index < modelImageReadCount; ++index) {
        ModelImageFileRead saved = *reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(modelImageReads) + index * modelImageReadStride);
        std::int32_t position;
        for (position = index - 1; position >= 0 &&
            ModelImageReadComesAfter(
                reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(modelImageReads) + position * modelImageReadStride), &saved);
            --position)
            *reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(modelImageReads) + (position + 1) * modelImageReadStride) =
                *reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(modelImageReads) + position * modelImageReadStride);
        *reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(modelImageReads) + (position + 1) * modelImageReadStride) = saved;
    }
    ModelImageFileRead *read = modelImageReads;
    ModelImageFileRead *previous;
    FileState *file = 0;
    void *allocation = 0;
    for (index = 0; index <= modelImageReadCount; ++index) {
        void *completed = 0;
        if (file) {
            FileStatAndProcessIfExists(file);
            CloseFile(file);
            completed = allocation;
            previous = read;
            read = reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(read) + modelImageReadStride);
        }
        if (completed)
            *previous->destination = InitializeTypedMT7Model(completed, read->filename);
        if (index < modelImageReadCount) {
            file = Archive_GetNextUnusedFileState();
            if (OpenFile(file, read->directory, read->filename)) {
                const std::uint32_t size = GetFileSize(file);
                allocation = HLib::AllocateTaskData(size, MEMORY_MODEL_FILE);
                CopyFileToHLibMemory(file, allocation, size);
            } else {
                file = 0;
                *read->destination = 0;
                read = reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(read) + modelImageReadStride);
            }
        }
    }
    for (index = 0; index < modelImageReadCount; ++index) {
        read = reinterpret_cast<ModelImageFileRead *>(reinterpret_cast<std::byte *>(read) - modelImageReadStride);
        SetTaskHeapData(read);
    }
    modelImageReadCount = 0;
    modelImageReads = 0;
}

// @verified - d
void AssignModelImageDefinition(HLib::Task *task, const CharacterTableArgument *arguments, std::int32_t, std::int32_t entry) {
    CharacterTableRecord record;
    char directory[32];
    const std::uint32_t *cursor;
    const std::uint32_t *end;
    std::int32_t count;
    ModelManagerTaskData *manager;
    PakGlobalData *package;
    ModelImageIndex *image;
    std::int32_t index;
    std::int32_t position;
    std::int32_t move;
    std::int32_t load;
    std::int32_t threshold;
    std::int32_t map;

    manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    package = GetPakLoadParams() ? pakLoadGetNext() : 0;
    image = AllocateModelImageIndex(task, arguments[0].value.string);
    if (image->levelCount > 0)
        return;
    ++arguments;
    cursor = arguments->source + 1;
    end = cursor + arguments->value.bits;
    count = 0;
    load = 1;
    // The task parameter is dead from here on and holds the explicit availability flag.
    task = 0;
    if (cursor < end) {
    parse:
        cursor = ParseCharacterTableRecord(&record, modelImageDefinitionTokens, cursor);
        switch (record.tokenIndex) {
        case ModelImageDefinitionCommand_Image:
            ++count;
            break;
        case ModelImageDefinitionCommand_Avail:
        case ModelImageDefinitionCommand_AvailExt:
            load = 0;
            for (index = 0; index < record.count; ++index) {
                switch (record.arguments[index].type) {
                case CharacterTableArgumentType_Integer:
                    if (record.arguments[index].value.integer == entry) {
                        load = 1;
                        task = reinterpret_cast<HLib::Task *>(1);
                    }
                    break;
                case CharacterTableArgumentType_Float:
                    if (record.arguments[index].value.floating == entry) {
                        load = 1;
                        task = reinterpret_cast<HLib::Task *>(1);
                    }
                    break;
                }
            }
            break;
        case ModelImageDefinitionCommand_NotAvail:
            load = 1;
            task = reinterpret_cast<HLib::Task *>(1);
            for (index = 0; index < record.count; ++index) {
                switch (record.arguments[index].type) {
                case CharacterTableArgumentType_Integer:
                    if (record.arguments[index].value.integer == entry) {
                        load = 0;
                        task = 0;
                    }
                    break;
                case CharacterTableArgumentType_Float:
                    if (record.arguments[index].value.floating == entry) {
                        load = 0;
                        task = 0;
                    }
                    break;
                }
            }
            break;
        case ModelImageDefinitionCommand_Dynamic:
            load = 2;
            break;
        }
        if (cursor < end)
            goto parse;
    }
    if (!count)
        return;
    if (task)
        load = 1;
    cursor = arguments->source + 1;
    image->levelCount = count;
    image->levels = static_cast<ModelImageLevel *>(HLib::AllocateTaskData(count * sizeof(ModelImageLevel), MEMORY_MAIL));
    image->pak = package;
    for (index = 0; index < count; ++index) {
        cursor = ParseCharacterTableRecord(&record, modelImageDefinitionTokens, cursor);
        threshold = static_cast<std::int32_t>(record.arguments[0].value.floating);
        for (position = 0; position < index && image->levels[position].threshold < threshold; ++position)
            ;
        for (move = index; move > position; --move)
            image->levels[move] = image->levels[move - 1];
        image->levels[position].threshold = threshold;
        if (*record.arguments[1].value.string == '@') {
            image->levels[position].filename = StoreImageString(manager, record.arguments[1].value.string + 1);
            std::sprintf(directory, "scene/%02d/model/chara", static_cast<int>(g_actNumber));
        } else if (*record.arguments[1].value.string == '$') {
            map = static_cast<std::int32_t>(g_currentMap);
            image->levels[position].filename = StoreImageString(manager, record.arguments[1].value.string + 1);
            std::sprintf(directory, "scene/%02d/%c%c%c%c", static_cast<int>(g_actNumber), static_cast<std::int8_t>(map),
                static_cast<std::int8_t>(static_cast<std::uint32_t>(map) >> 8), static_cast<std::int8_t>((map << 8) >> 24),
                static_cast<std::int8_t>(map >> 24));
        } else {
            image->levels[position].filename = StoreImageString(manager, record.arguments[1].value.string);
            if (record.arguments[1].value.string[3] == '_' && record.arguments[1].value.string[4] == 'U' &&
                (record.arguments[1].value.string[5] == 'L' || record.arguments[1].value.string[5] == 'R') && record.arguments[1].value.string[6] == '.')
                std::sprintf(directory, "scene/%02d/model/hand", static_cast<int>(g_actNumber));
            else
                std::sprintf(directory, "scene/%02d/model/chara", static_cast<int>(g_actNumber));
        }
        switch (load) {
        case 1:
            image->levels[position].model = reinterpret_cast<MT7Model *(*)(const char *)>(FindPreloadedCharacterImageNullsub)(image->levels[position].filename);
            if (!image->levels[position].model) {
                if (!package) {
                    image->levels[position].model = 0;
                    QueueModelImageFileRead(&image->levels[position].model, directory, image->levels[position].filename);
                } else
                    image->levels[position].model = LoadGameModel(package, image->levels[position].filename);
            }
            break;
        case 0:
        case 2:
            image->levels[position].model = 0;
            break;
        }
    }
}

void ReleaseImageIndex(HLib::Task *task, const char *name) {
    std::int32_t index;
    ModelImageIndex *const image = FindImageIndex(task, name);
    if (!image)
        return;
    for (index = 0; index < image->levelCount; ++index) {
        ModelImageLevel *const levels = image->levels;
        ModelImageLevel *const level = &levels[index];
        if (level->model) {
            if (!FindRetainingModelPackage(level->model))
                ReleaseMT7Model(levels[index].model);
            level->model = 0;
        }
    }
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    RemoveImageString(manager, image->name);
    for (index = 0; index < image->levelCount; ++index)
        RemoveImageString(manager, image->levels[index].filename);
    for (std::size_t rangeIndex113 = 0; rangeIndex113 < sizeof(manager->buckets) / sizeof((manager->buckets)[0]); ++rangeIndex113) {
        ModelImageIndex *&bucket = (manager->buckets)[rangeIndex113];

        ModelImageIndex **link = &bucket;
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

void ModelManagerNullsub1() {
}

// @verified - d
void SetImageDistance(HLib::Task *, ModelManagerEntry *entry, std::uint32_t priority) {
    entry->distancePriority = priority;
}

// @verified - d
void UpdateImageDistance(HLib::Task *task, ModelManagerEntry *entry, Vector3 *position) {
    if (position) {
        Vector3 camera;
        camCtrlGetNextCamPos(&camera);
        position->x -= camera.x;
        position->y -= camera.y;
        position->z -= camera.z;
        position->x *= position->x;
        position->y *= position->y;
        position->z *= position->z;
        float distance = position->x + position->y + position->z;
        if (std::fabs(distance) < 1.1920928955078125e-07f) {
            SetImageDistance(task, entry, 0);
        } else {
            distance = std::sqrt(distance);
            distance = 1000000.0f / distance;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
            SetImageDistance(task, entry, distance > 2147483648.0f ? 0x7fffffffU : static_cast<std::uint32_t>(static_cast<std::int32_t>(distance)));
#else
            std::int32_t value;
            asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(value) : "f"(distance) : "fpul");
            SetImageDistance(task, entry, distance > 2147483648.0f ? 0x7fffffffU : static_cast<std::uint32_t>(value));
#endif
        }
    } else {
        SetImageDistance(task, entry, 0);
    }
}

// @verified - d
ModelManagerEntry *AllocateImageEntry(HLib::Task *task, const char *name) {
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    ModelManagerEntry *entry = manager->entries;
    std::int32_t index = 0;
    if (entry->flags & MIEF_InUse) {
        do {
            ++index;
            ++entry;
        } while (index <= 255 && (entry->flags & MIEF_InUse));
    }
    ModelImageIndex *image = FindImageIndex(task, name);
    if (!image)
        image = AllocateModelImageIndex(task, name);
    entry->flags = MIEF_InUse;
    entry->distancePriority = 0;
    entry->image = image;
    ++image->referenceCount;
    entry->instance = 0;
    entry->source = 0;
    entry->childIndex = 0;
    entry->groupId = -1;
    entry->variant = 0;
    return entry;
}

// @verified - d
void ReleaseImageEntry(HLib::Task *task, ModelManagerEntry *entry) {
    do {
        HLib::GetTaskParameter(task);
    } while (0);
    if (entry->image) {
        --entry->image->referenceCount;
        if (!entry->image->referenceCount)
            ReleaseImageIndex(task, entry->image->name);
    }
    if (entry->instance) {
        ReleaseMT7ModelNodeTreeClone(entry->instance);
        entry->instance = 0;
    }
    entry->flags = 0;
}

// @verified - d
MT7Model *SelectImageEntrySource(HLib::Task *, const ModelManagerEntry *entry) {
    ModelImageIndex *const image = entry->image;
    if (!image)
        return 0;
    ModelImageLevel *candidate;
    ModelImageLevel *selected = candidate = image->levels;
    std::int32_t index = 0;
    if (!selected)
        return 0;
    if (index < image->levelCount && candidate->model && static_cast<std::int32_t>(entry->distancePriority) >= candidate->threshold) {
        do {
            selected = candidate;
            ++index;
            ++candidate;
        } while (index < image->levelCount && candidate->model && static_cast<std::int32_t>(entry->distancePriority) >= candidate->threshold);
    }
    return selected->model;
}

// @verified - d
MT7Node *ResolveImageEntry(HLib::Task *task, ModelManagerEntry *entry) {
    MT7Model *const source = SelectImageEntrySource(task, entry);
    if ((entry->flags & MIEF_SelectionDirty) || source != entry->source) {
        if (entry->instance) {
            ReleaseMT7ModelNodeTreeClone(entry->instance);
            entry->instance = 0;
        }
        if (source)
            entry->instance = CloneImageSelection(source, entry->childIndex, entry->groupId, entry->variant);
        entry->source = source;
        entry->flags &= ~MIEF_SelectionDirty;
    }
    return entry->instance;
}

// @verified - d
void SetImageEntrySelection(ModelManagerEntry *entry, std::uint32_t child, std::int32_t group, std::uint32_t variant) {
    if (entry->childIndex != child || entry->groupId != group || entry->variant != variant) {
        entry->childIndex = child;
        entry->groupId = group;
        entry->variant = variant;
        entry->flags |= MIEF_SelectionDirty;
    }
}

// @verified - d
bool AllocateModelImageLevel(HLib::Task *task, const char *name, const char *directory, const char *filename) {
    ModelManagerTaskData *const manager = static_cast<ModelManagerTaskData *>(HLib::GetTaskParameter(task));
    ModelImageIndex *const image = AllocateModelImageIndex(task, name);
    if (!image->levelCount) {
        image->levelCount = 1;
        image->levels = static_cast<ModelImageLevel *>(HLib::AllocateTaskData(sizeof(ModelImageLevel), MEMORY_MAIL));
        image->levels->filename = StoreImageString(manager, filename);
    }
    image->levels->threshold = 1;
    image->levels->model = LoadMT7Model(directory, image->levels->filename);
    return true;
}

// @verified - d
void InitializeImageModule(ImageModuleComponent *image) {
    image->manager = 0;
    image->entries = 0;
    image->count = 0;
    image->selected = 0;
    image->previous = 0;
    image->forceFirst = 0;
    *reinterpret_cast<std::uint8_t *>(&image->retainedAnimationState) = 0;
    image->remainingFrames = 0;
    image->animationFlags = 0;
    image->animationGroup = 0;
    image->frameStep = 0.0f;
    image->frameAccumulator = 0.0f;
}

// @verified - d
MT7Node *ResolveImageModule(ImageModuleComponent *image, Vector3 *position) {
    if (!image->entries)
        return 0;
    if (image->forceFirst) {
        image->selected = 0;
        image->entries[image->selected]->childIndex = 0;
        image->entries[image->selected]->groupId = 0;
        image->entries[image->selected]->variant = 0;
    }
    for (std::int32_t index = 0; index < image->count; ++index)
        UpdateImageDistance(image->manager, image->entries[index], position);
    MT7Node *const model = ResolveImageEntry(image->manager, image->entries[image->selected]);
    if (!image->entries[image->selected]->source)
        image->sourceType = 0;
    else
        image->sourceType = static_cast<std::int16_t>(image->entries[image->selected]->source->entityType);
    return model;
}

// @verified - d
void ReleaseImageModule(ImageModuleComponent *image) {
    if (!image->entries)
        return;
    for (std::int32_t index = 0; index < image->count; ++index)
        ReleaseImageEntry(image->manager, image->entries[index]);
    HLib::FreeTaskData(image->entries);
    image->entries = 0;
    image->count = 0;
    image->selected = 0;
    image->previous = 0;
    image->forceFirst = 0;
    *reinterpret_cast<std::uint8_t *>(&image->retainedAnimationState) = 0;
    image->remainingFrames = 0;
    image->frameStep = 0.0f;
    image->frameAccumulator = 0.0f;
}

// @verified - d
void ConfigureImageModuleVariants(ImageModuleComponent *image, HLib::Task *manager, const char *name, std::int32_t count) {
    ReleaseImageModule(image);
    image->manager = manager;
    if (!count) {
        image->count = 1;
        image->entries = static_cast<ModelManagerEntry **>(HLib::AllocateTaskData(sizeof(ModelManagerEntry *), MEMORY_HIMG));
        image->entries[0] = AllocateImageEntry(manager, name);
        SetImageEntrySelection(image->entries[0], 0, 63, 0);
        SetImageDistance(manager, image->entries[0], 1);
    } else {
        image->count = static_cast<std::uint8_t>(count);
        image->entries = static_cast<ModelManagerEntry **>(HLib::AllocateTaskData(static_cast<std::uint32_t>(count) * 4U, MEMORY_HIMG));
        for (std::int32_t index = 0; index < count; ++index) {
            image->entries[index] = AllocateImageEntry(manager, name);
            SetImageEntrySelection(image->entries[index], static_cast<std::uint32_t>(index + 1), 63, 0);
            SetImageDistance(manager, image->entries[index], 1);
        }
    }
    image->selected = 0;
}

void SetImageModuleSelection(ImageModuleComponent *image, std::int32_t selected) {
    image->selected = selected;
}

void SetImageModuleEntrySelection(ImageModuleComponent *image, std::int32_t entry, std::uint32_t child, std::int32_t group, std::uint32_t variant) {
    SetImageEntrySelection(image->entries[entry], child, group, variant);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void ConfigureImageModuleAnimation(ImageModuleComponent *image, std::int32_t mode, std::int32_t remaining, float step) {
#else
void ConfigureImageModuleAnimation(ImageModuleComponent *image, std::uint8_t mode, std::int32_t remaining, float step) {
#endif
    image->forceFirst = mode;
    *reinterpret_cast<std::uint8_t *>(&image->retainedAnimationState) = 0;
    image->remainingFrames = static_cast<std::uint32_t>(remaining);
    image->animationFlags = 0;
    image->animationGroup = 0;
    image->frameStep = step;
    image->frameAccumulator = 0.0f;
}

// @verified - d
void SetupImageModuleHandlers(Character *character) {
    ImageModuleComponent *const image =
        static_cast<ImageModuleComponent *>(AllocateModelComponent(character, CHARACTER_IMAGE, sizeof(ImageModuleComponent), ImageModuleUpdate));
    InitializeImageModule(image);
    SetModelComponentCleanup(character, CHARACTER_IMAGE, ImageModuleCleanup);
}

// @verified - d
void PublishImageModule(Character *character, ImageModuleComponent *image) {
    Vector3 position;
    Vector3 *visiblePosition;
    if (IsOfThisVisibilityType(character, CVM_All)) {
        visiblePosition = &position;
        GetCharacterWorldPosition(character, &position);
    } else
        visiblePosition = 0;
    character->model = ResolveImageModule(image, visiblePosition);
    mtwkSetSequenceFlags(character);
}

// @verified - d
void ImageModuleUpdate(Character *character, void *data) {
    Vector3 position;
    Vector3 *visiblePosition;
    if (IsOfThisVisibilityType(character, CVM_All)) {
        visiblePosition = &position;
        GetCharacterWorldPosition(character, &position);
    } else
        visiblePosition = 0;
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(data);
    MT7Node *const model = ResolveImageModule(image, visiblePosition);
    if (image->previous != image->selected) {
        character->model = model;
        mtwkSetSequenceFlags(character);
    }
    image->previous = image->selected;
}

// @verified - d
void ImageModuleCleanup(Character *character, void *data) {
    ReleaseImageModule(static_cast<ImageModuleComponent *>(data));
    character->model = 0;
}

// @verified - d
void AssignCharacterTableImage(Character *character, HLib::Task *manager, const CharacterTableArgument *arguments, std::int32_t count) {
    std::int32_t variants = 0;
    std::uint32_t group = 0;
    std::int32_t remaining = -1;
    float step = 1.0f;
    std::uint8_t mode = 0;
    if (count == 2) {
        const std::uint32_t *cursor = arguments[1].source + 1;
        const std::uint32_t *const end = cursor + arguments[1].value.bits;
        while (cursor < end) {
            CharacterTableRecord record;
            cursor = ParseCharacterTableRecord(&record, modelImageAnimationTokens, cursor);
            switch (record.tokenIndex) {
            case 0: {
                const float value = record.arguments[0].value.floating;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
                variants = static_cast<std::int32_t>(value);
#else
                asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(variants) : "f"(value) : "fpul");
#endif
                group = static_cast<std::uint32_t>(variants);
                break;
            }
            case 1: {
                const float value = record.arguments[0].value.floating;
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
                remaining = static_cast<std::int32_t>(value);
#else
                asm volatile("ftrc %1,FPUL\n\tsts FPUL,%0" : "=r"(remaining) : "f"(value) : "fpul");
#endif
                break;
            }
            case 2:
                step = record.arguments[0].value.floating;
                if (step < 0.1f)
                    step = 1.0f;
                break;
            case 3:
                if (!CompareCharacterTableTokens(record.arguments[0].value.string, "NONE"))
                    mode = 0;
                else if (!CompareCharacterTableTokens(record.arguments[0].value.string, "REPEAT"))
                    mode = 1;
                else
                    mode = 2;
                break;
            }
        }
    }
    AssignImageModuleParameters(character, mode, remaining, step, group);
    SetupImageModuleVariants(character, manager, arguments[0].value.string, variants);
}

// @verified - d
void ResetImageModule(Character *character) {
    ClearCharacterFacialModel(character);
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    ImageModuleCleanup(character, image);
}

// @verified - d
void SetupImageModuleVariants(Character *character, HLib::Task *manager, const char *name, std::int32_t count) {
    ClearCharacterFacialModel(character);
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    ConfigureImageModuleVariants(image, manager, name, count);
    PublishImageModule(character, image);
    BindCharacterFacialModel(character);
}

void SetupImageModule(Character *character, HLib::Task *manager, const char *name) {
    SetupImageModuleVariants(character, manager, name, 0);
}

void SetCharacterImageSelection(Character *character, std::int32_t selected) {
    SetImageModuleSelection(static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE)), selected);
}

void SetCharacterImageEntrySelection(Character *character, std::int32_t entry, std::uint32_t child, std::int32_t group, std::uint32_t variant) {
    SetImageModuleEntrySelection(static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE)), entry, child, group, variant);
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void AssignImageModuleParameters(Character *character, std::int32_t mode, std::int32_t remaining, float step, std::uint32_t group) {
#else
void AssignImageModuleParameters(Character *character, std::uint8_t mode, std::int32_t remaining, float step, std::uint32_t group) {
#endif
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    ConfigureImageModuleAnimation(image, mode, remaining, step);
    image->animationGroup = group;
}

// @verified - d
std::int32_t GetCharacterImageSourceType(const Character *character) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    if (image)
        return image->sourceType;
    return 0;
}

// @verified - d
MT7Model *GetImageModuleSource(const Character *character) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    if (image) {
        const ModelManagerEntry *const entry = image->entries[image->selected];
        if (entry)
            return entry->source;
    }
    return 0;
}

// @verified - d
const void *GetImageModuleSelectedTransform(const Character *character) {
    const MT7Model *const source = GetImageModuleSource(character);
    return source && source->root ? source->root->transformOverride : 0;
}

// @verified - d
void InitRigType(Character *character, const char *name) {
    MT7Model *const source = GetImageModuleSource(character);
    std::uint32_t index;
    for (index = 0; index < 21; ++index)
        if (!std::strcmp(name, modelRigNames[index]))
            break;
    if (index < 21)
        source->entityType = static_cast<std::int32_t>(index);
}

// @verified - d
void IMGM_AssignMTWK(Character *character) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    if (image->forceFirst) {
        charaAssignCoreCallback(character, DrawImageCharacterCore);
        imgmAssignUpdateCallback(character, imgmDrawModelNode);
    }
}

// @verified - d
void DrawImageCharacterCore(Character *character) {
    ImageModuleComponent *const image = static_cast<ImageModuleComponent *>(charFindComp(character, CHARACTER_IMAGE));
    std::uint32_t flags = image->animationFlags;
    flags |= image->animationGroup << 8U;
    if (image->forceFirst == 2)
        flags |= 0x2000U;
    else
        flags |= 0x1000U;
    flags |= 0x10000U;
    const float step = image->frameStep;
    float accumulator = image->frameAccumulator;
    if (!(g_loadingFlag & 2U) && image->remainingFrames) {
        for (accumulator += 1.0f; accumulator >= 1.0f; accumulator -= step) {
            flags = AdvanceImageModelFrame(character->model, flags);
            if (!(static_cast<std::uint8_t>(flags) | (flags & 0x100000U)) && image->remainingFrames)
                --image->remainingFrames;
        }
    }
    do {
        DrawImageModelSelection(character, flags);
    } while (0);
    image->animationFlags = flags;
    image->frameAccumulator = accumulator;
}

} // namespace s2
} // namespace shenmue

#endif
