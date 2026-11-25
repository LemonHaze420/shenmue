#pragma once

#include "s2/search_state_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

struct SearchPropIKStage {
    Vector3 position;
    std::int32_t rotation[3];
    float targetOffset;
    std::int16_t ticks;
    std::int8_t jointTable, axis;
};
struct SearchPropIKConfig {
    std::int8_t hand;
    std::uint8_t pose, retained02[2];
    std::uint32_t flags;
    struct {
        std::int32_t pitch, yaw;
    } angles[2];
    Vector3 endpoints[2], gripOffset;
    SearchPropIKStage stages[4];
};
typedef char Layout_search_prop_stage[(sizeof(SearchPropIKStage) == 0x20) ? 1 : -1];
typedef char Layout_search_prop_config[(sizeof(SearchPropIKConfig) == 0xbc) ? 1 : -1];
typedef char Layout_search_prop_stages[(offsetof(SearchPropIKConfig, stages) == 0x3c) ? 1 : -1];
struct SearchPropIKSlot {
    shen_name name;
    std::int32_t lookupIndex;
    std::uint32_t flags;
    std::uint32_t active;
    MT7Node *node;
    MT7Node *parent;
    SearchPropIKConfig *configuration;
    Matrix4x4 *transformData;
    Matrix4x4 *auxiliaryData;
    std::uint32_t retained24;
    void *resource;
    void *owner;
    std::byte *savedCursor;
    std::int32_t leftSide;
    std::int32_t phase;
};
typedef char Layout_search_prop_ik_dc_0[(sizeof(SearchPropIKSlot) == 0x3c) ? 1 : -1];
typedef char Layout_search_prop_ik_dc_1[(offsetof(SearchPropIKSlot, node) == 0x10) ? 1 : -1];
typedef char Layout_search_prop_ik_dc_2[(offsetof(SearchPropIKSlot, configuration) == 0x18) ? 1 : -1];
typedef char Layout_search_prop_ik_dc_3[(offsetof(SearchPropIKSlot, transformData) == 0x1c) ? 1 : -1];
typedef char Layout_search_prop_ik_dc_4[(offsetof(SearchPropIKSlot, auxiliaryData) == 0x20) ? 1 : -1];
typedef char Layout_search_prop_ik_dc_5[(offsetof(SearchPropIKSlot, savedCursor) == 0x30) ? 1 : -1];

static const std::int32_t searchPropIKLookup[15] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 11, 12, 13, 14, 15};
extern SearchPropIKSlot g_searchPropIKSlots[4];
extern Matrix4x4 searchPropIKTransformData[4][6], searchPropIKAuxiliaryData[4][4];
extern std::int32_t searchPropIKIndex;
extern std::int16_t searchPropIKRotationVelocity[2];
void srchClearPropIKSlot(std::int32_t index);
void srchResetIK();
std::int32_t srchGetPropIKIndex();
#if SHENMUE_IS_DC && SHENMUE_IS_S2
std::int32_t srchGetPropIKLookup(std::uint32_t marker);
#else
std::int32_t srchGetPropIKLookup(std::uint16_t marker);
#endif
bool srchPushPropIK(SearchTaskData &search);
bool srchPopPropIK(SearchTaskData &search);

} // namespace s2
} // namespace shenmue

#endif
