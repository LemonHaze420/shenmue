#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "common/types.hpp"

#define SHENMUE_STARTUP_DATA_LAYOUT_JOIN_(a, b) a##b
#define SHENMUE_STARTUP_DATA_LAYOUT_JOIN(a, b) SHENMUE_STARTUP_DATA_LAYOUT_JOIN_(a, b)
#define SHENMUE_STARTUP_DATA_LAYOUT(condition) typedef char SHENMUE_STARTUP_DATA_LAYOUT_JOIN(StartupDataLayout_, __LINE__)[(condition) ? 1 : -1]
namespace shenmue {
namespace s2 {

struct MapEventTransition;
struct AreaTableEntry {
    shen_name area;
    const MapEventTransition *transitions;
    std::uint32_t startupMode;
    char startupPayload[16];
};
SHENMUE_STARTUP_DATA_LAYOUT(sizeof(AreaTableEntry) == 0x1c);
SHENMUE_STARTUP_DATA_LAYOUT(offsetof(AreaTableEntry, transitions) == 4);
SHENMUE_STARTUP_DATA_LAYOUT(offsetof(AreaTableEntry, startupMode) == 8);
SHENMUE_STARTUP_DATA_LAYOUT(offsetof(AreaTableEntry, startupPayload) == 0x0c);
inline AreaTableEntry InitialAreaTableEntry() {
    AreaTableEntry entry;
    entry.area = UINT32_MAX;
    entry.transitions = 0;
    entry.startupMode = 0;
    for (std::size_t index = 0; index < sizeof(entry.startupPayload); ++index)
        entry.startupPayload[index] = 0;
    return entry;
}
using ::shenmue::shen_name;
#define initialAreaTableEntry InitialAreaTableEntry()
#define DISC_NUMBER_STORAGE shen_name(SHEN_NAME4('D', 'I', 'S', 'C'))
#define AREA_TABLE_STORAGE shen_name(SHEN_NAME4('A', 'R', 'E', 'A'))
#define RENDER_PROFILE_STORAGE shen_name(SHEN_NAME4('S', 'Y', 'S', 'P'))
#define DOOR_TABLE_STORAGE shen_name(SHEN_NAME4('D', 'O', 'O', 'R'))
extern std::int32_t g_loadedActNumber;
extern AreaTableEntry *g_areaTableEntries;
extern std::byte *g_doorTableData;
extern std::byte *g_doorData;
extern std::byte *g_doorTableCursor;
extern std::byte *g_doorDataCursor;

void ReadDiscNumber();
void LoadAreaTable();
#if defined(SHENMUE_TARGET_S2_DC_JP_GAMEJAM)
AreaTableEntry *GetAreaTableEntry(shen_name area);
#endif
void LoadDoorTable();

} // namespace s2
} // namespace shenmue
#undef SHENMUE_STARTUP_DATA_LAYOUT_JOIN_
#undef SHENMUE_STARTUP_DATA_LAYOUT_JOIN
#undef SHENMUE_STARTUP_DATA_LAYOUT

#endif
