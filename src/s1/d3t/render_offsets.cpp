#include "s1/d3t/render_offsets.hpp"
#include "s1/d3t/namespace.hpp"

#include "s1/archive_data.hpp"
#include "s1/d3t/d3t_asset_remap.hpp"
#include "s1/d3t/d3t_buffer.hpp"
#include "s1/d3t/d3t_content.hpp"
#include "s1/d3t/model.hpp"
#include "s1/d3t/d3t_render_handles.hpp"
#include "s1/d3t/d3t_texture.hpp"
#include "s1/graphics_resources.hpp"
#include "s1/mip_streaming.hpp"
#include "s1/initialization.hpp"
#include "s1/modern_material.hpp"
#include "d3t/shader.hpp"
#include "d3t/shader_load.hpp"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>

namespace shenmue::s1 {

namespace d3t {
std::byte *GetRenderOffsetEntry(std::uint32_t handle) {
    return reinterpret_cast<std::byte *>(d3t::ResolveModel(handle));
}

void *GetPointerFromOffset(std::uint32_t handle) {
    constexpr std::size_t kPayloadOffset = 0xe8;
    std::byte *const entry = d3t::GetRenderOffsetEntry(handle);
    return entry ? entry + kPayloadOffset : nullptr;
}
} // namespace d3t

namespace d3t {



bool CheckForFile(const char *path) {
    char fullHash[272]{};
    GenerateFullHashString(path, fullHash);
    char assetPath[272]{};
    std::snprintf(assetPath, 260, "%s/%s", "./tex/assets/", fullHash);
    char remapped[272]{};
    if (g_d3tAssetRemapping.state.load(std::memory_order_seq_cst) == 10 && genRemapPath(0, assetPath, remapped, 0, true)) {
        if (ArchiveData_GetFileSize(remapped))
            return true;
    } else if (ArchiveData_GetFileSize(assetPath)) {
        return true;
    }
    std::snprintf(assetPath, 260, "./Tex/Assets/%s", path);
    return ArchiveData_GetFileSize(assetPath) != 0;
}
} // namespace d3t

} // namespace shenmue::s1
