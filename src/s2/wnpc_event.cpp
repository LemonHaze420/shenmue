#include "s2/wnpc_event.hpp"
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

// @verified - p
void WNPCMain(std::uintptr_t) {
    SetPlayerControlEnabled(0);
    HLib::EnqueueTaskWithoutParameter(WNPCWaitNpc, 4, 0x0b, static_cast<std::int32_t>(WNPC_TASK.name));
    EV_debug_log("WNPC\\eWNPC_et_wait_npc.c", 35, "NpcWaitStart\n");
}

// @verified - p
void WNPCWaitNpc(void *) {
    if (!NpcWait()) {
        EV_debug_log("WNPC\\eWNPC_et_wait_npc.c", 46, "NpcWaitEnd\n");
        SetPlayerControlEnabled(1);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
        HLib::DestroyCurrentTask();
    }
}

const ScriptAreaDefinition wnpcScriptDefinition{
    .areaId = WNPC_AREA,
    .field04 = 0,
    .init = &WNPCInit,
    .main = &WNPCMain,
};

} // namespace shenmue::s2
