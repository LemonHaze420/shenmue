#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

using D3TContentCallback = void (*)(void *data, std::uint32_t contentHandle, void *loadedData, std::uint64_t loadedSize, const char *sourcePath);

struct D3TContentThreadDescriptor {
    D3TContentCallback callback;
    void *data;
};

struct D3TShaderLoadTask {
    const char *path;
    std::uint32_t renderHandle;
    std::uint32_t reserved_0c;
    D3TContentThreadDescriptor thread;
    void *destination;
    std::uint64_t destinationCapacity;
    std::uint8_t option;
    std::byte reserved_31[7];
};

enum ContentSlotFlag : std::uint32_t {
    CSF_Allocated = 0x01,
    CSF_ReleaseRequested = 0x04,
    CSF_SkipBufferDetach = 0x08,
};
struct D3TContentSlot {
    std::uint16_t generation;
    std::uint16_t reserved_02;
    std::atomic<std::uint32_t> flags;
    std::uint32_t reserved_08;
    std::uint32_t renderHandle;
    char sourcePath[256];
    void *loadedData;
    std::uint64_t loadedSize;
    void *failureData;
    D3TContentCallback callback;
    void *callbackData;
    std::atomic<std::int32_t> state;
    char resolvedPath[4096];
    std::uint32_t taskHandle;
    std::uint64_t stateTimestamp;
    std::atomic<std::uint8_t> allocationLock;
    std::byte reserved_1149[3];
    std::uint32_t archiveHandle;
    void *destination;
    std::uint64_t destinationCapacity;
    std::uint8_t preserveArchiveBuffer;
    std::uint8_t archivePathExists;
    std::uint8_t option;
    std::byte reserved_1163[5];
};

static_assert(sizeof(D3TContentThreadDescriptor) == 0x10);
static_assert(sizeof(D3TShaderLoadTask) == 0x38);
static_assert(offsetof(D3TContentSlot, flags) == 0x04);
static_assert(offsetof(D3TContentSlot, sourcePath) == 0x10);
static_assert(offsetof(D3TContentSlot, loadedData) == 0x110);
static_assert(offsetof(D3TContentSlot, callback) == 0x128);
static_assert(offsetof(D3TContentSlot, state) == 0x138);
static_assert(offsetof(D3TContentSlot, resolvedPath) == 0x13c);
static_assert(offsetof(D3TContentSlot, stateTimestamp) == 0x1140);
static_assert(offsetof(D3TContentSlot, allocationLock) == 0x1148);
static_assert(offsetof(D3TContentSlot, archiveHandle) == 0x114c);
static_assert(offsetof(D3TContentSlot, destination) == 0x1150);
static_assert(offsetof(D3TContentSlot, option) == 0x1162);
static_assert(sizeof(D3TContentSlot) == 0x1168);

extern std::array<D3TContentSlot, 8192> g_d3tContentSlots;
extern std::uint32_t g_pendingContentRemappingCount;

std::int64_t PushShaderLoadToThreadQueue(const D3TShaderLoadTask *task);
void ProcessThreadStates(std::uint32_t contentHandle, std::int32_t type);
void ProcessRemappings(std::uint64_t unused = 0);
void d3t_ContentLoad(std::uint16_t threadIndex);

} // namespace shenmue::d3t
