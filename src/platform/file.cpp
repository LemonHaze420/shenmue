#include "d3t/archive_handles.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace shenmue::d3t {

bool Platform_CloseFileHandle(PlatformFileHandle handle) {
#ifdef _WIN32
    return CloseHandle(reinterpret_cast<HANDLE>(handle)) != 0;
#else
    return handle != 0 && ::close(static_cast<int>(handle)) == 0;
#endif
}

} // namespace shenmue::d3t
