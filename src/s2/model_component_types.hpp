#pragma once

#include "shenmue.h"
#include "common/types.hpp"
#include "common/math.hpp"
#include "s2/player.hpp"
#include "s2/mt7.hpp"
#include "s2/model_image.hpp"

#include <cstddef>
#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace shenmue::s2 {

struct Character;
namespace HLib {
struct Task;
}
struct ModelComponentHeader;





#pragma pack(push, 4)
struct ModelComponentStorageBlock {
    ModelComponentStorageBlock *previous;
    std::uint32_t used;
    std::uint32_t reserved0c;
};

using ModelComponentUpdate = void (*)(Character *, void *);
using ModelComponentCleanup = void (*)(Character *, void *);
using ModelComponentCore = void (*)(Character *, void *);

struct ModelComponentHeader {
    ModelComponentHeader *tokenLeft;
    ModelComponentHeader *tokenRight;
    shen_name token;
    std::uint32_t reserved14;
    ModelComponentHeader *next;
    std::uint32_t flags;
    std::uint32_t reserved24;
    ModelComponentUpdate update;
    ModelComponentCleanup cleanup;
    ModelComponentCore core;
};

#pragma pack(pop)

#pragma pack(push, 4)
struct ModelManagerEntry {
    std::uint32_t flags;
#if SHENMUE_IS_PC
    std::uint32_t distancePriority;
    ModelImageIndex *image;
    MT7Model *instance;
    MT7Model *source;
    std::uint32_t childIndex;
    std::int32_t groupId;
    std::uint32_t variant;
    std::uint32_t reserved2c;
#else
    std::byte reserved04[0x1c];
#endif
};

struct ModelManagerTaskData {
    std::uint32_t marker;
    std::uint32_t state;
    std::byte managerState[0xc78];
    std::uint32_t entryCount;
#if SHENMUE_IS_PC
    std::uint32_t reservedC84;
    std::array<std::uintptr_t, 19> buckets;
#else
    std::array<std::uint32_t, 19> buckets;
#endif
    std::array<ModelManagerEntry, 256> entries;
};

static_assert(offsetof(ModelManagerTaskData, entryCount) == 0xc80);
#if SHENMUE_IS_PC
static_assert(sizeof(ModelManagerEntry) == 0x30);
static_assert(offsetof(ModelManagerTaskData, buckets) == 0xc88);
static_assert(offsetof(ModelManagerTaskData, entries) == 0xd20);
static_assert(sizeof(ModelManagerTaskData) == 0x3d20);
#else
static_assert(sizeof(ModelManagerEntry) == 0x20);
static_assert(offsetof(ModelManagerTaskData, buckets) == 0xc84);
static_assert(offsetof(ModelManagerTaskData, entries) == 0xcd0);
static_assert(sizeof(ModelManagerTaskData) == 0x2cd0);
#endif
#pragma pack(pop)

using CharacterCoreCallback = void (*)(Character *);

static_assert(sizeof(ModelComponentStorageBlock) == 0x10);
static_assert(offsetof(ModelComponentStorageBlock, used) == 0x08);
static_assert(sizeof(ModelComponentHeader) == 0x40);
static_assert(offsetof(ModelComponentHeader, token) == 0x10);
static_assert(offsetof(ModelComponentHeader, next) == 0x18);
static_assert(offsetof(ModelComponentHeader, flags) == 0x20);
static_assert(offsetof(ModelComponentHeader, update) == 0x28);
static_assert(offsetof(ModelComponentHeader, cleanup) == 0x30);
static_assert(offsetof(ModelComponentHeader, core) == 0x38);

} // namespace shenmue::s2
