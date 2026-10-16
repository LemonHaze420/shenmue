#pragma once

#include <cstdint>

namespace shenmue::d3t {
struct UIViewImage;
}
namespace shenmue::s2 {

struct UISceneTarget {
    std::uint32_t colour;
    std::uint32_t depth;
    std::int32_t width;
    std::int32_t height;
};
static_assert(sizeof(UISceneTarget) == 0x10);
extern UISceneTarget g_uiSceneTarget;
extern UISceneTarget g_uiSceneAuxiliaryTarget;

void CreateTexture2DForNuklear(std::int32_t width, std::int32_t height);
void BindUISceneImage(std::int32_t index, d3t::UIViewImage *image);
void SetUISceneCallback(void (*callback)());
bool BeginUISceneRenderQueue();
void RenderUIScene();

} // namespace shenmue::s2
