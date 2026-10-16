#pragma once

#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

struct D3TArchiveDirectory {
    const char *root;
    const char *backend;
    std::uint32_t type;
    std::uint32_t reserved14;
    std::uint64_t reserved18;
    std::uint8_t asynchronous;
    std::byte reserved21[7];
};

static_assert(sizeof(D3TArchiveDirectory) == 0x28);

struct D3TArchiveState {
    std::uint32_t type;
    std::uint32_t reserved04;
    char root[0x108];
    const char *backend;
    void *archives;
    std::uint32_t archiveCount;
    std::uint32_t archiveCapacity;
    void *primaryEntries;
    std::uint32_t *primaryHashes;
    std::uint16_t *primaryProbeDistances;
    std::uint32_t primaryCapacity;
    std::uint32_t reserved144;
    void *collisionEntries;
    std::uint32_t *collisionHashes;
    std::uint16_t *collisionProbeDistances;
    std::uint32_t collisionCapacity;
    std::uint32_t reserved164;
};

static_assert(offsetof(D3TArchiveState, root) == 0x08);
static_assert(offsetof(D3TArchiveState, backend) == 0x110);
static_assert(offsetof(D3TArchiveState, archives) == 0x118);
static_assert(offsetof(D3TArchiveState, archiveCount) == 0x120);
static_assert(offsetof(D3TArchiveState, primaryEntries) == 0x128);
static_assert(offsetof(D3TArchiveState, collisionEntries) == 0x148);
static_assert(sizeof(D3TArchiveState) == 0x168);

struct D3TArchiveReadContext {
    const char *root;
    const char *backend;
    std::uint32_t type;
    std::uint32_t reserved14;
    D3TArchiveState *state;
    std::uint64_t reserved20;
};

static_assert(sizeof(D3TArchiveReadContext) == 0x28);

extern std::uint32_t g_archiveReadTask;
extern D3TArchiveState g_archiveState;

void d3t_ReadArchives(const void *directory);
std::int32_t d3t_ReadArchives_Internal(D3TArchiveReadContext *context);

} // namespace shenmue::d3t
