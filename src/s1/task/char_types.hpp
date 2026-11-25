#pragma once

#include "common/math.hpp"

#include <cstddef>
#include <cstdint>

namespace shenmue::s1 {

struct Character;
namespace HLib {
struct Task;
} // namespace HLib

#pragma pack(push, 4)

struct CharacterNode {
    CharacterNode *child;
    Vector3 position;
    Vector3i rotation;
};

struct CharacterIdTransform {
    CharacterNode node;
    std::uint32_t layer;
    const void *collisionData;
};

struct CharacterEntity {
    std::uint32_t name;
    CharacterNode node;
};

struct CharacterComponentNode {
    CharacterComponentNode *greaterChild;
    CharacterComponentNode *lessChild;
    std::uint32_t componentId;
#if UINTPTR_MAX == UINT64_MAX
    std::uint32_t reserved_14;
#endif
    CharacterComponentNode *next;
    std::uint32_t flags;
#if UINTPTR_MAX == UINT64_MAX
    std::uint32_t reserved_24;
#endif
    void (*updateCallback)(Character *, void *);
    void (*destroyCallback)(Character *, void *);
    void (*drawCallback)(Character *, void *);
};

struct CharacterShadowState {
    std::byte reserved_00[8];
    std::int32_t lightIndex[2];
    float fade[2];
    std::uint32_t flag[2];
};

union CharacterFlags {
    std::uint32_t value;
    std::byte bytes[4];
};

struct CharacterBucketNode {
    CharacterBucketNode *next;
    Character *character;
    HLib::Task *task;
};

struct CharacterIdLookupNode {
    CharacterIdLookupNode *left;
    CharacterIdLookupNode *right;
    std::uint32_t id;
#if UINTPTR_MAX == UINT64_MAX
    std::uint32_t reserved_14;
#endif
    CharacterIdLookupNode *bucketNext;
    Character *character;
};

#pragma pack(pop)

static_assert(sizeof(CharacterShadowState) == 0x20);
static_assert(sizeof(CharacterFlags) == 4);

#if UINTPTR_MAX == UINT32_MAX
static_assert(sizeof(CharacterNode) == 0x1c);
static_assert(sizeof(CharacterIdTransform) == 0x24);
static_assert(offsetof(CharacterIdTransform, layer) == 0x1c);
static_assert(offsetof(CharacterIdTransform, collisionData) == 0x20);
static_assert(sizeof(CharacterEntity) == 0x20);
static_assert(sizeof(CharacterComponentNode) == 0x20);
static_assert(offsetof(CharacterComponentNode, componentId) == 8);
static_assert(offsetof(CharacterComponentNode, next) == 0xc);
static_assert(offsetof(CharacterComponentNode, flags) == 0x10);
static_assert(offsetof(CharacterComponentNode, updateCallback) == 0x14);
static_assert(offsetof(CharacterComponentNode, destroyCallback) == 0x18);
static_assert(offsetof(CharacterComponentNode, drawCallback) == 0x1c);
static_assert(sizeof(CharacterBucketNode) == 0xc);
static_assert(sizeof(CharacterIdLookupNode) == 0x14);
static_assert(offsetof(CharacterIdLookupNode, id) == 8);
static_assert(offsetof(CharacterIdLookupNode, bucketNext) == 0xc);
static_assert(offsetof(CharacterIdLookupNode, character) == 0x10);
#else
static_assert(sizeof(void *) == 8);
static_assert(sizeof(CharacterNode) == 0x20);
static_assert(sizeof(CharacterIdTransform) == 0x2c);
static_assert(offsetof(CharacterIdTransform, layer) == 0x20);
static_assert(offsetof(CharacterIdTransform, collisionData) == 0x24);
static_assert(sizeof(CharacterEntity) == 0x24);
static_assert(sizeof(CharacterComponentNode) == 0x40);
static_assert(offsetof(CharacterComponentNode, componentId) == 0x10);
static_assert(offsetof(CharacterComponentNode, next) == 0x18);
static_assert(offsetof(CharacterComponentNode, flags) == 0x20);
static_assert(offsetof(CharacterComponentNode, updateCallback) == 0x28);
static_assert(offsetof(CharacterComponentNode, destroyCallback) == 0x30);
static_assert(offsetof(CharacterComponentNode, drawCallback) == 0x38);
static_assert(sizeof(CharacterBucketNode) == 0x18);
static_assert(sizeof(CharacterIdLookupNode) == 0x28);
static_assert(offsetof(CharacterIdLookupNode, id) == 0x10);
static_assert(offsetof(CharacterIdLookupNode, bucketNext) == 0x18);
static_assert(offsetof(CharacterIdLookupNode, character) == 0x20);
#endif

} // namespace shenmue::s1
