#include "s2/wnpc_event.hpp"
#include "s2/events/event_api.h"

#if !defined(SHENMUE_EVENT_OVERLAY)
#include "s2/debug.hpp"
#include "s2/flag.hpp"
#include "s2/hlib.hpp"
#include "s2/npc_package.hpp"
#include "s2/player.hpp"
#include "s2/screen_film.hpp"
namespace shenmue::s2 {

// @verified - p
void WNPCInit() {
}
#endif

// @verified - pd
#if defined(SHENMUE_EVENT_OVERLAY)
void WNPCMain(void)
#else
void WNPCMain(std::uintptr_t)
#endif
{
    FTBL.SetPlayerControlEnabled(0);
#if defined(SHENMUE_EVENT_OVERLAY)
    FTBL.HLib_EnqueueTaskWithoutParameter(WNPCWaitNpc, 4, 0x0b, WNPC_TASK);
    FTBL.DebugLogToFileRemoved("NpcWaitStart\n");
#else
    FTBL.HLib_EnqueueTaskWithoutParameter(WNPCWaitNpc, 4, 0x0b, static_cast<std::int32_t>(WNPC_TASK.name));
    EV_debug_log("WNPC\\eWNPC_et_wait_npc.c", 35, "NpcWaitStart\n");
#endif
}

// @verified - pd
void WNPCWaitNpc(void *taskData) {
    if (!FTBL.NpcWait()) {
#if defined(SHENMUE_EVENT_OVERLAY)
        FTBL.DebugLogToFileRemoved("NpcWaitEnd\n");
#else
        EV_debug_log("WNPC\\eWNPC_et_wait_npc.c", 46, "NpcWaitEnd\n");
#endif
        FTBL.SetPlayerControlEnabled(1);
        FTBL.SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        FTBL.HLib_DestroyCurrentTask();
    }
}

#if !defined(SHENMUE_EVENT_OVERLAY)
const ScriptAreaDefinition wnpcScriptDefinition{
    .areaId = WNPC_AREA,
    .field04 = 0,
    .init = &WNPCInit,
    .main = &WNPCMain,
};
} // namespace shenmue::s2
#endif
