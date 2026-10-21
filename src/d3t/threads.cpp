#include "d3t/threads.hpp"

#include "d3t/content.hpp"

#include <algorithm>
#include <bit>
#include <condition_variable>
#include <cstring>
#include <limits>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif

namespace shenmue::d3t {

std::uint32_t g_invalidTaskHandle{};

std::array<D3TTaskRecord, 1024> g_d3tTasks{};
std::atomic<std::int32_t> g_d3tTaskCount{0};
std::atomic<std::int32_t> g_d3tDeferredCompletionCount{0};
std::atomic<bool> g_d3tThreadsInitialized{false};
std::atomic<bool> g_d3tShutdownThreadFlag{false};
bool g_d3tLowPriorityThreadFlag = false;
std::uint32_t g_workerThreadCount = 7;

void LoadContent(std::uint16_t threadIndex);

namespace {

std::mutex g_taskMutex;
std::thread::id g_taskOwnerThread;
std::atomic<bool> g_scheduleAging{false};
std::condition_variable_any g_highPriorityWake;
std::condition_variable_any g_lowPriorityWake;
std::vector<std::jthread> g_threads;
std::atomic<std::uint16_t> g_taskGeneration{0};
std::mutex g_audioMutex;
std::condition_variable_any g_audioWake;
void (*g_audioCallback)() = nullptr;
bool g_audioCallbackRunning = false;

void SetCurrentThreadName(const wchar_t *name) {
#ifdef _WIN32
    SetThreadDescription(GetCurrentThread(), name);
#else
    (void)name;
#endif
}

void CompleteTask(D3TTaskRecord &task) {
    g_d3tTaskCount.fetch_sub(1, std::memory_order_seq_cst);
    if (task.descriptor.completion == 0) {
        task.descriptor = {};
        task.state.store(0, std::memory_order_seq_cst);
    } else {
        g_d3tDeferredCompletionCount.fetch_add(1, std::memory_order_seq_cst);
        task.state.store(5, std::memory_order_seq_cst);
    }
}

D3TTaskRecord *ResolveTaskHandle(std::uint32_t handle) {
    const std::uint16_t slot = static_cast<std::uint16_t>(handle);
    if (slot == 0) {
        return nullptr;
    }
    D3TTaskRecord &task = g_d3tTasks[slot - 1];
    if (task.generation != static_cast<std::uint16_t>(handle >> 16U)) {
        return nullptr;
    }
    return &task;
}

D3TTaskRecord *ClaimScheduledTask(std::uint32_t minimumPriority) {
    D3TTaskRecord *selected = nullptr;
    std::int32_t selectedSchedule = (std::numeric_limits<std::int32_t>::max)();
    for (D3TTaskRecord &task : g_d3tTasks) {
        if (task.state.load(std::memory_order_seq_cst) != 2 || static_cast<std::uint32_t>(task.descriptor.priority) < minimumPriority) {
            continue;
        }
        if (selected && selected->descriptor.priority == 0)
            continue;
        const std::int32_t schedule = task.schedule.load(std::memory_order_relaxed);
        const bool wins = schedule < selectedSchedule || task.descriptor.priority == 0;
        if (wins) {
            selected = &task;
            selectedSchedule = schedule;
        }
    }
    if (!selected) {
        return nullptr;
    }
    std::int32_t expected = 2;
    return selected->state.compare_exchange_strong(expected, 3, std::memory_order_seq_cst) ? selected : nullptr;
}

bool ExecuteClaimedTask(D3TTaskRecord &task) {
    std::int32_t expected = 3;
    if (!task.state.compare_exchange_strong(expected, 4, std::memory_order_seq_cst)) {
        return false;
    }
    void *data = task.descriptor.data ? task.data.data() : nullptr;
    task.result = task.descriptor.callback ? task.descriptor.callback(data) : 0;
    if (task.result == 1) {
        const std::uint32_t schedule = std::bit_cast<std::uint32_t>(task.schedule.load(std::memory_order_relaxed));
        const std::uint32_t increment = 100U * static_cast<std::uint32_t>(task.descriptor.priority) + 100U;
        task.schedule.store(std::bit_cast<std::int32_t>(schedule + increment), std::memory_order_relaxed);
        task.state.store(2, std::memory_order_seq_cst);
    } else {
        CompleteTask(task);
    }
    return true;
}

void TaskWorker(std::stop_token stopToken, std::uint32_t minimumPriority, std::wstring name) {
    SetCurrentThreadName(name.c_str());
    while (!stopToken.stop_requested()) {
        if (RunTask(minimumPriority)) {

            continue;
        }
        std::unique_lock lock(g_taskMutex);
        std::condition_variable_any &wake = minimumPriority == 0 ? g_highPriorityWake : g_lowPriorityWake;
        wake.wait(
            lock, stopToken, [] { return g_d3tTaskCount.load(std::memory_order_seq_cst) > 0 || g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst); });
    }
}

void CallbackWorker(std::stop_token stopToken, std::uint16_t threadIndex, void (*callback)(std::uint16_t), std::wstring name) {
    SetCurrentThreadName(name.c_str());
    while (!stopToken.stop_requested() && !g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst)) {
        callback(threadIndex);
    }
}

void IdleWorker(std::stop_token stopToken, std::wstring name) {
    SetCurrentThreadName(name.c_str());
    std::mutex mutex;
    std::condition_variable_any condition;
    std::unique_lock lock(mutex);
    condition.wait(lock, stopToken, [] { return false; });
}

void AudioWorker(std::stop_token stopToken) {
    SetCurrentThreadName(L"Audio");
    std::unique_lock<std::mutex> lock(g_audioMutex);
    while (!stopToken.stop_requested() && !g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst)) {
        g_audioWake.wait(lock, stopToken, [] { return g_audioCallback || g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst); });
        if (stopToken.stop_requested() || g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst))
            break;
        void (*const callback)() = g_audioCallback;
        g_audioCallback = nullptr;
        g_audioCallbackRunning = true;
        lock.unlock();
        callback();
        lock.lock();
        g_audioCallbackRunning = false;
        g_audioWake.notify_all();
    }
}

} // namespace

void d3t_CreateThreads() {
    if (g_d3tThreadsInitialized.exchange(true, std::memory_order_seq_cst)) {
        return;
    }
    g_taskOwnerThread = std::this_thread::get_id();
    SetCurrentThreadName(L"Main");
    g_d3tShutdownThreadFlag.store(false, std::memory_order_seq_cst);

    const std::uint32_t hardwareThreads = std::thread::hardware_concurrency();
    std::uint32_t lowPriorityCount = 0;
    if (g_d3tLowPriorityThreadFlag && hardwareThreads > 6) {
        lowPriorityCount = hardwareThreads - 6;
        g_workerThreadCount += lowPriorityCount;
    }

    g_threads.reserve(g_workerThreadCount);
    g_threads.emplace_back(CallbackWorker, 0, LoadContent, L"Disk IO");
    g_threads.emplace_back(CallbackWorker, 1, d3t_ContentLoad, L"Content Load");
    g_threads.emplace_back(AudioWorker);
    g_threads.emplace_back(TaskWorker, 0, L"Tasks [HP] 0");
    g_threads.emplace_back(TaskWorker, 0, L"Tasks [HP] 1");
    g_threads.emplace_back(TaskWorker, 0, L"Tasks [HP] 2");
    for (std::uint32_t index = 0; index < lowPriorityCount; ++index) {
        const std::wstring name = L"Tasks [LP] " + std::to_wstring(index);
        g_threads.emplace_back(TaskWorker, 2, name);
    }
    while (g_threads.size() < g_workerThreadCount) {
        g_threads.emplace_back(IdleWorker, L"D3T Idle");
    }

#ifdef _WIN32
    if (g_threads.size() > 2) {
        SetThreadPriority(g_threads[2].native_handle(), 3);
    }
#endif
}

void d3t_SetAudioCallback(void (*callback)()) {
    std::unique_lock<std::mutex> lock(g_audioMutex);
    g_audioCallback = callback;
    if (callback)
        g_audioWake.notify_one();
    else
        g_audioWake.wait(lock, [] { return !g_audioCallbackRunning; });
}

std::uint32_t d3t_QueueTask(const D3TTaskDescriptor *descriptor, std::size_t dataSize) {
    if (!g_d3tThreadsInitialized.load(std::memory_order_seq_cst)) {
        return 0;
    }

    D3TTaskRecord *task = nullptr;
    std::size_t slot = 0;
    while (!task) {
        for (std::size_t index = 0; index < g_d3tTasks.size(); ++index) {
            std::int32_t expected = 0;
            if (g_d3tTasks[index].state.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
                task = &g_d3tTasks[index];
                slot = index;
                break;
            }
        }
        std::this_thread::yield();
    }

    task->descriptor = {};
    task->data.fill(std::byte{0});
    if (descriptor->data && dataSize != 0) {
        std::memcpy(task->data.data(), descriptor->data, dataSize);
    }
    task->descriptor = *descriptor;
    task->schedule = 100 * (static_cast<std::int32_t>(descriptor->priority) + 1);
    task->result = 0;
    task->generation = static_cast<std::uint16_t>(g_taskGeneration.fetch_add(1, std::memory_order_seq_cst) + 1);
    g_d3tTaskCount.fetch_add(1, std::memory_order_seq_cst);
    task->state.store(2, std::memory_order_seq_cst);
    g_highPriorityWake.notify_one();
    g_lowPriorityWake.notify_one();
    return (static_cast<std::uint32_t>(task->generation) << 16U) | static_cast<std::uint32_t>(slot + 1);
}

bool RunTask(std::uint32_t minimumPriority, const std::uint32_t *handles, std::uint32_t handleCount) {
    if (g_d3tTaskCount.load(std::memory_order_seq_cst) <= 0)
        return false;
    if (handles) {
        for (std::uint32_t index = 0; index < handleCount; ++index) {
            D3TTaskRecord *task = ResolveTaskHandle(handles[index]);
            if (!task) {
                continue;
            }
            std::int32_t expected = 2;
            if (task->state.compare_exchange_strong(expected, 3, std::memory_order_seq_cst)) {
                return ExecuteClaimedTask(*task);
            }
        }
        return false;
    }
    if (std::this_thread::get_id() != g_taskOwnerThread) {
        bool expected = false;
        if (g_scheduleAging.compare_exchange_strong(expected, true, std::memory_order_seq_cst)) {
            for (D3TTaskRecord &queued : g_d3tTasks) {
                if (queued.state.load(std::memory_order_seq_cst) == 2) {
                    const std::uint32_t schedule = std::bit_cast<std::uint32_t>(queued.schedule.load(std::memory_order_relaxed));
                    queued.schedule.store(std::bit_cast<std::int32_t>(schedule - 1U), std::memory_order_relaxed);
                }
            }
            g_scheduleAging.store(false, std::memory_order_seq_cst);
        }
    }
    D3TTaskRecord *task = ClaimScheduledTask(minimumPriority);
    return task && ExecuteClaimedTask(*task);
}

bool d3t_IsTaskActive(std::uint32_t handle) {
    D3TTaskRecord *const task = ResolveTaskHandle(handle);
    return task && task->state.load(std::memory_order_seq_cst) != 0;
}

void ExecuteADynamicFunctionOnce() {
    if (g_d3tDeferredCompletionCount.load(std::memory_order_seq_cst) <= 0) {
        return;
    }
    for (D3TTaskRecord &task : g_d3tTasks) {
        std::int32_t expected = 5;
        if (!task.state.compare_exchange_strong(expected, 6, std::memory_order_seq_cst)) {
            continue;
        }
        void *const data = task.descriptor.data ? task.data.data() : nullptr;
        task.descriptor.completion(data);
        g_d3tDeferredCompletionCount.fetch_sub(1, std::memory_order_seq_cst);
        task.descriptor = {};
        task.state.store(0, std::memory_order_seq_cst);
    }
}

std::int64_t ShutdownThreads() {
    g_d3tThreadsInitialized.store(false, std::memory_order_seq_cst);
    g_d3tShutdownThreadFlag.store(true, std::memory_order_seq_cst);
    d3t_SetAudioCallback(nullptr);
    g_audioWake.notify_all();
    g_highPriorityWake.notify_all();
    g_lowPriorityWake.notify_all();
    for (std::jthread &thread : g_threads) {
        thread.request_stop();
    }
    g_threads.clear();
    g_d3tTaskCount.store(0, std::memory_order_seq_cst);
    g_d3tDeferredCompletionCount.store(0, std::memory_order_seq_cst);
    for (D3TTaskRecord &task : g_d3tTasks) {
        task.state.store(0, std::memory_order_seq_cst);
    }
    return 0;
}

} // namespace shenmue::d3t
