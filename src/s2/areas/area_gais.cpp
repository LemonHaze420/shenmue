#include "s2/areas/area_gais.hpp"
#include "s2/areas/area_0547.hpp"
#include "s2/event_characters.hpp"
#include "s2/event_queued.hpp"
#include "s2/event_runtime.hpp"
#include "s2/hlib.hpp"
#include "s2/motion.hpp"
#include "s2/npc.hpp"
#include "s2/npc_package.hpp"
#include "s2/screen_film.hpp"

namespace shenmue::s2::areas {

// @verified - p
void AreaGAISInit() {
}

void GAISStopperCallback(void *) {
    if (EV_ForceLoadNpc(GAIS_STOPPER)) {
        InitiateCleanupForSceneChara(GAIS_STOPPER);
        EV_npcLockUnlock(GAIS_STOPPER);
        EventSetPosition position{.cleanup = 0,
            .character = GAIS_PLAYER,
            .parent = EVENT_CHARACTER_INVALID,
            .position = {121.30000305175781f, 92.80000305175781f, 184.89999389648438f},
            .delay = 0.0f};
        EVT_AT_SetPosRot(&position);
        EventSetAngle angle{.cleanup = 0, .character = GAIS_PLAYER, .parent = EVENT_CHARACTER_INVALID, .degrees = 292.5f, .delay = 0.0f};
        EVT_AT_SetAngY_Callback(&angle);
        EventSetMotion motion{.cleanup = 0, .character = GAIS_PLAYER, .animation = 0xf003, .frame = 0.0f, .speed = 1.0f, .flags = 0x40000000U, .delay = 0.0f};
        EVT_AT_SetMot_Callback(&motion);
        (void)GetMotionFrameCount(0xf003);
        HLib::SetCurrentTaskTo(Update0547Wake);
        HLib::SetupTaskDelay(30);
        SetScreenFilmFade(30, 255, 0, 0, 0, 0, 0, 0, 0);
    }
}

// @verified - p
void AreaGAISMain(std::uintptr_t) {
    HLib::EnqueueTaskWithoutParameter(GAISStopperCallback, 4, 0x0d, static_cast<std::int32_t>(GAIS_TASK.name));
    EV_CheckWarmStartNpcRoute(GAIS_STOPPER);
}

const ScriptAreaDefinition areaGAISScriptDefinition{.areaId = AREA_GAIS, .field04 = 0, .init = AreaGAISInit, .main = AreaGAISMain};

} // namespace shenmue::s2::areas
