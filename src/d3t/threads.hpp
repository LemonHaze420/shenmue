#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

namespace shenmue::d3t {

using D3TTaskCallback = std::int32_t (*)(void *data);
using D3TTaskCompletion = void (*)(void *data);

struct D3TTaskDescriptor {
    const void *data;
    D3TTaskCallback callback;
    D3TTaskCompletion completion;
    std::uint64_t priority;
};

static_assert(sizeof(D3TTaskDescriptor) == 0x20);

struct D3TTaskRecord {
    D3TTaskDescriptor descriptor;
    std::atomic<std::int32_t> state;
    std::array<std::byte, 0xc00> data;
    std::atomic<std::int32_t> schedule;
    std::uint16_t generation;
    std::uint16_t reserved_c2a;
    std::int32_t result;
};

static_assert(offsetof(D3TTaskRecord, state) == 0x20);
static_assert(offsetof(D3TTaskRecord, data) == 0x24);
static_assert(offsetof(D3TTaskRecord, schedule) == 0xc24);
static_assert(offsetof(D3TTaskRecord, generation) == 0xc28);
static_assert(offsetof(D3TTaskRecord, result) == 0xc2c);
static_assert(sizeof(D3TTaskRecord) == 0xc30);

extern std::uint32_t g_invalidTaskHandle;
extern std::array<D3TTaskRecord, 1024> g_d3tTasks;
extern std::atomic<std::int32_t> g_d3tTaskCount;
extern std::atomic<std::int32_t> g_d3tDeferredCompletionCount;
extern std::atomic<bool> g_d3tThreadsInitialized;
extern std::atomic<bool> g_d3tShutdownThreadFlag;
extern bool g_d3tLowPriorityThreadFlag;
extern std::uint32_t g_workerThreadCount;

void d3t_CreateThreads();
void d3t_SetAudioCallback(void (*callback)());
std::uint32_t d3t_QueueTask(const D3TTaskDescriptor *descriptor, std::size_t dataSize);
bool RunTask(std::uint32_t minimumPriority, const std::uint32_t *handles = nullptr, std::uint32_t handleCount = 0);
bool d3t_IsTaskActive(std::uint32_t handle);
void ExecuteADynamicFunctionOnce();
std::int64_t ShutdownThreads();

} // namespace shenmue::d3t
