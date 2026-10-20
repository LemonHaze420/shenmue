#include "d3t/content.hpp"
#include "logging.hpp"

#include "platform/performance_counter.hpp"
#include "d3t/archive_data.hpp"
#include "d3t/archive_handles.hpp"
#include "d3t/asset_remap.hpp"
#include "d3t/compression.hpp"
#include "d3t/render_handles.hpp"
#include "d3t/threads.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <string>
#include <thread>

namespace shenmue::d3t {
std::uint32_t g_pendingContentRemappingCount = 0;

std::array<D3TContentSlot, 8192> g_d3tContentSlots{};

namespace {

std::mutex g_contentMutex;
std::mutex g_contentProcessMutex;
std::condition_variable_any g_contentCondition;

std::uint64_t currentNanoseconds() {
    const std::int64_t frequency = platform::performanceCounterFrequency();
    const std::int64_t counter = platform::performanceCounter();
    return static_cast<std::uint64_t>(1000000000LL * (counter / frequency) + 1000000000LL * (counter % frequency) / frequency);
}

void setState(D3TContentSlot &slot, std::int32_t state) {
    slot.state.store(state, std::memory_order_seq_cst);
    slot.stateTimestamp = currentNanoseconds();
}

void completeContentSlot(D3TContentSlot &slot) {
    d3t_ReleaseRenderHandle(slot.renderHandle);
    setState(slot, 12);
}

bool validContentHandle(std::uint32_t handle, D3TContentSlot *&slot) {
    const std::uint16_t index = static_cast<std::uint16_t>(handle >> 16U);
    if (index >= g_d3tContentSlots.size()) {
        return false;
    }
    slot = &g_d3tContentSlots[index];
    return slot->generation == static_cast<std::uint16_t>(handle);
}

void clearCompletedSlot(D3TContentSlot &slot) {
    const std::uint16_t generation = slot.generation;
    slot.flags.store(0, std::memory_order_relaxed);
    slot.reserved_08 = 0;
    slot.renderHandle = 0;
    std::memset(slot.sourcePath, 0, sizeof(slot.sourcePath));
    slot.loadedData = nullptr;
    slot.loadedSize = 0;
    slot.failureData = nullptr;
    slot.callback = nullptr;
    slot.callbackData = nullptr;
    slot.state.store(0, std::memory_order_relaxed);
    std::memset(slot.resolvedPath, 0, sizeof(slot.resolvedPath));
    slot.taskHandle = 0;
    slot.stateTimestamp = 0;
    slot.archiveHandle = 0;
    slot.destination = nullptr;
    slot.destinationCapacity = 0;
    slot.preserveArchiveBuffer = 0;
    slot.archivePathExists = 0;
    slot.option = 0;
    slot.generation = generation;
    slot.allocationLock.store(0, std::memory_order_release);
}

std::string normalizedSourcePath(const char *path) {
    std::array<char, 280> cleaned{};
    std::snprintf(cleaned.data(), cleaned.size(), "%s", path);
    char *const query = std::strrchr(cleaned.data(), '?');
    std::string suffix;
    if (query) {
        suffix = query + 1;
        *query = '\0';
    }
    FilepathCleaner(cleaned.data());
    std::string result = cleaned.data();
    if (query) {
        result.push_back('?');
        result += suffix;
    }
    return result;
}

std::uint32_t contentFilenameHash(const char *path) {
    std::array<char, 260> lowercase{};
    std::snprintf(lowercase.data(), lowercase.size(), "%s", path);
    for (char &character : lowercase) {
        if (character >= 'A' && character <= 'Z') {
            character = static_cast<char>(character + ('a' - 'A'));
        }
    }
    return d3t_HashData(lowercase.data(), static_cast<std::uint32_t>(std::strlen(lowercase.data())));
}

bool resolveArchivePath(D3TContentSlot &slot) {
    if (!ArchiveData_HasArchives()) {
        return false;
    }

    const char *source = slot.sourcePath;
    std::array<char, 260> remapped{};
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10 && (genRemapPath(0, source, remapped.data(), 0, false) || ([&] {
            std::array<char, 260> fullHash{};
            GenerateFullHashString(source, fullHash.data());
            return genRemapPath(0, fullHash.data(), remapped.data(), 0, false);
        })())) {
        source = remapped.data();
    }

    const char *const query = std::strchr(source, '?');
    const std::size_t sourceLength = query ? static_cast<std::size_t>(query - source) : std::strlen(source);
    const std::uint32_t hash = contentFilenameHash(source);
    const int written =
        std::snprintf(slot.resolvedPath, sizeof(slot.resolvedPath), "/tex/assets%.*s.%08x.%08x", static_cast<int>(sourceLength), source, hash, 0U);
    if (written < 0 || static_cast<std::size_t>(written) >= sizeof(slot.resolvedPath)) {
        return false;
    }
    FilepathCleaner(slot.resolvedPath);
    slot.archivePathExists = ArchiveData_GetFileSize(slot.resolvedPath) != 0 ? 1 : 0;
    return slot.archivePathExists != 0;
}

bool resolveExplicitArchivePath(D3TContentSlot &slot) {
    constexpr char archivePrefix[] = "/archive";
    if (std::strncmp(slot.resolvedPath, archivePrefix, sizeof(archivePrefix) - 1) != 0) {
        return true;
    }

    std::array<char, 272> source{};
    std::snprintf(source.data(), source.size(), "%s", slot.resolvedPath + sizeof(archivePrefix) - 1);
    FilepathCleaner(source.data());

    std::array<char, 260> remapped{};
    const char *archiveSource = source.data();
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10 && (genRemapPath(0, archiveSource, remapped.data(), 0, false) || ([&] {
            std::array<char, 260> fullHash{};
            GenerateFullHashString(archiveSource, fullHash.data());
            return genRemapPath(0, fullHash.data(), remapped.data(), 0, false);
        })())) {
        archiveSource = remapped.data();
    }

    const std::uint32_t hash = contentFilenameHash(archiveSource);
    const char *const query = std::strrchr(archiveSource, '?');
    const std::size_t pathLength = query ? static_cast<std::size_t>(query - archiveSource) : std::strlen(archiveSource);
    const int written =
        std::snprintf(slot.resolvedPath, sizeof(slot.resolvedPath), "/tex/assets%.*s.%08x.%08x", static_cast<int>(pathLength), archiveSource, hash, 0U);
    if (written < 0 || static_cast<std::size_t>(written) >= sizeof(slot.resolvedPath)) {
        return false;
    }
    FilepathCleaner(slot.resolvedPath);
    return true;
}

void startArchiveRead(D3TContentSlot &slot) {
    if (!resolveExplicitArchivePath(slot)) {
        setState(slot, 13);
        return;
    }
    const std::uint64_t size = ArchiveData_GetFileSize(slot.resolvedPath);
    if (size == 0) {
        setState(slot, 13);
        slot.failureData = slot.resolvedPath;
        return;
    }
    std::uint32_t handle = Archive_AllocateHandle(0);
    if (!Archive_DoesHandleExist(handle)) {
        setState(slot, 13);
        return;
    }
    void *const data = std::malloc(static_cast<std::size_t>(size));
    if (!data) {
        ReadFile_0(0, &handle);
        setState(slot, 13);
        return;
    }
    slot.archiveHandle = handle;
    Archive_BindBuffer(handle, data, size, false);
    Archive_QueueRead(handle, 0, slot.resolvedPath, 0, size, 17);
    setState(slot, 8);
}

std::int32_t finishContentTask(void *taskData) {
    if (!taskData) {
        return 0;
    }
    D3TContentSlot *slot = nullptr;
    std::memcpy(&slot, taskData, sizeof(slot));
    if (!slot) {
        return 0;
    }
    std::int32_t expectedState = 10;
    if (!slot->state.compare_exchange_strong(expectedState, 11, std::memory_order_seq_cst)) {
        return 0;
    }
    slot->stateTimestamp = currentNanoseconds();



    std::size_t decodedSize = static_cast<std::size_t>(slot->loadedSize);
    std::uint32_t decodeResult = 0;
    void *decoded = d3t_DecompressContent(slot->loadedData, &decodedSize, &decodeResult);
    if (decoded) {
        if (slot->destination) {
            const std::size_t copySize = std::min(decodedSize, static_cast<std::size_t>(slot->destinationCapacity));
            std::memcpy(slot->destination, static_cast<std::byte *>(decoded) + decodedSize - copySize, copySize);
            std::free(decoded);
            decoded = slot->destination;
            decodedSize = copySize;
        } else {
            Archive_BindBuffer(slot->archiveHandle, decoded, decodedSize, false);
        }
        slot->loadedData = decoded;
        slot->loadedSize = decodedSize;
    }

    if (slot->callback) {
        const std::uint32_t index = static_cast<std::uint32_t>(slot - g_d3tContentSlots.data());
        const std::uint32_t handle = (index << 16U) | static_cast<std::uint32_t>(slot->generation);
        slot->callback(slot->callbackData, handle, slot->loadedData, slot->loadedSize, slot->sourcePath);
    }
    completeContentSlot(*slot);
    return 0;
}

bool processContentSlots() {
    const std::lock_guard processLock(g_contentProcessMutex);
    bool busy = false;
    for (D3TContentSlot &slot : g_d3tContentSlots) {
        const std::int32_t state = slot.state.load(std::memory_order_seq_cst);
        if (state != 0 && state != 12 && state != 13) {
            busy = true;
        }
        if ((state == 2 && currentNanoseconds() - slot.stateTimestamp > 20000000ULL) ||
            (state == 4 && currentNanoseconds() - slot.stateTimestamp > 5000000ULL)) {
            setState(slot, 1);
        }
        if (slot.state.load(std::memory_order_seq_cst) == 1) {
            if (resolveArchivePath(slot)) {
                setState(slot, 6);
            } else {
                std::fflush(stderr);
                std::snprintf(slot.resolvedPath, sizeof(slot.resolvedPath), "Local file not found [%s]", slot.sourcePath);
                setState(slot, 13);
            }
        }
        if (slot.state.load(std::memory_order_seq_cst) == 6) {
            startArchiveRead(slot);
        }
        if (slot.state.load(std::memory_order_seq_cst) == 8 && !Archive_HasPendingRead(slot.archiveHandle)) {
            ArchiveHandleEntry *const archive = Archive_GetHandle(slot.archiveHandle);
            if (archive && archive->readSucceeded && archive->bufferedData) {
                slot.loadedData = archive->bufferedData;
                slot.loadedSize = archive->bufferedDataSize;
                setState(slot, 9);
            } else {
                setState(slot, 13);
            }
        }
        const std::int32_t completionState = slot.state.load(std::memory_order_seq_cst);
        if (completionState == 9 || completionState == 13) {
            if (completionState == 9 && slot.callback) {
                D3TContentSlot *slotPointer = &slot;
                const D3TTaskDescriptor descriptor{&slotPointer, finishContentTask, 0, 2};
                setState(slot, 10);
                slot.taskHandle = d3t_QueueTask(&descriptor, sizeof(slotPointer));
            } else {
                slot.flags.fetch_or(CSF_ReleaseRequested, std::memory_order_seq_cst);
                completeContentSlot(slot);
            }
        }
        if (slot.state.load(std::memory_order_seq_cst) == 12 && (slot.flags.load(std::memory_order_seq_cst) & CSF_ReleaseRequested) != 0) {
            if ((slot.flags.load(std::memory_order_seq_cst) & CSF_SkipBufferDetach) == 0) {
                Archive_DetachBuffer(slot.archiveHandle);
            }
            ReadFile_0(0, &slot.archiveHandle);
            clearCompletedSlot(slot);
        }
    }
    return busy;
}

} // namespace

std::int64_t PushShaderLoadToThreadQueue(const D3TShaderLoadTask *task) {
    if (!task || !task->path || task->path[0] != '/' || std::strchr(task->path, '\\')) {
        return -4;
    }
    const std::string path = normalizedSourcePath(task->path);
    if (path.size() >= sizeof(g_d3tContentSlots[0].sourcePath)) {
        return -4;
    }

    for (std::size_t index = 0; index < g_d3tContentSlots.size(); ++index) {
        D3TContentSlot &slot = g_d3tContentSlots[index];
        std::uint8_t expected = 0;
        if (!slot.allocationLock.compare_exchange_strong(expected, 1, std::memory_order_seq_cst)) {
            continue;
        }
        ++slot.generation;
        slot.flags.fetch_or(CSF_Allocated, std::memory_order_seq_cst);
        slot.reserved_08 = 0;
        std::memcpy(slot.sourcePath, path.c_str(), path.size() + 1);
        slot.renderHandle = task->renderHandle;
        d3t_AddRenderHandleReference(slot.renderHandle);
        slot.callback = task->thread.callback;
        slot.callbackData = task->thread.data;
        slot.destination = task->destination;
        slot.destinationCapacity = task->destinationCapacity;
        slot.option = task->option;
        setState(slot, 1);
        g_contentCondition.notify_one();
        return static_cast<std::int64_t>((static_cast<std::uint32_t>(index) << 16U) | static_cast<std::uint32_t>(slot.generation));
    }
    return -3;
}

void ProcessThreadStates(std::uint32_t contentHandle, std::int32_t type) {
    D3TContentSlot *slot = nullptr;
    if (!validContentHandle(contentHandle, slot)) {
        return;
    }
    slot->flags.store(type ? CSF_ReleaseRequested : CSF_ReleaseRequested | CSF_SkipBufferDetach, std::memory_order_seq_cst);
    if (type) {
        Archive_DetachBuffer(slot->archiveHandle);
    } else {
        slot->loadedData = nullptr;
    }
    ReadFile_0(0, &slot->archiveHandle);
    g_contentCondition.notify_one();
}

void ProcessRemappings(std::uint64_t unused) {
    static_cast<void>(unused);
    g_pendingContentRemappingCount = 0;
    for (const D3TContentSlot &slot : g_d3tContentSlots) {
        const std::int32_t state = slot.state.load(std::memory_order_seq_cst);
        if (state != 0 && state != 9 && state != 12 && state != 13)
            ++g_pendingContentRemappingCount;
    }
    processContentSlots();
}

void d3t_ContentLoad(std::uint16_t threadIndex) {
    const bool busy = processContentSlots();
    if (!busy && !g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst) && threadIndex < g_workerThreadCount) {
        std::unique_lock lock(g_contentMutex);
        g_contentCondition.wait_for(lock, std::chrono::milliseconds(100));
    }
}

} // namespace shenmue::d3t
