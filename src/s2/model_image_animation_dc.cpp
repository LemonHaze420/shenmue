#include "s2/model_image_component_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/frame_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
std::uint32_t CountImageModelFrames(const MT7Node *node) {
    std::uint32_t count = 0;
    if (!node)
        return count;
    while (node->sibling) {
        node = node->sibling;
        ++count;
    }
    return count;
}

// @verified - d
std::uint32_t AdvanceImageModelFrame(MT7Node *root, std::uint32_t flags) {
    MT7Node *group = root->child;
    for (std::uint32_t index = (flags >> 8U) & 15U; index; --index)
        group = group->sibling;
    const std::uint32_t last = CountImageModelFrames(group->child);
    const std::uint32_t speed = flags & 0xf0000U;
    std::uint32_t step = 1;
    if (speed == 0x20000U)
        step = (g_frameCounter & 3U) == 0;
    else if (speed == 0x40000U)
        step = (g_frameCounter & 7U) == 0;
    else if (speed == 0x80000U)
        step = 0;
    std::uint32_t reverse = flags & 0x100000U;
    std::uint32_t selected;
    if (flags & 0x2000U) {
        if (reverse)
            step = 0U - step;
        selected = (flags & 0xffU) + step;
        if (static_cast<std::int32_t>(selected) < 0) {
            selected = 0;
            reverse = 0;
        }
        if (static_cast<std::int32_t>(last) < static_cast<std::int32_t>(selected)) {
            selected = last;
            reverse = 0x100000U;
        }
    } else {
        selected = (flags & 0xffU) + step;
        reverse = 0;
        if (static_cast<std::int32_t>(last) < static_cast<std::int32_t>(selected))
            selected = 0;
    }
    return (flags & 0xfff00U) | reverse | selected;
}

} // namespace s2
} // namespace shenmue

#endif
