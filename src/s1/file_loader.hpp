#pragma once

#include "s1/file_state.hpp"

#include <cstdint>

namespace shenmue::s1 {
extern bool g_skipArchiveReadWait;

std::uint64_t OpenFileInternal(const char *path);
void OpenFile(FileState *file, void *destination, std::int32_t size);
void FileStatAndProcessIfExists(FileState *file);
std::uint64_t shenOpenFile(const char *directory, const char *filename);
std::int32_t PrepareDirectoryPathForFile(FileState *file, const char *directory, const char *filename);
std::int64_t shenLoadFile(const char *directory, const char *filename, void *destination, std::int32_t size);
void *ReadFile(const char *directory, const char *filename, std::int32_t token);

} // namespace shenmue::s1
