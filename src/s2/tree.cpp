#include "s2/tree.hpp"

#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/weather.hpp"
#include "common/math.hpp"

#include <cmath>

namespace shenmue::s2 {

HLib::Task *g_treeTask{};

namespace {


float treePreviousWindSpeed{};

std::uint8_t TreeMarker(const MT7Node *node) {
    return static_cast<std::uint8_t>(IsMDPXorMDP7(node));
}

std::uint8_t TreeTemplateName(const MT7Node *node) {
    return static_cast<std::uint8_t>(IsMDPXorMDP7(node) >> 8);
}

} // namespace




void tree_Callback_Enqueue(std::uint8_t nextFunctionIndex) {
    treePreviousWindSpeed = GetCurrentWindSpeed();
    g_treeTask = nullptr;
    MT7Node *const root = GetPropRoot();
    if (!root || !mtGetTransform(root))
        return;
    std::int32_t treeCount = 0;
    std::int32_t templateCount = 0;
    std::array<std::array<MT7Node *, 3>, TREE_TEMPLATE_CAPACITY> templates{};
    std::array<std::uint8_t, TREE_TEMPLATE_CAPACITY> templateNames{};
    for (MT7Node *node = mtGetTransform(root); node; node = mtGetSibling(node)) {
        if (TreeMarker(node) != TREE_MODEL_MARKER)
            continue;
        ++treeCount;
        MT7Node *const branch = mt7GetXb01Offset(node) ? mtGetTransform(node) : nullptr;
        MT7Node *const twig = branch && mt7GetXb01Offset(branch) ? mtGetTransform(branch) : nullptr;
        if (!twig || !mt7GetXb01Offset(twig) || templateCount >= TREE_TEMPLATE_CAPACITY)
            continue;
        templates[static_cast<std::size_t>(templateCount)] = {node, branch, twig};
        templateNames[static_cast<std::size_t>(templateCount++)] = TreeTemplateName(node);
    }
    if (treeCount == 0)
        return;
    g_treeTask =
        HLib::EnqueueTaskWithParameter(tree_Callback_Callback, nextFunctionIndex, 2, sizeof(TreeSwayRecord) * static_cast<std::size_t>(treeCount), TREE_TASK);
    auto *const records = static_cast<TreeSwayRecord *>(HLib::GetTaskParameter(g_treeTask));
    MT7Node *const branchNodes = AllocateMT7TaskNodeCopies(records, 2 * static_cast<std::size_t>(treeCount));
    std::size_t index = 0;
    for (MT7Node *node = mtGetTransform(root); node; node = mtGetSibling(node)) {
        if (TreeMarker(node) != TREE_MODEL_MARKER)
            continue;
        TreeSwayRecord &record = records[index];
        record.tree = node;
        if (!mt7GetXb01Offset(node)) {
            const std::uint8_t name = TreeTemplateName(node);
            std::int32_t match = 0;
            while (match < templateCount && templateNames[static_cast<std::size_t>(match)] != name)
                ++match;
            if (match < templateCount) {
                const std::array<MT7Node *, 3> &source = templates[static_cast<std::size_t>(match)];
                MT7Node &branch = branchNodes[2 * index];
                mt7_CopyXB01Offset(node, source[0]);
                mt7_UpdateChildOffset(node, &branch);
                branch = *source[1];
                mt7_UpdateParentOffset(&branch, node);
                mt7_UpdateChildOffset(&branch, &branchNodes[2 * index + 1]);
                mt7_UpdateSiblingOffset(&branch, nullptr);
                MT7Node *const twig = mtGetTransform(&branch);
                *twig = *source[2];
                mt7_UpdateParentOffset(twig, &branch);
                mt7_UpdateChildOffset(twig, nullptr);
                mt7_UpdateSiblingOffset(twig, nullptr);
            }
        }
        record.phaseX = 2 * IncrementSharedPrngUInt16();
        record.phaseZ = 2 * IncrementSharedPrngUInt16();
        const std::int32_t speedX = TREE_SPEED_X_BASE + 2 * (IncrementSharedPrngUInt16() & 0x1f);
        record.speedX = static_cast<std::int32_t>(static_cast<float>(speedX) - treePreviousWindSpeed * 10240.0f);
        const std::int32_t speedZ = TREE_SPEED_Z_BASE + 2 * (IncrementSharedPrngUInt16() & 0x7f);
        record.speedZ = static_cast<std::int32_t>(static_cast<float>(speedZ) - treePreviousWindSpeed * 4096.0f);
        record.amplitudeX = TREE_AMPLITUDE_X_BASE + 2 * (IncrementSharedPrngUInt16() & 0x3f);
        record.amplitudeZ = TREE_AMPLITUDE_Z_BASE + 2 * (IncrementSharedPrngUInt16() & 0x3f);
        ++index;
    }
}




void tree_Callback_Callback(void *parameter) {
    auto *record = static_cast<TreeSwayRecord *>(parameter);
    MT7Node *const root = GetPropRoot();
    const float wind = GetCurrentWindSpeed();
    const bool windChanged = std::fabs(treePreviousWindSpeed - wind) > 0.00000011920929f;
    for (MT7Node *node = mtGetTransform(root); node; node = mtGetSibling(node)) {
        if (TreeMarker(node) != TREE_MODEL_MARKER)
            continue;
        if (windChanged) {
            const float change = wind - treePreviousWindSpeed;
            record->speedX += static_cast<std::int32_t>(change * -10240.0f);
            record->speedZ += static_cast<std::int32_t>(change * -4096.0f);
        }
        const auto phaseX = static_cast<std::uint32_t>(record->phaseX);
        record->phaseX = static_cast<std::int32_t>((phaseX & 0x8000U) != 0 ?
                phaseX - static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(record->speedX) * -1.3f)) :
                phaseX + static_cast<std::uint32_t>(record->speedX));
        const auto phaseZ = static_cast<std::uint32_t>(record->phaseZ);
        record->phaseZ = static_cast<std::int32_t>((phaseZ & 0x8000U) != 0 ?
                phaseZ - static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(record->speedZ) * -1.3f)) :
                phaseZ + static_cast<std::uint32_t>(record->speedZ));
        const float swayX = static_cast<float>(record->amplitudeX) * ((sin_fixed16(static_cast<std::int16_t>(record->phaseX)) + 1.0f) * 0.5f);
        const float swayZ = static_cast<float>(record->amplitudeZ) * ((sin_fixed16(static_cast<std::int16_t>(record->phaseZ)) + 1.0f) * 0.5f);
        const float sway = static_cast<float>(-static_cast<std::int32_t>(swayZ + swayX));
        const float lean = wind * 768.0f;
        MT7Node *const branch = mtGetTransform(node);
        if (branch) {
            branch->rotation.x = static_cast<std::int32_t>(static_cast<float>(TREE_SWAY_X_PERCENT) / 100.0f * sway - lean);
            if (MT7Node *const twig = mtGetTransform(branch))
                twig->rotation.y = static_cast<std::int32_t>(static_cast<float>(100 - TREE_SWAY_X_PERCENT) / 100.0f * sway - lean);
        }
        ++record;
    }
    treePreviousWindSpeed = wind;
}

} // namespace shenmue::s2
