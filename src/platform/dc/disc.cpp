#include "platform/dc/disc.hpp"

#if SHENMUE_IS_DC

#include <sg_gd.h>
#if SHENMUE_S2_GCC295
#include <string.h>
#define SHENMUE_DISC_MEMCPY ::memcpy
#define SHENMUE_DISC_NULL 0
#else
#include <cstring>
#define SHENMUE_DISC_MEMCPY std::memcpy
#define SHENMUE_DISC_NULL 0
#endif

#if SHENMUE_IS_S2
namespace shenmue {
namespace s2 {
void ResetDiscAndExit();
#if SHENMUE_S2_GCC295
std::int32_t IsAnyDiscFileReading();
bool IsDiscWorkHandleBusy();
#endif
#else
namespace shenmue {
namespace s1 {
void CheckDiscTray();
#endif

extern "C" void syBtExit();
#if SHENMUE_S2_GCC295
FileState g_files[4];
#else
std::array<FileState, 4> g_files;
#endif

void WaitForDiscCommand() {
    while (gdFsReqDrvStat()) {
#if SHENMUE_IS_S2
        if (gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
#else
        CheckDiscTray();
#endif
    }
    GDFS const handle = gdFsGetSysHn();
    do {
#if SHENMUE_IS_S2
        if (gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
#else
        CheckDiscTray();
#endif
    } while (gdFsGetStat(handle) == GDD_STAT_BUSY);
}

#if SHENMUE_S2_GCC295 && !SHENMUE_IS_DEMO
// @verified - d
void RequestDiscRead(FileState *file, std::byte *destination, std::uint32_t bytes) {
    std::int32_t result;
    file->destination = destination;
    file->pendingBytes = bytes;
    g_discDirectoryError = 0;
    if (!(*static_cast<volatile std::uint32_t *>(&file->flags) & DiscFileUnalignedStart) && bytes >= 0x1000000) {
        do {
            result = gdFsReqRd32(static_cast<GDFS>(file->handle), 0x2000, destination);
            if (result == -10 || result == -14) {
                g_discDirectoryError = 1;
                return;
            }
            if (!result)
                WaitForDiscCommand();
        } while (!result);
        *static_cast<volatile std::uint32_t *>(&file->flags) &= ~DiscFileBuffered;
    } else if (!(*static_cast<volatile std::uint32_t *>(&file->flags) & DiscFileUnalignedStart) && bytes >= 0x800) {
        const std::int32_t sectors = bytes >> 11;
        do {
            result = gdFsReqRd32(static_cast<GDFS>(file->handle), sectors, destination);
            if (result == -10 || result == -14) {
                g_discDirectoryError = 1;
                return;
            }
            if (!result)
                WaitForDiscCommand();
        } while (!result);
        *static_cast<volatile std::uint32_t *>(&file->flags) &= ~DiscFileBuffered;
    } else {
        do {
            result = gdFsReqRd32(static_cast<GDFS>(file->handle), 1, file->sectorBuffer);
            if (result == -10 || result == -14) {
                g_discDirectoryError = 1;
                return;
            }
            if (!result)
                WaitForDiscCommand();
        } while (!result);
        *static_cast<volatile std::uint32_t *>(&file->flags) |= DiscFileBuffered;
    }
}
#else
void RequestDiscRead(FileState *file, std::byte *destination, std::uint32_t bytes) {
    file->destination = destination;
    file->pendingBytes = bytes;
    GDFS const handle = static_cast<GDFS>(file->handle);
    g_discDirectoryError = 0;
    const bool buffered = (file->flags & DiscFileUnalignedStart) || bytes < 0x800;
    const std::int32_t sectors = buffered ? 1 : bytes >= 0x1000000 ? 0x2000 : static_cast<std::int32_t>(bytes >> 11);
    for (;;) {
        const std::int32_t result = gdFsReqRd32(handle, sectors, buffered ? file->sectorBuffer : destination);
        if (result == -10 || result == -14) {
            g_discDirectoryError = 1;
            return;
        }
        if (result)
            break;
        WaitForDiscCommand();
    }
    if (buffered)
        file->flags |= DiscFileBuffered;
    else
        file->flags &= ~DiscFileBuffered;
}
#endif

// @verified - d
FileState *Archive_GetNextUnusedFileState() {
#if SHENMUE_S2_GCC295
    for (std::int32_t index = 0; index < 4; ++index)
        if (!g_files[index].flags)
            return &g_files[index];
#else
    for (FileState &file : g_files) {
        if (!file.flags)
            return &file;
    }
#endif
    return SHENMUE_DISC_NULL;
}

#if SHENMUE_S2_GCC295
void CloseFile(FileState *file) {
    if (file) {
        if (file->flags & DiscFileInUse) {
            if (!(file->flags & DiscFileMemory))
                gdFsClose(static_cast<GDFS>(file->handle));
            else if (file->flags & DiscFileCompressed)
                g_simmCompressedFileActive = 0;
        }
        file->flags = 0;
    }
}
#else
void CloseFile(FileState *file) {
    if (file) {
        if (file->flags & DiscFileInUse) {
            if (!(file->flags & DiscFileMemory))
                gdFsClose(static_cast<GDFS>(file->handle));
            else if (file->flags & DiscFileCompressed)
                g_simmCompressedFileActive = 0;
        }
        file->flags = 0;
    }
}
#endif

#if SHENMUE_S2_GCC295
void ProcessDiscFileReads() {
    std::byte *destination;
    std::uint32_t pending;
    std::int32_t offset;
    FileState *file = g_files;
    for (std::int32_t index = 0; index < 4; ++index, ++file) {
        if (!(file->flags & DiscFileInUse) || (file->flags & DiscFileMemory) || !(file->flags & DiscFileReading))
            continue;
        const std::uint32_t status = gdFsGetStat(static_cast<GDFS>(file->handle));
        if (status == 0x40 || gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
        if (status > 1)
            return;
        destination = file->destination;
        pending = file->pendingBytes;
        union {
            std::uint32_t unsignedOffset;
            std::int32_t signedOffset;
        } offsetBits;
        offsetBits.unsignedOffset = file->size - file->remaining - pending;
        offset = offsetBits.signedOffset % 0x800;
        if (file->flags & DiscFileBuffered) {
            if (!(file->flags & DiscFileUnalignedStart) && (file->flags & DiscFileUnalignedEnd)) {
                SHENMUE_DISC_MEMCPY(destination, file->sectorBuffer, pending);
                pending = 0;
            } else {
                SHENMUE_DISC_MEMCPY(destination, file->sectorBuffer + offset, 0x800 - offset);
                pending -= 0x800 - offset;
                destination -= offset - 0x800;
                file->flags &= ~DiscFileBuffered;
                file->flags &= ~DiscFileUnalignedStart;
            }
        } else {
            const std::uint32_t maximumRead = SHENMUE_IS_DEMO ? 0x20000 : 0x1000000;
            if (pending >= maximumRead) {
                pending -= maximumRead;
                destination += maximumRead;
            } else {
                const std::uint32_t sectors = pending >> 11;
                pending -= sectors * 0x800;
                destination += sectors * 0x800;
            }
        }
        if (!pending) {
            file->flags &= ~DiscFileReading;
            file->flags &= ~DiscFileUnalignedStart;
            file->flags &= ~DiscFileUnalignedEnd;
            if (!file->remaining)
                file->flags &= ~DiscFileBuffered;
        } else {
            RequestDiscRead(file, destination, pending);
            if (g_discDirectoryError)
                file->flags &= ~DiscFileReading;
        }
    }
}
#else
void ProcessDiscFileReads() {
    for (FileState &file : g_files) {
        if (!(file.flags & DiscFileInUse) || (file.flags & DiscFileMemory) || !(file.flags & DiscFileReading))
            continue;
        const std::int32_t status = gdFsGetStat(static_cast<GDFS>(file.handle));
#if SHENMUE_IS_S2
        if (status == 0x40 || gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
        if (static_cast<std::uint32_t>(status) > 1)
            return;
#else
        CheckDiscTray();
        if (status == GDD_STAT_READ || status == GDD_STAT_ERR)
            return;
#endif
        std::uint32_t pending = file.pendingBytes;
        std::byte *destination = file.destination;
#if SHENMUE_IS_S2
        const std::int32_t offset = static_cast<std::int32_t>(file.size - file.remaining - pending) % 0x800;
#else
        const std::uint32_t offset = (file.size - file.remaining - pending) & 0x7ff;
#endif
        if (!(file.flags & DiscFileBuffered)) {
            constexpr std::uint32_t maximumRead = SHENMUE_IS_DEMO ? 0x20000 : 0x1000000;
            if (pending < maximumRead) {
                destination += (pending >> 11) * 0x800;
                pending &= 0x7ff;
            } else {
                pending -= maximumRead;
                destination += maximumRead;
            }
        } else if (!(file.flags & DiscFileUnalignedStart) && (file.flags & DiscFileUnalignedEnd)) {
            SHENMUE_DISC_MEMCPY(destination, file.sectorBuffer, pending);
            pending = 0;
        } else {
            SHENMUE_DISC_MEMCPY(destination, file.sectorBuffer + offset, 0x800 - offset);
            destination += 0x800 - offset;
            pending -= 0x800 - offset;
            file.flags &= ~(DiscFileBuffered | DiscFileUnalignedStart);
        }
        if (!pending) {
            file.flags &= ~(DiscFileReading | DiscFileUnalignedStart | DiscFileUnalignedEnd);
            if (!file.remaining)
                file.flags &= ~DiscFileBuffered;
        } else {
            RequestDiscRead(&file, destination, pending);
            if (g_discDirectoryError)
                file.flags &= ~DiscFileReading;
        }
    }
}

#endif

#if SHENMUE_S2_GCC295
void WaitForDiscFileReads() {
    while (IsAnyDiscFileReading())
        ProcessDiscFileReads();
    while (IsDiscWorkHandleBusy()) {
        if (gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
        WaitForDiscCommand();
    }
}
#else
void WaitForDiscFileReads() {
    for (;;) {
        bool pending = false;
#if SHENMUE_S2_GCC295
        for (std::size_t index = 0; index < 4; ++index)
            pending |= (g_files[index].flags & DiscFileReading) != 0;
#else
        for (const FileState &file : g_files)
            pending |= (file.flags & DiscFileReading) != 0;
#endif
        if (!pending)
            break;
        ProcessDiscFileReads();
    }
    while (gdFsGetWorkHn()) {
#if SHENMUE_IS_S2
        if (gdFsGetDrvStat() == 6)
            ResetDiscAndExit();
#else
        CheckDiscTray();
#endif
        WaitForDiscCommand();
    }
}
#endif
}
} // namespace shenmue

#undef SHENMUE_DISC_MEMCPY
#undef SHENMUE_DISC_NULL

#endif
