#include "s2/startup_dc.hpp"

#if SHENMUE_IS_DC && SHENMUE_IS_S2

namespace shenmue {
namespace s2 {

shen_name g_currentTaskName;

} // namespace s2
} // namespace shenmue

// @verified - d
extern "C" int shenmue_dc_entry() {
    shenmue::s2::StartupNullsub();
    shenmue::s2::InitializeMemoryAndHLib();
    shenmue::s2::ExecuteTasks();
    return 0;
}

#endif
