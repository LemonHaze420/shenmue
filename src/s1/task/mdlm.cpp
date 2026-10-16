#include "s1/task/mdlm.hpp"

#include "s1/task/char.hpp"
#include "s1/task/pack.hpp"
#include "s1/task/came.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace shenmue::s1 {

namespace {

constexpr std::uint32_t kMdlmTag = SHEN_NAME(MMTW);
constexpr std::uint32_t kRecordToken = SHEN_NAME(MAIX);
constexpr std::uint32_t kImageListToken = SHEN_NAME(MAIL);
constexpr std::uint32_t kEntryOwnsModel = 1;
constexpr std::uint32_t kImageSlotInUse = 1;

std::size_t MDLM_HashName(const char *name) {
    std::uint32_t hash = 0;
    while (name && *name) {
        hash += static_cast<unsigned char>(*name++);
    }
    return hash % 19U;
}

void MDLM_NameToString(shen_name name, char (&text)[5]) {
    std::memcpy(text, &name.name, sizeof(name.name));
    text[4] = '\0';
}

MdlmRecord *MDLM_FindRecordInternal(MdlmTaskData *data, const char *name, PackSlot *scope, bool explicitScope) {
    if (!data || !name || !*name)
        return nullptr;
    if (!explicitScope && g_packTaskData && g_packTaskData->chidParseGuard) {
        scope = g_packTaskData->activeReader;
        explicitScope = scope != nullptr;
    }
    for (MdlmRecord *record = data->buckets[MDLM_HashName(name)]; record; record = record->next) {
        if (std::strcmp(record->name, name) == 0 && (!explicitScope || record->ownerPack == scope)) {
            return record;
        }
    }
    return nullptr;
}

char *MDLM_StoreString(MdlmTaskData *data, const char *text) {
    if (!data || !text)
        return nullptr;
    const std::size_t length = std::strlen(text) + 1;
    if (length > sizeof(data->stringPool) - data->stringBytes) {
        return nullptr;
    }
    char *const destination = data->stringPool + data->stringBytes;
    std::memcpy(destination, text, length);
    data->stringBytes += static_cast<std::uint32_t>(length);
    return destination;
}

void MDLM_AdjustStringPointers(MdlmTaskData *data, const char *removed, std::size_t length) {
    for (MdlmRecord *&bucket : data->buckets) {
        for (MdlmRecord *record = bucket; record; record = record->next) {
            if (record->name > removed)
                record->name -= length;
            for (std::int32_t index = 0; index < record->imageCount; ++index) {
                if (record->images[index].imageName > removed) {
                    record->images[index].imageName -= length;
                }
            }
        }
    }
}

void MDLM_ReleaseString(MdlmTaskData *data, char *text) {
    if (!data || !text || text < data->stringPool || text >= data->stringPool + data->stringBytes) {
        return;
    }
    const std::size_t length = std::strlen(text) + 1;
    const std::size_t offset = static_cast<std::size_t>(text - data->stringPool);
    if (offset + length > data->stringBytes)
        return;
    const std::size_t trailing = data->stringBytes - offset - length;
    std::memmove(text, text + length, trailing);
    data->stringBytes -= static_cast<std::uint32_t>(length);
    MDLM_AdjustStringPointers(data, text, length);
}

MdlmRecord *MDLM_CreateRecord(MdlmTaskData *data, const char *name) {
    auto *const record = static_cast<MdlmRecord *>(HLib_AllocHeapMemBlock(sizeof(MdlmRecord), kRecordToken));
    if (!record)
        return nullptr;
    record->name = MDLM_StoreString(data, name);
    if (!record->name) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(record), 0);
        return nullptr;
    }
    record->images = nullptr;
    record->imageCount = 0;
    record->referenceCount = 0;
    const std::size_t bucket = MDLM_HashName(name);
    record->next = data->buckets[bucket];
    data->buckets[bucket] = record;
    return record;
}

bool MDLM_OwnedModelReferencedElsewhere(MdlmTaskData *data, const MdlmRecord *ignoredRecord, const MdlmImageEntry *ignoredEntry, PackModel *model) {
    for (MdlmRecord *bucket : data->buckets) {
        for (MdlmRecord *record = bucket; record; record = record->next) {
            for (std::int32_t index = 0; index < record->imageCount; ++index) {
                const MdlmImageEntry *const entry = &record->images[index];
                if (record == ignoredRecord && entry == ignoredEntry)
                    continue;
                if (entry->model == model && (entry->flags & kEntryOwnsModel) != 0) {
                    return true;
                }
            }
        }
    }
    return false;
}

void MDLM_ReleaseOwnedEntryModel(MdlmTaskData *data, MdlmRecord *record, MdlmImageEntry *entry) {
    if (!entry || !entry->model || (entry->flags & kEntryOwnsModel) == 0) {
        return;
    }
    if (!MDLM_OwnedModelReferencedElsewhere(data, record, entry, entry->model)) {
        FreePVR(entry->model);
    }
}

void MDLM_RemoveRecord(MdlmTaskData *data, MdlmRecord *record);

void MDLM_ReleaseSlot(MdlmTaskData *data, MdlmImageSlot *slot) {
    if (!data || !slot || (slot->flags & kImageSlotInUse) == 0)
        return;
    MdlmRecord *const record = slot->record;
    if (record)
        --record->referenceCount;
    if (record && record->referenceCount == 0 && record->ownerPack && g_packTaskData && g_packTaskData->chidParseGuard) {
        MdlmRecord *const scoped = MDLM_FindRecordInternal(data, record->name, g_packTaskData->activeReader, g_packTaskData->activeReader != nullptr);
        if (scoped)
            MDLM_RemoveRecord(data, scoped);
    }
    HMDL_FreeAssignedModel(slot->activeModel);
    slot->activeModel = nullptr;
    slot->flags = 0;
}

void MDLM_RemoveRecord(MdlmTaskData *data, MdlmRecord *record) {
    if (!data || !record)
        return;
    for (std::int32_t index = 0; index < record->imageCount; ++index) {
        MdlmImageEntry &entry = record->images[index];
        if (entry.model && !PACK_FindModelOwner(entry.model))
            FreePVR(entry.model);
        entry.model = nullptr;
    }
    MDLM_ReleaseString(data, record->name);
    for (std::int32_t index = 0; index < record->imageCount; ++index)
        MDLM_ReleaseString(data, record->images[index].imageName);
    for (MdlmRecord *&bucket : data->buckets) {
        MdlmRecord **link = &bucket;
        while (*link && *link != record)
            link = &(*link)->next;
        if (*link == record) {
            *link = record->next;
            break;
        }
    }
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(record->images), 0);
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(record), 0);
}

MdlmImageSlot *MDLM_AcquireSlot(MdlmTaskData *data, MdlmRecord *record) {
    for (MdlmImageSlot &slot : data->imageSlots) {
        if ((slot.flags & kImageSlotInUse) != 0)
            continue;
        slot.flags = kImageSlotInUse;
        slot.selectionMetric = 0;
        slot.record = record;
        slot.activeModel = nullptr;
        slot.selectedModel = nullptr;
        slot.childIndex = 0;
        slot.nodeKind = -1;
        slot.occurrence = 0;
        record->referenceCount++;
        return &slot;
    }
    return nullptr;
}

MdlmImageEntry *MDLM_SelectImage(MdlmRecord *record, std::int32_t selectionMetric) {
    if (!record || !record->images || record->imageCount <= 0)
        return nullptr;
    MdlmImageEntry *selected = record->images;
    for (std::int32_t index = 0; index < record->imageCount; ++index) {
        MdlmImageEntry *const entry = record->images + index;
        if (!entry->model || selectionMetric < entry->selector)
            break;
        selected = entry;
    }
    return selected;
}

} // namespace

HLib::Task *g_mdlmTask = nullptr;

MdlmTaskData *MDLM_GetTaskData(HLib::Task *task) {
    HLib::Task *const owner = task ? task : g_mdlmTask;
    return owner ? reinterpret_cast<MdlmTaskData *>(HLib_GetTaskDataForTask(owner)) : nullptr;
}

HLib::Task *MDLM_Init() {
    HLib::Task *task = HLib_EnqueueTaskWithoutParameter(MDLM_Callback, 3, 2, SHEN_NAME(MDLM));
    MdlmTaskData *data = nullptr;
    if (task && !task->task_data_ptr) {
        data = static_cast<MdlmTaskData *>(HLib_AllocHeapMemBlock(sizeof(MdlmTaskData), SHEN_NAME(TASK)));
        if (data)
            HLib_EnableTokenCheck(data);
        task->task_data_ptr = reinterpret_cast<HLib::TaskData *>(data);
    }
    if (!task || !data) {
        if (task) {
            task->initCallbackFnPtr = nullptr;
            task->mainTaskCallbackFnPtr = std::bit_cast<HLib::TaskCallback>(&HLib_DestroyCurrentTask);
            task->autoInitTaskCallbackFnPtr = nullptr;
        }
        task = nullptr;
        data = nullptr;
    }

    data->tag = kMdlmTag;
    data->field04 = 0;
    data->stringBytes = 0;
    for (MdlmRecord *&bucket : data->buckets)
        bucket = nullptr;
    for (MdlmImageSlot &slot : data->imageSlots)
        slot.flags = 0;
    g_mdlmTask = task;

    if (task) {
        if (!task->initCallbackFnPtr) {
            task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
            task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
            task->mainExecutionIterations = 0;
        }
        task->mainExecutionIterations = 0;
    }
    return task;
}

void MDLM_Callback(HLib::TaskData *) {
    HLib::Task *const task = CurrentTask;
    if (!task)
        return;
    if (!task->initCallbackFnPtr) {
        task->initCallbackFnPtr = task->mainTaskCallbackFnPtr;
        task->mainTaskCallbackFnPtr = HLib_SetTaskCallbackToInitCallback;
    }
    task->mainExecutionIterations = 0;
}

MdlmRecord *MDLM_StoreCharacterName(HLib::Task *owner, const char *name) {
    MdlmTaskData *const data = reinterpret_cast<MdlmTaskData *>(HLib_GetTaskDataForTask(owner));
    if (!data || !name)
        return nullptr;
    PackSlot *const scope = g_packTaskData && g_packTaskData->chidParseGuard ? g_packTaskData->activeReader : nullptr;
    MdlmRecord *const record = MDLM_FindRecordInternal(data, name, scope, scope != nullptr);
    return record ? record : MDLM_CreateRecord(data, name);
}

MdlmRecord *MDLM_DefineImages(const char *name, std::int32_t imageCount, PackSlot *ownerPack) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    if (!data || !name || imageCount <= 0)
        return nullptr;
    MdlmRecord *record = MDLM_FindRecordInternal(data, name, ownerPack, ownerPack != nullptr);
    if (record && record->imageCount > 0)
        return nullptr;
    if (!record)
        record = MDLM_CreateRecord(data, name);
    if (!record)
        return nullptr;
    auto *const entries =
        static_cast<MdlmImageEntry *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(imageCount) * sizeof(MdlmImageEntry), kImageListToken));
    if (!entries)
        return nullptr;
    std::memset(entries, 0, static_cast<std::size_t>(imageCount) * sizeof(*entries));
    record->images = entries;
    record->imageCount = imageCount;
    record->ownerPack = ownerPack;
    return record;
}

MdlmImageEntry *MDLM_InsertImageDefinition(MdlmRecord *record, std::int32_t populatedCount, const char *imageName, std::int32_t selector) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    if (!data || !record || !record->images || populatedCount < 0 || populatedCount >= record->imageCount) {
        return nullptr;
    }
    char *const storedName = MDLM_StoreString(data, imageName);
    if (!storedName)
        return nullptr;
    std::int32_t index = 0;
    while (index < populatedCount && record->images[index].selector < selector) {
        ++index;
    }
    std::memmove(record->images + index + 1, record->images + index, static_cast<std::size_t>(populatedCount - index) * sizeof(MdlmImageEntry));
    MdlmImageEntry *const entry = record->images + index;
    *entry = {nullptr, storedName, selector, 0};
    return entry;
}

void MDLM_PublishImage(MdlmImageEntry *entry, PackModel *model, bool ownsStandaloneModel) {
    if (!entry)
        return;
    entry->model = model;
    entry->flags = ownsStandaloneModel ? kEntryOwnsModel : 0;
}

const char *MDLM_FindModelImageName(PackModel *model) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    if (!data || !model)
        return nullptr;
    PackSlot *const scope = g_packTaskData && g_packTaskData->chidParseGuard ? g_packTaskData->activeReader : nullptr;
    for (MdlmRecord *bucket : data->buckets) {
        for (MdlmRecord *record = bucket; record; record = record->next) {
            if (scope && record->ownerPack != scope)
                continue;
            for (std::int32_t index = 0; index < record->imageCount; ++index) {
                if (record->images[index].model == model)
                    return record->name;
            }
        }
    }
    return nullptr;
}

MdlmImageSlot *MDLM_AcquireImageSlot(HLib::Task *owner, const char *name) {
    MdlmTaskData *const data = reinterpret_cast<MdlmTaskData *>(HLib_GetTaskDataForTask(owner));
    MdlmRecord *const record = MDLM_StoreCharacterName(owner, name);
    return record ? MDLM_AcquireSlot(data, record) : nullptr;
}

void MDLM_ReleaseImageSlot(HLib::Task *owner, MdlmImageSlot *slot) {
    MDLM_ReleaseSlot(reinterpret_cast<MdlmTaskData *>(HLib_GetTaskDataForTask(owner)), slot);
}

void MDLM_UpdateImageDistance(MdlmImageSlot *slot, Vector3 *position) {
    if (!slot)
        return;
    slot->selectionMetric = 0;
    if (!position)
        return;
    const float x = position->x - g_CameraEyePos.x;
    const float y = position->y - g_CameraEyePos.y;
    const float z = position->z - g_CameraEyePos.z;
    position->x = x * x;
    position->y = y * y;
    position->z = z * z;
    const float squared = (position->y + position->x) + position->z;
    if (std::fabs(squared) < std::numeric_limits<float>::epsilon())
        return;
    const float metric = static_cast<float>(1000000.0 / static_cast<double>(std::sqrt(squared)));
    if (static_cast<double>(metric) > 2147483647.0) {
        slot->selectionMetric = std::numeric_limits<std::int32_t>::max();
    } else if (std::isnan(metric)) {
        slot->selectionMetric = std::numeric_limits<std::int32_t>::min();
    } else {
        slot->selectionMetric = static_cast<std::int32_t>(metric);
    }
}

void *MDLM_CreateAssignedImage(MdlmImageSlot *slot) {
    if (!slot || (slot->flags & kImageSlotInUse) == 0)
        return nullptr;
    MdlmImageEntry *const entry = MDLM_SelectImage(slot->record, slot->selectionMetric);
    PackModel *const source = entry ? entry->model : nullptr;
    if ((slot->flags & 2U) != 0 || source != slot->selectedModel) {
        HMDL_FreeAssignedModel(slot->activeModel);
        slot->activeModel = source ? hmdlAssignFor(source, slot->childIndex, slot->nodeKind, slot->occurrence) : nullptr;
        slot->flags &= ~2U;
        slot->selectedModel = source;
    }
    return slot->activeModel;
}

bool MDLM_RegisterImage(const char *chidName, const char *imageName, PackModel *model, std::int32_t selector, PackSlot *ownerPack, bool ownsStandaloneModel) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    if (!data || !chidName || !*chidName || !imageName || !*imageName || !model || !model->model) {
        return false;
    }
    MdlmRecord *record = MDLM_FindRecordInternal(data, chidName, ownerPack, ownerPack != nullptr);
    if (!record) {
        record = MDLM_CreateRecord(data, chidName);
        if (!record)
            return false;
        record->ownerPack = ownerPack;
    }

    for (std::int32_t index = 0; index < record->imageCount; ++index) {
        MdlmImageEntry &entry = record->images[index];
        if (entry.selector == selector && std::strcmp(entry.imageName, imageName) == 0) {
            if (entry.model != model) {
                MDLM_ReleaseOwnedEntryModel(data, record, &entry);
            }
            entry.model = model;
            entry.flags = ownsStandaloneModel ? kEntryOwnsModel : 0;
            return true;
        }
    }

    const std::size_t newCount = static_cast<std::size_t>(record->imageCount) + 1;
    auto *const entries = static_cast<MdlmImageEntry *>(HLib_AllocHeapMemBlock(static_cast<std::int64_t>(newCount * sizeof(MdlmImageEntry)), kImageListToken));
    if (!entries)
        return false;
    if (record->images) {
        std::memcpy(entries, record->images, static_cast<std::size_t>(record->imageCount) * sizeof(MdlmImageEntry));
    }
    char *const storedName = MDLM_StoreString(data, imageName);
    if (!storedName) {
        HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(entries), 0);
        return false;
    }
    entries[record->imageCount] = {model, storedName, selector, ownsStandaloneModel ? kEntryOwnsModel : 0};
    std::stable_sort(entries, entries + newCount, [](const MdlmImageEntry &left, const MdlmImageEntry &right) { return left.selector < right.selector; });
    HLib_FreeTaskData(reinterpret_cast<HLib::TaskData *>(record->images), 0);
    record->images = entries;
    record->imageCount = static_cast<std::int32_t>(newCount);
    if (!record->ownerPack)
        record->ownerPack = ownerPack;
    return true;
}

PackModel *MDLM_FindImage(const char *chidName, std::int32_t selectionMetric, PackSlot *scope) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    MdlmRecord *const record = MDLM_FindRecordInternal(data, chidName, scope, scope != nullptr);
    MdlmImageEntry *const entry = MDLM_SelectImage(record, selectionMetric);
    return entry ? entry->model : nullptr;
}

PackModel *MDLM_FindImage(shen_name chidName, std::int32_t selectionMetric, PackSlot *scope) {
    char text[5]{};
    MDLM_NameToString(chidName, text);
    return MDLM_FindImage(text, selectionMetric, scope);
}

bool MDLM_ActivateCharacterImage(Character *character, const char *chidName, std::uint32_t imageCount) {
    if (!IMGM_Create(character, chidName, imageCount))
        return false;
    ImageModuleWork *const image = IMGM_GetWork(character);
    Vector3 position{};
    Vector3 *positionPointer = nullptr;
    if ((character->motionFlags & 0x3f) == 0x3f) {
        position = GetPositionForEntity(character->entity.node);
        positionPointer = &position;
    }
    character->model = imgmCreateHandlerForWithPos(character, image, positionPointer);
    return character->model != nullptr;
}

bool MDLM_ActivateCharacterImage(Character *character, shen_name chidName, std::uint32_t imageCount) {
    char text[5]{};
    MDLM_NameToString(chidName, text);
    return MDLM_ActivateCharacterImage(character, text, imageCount);
}

bool MDLM_ReplaceCharacterImage(Character *character, const char *chidName, PackModel *replacement, bool ownsStandaloneModel, PackSlot *ownerPack) {
    if (!MDLM_RegisterImage(chidName, chidName, replacement, 0, ownerPack, ownsStandaloneModel)) {
        return false;
    }
    return MDLM_ActivateCharacterImage(character, chidName, 0);
}

bool MDLM_ReplaceCharacterImage(Character *character, shen_name chidName, PackModel *replacement, bool ownsStandaloneModel, PackSlot *ownerPack) {
    char text[5]{};
    MDLM_NameToString(chidName, text);
    return MDLM_ReplaceCharacterImage(character, text, replacement, ownsStandaloneModel, ownerPack);
}

bool MDLM_UnloadImage(const char *chidName) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    MdlmRecord *const record = MDLM_FindRecordInternal(data, chidName, nullptr, false);
    if (!record)
        return false;
    MDLM_RemoveRecord(data, record);
    return true;
}

bool MDLM_UnloadImage(shen_name chidName) {
    char text[5]{};
    MDLM_NameToString(chidName, text);
    return MDLM_UnloadImage(text);
}

void MDLM_UnloadPackImages(PackSlot *ownerPack) {
    MdlmTaskData *const data = MDLM_GetTaskData();
    if (!data || !ownerPack)
        return;
    for (std::size_t bucket = 0; bucket < 19; ++bucket) {
        MdlmRecord *record = data->buckets[bucket];
        while (record) {
            MdlmRecord *const next = record->next;
            if (record->ownerPack == ownerPack) {
                MDLM_RemoveRecord(data, record);
            }
            record = next;
        }
    }
}

} // namespace shenmue::s1
