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
    std::uint32_t selected = flags & 0xffU;
    MT7Node *group = root->child;
    for (std::uint32_t index = (flags >> 8U) & 15U; index; --index)
        group = group->sibling;
    const std::uint32_t last = CountImageModelFrames(group->child);
    const std::int32_t speed = flags & 0xf0000U;
    std::uint32_t step;
    switch (speed) {
    case 0x20000:
        step = (g_frameCounter & 3U) == 0;
        break;
    case 0x40000:
        step = (g_frameCounter & 7U) == 0;
        break;
    case 0x80000:
        step = 0;
        break;
    case 0:
    default:
        step = 1;
        break;
    }
    std::uint32_t reverse = flags & 0x100000U;
    if (flags & 0x2000U) {
        if (reverse)
            selected -= step;
        else
            selected += step;
        if (static_cast<std::int32_t>(selected) < 0) {
            selected = 0;
            reverse = 0;
        }
        if (static_cast<std::int32_t>(selected) > static_cast<std::int32_t>(last)) {
            selected = last;
            reverse = 0x100000U;
        }
    } else {
        selected += step;
        reverse = 0;
        if (static_cast<std::int32_t>(selected) > static_cast<std::int32_t>(last))
            selected = 0;
    }
    MT7Node *node = group->child;
    for (std::int32_t index = 0; index < static_cast<std::int32_t>(selected); ++index)
        node = node->sibling;
    flags &= 0xfff00U;
    flags |= reverse;
    flags |= selected;
    return flags;
}

} // namespace s2
} // namespace shenmue

#endif
