#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2
#include "s2/tree.hpp"

#include "s2/prop_dc.hpp"
#include "s2/search_dc.hpp"
#include "s2/weather.hpp"
#include "platform/dc/sdk.hpp"
#include <cmath>
#include <cstring>
#else
#include "s2/tree.hpp"

#include "s2/map_assets.hpp"
#include "s2/motion_work.hpp"
#include "s2/mt7.hpp"
#include "s2/weather.hpp"
#include "common/math.hpp"

#include <cmath>
#endif

namespace shenmue {
namespace s2 {

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

HLib::Task *g_treeTask{};

namespace {

// Native g_PrevWindSpeed: the wind speed the sway speeds were last adjusted for.
float treePreviousWindSpeed;

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)
void tree_Callback_Enqueue(std::uint8_t nextFunctionIndex);
void tree_Callback_Callback(void *parameter);
#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t TreeMarker(const MT7Node *node) {
    return static_cast<std::uint8_t>(IsMDPXorMDP7(node));
}

} // namespace

#endif

#if !(SHENMUE_IS_DC && SHENMUE_IS_S2)

namespace {

std::uint8_t TreeTemplateName(const MT7Node *node) {
    return static_cast<std::uint8_t>(IsMDPXorMDP7(node) >> 8);
}

} // namespace

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

HLib::Task *g_treeTask = 0;

namespace {

float treePreviousWindSpeed;

} // namespace

std::int32_t treeSpeedXBase = TREE_SPEED_X_BASE;
std::int32_t treeSpeedZBase = TREE_SPEED_Z_BASE;
std::int32_t treeAmplitudeXBase = TREE_AMPLITUDE_X_BASE;
std::int32_t treeAmplitudeZBase = TREE_AMPLITUDE_Z_BASE;
std::int32_t treeSwayXPercent = TREE_SWAY_X_PERCENT;

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void tree_Callback_Enqueue(char nextFunctionIndex) {
    MT7Node *root;
    MT7Node *node;
    std::int32_t treeCount;
    std::int32_t templateCount;
    MT7Node *templates[TREE_TEMPLATE_CAPACITY][3];
    char templateNames[TREE_TEMPLATE_CAPACITY];
    TreeSwayRecord *records;
    std::int32_t index;
    std::int32_t match;
    std::uint32_t marker;
    MT7Node *branch;
    MT7Node *twig;

    treePreviousWindSpeed = GetCurrentWindSpeed();
    g_treeTask = 0;
    root = GetPropRoot();
    if (!root || !root->child)
        return;
    node = root->child;
    treeCount = 0;
    templateCount = 0;
    for (; node; node = node->sibling) {
        marker = GetModelMarker(node);
        if ((marker & 0xff) != TREE_MODEL_MARKER)
            continue;
        ++treeCount;
        if (node->mesh && node->child->mesh && node->child->child->mesh && templateCount < TREE_TEMPLATE_CAPACITY) {
            templates[templateCount][0] = node;
            templates[templateCount][1] = node->child;
            templates[templateCount][2] = node->child->child;
            templateNames[templateCount] = static_cast<char>(GetModelMarker(node) >> 8);
            ++templateCount;
        }
    }
    if (!treeCount)
        return;
    g_treeTask = HLib::EnqueueTaskWithParameter(tree_Callback_Callback, nextFunctionIndex, 2, sizeof(TreeSwayRecord) * treeCount, TREE_TASK);
    records = static_cast<TreeSwayRecord *>(HLib::GetTaskParameter(g_treeTask));
    node = root->child;
    index = 0;
    for (; node; node = node->sibling) {
        marker = GetModelMarker(node);
        if ((marker & 0xff) != TREE_MODEL_MARKER)
            continue;
        records[index].tree = node;
        if (!node->mesh) {
            marker = (GetModelMarker(node) & 0xff00) >> 8;
            for (match = 0; match < templateCount; ++match)
                if (static_cast<char>(marker) == templateNames[match])
                    break;
            if (match < templateCount) {
                node->mesh = templates[match][0]->mesh;
                node->child = &records[index].branchCopies[0];
                branch = &records[index].branchCopies[0];
                *branch = *templates[match][1];
                branch->parent = node;
                twig = &records[index].branchCopies[1];
                branch->child = twig;
                branch->sibling = 0;
                *twig = *templates[match][2];
                twig->parent = branch;
                twig->child = 0;
                twig->sibling = 0;
            }
        }
        records[index].phaseX = dc::sdk::RandomInt() << 1;
        records[index].phaseZ = dc::sdk::RandomInt() << 1;
        records[index].speedX = static_cast<std::int32_t>(
            static_cast<float>(treeSpeedXBase + 2 * static_cast<std::int32_t>(dc::sdk::RandomInt() & 0x1f)) - treePreviousWindSpeed * 10240.0f);
        records[index].speedZ = static_cast<std::int32_t>(
            static_cast<float>(treeSpeedZBase + 2 * static_cast<std::int32_t>(dc::sdk::RandomInt() & 0x7f)) - treePreviousWindSpeed * 4096.0f);
        records[index].amplitudeX = treeAmplitudeXBase + 2 * static_cast<std::int32_t>(dc::sdk::RandomInt() & 0x3f);
        records[index].amplitudeZ = treeAmplitudeZBase + 2 * static_cast<std::int32_t>(dc::sdk::RandomInt() & 0x3f);
        ++index;
    }
}

#else

// Native tree_Callback_Enqueue: trees with a two-level mesh chain become templates; every meshless tree adopts its
// template's mesh and gets copies of the template's two branch nodes spliced beneath it. Each tree draws random sway
// phases, speeds (slowed by the current wind) and amplitudes.
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
    TreeSwayRecord *const records = static_cast<TreeSwayRecord *>(HLib::GetTaskParameter(g_treeTask));
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

#endif

#if SHENMUE_IS_DC && SHENMUE_IS_S2

void tree_Callback_Callback(void *parameter) {
    TreeSwayRecord *record = static_cast<TreeSwayRecord *>(parameter);
    std::int32_t windChanged = 0;
    MT7Node *node = GetPropRoot();
    const float wind = GetCurrentWindSpeed();
    node = node->child;
    if (std::fabs(treePreviousWindSpeed - wind) > 1.1920928955078125e-07f)
        windChanged = 1;
    for (; node; node = node->sibling) {
        if (static_cast<std::uint8_t>(GetModelMarker(node)) != TREE_MODEL_MARKER)
            continue;
        if (windChanged) {
            const float speedX = static_cast<float>(record->speedX);
            const float change = wind - treePreviousWindSpeed;
            record->speedX = static_cast<std::int32_t>(speedX - 10240.0f * change);
            record->speedZ = static_cast<std::int32_t>(static_cast<float>(record->speedZ) - change * 4096.0f);
        }
        if (static_cast<std::uint32_t>(record->phaseX) & 0x8000U)
            record->phaseX = static_cast<std::int32_t>(static_cast<float>(record->phaseX) + static_cast<float>(record->speedX) * 1.3f);
        else
            record->phaseX = static_cast<std::int32_t>(static_cast<std::uint32_t>(record->phaseX) + static_cast<std::uint32_t>(record->speedX));
        if (static_cast<std::uint32_t>(record->phaseZ) & 0x8000U)
            record->phaseZ = static_cast<std::int32_t>(static_cast<float>(record->phaseZ) + static_cast<float>(record->speedZ) * 1.3f);
        else
            record->phaseZ = static_cast<std::int32_t>(static_cast<std::uint32_t>(record->phaseZ) + static_cast<std::uint32_t>(record->speedZ));
        const float swayX = ((1.0f + dc::sdk::SinFixed(record->phaseX)) * 0.5f) * static_cast<float>(record->amplitudeX);
        const float phaseZ = (1.0f + dc::sdk::SinFixed(record->phaseZ)) * 0.5f;
        const float amplitudeZ = static_cast<float>(record->amplitudeZ);
        MT7Node *const branch = node->child;
        const float sum = swayX + phaseZ * amplitudeZ;
        const float sway = static_cast<float>(static_cast<std::int32_t>(0U - static_cast<std::uint32_t>(static_cast<std::int32_t>(sum))));
        const std::int32_t swayXPercent = treeSwayXPercent;
        float branchSway = sway;
        branchSway *= static_cast<float>(swayXPercent) / 100.0f;
        const float lean = wind * 768.0f;
        branch->rotation.x = static_cast<std::int32_t>(branchSway - lean);
        node->child->child->rotation.y = static_cast<std::int32_t>(sway * (static_cast<float>(100 - swayXPercent) / 100.0f) - lean);
        ++record;
    }
    treePreviousWindSpeed = wind;
}

#else

// Native tree_Callback_Callback: a wind change retunes every sway speed; each phase advances (faster through its
// second half-turn), and the summed sway bends the first branch node about X and the second about Y, both leaning
// with the wind.
void tree_Callback_Callback(void *parameter) {
    TreeSwayRecord *record = static_cast<TreeSwayRecord *>(parameter);
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
        const std::uint32_t phaseX = static_cast<std::uint32_t>(record->phaseX);
        record->phaseX = static_cast<std::int32_t>((phaseX & 0x8000U) != 0 ?
                phaseX - static_cast<std::uint32_t>(static_cast<std::int32_t>(static_cast<float>(record->speedX) * -1.3f)) :
                phaseX + static_cast<std::uint32_t>(record->speedX));
        const std::uint32_t phaseZ = static_cast<std::uint32_t>(record->phaseZ);
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

#endif

} // namespace s2
} // namespace shenmue
