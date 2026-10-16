#include "d3t/file_state.hpp"

#include <cstddef>
#include <mutex>
#include <system_error>

namespace shenmue::d3t {

std::array<FileState, 2048> g_files{};

namespace {

struct FileLockEntry {
    union {
        std::mutex mutex;
        std::byte state[0x50];
    };
    std::uint8_t active;
    std::uint8_t reserved_51;
    std::uint16_t sequence;
    std::byte reserved_54[4];
    FileLockEntry() : mutex{} {
    }
    ~FileLockEntry() {
        mutex.~mutex();
    }
};

static_assert(sizeof(FileLockEntry) == 0x58);
static_assert(offsetof(FileLockEntry, active) == 0x50);
static_assert(offsetof(FileLockEntry, sequence) == 0x52);

std::array<FileLockEntry, 64> g_fileLockTable{};
std::mutex g_fileLockTableMutex;
std::uint16_t g_fileLockSequenceCounter = 2;
constexpr std::uint32_t kInvalidFileLockHandle = 0xffffU;

FileLockEntry &ResolveFileLock(std::uint32_t handle) {
    const std::uint32_t index = handle & 0xffffU;
    if (handle == kInvalidFileLockHandle || index >= g_fileLockTable.size() || g_fileLockTable[index].sequence != (handle >> 16U))
        throw std::system_error(std::make_error_code(std::errc::invalid_argument));
    return g_fileLockTable[index];
}

} // namespace

std::uint32_t AllocateFileLockHandle() {
    std::uint32_t index = 0;
    {
        const std::lock_guard lock(g_fileLockTableMutex);
        while (index < g_fileLockTable.size() && g_fileLockTable[index].active) {
            ++index;
        }
        if (index == g_fileLockTable.size()) {
            return kInvalidFileLockHandle;
        }
        g_fileLockTable[index].active = 1;
    }

    const std::uint16_t sequence = ++g_fileLockSequenceCounter;
    g_fileLockTable[index].sequence = sequence;
    return index | (static_cast<std::uint32_t>(sequence) << 16);
}

void LockFileHandle(std::uint32_t handle) {
    ResolveFileLock(handle).mutex.lock();
}

void UnlockFileHandle(std::uint32_t handle) {
    ResolveFileLock(handle).mutex.unlock();
}

} // namespace shenmue::d3t
