#include "s2/scene_data.hpp"
#include "s2/hlib.hpp"

#include <cstring>
#define SHENMUE_SCENE_MEMCPY std::memcpy
#define SHENMUE_SCENE_NULL nullptr

namespace shenmue { namespace s2 {

SceneTextHeap *g_sceneTextHeap = SHENMUE_SCENE_NULL;
std::size_t g_sceneTextHeapCursor = 0;

// @verified - d
SceneTextHeap *AllocateSTCH() {
    g_sceneTextHeap = static_cast<SceneTextHeap *>(HLib::AllocateTaskData(sizeof(SceneTextHeap), SCENE_TEXT_HEAP));
    g_sceneTextHeapCursor = 0;
    return g_sceneTextHeap;
}

// @verified - d
void ClearSTCH() {
    g_sceneTextHeapCursor = 0;
}

// @verified - d
std::byte *allocateInGlobalHeap(std::intptr_t size) {
    if (!size)
        return SHENMUE_SCENE_NULL;
    std::byte *const allocation = g_sceneTextHeap->buffer + g_sceneTextHeapCursor;
    g_sceneTextHeapCursor += (static_cast<std::size_t>(size) + 7) & ~std::size_t{3};
    const std::uint32_t tag = SHEN_NAME(USSH);
    SHENMUE_SCENE_MEMCPY(allocation, &tag, sizeof(tag));
    return allocation + 4;
}

// @verified - d
void SetTaskHeapData(void *allocation) {
    if (allocation)
        g_sceneTextHeapCursor = static_cast<std::byte *>(allocation) - g_sceneTextHeap->buffer - 4;
}

} } // namespace shenmue::s2

#undef SHENMUE_SCENE_MEMCPY
#undef SHENMUE_SCENE_NULL
