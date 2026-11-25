#include "s2/scene_data.hpp"
#include "s2/hlib.hpp"

#if SHENMUE_S2_GCC295
#include <string.h>
#define SHENMUE_SCENE_MEMCPY ::memcpy
#define SHENMUE_SCENE_NULL 0
#else
#include <cstring>
#define SHENMUE_SCENE_MEMCPY std::memcpy
#define SHENMUE_SCENE_NULL nullptr
#endif

namespace shenmue {
namespace s2 {

SceneTextHeap *g_sceneTextHeap = SHENMUE_SCENE_NULL;
std::size_t g_sceneTextHeapCursor;

// @verified - d
SceneTextHeap *AllocateSTCH() {
#if SHENMUE_S2_GCC295
    SceneTextHeap *const heap = static_cast<SceneTextHeap *>(HLib::AllocateTaskData(sizeof(SceneTextHeap), SCENE_TEXT_HEAP));
    g_sceneTextHeap = heap;
    ClearSTCH();
#else
    g_sceneTextHeap = static_cast<SceneTextHeap *>(HLib::AllocateTaskData(sizeof(SceneTextHeap), SCENE_TEXT_HEAP));
    g_sceneTextHeapCursor = 0;
    return g_sceneTextHeap;
#endif
}

// @verified - d
void ClearSTCH() {
    g_sceneTextHeapCursor = 0;
}

// @verified - d
std::byte *allocateInGlobalHeap(std::intptr_t size) {
    if (!size)
        return SHENMUE_SCENE_NULL;
#if SHENMUE_S2_GCC295
    size = (size + 7) & ~3;
    std::byte *const allocation = g_sceneTextHeap->buffer + g_sceneTextHeapCursor;
    g_sceneTextHeapCursor += size;
    *reinterpret_cast<std::uint32_t *>(allocation) = SHEN_NAME(USSH);
#else
    std::byte *const allocation = g_sceneTextHeap->buffer + g_sceneTextHeapCursor;
    g_sceneTextHeapCursor += (static_cast<std::size_t>(size) + 7) & ~std::size_t{3};
    const std::uint32_t tag = SHEN_NAME(USSH);
    SHENMUE_SCENE_MEMCPY(allocation, &tag, sizeof(tag));
#endif
    return allocation + 4;
}

// @verified - d
void SetTaskHeapData(void *allocation) {
#if SHENMUE_IS_DC && SHENMUE_IS_S2
    if (allocation) {
        allocation = static_cast<std::byte *>(allocation) - 4;
        g_sceneTextHeapCursor = static_cast<std::byte *>(allocation) - g_sceneTextHeap->buffer;
#else
    if (allocation)
        g_sceneTextHeapCursor = static_cast<std::byte *>(allocation) - g_sceneTextHeap->buffer - 4;
#endif
    }
#if SHENMUE_IS_DC && SHENMUE_IS_S2
}
#endif

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
void SceneTextHeapUnusedNullsub() {
}

void SceneTextHeapUnusedNullsub2() {
}
#endif

} // namespace s2
} // namespace shenmue

#undef SHENMUE_SCENE_MEMCPY
#undef SHENMUE_SCENE_NULL
