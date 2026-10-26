#include "d3t/steam.hpp"

#include <atomic>
#include <cstring>
#include <mutex>
#include <string>
#include <windows.h>

namespace shenmue::d3t {
void **OriginalSteamContext();
bool IsSteamThread();
void RunOriginalSteamCallbacks();
void *OriginalExecutableFunction(std::uint32_t rva);

namespace {

struct StorageRequest {
    std::array<char, 260> path;
    void *buffer;
    std::uint32_t offset;
    std::uint32_t size;
    SteamStorageCompletion completion;
    void *user;
    bool write;
    bool started;
};

struct SynchronousResult {
    std::atomic<bool> done{false};
    bool success{};
};

std::mutex storageMutex;
StorageRequest request{};
bool pending;
bool accepting = true;

std::string StoragePath(const char *name) {
    RequireSteamInitialized();
    const std::string filename = std::filesystem::path(name).filename().string();
    if (filename.empty() || filename == "." || filename == ".." || filename.size() > 240)
        return {};
#if SHENMUE_IS_S1
    return ".\\sm1saves\\" + filename;
#else
    std::array<char, 260> path{};
    if (!reinterpret_cast<bool (*)(const char *, char *)>(OriginalExecutableFunction(ExeInfo.buildStoragePath))(filename.c_str(), path.data()))
        return {};
    return path.data();
#endif
}

std::int64_t CompleteStorage(std::int32_t stage, std::int32_t status, void *) {
    SteamStorageCompletion completion;
    void *user;
    {
        const std::lock_guard<std::mutex> lock(storageMutex);
        completion = request.completion;
        user = request.user;
        pending = false;
    }
    if (completion)
        completion(stage, status, user);
    return 0;
}

void CompleteSynchronous(std::int32_t, std::int32_t status, void *user) {
    SynchronousResult &result = *static_cast<SynchronousResult *>(user);
    result.success = status == 0;
    result.done.store(true, std::memory_order_release);
}

bool BeginStorage(const char *name, void *buffer, std::uint32_t offset, std::uint32_t size, bool write,
    SteamStorageCompletion completion, void *user) {
    const std::string path = StoragePath(name);
    if (path.empty())
        return false;
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(storageMutex);
            if (!accepting)
                return false;
            if (!pending) {
                request = {.path = {}, .buffer = buffer, .offset = offset, .size = size, .completion = completion, .user = user, .write = write};
                std::memcpy(request.path.data(), path.c_str(), path.size() + 1);
                pending = true;
                return true;
            }
        }
        RunSteamCallbacks();
        if (!IsSteamThread())
            Sleep(1);
    }
}

} // namespace

void RunSteamCallbacks() {
    if (!IsSteamInitialized() || !IsSteamThread())
        return;
    StorageRequest next{};
    bool start = false;
    {
        const std::lock_guard<std::mutex> lock(storageMutex);
        if (pending && !request.started) {
            request.started = true;
            next = request;
            start = true;
        }
    }
    if (start) {
        using Completion = std::int64_t (*)(std::int32_t, std::int32_t, void *);
        if (next.write) {
            using WriteFunction = std::int64_t (*)(void *, const char *, std::uint32_t, const void *, Completion);
            reinterpret_cast<WriteFunction>(OriginalExecutableFunction(ExeInfo.writeStorage))(
                nullptr, next.path.data(), next.size, next.buffer, CompleteStorage);
        } else {
            using ReadFunction = std::int64_t (*)(void *, const char *, std::uint32_t, std::uint32_t, void *, Completion);
            reinterpret_cast<ReadFunction>(OriginalExecutableFunction(ExeInfo.readStorage))(
                nullptr, next.path.data(), next.offset, next.size, next.buffer, CompleteStorage);
        }
    }
    RunOriginalSteamCallbacks();
}

void ShutdownSteamStorage() {
    if (!IsSteamInitialized())
        return;
    {
        const std::lock_guard<std::mutex> lock(storageMutex);
        accepting = false;
    }
    while (true) {
        {
            const std::lock_guard<std::mutex> lock(storageMutex);
            if (!pending)
                break;
        }
        RunSteamCallbacks();
        Sleep(1);
    }
}

bool SteamFileExists(const char *name) {
    const std::string path = StoragePath(name);
    if (path.empty())
        return false;
    void *const storage = OriginalSteamContext()[9];
    return reinterpret_cast<bool (*)(void *, const char *)>((*static_cast<void ***>(storage))[13])(storage, path.c_str());
}

std::int32_t GetSteamFileSize(const char *name) {
    const std::string path = StoragePath(name);
    if (path.empty())
        return -1;
    void *const storage = OriginalSteamContext()[9];
    void **const vtable = *static_cast<void ***>(storage);
    if (!reinterpret_cast<bool (*)(void *, const char *)>(vtable[13])(storage, path.c_str()))
        return -1;
    return reinterpret_cast<std::int32_t (*)(void *, const char *)>(vtable[15])(storage, path.c_str());
}

bool DeleteSteamFile(const char *name) {
    const std::string path = StoragePath(name);
    if (path.empty())
        return false;
    void *const storage = OriginalSteamContext()[9];
    return reinterpret_cast<bool (*)(void *, const char *)>((*static_cast<void ***>(storage))[6])(storage, path.c_str());
}

std::uint64_t GetSteamStorageAvailableBytes() {
    void *const storage = OriginalSteamContext()[9];
    std::uint64_t total{}, available{};
    const bool success = reinterpret_cast<bool (*)(void *, std::uint64_t *, std::uint64_t *)>(
        (*static_cast<void ***>(storage))[20])(storage, &total, &available);
    return success ? available : 0;
}

bool ReadSteamFileAsync(const char *name, void *destination, std::uint32_t offset, std::uint32_t size, SteamStorageCompletion completion, void *user) {
    return BeginStorage(name, destination, offset, size, false, completion, user);
}

bool WriteSteamFileAsync(const char *name, const void *source, std::uint32_t size, SteamStorageCompletion completion, void *user) {
    return BeginStorage(name, const_cast<void *>(source), 0, size, true, completion, user);
}

bool ReadSteamFile(const char *name, void *destination, std::uint32_t offset, std::uint32_t size) {
    SynchronousResult result;
    if (!ReadSteamFileAsync(name, destination, offset, size, CompleteSynchronous, &result))
        return false;
    while (!result.done.load(std::memory_order_acquire)) {
        RunSteamCallbacks();
        if (!IsSteamThread())
            Sleep(1);
    }
    return result.success;
}

bool WriteSteamFile(const char *name, const void *source, std::uint32_t size) {
    SynchronousResult result;
    if (!WriteSteamFileAsync(name, source, size, CompleteSynchronous, &result))
        return false;
    while (!result.done.load(std::memory_order_acquire)) {
        RunSteamCallbacks();
        if (!IsSteamThread())
            Sleep(1);
    }
    return result.success;
}

std::vector<std::byte> ReadSteamFileBytes(const char *name) {
    const std::int32_t size = GetSteamFileSize(name);
    if (size <= 0)
        return {};
    std::vector<std::byte> bytes(static_cast<std::size_t>(size));
    if (!ReadSteamFile(name, bytes.data(), 0, static_cast<std::uint32_t>(size)))
        bytes.clear();
    return bytes;
}

} // namespace shenmue::d3t
