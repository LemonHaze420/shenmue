#pragma once

#include "shenmue.h"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

#include "s2/hlib.hpp"

namespace shenmue { namespace s2 {

struct ScenarioWindowRecord {
    HLib::Task *task;
    std::uint32_t retained04, retained08;
};
typedef char Layout_scnw_dc_0[(sizeof(ScenarioWindowRecord) == 0x0c) ? 1 : -1];
typedef char Layout_scnw_dc_1[(offsetof(ScenarioWindowRecord, retained04) == 4) ? 1 : -1];
typedef char Layout_scnw_dc_2[(offsetof(ScenarioWindowRecord, retained08) == 8) ? 1 : -1];
#define SCENARIO_WINDOW_STORAGE SHEN_NAME4('S', 'C', 'N', 'W')

namespace event_runtime_detail {
extern ScenarioWindowRecord *scenarioWindowRecord;
extern ScenarioWindowRecord **g_pSCNW;
} // namespace event_runtime_detail

ScenarioWindowRecord *AllocateSCNW(HLib::Task *task);

} } // namespace shenmue::s2

#endif
