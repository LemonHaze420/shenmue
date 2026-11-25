#include "s2/weather_particles_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/door_control_dc.hpp"
#include "s2/hlib.hpp"
#include "s2/scene_state_dc.hpp"

namespace shenmue {
namespace s2 {

// @verified - d
WeatherFixcNode *AllocateWeatherFixcNode(std::int32_t index, void *payload) {
    WeatherFixcNode *const node = static_cast<WeatherFixcNode *>(HLib::AllocateTaskData(sizeof(WeatherFixcNode), WEATHER_FIXC_NODE_STORAGE));
    node->previous = 0;
    node->next = 0;
    node->index = index;
    node->payload.pointer = payload;
    return node;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
std::int32_t GetWeatherFixcNodeIndex(const WeatherFixcNode *node) {
    return node->index;
}
#endif

// @verified - d
void *GetWeatherFixcNodeData(const WeatherFixcNode *node) {
    return node->payload.pointer;
}

// @verified - d
WeatherFixcNode *FindWeatherFixcNode(WeatherFixcNode *node, const void *payload) {
    while (node && node->payload.pointer != payload)
        node = node->next;
    return node;
}

#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
WeatherFixcNode *FindPreviousWeatherFixcNode(WeatherFixcNode *node, const void *payload) {
    while (node && node->payload.pointer != payload)
        node = node->previous;
    return node;
}

WeatherFixcNode *FindWeatherFixcNodeByIndex(WeatherFixcNode *node, std::int32_t index) {
    while (node && node->index != index)
        node = node->next;
    return node;
}

WeatherFixcNode *FindPreviousWeatherFixcNodeByIndex(WeatherFixcNode *node, std::int32_t index) {
    while (node && node->index != index)
        node = node->previous;
    return node;
}

void InsertWeatherFixcNode(WeatherFixcNode *previous, WeatherFixcNode *node, WeatherFixcNode *next) {
    LinkWeatherFixcNodes(previous, node);
    LinkWeatherFixcNodes(node, next);
}

WeatherFixcNode *GetFirstWeatherFixcNode(WeatherFixcNode *node) {
    if (!node)
        return 0;
    for (;;) {
        WeatherFixcNode *const previous = node->previous;
        if (!previous)
            break;
        node = previous;
    }
    return node;
}
#endif

// @verified - d
WeatherFixcNode *GetLastWeatherFixcNode(WeatherFixcNode *node) {
    if (!node)
        return 0;
    for (;;) {
        WeatherFixcNode *const next = node->next;
        if (!next)
            break;
        node = next;
    }
    return node;
}

// @verified - d
WeatherFixcNode *GetPreviousWeatherFixcNode(const WeatherFixcNode *node) {
    return node->previous;
}

// @verified - d
WeatherFixcNode *GetNextWeatherFixcNode(const WeatherFixcNode *node) {
    return node->next;
}

// @verified - d
void LinkWeatherFixcNodes(WeatherFixcNode *first, WeatherFixcNode *second) {
    if (first)
        first->next = second;
    if (second)
        second->previous = first;
}

// @verified - d
void SplitWeatherFixcNodes(WeatherFixcNode *first, WeatherFixcNode *second) {
    if (first)
        first->next = 0;
    if (second)
        second->previous = 0;
}

// @verified - d
void DetachWeatherFixcNode(WeatherFixcNode *node) {
    WeatherFixcNode *const previous = GetPreviousWeatherFixcNode(node);
    WeatherFixcNode *const next = GetNextWeatherFixcNode(node);
    SplitWeatherFixcNodes(previous, node);
    SplitWeatherFixcNodes(node, next);
    LinkWeatherFixcNodes(previous, next);
}

// @verified - d
void FreeWeatherFixcNode(WeatherFixcNode *node) {
    DetachWeatherFixcNode(node);
    HLib::FreeTaskData(node);
}

static std::int32_t doorFlagRulesAR03_06[] = {144, 258, 0, 0};
static DoorAreaMetadata doorMetadataOR00[] = {{2, 0, 0, 2400, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQ100[] = {
    {5, 1, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {11, 1, 0, 2400, 0, 2, 0}, {12, 1, 0, 2400, 0, 2, 0}, {13, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQ300[] = {
    {7, 1, 0, 2400, 0, 3, 0}, {8, 0, 0, 2400, 0, 3, 0}, {9, 1, 0, 2400, 0, 3, 0}, {10, 0, 0, 2400, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQAXX[] = {{1, 1, 0, 2400, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQA00[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {31, 0, 0, 2400, 1, 1, 0},
    {32, 0, 0, 2400, 1, 1, 0}, {31, 0, 0, 2400, 1, 1, 0}, {32, 0, 0, 2400, 1, 1, 0}, {40, 0, 0, 2400, 0, 2, 0}, {42, 0, 0, 2400, 0, 3, 0},
    {43, 0, 0, 2400, 0, 3, 0}, {10, 0, 0, 2400, 1, 1, 0}, {11, 0, 0, 2400, 1, 1, 0}, {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0},
    {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0}, {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0},
    {41, 0, 0, 0, 0, 2, 0}, {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0}, {22, 0, 0, 2400, 1, 1, 0},
    {23, 0, 0, 2400, 1, 1, 0}, {24, 0, 0, 2400, 1, 1, 0}, {25, 0, 0, 2400, 1, 1, 0}, {26, 0, 0, 2400, 1, 1, 0}, {27, 0, 0, 2400, 1, 1, 0},
    {27, 0, 0, 2400, 1, 1, 0}, {28, 0, 0, 2400, 1, 1, 0}, {29, 0, 0, 2400, 1, 1, 0}, {30, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQAW1[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {31, 0, 0, 2400, 1, 1, 0},
    {32, 0, 0, 2400, 1, 1, 0}, {48, 0, 0, 2400, 0, 3, 0}, {49, 0, 0, 2400, 0, 3, 0}, {50, 0, 0, 2400, 0, 3, 0}, {45, 0, 0, 2400, 0, 3, 0},
    {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0}, {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0},
    {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0}, {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0},
    {22, 0, 0, 2400, 1, 1, 0}, {23, 0, 0, 2400, 1, 1, 0}, {24, 0, 0, 2400, 1, 1, 0}, {25, 0, 0, 2400, 1, 1, 0}, {26, 0, 0, 2400, 1, 1, 0},
    {27, 0, 0, 2400, 1, 1, 0}, {27, 0, 0, 2400, 1, 1, 0}, {28, 0, 0, 2400, 1, 1, 0}, {29, 0, 0, 2400, 1, 1, 0}, {30, 0, 0, 2400, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQAW6[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {31, 0, 0, 2400, 1, 1, 0},
    {32, 0, 0, 2400, 1, 1, 0}, {46, 0, 0, 2400, 0, 3, 0}, {47, 0, 0, 2400, 0, 3, 0}, {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0},
    {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0}, {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0},
    {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0}, {22, 0, 0, 2400, 1, 1, 0}, {23, 0, 0, 2400, 1, 1, 0},
    {24, 0, 0, 2400, 1, 1, 0}, {25, 0, 0, 2400, 1, 1, 0}, {26, 0, 0, 2400, 1, 1, 0}, {27, 0, 0, 2400, 1, 1, 0}, {28, 0, 0, 2400, 1, 1, 0},
    {29, 0, 0, 2400, 1, 1, 0}, {30, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQAE1[] = {
    {35, 0, 0, 2400, 0, 3, 0}, {36, 0, 0, 2400, 0, 3, 0}, {37, 0, 0, 2400, 0, 3, 0}, {38, 0, 0, 2400, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQB00[] = {{6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {1, 0, 0, 2400, 0, 3, 0}, {2, 0, 0, 2400, 0, 3, 0},
    {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 0, 3, 0}, {5, 0, 0, 0, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQC00[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {5, 0, 0, 2400, 1, 1, 0}, {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 1, 0, 2400, 0, 1, 0},
    {9, 0, 0, 2400, 0, 1, 0}, {43, 0, 0, 2400, 0, 2, 0}, {44, 0, 0, 2400, 0, 2, 0}, {45, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQC01[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQC06[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {40, 0, 0, 2400, 0, 1, 0}, {41, 0, 0, 2400, 0, 1, 0}, {42, 0, 0, 2400, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQCAE[] = {{9, 0, 0, 2400, 0, 2, 0}, {5, 0, 0, 2400, 0, 2, 0}, {10, 0, 0, 2400, 0, 2, 0}, {6, 0, 0, 2400, 0, 2, 0},
    {11, 0, 0, 2400, 0, 2, 0}, {7, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQD00[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {40, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQD01[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQD06[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQE00[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {10, 0, 0, 2400, 1, 1, 0},
    {11, 0, 0, 2400, 1, 1, 0}, {11, 0, 0, 2400, 1, 1, 0}, {41, 0, 0, 2400, 0, 2, 0}, {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0},
    {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0}, {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0},
    {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0}, {43, 0, 0, 2400, 1, 1, 0}, {44, 0, 0, 2400, 1, 1, 0},
    {45, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQE01[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {10, 0, 0, 2400, 1, 1, 0},
    {11, 0, 0, 2400, 1, 1, 0}, {11, 0, 0, 2400, 1, 1, 0}, {40, 0, 0, 2400, 0, 3, 0}, {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0},
    {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0}, {16, 0, 0, 2400, 1, 1, 0}, {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0},
    {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQE06[] = {{1, 0, 0, 2400, 1, 1, 0}, {2, 0, 0, 2400, 1, 1, 0}, {3, 0, 0, 2400, 1, 1, 0}, {4, 0, 0, 2400, 1, 1, 0},
    {5, 0, 0, 2400, 1, 1, 0}, {6, 0, 0, 2400, 1, 1, 0}, {7, 0, 0, 2400, 1, 1, 0}, {8, 0, 0, 2400, 1, 1, 0}, {9, 0, 0, 2400, 1, 1, 0}, {10, 0, 0, 2400, 1, 1, 0},
    {11, 0, 0, 2400, 1, 1, 0}, {12, 0, 0, 2400, 1, 1, 0}, {13, 0, 0, 2400, 1, 1, 0}, {14, 0, 0, 2400, 1, 1, 0}, {15, 0, 0, 2400, 1, 1, 0},
    {16, 0, 0, 2400, 1, 1, 0}, {17, 0, 0, 2400, 1, 1, 0}, {18, 0, 0, 2400, 1, 1, 0}, {19, 0, 0, 2400, 1, 1, 0}, {20, 0, 0, 2400, 1, 1, 0},
    {21, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQF39[] = {{40, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQF00[] = {{1, 0, 0, 2400, 0, 2, 0}, {2, 0, 0, 2400, 0, 2, 0}, {3, 0, 0, 2400, 0, 2, 0}, {4, 0, 0, 2400, 0, 2, 0},
    {5, 0, 0, 2400, 0, 2, 0}, {6, 0, 0, 2400, 0, 2, 0}, {7, 0, 0, 2400, 0, 2, 0}, {8, 0, 0, 2400, 0, 2, 0}, {9, 0, 0, 2400, 0, 2, 0}, {10, 0, 0, 2400, 0, 2, 0},
    {11, 0, 0, 2400, 0, 2, 0}, {12, 0, 0, 2400, 0, 2, 0}, {13, 0, 0, 2400, 0, 2, 0}, {14, 0, 0, 2400, 0, 2, 0}, {15, 0, 0, 2400, 0, 2, 0},
    {16, 0, 0, 2400, 0, 2, 0}, {17, 0, 0, 2400, 0, 2, 0}, {18, 0, 0, 2400, 0, 2, 0}, {19, 0, 0, 2400, 0, 2, 0}, {20, 0, 0, 2400, 0, 2, 0},
    {21, 0, 0, 2400, 0, 2, 0}, {22, 0, 0, 2400, 0, 2, 0}, {23, 0, 0, 2400, 0, 2, 0}, {24, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQF01[] = {{1, 0, 0, 2400, 0, 2, 0}, {2, 0, 0, 2400, 0, 2, 0}, {3, 0, 0, 2400, 0, 2, 0}, {4, 0, 0, 2400, 0, 2, 0},
    {5, 0, 0, 2400, 0, 2, 0}, {6, 0, 0, 2400, 0, 2, 0}, {7, 0, 0, 2400, 0, 2, 0}, {8, 0, 0, 2400, 0, 2, 0}, {9, 0, 0, 2400, 0, 2, 0}, {10, 0, 0, 2400, 0, 2, 0},
    {11, 0, 0, 2400, 0, 2, 0}, {12, 0, 0, 2400, 0, 2, 0}, {13, 0, 0, 2400, 0, 2, 0}, {14, 0, 0, 2400, 0, 2, 0}, {15, 0, 0, 2400, 0, 2, 0},
    {16, 0, 0, 2400, 0, 2, 0}, {17, 0, 0, 2400, 0, 2, 0}, {18, 0, 0, 2400, 0, 2, 0}, {19, 0, 0, 2400, 0, 2, 0}, {20, 0, 0, 2400, 0, 2, 0},
    {21, 0, 0, 2400, 0, 2, 0}, {22, 0, 0, 2400, 0, 2, 0}, {23, 0, 0, 2400, 0, 2, 0}, {24, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataQF02[] = {{1, 0, 0, 2400, 0, 2, 0}, {2, 0, 0, 2400, 0, 2, 0}, {3, 0, 0, 2400, 0, 2, 0}, {4, 0, 0, 2400, 0, 2, 0},
    {5, 0, 0, 2400, 0, 2, 0}, {6, 0, 0, 2400, 0, 2, 0}, {7, 0, 0, 2400, 0, 2, 0}, {8, 0, 0, 2400, 0, 2, 0}, {9, 0, 0, 2400, 0, 2, 0}, {10, 0, 0, 2400, 0, 2, 0},
    {11, 0, 0, 2400, 0, 2, 0}, {12, 0, 0, 2400, 0, 2, 0}, {13, 0, 0, 2400, 0, 2, 0}, {14, 0, 0, 2400, 0, 2, 0}, {15, 0, 0, 2400, 0, 2, 0},
    {16, 0, 0, 2400, 0, 2, 0}, {17, 0, 0, 2400, 0, 2, 0}, {18, 0, 0, 2400, 0, 2, 0}, {19, 0, 0, 2400, 0, 2, 0}, {20, 0, 0, 2400, 0, 2, 0},
    {21, 0, 0, 2400, 0, 2, 0}, {22, 0, 0, 2400, 0, 2, 0}, {23, 0, 0, 2400, 0, 2, 0}, {24, 0, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAK00[] = {{1, 1, 0, 2400, 0, 2, 0}, {5, 0, 1100, 2200, 0, 3, 0}, {18, 0, 1300, 200, 0, 3, 0}, {10, 1, 0, 2400, 0, 2, 0},
    {9, 1, 0, 2400, 0, 2, 0}, {12, 1, 0, 2400, 0, 2, 0}, {13, 1, 0, 2400, 0, 2, 0}, {7, 0, 600, 2200, 1, 1, 0}, {14, 1, 0, 2400, 0, 2, 0},
    {22, 1, 0, 2400, 0, 2, 0}, {19, 1, 600, 1900, 0, 2, 0}, {20, 1, 600, 1900, 0, 2, 0}, {21, 1, 600, 1900, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAKY0[] = {{2, 1, 0, 2400, 0, 2, 0}, {24, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAKT0[] = {{11, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAKT1[] = {{15, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAKT2[] = {{16, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAKT3[] = {{17, 1, 0, 2400, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAR02[] = {{23, 0, 0, 2400, 0, 2, 0}, {24, 1, 0, 2400, 0, 2, 0}, {1, 0, 600, 2400, 0, 3, 0}, {20, 1, 1100, 200, 0, 3, 0},
    {21, 0, 1100, 200, 0, 3, 0}, {2, 1, 1000, 2400, 0, 3, 0}, {3, 0, 1000, 2400, 0, 3, 0}, {4, 1, 900, 2000, 0, 3, 0}, {5, 0, 900, 2000, 0, 3, 0},
    {12, 0, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAR03[] = {{13, 0, 0, 0, 0, 1, 0}, {14, 0, 0, 0, 0, 1, 0}, {23, 0, 0, 2400, 0, 2, 0}, {24, 1, 0, 2400, 0, 2, 0},
    {8, 1, 500, 1900, 0, 3, 0}, {9, 0, 500, 1900, 0, 3, 0}, {10, 1, 0, 2400, 0, 3, 0}, {11, 0, 0, 2400, 0, 3, 0},
    {6, 1, 800, 2400, 0, 3, reinterpret_cast<std::uint32_t>(doorFlagRulesAR03_06)}, {15, 0, 0, 0, 0, 1, 0}, {16, 1, 0, 0, 0, 1, 0}, {17, 1, 0, 0, 0, 1, 0},
    {18, 1, 0, 0, 0, 1, 0}, {19, 1, 0, 0, 0, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataAB00[] = {{2, 0, 0, 2400, 0, 2, 0}, {3, 1, 0, 2400, 0, 2, 0}, {1, 1, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWS00[] = {{6, 1, 800, 2200, 0, 3, 0}, {10, 1, 900, 2400, 0, 3, 0}, {11, 0, 900, 2400, 0, 3, 0}, {14, 1, 0, 2400, 0, 3, 0},
    {15, 0, 0, 2400, 1, 1, 0}, {25, 1, 0, 2400, 0, 3, 0}, {41, 0, 0, 2400, 0, 3, 0}, {27, 1, 1100, 2400, 0, 3, 0}, {42, 0, 1100, 2400, 0, 3, 0},
    {33, 1, 1000, 2100, 0, 3, 0}, {34, 0, 1000, 2100, 0, 3, 0}, {35, 1, 1700, 200, 0, 3, 0}, {37, 1, 1700, 200, 0, 3, 0}, {44, 0, 1700, 200, 0, 3, 0},
    {39, 1, 900, 2200, 0, 3, 0}, {45, 0, 900, 2200, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWSY0[] = {{16, 1, 0, 2400, 1, 1, 0}, {17, 1, 0, 2400, 1, 1, 0}, {18, 1, 0, 2400, 1, 1, 0}, {19, 1, 0, 2400, 1, 1, 0},
    {20, 1, 0, 2400, 1, 1, 0}, {21, 1, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWK00[] = {{1, 1, 0, 2400, 0, 3, 0}, {19, 1, 900, 2300, 0, 3, 0}, {23, 1, 800, 2000, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWK01[] = {{2, 1, 0, 2400, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWKA0[] = {{3, 1, 0, 2400, 1, 1, 0}, {4, 1, 0, 2400, 1, 1, 0}, {5, 1, 0, 2400, 1, 1, 0}, {6, 1, 0, 2400, 1, 1, 0},
    {7, 1, 0, 2400, 1, 1, 0}, {8, 1, 0, 2400, 1, 1, 0}, {9, 1, 0, 2400, 1, 1, 0}, {10, 1, 0, 2400, 1, 1, 0}, {11, 0, 0, 2400, 1, 1, 0},
    {12, 1, 0, 2400, 1, 1, 0}, {13, 1, 0, 2400, 1, 1, 0}, {14, 1, 0, 2400, 1, 1, 0}, {15, 1, 0, 2400, 1, 1, 0}, {16, 1, 0, 2400, 1, 1, 0},
    {17, 1, 0, 2400, 1, 1, 0}, {18, 1, 0, 2400, 1, 1, 0}, {28, 1, 0, 2400, 1, 1, 0}, {29, 1, 0, 2400, 1, 1, 0}, {30, 1, 0, 2400, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWE00[] = {{6, 0, 1100, 2100, 0, 3, 0}, {5, 1, 1100, 2100, 0, 3, 0}, {30, 1, 0, 2400, 0, 3, 0}, {9, 1, 1000, 2000, 0, 3, 0},
    {10, 0, 1000, 2000, 0, 3, 0}, {11, 1, 900, 2200, 0, 3, 0}, {12, 0, 900, 2200, 0, 3, 0}, {15, 1, 0, 2400, 0, 3, 0}, {19, 1, 1000, 2300, 0, 3, 0},
    {21, 1, 1000, 2000, 0, 3, 0}, {22, 0, 1000, 2000, 0, 3, 0}, {37, 0, 1100, 2000, 0, 3, 0}, {36, 1, 1100, 2000, 0, 3, 0}, {25, 1, 0, 2400, 0, 3, 0},
    {38, 1, 1800, 300, 0, 3, 0}, {39, 0, 1800, 300, 0, 3, 0}, {40, 1, 800, 2000, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWEG0[] = {{32, 1, 0, 2400, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWEG1[] = {{34, 1, 0, 2400, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWN00[] = {{38, 0, 700, 2100, 0, 3, 0}, {39, 1, 700, 2100, 0, 3, 0}, {42, 0, 700, 2200, 0, 3, 0},
    {43, 1, 700, 2200, 0, 3, 0}, {46, 0, 700, 2000, 0, 3, 0}, {47, 1, 700, 2000, 0, 3, 0}, {2, 1, 1000, 2200, 0, 3, 0}, {6, 1, 1000, 2200, 0, 3, 0},
    {10, 1, 1000, 2200, 0, 3, 0}, {14, 0, 1000, 2200, 0, 3, 0}, {50, 0, 700, 2100, 0, 3, 0}, {51, 1, 700, 2100, 0, 3, 0}, {54, 0, 700, 2300, 0, 3, 0},
    {55, 1, 700, 2300, 0, 3, 0}, {58, 0, 700, 2000, 0, 3, 0}, {60, 0, 700, 2100, 0, 3, 0}, {18, 0, 700, 2300, 0, 3, 0}, {70, 0, 900, 2300, 0, 3, 0},
    {20, 1, 1000, 2200, 0, 3, 0}, {24, 0, 900, 2200, 0, 3, 0}, {27, 0, 0, 2400, 0, 3, 0}, {29, 0, 800, 2400, 0, 3, 0}, {31, 0, 1100, 2400, 0, 3, 0},
    {62, 0, 700, 2000, 0, 3, 0}, {63, 1, 700, 2000, 0, 3, 0}, {66, 0, 700, 2200, 0, 3, 0}, {67, 1, 700, 2200, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWT00[] = {{35, 0, 1000, 2200, 0, 3, 0}, {36, 0, 1000, 2200, 0, 3, 0}, {37, 0, 1000, 2200, 0, 3, 0}, {38, 0, 0, 0, 0, 3, 0},
    {39, 0, 900, 2200, 0, 3, 0}, {1, 0, 0, 2400, 0, 3, 0}, {2, 1, 0, 2400, 0, 3, 0}, {3, 0, 0, 2400, 0, 3, 0}, {4, 1, 0, 2400, 0, 3, 0},
    {40, 0, 900, 2200, 0, 3, 0}, {41, 0, 900, 2200, 0, 3, 0}, {42, 0, 900, 2200, 0, 3, 0}, {43, 0, 800, 2000, 0, 3, 0}, {44, 0, 1800, 2400, 0, 3, 0},
    {45, 0, 900, 1900, 0, 3, 0}, {46, 0, 1000, 1800, 0, 3, 0}, {47, 0, 1100, 2100, 0, 3, 0}, {48, 0, 1100, 2100, 0, 3, 0}, {49, 0, 900, 2200, 0, 3, 0},
    {50, 0, 800, 2000, 0, 3, 0}, {9, 1, 1100, 2400, 0, 3, 0}, {51, 0, 900, 2300, 0, 3, 0}, {13, 0, 0, 0, 0, 2, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWTA0[] = {{20, 0, 0, 2400, 1, 1, 0}, {21, 0, 0, 2400, 1, 1, 0}, {22, 0, 0, 2400, 1, 1, 0}, {23, 0, 0, 2400, 1, 1, 0},
    {24, 0, 0, 2400, 1, 1, 0}, {25, 0, 0, 2400, 1, 1, 0}, {26, 0, 0, 2400, 1, 1, 0}, {27, 0, 0, 2400, 1, 1, 0}, {28, 0, 0, 2400, 1, 1, 0},
    {29, 0, 0, 2400, 1, 1, 0}, {30, 0, 0, 2400, 1, 1, 0}, {31, 0, 0, 2400, 1, 1, 0}, {32, 0, 0, 2400, 1, 1, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWB00[] = {{7, 0, 700, 1900, 0, 3, 0}, {8, 1, 700, 1900, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};
static DoorAreaMetadata doorMetadataWR00[] = {{2, 1, 1200, 2400, 0, 3, 0}, {3, 1, 1800, 200, 0, 3, 0}, {4, 1, 1800, 200, 0, 3, 0}, {0, 0, 0, 0, 0, 0, 0}};

// @verified - d
DoorAreaMetadata *GetDoorAreaMetadataList() {
    DoorAreaMetadata *metadata = 0;
    switch (g_currentMap) {
    case SHEN_NAME4('O', 'R', '0', '0'):
        metadata = doorMetadataOR00;
        break;
    case SHEN_NAME4('Q', '1', '0', '0'):
        metadata = doorMetadataQ100;
        break;
    case SHEN_NAME4('Q', '3', '0', '0'):
        metadata = doorMetadataQ300;
        break;
    case SHEN_NAME4('Q', 'A', 'X', 'X'):
        metadata = doorMetadataQAXX;
        break;
    case SHEN_NAME4('Q', 'A', '0', '0'):
        metadata = doorMetadataQA00;
        break;
    case SHEN_NAME4('Q', 'A', 'W', '1'):
        metadata = doorMetadataQAW1;
        break;
    case SHEN_NAME4('Q', 'A', 'W', '6'):
        metadata = doorMetadataQAW6;
        break;
    case SHEN_NAME4('Q', 'A', 'E', '1'):
        metadata = doorMetadataQAE1;
        break;
    case SHEN_NAME4('Q', 'B', '0', '0'):
        metadata = doorMetadataQB00;
        break;
    case SHEN_NAME4('Q', 'C', '0', '0'):
        metadata = doorMetadataQC00;
        break;
    case SHEN_NAME4('Q', 'C', '0', '1'):
        metadata = doorMetadataQC01;
        break;
    case SHEN_NAME4('Q', 'C', '0', '6'):
        metadata = doorMetadataQC06;
        break;
    case SHEN_NAME4('Q', 'C', 'A', 'E'):
        metadata = doorMetadataQCAE;
        break;
    case SHEN_NAME4('Q', 'D', '0', '0'):
        metadata = doorMetadataQD00;
        break;
    case SHEN_NAME4('Q', 'D', '0', '1'):
        metadata = doorMetadataQD01;
        break;
    case SHEN_NAME4('Q', 'D', '0', '6'):
        metadata = doorMetadataQD06;
        break;
    case SHEN_NAME4('Q', 'E', '0', '0'):
        metadata = doorMetadataQE00;
        break;
    case SHEN_NAME4('Q', 'E', '0', '1'):
        metadata = doorMetadataQE01;
        break;
    case SHEN_NAME4('Q', 'E', '0', '6'):
        metadata = doorMetadataQE06;
        break;
    case SHEN_NAME4('Q', 'F', '3', '9'):
        metadata = doorMetadataQF39;
        break;
    case SHEN_NAME4('Q', 'F', '0', '0'):
        metadata = doorMetadataQF00;
        break;
    case SHEN_NAME4('Q', 'F', '0', '1'):
        metadata = doorMetadataQF01;
        break;
    case SHEN_NAME4('Q', 'F', '0', '2'):
        metadata = doorMetadataQF02;
        break;
    case SHEN_NAME4('A', 'K', '0', '0'):
        metadata = doorMetadataAK00;
        break;
    case SHEN_NAME4('A', 'K', 'Y', '0'):
        metadata = doorMetadataAKY0;
        break;
    case SHEN_NAME4('A', 'K', 'T', '0'):
        metadata = doorMetadataAKT0;
        break;
    case SHEN_NAME4('A', 'K', 'T', '1'):
        metadata = doorMetadataAKT1;
        break;
    case SHEN_NAME4('A', 'K', 'T', '2'):
        metadata = doorMetadataAKT2;
        break;
    case SHEN_NAME4('A', 'K', 'T', '3'):
        metadata = doorMetadataAKT3;
        break;
    case SHEN_NAME4('A', 'R', '0', '2'):
        metadata = doorMetadataAR02;
        break;
    case SHEN_NAME4('A', 'R', '0', '3'):
        metadata = doorMetadataAR03;
        break;
    case SHEN_NAME4('A', 'B', '0', '0'):
        metadata = doorMetadataAB00;
        break;
    case SHEN_NAME4('W', 'S', '0', '0'):
        metadata = doorMetadataWS00;
        break;
    case SHEN_NAME4('W', 'S', 'Y', '0'):
        metadata = doorMetadataWSY0;
        break;
    case SHEN_NAME4('W', 'K', '0', '0'):
        metadata = doorMetadataWK00;
        break;
    case SHEN_NAME4('W', 'K', '0', '1'):
        metadata = doorMetadataWK01;
        break;
    case SHEN_NAME4('W', 'K', 'A', '0'):
        metadata = doorMetadataWKA0;
        break;
    case SHEN_NAME4('W', 'E', '0', '0'):
        metadata = doorMetadataWE00;
        break;
    case SHEN_NAME4('W', 'E', 'G', '0'):
        metadata = doorMetadataWEG0;
        break;
    case SHEN_NAME4('W', 'E', 'G', '1'):
        metadata = doorMetadataWEG1;
        break;
    case SHEN_NAME4('W', 'N', '0', '0'):
        metadata = doorMetadataWN00;
        break;
    case SHEN_NAME4('W', 'T', '0', '0'):
        metadata = doorMetadataWT00;
        break;
    case SHEN_NAME4('W', 'T', 'A', '0'):
        metadata = doorMetadataWTA0;
        break;
    case SHEN_NAME4('W', 'B', '0', '0'):
        metadata = doorMetadataWB00;
        break;
    case SHEN_NAME4('W', 'R', '0', '0'):
        metadata = doorMetadataWR00;
        break;
    }
    return metadata;
}

} // namespace s2
} // namespace shenmue

#endif
