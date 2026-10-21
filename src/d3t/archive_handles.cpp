#include "d3t/archive_handles.hpp"

#include "d3t/archive_data.hpp"
#include "d3t/threads.hpp"
#include "d3t/archives.hpp"
#include "d3t/compression.hpp"
#include "d3t/file_state.hpp"

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <thread>
#if defined(_WIN32)
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#endif

namespace shenmue::d3t {

bool Platform_CloseFileHandle(PlatformFileHandle handle);

ArchiveHandleTable g_archiveHandles = [] {
    ArchiveHandleTable table{};
    table.handleIds.fill(0x800);
    return table;
}();
std::atomic<std::int32_t> g_archiveHandleReaders{0};
std::atomic<std::uint8_t> g_archiveHandleWriter{0};
std::uint32_t g_invalidArchiveHandle = 0;
std::uint32_t g_nextArchiveHandleIndex = 0;
std::array<AsyncArchiveRequest, 2048> g_asyncArchiveRequests{};
std::array<LargeFileDataEntry, 2048> g_largeFileData{};

namespace {

std::atomic<std::int32_t> g_openFileSizeReaders{0};
std::atomic<std::uint8_t> g_openFileSizeWriter{0};
std::atomic<std::uint64_t> g_openFileSizes{0};
ArchiveSourceDesc g_currentArchiveSource{};
std::mutex g_archiveWorkerMutex;
std::condition_variable_any g_archiveWorkerCondition;

// @verified - p
bool IsArchiveSourceDescActive(const ArchiveSourceDesc &source) {
    const ArchiveSourceDesc &empty = archive_data::EMPTY_ARCHIVE_SOURCE;
    return (source.sourceIndex != empty.sourceIndex || source.sourceOffset != empty.sourceOffset || source.fileSize != empty.fileSize ||
               source.flags != empty.flags) &&
        source.sourceIndex < g_archiveState.archiveCount && source.fileSize != 0;
}

// @verified - p
bool CheckArchiveHandleOpen(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle))
        return false;
    const ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    return entry && entry->platformHandle != -1;
}

// @verified - p
bool IsArchiveHandleOpenForMode(std::uint32_t handle, std::uint8_t required) {
    if (!Archive_DoesHandleExist(handle))
        return false;
    const ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    return entry && entry->platformHandle != -1 && (entry->accessMode & required) != 0;
}

// @verified - p
std::uint64_t GetArchiveFileSize(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle))
        return 0;
    const ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry)
        return 0;
    if (IsArchiveSourceDescActive(entry->source))
        return entry->source.fileSize;
    if (entry->platformHandle != -1) {
#if defined(_WIN32)
        LARGE_INTEGER size{};
        if (GetFileSizeEx(reinterpret_cast<HANDLE>(entry->platformHandle), &size))
            return static_cast<std::uint64_t>(size.QuadPart);
#else
        struct stat attributes {};
        if (!fstat(static_cast<int>(entry->platformHandle), &attributes))
            return static_cast<std::uint64_t>(attributes.st_size);
#endif
    }
    return 0;
}

std::int32_t OpenArchiveSlotBackingFile(std::uint32_t handle, const char *path, std::uint8_t mode) {
    if (!Archive_DoesHandleExist(handle))
        return 1;
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry)
        return 1;
    if (CheckArchiveHandleOpen(handle))
        Archive_ClosePlatformHandle(handle);
    ArchiveSourceDesc source = archive_data::EMPTY_ARCHIVE_SOURCE;
    if (!(entry->archiveLookupFlag & 1U))
        ArchiveData_GetSourceDesc(path, source);
    char backing[260];
    if (IsArchiveSourceDescActive(source)) {
        if (mode & AOF_Write)
            return 1;
        if (!ArchiveData_GetSourceBackingPath(source, backing, sizeof(backing)))
            return 1;
        path = backing;
    }
    entry->accessMode = mode;
    std::snprintf(entry->path, sizeof(entry->path), "%s", path);
    entry->source = source;
    char *filename = entry->path;
#if defined(_WIN32)

    while (*filename == '/' || *filename == '\\')
        ++filename;
    DWORD access = (mode & AOF_Read) ? GENERIC_READ : 0;
    DWORD disposition = (mode & AOF_Read) ? OPEN_EXISTING : 0;
    if (mode & AOF_Write) {
        access |= GENERIC_WRITE;
        disposition = (mode & AOF_Truncate) ? CREATE_ALWAYS : OPEN_ALWAYS;
    }
    if (access != 0) {
        const HANDLE file =
            CreateFileA(filename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, disposition, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, nullptr);
        entry->platformHandle = reinterpret_cast<PlatformFileHandle>(file);
        if (file != INVALID_HANDLE_VALUE && (!(mode & AOF_Write) || GetFileAttributesA(filename) != INVALID_FILE_ATTRIBUTES))
            return 0;
        (void)GetLastError();
    }
#else
    if (mode & (AOF_Read | AOF_Write)) {
        const int access = (mode & AOF_Write) ? O_CREAT | ((mode & AOF_Read) ? O_RDWR : O_WRONLY) | ((mode & AOF_Truncate) ? O_TRUNC : 0) : O_RDONLY;
        int file = open(filename, access, 0666);
        if (file >= 0 && file < 3) {
            const int duplicate = fcntl(file, F_DUPFD, 3);

            close(file);
            file = duplicate;
        }
        entry->platformHandle = file;
        if (file != -1)
            return 0;
    }
#endif
    return 1;
}

void MarkArchiveJobPending(std::uint32_t handle) {
    for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load(std::memory_order_seq_cst) != AJS_Free && request.archiveHandle == handle) {
            request.state.exchange(AJS_Pending, std::memory_order_seq_cst);
            g_archiveWorkerCondition.notify_one();
            return;
        }
    }
}

// @verified - p
bool BeginArchiveRead(std::uint32_t handle, std::uint64_t offset, std::uint64_t size) {
    if (!Archive_DoesHandleExist(handle))
        return false;
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry || !IsArchiveHandleOpenForMode(handle, AOF_Read) || !Archive_GetHandleBuffer(handle, size))
        return false;
    const bool active = IsArchiveSourceDescActive(entry->source);
    const std::uint64_t remaining = GetArchiveFileSize(handle) - offset;
    std::uint32_t count = static_cast<std::uint32_t>(size);
    if (size > remaining)
        count = static_cast<std::uint32_t>(remaining);
    if (active)
        offset += entry->source.sourceOffset;
    entry->overlapped = {};
    entry->overlapped.offset = offset;
#if defined(_WIN32)
    static_assert(sizeof(ArchiveOverlapped) == sizeof(OVERLAPPED) && alignof(ArchiveOverlapped) == alignof(OVERLAPPED));
    static_assert(offsetof(ArchiveOverlapped, internal) == offsetof(OVERLAPPED, Internal));
    static_assert(offsetof(ArchiveOverlapped, transferred) == offsetof(OVERLAPPED, InternalHigh));
    static_assert(offsetof(ArchiveOverlapped, offset) == offsetof(OVERLAPPED, Offset));
    static_assert(offsetof(ArchiveOverlapped, event) == offsetof(OVERLAPPED, hEvent));
    bool accepted =
        ReadFile(reinterpret_cast<HANDLE>(entry->platformHandle), entry->bufferedData, count, nullptr, reinterpret_cast<OVERLAPPED *>(&entry->overlapped)) != 0;
    if (!accepted)
        accepted = GetLastError() == ERROR_IO_PENDING;
#else
    const ssize_t read = pread(static_cast<int>(entry->platformHandle), entry->bufferedData, count, static_cast<off_t>(offset));
    const bool accepted = read >= 0;
    entry->overlapped.internal = accepted ? 0 : 1;
    entry->overlapped.transferred = accepted ? static_cast<std::uintptr_t>(read) : 0;
#endif
    MarkArchiveJobPending(handle);
    return accepted;
}

// @verified - p
bool BeginArchiveReadChecked(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle) || !Archive_GetHandle(handle) || !IsArchiveHandleOpenForMode(handle, AOF_Read))
        return false;
    return BeginArchiveRead(handle, 0, GetArchiveFileSize(handle));
}

// @verified - p
bool BeginArchiveWrite(std::uint32_t handle, std::uint64_t offset) {
    if (!Archive_DoesHandleExist(handle))
        return false;
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry || IsArchiveSourceDescActive(entry->source) || !IsArchiveHandleOpenForMode(handle, AOF_Write) || !entry->bufferedData)
        return false;
    entry->overlapped = {};
    entry->overlapped.offset = offset;
    const std::uint32_t count = static_cast<std::uint32_t>(entry->bufferedDataSize);
#if defined(_WIN32)
    bool accepted = WriteFile(reinterpret_cast<HANDLE>(entry->platformHandle), entry->bufferedData, count, nullptr,
                        reinterpret_cast<OVERLAPPED *>(&entry->overlapped)) != 0;
    if (!accepted)
        accepted = GetLastError() == ERROR_IO_PENDING;
#else
    const ssize_t written = pwrite(static_cast<int>(entry->platformHandle), entry->bufferedData, count, static_cast<off_t>(offset));
    const bool accepted = written >= 0;
    entry->overlapped.internal = accepted ? 0 : 1;
    entry->overlapped.transferred = accepted ? static_cast<std::uintptr_t>(written) : 0;
#endif
    MarkArchiveJobPending(handle);
    return accepted;
}

// @verified - p
bool FinalizeCompletedArchiveRead(std::uint32_t handle) {
#if defined(_WIN32)
    DWORD transferred = handle;
#else
    std::uint32_t transferred = handle;
#endif
    if (!CheckArchiveHandleOpen(handle) || !Archive_DoesHandleExist(handle))
        return false;
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry)
        return false;
#if defined(_WIN32)
    if (!GetOverlappedResult(reinterpret_cast<HANDLE>(entry->platformHandle), reinterpret_cast<OVERLAPPED *>(&entry->overlapped), &transferred, FALSE) &&
        GetLastError() != ERROR_IO_INCOMPLETE)
        return false;
#else
    if (entry->overlapped.internal != 0)
        return false;
    transferred = static_cast<std::uint32_t>(entry->overlapped.transferred);
#endif
    const bool wholeFile = transferred == GetArchiveFileSize(handle);
    const bool active = IsArchiveSourceDescActive(entry->source);
    if (wholeFile && !(entry->accessMode & AOF_SkipDecompression) && (!active || (IsArchiveSourceDescActive(entry->source) && entry->source.flags != 0))) {
        std::size_t size = transferred;
        std::uint32_t result;
        void *const decoded = d3t_DecompressContent(entry->bufferedData, &size, &result);
        if (decoded)
            Archive_BindBuffer(handle, decoded, size, false);
    }
    return true;
}

// @verified - p
bool PollArchiveIoCompletions() {
    bool pending = false;
    for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load(std::memory_order_seq_cst) != AJS_Pending)
            continue;
        pending = true;
        const std::uint32_t handle = request.archiveHandle;
        const ArchiveHandleEntry *entry = nullptr;
        if (CheckArchiveHandleOpen(handle) && Archive_DoesHandleExist(handle))
            entry = Archive_GetHandle(handle);
        if (entry && static_cast<std::uint32_t>(entry->overlapped.internal) == ARCHIVE_IO_PENDING)
            continue;
        if (request.archiveHandle != g_invalidArchiveHandle && Archive_GetHandle(request.archiveHandle)) {
            const bool succeeded = request.operation == 0 ? FinalizeCompletedArchiveRead(request.archiveHandle) : request.operation == 1;
            Archive_ClosePlatformHandle(request.archiveHandle);
            ArchiveHandleEntry *const completed = Archive_DoesHandleExist(request.archiveHandle) ? Archive_GetHandle(request.archiveHandle) : nullptr;
            if (completed)
                completed->readSucceeded = succeeded;
        }
        request.state.exchange(AJS_Free, std::memory_order_seq_cst);
    }
    return pending;
}

bool processPublishedReads() {
    const

        std::lock_guard lock(g_archiveWorkerMutex);
    if (g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst)) {
        while (PollArchiveIoCompletions()) {
        }
        return false;
    }
    bool processed = false;
    ArchiveSourceDesc source = g_currentArchiveSource;
    AsyncArchiveRequest *request = GetBestArchiveIoJob(source);
    while (request) {
        bool accepted = false;
        std::int32_t openResult = 1;
        if (request->archiveHandle != g_invalidArchiveHandle && Archive_GetHandle(request->archiveHandle)) {
            g_currentArchiveSource = request->source;
            if (request->operation == 0) {
                openResult = OpenArchiveSlotBackingFile(request->archiveHandle, request->path, request->flags | AOF_Read);
                if (openResult == 0) {
                    if (request->size == 0 && request->offset == 0) {
                        accepted = BeginArchiveReadChecked(request->archiveHandle);
                    } else {
                        if (request->size == 0)
                            request->size = GetArchiveFileSize(request->archiveHandle) - request->offset;
                        accepted = BeginArchiveRead(request->archiveHandle, request->offset, request->size);
                    }
                }
            } else if (request->operation == 1) {
                openResult = OpenArchiveSlotBackingFile(request->archiveHandle, request->path, request->flags | AOF_Write);
                if (openResult == 0)
                    accepted = BeginArchiveWrite(request->archiveHandle, request->offset);
            }
        }
        processed = true;
        if (openResult == 2) {
            request->state.exchange(AJS_Queued, std::memory_order_seq_cst);
            break;
        }
        if (!accepted) {
            ArchiveHandleEntry *const failed = Archive_DoesHandleExist(request->archiveHandle) ? Archive_GetHandle(request->archiveHandle) : nullptr;
            if (failed)
                failed->readSucceeded = 0;

            request->state.exchange(AJS_Free, std::memory_order_seq_cst);
        }
        PollArchiveIoCompletions();
        source = g_currentArchiveSource;
        request = GetBestArchiveIoJob(source);
    }
    const bool pending = PollArchiveIoCompletions();
    if (g_d3tShutdownThreadFlag.load(std::memory_order_seq_cst)) {
        while (PollArchiveIoCompletions()) {
        }
    }
    return processed || pending;
}

class ArchiveReadGuard {
  public:
    ArchiveReadGuard() {
        g_archiveHandleReaders.fetch_add(1);
        while (g_archiveHandleWriter.load() != 0) {
        }
    }

    ~ArchiveReadGuard() {
        g_archiveHandleReaders.fetch_sub(1);
    }
};

void acquireWriter(std::atomic<std::int32_t> &readers, std::atomic<std::uint8_t> &writer) {
    for (;;) {
        while (readers.load() != 0) {
        }
        std::uint8_t expected = 0;
        if (!writer.compare_exchange_strong(expected, 1)) {
            continue;
        }
        if (readers.load() == 0) {
            return;
        }
        writer.store(0);
    }
}

void releaseBufferedSize(std::uint64_t size) {
    acquireWriter(g_openFileSizeReaders, g_openFileSizeWriter);
    g_openFileSizes = g_openFileSizes > size ? g_openFileSizes - size : 0;
    g_openFileSizeWriter.store(0);
}

void releaseBufferedData(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle)) {
        return;
    }
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry) {
        return;
    }
    if (!entry->preserveBufferedData) {
        releaseBufferedSize(entry->bufferedDataSize);
        std::free(entry->bufferedData);
    }
    entry->bufferedData = nullptr;
    entry->bufferedDataSize = 0;
    entry->preserveBufferedData = 0;
}

} // namespace

AsyncArchiveRequest *GetBestArchiveIoJob(const ArchiveSourceDesc &source) {
    AsyncArchiveRequest *selected = nullptr;
    std::uint64_t bestScore = UINT64_MAX;
    for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load() != 2)
            continue;
        const std::uint64_t sourceDistance = source.sourceIndex >= request.source.sourceIndex ?
            static_cast<std::uint64_t>(source.sourceIndex) - request.source.sourceIndex :
            static_cast<std::uint64_t>(request.source.sourceIndex) - source.sourceIndex;
        const std::uint64_t seek = source.sourceOffset <= request.source.sourceOffset ? request.source.sourceOffset - source.sourceOffset :
                                                                                        10U * (source.sourceOffset - request.source.sourceOffset);
        const std::uint64_t score = (request.flags & AIF_Urgent) ? 0 : kArchiveSourceDistanceCost * sourceDistance + 1U + seek;
        if (g_openFileSizes.load(std::memory_order_relaxed) > kArchiveBufferBudget && !(request.flags & AIF_Urgent) && request.operation != 1) {
            if (!request.source.fileSize)
                continue;
            ArchiveHandleEntry *const handle = Archive_DoesHandleExist(request.archiveHandle) ? Archive_GetHandle(request.archiveHandle) : nullptr;
            if (!handle || handle->bufferedDataSize < request.source.fileSize)
                continue;
        }
        if (score < bestScore) {
            selected = &request;
            bestScore = score;
            if (!score)
                break;
        }
    }
    std::int32_t expected = 2;
    return selected && selected->state.compare_exchange_strong(expected, 3) ? selected : nullptr;
}

void LoadContent(std::uint16_t threadIndex) {
    if (!processPublishedReads() && threadIndex < g_workerThreadCount) {
        std::unique_lock lock(g_archiveWorkerMutex);
        g_archiveWorkerCondition.wait_for(lock, std::chrono::milliseconds(100));
    }
}

ArchiveHandleEntry *Archive_GetHandle(std::uint32_t handle) {
    ArchiveReadGuard guard;
    const std::uint32_t index = handle & 0x7ff;
    const std::uint32_t generation = handle & 0xfffff800;
    return g_archiveHandles.handleIds[index] == generation ? &g_archiveHandles.entries[index] : nullptr;
}

bool Archive_DoesHandleExist(std::uint32_t handle) {
    if (handle == g_invalidArchiveHandle) {
        return false;
    }
    return Archive_GetHandle(handle) != nullptr;
}

std::uint32_t Archive_AllocateHandle(std::uint8_t archiveLookupFlag) {
    acquireWriter(g_archiveHandleReaders, g_archiveHandleWriter);
    const std::uint32_t firstIndex = g_nextArchiveHandleIndex;
    std::uint32_t index = firstIndex;
    std::uint32_t handle = 0;
    for (;;) {
        const std::uint32_t generation = g_archiveHandles.handleIds[index] >> 11;
        if (generation < 0x100000) {
            const std::uint32_t nextGeneration = generation + 1;
            const std::uint32_t wrappedGeneration = (nextGeneration & 0xfff00000) == 0x100000 ? 1 : nextGeneration;
            const std::uint32_t activeGeneration = (wrappedGeneration | 0xfff00000) << 11;
            g_archiveHandles.handleIds[index] = activeGeneration;
            g_nextArchiveHandleIndex = (index + 1) & 0x7ff;
            handle = activeGeneration | index;
            break;
        }
        index = (index + 1) & 0x7ff;
        if (index == firstIndex) {
            break;
        }
    }
    g_archiveHandleWriter.store(0);

    if (Archive_DoesHandleExist(handle)) {
        ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
        if (entry) {
            std::memset(entry, 0, sizeof(*entry));
            entry->accessMode = 0;
            entry->archiveLookupFlag = archiveLookupFlag;
        }
    }
    return handle;
}

std::int32_t Archive_ClosePlatformHandle(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle)) {
        return 0;
    }
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry) {
        return 0;
    }
    if (entry->platformHandle == -1) {
        return 0;
    }
    const bool result = Platform_CloseFileHandle(entry->platformHandle);
    entry->platformHandle = 0;
    return result ? 1 : 0;
}

// @verified - p
void *Archive_GetHandleBuffer(std::uint32_t handle, std::uint64_t size) {
    if (!Archive_DoesHandleExist(handle))
        return nullptr;
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry)
        return nullptr;
    if (entry->bufferedData && entry->bufferedDataSize != size)
        releaseBufferedData(handle);
    if (!entry->bufferedData) {
        void *const data = std::malloc(static_cast<std::size_t>(size));
        entry->bufferedDataSize = size;
        entry->bufferedData = data;
        entry->preserveBufferedData = 0;
        acquireWriter(g_openFileSizeReaders, g_openFileSizeWriter);
        g_openFileSizes += size;
        g_openFileSizeWriter.store(0);
    }
    return entry->bufferedData;
}

void *Archive_BindBuffer(std::uint32_t handle, void *destination, std::uint64_t size, bool preserveBuffer) {
    if (!Archive_DoesHandleExist(handle)) {
        return nullptr;
    }
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry) {
        return nullptr;
    }
    if (entry->bufferedData != destination) {
        releaseBufferedData(handle);
        entry->bufferedData = destination;
        entry->bufferedDataSize = size;
    }
    entry->preserveBufferedData = preserveBuffer ? 1 : 0;
    if (!preserveBuffer) {
        acquireWriter(g_openFileSizeReaders, g_openFileSizeWriter);
        g_openFileSizes += size;
        g_openFileSizeWriter.store(0);
    }
    return entry->bufferedData;
}

void *Archive_DetachBuffer(std::uint32_t handle) {
    if (!Archive_DoesHandleExist(handle)) {
        return nullptr;
    }
    ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
    if (!entry) {
        return nullptr;
    }
    void *const data = entry->bufferedData;
    if (!entry->preserveBufferedData) {
        releaseBufferedSize(entry->bufferedDataSize);
    }
    entry->bufferedData = nullptr;
    entry->bufferedDataSize = 0;
    entry->preserveBufferedData = 0;
    return data;
}

void Archive_QueueRead(std::uint32_t handle, std::int32_t operation, const char *path, std::uint64_t offset, std::uint64_t size, std::uint8_t flags) {
    if (!path || !Archive_DoesHandleExist(handle)) {
        return;
    }

    AsyncArchiveRequest *request = nullptr;
    while (!request) {
        for (AsyncArchiveRequest &candidate : g_asyncArchiveRequests) {
            std::int32_t expected = 0;
            if (candidate.state.compare_exchange_strong(expected, 1)) {
                request = &candidate;
                break;
            }
        }
    }

    request->archiveHandle = handle;
    request->operation = operation;
    request->offset = offset;
    request->size = size;
    request->flags = flags;
    std::strcpy(request->path, path);

    ArchiveData_GetSourceDesc(path, request->source);
    request->state.store(2);
    g_archiveWorkerCondition.notify_one();
}

bool Archive_HasPendingRead(std::uint32_t handle) {
    for (const AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load() != 0 && request.archiveHandle == handle) {
            return true;
        }
    }
    return false;
}

bool Archive_FileExists(const char *path) {
    ArchiveSourceDesc source;
    ArchiveData_GetSourceDesc(path, source);
    if (IsArchiveSourceDescActive(source))
        return true;
#if defined(_WIN32)
    while (*path == '/' || *path == '\\')
        ++path;
    const DWORD attributes = GetFileAttributesA(path);
    return attributes != INVALID_FILE_ATTRIBUTES && (attributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
#else
    struct stat attributes {};
    return stat(path, &attributes) == 0 && !S_ISDIR(attributes.st_mode);
#endif
}

// @verified - p
void *LoadLegacyFile(const char *path, std::uint64_t *size) {
    if (!Archive_FileExists(path))
        return nullptr;
    void *data = nullptr;
    const std::uint32_t handle = Archive_AllocateHandle(0);
    if (Archive_DoesHandleExist(handle)) {
        ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
        if (entry)
            entry->platformHandle = -1;
    }
    std::uint32_t releasedHandle = handle;
    if (Archive_DoesHandleExist(handle)) {
        Archive_QueueRead(handle, 0, path, 0, 0, AOF_Read | AIF_Urgent);
        while (Archive_HasPendingRead(handle)) {
        }
        std::uint64_t loadedSize = 0;
        if (Archive_DoesHandleExist(handle)) {
            const ArchiveHandleEntry *const entry = Archive_GetHandle(handle);
            if (entry)
                loadedSize = entry->bufferedDataSize;
        }
        data = Archive_DetachBuffer(handle);
        if (data && size)
            *size = loadedSize;
        ReadFile_0(0, &releasedHandle);
    }
    return data;
}
void ReadFile_0(std::uint64_t, std::uint32_t *fileState) {
    if (!fileState) {
        return;
    }
    // A canceled cached read must stop owning its destination before the
    // caller can reuse it. Serialize with an in-flight platform read, and
    // retire queued requests before invalidating the handle generation.
    std::lock_guard lock(g_archiveWorkerMutex);
    for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load() == 2 && request.archiveHandle == *fileState) {
            request.state.store(0);
        }
    }
    if (Archive_GetHandle(*fileState)) {
        releaseBufferedData(*fileState);
        Archive_ClosePlatformHandle(*fileState);

        acquireWriter(g_archiveHandleReaders, g_archiveHandleWriter);
        const std::uint32_t index = *fileState & 0x7ff;
        const std::uint32_t generation = *fileState & 0xfffff800;
        std::uint32_t &activeGeneration = g_archiveHandles.handleIds[index];
        if (activeGeneration == generation) {
            activeGeneration &= ~0x80000000U;
        }
        g_archiveHandleWriter.store(0);
    }
    *fileState = g_invalidArchiveHandle;
}

void ReadCachedFile(void *file) {
    auto *const handle = static_cast<LargeFileHandle *>(file);
    if (!handle || handle->entryIndex >= 2048) {
        return;
    }

    LargeFileDataEntry &entry = g_largeFileData[static_cast<std::size_t>(handle->entryIndex)];
    if (entry.parent) {
        --entry.parent->childReferenceCount;
    }
    if (Archive_DoesHandleExist(entry.archiveHandle)) {
        ReadFile_0(0, &entry.archiveHandle);
    }
    std::free(entry.tocEntries);
    entry.tocEntries = nullptr;
    entry.inUse = 0;
}

std::int64_t GetFileFromCachedBuffer(LargeFileHandle *handle, std::uint64_t blockCount, void *destination, std::int32_t forceWait) {
    const std::uint64_t size = blockCount << 11;
    if (!handle || handle->entryIndex < 0 || handle->entryIndex >= 2048) {
        return 0;
    }

    LargeFileDataEntry &entry = g_largeFileData[static_cast<std::size_t>(handle->entryIndex)];
    Archive_BindBuffer(entry.archiveHandle, destination, size, true);

    const char *path = entry.cacheKey;
    std::uint64_t offset = 0;
    if (entry.parent) {
        path = entry.parent->cacheKey;
        offset = static_cast<std::uint64_t>(handle->offsetBytes) + entry.parent->tocEntries[entry.tocIndex].offset;
    }
    Archive_QueueRead(entry.archiveHandle, 0, path, offset, size, 0x21);





    if (forceWait) {
        while (Archive_HasPendingRead(entry.archiveHandle)) {
            std::this_thread::yield();
        }
    }
    return 1;
}

std::int64_t LargeFile_LoadADFSArchiveToc(std::int32_t fileId, const char *path) {
    LargeFileDataEntry *freeEntry = nullptr;
    std::int32_t freeIndex = 0;
    for (std::int32_t index = 0; index < 2048; ++index) {
        LargeFileDataEntry &entry = g_largeFileData[static_cast<std::size_t>(index)];
        if (entry.inUse && entry.handle.fileId == fileId) {
            if (std::strcmp(path, entry.cacheKey) == 0) {
                return 0;
            }
            ReadCachedFile(&entry.handle);
        }
        if (!entry.inUse && !freeEntry) {
            freeEntry = &entry;
            freeIndex = index;
        }
    }
    if (!freeEntry) {
        return -4;
    }

    std::memset(freeEntry, 0, sizeof(*freeEntry));
    std::uint32_t header[2]{};
    if (!ArchiveData_Read(path, 0, header, sizeof(header))) {
        return -4;
    }

    const std::uint32_t entryCount = header[1];
    if (entryCount > SIZE_MAX / sizeof(LargeFileTocEntry)) {
        return -4;
    }
    auto *const toc = static_cast<LargeFileTocEntry *>(std::malloc(static_cast<std::size_t>(entryCount) * sizeof(LargeFileTocEntry)));
    if (!toc && entryCount != 0) {
        return -4;
    }
    if (entryCount != 0 && !ArchiveData_Read(path, sizeof(header), toc, static_cast<std::uint64_t>(entryCount) * sizeof(LargeFileTocEntry))) {
        std::free(toc);
        return -4;
    }

    freeEntry->inUse = 1;
    std::strcpy(freeEntry->cacheKey, path);
    freeEntry->archiveHandle = g_invalidArchiveHandle;
    freeEntry->handle.field_00 = 0;
    freeEntry->handle.entryIndex = freeIndex;
    freeEntry->handle.fileId = fileId;
    freeEntry->tocHeader = header[0];
    freeEntry->tocEntryCount = entryCount;
    freeEntry->tocEntries = toc;
    return 0;
}

std::int32_t CheckFileInfo(void *file) {
    if (!file) {
        return 3;
    }

    const auto &handle = *static_cast<const LargeFileHandle *>(file);
    if (handle.entryIndex < 0 || handle.entryIndex >= 2048) {
        return 3;
    }

    const LargeFileDataEntry &entry = g_largeFileData[static_cast<std::size_t>(handle.entryIndex)];
    if (!entry.inUse) {
        return 3;
    }

    for (const AsyncArchiveRequest &request : g_asyncArchiveRequests) {
        if (request.state.load() != 0 && request.archiveHandle == entry.archiveHandle) {
            return 2;
        }
    }
    const ArchiveHandleEntry *const archive = Archive_GetHandle(entry.archiveHandle);
    return archive && archive->readSucceeded ? 3 : 1;
}

LargeFileHandle *LargeFile_OpenPathHandle(const char *path) {
    if (!path)
        return nullptr;
    const std::size_t pathSize = std::strlen(path) + 1;
    if (pathSize > sizeof(LargeFileDataEntry::cacheKey))
        return nullptr;
    for (std::size_t index = 0; index < g_largeFileData.size(); ++index) {
        LargeFileDataEntry &entry = g_largeFileData[index];
        if (entry.inUse)
            continue;
        entry = {};
        const std::uint32_t archive = Archive_AllocateHandle(0);
        ArchiveHandleEntry *const archiveEntry = Archive_GetHandle(archive);
        if (!archiveEntry || !Archive_DoesHandleExist(archive))
            return nullptr;
        archiveEntry->platformHandle = -1;
        entry.archiveHandle = archive;
        std::memcpy(entry.cacheKey, path, pathSize);
        std::memcpy(entry.openPath, path, pathSize);
        entry.inUse = 1;
        entry.handle.entryIndex = static_cast<std::int32_t>(index);
        entry.handle.fileId = -1;
        return &entry.handle;
    }
    return nullptr;
}

LargeFileHandle *LargeFile_OpenTocEntryHandle(std::int32_t fileId, std::int32_t tocIndex) {
    LargeFileDataEntry *parent = nullptr;
    for (LargeFileDataEntry &candidate : g_largeFileData) {
        if (candidate.inUse && candidate.handle.fileId == fileId) {
            parent = &candidate;
            break;
        }
    }
    if (!parent || tocIndex < 0 || tocIndex >= static_cast<std::int32_t>(parent->tocEntryCount)) {
        return nullptr;
    }

    LargeFileDataEntry *child = nullptr;
    std::int32_t childIndex = 0;
    for (LargeFileDataEntry &candidate : g_largeFileData) {
        if (!candidate.inUse) {
            child = &candidate;
            break;
        }
        ++childIndex;
    }
    if (!child) {
        return nullptr;
    }

    std::memset(child, 0, sizeof(*child));
    ++parent->childReferenceCount;
    child->inUse = 1;
    child->archiveHandle = Archive_AllocateHandle(0);
    if (ArchiveHandleEntry *const archive = Archive_GetHandle(child->archiveHandle)) {
        archive->platformHandle = -1;
    }
    child->handle.field_00 = 0;
    child->parent = parent;
    child->tocIndex = tocIndex;
    child->handle.entryIndex = childIndex;
    child->handle.fileId = -2;
    return &child->handle;
}

std::int64_t LargeFile_GetSizeInBlocks(LargeFileHandle *handle) {
    std::uint32_t size = 0;
    if (handle && handle->entryIndex >= 0 && handle->entryIndex < 2048) {
        const LargeFileDataEntry &entry = g_largeFileData[static_cast<std::size_t>(handle->entryIndex)];
        if (entry.parent) {
            size = entry.parent->tocEntries[entry.tocIndex].size;
        } else {
            size = static_cast<std::uint32_t>(ArchiveData_GetFileSize(entry.openPath));
        }
    }
    return static_cast<std::int64_t>((size + 2047) / 2048);
}

FileState *Archive_GetNextUnusedFileState() {
    const std::uint32_t handle = Archive_AllocateHandle(0);
    if (Archive_DoesHandleExist(handle)) {
        if (ArchiveHandleEntry *const entry = Archive_GetHandle(handle)) {
            entry->platformHandle = -1;
        }
    }
    if (!Archive_DoesHandleExist(handle)) {
        return nullptr;
    }

    for (FileState &file : g_files) {
        const std::uint32_t existingHandle = file.handle;
        if (existingHandle == g_invalidArchiveHandle || !Archive_GetHandle(existingHandle)) {
            file.handle = handle;
            return &file;
        }
    }
    return nullptr;
}

} // namespace shenmue::d3t
