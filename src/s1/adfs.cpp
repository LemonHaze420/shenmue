#include "s1/archive_handles.hpp"
#include "s1/hlib.hpp"
#include "s1/task/pack.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>

namespace shenmue::s1 {

std::array<PackAdfsWrapper, 12> g_adfsWrappers = [] {
    std::array<PackAdfsWrapper, 12> wrappers{};
    for (PackAdfsWrapper &wrapper : wrappers) {
        wrapper.status = -1;
    }
    return wrappers;
}();

std::int64_t ReadADFS(char *directory, const char *filename, std::int64_t, std::int32_t fileId) {
    char normalizedDirectory[512];
    std::strcpy(normalizedDirectory, directory[0] == '/' ? directory + 1 : directory);
    for (char *cursor = normalizedDirectory; *cursor; ++cursor) {
        if (*cursor == '/') {
            *cursor = '\\';
        }
    }

    char path[512];
    char *output = path;
    for (const char *input = normalizedDirectory; (*output++ = *input++) != '\0';) {
    }
    --output;
    *output++ = '\\';
    while ((*output++ = *filename++) != '\0') {
    }
    return LargeFile_LoadADFSArchiveToc(fileId, path);
}

PackAdfsWrapper *ReadADFS_Wrapper(char *directory, const char *filename, std::int32_t timeout) {
    PackAdfsWrapper *wrapper = nullptr;
    std::int32_t fileId = 0;
    for (PackAdfsWrapper &candidate : g_adfsWrappers) {
        if (candidate.status == -1) {
            wrapper = &candidate;
            break;
        }
        ++fileId;
    }
    if (!wrapper) {
        return nullptr;
    }

    wrapper->status = fileId;
    const std::int64_t allocationSize = (2LL * (static_cast<std::int64_t>(timeout) + 1) + 278) & ~3LL;
    wrapper->taskData = static_cast<HLib::TaskData *>(HLib_AllocHeapMemBlock(allocationSize, SHEN_NAME(ADFS)));
    std::memset(wrapper->taskData, 0, static_cast<std::size_t>(allocationSize));
    if (ReadADFS(directory, filename, 0, fileId) != 0) {
        return nullptr;
    }
    return wrapper;
}

} // namespace shenmue::s1
