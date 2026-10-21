#include "s1/file_loader.hpp"
#include "s1/archive_data.hpp"
#include "s1/hlib.hpp"

#include "s1/collision.hpp"
#include "s1/player_control.hpp"
#include "s1/task/cycle_man.hpp"
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <thread>

namespace shenmue::s1 {

void RenderScreen();
bool g_skipArchiveReadWait = false;

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
    return ArchiveData_GetFileSize(path);
}

void OpenFile(FileState *file, void *destination, std::int32_t size) {
    if (!Archive_DoesHandleExist(file->handle)) {
        return;
    }
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
    char filenameCopy[260];
    char path[272];
    std::strcpy(filenameCopy, filename);
    std::sprintf(path, "%s/%s", directory, filenameCopy);
    return OpenFileInternal(path);
}

std::int32_t PrepareDirectoryPathForFile(FileState *file, const char *directory, const char *filename) {
    if (!Archive_DoesHandleExist(file->handle)) {
        return 0;
    }
    char filenameCopy[260];
    std::strcpy(filenameCopy, filename);
    std::sprintf(file->path, "%s/%s", directory, filenameCopy);
    file->offset = 0;
    return 1;
}

std::int64_t shenLoadFile(const char *directory, const char *filename, void *destination, std::int32_t size) {
    FileState *const file = Archive_GetNextUnusedFileState();
    if (!file)
        return 0;

    char filenameCopy[260]{};
    if (!CopyNormalizedFilename(filenameCopy, filename))
        return 0;
    if (Archive_DoesHandleExist(file->handle)) {
        const int length = std::snprintf(file->path, sizeof(file->path), "%s/%s", directory, filenameCopy);
        if (length < 0 || static_cast<std::size_t>(length) >= sizeof(file->path))
            return 0;
        file->offset = 0;
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
        PCCTL_BeginLoadHold();
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

} // namespace shenmue::s1
