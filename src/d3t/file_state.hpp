#pragma once

#include <array>
#include <cstdint>

#include "d3t/archive_handles.hpp"

namespace shenmue::d3t {

struct FileState {
    char path[0x104];
    std::uint32_t handle;
    std::uint64_t offset;
};

static_assert(sizeof(FileState) == 0x110);
static_assert(offsetof(FileState, handle) == 0x104);
static_assert(offsetof(FileState, offset) == 0x108);

extern std::array<FileState, 2048> g_files;

FileState *Archive_GetNextUnusedFileState();
std::uint32_t AllocateFileLockHandle();
void LockFileHandle(std::uint32_t handle);
void UnlockFileHandle(std::uint32_t handle);

} // namespace shenmue::d3t
