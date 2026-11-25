#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"

namespace shenmue {
namespace s2 {

struct DebugTreeNode {
    DebugTreeNode *sibling;
    DebugTreeNode *child;
};

void DebugWalkTree(DebugTreeNode *node, std::int32_t depth) {
    if (node->child)
        DebugWalkTree(node->child, depth + 1);
    for (std::int32_t indent = 0; indent < depth; ++indent) {
    }
    if (node->sibling)
        DebugWalkTree(node->sibling, depth + 1);
}

} // namespace s2
} // namespace shenmue

#endif
