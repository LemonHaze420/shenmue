#include "s1/file_loader.hpp"
#include "s1/archive_data.hpp"
#include "s1/hlib.hpp"
#include <array>
#include <vector>

#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/task/cycle_man.hpp"
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <limits>
#include <thread>

namespace shenmue::s1 {

void RenderScreen();
bool g_skipArchiveReadWait = false;
#if SHENMUE_IS_DEMO
std::int32_t g_simmError = 0;
alignas(32) std::array<std::byte, kSimmStartupBytes> g_simmStartupData{};
std::array<const void *, 2> g_simmRoots{g_simmStartupData.data(), nullptr};
std::int32_t g_simmCompressedFileActive = 0;

namespace {

struct SimmFileBinding {
    std::uint32_t handle;
    const std::byte *source;
    std::uint32_t size;
    std::uint32_t remaining;
    bool compressed;
};
std::array<SimmFileBinding, std::tuple_size_v<decltype(g_files)>> g_simmFileBindings{};
std::vector<std::byte> g_simmDecodedData;
} // namespace

void decompress(void *source, void *destination, std::int64_t, std::int64_t, std::uint32_t, std::int64_t, std::uint32_t);

void ReadFile_0(std::uint64_t unused, std::uint32_t *fileState) {
    if (fileState) {
        for (SimmFileBinding &binding : g_simmFileBindings) {
            if (binding.source && binding.handle == *fileState) {
                if (binding.compressed)
                    g_simmCompressedFileActive = 0;
                binding = {};
            }
        }
    }
    d3t::ReadFile_0(unused, fileState);
}

void SIMM_LoadStartupData() {
    std::memset(g_simmStartupData.data(), 0, g_simmStartupData.size());
    const std::uint32_t size = static_cast<std::uint32_t>(shenOpenFile("scene/80", "SIMMDATA.BIN"));
    shenLoadFile("scene/80", "SIMMDATA.BIN", g_simmStartupData.data(), static_cast<std::int32_t>(size));
}

// @verified - d
std::int32_t SIMM_UppercaseAscii(std::int32_t value) {
    if (value >= 'a' && value <= 'z')
        value -= 'a' - 'A';
    return value;
}

// @verified - d
std::int32_t SIMM_CompareNames(const char *left, const char *right) {
    for (;;) {
        const std::int8_t a = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*left++)));
        const std::int8_t b = static_cast<std::int8_t>(SIMM_UppercaseAscii(static_cast<std::int8_t>(*right++)));
        if (b < a)
            return 1;
        if (a < b)
            return -1;
        if (!a)
            return 0;
    }
}

// @verified - d
const std::byte *SIMM_GetRoot(const void *buffer) {
    if (buffer) {
        std::uint32_t identifier;
        std::memcpy(&identifier, buffer, sizeof(identifier));
        if (identifier == 0x4d4d4953)
            return static_cast<const std::byte *>(buffer) + 4;
    }
    return nullptr;
}

// @verified - d
const SimmEntry *SIMM_FindEntry(const std::byte *table, const char *name) {
    while (*table != std::byte{0}) {
        const SimmEntry *const entry = reinterpret_cast<const SimmEntry *>(table);
        if (!SIMM_CompareNames(entry->name, name))
            return entry;
        table += sizeof(SimmEntry);
    }
    return nullptr;
}

// @verified - d
const std::byte *SIMM_FindDirectory(const std::byte *root, const char *path) {
    if (!path)
        path = "";
    else if (*path == '/' || *path == '\\')
        ++path;
    const std::byte *table = root;
    while (*path) {
        if (!table)
            return nullptr;
        char component[20];
        std::int32_t length = 0;
        do {
            component[length] = *path;
            if (!component[length])
                break;
            if (component[length] == '/' || component[length] == '\\') {
                ++path;
                break;
            }
            ++length;
            ++path;
        } while (length < 13);
        component[length] = '\0';
        const SimmEntry *const entry = SIMM_FindEntry(table, component);
        if (!entry || entry->type != 1) {
            g_simmError = 1;
            return nullptr;
        }
        table = root + entry->offset;
    }
    return table;
}

// @verified - d
const SimmEntry *SIMM_ResolveFile(const std::byte *root, const char *directory, const char *filename) {
    const std::byte *const table = SIMM_FindDirectory(root, directory);
    return table ? SIMM_FindEntry(table, filename) : nullptr;
}

// @verified - d
std::int32_t SIMM_GetEntrySize(const void *buffer, const char *directory, const char *filename, std::uint32_t *size) {
    const std::byte *const root = SIMM_GetRoot(buffer);
    const SimmEntry *const entry = root ? SIMM_ResolveFile(root, directory, filename) : nullptr;
    if (!entry) {
        *size = 0;
        return 0;
    }
    if (entry->type == 3) {
        std::memcpy(size, root + entry->offset + entry->size - 4, sizeof(*size));
        return 3;
    }
    *size = entry->size;
    return 2;
}

// @verified - d
const std::byte *SIMM_GetEntryData(const void *buffer, const char *directory, const char *filename) {
    const std::byte *const root = SIMM_GetRoot(buffer);
    const SimmEntry *const entry = root ? SIMM_ResolveFile(root, directory, filename) : nullptr;
    if (entry)
        return root + entry->offset;
    g_simmError = 2;
    return nullptr;
}

#endif

namespace {

bool CopyNormalizedFilename(char (&destination)[260], const char *filename) {
    if (!filename)
        return false;
    const int length = std::snprintf(destination, sizeof(destination), "%s", filename);
    if (length < 0 || static_cast<std::size_t>(length) >= sizeof(destination)) {
        return false;
    }
    if (char *const extension = std::strstr(destination, ".MT3"))
        std::strcpy(extension, ".MT5");
    return true;
}

} // namespace

std::uint64_t OpenFileInternal(const char *path) {
#if SHENMUE_IS_DEMO
    const char *separator = nullptr;
    for (const char *cursor = path; *cursor; ++cursor) {
        if (*cursor == '/' || *cursor == '\\')
            separator = cursor;
    }
    char directory[260]{};
    if (!separator || static_cast<std::size_t>(separator - path) < sizeof(directory)) {
        if (separator)
            std::memcpy(directory, path, separator - path);
        for (const void *const root : g_simmRoots) {
            std::uint32_t size = 0;
            if (root && SIMM_GetEntrySize(root, directory, separator ? separator + 1 : path, &size))
                return size;
        }
    }
#else
#endif
    return ArchiveData_GetFileSize(path);
}

void OpenFile(FileState *file, void *destination, std::int32_t size) {
    if (!Archive_DoesHandleExist(file->handle)) {
        return;
    }
#if SHENMUE_IS_DEMO
    for (SimmFileBinding &binding : g_simmFileBindings) {
        if (!binding.source || binding.handle != file->handle)
            continue;
        std::uint32_t count = static_cast<std::uint32_t>(size);
        if (count > binding.remaining)
            count = binding.remaining;
        if (count) {
            const std::byte *const source = binding.compressed ? g_simmDecodedData.data() : binding.source;
            std::memcpy(destination, source + binding.size - binding.remaining, count);
            binding.remaining -= count;
            file->offset += count;
        }
        return;
    }
#else
#endif
    if (size == -1) {
        size = static_cast<std::int32_t>(OpenFileInternal(file->path));
    }

    Archive_BindBuffer(file->handle, destination, static_cast<std::uint32_t>(size), true);
    Archive_QueueRead(file->handle, 0, file->path, file->offset, static_cast<std::uint32_t>(size), 0x21);
    if (!g_skipArchiveReadWait) {
        while (Archive_HasPendingRead(file->handle)) {
            std::this_thread::yield();
        }
    }
    file->offset += static_cast<std::uint32_t>(size);
}

void FileStatAndProcessIfExists(FileState *file) {
    for (;;) {
        bool pending = false;
        for (AsyncArchiveRequest &request : g_asyncArchiveRequests) {
            if (request.state.load() != 0 && request.archiveHandle == file->handle) {
                pending = true;
                break;
            }
        }
        if (!pending) {
            return;
        }
        RenderScreen();
    }
}

std::uint64_t shenOpenFile(const char *directory, const char *filename) {
#if SHENMUE_IS_DEMO
    for (const void *const root : g_simmRoots) {
        std::uint32_t size = 0;
        if (root && SIMM_GetEntrySize(root, directory, filename, &size))
            return size;
    }
    char filenameCopy[260];
    char path[272];
    std::strcpy(filenameCopy, filename);
    std::sprintf(path, "%s/%s", directory ? directory : "", filenameCopy);
#else
    char filenameCopy[260];
    char path[272];
    std::strcpy(filenameCopy, filename);
    std::sprintf(path, "%s/%s", directory, filenameCopy);
#endif
    return OpenFileInternal(path);
}

std::int32_t PrepareDirectoryPathForFile(FileState *file, const char *directory, const char *filename) {
    if (!Archive_DoesHandleExist(file->handle)) {
        return 0;
    }
    char filenameCopy[260];
    std::strcpy(filenameCopy, filename);
#if SHENMUE_IS_DEMO
    std::sprintf(file->path, "%s/%s", directory ? directory : "", filenameCopy);
    file->offset = 0;
    const std::ptrdiff_t index = file - g_files.data();
    if (index >= 0 && static_cast<std::size_t>(index) < g_simmFileBindings.size()) {
        SimmFileBinding &binding = g_simmFileBindings[static_cast<std::size_t>(index)];
        binding = {};
        for (const void *const buffer : g_simmRoots) {
            std::uint32_t size = 0;
            const std::int32_t type = buffer ? SIMM_GetEntrySize(buffer, directory, filename, &size) : 0;
            if (!type)
                continue;
            const std::byte *const source = SIMM_GetEntryData(buffer, directory, filename);
            binding = {.handle = file->handle, .source = source, .size = size, .remaining = size, .compressed = type == 3};
            if (binding.compressed) {
                g_simmCompressedFileActive = 1;
                g_simmDecodedData.resize(size);
                const SimmEntry *const entry = SIMM_ResolveFile(SIMM_GetRoot(buffer), directory, filename);
                decompress(const_cast<std::byte *>(source), g_simmDecodedData.data(), 0, 0, size, 0, entry->size);
            }
            break;
        }
    }
#else
    std::sprintf(file->path, "%s/%s", directory, filenameCopy);
    file->offset = 0;
#endif
    return 1;
}

std::int64_t shenLoadFile(const char *directory, const char *filename, void *destination, std::int32_t size) {
    FileState *const file = Archive_GetNextUnusedFileState();
    if (!file)
        return 0;

    char filenameCopy[260]{};
#if SHENMUE_IS_DEMO
    if (!CopyNormalizedFilename(filenameCopy, filename)) {
        ReadFile_0(0, &file->handle);
        return 0;
    }
    if (!PrepareDirectoryPathForFile(file, directory, filenameCopy)) {
        ReadFile_0(0, &file->handle);
        return 0;
#else
    if (!CopyNormalizedFilename(filenameCopy, filename))
        return 0;
    if (Archive_DoesHandleExist(file->handle)) {
        const int length = std::snprintf(file->path, sizeof(file->path), "%s/%s", directory, filenameCopy);
        if (length < 0 || static_cast<std::size_t>(length) >= sizeof(file->path))
            return 0;
        file->offset = 0;
#endif
    }

    OpenFile(file, destination, size);
    FileStatAndProcessIfExists(file);
    ReadFile_0(0, &file->handle);
    return 1;
}

void *ReadFile(const char *directory, const char *filename, std::int32_t token) {
#if SHENMUE_IS_PC
    const std::int32_t loadingEnabled = g_cm_unk_flag;
    CycleManager::LinkCyclesInStack(0);
    if (loadingEnabled)
        g_skipAuxiliaryCollisionRefresh = 1;
    const std::int64_t size = static_cast<std::int32_t>(shenOpenFile(directory, filename));
    void *destination = nullptr;
    if (size != 0) {
        destination = HLib_AllocHeapMemBlock(size, token);
        if (destination && !shenLoadFile(directory, filename, destination, static_cast<std::int32_t>(size))) {
            HLib_FreeTaskData(static_cast<HLib::TaskData *>(destination), 0);
            destination = nullptr;
        }
    }
    CycleManager::LinkCyclesInStack(loadingEnabled);
    if (loadingEnabled)
        navBeginLoadHold();
    return destination;
#else
    void *destination = nullptr;
    const std::uint32_t size = static_cast<std::uint32_t>(shenOpenFile(directory, filename));
    if (size == 0) {
        return nullptr;
    }

    destination = HLib_AllocHeapMemBlock(size, token);
    if (!destination) {
        return nullptr;
    }
    FileState *const file = Archive_GetNextUnusedFileState();
    if (!file) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(destination), 0);
        return nullptr;
    }

    char filenameCopy[260]{};
    if (!CopyNormalizedFilename(filenameCopy, filename)) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(destination), 0);
        return nullptr;
    }
    if (!PrepareDirectoryPathForFile(file, directory, filenameCopy)) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(destination), 0);
        return nullptr;
    }

    OpenFile(file, destination, static_cast<std::int32_t>(size));
    FileStatAndProcessIfExists(file);
    ReadFile_0(0, &file->handle);
    return destination;
#endif
}

#if SHENMUE_IS_DEMO
void *ReadLooseFile(const char *directory, const char *filename, std::int32_t token) {
    std::ifstream input(std::filesystem::path(directory) / filename, std::ios::binary | std::ios::ate);
    if (!input)
        return nullptr;
    const std::streampos size = input.tellg();
    if (size <= 0 || size > std::numeric_limits<std::int32_t>::max())
        return nullptr;
    void *const bytes = HLib_AllocHeapMemBlock(static_cast<std::int32_t>(size), token);
    if (!bytes)
        return nullptr;
    input.seekg(0, std::ios::beg);
    input.read(static_cast<char *>(bytes), static_cast<std::streamsize>(size));
    if (!input) {
        HLib_FreeTaskData(static_cast<HLib::TaskData *>(bytes), 0);
        return nullptr;
    }
    return bytes;
}
#endif

} // namespace shenmue::s1
