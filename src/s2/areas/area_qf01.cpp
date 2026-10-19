#include "s2/areas/area_qf01.hpp"
#include "s2/areas/area_qf06.hpp"
#include "s2/event_runtime.hpp"
#include "s2/fog.hpp"
#include "s2/hlib.hpp"
#include "s2/lighting.hpp"
#include "s2/map_control.hpp"

namespace shenmue::s2::areas {
namespace {
inline constexpr shen_name QF01_EVENT_TASK{SHEN_NAME(EVNT)};
void QF01MainloopNullsub(void *) {
}
} // namespace

void AreaQF01Init() {
}

void AreaQF01Main(std::uint32_t) {
    SetMapControlStatusFlag(0, 0);
    (void)EV_GetShenmueFlag(0xff);
    EV_SetWeatherInstruction(0x0f, 0);
    EV_SetWeatherInstruction(0x780, 0);
    EV_SetWeatherInstruction(0x1800, 0);
    EV_SetModelDataDefaultVertexShader();
    EV_lightLoad(LIGHT_DEFAULT_DEFINITION);
    PushGlobalLightingProgram();
    (void)CheckIsArgEquZero(1);
    SetAreaFogParams();
    HLib::EnqueueTaskWithoutParameter(QF01MainloopNullsub, 4, 0x0b, static_cast<std::int32_t>(QF01_EVENT_TASK.name));
    QueueMagicRoom("scene/03/stream", "ROOM.afs", QF01_MAGIC_ROOM_FLOORS, 61952);
}

const GameplayAreaDefinition areaQF01GameplayDefinition{
    .areaId = SHEN_NAME(QF01),
    .padding04 = 0,
    .init = &AreaQF01Init,
    .main = &AreaQF01Main,
    .cleanup = &AreaQF01Cleanup,
};

} // namespace shenmue::s2::areas
