#include "s2/file_loader_dc.hpp"

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
#include "platform/dc/am2_vector.hpp"
#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/directory_cache.hpp"
#include "s2/hlib.hpp"
#include "s2/load_title_dc.hpp"
#include "s2/npc_data_dc.hpp"
#include "platform/dc/sdk.hpp"

#include <sg_gd.h>
#include <string.h>
#define SHENMUE_LOADER_NULL 0
#define SHENMUE_LOADER_ALIGN __attribute__((aligned(32)))
#define SHENMUE_LOADER_INIT =
#define SHENMUE_LOADER_MEMCPY ::memcpy
#define SHENMUE_LOADER_STRCPY ::strcpy
#define SHENMUE_LOADER_STRCMP ::strcmp
#define SHENMUE_LOADER_STRSTR ::strstr
#define SHENMUE_LOADER_STRLEN ::strlen
#define SHENMUE_LOADER_STRNCPY ::strncpy
#define SHENMUE_LOADER_FILE_COUNT (sizeof(g_files) / sizeof(g_files[0]))
#define SHENMUE_LOADER_CONST_BYTES(value) static_cast<const std::byte *>(static_cast<const void *>(value))
#include <zlib.h>
#include "s2/startup_assets_dc.hpp"
#include "s2/event_overlay.hpp"
#include "s2/map_render_profile.hpp"
#include "s2/initialization_dc.hpp"
#include <cstring>

namespace shenmue {
namespace s2 {

void CloseAllFiles();
std::int32_t SelectDiscDirectory(const char *directory, std::int32_t mode);
std::uint32_t GetFilePosition(const FileState *file);
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size);
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName);

std::int32_t g_discFileResult = 0;
std::int32_t g_discDirectoryError = 0;
std::int32_t g_simmCompressedFileActive;
std::int32_t g_simmError = 0;
const void *g_simmRoots[2] = {reinterpret_cast<const void *>(0x0d000000), 0};
std::int32_t g_ryoModelVariant = 0;

namespace {

SHENMUE_LOADER_ALIGN std::byte discSectorBuffers[4][GDD_FS_SCTSIZE] SHENMUE_LOADER_INIT{};
char discDirectoryPath[128] SHENMUE_LOADER_INIT{};
DiscFileLocation discFileLocation SHENMUE_LOADER_INIT{};
extern "C" std::uint32_t shenmue_dc_loader_sentinel;
extern "C" void gdFsFinish();
AreaTableEntry initialAreaTable = initialAreaTableEntry;

} // namespace

std::int32_t g_loadedActNumber __attribute__((section(".data.dc_loaded_act"))) = 0;
AreaTableEntry *g_areaTableEntries = &initialAreaTable;
std::byte *g_doorTableData = 0;
std::byte *g_doorData;
std::byte *g_doorTableCursor = 0;
std::byte *g_doorDataCursor;

char g_lastFileDirectory[32] __attribute__((section(".data.dc_file_directory"))) SHENMUE_LOADER_INIT{};

char g_lastFileName[16] __attribute__((section(".data.dc_file_name"))) SHENMUE_LOADER_INIT{};

namespace {

SHENMUE_LOADER_ALIGN std::byte discWork[GDFS_WORK_SIZE(12)] SHENMUE_LOADER_INIT{};

SHENMUE_LOADER_ALIGN std::byte discDirectoryStorage[GDFS_DIRREC_SIZE(512)] SHENMUE_LOADER_INIT{};

void *discWorkArea = discWork;

// @verified - d
void SystemShutdownNullsub() {
}

} // namespace

void DecompressMemoryFile(const void *source, void *destination, std::uint32_t outputBytes) {
    z_stream stream SHENMUE_LOADER_INIT{};
    stream.next_in = static_cast<Bytef *>(const_cast<void *>(source));
    stream.avail_in = UINT32_MAX;
    stream.next_out = static_cast<Bytef *>(destination);
    stream.avail_out = outputBytes;
    if (inflateInit2(&stream, MAX_WBITS + 16) == Z_OK) {
        inflate(&stream, Z_FINISH);
        inflateEnd(&stream);
    }
}

// @verified - d
void ResetDiscAndExit() {
    ShutdownSystemLibrariesNullsub();
    WaitForBackupMemoryIdle();
    gdFsFinish();
    typedef void (*BootExit)(std::int32_t);
    const BootExit exit = *reinterpret_cast<BootExit *>(0x8c0000e0);
    exit(1);
}

void CheckDiscTray() {
    if (gdFsGetDrvStat() == 6)
        ResetDiscAndExit();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::byte *simmDecompressedData = reinterpret_cast<std::byte *>(0x0dc04000);
#endif

// @verified - d
void InitializeDiscFileSystem() {
    void *directory = reinterpret_cast<void *>((reinterpret_cast<std::uintptr_t>(discDirectoryStorage) + 3) & ~3U);
    std::int32_t count = 8;
    do {
        g_discFileResult = gdFsInit(12, discWorkArea, 512, directory);
        if (g_discFileResult == GDD_ERR_TRAYOPEND || g_discFileResult == GDD_ERR_UNITATTENT)
            ResetDiscAndExit();
        else if (!g_discFileResult)
            break;
    } while (--count > 0);
    if (!count)
        ResetDiscAndExit();
    std::byte *sectorBuffer = reinterpret_cast<std::byte *>((reinterpret_cast<std::uintptr_t>(discSectorBuffers) + 31) & ~31U);
    for (count = 0; count < 4; ++count) {
        g_files[count].flags = 0;
        g_files[count].sectorBuffer = sectorBuffer;
        sectorBuffer += GDD_FS_SCTSIZE;
    }
    InitializeDirectoryCache();
    CloseAllFiles();
}

// @verified - d
void CloseAllFiles() {
    std::strcpy(discDirectoryPath, "");
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(SHENMUE_LOADER_FILE_COUNT); ++index) {
        if (g_files[index].flags)
            CloseFile(&g_files[index]);
    }
    ResetDirectoryCache(0);
    g_simmCompressedFileActive = 0;
}

std::int32_t IsAnyDiscFileReading() {
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(SHENMUE_LOADER_FILE_COUNT); ++index) {
        if (g_files[index].flags & DiscFileReading)
            return 1;
    }
    return 0;
}

bool IsDiscWorkHandleBusy() {
    if (gdFsGetWorkHn())
        return true;
    else
        return false;
}

namespace {

void CopyMemoryFileData(void *destination, const void *source, std::uint32_t offset, std::uint32_t bytes, volatile std::uint32_t *flags) {
    SHENMUE_LOADER_MEMCPY(destination, SHENMUE_LOADER_CONST_BYTES(source) + offset, bytes);
    *flags &= ~DiscFileReading;
}

inline void InvalidateTableCode() {
    std::uint32_t status;
    asm volatile("stc sr,%0" : "=r"(status));
    status = (status & ~0xf0U) | 0xe0U;
    asm volatile("ldc %0,sr" : : "r"(status));
    FlushEventOperandCache();
    SetEventCacheControl(0x105);
    std::uint32_t restore;
    asm volatile("stc sr,%0" : "=r"(restore));
    restore &= ~0xf0U;
    asm volatile("ldc %0,sr" : : "r"(restore));
}

} // namespace

// @verified - d
void ResetSceneArchiveFileHandles() {
    CloseAllFiles();
}

// @verified - d
std::int32_t SelectDiscDirectoryChange(const char *directory) {
    return SelectDiscDirectory(directory, 1);
}

// @verified - d
std::int32_t SelectDiscDirectoryDefault(const char *directory) {
    return SelectDiscDirectory(directory, 0);
}

// @verified - d
std::int32_t SelectDiscDirectory(const char *directory, std::int32_t) {
    WaitForDiscFileReads();
    char normalized[256];
    NormalizeDirectoryPath(directory, normalized);
    if (SHENMUE_LOADER_STRCMP(discDirectoryPath, normalized)) {
        GDFS_DIRREC const cached = static_cast<GDFS_DIRREC>(GetCachedDiscDirectory(normalized));
        std::int32_t attempts = 3;
        if (cached) {
            gdFsSetDir(cached);
            SHENMUE_LOADER_STRCPY(discDirectoryPath, normalized);
        } else {
            g_discDirectoryError = 0;
            std::int32_t result;
            do {
                result = gdFsChangeDir(normalized);
                if ((result == -8 || result == -11) && attempts < 1) {
                    g_discDirectoryError = 1;
                    return 0;
                }
                if (result == -12 && attempts < 1) {
                    g_discDirectoryError = 1;
                    return 0;
                }
                if (result)
                    WaitForDiscCommand();
                --attempts;
            } while (result);
            SHENMUE_LOADER_STRCPY(discDirectoryPath, normalized);
        }
    }
    return 1;
}

const char *GetDiscFileLocationName(const DiscFileLocation *location) {
    if (location->kind & 2U)
        return location->memory.filename;
    else
        return location->disc.filename;
}

std::uint32_t GetDiscFileLocationSize(const DiscFileLocation *location) {
    if (location->kind & 2U)
        return location->memory.size;
    else
        return location->disc.size;
}

void ReleaseDiscFileLocation(DiscFileLocation *location) {
    location->kind = 0;
}

// @verified - d
void NormalizeDiscFilename(const char *filename, char *destination) {
    SHENMUE_LOADER_STRCPY(destination, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(destination, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    if (g_ryoModelVariant == 'M') {
        if (char *const model = SHENMUE_LOADER_STRSTR(destination, "RYO_M.MT7"))
            SHENMUE_LOADER_MEMCPY(model, "356_L.MT7", 10);
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
DiscFileLocation *FindDiscFileLocation(const char *directory, const char *filename) {
    char name[16], normalized[16], path[128];
    GDFS_DIRINFO info;
    std::uint32_t size;
    SHENMUE_LOADER_STRCPY(path, directory);
    std::int32_t length = 0;
    while (path[length])
        ++length;
    if (length && path[length - 1] == '/')
        path[length - 1] = '\0';
    DiscFileLocation *const location = &discFileLocation;
    NormalizeDiscFilename(filename, normalized);
    std::int32_t index = 0;
    while (normalized[index]) {
        name[index] = static_cast<char>(SIMM_UppercaseAscii(normalized[index]));
        ++index;
    }
    name[index] = '\0';
    if (shenmue_dc_loader_sentinel == 1) {
        for (index = 0; index < 2; ++index) {
            if (!g_simmRoots[index])
                continue;
            const std::int32_t kind = SIMM_GetEntrySize(g_simmRoots[index], path, name, &size);
            if (kind == 2) {
                location->kind = 3;
                location->sectorOrCompression = 0;
                location->memory.data = SIMM_GetEntryData(g_simmRoots[index], path, name);
            } else if (kind == 3) {
                location->kind = 3;
                location->sectorOrCompression = 1;
                location->memory.data = SIMM_GetEntryData(g_simmRoots[index], path, name);
            } else
                continue;
            location->memory.size = size;
            SHENMUE_LOADER_STRCPY(location->memory.filename, name);
            return location;
        }
    }
    if (SelectDiscDirectory(path, 1)) {
        index = gdFsGetDirInfo(name, &info);
        if (!index) {
            location->sectorOrCompression = static_cast<std::uint32_t>(info.fad);
            location->disc.size = static_cast<std::uint32_t>(info.fsize);
            SHENMUE_LOADER_STRCPY(location->disc.filename, name);
            location->kind = 1;
            return location;
        }
        SelectDiscDirectory(path, 1);
    }
    return SHENMUE_LOADER_NULL;
}
#else
// @verified - d
DiscFileLocation *FindDiscFileLocation(const char *directory, const char *filename) {
    char path[128];
    SHENMUE_LOADER_STRCPY(path, directory);
    const std::size_t length = SHENMUE_LOADER_STRLEN(path);
    if (length && path[length - 1] == '/')
        path[length - 1] = '\0';
    char normalized[20];
    char name[16];
    NormalizeDiscFilename(filename, normalized);
    std::size_t index = 0;
    while (normalized[index]) {
        name[index] = static_cast<char>(SIMM_UppercaseAscii(normalized[index]));
        ++index;
    }
    name[index] = '\0';
    if (shenmue_dc_loader_sentinel == 1) {
        for (std::size_t rootIndex = 0; rootIndex < sizeof(g_simmRoots) / sizeof(g_simmRoots[0]); ++rootIndex) {
            const void *const root = g_simmRoots[rootIndex];
            if (!root)
                continue;
            std::uint32_t size;
            const std::int32_t kind = SIMM_GetEntrySize(root, path, name, &size);
            if (kind == 2 || kind == 3) {
                discFileLocation.kind = 3;
                discFileLocation.sectorOrCompression = kind == 3 ? 1 : 0;
                discFileLocation.memory.data = SIMM_GetEntryData(root, path, name);
                discFileLocation.memory.size = size;
                SHENMUE_LOADER_STRCPY(discFileLocation.memory.filename, name);
                return &discFileLocation;
            }
        }
    }
    if (SelectDiscDirectory(path, 1)) {
        GDFS_DIRINFO info;
        if (!gdFsGetDirInfo(name, &info)) {
            discFileLocation.sectorOrCompression = static_cast<std::uint32_t>(info.fad);
            discFileLocation.disc.size = static_cast<std::uint32_t>(info.fsize);
            SHENMUE_LOADER_STRCPY(discFileLocation.disc.filename, name);
            discFileLocation.kind = 1;
            return &discFileLocation;
        }
        SelectDiscDirectory(path, 1);
    }
    return SHENMUE_LOADER_NULL;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t OpenFileFromLocation(FileState *file, const DiscFileLocation *location, std::int32_t sectorOffset, std::uint32_t maximumBytes) {
    std::uint32_t bytes;
    if (shenmue_dc_loader_sentinel == 1 && (location->kind & 2U)) {
        bytes = location->memory.size - static_cast<std::uint32_t>(sectorOffset) * 0x800;
        if (maximumBytes < bytes)
            bytes = maximumBytes;
        if (location->sectorOrCompression) {
            g_simmCompressedFileActive = 1;
            file->flags |= DiscFileInUse | DiscFileMemory | DiscFileCompressed;
            file->size = bytes;
            file->remaining = bytes;
            DecompressMemoryFile(location->memory.data, reinterpret_cast<std::byte *>(0x0dc04000), location->memory.size + 4);
            file->handle = simmDecompressedData + static_cast<std::uint32_t>(sectorOffset) * 0x800;
        } else {
            file->flags |= DiscFileInUse | DiscFileMemory;
            file->size = bytes;
            file->remaining = bytes;
            file->handle = const_cast<std::byte *>(location->memory.data + static_cast<std::uint32_t>(sectorOffset) * 0x800);
        }
    } else {
        bytes = location->disc.size;
        bytes -= static_cast<std::uint32_t>(sectorOffset) * 0x800;
        if (maximumBytes < bytes)
            bytes = maximumBytes;
        file->handle = gdFsOpen(location->disc.filename, SHENMUE_LOADER_NULL);
        if (!file->handle) {
            for (;;) {
                sectorOffset = gdFsGetDrvStat();
                if (sectorOffset == 6)
                    ResetDiscAndExit();
                if (sectorOffset == 2)
                    break;
                gdFsReqDrvStat();
            }
        }
        file->flags |= DiscFileInUse;
        file->size = bytes;
        file->remaining = bytes;
    }
    return 1;
}
#else
// @verified - d
std::int32_t OpenFileFromLocation(FileState *file, const DiscFileLocation *location, std::int32_t sectorOffset, std::uint32_t maximumBytes) {
    const std::uint32_t offset = static_cast<std::uint32_t>(sectorOffset) * 0x800;
    if (shenmue_dc_loader_sentinel == 1 && (location->kind & 2U)) {
        std::uint32_t bytes = location->memory.size - offset;
        if (maximumBytes < bytes)
            bytes = maximumBytes;
        const std::byte *source;
        if (!location->sectorOrCompression) {
            file->flags |= DiscFileInUse | DiscFileMemory;
            file->size = bytes;
            file->remaining = bytes;
            source = location->memory.data;
        } else {
            g_simmCompressedFileActive = 1;
            file->flags |= DiscFileInUse | DiscFileMemory | DiscFileCompressed;
            file->size = bytes;
            file->remaining = bytes;
            std::byte *const output = reinterpret_cast<std::byte *>(0x0dc04000);
            DecompressMemoryFile(location->memory.data, output, location->memory.size + 4);
            source = output;
        }
        file->handle = const_cast<std::byte *>(source + offset);
    } else {
        std::uint32_t bytes = location->disc.size - offset;
        if (maximumBytes < bytes)
            bytes = maximumBytes;
        file->handle = gdFsOpen(location->disc.filename, SHENMUE_LOADER_NULL);
        if (!file->handle) {
            for (;;) {
                const std::int32_t status = gdFsGetDrvStat();
                if (status == 6)
                    ResetDiscAndExit();
                if (status == 2)
                    break;
                gdFsReqDrvStat();
            }
        }
        file->flags |= DiscFileInUse;
        file->size = bytes;
        file->remaining = bytes;
    }
    return 1;
}
#endif

// @verified - d
std::int32_t OpenFile(FileState *file, const char *directory, const char *filename) {
    char normalized[16];
    NormalizeDiscFilename(filename, normalized);
    WaitForDiscFileReads();
    SHENMUE_LOADER_STRNCPY(g_lastFileDirectory, directory, 31);
    SHENMUE_LOADER_STRNCPY(g_lastFileName, normalized, 12);
    DiscFileLocation *const location = FindDiscFileLocation(directory, normalized);
    if (!location)
        return 0;
    const std::int32_t result = OpenFileFromLocation(file, location, 0, UINT32_MAX);
    ReleaseDiscFileLocation(location);
    return result;
}

// @verified - d
std::uint32_t CopyFileToHLibMemory(FileState *file, void *destination, std::uint32_t bytes) {
    WaitForDiscFileReads();
    if (bytes > file->remaining)
        bytes = file->remaining;
    if (!bytes)
        return 0;
    static_cast<volatile FileState *>(file)->flags |= DiscFileReading;
    if (static_cast<volatile FileState *>(file)->flags & (DiscFileUnknown01 | DiscFileMemory)) {
        CopyMemoryFileData(destination, file->handle, GetFilePosition(file), bytes, &file->flags);
        file->remaining -= bytes;
        return bytes;
    }
    std::uint32_t pending = bytes;
    const std::uint32_t offset = GetFilePosition(file) & 0x7ff;
    if (offset)
        static_cast<volatile FileState *>(file)->flags |= DiscFileUnalignedStart;
    if ((offset + bytes) & 0x7ff)
        static_cast<volatile FileState *>(file)->flags |= DiscFileUnalignedEnd;
    if (static_cast<volatile FileState *>(file)->flags & DiscFileBuffered) {
        std::uint32_t copied = 0x800 - offset;
        std::byte *source;
        if (copied > pending) {
            copied = pending;
            source = file->sectorBuffer;
        } else {
            static_cast<volatile FileState *>(file)->flags &= ~DiscFileBuffered;
            static_cast<volatile FileState *>(file)->flags &= ~DiscFileUnalignedStart;
            source = file->sectorBuffer;
        }
        SHENMUE_LOADER_MEMCPY(destination, source + offset, copied);
        destination = static_cast<std::byte *>(destination) + copied;
        pending -= copied;
    }
    file->remaining -= bytes;
    if (!pending) {
        static_cast<volatile FileState *>(file)->flags &= ~DiscFileReading;
        static_cast<volatile FileState *>(file)->flags &= ~DiscFileUnalignedStart;
        static_cast<volatile FileState *>(file)->flags &= ~DiscFileUnalignedEnd;
    } else {
        RequestDiscRead(file, static_cast<std::byte *>(destination), pending);
        if (g_discDirectoryError)
            static_cast<volatile FileState *>(file)->flags &= ~DiscFileReading;
    }
    return bytes;
}

// @verified - d
void FileStatAndProcessIfExists(FileState *file) {
    while (static_cast<volatile FileState *>(file)->flags & DiscFileReading) {
        if (IsLoadingDrawActive()) {
            DrawLoadingModes();
            ProcessDiscFileReads();
            dc::sdk::SubmitFrame();
        } else
            ProcessDiscFileReads();
    }
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int IsFileReadIdle(const FileState *file) {
    return file->flags & DiscFileReading ? 0 : 1;
}
#else
bool IsFileReadIdle(const FileState *file) {
    return !(file->flags & DiscFileReading);
}
#endif

std::int32_t SeekFile(FileState *file, std::int32_t offset, std::int32_t origin) {
    std::int32_t sectors;
    WaitForDiscFileReads();
    FileStatAndProcessIfExists(file);
    const std::int32_t position = static_cast<std::int32_t>(GetFilePosition(file));
    switch (static_cast<std::uint32_t>(origin)) {
    case 0:
        break;
    case 1:
        offset = static_cast<std::int32_t>(file->size) - offset;
        break;
    case 2:
        offset += static_cast<std::int32_t>(GetFilePosition(file));
        break;
    }
    if (offset < 0)
        offset = 0;
    else if (offset > static_cast<std::int32_t>(file->size))
        offset = static_cast<std::int32_t>(file->size);
    if (offset == position)
        return offset;
    file->remaining = file->size - static_cast<std::uint32_t>(offset);
    sectors = offset / GDD_FS_SCTSIZE - gdFsTell(static_cast<GDFS>(file->handle));
    if (gdFsSeek(static_cast<GDFS>(file->handle), sectors, GDD_SEEK_CUR)) {
        gdFsGetDrvStat();
    } else if (sectors) {
        static_cast<volatile FileState *>(file)->flags &= ~DiscFileBuffered;
    }
    return offset;
}

std::uint32_t GetFilePosition(const FileState *file) {
    return file->size - file->remaining;
}

std::uint32_t GetFileSize(const FileState *file) {
    return file->size;
}

// @verified - d
std::uint32_t shenOpenFile(const char *directory, const char *filename) {
    char normalized[16];
    std::uint32_t size;
    NormalizeDiscFilename(filename, normalized);
    if (shenmue_dc_loader_sentinel == 1) {
        for (std::int32_t rootIndex = 0; rootIndex < static_cast<std::int32_t>(sizeof(g_simmRoots) / sizeof(g_simmRoots[0])); ++rootIndex) {
            const void *const root = g_simmRoots[rootIndex];
            if (!root)
                continue;
            const std::int32_t kind = SIMM_GetEntrySize(root, directory, normalized, &size);
            if (kind == 2 || kind == 3)
                return size;
        }
    }
    NormalizeDiscFilename(filename, normalized);
    DiscFileLocation *const location = FindDiscFileLocation(directory, normalized);
    if (!location)
        return 0;
    size = GetDiscFileLocationSize(location);
    ReleaseDiscFileLocation(location);
    return size;
}

// @verified - d
void ReadDiscNumber() {
    std::int32_t *const disc = static_cast<std::int32_t *>(shenLoadFile("", "DISCNUM.BIN", DISC_NUMBER_STORAGE));
    g_loadedActNumber = *disc;
    HLib::FreeTaskData(disc);
}

// @verified - d
void LoadAreaTable() {
    typedef std::uint32_t (*TableHeader)();
    char filename[] = "areatbl*.bin";
    filename[7] = static_cast<char>(g_loadedActNumber + '0');
    std::byte *table = static_cast<std::byte *>(shenLoadFile("misc", filename, AREA_TABLE_STORAGE));
    TableHeader tableHeader = reinterpret_cast<TableHeader>(reinterpret_cast<std::uintptr_t>(table));
    InvalidateTableCode();
    g_areaTableEntries = reinterpret_cast<AreaTableEntry *>(tableHeader() + reinterpret_cast<std::uintptr_t>(table));
    for (AreaTableEntry *entry = g_areaTableEntries; entry->area != UINT32_MAX; ++entry)
        entry->transitions = reinterpret_cast<const MapEventTransition *>(table + reinterpret_cast<std::uintptr_t>(entry->transitions));
    table = static_cast<std::byte *>(shenLoadFile("misc", "areatbl.bin", RENDER_PROFILE_STORAGE));
    tableHeader = reinterpret_cast<TableHeader>(reinterpret_cast<std::uintptr_t>(table));
    InvalidateTableCode();
    g_mapRenderProfiles = reinterpret_cast<const MapRenderProfile *>(tableHeader() + reinterpret_cast<std::uintptr_t>(table));
}

// @verified - d
void LoadDoorTable() {
    char tableName[] = "doortbl*.bin";
    char dataName[] = "doordat*.bin";
    tableName[7] = g_loadedActNumber == 2 ? '1' : static_cast<char>(g_loadedActNumber + '0');
    g_doorData = static_cast<std::byte *>(shenLoadFile("misc", tableName, DOOR_TABLE_STORAGE));
    g_doorDataCursor = g_doorData;
    InvalidateTableCode();
    dataName[7] = g_loadedActNumber == 2 ? '1' : static_cast<char>(g_loadedActNumber + '0');
    g_doorTableData = static_cast<std::byte *>(shenLoadFile("misc", dataName, DOOR_TABLE_STORAGE));
    g_doorTableCursor = g_doorTableData;
    InvalidateTableCode();
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
// @verified - d
AreaTableEntry *GetAreaTableEntry(shen_name area) {
    AreaTableEntry *entry = g_areaTableEntries;
    while (entry->area != UINT32_MAX && entry->area != area)
        ++entry;
    return entry;
}
#endif

// @verified - d
std::int32_t shenLoadFile(const char *directory, const char *filename, void *destination, std::uint32_t size) {
    char normalized[16];
    SHENMUE_LOADER_STRCPY(normalized, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(normalized, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    FileState *const file = Archive_GetNextUnusedFileState();
    if (!file || !OpenFile(file, directory, normalized))
        return 0;
    CopyFileToHLibMemory(file, destination, size);
    FileStatAndProcessIfExists(file);
    CloseFile(file);
    return 1;
}

// @verified - d
void *shenLoadFile(const char *directory, const char *filename, shen_name allocationName) {
    char normalized[16];
    void *memory = SHENMUE_LOADER_NULL;
    SHENMUE_LOADER_STRCPY(normalized, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(normalized, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    const std::uint32_t size = shenOpenFile(directory, normalized);
    if (size && (memory = HLib::AllocateTaskData(size, allocationName)) != SHENMUE_LOADER_NULL) {
        FileState *const file = Archive_GetNextUnusedFileState();
        if (!file || !OpenFile(file, directory, normalized)) {
            HLib::FreeTaskData(memory);
            return SHENMUE_LOADER_NULL;
        } else {
            CopyFileToHLibMemory(file, memory, size);
            FileStatAndProcessIfExists(file);
            CloseFile(file);
        }
    }
    return memory;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void *shenLoadCompressedFile(const char *directory, const char *filename, shen_name allocationName) {
    char normalized[16];
    SHENMUE_LOADER_STRCPY(normalized, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(normalized, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    const std::uint32_t size = shenOpenFile(directory, normalized);
    void *const compressed = HLib::AllocateStaticOrDynamicBlock(size, allocationName);
    if (!compressed)
        return SHENMUE_LOADER_NULL;
    void *const scratch = HLib::AllocateStaticOrDynamicBlock(0x4000, COMPRESSED_FILE_WORK);
    if (scratch) {
        if (shenLoadFile(directory, normalized, compressed, size)) {
            const std::uint32_t outputBytes = am2ReadUnalignedLe32(compressed, static_cast<std::int32_t>(size));
            if (outputBytes != 0) {
                void *const output = HLib::AllocateTaskData(outputBytes, allocationName);
                if (output) {
                    DecompressMemoryFile(compressed, output, outputBytes);
                    HLib::FreeTaskData(compressed);
                    HLib::FreeTaskData(scratch);
                    return output;
                }
            }
        }
        HLib::FreeTaskData(scratch);
    }
    HLib::FreeTaskData(compressed);
    return SHENMUE_LOADER_NULL;
}
#else
void *shenLoadCompressedFile(const char *directory, const char *filename, shen_name allocationName) {
    char normalized[20];
    SHENMUE_LOADER_STRCPY(normalized, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(normalized, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    const std::uint32_t size = shenOpenFile(directory, normalized);
    void *const compressed = HLib::AllocateStaticOrDynamicBlock(size, allocationName);
    if (!compressed)
        return SHENMUE_LOADER_NULL;
    void *const scratch = HLib::AllocateStaticOrDynamicBlock(0x4000, COMPRESSED_FILE_WORK);
    if (scratch) {
        if (shenLoadFile(directory, normalized, compressed, size)) {
            std::uint32_t outputBytes;
            SHENMUE_LOADER_MEMCPY(&outputBytes, SHENMUE_LOADER_CONST_BYTES(compressed) + size - 4U, sizeof(outputBytes));
            if (outputBytes != 0) {
                void *const output = HLib::AllocateTaskData(outputBytes, allocationName);
                if (output) {
                    DecompressMemoryFile(compressed, output, outputBytes);
                    HLib::FreeTaskData(compressed);
                    HLib::FreeTaskData(scratch);
                    return output;
                }
            }
        }
        HLib::FreeTaskData(scratch);
    }
    HLib::FreeTaskData(compressed);
    return SHENMUE_LOADER_NULL;
}
#endif

// @verified - d
void *ReadFile(const char *directory, const char *filename, shen_name allocationName) {
    char normalized[16];
    SHENMUE_LOADER_STRCPY(normalized, filename);
    if (char *const extension = SHENMUE_LOADER_STRSTR(normalized, ".MT5"))
        SHENMUE_LOADER_MEMCPY(extension, ".MT7", 5);
    const std::uint32_t size = shenOpenFile(directory, normalized);
    if (!size)
        return SHENMUE_LOADER_NULL;
    void *const memory = HLib::AllocateStaticOrDynamicBlock(size, allocationName);
    if (!memory)
        return SHENMUE_LOADER_NULL;
    if (!shenLoadFile(directory, normalized, memory, size)) {
        HLib::FreeTaskData(memory);
        return SHENMUE_LOADER_NULL;
    }
    return memory;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
int shenFileExists(const char *directory, const char *filename) {
    char name[256], previousDirectory[256];
    GDFS_DIRINFO info;
    char normalized[16];
    std::uint32_t size;
    NormalizeDiscFilename(filename, normalized);
    std::int32_t index = 0;
    while (normalized[index]) {
        name[index] = static_cast<char>(SIMM_UppercaseAscii(normalized[index]));
        ++index;
    }
    name[index] = '\0';
    if (shenmue_dc_loader_sentinel == 1) {
        for (index = 0; index < 2; ++index) {
            if (!g_simmRoots[index])
                continue;
            const std::int32_t kind = SIMM_GetEntrySize(g_simmRoots[index], directory, name, &size);
            if (kind == 2 || kind == 3)
                return 1;
        }
    }
    SHENMUE_LOADER_STRCPY(previousDirectory, discDirectoryPath);
    if (!SelectDiscDirectory(directory))
        return 0;
    index = gdFsGetDirInfo(name, &info);
    gdFsChangeDir(discDirectoryPath);
    if (index == 0)
        return 1;
    return 0;
}
#else
// @verified - d
bool shenFileExists(const char *directory, const char *filename) {
    char normalized[20], name[256];
    NormalizeDiscFilename(filename, normalized);
    std::size_t index = 0;
    while (normalized[index]) {
        name[index] = static_cast<char>(SIMM_UppercaseAscii(normalized[index]));
        ++index;
    }
    name[index] = '\0';
    if (shenmue_dc_loader_sentinel == 1) {
        for (std::size_t root = 0; root < sizeof(g_simmRoots) / sizeof(g_simmRoots[0]); ++root) {
            if (!g_simmRoots[root])
                continue;
            std::uint32_t size;
            const std::int32_t kind = SIMM_GetEntrySize(g_simmRoots[root], directory, name, &size);
            if (kind == 2 || kind == 3)
                return true;
        }
    }
    char previousDirectory[256];
    SHENMUE_LOADER_STRCPY(previousDirectory, discDirectoryPath);
    if (!SelectDiscDirectory(directory))
        return false;
    GDFS_DIRINFO info;
    const std::int32_t result = gdFsGetDirInfo(name, &info);
    SelectDiscDirectory(previousDirectory);
    return result == 0;
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::uint8_t fileLoaderUnusedFlag;

void ClearFileLoaderUnusedFlag() {
    fileLoaderUnusedFlag = 0;
}
#endif

} // namespace s2
} // namespace shenmue

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
extern "C" __attribute__((renesas)) shenmue::s2::AreaTableEntry *shenmue_s2_event_FindScenarioCharacterType(std::uint32_t area) {
    return shenmue::s2::GetAreaTableEntry(shenmue::shen_name(area));
}
#endif
#undef SHENMUE_LOADER_NULL
#undef SHENMUE_LOADER_ALIGN
#undef SHENMUE_LOADER_INIT
#undef SHENMUE_LOADER_MEMCPY
#undef SHENMUE_LOADER_STRCPY
#undef SHENMUE_LOADER_STRCMP
#undef SHENMUE_LOADER_STRSTR
#undef SHENMUE_LOADER_STRLEN
#undef SHENMUE_LOADER_STRNCPY
#undef SHENMUE_LOADER_FILE_COUNT
#undef SHENMUE_LOADER_CONST_BYTES

#endif
